#include "MyToolLibFunc.h"
#include "FJCAApiConst.h"
#include "MySMData.h"
#include "MySMLibFunc.h"
//#include "MySocket.h"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>

#include <openssl/asn1t.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/sha.h>
#include <openssl/ts.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/timeb.h>

#ifdef _WIN32
#include <atlbase.h>
#include <locale.h>
#else
#include <dirent.h>
#include <fcntl.h>
#include <iconv.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "MySocket.h"
#endif

#include <vector>
#include <string>

typedef std::string					String;
typedef std::vector<char>			CharArray;
typedef std::vector<unsigned char>	ByteArray;
typedef std::vector<String>			StringArray;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD g_dwMyToolsReference = 0;

////////////////////////////////////////////////////////////////
// 国密SM2数据结构

// SM2公钥
IMPLEMENT_ASN1_TYPE_ex(SM2_PUBLICKEY, ASN1_BIT_STRING, 0)
IMPLEMENT_ASN1_FUNCTIONS(SM2_PUBLICKEY)

// SM2私钥
IMPLEMENT_ASN1_TYPE_ex(SM2_PRIVATEKEY, ASN1_INTEGER, 0)
IMPLEMENT_ASN1_FUNCTIONS(SM2_PRIVATEKEY)

// SM2签名
ASN1_SEQUENCE(SM2_SIGNATURE) = {
	ASN1_SIMPLE(SM2_SIGNATURE, r, ASN1_INTEGER),
	ASN1_SIMPLE(SM2_SIGNATURE, s, ASN1_INTEGER),
} ASN1_SEQUENCE_END(SM2_SIGNATURE)

IMPLEMENT_ASN1_FUNCTIONS(SM2_SIGNATURE)

// SM2密文
ASN1_SEQUENCE(SM2_CIPHER) = {
	ASN1_SIMPLE(SM2_CIPHER, x, ASN1_INTEGER),
	ASN1_SIMPLE(SM2_CIPHER, y, ASN1_INTEGER),
	ASN1_SIMPLE(SM2_CIPHER, hash, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM2_CIPHER, cipherText, ASN1_OCTET_STRING),
} ASN1_SEQUENCE_END(SM2_CIPHER)

IMPLEMENT_ASN1_FUNCTIONS(SM2_CIPHER)

ASN1_INTEGER *bin_to_ASN1_INTEGER(unsigned char *in, int inlen)
{
	if (inlen != SGD_SIZE_ECC && inlen != SGD_SIZE_ECC_64)
		return NULL;

	int nDataLen = 2 + inlen;
	if (in[0] >= 0x80)
		nDataLen = 3 + inlen;

	unsigned char byData[256];
	memset(byData, 0, sizeof(byData));
	byData[0] = 0x02;
	byData[1] = (unsigned char)nDataLen - 2;
	if (in[0] >= 0x80)
	{
		byData[2] = 0x00;
		memcpy(byData + 3, in, inlen);
	}
	else
	{
		memcpy(byData + 2, in, inlen);
	}

	unsigned char *pbTmp = byData;
	ASN1_INTEGER *out = d2i_ASN1_INTEGER(NULL, (const unsigned char **)&pbTmp, nDataLen);
	return out;
}

int ASN1_INTEGER_to_bin(ASN1_INTEGER *in, unsigned char *out)
{
	unsigned char byData[256];
	memset(byData, 0, sizeof(byData));
	unsigned char *pbTmp = byData;
	int nDataLen = i2d_ASN1_INTEGER(in, &pbTmp);
	if (nDataLen <= 0)
		return 0;

	int nOffset = 2;
	if (byData[2] == 0)
		nOffset++;
	nDataLen -= nOffset;
	if (nDataLen == 0 || nDataLen > SGD_SIZE_ECC_64)
		return 0;

	int outlen = SGD_SIZE_ECC;
	if (nDataLen > SGD_SIZE_ECC)
		outlen = SGD_SIZE_ECC_64;
	memcpy(out + outlen - nDataLen, byData + nOffset, nDataLen);

	return outlen;
}

// public key
long MYTOOL_SM2PublicKeyEncode(unsigned char *in_x, long inlen_x, unsigned char *in_y, long inlen_y, unsigned char *out, long *outlen)
{
	if ((inlen_x != inlen_y) ||
		(inlen_x != SGD_SIZE_ECC && inlen_x != SGD_SIZE_ECC_64))
		return SAR_ECCDATAErr;

	unsigned char byData[256];
	memset(byData, 0, sizeof(byData));
	int nDataLen = 0;
	memcpy(byData, "\x04", 1);
	nDataLen += 1;
	memcpy(byData + nDataLen, in_x, inlen_x);
	nDataLen += inlen_x;
	memcpy(byData + nDataLen, in_y, inlen_y);
	nDataLen += inlen_y;

	SM2_PUBLICKEY *sm2PubKey = SM2_PUBLICKEY_new();
	if (sm2PubKey == NULL)
		return SAR_ECCDATAErr;

	if (!ASN1_BIT_STRING_set(sm2PubKey, byData, nDataLen))
	{
		SM2_PUBLICKEY_free(sm2PubKey);
		return SAR_ECCDATAErr;
	}
	sm2PubKey->flags &= ~7;
	sm2PubKey->flags |= ASN1_STRING_FLAG_BITS_LEFT;

	unsigned char *pbTmp = out;
	long lOutLen = i2d_SM2_PUBLICKEY(sm2PubKey, &pbTmp);
	SM2_PUBLICKEY_free(sm2PubKey);
	if (lOutLen <= 0)
		return SAR_ECCDATAErr;

	*outlen = lOutLen;

	return SAR_OK;
}

long MYTOOL_SM2PublicKeyDecode(unsigned char *in, long inlen, unsigned char *out_x, long *outlen_x, unsigned char *out_y, long *outlen_y)
{
	unsigned char *pbTmp = in;
	SM2_PUBLICKEY *sm2PubKey = d2i_SM2_PUBLICKEY(NULL, (const unsigned char **)&pbTmp, inlen);
	if (sm2PubKey == NULL)
 		return SAR_ECCDATAErr;

	unsigned char byData[256];
	memset(byData, 0, sizeof(byData));
	int nLen = sm2PubKey->length;
	memcpy(byData, sm2PubKey->data, nLen);
	SM2_PUBLICKEY_free(sm2PubKey);

	nLen -= 1;
	int nSM2Len = nLen / 2;
	if (nSM2Len != SGD_SIZE_ECC && nSM2Len != SGD_SIZE_ECC_64)
		return SAR_ECCDATAErr;

	memcpy(out_x, byData + 1, nSM2Len);
	memcpy(out_y, byData + 1 + nSM2Len, nSM2Len);
	*outlen_x = nSM2Len;
	*outlen_y = nSM2Len;

	return SAR_OK;
}

// private key
long MYTOOL_SM2PrivateKeyEncode(unsigned char *in, long inlen, unsigned char *out, long *outlen)
{
	if (inlen != SGD_SIZE_ECC && inlen != SGD_SIZE_ECC_64)
		return SAR_ECCDATAErr;

	SM2_PRIVATEKEY *sm2PrvKey = bin_to_ASN1_INTEGER(in, inlen);
	if (sm2PrvKey == NULL)
		return SAR_ECCDATAErr;

	unsigned char *pbTmp = out;
	long lOutLen = i2d_SM2_PRIVATEKEY(sm2PrvKey, &pbTmp);
	SM2_PRIVATEKEY_free(sm2PrvKey);
	if (lOutLen <= 0)
		return SAR_ECCDATAErr;

	*outlen = lOutLen;

	return SAR_OK;
}

long MYTOOL_SM2PrivateKeyDecode(unsigned char *in, long inlen, unsigned char *out, long *outlen)
{
	unsigned char *pbTmp = in;
	SM2_PRIVATEKEY *sm2PrvKey = d2i_SM2_PRIVATEKEY(NULL, (const unsigned char **)&pbTmp, inlen);
	if (sm2PrvKey == NULL)
 		return SAR_ECCDATAErr;

	unsigned char byData[256];
	memset(byData, 0, sizeof(byData));
	int nLen = ASN1_INTEGER_to_bin(sm2PrvKey, byData);
	SM2_PRIVATEKEY_free(sm2PrvKey);
	if (nLen != SGD_SIZE_ECC && nLen != SGD_SIZE_ECC_64)
 		return SAR_ECCDATAErr;

	memcpy(out, byData, nLen);
	*outlen = nLen;

	return SAR_OK;
}

// sign data
long MYTOOL_SM2SignDataEncode(unsigned char *in_r, long inlen_r, unsigned char *in_s, long inlen_s, unsigned char *out, long *outlen)
{
	if ((inlen_r != inlen_s) ||
		(inlen_r != SGD_SIZE_ECC && inlen_r != SGD_SIZE_ECC_64))
		return SAR_ECCDATAErr;

	ASN1_INTEGER *r = bin_to_ASN1_INTEGER(in_r, inlen_r);
	if (r == NULL)
		return SAR_ECCDATAErr;
	ASN1_INTEGER *s = bin_to_ASN1_INTEGER(in_s, inlen_s);
	if (s == NULL)
	{
		ASN1_INTEGER_free(r);
		return SAR_ECCDATAErr;
	}

	SM2_SIGNATURE *sm2Sign = SM2_SIGNATURE_new();
	if (sm2Sign == NULL)
	{
		ASN1_INTEGER_free(r);
		ASN1_INTEGER_free(s);
		return SAR_ECCDATAErr;
	}

	ASN1_INTEGER_free(sm2Sign->r);
	sm2Sign->r = r;
	ASN1_INTEGER_free(sm2Sign->s);
	sm2Sign->s = s;

	unsigned char *pbTmp = out;
	long lOutLen = i2d_SM2_SIGNATURE(sm2Sign, &pbTmp);
	SM2_SIGNATURE_free(sm2Sign);
	if (lOutLen <= 0)
		return SAR_ECCDATAErr;

	*outlen = lOutLen;

	return SAR_OK;
}

long MYTOOL_SM2SignDataDecode(unsigned char *in, long inlen, unsigned char *out_r, long *outlen_r, unsigned char *out_s, long *outlen_s)
{
	unsigned char *pbTmp = in;
	SM2_SIGNATURE *sm2Sign = d2i_SM2_SIGNATURE(NULL, (const unsigned char **)&pbTmp, inlen);
	if (sm2Sign == NULL)
 		return SAR_ECCDATAErr;

	unsigned char byData[256];
	memset(byData, 0, sizeof(byData));
	int nLen = ASN1_INTEGER_to_bin(sm2Sign->r, byData);
	if (nLen != SGD_SIZE_ECC && nLen != SGD_SIZE_ECC_64)
	{
		SM2_SIGNATURE_free(sm2Sign);
 		return SAR_ECCDATAErr;
	}
	int nSM2Len = nLen;

	nLen = ASN1_INTEGER_to_bin(sm2Sign->s, byData + nSM2Len);
	if (nLen != nSM2Len)
	{
		SM2_SIGNATURE_free(sm2Sign);
 		return SAR_ECCDATAErr;
	}
	SM2_SIGNATURE_free(sm2Sign);

	memcpy(out_r, byData, nSM2Len);
	memcpy(out_s, byData + nSM2Len, nSM2Len);
	*outlen_r = nSM2Len;
	*outlen_s = nSM2Len;

	return SAR_OK;
}

// cipher data
long MYTOOL_SM2CipherDataEncode(unsigned char *in_c1, long inlen_c1, unsigned char *in_c2, long inlen_c2, unsigned char *in_c3, long inlen_c3, unsigned char *out, long *outlen)
{
	if ((inlen_c1 != SGD_SIZE_ECC * 2 && inlen_c1 != SGD_SIZE_ECC_64 * 2) ||
		(inlen_c3 != SGD_SIZE_SM3HASH))
		return SAR_ECCDATAErr;

	ASN1_INTEGER *x = bin_to_ASN1_INTEGER(in_c1, inlen_c1 / 2);
	if (x == NULL)
		return SAR_ECCDATAErr;
	ASN1_INTEGER *y = bin_to_ASN1_INTEGER(in_c1 + inlen_c1 / 2, inlen_c1 / 2);
	if (y == NULL)
	{
		ASN1_INTEGER_free(x);
		return SAR_ECCDATAErr;
	}

	SM2_CIPHER *sm2Cipher = SM2_CIPHER_new();
	if (sm2Cipher == NULL)
	{
		ASN1_INTEGER_free(x);
		ASN1_INTEGER_free(y);
		return SAR_ECCDATAErr;
	}

	ASN1_INTEGER_free(sm2Cipher->x);
	sm2Cipher->x = x;
	ASN1_INTEGER_free(sm2Cipher->y);
	sm2Cipher->y = y;
	int nRet = ASN1_OCTET_STRING_set(sm2Cipher->cipherText, in_c2, inlen_c2);
	if (!nRet)
	{
		SM2_CIPHER_free(sm2Cipher);
		return SAR_ECCDATAErr;
	}
	nRet = ASN1_OCTET_STRING_set(sm2Cipher->hash, in_c3, inlen_c3);
	if (!nRet)
	{
		SM2_CIPHER_free(sm2Cipher);
		return SAR_ECCDATAErr;
	}

	unsigned char *pbTmp = out;
	long lOutLen = i2d_SM2_CIPHER(sm2Cipher, &pbTmp);
	SM2_CIPHER_free(sm2Cipher);
	if (lOutLen <= 0)
		return SAR_ECCDATAErr;

	*outlen = lOutLen;

	return SAR_OK;
}

long MYTOOL_SM2CipherDataDecode(unsigned char *in, long inlen, unsigned char *out_c1, long *outlen_c1, unsigned char *out_c2, long *outlen_c2, unsigned char *out_c3, long *outlen_c3)
{
	unsigned char *pbTmp = in;
	SM2_CIPHER *sm2Cipher = d2i_SM2_CIPHER(NULL, (const unsigned char **)&pbTmp, inlen);
	if (sm2Cipher == NULL)
 		return SAR_ECCDATAErr;

	unsigned char byData[256];
	memset(byData, 0, sizeof(byData));
	int nLen = ASN1_INTEGER_to_bin(sm2Cipher->x, byData);
	if (nLen != SGD_SIZE_ECC && nLen != SGD_SIZE_ECC_64)
	{
		SM2_CIPHER_free(sm2Cipher);
 		return SAR_ECCDATAErr;
	}
	int nSM2Len = nLen;

	nLen = ASN1_INTEGER_to_bin(sm2Cipher->y, byData + nSM2Len);
	if (nLen != nSM2Len)
	{
		SM2_CIPHER_free(sm2Cipher);
 		return SAR_ECCDATAErr;
	}

	memcpy(out_c1, byData, nSM2Len * 2);
	*outlen_c1 = nSM2Len * 2;
	memcpy(out_c2, sm2Cipher->cipherText->data, sm2Cipher->cipherText->length);
	*outlen_c2 = sm2Cipher->cipherText->length;
	memcpy(out_c3, sm2Cipher->hash->data, sm2Cipher->hash->length);
	*outlen_c3 = sm2Cipher->hash->length;

	SM2_CIPHER_free(sm2Cipher);

	return SAR_OK;
}

////////////////////////////////////////////////////////////////
// 辅助功能函数

// 兼容linux
#ifndef _WIN32

char *strlwr_s(char *str)
{
	char *tmp = str;
	for (; *tmp != '\0'; tmp++)
		*tmp = tolower(*tmp);
	return str; 
} 

#endif

int mystr2bmpstr(const char *pcStr, unsigned int unStrSize, char *pcBMP, bool bBigEndian = true)
{
#ifdef _WIN32
	int n = MultiByteToWideChar(CP_ACP, 0,
		pcStr, unStrSize, (LPWSTR)pcBMP, unStrSize);
#else
/*	#include <locale.h>
	const char *pc_locale = setlocale(LC_CTYPE, "");
	int n = sprintf((char *)&rOutput[0], "%ls", pcStr);
*/
	CharArray rTmp(unStrSize * 2 + 2);
	char *pin = (char *)pcStr;
	char *pout = (char *)&rTmp[0];
	size_t inlen = unStrSize;
	size_t outlen = unStrSize * 2 + 2;
#ifdef KT_IOS
	iconv_t cd = iconv_open("UCS-2", "GB18030");
#else
	iconv_t cd = iconv_open("UNICODE", "GB18030");
#endif
	if (cd == NULL)
		return -1;

	int nRet = iconv(cd, &pin, &inlen, &pout, &outlen);
	iconv_close(cd);
	if (nRet == -1)
		return -2;
	int n = unStrSize * 2 + 2 - outlen;

	//Get rid off 0xFF,0xFE
	n -= 2;
	memcpy(pcBMP, &rTmp[0] + 2, n);
	n /= 2;
#endif // _WIN32

	//android(2012-08-06)
//#if !defined(KT_BIG_ENDIAN) //&& defined(_WIN32)
	if (bBigEndian)
	{
		unsigned char nEx = 0;
		int nHalfSize = n;
		for (int i = 0; i < nHalfSize; i++)
		{
			nEx = pcBMP[2 * i + 1];
			pcBMP[2 * i + 1] = pcBMP[2 * i];
			pcBMP[2 * i] = nEx;
		}
	}
//#endif

	return n * 2;
}

int mystr2utf8str(const char *pcStr, unsigned int unStrSize, char *pcUTF8)
{
#ifdef _WIN32
	CharArray rTmp(unStrSize * 2);
	int n = MultiByteToWideChar(CP_ACP, 0, 
		pcStr, unStrSize, (LPWSTR)&rTmp[0], rTmp.size() / 2);
	n = WideCharToMultiByte(CP_UTF8, 0, 
		(LPCWSTR)&rTmp[0], n, pcUTF8, unStrSize * 2, NULL, NULL);
#else	
	char *pin = (char *)pcStr;
	char *pout = pcUTF8;
	size_t inlen = unStrSize;
	size_t outlen = unStrSize * 2;
#ifdef KT_IOS
	iconv_t cd = iconv_open("UTF-8", "GB18030");
#else
	iconv_t cd = iconv_open("UTF-8", "GB18030");
#endif
	if (cd == NULL)
		return -1;

	int nRet = iconv(cd, &pin, &inlen, &pout, &outlen);
	iconv_close(cd);
	if (nRet == -1)
		return -2;
	int n = unStrSize * 2 - outlen;
#endif

	pcUTF8[n] = 0;
	return n;
}

int mybmpstr2str(const char *pcBMP, unsigned int unBMPSize, char *pcStr, bool bBigEndian = true)
{
	CharArray rBMP(pcBMP, pcBMP + unBMPSize);

	//android(2012-08-06)
//#if !defined(KT_BIG_ENDIAN) //&& defined(_WIN32)
	if (bBigEndian)
	{
		int nHalfSize = unBMPSize / 2;
		for (int i = 0; i < nHalfSize; i++)
		{
			rBMP[2 * i + 1] = *pcBMP++;
			rBMP[2 * i] = *pcBMP++;
		}
	}
//#endif

#ifdef _WIN32
	int n = WideCharToMultiByte(CP_ACP, 0,
		(LPCWSTR)&rBMP[0], unBMPSize / 2, pcStr, unBMPSize, NULL, NULL);
#else
	CharArray rTmp(unBMPSize + 2);
	rTmp[0] = 0xFF;
	rTmp[1] = 0xFE;
	memcpy(&rTmp[2], &rBMP[0], unBMPSize);

	char *pin = (char *)&rTmp[0];
	char *pout = pcStr;
	size_t inlen = unBMPSize + 2;
	size_t outlen = unBMPSize;
#ifdef KT_IOS
	iconv_t cd = iconv_open("GB18030", "UCS-2");
#else
	iconv_t cd = iconv_open("GB18030", "UNICODE");
#endif
	if (cd == NULL)
		return -1;

	int nRet = iconv(cd, &pin, &inlen, &pout, &outlen);
	iconv_close(cd);
	if (nRet == -1)
		return -2;
	int n = unBMPSize - outlen;
#endif

	pcStr[n] = 0;
	return n;
}

int mybmpstr2utf8str(const char *pcBMP, unsigned int unBMPSize, char *pcUTF8, bool bBigEndian = true)
{
	CharArray rBMP(pcBMP, pcBMP + unBMPSize);

	//android(2012-08-06)
//#if !defined(KT_BIG_ENDIAN) //&& defined(_WIN32)
	if (bBigEndian)
	{
		int nHalfSize = unBMPSize / 2;
		for (int i = 0; i < nHalfSize; i++)
		{
			rBMP[2 * i + 1] = *pcBMP++;
			rBMP[2 * i] = *pcBMP++;
		}
	}
//#endif
	
#ifdef _WIN32
	int n = WideCharToMultiByte(CP_UTF8, 0,
		(LPCWSTR)&rBMP[0], unBMPSize / 2, pcUTF8, unBMPSize * 2, NULL, NULL);
#else	
	CharArray rTmp(unBMPSize + 2);
	rTmp[0] = 0xFF;
	rTmp[1] = 0xFE;
	memcpy(&rTmp[2], &rBMP[0], unBMPSize);

	char *pin = (char *)&rTmp[0];
	char *pout = pcUTF8;
	size_t inlen = unBMPSize + 2;
	size_t outlen = unBMPSize * 2;
#ifdef KT_IOS
	iconv_t cd = iconv_open("UTF-8", "UCS-2");
#else
	iconv_t cd = iconv_open("UTF-8", "UNICODE");
#endif
	if (cd == NULL)
		return -1;

	int nRet = iconv(cd, &pin, &inlen, &pout, &outlen);
	iconv_close(cd);
	if (nRet == -2)
		return -2;
	int n = unBMPSize * 2 - outlen;
#endif

	pcUTF8[n] = 0;
	return n;
}

int myutf8str2str(const char *pcUTF8, unsigned int unUTF8Size, char *pcStr)
{
#ifdef _WIN32
	CharArray rTmp(unUTF8Size * 2);
	int n = MultiByteToWideChar(CP_UTF8, 0,
		pcUTF8, unUTF8Size, (LPWSTR)&rTmp[0], rTmp.size()/2);
	n = WideCharToMultiByte(CP_ACP, 0,
		(LPCWSTR)&rTmp[0], n, pcStr, unUTF8Size, NULL, NULL);
#else	
	char *pin = (char *)pcUTF8;
	char *pout = pcStr;
	size_t inlen = unUTF8Size;
	size_t outlen = unUTF8Size;
#ifdef KT_IOS
	iconv_t cd = iconv_open("GB18030", "UTF-8");
#else
	iconv_t cd = iconv_open("GB18030", "UTF-8");
#endif
	if (cd == NULL)
		return -1;

	int nRet = iconv(cd, &pin, &inlen, &pout, &outlen);
	iconv_close(cd);
	if (nRet == -1)
		return -2;
	int n = unUTF8Size - outlen;
#endif

	pcStr[n] = 0;
	return n;
}

int myutf8str2bmpstr(const char *pcUTF8, unsigned int unUTF8Size, char *pcBMP, bool bBigEndian = true)
{
#ifdef _WIN32
	int n = MultiByteToWideChar(CP_UTF8, 0,
		pcUTF8, unUTF8Size, (LPWSTR)pcBMP, unUTF8Size);
#else
	CharArray rTmp(unUTF8Size * 2 + 2);
	char *pin = (char *)pcUTF8;
	char *pout = (char *)&rTmp[0];
	size_t inlen = unUTF8Size;
	size_t outlen = unUTF8Size * 2 + 2;
#ifdef KT_IOS
	iconv_t cd = iconv_open("UCS-2", "UTF-8");
#else
	iconv_t cd = iconv_open("UNICODE", "UTF-8");
#endif
	if (cd == NULL)
		return -1;

	int nRet = iconv(cd, &pin, &inlen, &pout, &outlen);
	iconv_close(cd);
	if (nRet == -1)
		return -2;
	int n = unUTF8Size * 2 + 2 - outlen;

	//Get rid off 0xFF,0xFE
	n -= 2;
	memcpy(pcBMP, &rTmp[0] + 2, n);
	n /= 2;
#endif // _WIN32

	//android(2012-08-06)
//#if !defined(KT_BIG_ENDIAN) //&& defined(_WIN32)
	if (bBigEndian)
	{
		unsigned char nEx = 0;
		int nHalfSize = n;
		for (int i = 0; i < nHalfSize; i++)
		{
			nEx = pcBMP[2 * i + 1];
			pcBMP[2 * i + 1] = pcBMP[2 * i];
			pcBMP[2 * i] = nEx;
		}
	}
//#endif

	return n * 2;
}

// 1
long MYTOOL_Initialize()
{
//	OpenSSL_add_all_algorithms();
	
	if (g_dwMyToolsReference == 0)
	{
		MYSM_Initialize();

#if defined(_WIN32)
		WSADATA wsa_data;
		WSAStartup(0x202, &wsa_data);
#endif
	}
	g_dwMyToolsReference++;

	return SAR_OK;
}

long MYTOOL_Finalize()
{
// 	CONF_modules_unload(1);
// 	EVP_cleanup();
// 	ENGINE_cleanup();
// 	CRYPTO_cleanup_all_ex_data();
// 	ERR_remove_state(0);
// 	ERR_free_strings();

	g_dwMyToolsReference--;
	if (g_dwMyToolsReference == 0)
	{
		MYSM_Finalize();

#if defined(_WIN32)
		WSACleanup();
#endif
	}

	return SAR_OK;
}

long MYTOOL_GetReturnString(long lRet, char *pszReturnString)
{
	switch (lRet)
	{
	// 0
	case SAR_OK:
		strcpy(pszReturnString, "成功");
		break;
	// 1-12
	case SAR_UnknownErr:
		strcpy(pszReturnString, "异常错误");
		break;
	case SAR_NotSupportYetErr:
		strcpy(pszReturnString, "不支持的服务");
		break;
	case SAR_FileErr:
		strcpy(pszReturnString, "文件操作错误");
		break;
	case SAR_ProviderTypeErr:
		strcpy(pszReturnString, "服务提供者参数类型错误");
		break;
	case SAR_LoadProviderErr:
		strcpy(pszReturnString, "导入服务提供者接口错误");
		break;
	case SAR_LoadDevMngApiErr:
		strcpy(pszReturnString, "导入设备管理接口错误");
		break;
	case SAR_AlgoTypeErr:
		strcpy(pszReturnString, "算法类型错误");
		break;
	case SAR_NameLenErr:
		strcpy(pszReturnString, "名称长度错误");
		break;
	case SAR_KeyUsageErr:
		strcpy(pszReturnString, "密钥用途错误");
		break;
	case SAR_ModulusLenErr:
		strcpy(pszReturnString, "模的长度错误");
		break;
	case SAR_NotInitializeErr:
		strcpy(pszReturnString, "未初始化");
		break;
	case SAR_ObjErr:
		strcpy(pszReturnString, "对象错误");
		break;
	// 100-101
	case SAR_MemoryErr:
		strcpy(pszReturnString, "内存错误");
		break;
	case SAR_TimeoutErr:
		strcpy(pszReturnString, "服务超时");
		break;
	// 200-201
	case SAR_IndataLenErr:
		strcpy(pszReturnString, "输入数据长度错误");
		break;
	case SAR_IndataErr:
		strcpy(pszReturnString, "输入数据错误");
		break;
	// 300-311
	case SAR_GenRandErr:
		strcpy(pszReturnString, "生成随机数错误");
		break;
	case SAR_HashObjErr:
		strcpy(pszReturnString, "HASH对象错");
		break;
	case SAR_HashErr:
		strcpy(pszReturnString, "HASH运算错误");
		break;
	case SAR_GenRsaKeyErr:
		strcpy(pszReturnString, "产生RSA密钥错");
		break;
	case SAR_RsaModulusLenErr:
		strcpy(pszReturnString, "RSA密钥模长错误");
		break;
	case SAR_CspImprtPubKeyErr:
		strcpy(pszReturnString, "CSP服务导入公钥错误");
		break;
	case SAR_RsaEncErr:
		strcpy(pszReturnString, "RSA加密错误");
		break;
	case SAR_RSGDecErr:
		strcpy(pszReturnString, "RSA解密错误");
		break;
	case SAR_HashNotEqualErr:
		strcpy(pszReturnString, "HASH值不相等");
		break;
	case SAR_KeyNotFountErr:
		strcpy(pszReturnString, "密钥未发现");
		break;
	case SAR_CertNotFountErr:
		strcpy(pszReturnString, "证书未发现");
		break;
	case SAR_NotExportErr:
		strcpy(pszReturnString, "对象未导出");
		break;
	// 400-422
	case SAR_DecryptPadErr:
		strcpy(pszReturnString, "解密时做补丁错误");
		break;
	case SAR_MacLenErr:
		strcpy(pszReturnString, "MAC长度错误");
		break;
	case SAR_KeyInfoTypeErr:
		strcpy(pszReturnString, "密钥类型错误");
		break;
	case SAR_NULLPointerErr:
		strcpy(pszReturnString, "某一个参数为空指针");
		break;
	case SAR_APPNOTFOUNDErr:
		strcpy(pszReturnString, "没有找到该应用");
		break;
	case SAR_CERTENCODEErr:
		strcpy(pszReturnString, "证书编码格式错误");
		break;
	case SAR_CERTINVALIDErr:
		strcpy(pszReturnString, "证书无效，不是可信ca颁发的证书");
		break;
	case SAR_CERTHASEXPIREDErr:
		strcpy(pszReturnString, "证书已过期");
		break;
	case SAR_CERTREVOKEDErr:
		strcpy(pszReturnString, "证书已经被吊销");
		break;
	case SAR_SIGNDATAErr:
		strcpy(pszReturnString, "签名失败");
		break;
	case SAR_VERIFYSIGNDATAErr:
		strcpy(pszReturnString, "验证签名失败");
		break;
	case SAR_READFILEErr:
		strcpy(pszReturnString, "读文件异常，可能文件不存在或没有读取权限等");
		break;
	case SAR_WRITEFILEErr:
		strcpy(pszReturnString, "写文件异常，可能文件不存在或没有写权限等");
		break;
	case SAR_SECRETSEGMENTErr:
		strcpy(pszReturnString, "门限算法密钥分割失败");
		break;
	case SAR_SECERTRECOVERYErr:
		strcpy(pszReturnString, "门限恢复失败");
		break;
	case SAR_ENCRYPTDATAErr:
		strcpy(pszReturnString, "对数据的对称加密失败");
		break;
	case SAR_DECRYPTDATAErr:
		strcpy(pszReturnString, "对称算法的数据解密失败");
		break;
	case SAR_PKCS7ENCODEErr:
		strcpy(pszReturnString, "PKCS#7编码格式错误");
		break;
	case SAR_XMLENCODEErr:
		strcpy(pszReturnString, "不是合法的xml编码数据");
		break;
	case SAR_PARAMETERNOTSUPPORTErr:
		strcpy(pszReturnString, "不支持的参数");
		break;
	case SAR_LoginErr:
		strcpy(pszReturnString, "登录失败");
		break;
	case SAR_AddP7BErr:
		strcpy(pszReturnString, "添加信任列表失败");
		break;
	case SAR_CertRevokedErr:
		strcpy(pszReturnString, "证书已注销");
		break;
	// 添加 500-522
	case SAR_BASE64ENCODEDECODEErr:
		strcpy(pszReturnString, "Base64编解码失败");
		break;
	case SAR_PINErr:
		strcpy(pszReturnString, "核对证书口令失败");
		break;
	case SAR_ObjNotFountErr:
		strcpy(pszReturnString, "对象未发现");
		break;
	case SAR_COMMUNICATIONErr:
		strcpy(pszReturnString, "通讯数据传输错误");
		break;
	case SAR_DATABASEErr:
		strcpy(pszReturnString, "访问数据库时，发生系统错误");
		break;
	case SAR_CERTNOTYETVALIDErr:
		strcpy(pszReturnString, "证书未生效");
		break;
	case SAR_CERTNOTINRLErr:
		strcpy(pszReturnString, "企业不在红名单中，企业还未交网上服务费");
		break;
	case SAR_CERTNOTINDZBErr:
		strcpy(pszReturnString, "企业不在对照表中，数据整理还未完成");
		break;
	case SAR_SERVICEHASEXPIREDErr:
		strcpy(pszReturnString, "服务已过期");
		break;
	case SAR_TSSERVERErr:
		strcpy(pszReturnString, "时间戳服务器内部错误");
		break;
	// 510
	case SAR_CREATETSREQUESTErr:
		strcpy(pszReturnString, "创建时间戳请求失败");
		break;
	case SAR_SIGNTSErr:
		strcpy(pszReturnString, "签发时间戳失败");
		break;
	case SAR_TSENCODEErr:
		strcpy(pszReturnString, "时间戳编码格式错误");
		break;
	case SAR_VERIFYTSErr:
		strcpy(pszReturnString, "验证时间戳失败");
		break;
	case SAR_ObjExistErr:
		strcpy(pszReturnString, "对象已存在");
		break;
	case SAR_CRLENCODEErr:
		strcpy(pszReturnString, "CRL编码格式错误");
		break;
	case SAR_CRLINVALIDErr:
		strcpy(pszReturnString, "不是可信ca颁发的CRL");
		break;
	case SAR_ECCCERTENCODEErr:
		strcpy(pszReturnString, "ECC证书编码格式错误");
		break;
	case SAR_ECCOPERErr:
		strcpy(pszReturnString, "ECC运算错误");
		break;
	case SAR_ECCSIGNErr:
		strcpy(pszReturnString, "ECC签名错误");
		break;
	// 520
	case SAR_ECCVERIFYSIGNErr: 
		strcpy(pszReturnString, "ECC验证签名错误");
		break;
	case SAR_ECCENCErr:
		strcpy(pszReturnString, "ECC加密错误");
		break;
	case SAR_ECCDECErr:
		strcpy(pszReturnString, "ECC解密错误");
		break;
	case SAR_ECCDATAErr:
		strcpy(pszReturnString, "ECC数据编码错误");
		break;
	default:
		strcpy(pszReturnString, "其它错误");
		break;
	}

	return SAR_OK;
}

// 2
long MYTOOL_GetCertVersion(unsigned char *cert, long certlen, char *version, long *versionlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	long lVersion = ASN1_INTEGER_get(x509->cert_info->version);
	sprintf(version, "V%d", lVersion + 1);
	*versionlen = strlen(version);

	X509_free(x509);

	return SAR_OK;
}

long MYTOOL_GetCertSerialNumber(unsigned char *cert, long certlen, char *sn, long *snlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	BIO *mbio = BIO_new(BIO_s_mem());
	if (mbio == NULL)
	{
		X509_free(x509);
		return SAR_MemoryErr;
	}
	BIO_set_close(mbio, BIO_CLOSE); // BIO_free() free BUF_MEM
	int nRet = i2a_ASN1_INTEGER(mbio, X509_get_serialNumber(x509));
	X509_free(x509);
	if (nRet <= 0)
	{
		BIO_free(mbio);
		return SAR_CERTENCODEErr;
	}

	BUF_MEM *bptr;
	BIO_get_mem_ptr(mbio, &bptr);
	int i = 0, j = 0;
	for (i = 0; i < bptr->length; )
	{
		sn[j++] = (char)bptr->data[i++];
	}
	sn[j] = 0;
	*snlen = j;
	strlwr_s(sn);
	BIO_free(mbio);

	return SAR_OK;
}

long MYTOOL_GetCertSignAlg(unsigned char *cert, long certlen, char *signalg, long *signalglen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	char szTmp[128];
	memset(szTmp, 0, sizeof(szTmp));
	OBJ_obj2txt(szTmp, sizeof(szTmp), x509->sig_alg->algorithm, 0);
	X509_free(x509);

	// 兼容linux
/*	CString strSignAlg = szTmp;
	strSignAlg.Replace("With", "");
	strSignAlg.Replace("Encryption", "");
	strcpy(signalg, strSignAlg);
	*signalglen = strlen(signalg);
*/
	String strSignAlg = szTmp;
	String::size_type pos;
	if ((pos = strSignAlg.find("With")) != String::npos)
		strSignAlg.erase(pos, 4);
	if ((pos = strSignAlg.find("Encryption")) != String::npos)
		strSignAlg.erase(pos, 10);

	if (strSignAlg == "1.2.156.197.1.501" ||
		strSignAlg == "1.2.156.10197.1.501" ||
		strSignAlg == "1-2-156-197-1-501" ||
		strSignAlg == "1-2-156-10197-1-501")
		strSignAlg = "sm3SM2";

	strcpy(signalg, strSignAlg.c_str());
	*signalglen = strlen(signalg);

	return SAR_OK;
}

#define g2(p) (((p)[0] - '0') * 10 + (p)[1] - '0')
#define g4(p) (((p)[0] - '0') * 1000 + ((p)[1] - '0') * 100 + ((p)[2] - '0') * 10 + (p)[3] - '0')
void mycerttime2string(asn1_string_st *stTime, char *pszTime)
{
	int nBias = -480;
	// 兼容linux
#ifdef _WIN32
	TIME_ZONE_INFORMATION TimeZoneInformation;
	if (GetTimeZoneInformation(&TimeZoneInformation) != TIME_ZONE_ID_INVALID)
		nBias = TimeZoneInformation.Bias;
#else
	struct timeb tstruct;
	ftime(&tstruct);
	nBias = tstruct.timezone;
#endif
	
	ASN1_UTCTIME *utcTime = ASN1_STRING_dup(stTime);
	int nLen = utcTime->length;
	int nYearLen = 2;
	if (nLen == 15)
		nYearLen = 4;
	int year, month, day, hour, minute, second;
	if (nYearLen == 2)
	{
		year = g2(utcTime->data);
		year < 50 ? year += 2000 : year += 1900;
	}
	else
	{
		year = g4(utcTime->data);
	}
	month = g2(utcTime->data + nYearLen);
	day = g2(utcTime->data + nYearLen + 2);
	hour = g2(utcTime->data + nYearLen + 4);
	minute = g2(utcTime->data + nYearLen + 6);
	second = g2(utcTime->data + nYearLen + 8);

	// 兼容linux
/*	CTime ctmTime(
		year,
		month,
		day,
		hour,
		minute - nBias,					// 转换为本地时间
		second);
	sprintf(pszTime, "%d-%02d-%02d %02d:%02d:%02d",
		ctmTime.GetYear(), ctmTime.GetMonth(), ctmTime.GetDay(),
		ctmTime.GetHour(), ctmTime.GetMinute(), ctmTime.GetSecond());
*/
	struct tm tmTmp;
	memset(&tmTmp, 0, sizeof(tm));
	tmTmp.tm_year = year - 1900;
	tmTmp.tm_mon = month - 1;
	tmTmp.tm_mday = day;
	tmTmp.tm_hour = hour;
	tmTmp.tm_min = minute;
	tmTmp.tm_sec = second;

	int n = tmTmp.tm_year + 1900 - 2037;
	if (n > 0)
		tmTmp.tm_year -= n;

	time_t timeTmp = mktime(&tmTmp);
	timeTmp -= nBias * 60;
	tmTmp = *localtime(&timeTmp);

	if (n > 0)
		tmTmp.tm_year += n;

	sprintf(pszTime, "%d-%02d-%02d %02d:%02d:%02d",
		tmTmp.tm_year + 1900, tmTmp.tm_mon + 1, tmTmp.tm_mday,
		tmTmp.tm_hour, tmTmp.tm_min, tmTmp.tm_sec);

	M_ASN1_UTCTIME_free(utcTime);
}

long MYTOOL_GetCertValidity(unsigned char *cert, long certlen, char *notbefore, char *notafter)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	asn1_string_st *stTimeNotBefore = X509_get_notBefore(x509);
	asn1_string_st *stTimeNotAfter = X509_get_notAfter(x509);
	if (stTimeNotBefore == NULL || stTimeNotAfter == NULL)
	{
		X509_free(x509);
		return SAR_CERTENCODEErr;
	}

	mycerttime2string(stTimeNotBefore, notbefore);
	mycerttime2string(stTimeNotAfter, notafter);

	X509_free(x509);

	return SAR_OK;
}

// 字符串反向（用于DN：从大到小，从小到大）
void ReverseString(const char *pszSeparator, char *str)
{
	// 兼容linux
	char *pszTmp1 = str, *pszTmp2, szTmp;
//	CStringArray astrTmp;
//	astrTmp.RemoveAll();
	StringArray astrTmp;
	astrTmp.empty();
	while (1)
	{
		pszTmp2 = strstr(pszTmp1, pszSeparator);
		if (pszTmp2 != NULL)
		{
			szTmp = *pszTmp2;
			*pszTmp2 = 0;
			if (strlen(pszTmp1) > 0)
//				astrTmp.Add(pszTmp1);
				astrTmp.push_back(pszTmp1);
			*pszTmp2 = szTmp;
			pszTmp1 = pszTmp2 + strlen(pszSeparator);
		}
		else
		{
			if (strlen(pszTmp1) > 0)
//				astrTmp.Add(pszTmp1);
				astrTmp.push_back(pszTmp1);
			break;
		}
	}

//	if (astrTmp.GetSize() > 0)
	if (astrTmp.size() > 0)
	{
		strcpy(str, "");
//		for (int i = astrTmp.GetSize() - 1; i >= 0 ; i--)
		for (int i = astrTmp.size() - 1; i >= 0 ; i--)
		{
			if (strlen(str) > 0)
				strcat(str, pszSeparator);
//			strcat(str, astrTmp.GetAt(i));
			strcat(str, astrTmp[i].c_str());
		}
	}
}

// 取证书DN
KTBOOL32 mycertname2string(const char *pszSeparator, X509_NAME *nm, char *pszDN)
{
	if (nm == NULL)
		return FALSE;

	int num = X509_NAME_entry_count(nm);
	if (num <= 0)
		return FALSE;

	// 兼容linux
//	USES_CONVERSION;
//	setlocale(LC_CTYPE, "");
//	char asndata2[512];
//	wchar_t wdata[512];
	long n;

	int fn_nid, asnlen, asntype;
	char szOut[512];
	char szName[512], szValue[512];
	char *asndata;
	X509_NAME_ENTRY *entry;
	ASN1_OBJECT *obj;
	ASN1_STRING *data;

	memset(szOut, 0, sizeof(szOut));
	for (int i = 0; i < num; i++)
	{
		memset(szName, 0, sizeof(szName));
		memset(szValue, 0, sizeof(szValue));

		entry = (X509_NAME_ENTRY *)X509_NAME_get_entry(nm, i);
		obj = X509_NAME_ENTRY_get_object(entry);
		data = X509_NAME_ENTRY_get_data(entry);

		// 数据类型
		fn_nid = OBJ_obj2nid(obj);
		if (fn_nid == NID_undef)
			OBJ_obj2txt(szName, sizeof(szName), obj, 1);
		else
			strcpy(szName, OBJ_nid2sn(fn_nid));

		if (strcmp(szName, "ST") == 0)
			strcpy(szName, "S");
		else if (strcmp(szName, "GN") == 0)
			strcpy(szName, "G");
		else if (strcmp(szName, "emailAddress") == 0)
			strcpy(szName, "E");

		// 数据值
		asndata = (char *)ASN1_STRING_data(data);
		asnlen = ASN1_STRING_length(data);
		asntype = ASN1_STRING_type(data);

		if (asntype == V_ASN1_BMPSTRING)
		{
			// 兼容linux
/*			memset(asndata2, 0, sizeof(asndata2));
			for (int j = 0; j < asnlen / 2; j++)
			{
				asndata2[2 * j + 1] = *asndata++;
				asndata2[2 * j] = *asndata++;
			}
			strcpy(szValue, W2A((PWSTR)asndata2));
*/			n = mybmpstr2str(asndata, asnlen, szValue);
			if (n <= 0)
				return FALSE;
		}
		else if (asntype == V_ASN1_UTF8STRING)
		{
			// 兼容linux
/*			memset(wdata, 0, sizeof(wdata));
			int wdatalen = MultiByteToWideChar(
				CP_UTF8,
				0,
				asndata,
				asnlen,
				wdata,
				512);
			if (wdatalen <= 0)
				return FALSE;
			int datalen = WideCharToMultiByte(
				CP_ACP,
				0,
				wdata,
				wdatalen,
				szValue,
				512,
				NULL,
				NULL);
			if (datalen <= 0)
				return FALSE;
*/			n = myutf8str2str(asndata, asnlen, szValue);
			if (n <= 0)
				return FALSE;
		}
		else
			memcpy(szValue, asndata, asnlen);

		if (i > 0)
			strcat(szOut, pszSeparator);
		strcat(szOut, szName);
		strcat(szOut, "=");
		strcat(szOut, szValue);
	}

	if (strlen(szOut) == 0)
		return FALSE;
	strcpy(pszDN, szOut);

	return TRUE;
}

long MYTOOL_GetCertDN(unsigned char *cert, long certlen, char *dn, long *dnlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	X509_NAME *nm = X509_get_subject_name(x509);
	if (nm == NULL)
	{
		X509_free(x509);
		return SAR_CERTENCODEErr;
	}

	if (!mycertname2string(SGD_STR_SEPARATOR, nm, dn))
	{
		X509_free(x509);
		return SAR_CERTENCODEErr;
	}
	X509_free(x509);

	*dnlen = strlen(dn);

	return SAR_OK;
}

long MYTOOL_GetCertIssuerDN(unsigned char *cert, long certlen, char *issuerdn, long *issuerdnlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	X509_NAME *nm = X509_get_issuer_name(x509);
	if (nm == NULL)
	{
		X509_free(x509);
		return SAR_CERTENCODEErr;
	}

	if (!mycertname2string(SGD_STR_SEPARATOR, nm, issuerdn))
	{
		X509_free(x509);
		return SAR_CERTENCODEErr;
	}
	X509_free(x509);

	*issuerdnlen = strlen(issuerdn);

	return SAR_OK;
}

KTBOOL32 GetDNItemValue(char *dn, long dnlen, char *itemname, char *itemvalue, long *itemvaluelen)
{
	char szItemName[20];
	char szItemValue[512];
	memset(szItemValue, 0, sizeof(szItemValue));
	char *pszTmp1 = dn, *pszTmp2, *pszTmp3, szTmp;
	while (1)
	{
		memset(szItemName, 0, sizeof(szItemName));
		if (pszTmp1 != dn)
			strcpy(szItemName, SGD_STR_SEPARATOR);
		strcat(szItemName, itemname);
		strcat(szItemName, "=");
	
		pszTmp2 = strstr(pszTmp1, szItemName);
		if (pszTmp2 == NULL)
			break;

		pszTmp2 += strlen(szItemName);
		pszTmp3 = strstr(pszTmp2, SGD_STR_SEPARATOR);
		if (strlen(szItemValue) > 0)
			strcat(szItemValue, SGD_STR_SEPARATOR);
		if (pszTmp3 != NULL)
		{
			szTmp = *pszTmp3;
			*pszTmp3 = 0;
			strcat(szItemValue, pszTmp2);
			*pszTmp3 = szTmp;

			pszTmp1 = pszTmp3;
		}
		else
		{
			strcat(szItemValue, pszTmp2);
			break;
		}
	}
	if (strlen(szItemValue) == 0)
		return FALSE;

	*itemvaluelen = strlen(szItemValue);
	strcpy(itemvalue, szItemValue);

	return TRUE;
}

long MYTOOL_GetCertDNItemValue(unsigned char *cert, long certlen, char *itemname, char *itemvalue, long *itemvaluelen)
{
	long dnlen = 512;
	char dn[512];
	memset(dn, 0, sizeof(dn));
	long lRet = MYTOOL_GetCertDN(cert, certlen, dn, &dnlen);
	if (lRet != SAR_OK)
		return lRet;

	KTBOOL32 bRet = GetDNItemValue(dn, dnlen, itemname, itemvalue, itemvaluelen);
	if (bRet)
		return SAR_OK;
	else
		return SAR_ObjNotFountErr;
}

long MYTOOL_GetCertIssuerDNItemValue(unsigned char *cert, long certlen, char *itemname, char *itemvalue, long *itemvaluelen)
{
	long dnlen = 512;
	char dn[512];
	memset(dn, 0, sizeof(dn));
	long lRet = MYTOOL_GetCertIssuerDN(cert, certlen, dn, &dnlen);
	if (lRet != SAR_OK)
		return lRet;

	KTBOOL32 bRet = GetDNItemValue(dn, dnlen, itemname, itemvalue, itemvaluelen);
	if (bRet)
		return SAR_OK;
	else
		return SAR_ObjNotFountErr;
}

long MYTOOL_GetCertEmail(unsigned char *cert, long certlen, char *email, long *emaillen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

/*	int pos = X509_get_ext_by_NID(x509, NID_subject_alt_name, -1);
	if (pos == -1)
	{
		X509_free(x509);
		return SAR_ObjNotFountErr;
	}

	X509_EXTENSION *ext = X509_get_ext(x509, pos);
	if (ext == NULL)
	{
		X509_free(x509);
		return SAR_ObjNotFountErr;
	}

	GENERAL_NAMES *gns = (GENERAL_NAMES *)X509V3_EXT_d2i(ext);
*/	GENERAL_NAMES *gns = (GENERAL_NAMES *)X509_get_ext_d2i(x509, NID_subject_alt_name, NULL, NULL);
	X509_free(x509);
	if (gns == NULL)
		return SAR_ObjNotFountErr;

	GENERAL_NAME *gn = NULL;
	for (int i = 0; i < sk_GENERAL_NAME_num(gns); i++)
	{
		GENERAL_NAME *gni = sk_GENERAL_NAME_value(gns, i);
		if (gni == NULL)
			continue;

		if (gni->type == GEN_EMAIL)
		{
			gn = gni;
			break;
		}
	}
	if (gn == NULL)
	{
		GENERAL_NAMES_free(gns);
		return SAR_ObjNotFountErr;
	}
	*emaillen = ASN1_STRING_length(gn->d.rfc822Name);
	memcpy(email, (char *)ASN1_STRING_data(gn->d.rfc822Name), *emaillen);

	GENERAL_NAMES_free(gns);

	return SAR_OK;
}

long MYTOOL_RSAGetCertPubKey(unsigned char *cert, long certlen, unsigned char *pubkey, long *pubkeylen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	EVP_PKEY *pkey = X509_get_pubkey(x509);
	X509_free(x509);
	if (pkey == NULL)
		return SAR_CERTENCODEErr;

	unsigned char byPubKey[256];
	memset(byPubKey, 0, sizeof(byPubKey));
	pszTmp = byPubKey;
	long lPubKeyLen = i2d_PublicKey(pkey, &pszTmp);
	EVP_PKEY_free(pkey);
	if (lPubKeyLen <= 0)
		return SAR_CERTENCODEErr;

	memcpy(pubkey, byPubKey, lPubKeyLen);
	*pubkeylen = lPubKeyLen;

	return SAR_OK;
}

long MYTOOL_GetCertPubKey(unsigned char *cert, long certlen, char *pubkey, long *pubkeylen)
{
	unsigned char byPubKey[256];
	memset(byPubKey, 0, sizeof(byPubKey));
	long lPubKeyLen;

	unsigned char byPubKey_x[256], byPubKey_y[256];
	memset(byPubKey_x, 0, sizeof(byPubKey_x));
	memset(byPubKey_y, 0, sizeof(byPubKey_y));
	long lPubKeyLen_x, lPubKeyLen_y;
	long lRet = MYSM_ECCGetPubKeyFromCert(
		cert,
		certlen,
		byPubKey_x,
		&lPubKeyLen_x,
		byPubKey_y,
		&lPubKeyLen_y);
	if (lRet != MYSMERR_SUCCESS)
	{
		// RSA
		lRet = MYTOOL_RSAGetCertPubKey(
			cert,
			certlen,
			byPubKey,
			&lPubKeyLen);
		if (lRet != SAR_OK)
			return lRet;
	}
	else
	{
		// ECC
		lPubKeyLen = 0;
		memcpy(byPubKey, "\x04", 1);
		lPubKeyLen += 1;
		memcpy(byPubKey + lPubKeyLen, byPubKey_x, lPubKeyLen_x);
		lPubKeyLen += lPubKeyLen_x;
		memcpy(byPubKey + lPubKeyLen, byPubKey_y, lPubKeyLen_y);
		lPubKeyLen += lPubKeyLen_y;
	}

	// Base64编码
//	long lPubKeyB64Len = CMailCoder::base64_encode((char *)byPubKey, lPubKeyLen, (char *)pubkey);
//	if (lPubKeyB64Len <= 0)
//		return SAR_BASE64ENCODEDECODEErr;
//	*pubkeylen = lPubKeyB64Len;

	char szTmp[10];
	strcpy(pubkey, "");
	for (long i = 0; i < lPubKeyLen; i++)
	{
//		if (i > 0 && i % 20 == 0)
//			strcat(pubkey, SGD_STR_RETURN);
		sprintf(szTmp, "%02x ", byPubKey[i]);
		strcat(pubkey, szTmp);
	}
	*pubkeylen = strlen(pubkey);

	return SAR_OK;
}

long MYTOOL_GetCertExtensions(unsigned char *cert, long certlen, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	STACK_OF(X509_EXTENSION) *exts = x509->cert_info->extensions;
	if (exts == NULL)
	{
		X509_free(x509);
		return SAR_ObjNotFountErr;
	}

	unsigned char byExtensions[SGD_SIZE_CERT];
	pszTmp = byExtensions;
	long lExtensionsLen = i2d_ASN1_SET_OF_X509_EXTENSION(exts, &pszTmp, i2d_X509_EXTENSION,
		V_ASN1_SEQUENCE, V_ASN1_UNIVERSAL, IS_SEQUENCE);
	X509_free(x509);
	if (lExtensionsLen <= 0)
		return SAR_ObjNotFountErr;

	// Base64编码
//	long lExtensionsB64Len = CMailCoder::base64_encode((char *)byExtensions, lExtensionsLen, out);
//	if (lExtensionsB64Len <= 0)
//		return SAR_BASE64ENCODEDECODEErr;
//	*outlen = lExtensionsB64Len;

	char szTmp[10];
	strcpy(out, "");
	for (long i = 0; i < lExtensionsLen; i++)
	{
//		if (i > 0 && i % 20 == 0)
//			strcat(out, SGD_STR_RETURN);
		sprintf(szTmp, "%02x ", byExtensions[i]);
		strcat(out, szTmp);
	}
	*outlen = strlen(out);

	return SAR_OK;
}

// 3
long GetExtKeyIdentifier(unsigned char *cert, long certlen, char *out, long *outlen, KTBOOL32 bAuthority)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	void *ext;
	if (bAuthority)
		ext = X509_get_ext_d2i(x509, NID_authority_key_identifier, NULL, NULL);
	else
		ext = X509_get_ext_d2i(x509, NID_subject_key_identifier, NULL, NULL);
	X509_free(x509);
	if (ext == NULL)
		return SAR_ObjNotFountErr;

	int datalen;
	if (bAuthority)
		datalen = ((AUTHORITY_KEYID *)ext)->keyid->length;
	else
		datalen = ((ASN1_BIT_STRING *)ext)->length;
	unsigned char *data;
	if (bAuthority)
		data = ((AUTHORITY_KEYID *)ext)->keyid->data;
	else
		data = ((ASN1_BIT_STRING *)ext)->data;

	char szTmp[10];
	if (bAuthority)
		strcpy(out, "KeyID=");
	else
		strcpy(out, "");
	for (int i = 0; i < datalen; i++)
	{
		if (i > 0)
			strcat(out, " ");
		sprintf(szTmp, "%02x", data[i]);
		strcat(out, szTmp);
	}
	*outlen = strlen(out);

	if (bAuthority)
		AUTHORITY_KEYID_free((AUTHORITY_KEYID *)ext);
	else
		ASN1_BIT_STRING_free((ASN1_BIT_STRING *)ext);

	return SAR_OK;
}

long MYTOOL_GetExtAuthorityKeyIdentifier(unsigned char *cert, long certlen, char *out, long *outlen)
{
	return GetExtKeyIdentifier(cert, certlen, out, outlen, TRUE);
}

long MYTOOL_GetExtSubjectKeyIdentifier(unsigned char *cert, long certlen, char *out, long *outlen)
{
	return GetExtKeyIdentifier(cert, certlen, out, outlen, FALSE);
}

long MYTOOL_GetExtKeyUsage(unsigned char *cert, long certlen, DWORD *keyusage, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	ASN1_BIT_STRING *ext = (ASN1_BIT_STRING *)X509_get_ext_d2i(x509, NID_key_usage, NULL, NULL);
	X509_free(x509);
	if (ext == NULL)
		return SAR_ObjNotFountErr;

	// 密钥用途值
	DWORD dwKeyUsage = 0;
	if (ext->length > 0)
	{
		dwKeyUsage = ext->data[0];
		if (ext->length > 1)
			dwKeyUsage |= ext->data[1] << 8;
	}
	ASN1_BIT_STRING_free(ext);

	// 密钥用途字符串
	strcpy(out, "");
	if (dwKeyUsage & X509v3_KU_DIGITAL_SIGNATURE)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Digital Signature");
	}
	if (dwKeyUsage & X509v3_KU_NON_REPUDIATION)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Non-Repudiation");
	}
	if (dwKeyUsage & X509v3_KU_KEY_ENCIPHERMENT)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Key Encipherment");
	}
	if (dwKeyUsage & X509v3_KU_DATA_ENCIPHERMENT)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Data Encipherment");
	}
	if (dwKeyUsage & X509v3_KU_KEY_AGREEMENT)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Key Agreement");
	}
	if (dwKeyUsage & X509v3_KU_KEY_CERT_SIGN)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Certificate Sign");
	}
	if (dwKeyUsage & X509v3_KU_CRL_SIGN)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "CRL Sign");
	}
	if (dwKeyUsage & X509v3_KU_ENCIPHER_ONLY)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Encipher Only");
	}
	if (dwKeyUsage & X509v3_KU_DECIPHER_ONLY)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Decipher Only");
	}
	if (strlen(out) == 0)
		strcpy(out, "Undefined");

	char szTmp[10];
	sprintf(szTmp, " (%02x)", dwKeyUsage);
	strcat(out, szTmp);
	*keyusage = dwKeyUsage;
	*outlen = strlen(out);

	return SAR_OK;
}

long MYTOOL_GetExtCertificatePolicies(unsigned char *cert, long certlen, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	CERTIFICATEPOLICIES *cps = (CERTIFICATEPOLICIES *)X509_get_ext_d2i(x509, NID_certificate_policies, NULL, NULL);
	X509_free(x509);
	if (cps == NULL)
		return SAR_ObjNotFountErr;

	int nRet, ncount = 0, ncount2 = 0;
	char szTmp[512], szSpace[10];
	strcpy(szSpace, "     ");
	POLICYINFO *cp;
	ASN1_OBJECT *policyid;
	STACK_OF(POLICYQUALINFO) *pqs;
	POLICYQUALINFO *pq;

	strcpy(out, "");
	for (int i = 0; i < sk_POLICYINFO_num(cps); i++)
	{
		cp = sk_POLICYINFO_value(cps, i);
		if (cp == NULL)
			continue;

		// Policy Identifier
		policyid = cp->policyid;
		if (policyid == NULL)
			continue;

		ncount++;
		if (ncount > 1)
			strcat(out, SGD_STR_RETURN);
		sprintf(szTmp, "[%d]Certificate Policy:", ncount);
		strcat(out, szTmp);

		memset(szTmp, 0, sizeof(szTmp));
		nRet = OBJ_obj2txt(szTmp, sizeof(szTmp), policyid, 0);
		if (nRet > 0)
		{
			strcat(out, SGD_STR_RETURN);
			strcat(out, szSpace);
			strcat(out, "Policy Identifier=");
			strcat(out, szTmp);
		}

		// Policy Qualifier
		pqs = cp->qualifiers;
		if (pqs == NULL)
			continue;

		ncount2 = 0;
		for (int j = 0; j < sk_POLICYQUALINFO_num(pqs); j++)
		{
			pq = sk_POLICYQUALINFO_value(pqs, j);
			if (pq == NULL)
				continue;

			ncount2++;
			sprintf(szTmp, "[%d,%d]Policy Qualifier Info:", ncount, ncount2);
			strcat(out, SGD_STR_RETURN);
			strcat(out, szSpace);
			strcat(out, szTmp);

			switch (OBJ_obj2nid(pq->pqualid))
            {
			case NID_id_qt_cps:
				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Policy Qualifier Id=CPS");

				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Qualifier:");

				memset(szTmp, 0, sizeof(szTmp));
				memcpy(szTmp, pq->d.cpsuri->data, pq->d.cpsuri->length);
				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szTmp);
				break;

			case NID_id_qt_unotice:
				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Policy Qualifier Id=User Notice");

				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Qualifier:");

				// Notice Reference
				USERNOTICE *un;
				un = pq->d.usernotice;
				NOTICEREF *unref;
				unref = un->noticeref;

				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Notice Reference:");

				memset(szTmp, 0, sizeof(szTmp));
				memcpy(szTmp, unref->organization->data, unref->organization->length);
				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Organization=");
				strcat(out, szTmp);

				int k;
				long lNN;
				STACK_OF(ASN1_INTEGER) *nns;
				nns = unref->noticenos;
				for (k = 0; k < sk_ASN1_INTEGER_num(nns); k++)
				{
					lNN = ASN1_INTEGER_get(sk_ASN1_INTEGER_value(nns, k));

					memset(szTmp, 0, sizeof(szTmp));
					sprintf(szTmp, "Notice Number=%d", lNN);
					strcat(out, SGD_STR_RETURN);
					strcat(out, szSpace);
					strcat(out, szSpace);
					strcat(out, szSpace);
					strcat(out, szSpace);
					strcat(out, szTmp);
				}

				// Notice Text
				memset(szTmp, 0, sizeof(szTmp));
				memcpy(szTmp, un->exptext->data, un->exptext->length);
				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Notice Text=");
				strcat(out, szTmp);
				break;

			default:
				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Unknown Policy Qualifier");
				break;
			}
		}
	}

	CERTIFICATEPOLICIES_free(cps);

	if (strlen(out) == 0)
		return SAR_ObjNotFountErr;
	*outlen = strlen(out);

	return SAR_OK;
}

long MYTOOL_GetExtPolicyMappings(unsigned char *cert, long certlen, char *out, long *outlen)
{
	return SAR_PARAMETERNOTSUPPORTErr;
}

long MYTOOL_GetExtBasicConstraints(unsigned char *cert, long certlen, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	BASIC_CONSTRAINTS *ext = (BASIC_CONSTRAINTS *)X509_get_ext_d2i(x509, NID_basic_constraints, NULL, NULL);
	X509_free(x509);
	if (ext == NULL)
		return SAR_ObjNotFountErr;

	if (ext->ca != 0)
		strcpy(out, "Subject Type=CA");
	else
		strcpy(out, "Subject Type=End Entity");

	strcat(out, SGD_STR_RETURN);
	strcat(out, "Path Length Constraint=");
	long lPathLen = -1;
	if (ext->pathlen != NULL)
	{
		if ((ext->pathlen->type == V_ASN1_NEG_INTEGER) || !ext->ca)
			lPathLen = 0;
		else
			lPathLen = ASN1_INTEGER_get(ext->pathlen);
	}
	if (lPathLen != -1)
	{
		char szTmp[10];
		memset(szTmp, 0, sizeof(szTmp));
		sprintf(szTmp, "%d", lPathLen);
		strcat(out, szTmp);
	}
	else
		strcat(out, "None");
	*outlen = strlen(out);

	BASIC_CONSTRAINTS_free(ext);

	return SAR_OK;
}

long MYTOOL_GetExtPolicyConstraints(unsigned char *cert, long certlen, char *out, long *outlen)
{
	return SAR_PARAMETERNOTSUPPORTErr;
}

long MYTOOL_GetExtExtKeyUsage(unsigned char *cert, long certlen, DWORD *extkeyusage, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	EXTENDED_KEY_USAGE *ext = (EXTENDED_KEY_USAGE *)X509_get_ext_d2i(x509, NID_ext_key_usage, NULL, NULL);
	X509_free(x509);
	if (ext == NULL)
		return SAR_ObjNotFountErr;

	int nRet;
	DWORD dwTmp = 0, dwExtKeyUsage = 0;
	char szTmp[512];

	strcpy(out, "");
	for (int i = 0; i < sk_ASN1_OBJECT_num(ext); i++)
	{
		if (i > 0)
			strcat(out, SGD_STR_RETURN);

		dwTmp = 0;
		switch (OBJ_obj2nid(sk_ASN1_OBJECT_value(ext, i)))
		{
		case NID_server_auth:
			dwTmp = XKU_SSL_SERVER;
//			strcat(out, "TLS Web Server Authentication");
			break;
		case NID_client_auth:
			dwTmp = XKU_SSL_CLIENT;
//			strcat(out, "TLS Web Client Authentication");
			break;
		case NID_email_protect:
			dwTmp = XKU_SMIME;
//			strcat(out, "E-mail Protection");
			break;
		case NID_code_sign:
			dwTmp = XKU_CODE_SIGN;
//			strcat(out, "Code Signing");
			break;
		case NID_ms_sgc:
			dwTmp = XKU_SGC;
//			strcat(out, "Microsoft Server Gated Crypto");
			break;
		case NID_ns_sgc:
			dwTmp = XKU_SGC;
//			strcat(out, "Netscape Server Gated Crypto");
			break;
		case NID_OCSP_sign:
			dwTmp = XKU_OCSP_SIGN;
//			strcat(out, "OCSP Signing");
			break;
		case NID_time_stamp:
			dwTmp = XKU_TIMESTAMP;
//			strcat(out, "Time Stamping");
			break;
		case NID_dvcs:
			dwTmp = XKU_DVCS;
//			strcat(out, "dvcs");
			break;
		default:
//			strcat(out, "未知密钥用法");
			break;
		}
		dwExtKeyUsage |= dwTmp;

		memset(szTmp, 0, sizeof(szTmp));
		if (dwTmp != 0)
		{
			// 最后一个参数no_name为0，若对象有长名（LN）或短名（SN），则使用长名或短名；否则使用数字形式。
			// no_name为1，则使用数字形式。
			OBJ_obj2txt(szTmp, sizeof(szTmp), sk_ASN1_OBJECT_value(ext, i), 0);
		}
		if (strlen(szTmp) > 0)
			strcat(out, szTmp);
		else
			strcat(out, "未知密钥用法");

		memset(szTmp, 0, sizeof(szTmp));
		nRet = OBJ_obj2txt(szTmp, sizeof(szTmp), sk_ASN1_OBJECT_value(ext, i), 1);
		if (nRet > 0)
		{
			strcat(out, " (");
			strcat(out, szTmp);
			strcat(out, ")");
		}
	}

//	sk_ASN1_OBJECT_pop_free(ext, ASN1_OBJECT_free);
	EXTENDED_KEY_USAGE_free(ext);

	if (strlen(out) == 0)
		return SAR_ObjNotFountErr;
	*outlen = strlen(out);
	*extkeyusage = dwExtKeyUsage;

	return SAR_OK;
}

long MYTOOL_GetExtCrlDistributionPoints(unsigned char *cert, long certlen, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	CRL_DIST_POINTS *ds = (CRL_DIST_POINTS *)X509_get_ext_d2i(x509, NID_crl_distribution_points, NULL, NULL);
	X509_free(x509);
	if (ds == NULL)
		return SAR_ObjNotFountErr;

	int ncount = 0;
	char szTmp[512], szSpace[10];
	strcpy(szSpace, "     ");
	DIST_POINT *d;
	GENERAL_NAMES *gns;
	GENERAL_NAME *gn;

	strcpy(out, "");
	for (int i = 0; i < sk_DIST_POINT_num(ds); i++)
	{
		d = sk_DIST_POINT_value(ds, i);
		if (d == NULL || d->distpoint == NULL)
			continue;
		gns = d->distpoint->name.fullname;
		if (gns == NULL)
			continue;

		ncount++;
		if (ncount > 1)
			strcat(out, SGD_STR_RETURN);
		sprintf(szTmp, "\
[%d]CRL Distribution Point%s\
%sDistribution Point Name:%s\
%s%sFull Name:", ncount, SGD_STR_RETURN, szSpace, SGD_STR_RETURN, szSpace, szSpace);
		strcat(out, szTmp);

		for (int j = 0; j < sk_GENERAL_NAME_num(gns); j++)
		{
			gn = sk_GENERAL_NAME_value(gns, j);
			if (gn == NULL)
				continue;

			if (gn->type == GEN_DIRNAME)
			{
				memset(szTmp, 0, sizeof(szTmp));
				if (!mycertname2string(SGD_STR_SEPARATOR2, gn->d.directoryName, szTmp))
					continue;

				// 兼容linux
//				CString strTmp = SGD_STR_SEPARATOR2;
				String strTmp = SGD_STR_SEPARATOR2;
				strTmp += szTmp;
				strcpy(szTmp, SGD_STR_RETURN);
				strcat(szTmp, szSpace);
				strcat(szTmp, szSpace);
				strcat(szTmp, szSpace);
				strcat(szTmp, szSpace);
//				strTmp.Replace(SGD_STR_SEPARATOR2, szTmp);
				String::size_type pos;
				while ((pos = strTmp.find(SGD_STR_SEPARATOR2)) != String::npos)
					strTmp.replace(pos, strlen(SGD_STR_SEPARATOR2), szTmp);

				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "Directory Address:");
//				strcat(out, (LPCTSTR)strTmp);
				strcat(out, strTmp.c_str());
			}
			else if (gn->type == GEN_URI)
			{
				memset(szTmp, 0, sizeof(szTmp));
				memcpy(szTmp, ASN1_STRING_data(gn->d.uniformResourceIdentifier), ASN1_STRING_length(gn->d.uniformResourceIdentifier));

				strcat(out, SGD_STR_RETURN);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, szSpace);
				strcat(out, "URL=");
				strcat(out, szTmp);
			}
		}
	}

//	sk_DIST_POINT_pop_free(ds, DIST_POINT_free);
	CRL_DIST_POINTS_free(ds);

	if (strlen(out) == 0)
		return SAR_ObjNotFountErr;
	*outlen = strlen(out);

	return SAR_OK;
}

long MYTOOL_GetExtNetScapeCertType(unsigned char *cert, long certlen, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	ASN1_BIT_STRING *ext = (ASN1_BIT_STRING *)X509_get_ext_d2i(x509, NID_netscape_cert_type, NULL, NULL);
	X509_free(x509);
	if (ext == NULL)
		return SAR_ObjNotFountErr;

	// Netscape证书类型值
	DWORD dwNCertType = 0;
	if (ext->length > 0)
		dwNCertType = ext->data[0];
	ASN1_BIT_STRING_free(ext);

	// Netscape证书类型字符串
	strcpy(out, "");
	if (dwNCertType & NS_SSL_CLIENT)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "SSL Client");
	}
	if (dwNCertType & NS_SSL_SERVER)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "SSL Server");
	}
	if (dwNCertType & NS_SMIME)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "S/MIME");
	}
	if (dwNCertType & NS_OBJSIGN)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Object Signing");
	}
	if (dwNCertType & NS_SSL_CA)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "SSL CA");
	}
	if (dwNCertType & NS_SMIME_CA)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "S/MIME CA");
	}
	if (dwNCertType & NS_OBJSIGN_CA)
	{
		if (strlen(out) > 0)
			strcat(out, ", ");
		strcat(out, "Object Signing CA");
	}
	if (strlen(out) == 0)
		strcpy(out, "Undefined");

	char szTmp[10];
	sprintf(szTmp, " (%02x)", dwNCertType);
	strcat(out, szTmp);
	*outlen = strlen(out);

	return SAR_OK;
}

long MYTOOL_GetExtAuthorityInfoAccess(unsigned char *cert, long certlen, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	AUTHORITY_INFO_ACCESS *ds = (AUTHORITY_INFO_ACCESS *)X509_get_ext_d2i(x509, NID_info_access, NULL, NULL);
	X509_free(x509);
	if (ds == NULL)
		return SAR_ObjNotFountErr;

	int ncount = 0;
	int nRet;
	char szTmp[512], szSpace[10];
	strcpy(szSpace, "     ");
	ACCESS_DESCRIPTION *d;
	GENERAL_NAME *gn;

	strcpy(out, "");
	for (int i = 0; i < sk_ACCESS_DESCRIPTION_num(ds); i++)
	{
		d = sk_ACCESS_DESCRIPTION_value(ds, i);
		if (d == NULL || d->method == NULL)
			continue;

		ncount++;
		if (ncount > 1)
			strcat(out, SGD_STR_RETURN);
		sprintf(szTmp, "\
[%d]Authority Info Access", ncount);
		strcat(out, szTmp);

		// 1 access method
		strcat(out, SGD_STR_RETURN);
		strcat(out, szSpace);
		strcat(out, "Access Method=");

		memset(szTmp, 0, sizeof(szTmp));
		// 最后一个参数no_name为0，若对象有长名（LN）或短名（SN），则使用长名或短名；否则使用数字形式。
		// no_name为1，则使用数字形式。
		OBJ_obj2txt(szTmp, sizeof(szTmp), d->method, 0);
		if (strlen(szTmp) > 0)
			strcat(out, szTmp);
		else
			strcat(out, "未知");

		memset(szTmp, 0, sizeof(szTmp));
		nRet = OBJ_obj2txt(szTmp, sizeof(szTmp), d->method, 1);
		if (nRet > 0)
		{
			strcat(out, " (");
			strcat(out, szTmp);
			strcat(out, ")");
		}

		// 2 alternative name
		gn = d->location;
		if (gn == NULL)
			continue;
		
		strcat(out, SGD_STR_RETURN);
		strcat(out, szSpace);
		strcat(out, "Alternative Name:");
		if (gn->type == GEN_DIRNAME)
		{
			memset(szTmp, 0, sizeof(szTmp));
			if (!mycertname2string(SGD_STR_SEPARATOR2, gn->d.directoryName, szTmp))
				continue;

			// 兼容linux
//			CString strTmp = SGD_STR_SEPARATOR2;
			String strTmp = SGD_STR_SEPARATOR2;
			strTmp += szTmp;
			strcpy(szTmp, SGD_STR_RETURN);
			strcat(szTmp, szSpace);
			strcat(szTmp, szSpace);
			strcat(szTmp, szSpace);
//			strTmp.Replace(SGD_STR_SEPARATOR2, szTmp);
			String::size_type pos;
			while ((pos = strTmp.find(SGD_STR_SEPARATOR2)) != String::npos)
				strTmp.replace(pos, strlen(SGD_STR_SEPARATOR2), szTmp);

			strcat(out, SGD_STR_RETURN);
			strcat(out, szSpace);
			strcat(out, szSpace);
			strcat(out, "Directory Address:");
//			strcat(out, (LPCTSTR)strTmp);
			strcat(out, strTmp.c_str());
		}
		else if (gn->type == GEN_URI)
		{
			memset(szTmp, 0, sizeof(szTmp));
			memcpy(szTmp, ASN1_STRING_data(gn->d.uniformResourceIdentifier), ASN1_STRING_length(gn->d.uniformResourceIdentifier));

			strcat(out, SGD_STR_RETURN);
			strcat(out, szSpace);
			strcat(out, szSpace);
			strcat(out, "URL=");
			strcat(out, szTmp);
		}
	}

//	sk_ACCESS_DESCRIPTION_pop_free(ds, ACCESS_DESCRIPTION_free);
	AUTHORITY_INFO_ACCESS_free(ds);

	if (strlen(out) == 0)
		return SAR_ObjNotFountErr;
	*outlen = strlen(out);

	return SAR_OK;
}

long MYTOOL_GetExtByOid(unsigned char *cert, long certlen, char *oid, char *out, long *outlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	int nExtCount = X509_get_ext_count(x509);
	if (nExtCount <= 0)
	{
		X509_free(x509);
		return SAR_ObjNotFountErr;
	}

	// 兼容linux
//	USES_CONVERSION;
//	setlocale(LC_CTYPE, "");
//	char asndata2[512];
//	wchar_t wdata[512];
	long n;

	int asnlen, asntype;
	char szName[512], szValue[512];
	char *asndata;
	X509_EXTENSION *ext;
	ASN1_OBJECT *obj;
	ASN1_OCTET_STRING *data;
	ASN1_PRINTABLESTRING *prtstr;

	for (int i = 0; i < nExtCount; i++)
	{
		memset(szName, 0, sizeof(szName));
		memset(szValue, 0, sizeof(szValue));

		ext = X509_get_ext(x509, i);
		if (ext == NULL)
			continue;

		obj = X509_EXTENSION_get_object(ext);
		data = X509_EXTENSION_get_data(ext);

		OBJ_obj2txt(szName, sizeof(szName), obj, 1);
		if (strcmp(szName, oid) == 0)
		{
			asndata = (char *)ASN1_STRING_data(data);
			asnlen = ASN1_STRING_length(data);
			asntype = ASN1_STRING_type(data);

			if (asntype == V_ASN1_BMPSTRING)
			{
				// 兼容linux
/*				memset(asndata2, 0, sizeof(asndata2));
				for (int j = 0; j < asnlen / 2; j++)
				{
					asndata2[2 * j + 1] = *asndata++;
					asndata2[2 * j] = *asndata++;
				}
				strcpy(szValue, W2A((PWSTR)asndata2));
*/				n = mybmpstr2str(asndata, asnlen, szValue);
				if (n <= 0)
					return SAR_CERTENCODEErr;
			}
			else if (asntype == V_ASN1_UTF8STRING)
			{
				// 兼容linux
/*				memset(wdata, 0, sizeof(wdata));
				int wdatalen = MultiByteToWideChar(
					CP_UTF8,
					0,
					asndata,
					asnlen,
					wdata,
					512);
				if (wdatalen <= 0)
					return FALSE;
				int datalen = WideCharToMultiByte(
					CP_ACP,
					0,
					wdata,
					wdatalen,
					szValue,
					512,
					NULL,
					NULL);
				if (datalen <= 0)
					return FALSE;
*/				n = myutf8str2str(asndata, asnlen, szValue);
				if (n <= 0)
					return SAR_CERTENCODEErr;
			}
			else if (*asndata == V_ASN1_PRINTABLESTRING)
			{
//				memcpy(szValue, asndata + 2, asnlen - 2);
				pszTmp = (unsigned char *)asndata;
				prtstr = d2i_ASN1_PRINTABLESTRING(NULL, (const unsigned char **)&pszTmp, asnlen);
				memcpy(szValue, prtstr->data, prtstr->length);
				ASN1_PRINTABLESTRING_free(prtstr);
			}
			else
				memcpy(szValue, asndata, asnlen);

			break;
		}
	}

	X509_free(x509);

	if (strlen(szValue) == 0)
		return SAR_ObjNotFountErr;
	strcpy(out, szValue);
	*outlen = strlen(out);

	return SAR_OK;
}

// 4
long MYTOOL_HashData(unsigned char *in, long inlen, unsigned char *out, long *outlen, long hashmethod)
{
	long lHashMethod = hashmethod;
	switch (lHashMethod)
	{
	case SGD_SM3:
		break;
	case SGD_SHA1:
		break;
	case SGD_SHA256:
		break;
	default:
		return SAR_AlgoTypeErr;
	}

	if (lHashMethod == SGD_SHA1)
	{
		SHA_CTX context;
		SHA1_Init(&context);
		SHA1_Update(&context, (const unsigned char *)in, inlen);
		SHA1_Final(out, &context);
		*outlen = 20;
	}
	else if (lHashMethod == SGD_SHA256)
	{
		SHA256_CTX context256;
		SHA256_Init(&context256);
		SHA256_Update(&context256, (const unsigned char *)in, inlen);
		SHA256_Final(out, &context256);
		*outlen = 32;
	}
	else
	{
		long lRet = MYSM_SM3(
			in,
			inlen,
			out,
			outlen);
		if (lRet != MYSMERR_SUCCESS)
			return SAR_HashErr;
	}

	return SAR_OK;
}

long MYTOOL_HashFile(char *infile, unsigned char *out, long *outlen, long hashmethod)
{
	long lHashMethod = hashmethod;
	switch (lHashMethod)
	{
	case SGD_SM3:
		break;
	case SGD_SHA1:
		break;
	case SGD_SHA256:
		break;
	default:
		return SAR_AlgoTypeErr;
	}

	FILE *f = fopen(infile, "rb");
	if (f == NULL)
		return SAR_FileErr;
	fseek(f, 0, SEEK_END);
	long lLen = ftell(f);
	if (lLen == 0)
	{
		fclose(f);
		return SAR_FileErr;
	}
	fseek(f, 0, SEEK_SET);

	// init
	long lRet;
	SHA_CTX context;
	SHA256_CTX context256;
	HANDLE hHandle;
	if (lHashMethod == SGD_SHA1)
		SHA1_Init(&context);
	else if (lHashMethod == SGD_SHA256)
		SHA256_Init(&context256);
	else
	{
		lRet = MYSM_SM3Init(&hHandle);
		if (lRet != MYSMERR_SUCCESS)
		{
			fclose(f);
			return SAR_HashErr;
		}
	}

	unsigned char byTmp[SGD_SIZE_SM3HASH];
	memset(byTmp, 0, sizeof(byTmp));
	long lTmpLen;

	unsigned char byData[SGD_SIZE_DATABLOCK];
	while (!feof(f))
	{
		memset(byData, 0, sizeof(byData));
		lLen = fread(byData, 1, SGD_SIZE_DATABLOCK, f);
		if (ferror(f))
		{
			fclose(f);
			if (lHashMethod == SGD_SM3)
				MYSM_SM3Final(hHandle, byTmp, &lTmpLen);
			return SAR_FileErr;
		}

		// update
		if (lLen > 0)
		{
			if (lHashMethod == SGD_SHA1)
				SHA1_Update(&context, byData, lLen);
			else if (lHashMethod == SGD_SHA256)
				SHA256_Update(&context256, byData, lLen);
			else
			{
				lRet = MYSM_SM3Update(hHandle, byData, lLen);
				if (lRet != MYSMERR_SUCCESS)
				{
					fclose(f);
					return SAR_HashErr;
				}
			}
		}
	}
	fclose(f);

	if (lHashMethod == SGD_SHA1)
	{
		SHA1_Final(out, &context);
		*outlen = 20;
	}
	else if (lHashMethod == SGD_SHA256)
	{
		SHA256_Final(out, &context256);
		*outlen = 32;
	}
	else
	{
		lRet = MYSM_SM3Final(hHandle, out, outlen);
		if (lRet != MYSMERR_SUCCESS)
			return SAR_HashErr;
	}

	return SAR_OK;
}

long MYTOOL_FormDigestInfo(unsigned char *in, long inlen, unsigned char *out, long *outlen, long hashmethod)
{
	long lHashMethod = hashmethod;
	switch (lHashMethod)
	{
	case SGD_SM3:
		return SAR_PARAMETERNOTSUPPORTErr;
	case SGD_SHA1:
		break;
	case SGD_SHA256:
		break;
	default:
		return SAR_AlgoTypeErr;
	}

	ASN1_TYPE parameter;
	parameter.type = V_ASN1_NULL;
	parameter.value.ptr = NULL;
	X509_ALGOR algor;
	if (lHashMethod == SGD_SHA1)
		algor.algorithm = OBJ_nid2obj(NID_sha1);
	else
		algor.algorithm = OBJ_nid2obj(NID_sha256);
	algor.parameter = &parameter;

	ASN1_OCTET_STRING *digest;
	digest = ASN1_OCTET_STRING_new();
	if (!digest)
		return SAR_MemoryErr;
	ASN1_OCTET_STRING_set(digest, in, inlen);

	X509_SIG sig;
	sig.algor = &algor;
	sig.digest = digest;

	BYTE *pbOut = out;
	*outlen = i2d_X509_SIG(&sig, &pbOut);
	ASN1_OCTET_STRING_free(digest);
	if (*outlen <= 0)
		return SAR_HashErr;

	return SAR_OK;
}

long MYTOOL_CompareDigest(unsigned char *in, long inlen, unsigned char *digestinfo, long digestinfolen, KTBOOL32 data)
{
	// 解析摘要信息数据
	unsigned char *pszTmp = digestinfo;
	X509_SIG *sig = d2i_X509_SIG(NULL, (const unsigned char **)&pszTmp, digestinfolen);
	if (sig == NULL)
		return SAR_HashErr;

	if (sig->algor->parameter && ASN1_TYPE_get(sig->algor->parameter) != V_ASN1_NULL)
	{
		X509_SIG_free(sig);
		return SAR_HashErr;
	}

	int sigtype = OBJ_obj2nid(sig->algor->algorithm);
	if (sigtype != NID_sha1 && sigtype != NID_sha256)
	{
		X509_SIG_free(sig);
		return SAR_PARAMETERNOTSUPPORTErr;
	}

	long lHashMethod = SGD_SHA1;
	if (sigtype == NID_sha256)
		lHashMethod = SGD_SHA256;

	// 计算原文摘要
	long lDigestLen = 128;
	unsigned char byDigest[128];
	memset(byDigest, 0, sizeof(byDigest));
	long lRet;
	if (data)
		lRet = MYTOOL_HashData((unsigned char *)in, inlen, byDigest, &lDigestLen, lHashMethod);
	else
		lRet = MYTOOL_HashFile((char *)in, byDigest, &lDigestLen, lHashMethod);
	if (lRet != SAR_OK)
	{
		X509_SIG_free(sig);
		return lRet;
	}

	// 比较摘要数据
	if (sig->digest->length != lDigestLen ||
		memcmp(sig->digest->data, byDigest, lDigestLen) != 0)
	{
		X509_SIG_free(sig);
		return SAR_HashNotEqualErr;
	}

	X509_SIG_free(sig);

	return SAR_OK;
}

KTBOOL32 MYTOOL_IsECCCert(unsigned char *cert, long certlen)
{
	unsigned char byPubKey_x[256], byPubKey_y[256];
	memset(byPubKey_x, 0, sizeof(byPubKey_x));
	memset(byPubKey_y, 0, sizeof(byPubKey_y));
	long lPubKeyLen_x, lPubKeyLen_y;
	long lRet = MYSM_ECCGetPubKeyFromCert(
		cert,
		certlen,
		byPubKey_x,
		&lPubKeyLen_x,
		byPubKey_y,
		&lPubKeyLen_y);
	if (lRet != MYSMERR_SUCCESS)
		return FALSE;
	else
		return TRUE;
}

long MYTOOL_ECCGetPubKeyFromCert(unsigned char *cert, long certlen, unsigned char *pubkey_x, long *pubkeylen_x, unsigned char *pubkey_y, long *pubkeylen_y)
{
	unsigned char byPubKey_x[256], byPubKey_y[256];
	memset(byPubKey_x, 0, sizeof(byPubKey_x));
	memset(byPubKey_y, 0, sizeof(byPubKey_y));
	long lPubKeyLen_x, lPubKeyLen_y;
	long lRet = MYSM_ECCGetPubKeyFromCert(
		cert,
		certlen,
		byPubKey_x,
		&lPubKeyLen_x,
		byPubKey_y,
		&lPubKeyLen_y);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCCERTENCODEErr;

	memcpy(pubkey_x, byPubKey_x, lPubKeyLen_x);
	*pubkeylen_x = lPubKeyLen_x;
	memcpy(pubkey_y, byPubKey_y, lPubKeyLen_y);
	*pubkeylen_y = lPubKeyLen_y;

	return SAR_OK;
}

// 预处理1
// Z = SM3(ENTL‖ID‖a‖b‖Gx‖Gy‖Px‖Py)
// 256bits
long MYTOOL_ECCGetZ(unsigned char *userid, long useridlen, unsigned char *pubkey_x, unsigned char *pubkey_y, unsigned char *out, long *outlen, long ecclen)
{
	long lRet = MYSM_ECCGetZ(
		userid,
		useridlen,
		pubkey_x,
		pubkey_y,
		out,
		outlen,
		ecclen);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCOPERErr;

	return SAR_OK;
}

// 预处理2
// H = SM3(Z‖M)
long MYTOOL_ECCGetH(unsigned char *z, long zlen, unsigned char *in, long inlen, unsigned char *out, long *outlen)
{
	long lRet = MYSM_ECCGetH(
		z,
		zlen,
		in,
		inlen,
		out,
		outlen);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCOPERErr;

	return SAR_OK;
}

// 预处理2
// H = SM3(Z‖M)
long MYTOOL_ECCGetFileH(unsigned char *z, long zlen, char *infile, unsigned char *out, long *outlen)
{
	long lRet = MYSM_ECCGetFileH(
		z,
		zlen,
		infile,
		out,
		outlen);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCOPERErr;

	return SAR_OK;
}

long MYTOOL_RSAEncryptWithCert(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *cert, long certlen, KTBOOL32 encrypt)
{
	// 解析证书数据
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	// 取公钥
	EVP_PKEY *pkey = X509_get_pubkey(x509);
	X509_free(x509);
	if (pkey == NULL)
		return SAR_CERTENCODEErr;

	RSA *rsa = EVP_PKEY_get1_RSA(pkey);
	EVP_PKEY_free(pkey);
	if (!rsa)
		return SAR_CERTENCODEErr;

	// 加密/解密
	int nRet;
	if (encrypt)
		nRet = RSA_public_encrypt(inlen, in, out, rsa, RSA_PKCS1_PADDING);
	else
		nRet = RSA_public_decrypt(inlen, in, out, rsa, RSA_PKCS1_PADDING);
	RSA_free(rsa);
	if (nRet <= 0)
	{
		if (encrypt)
			return SAR_RsaEncErr;
		else
			return SAR_RSGDecErr;
	}
	*outlen = nRet;

	return SAR_OK;
}

long MYTOOL_ECCEncryptWithCert(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *cert, long certlen, KTBOOL32 encrypt)
{
	if (!encrypt)
		return SAR_ECCDECErr;

	unsigned char byPubKey_x[256], byPubKey_y[256];
	memset(byPubKey_x, 0, sizeof(byPubKey_x));
	memset(byPubKey_y, 0, sizeof(byPubKey_y));
	long lPubKeyLen_x, lPubKeyLen_y;
	long lRet = MYSM_ECCGetPubKeyFromCert(
		cert,
		certlen,
		byPubKey_x,
		&lPubKeyLen_x,
		byPubKey_y,
		&lPubKeyLen_y);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCCERTENCODEErr;
	if ((lPubKeyLen_x != lPubKeyLen_y) ||
		(lPubKeyLen_x != SGD_SIZE_ECC && lPubKeyLen_x != SGD_SIZE_ECC_64))
		return SAR_ECCCERTENCODEErr;
	int nSM2Len = lPubKeyLen_x;

	BYTE byC1[1024], byC3[1024];
	memset(byC1, 0, sizeof(byC1));
	memset(byC3, 0, sizeof(byC3));
	ByteArray rC2(inlen);
	memset(&rC2[0], 0, inlen);
	lRet = MYSM_ECCEncrypt(
		in,
		inlen,
		byC1,
		&rC2[0],
		byC3,
		byPubKey_x,
		byPubKey_y,
		nSM2Len);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCENCErr;

// 	long lOutLen = 0;
// 	*((DWORD *)out) = nSM2Len * 2;
// 	lOutLen += 4;
// 	memcpy(out + lOutLen, byC1, nSM2Len * 2);
// 	lOutLen += nSM2Len * 2;
// 	*((DWORD *)(out + lOutLen)) = inlen;
// 	lOutLen += 4;
// 	memcpy(out + lOutLen, &rC2[0], inlen);
// 	lOutLen += inlen;
// 	*((DWORD *)(out + lOutLen)) = SGD_SIZE_SM3HASH;
// 	lOutLen += 4;
// 	memcpy(out + lOutLen, byC3, SGD_SIZE_SM3HASH);
// 	lOutLen += SGD_SIZE_SM3HASH;

	// output data
	lRet = MYTOOL_SM2CipherDataEncode(
		byC1,
		nSM2Len * 2,
		&rC2[0],
		inlen,
		byC3,
		SGD_SIZE_SM3HASH,
		out,
		outlen);
	if (lRet != SAR_OK)
		return lRet;

	return SAR_OK;
}

long MYTOOL_EncryptWithCert(unsigned char *in, long inlen, unsigned char *out, long *outlen, unsigned char *cert, long certlen, KTBOOL32 encrypt)
{
	KTBOOL32 bIsSM2Cert = MYTOOL_IsECCCert(cert, certlen);
	if (!bIsSM2Cert)
	{
		// RSA
		return MYTOOL_RSAEncryptWithCert(
			in,
			inlen,
			out,
			outlen,
			cert,
			certlen,
			encrypt);
	}
	else
	{
		// ECC
		return MYTOOL_ECCEncryptWithCert(
			in,
			inlen,
			out,
			outlen,
			cert,
			certlen,
			encrypt);
	}
}

long MYTOOL_RSAVerifySignedData(unsigned char *in, long inlen, unsigned char *sign, long signlen, unsigned char *cert, long certlen, KTBOOL32 data)
{
	// 证书解密
	unsigned char byDigestInfo[SGD_SIZE_RSADATA];
	memset(byDigestInfo, 0, sizeof(byDigestInfo));
	long lDigestInfoLen = SGD_SIZE_RSADATA;
	long lRet = MYTOOL_RSAEncryptWithCert(
		sign,
		signlen,
		byDigestInfo,
		&lDigestInfoLen,
		cert,
		certlen,
		FALSE);
	if (lRet != SAR_OK)
		return lRet;

	// 比较摘要
	lRet = MYTOOL_CompareDigest(
		in,
		inlen,
		byDigestInfo,
		lDigestInfoLen,
		data);
	if (lRet != SAR_OK)
		return lRet;

	return SAR_OK;
}

long MYTOOL_ECCVerifySignedData(unsigned char *in, long inlen, unsigned char *sign, long signlen, unsigned char *cert, long certlen, KTBOOL32 data)
{
// 	if (signlen != SGD_SIZE_ECC * 2)
// 		return SAR_ECCVERIFYSIGNErr;

	unsigned char byPubKey_x[256], byPubKey_y[256];
	memset(byPubKey_x, 0, sizeof(byPubKey_x));
	memset(byPubKey_y, 0, sizeof(byPubKey_y));
	long lPubKeyLen_x, lPubKeyLen_y;
	long lRet = MYSM_ECCGetPubKeyFromCert(
		cert,
		certlen,
		byPubKey_x,
		&lPubKeyLen_x,
		byPubKey_y,
		&lPubKeyLen_y);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCCERTENCODEErr;
	if ((lPubKeyLen_x != lPubKeyLen_y) ||
		(lPubKeyLen_x != SGD_SIZE_ECC && lPubKeyLen_x != SGD_SIZE_ECC_64))
		return SAR_ECCCERTENCODEErr;
	int nSM2Len = lPubKeyLen_x;

	// user id
	char szUserID[32];
	memset(szUserID, 0, sizeof(szUserID));
	strcpy(szUserID, SGD_STR_ECCUSERID);
	long lUserIDLen = strlen(szUserID);

	// 预处理1
	BYTE byZ[128];
	memset(byZ, 0, sizeof(byZ));
	long lZLen = 128;
	lRet = MYSM_ECCGetZ(
		(BYTE *)szUserID,
		lUserIDLen,
		byPubKey_x,
		byPubKey_y,
		byZ,
		&lZLen,
		nSM2Len);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCOPERErr;

	// 预处理2
	BYTE byH[128];
	memset(byH, 0, sizeof(byH));
	long lHLen = 128;
	if (data)
		lRet = MYSM_ECCGetH(
			byZ,
			lZLen,
			in,
			inlen,
			byH,
			&lHLen);
	else
		lRet = MYSM_ECCGetFileH(
			byZ,
			lZLen,
			(char *)in,
			byH,
			&lHLen);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCOPERErr;

	// verify sign
	unsigned char bySign_r[256], bySign_s[256];
	memset(bySign_r, 0, sizeof(bySign_r));
	memset(bySign_s, 0, sizeof(bySign_s));
	long lRLen = sizeof(bySign_r), lSLen = sizeof(bySign_s);
	lRet = MYTOOL_SM2SignDataDecode(
		sign,
		signlen,
		bySign_r,
		&lRLen,
		bySign_s,
		&lSLen);
	if (lRet != SAR_OK)
		return lRet;
	if (lRLen != nSM2Len || lSLen != nSM2Len)
 		return SAR_ECCVERIFYSIGNErr;

	lRet = MYSM_ECCVerifySign(
		byH,
		lHLen,
		bySign_r,
		bySign_s,
		byPubKey_x,
		byPubKey_y,
		nSM2Len);
	if (lRet != MYSMERR_SUCCESS)
		return SAR_ECCVERIFYSIGNErr;

	return SAR_OK;
}

long MYTOOL_VerifySignedData(unsigned char *in, long inlen, unsigned char *sign, long signlen, unsigned char *cert, long certlen, KTBOOL32 data)
{
	KTBOOL32 bIsSM2Cert = MYTOOL_IsECCCert(cert, certlen);
	if (!bIsSM2Cert)
	{
		// RSA
		return MYTOOL_RSAVerifySignedData(
			in,
			inlen,
			sign,
			signlen,
			cert,
			certlen,
			data);
	}
	else
	{
		// ECC
		return MYTOOL_ECCVerifySignedData(
			in,
			inlen,
			sign,
			signlen,
			cert,
			certlen,
			data);
	}
}

// 5
// 连接服务器
#define TIMEOUTOFCONNECTION				5					// 网络连接超时等待时间，5秒
#define TIMEOUTOFCOMMUNICATION			5					// 网络通信超时等待时间，30秒
SOCKET MYTOOL_SocketConnect(char *pszServerIP, WORD wPort)
{
	unsigned long ulServerIP = inet_addr(pszServerIP);
	if (ulServerIP == INADDR_NONE)
		return INVALID_SOCKET;

	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return INVALID_SOCKET;

	// linger
// 	KTBOOL32 bDontLinger = FALSE;
// 	if (setsockopt(sock, SOL_SOCKET, SO_DONTLINGER, (char *)&bDontLinger, sizeof(bDontLinger)) == SOCKET_ERROR)
// 	{
// 		closesocket(sock);
// 		return INVALID_SOCKET;
// 	}

	struct linger ling;
	ling.l_onoff = 1;
	ling.l_linger = 0;   
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling)) == SOCKET_ERROR)
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}

	// 连接
	SOCKADDR_IN ServerAddress;
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = ulServerIP;
    ServerAddress.sin_port = htons(wPort);

#ifdef _LINUX
	while (connect(sock, (SOCKADDR *)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
	{
		if (errno == EINTR)     // Interrupted by system signal
			continue;
		else
		{
			closesocket(sock);
			return INVALID_SOCKET;
		}
	}
#else
	// 设置非阻塞方式连接
	unsigned long ul = 1;
	if (ioctlsocket(sock, FIONBIO, (unsigned long *)&ul) == SOCKET_ERROR)
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}

	connect(sock, (SOCKADDR *)&ServerAddress, sizeof(ServerAddress));

	// select模型，设置超时
    // Add the source socket into the set
    fd_set	fset;
	FD_ZERO(&fset);
    FD_SET(sock, &fset);
	// Set the timeout value
	struct	timeval tval;
	tval.tv_sec = TIMEOUTOFCONNECTION;
	tval.tv_usec = 0;

	int nRet = select(0, NULL, &fset, NULL, &tval);
	if (nRet <= 0)
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}

	// 设回阻塞模式
	ul = 0 ;
	if (ioctlsocket(sock, FIONBIO, (unsigned long *)&ul) == SOCKET_ERROR)
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}
#endif

	return sock;
}

// Wait for send data.
int WaitForSendData(SOCKET sock)
{
	fd_set fset;
	FD_ZERO(&fset);
	FD_SET(sock, &fset);

	struct timeval tval;
	tval.tv_sec = TIMEOUTOFCOMMUNICATION;
	tval.tv_usec = 0;

	int nRet = select(sock + 1, NULL, &fset, NULL, &tval);
	if (nRet > 0 && FD_ISSET(sock, &fset))
		FD_CLR(sock, &fset);
	return nRet;
}

// Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
long MYTOOL_SocketSend(SOCKET sock, const void *pBuff, long lSize)
{
	int nRet = WaitForSendData(sock);
	if (nRet < 0)
		return -1;
	else if (nRet == 0)
		return 0;
	else
		return send(sock, (char *)pBuff, lSize, 0);
}

// Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
long MYTOOL_SocketSend_n(SOCKET sock, const void *pBuff, long lSize)
{
	long bt = 0;
	while (bt < lSize)
	{
		// Try to transfer as much of the remaining data as possible.
		int nRet = WaitForSendData(sock);
		if (nRet < 0)
			return -1;
		else if (nRet == 0)
			break; // timeout.

		long bytes_send = send(sock, (const char *)pBuff + bt, lSize - bt, 0);
		if (bytes_send > 0)
			bt += bytes_send;
		else
			return -1;
	}

	return bt;
}

// Wait for recv data.
int WaitForRecvData(SOCKET sock)
{
	fd_set fset;
	FD_ZERO(&fset);
	FD_SET(sock, &fset);
	
	struct timeval tval;
	tval.tv_sec = TIMEOUTOFCOMMUNICATION;
	tval.tv_usec = 0;

	int nRet = select(sock + 1, &fset, NULL, NULL, &tval);
	if (nRet > 0 && FD_ISSET(sock, &fset))
		FD_CLR(sock, &fset);
	return nRet;
}

// Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
long MYTOOL_SocketRecv(SOCKET sock, void *pBuff, long lSize)
{
	int nRet = WaitForRecvData(sock);
	if (nRet < 0)
		return -1;
	else if (nRet == 0)
		return 0;
	return recv(sock, (char *)pBuff, lSize, 0);
}

// Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
long MYTOOL_SocketRecv_n(SOCKET sock, void *pBuff, long lSize)
{
	long bt = 0;
	while (bt < lSize)
	{
		// Try to transfer as much of the remaining data as possible.
		int nRet = WaitForRecvData(sock);
		if (nRet < 0)
			return -1;
		else if (nRet == 0)
			break;

		long bytes_recv = recv(sock, (char *)pBuff + bt, lSize - bt, 0);
		if (bytes_recv > 0)
			bt += bytes_recv;
		else if (bytes_recv == 0)
			break;
		else
			return -1;
	}
	return bt;
}

// 6
//////////////////////////////////////////////////////////
//验证证书函数
//////////////////////////////////////////////////////////
#define FORMAT_UNDEF    0
#define FORMAT_ASN1     1
#define FORMAT_TEXT     2
#define FORMAT_PEM      3
#define FORMAT_NETSCAPE 4
#define FORMAT_PKCS12   5
#define FORMAT_SMIME    6
#define FORMAT_ENGINE   7
#define FORMAT_IISSGC	8	/* XXX this stupid macro helps us to avoid
				 * adding yet another param to load_*key() */
#define FORMAT_PEMRSA	9	/* PEM RSAPubicKey format */
#define FORMAT_ASN1RSA	10	/* DER RSAPubicKey format */
#define FORMAT_MSBLOB	11	/* MS Key blob format */
#define FORMAT_PVK	12	/* MS PVK file format */

static X509_CRL *load_crl(char *infile, int format)
	{
	X509_CRL *x=NULL;
	BIO *in=NULL;

	in=BIO_new(BIO_s_file());
	if (in == NULL)
		{
//		ERR_print_errors(bio_err);
		goto end;
		}

	if (infile == NULL)
		BIO_set_fp(in,stdin,BIO_NOCLOSE);
	else
		{
		if (BIO_read_filename(in,infile) <= 0)
			{
			perror(infile);
			goto end;
			}
		}
	if 	(format == FORMAT_ASN1)
		x=d2i_X509_CRL_bio(in,NULL);
	else if (format == FORMAT_PEM)
		x=PEM_read_bio_X509_CRL(in,NULL,NULL,NULL);
	else	{
//		BIO_printf(bio_err,"bad input format specified for input crl\n");
		goto end;
		}
	if (x == NULL)
		{
//		BIO_printf(bio_err,"unable to load CRL\n");
//		ERR_print_errors(bio_err);
		goto end;
		}
	
end:
	BIO_free(in);
	return(x);
	}

//X509_STORE *setup_verify(BIO *bp, char *CAfile, char *CApath)
X509_STORE *setup_verify(char *CAfile, char *CApath)
{
	X509_STORE *store;
	X509_LOOKUP *lookup;
	if(!(store = X509_STORE_new())) goto end;
	lookup=X509_STORE_add_lookup(store,X509_LOOKUP_file());
	if (lookup == NULL) goto end;
	if (CAfile) {
		if(!X509_LOOKUP_load_file(lookup,CAfile,X509_FILETYPE_PEM)) {
//			BIO_printf(bp, "Error loading file %s\n", CAfile);
			goto end;
		}
	} else X509_LOOKUP_load_file(lookup,NULL,X509_FILETYPE_DEFAULT);
		
	lookup=X509_STORE_add_lookup(store,X509_LOOKUP_hash_dir());
	if (lookup == NULL) goto end;
	if (CApath) {
		if(!X509_LOOKUP_add_dir(lookup,CApath,X509_FILETYPE_PEM)) {
//			BIO_printf(bp, "Error loading directory %s\n", CApath);
			goto end;
		}
	} else X509_LOOKUP_add_dir(lookup,NULL,X509_FILETYPE_DEFAULT);

	ERR_clear_error();
	return store;
	end:
	X509_STORE_free(store);
	return NULL;
}

long MYTOOL_VerifyCertSign(unsigned char *cert, int certlen, unsigned char *cacert, int cacertlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	char szTmp[128];
	memset(szTmp, 0, sizeof(szTmp));
	OBJ_obj2txt(szTmp, sizeof(szTmp), x509->sig_alg->algorithm, 0);
	KTBOOL32 bIsSM2Cert = FALSE;
	if (strstr(szTmp, "RSA") == NULL &&
		strstr(szTmp, "rsa") == NULL &&
		strstr(szTmp, "Rsa") == NULL)
		bIsSM2Cert = TRUE;

	if (!bIsSM2Cert)
	{
		pszTmp = cacert;
		X509 *x509ca = d2i_X509(NULL, (const unsigned char **)&pszTmp, cacertlen);
		if (x509ca == NULL)
		{
			X509_free(x509);
			return SAR_CERTENCODEErr;
		}

		EVP_PKEY *pkey = X509_get_pubkey(x509ca);
		if (pkey == NULL)
		{
			X509_free(x509);
			X509_free(x509ca);
			return SAR_CERTENCODEErr;
		}

		int nRet = X509_verify(x509, pkey);
		EVP_PKEY_free(pkey);
		X509_free(x509);
		X509_free(x509ca);
		if (nRet != 1)
			return SAR_CERTINVALIDErr;
	}
	else
	{
		// get cert info
		unsigned char byCertInfo[4096];
		memset(byCertInfo, 0, sizeof(byCertInfo));
		unsigned char *pbTmp = byCertInfo;
		int nCertInfoLen = i2d_X509_CINF(x509->cert_info, &pbTmp);
		if (nCertInfoLen <= 0)
		{
			X509_free(x509);
			return SAR_CERTENCODEErr;
		}

		long lRet = MYTOOL_ECCVerifySignedData(
			byCertInfo,
			nCertInfoLen,
			x509->signature->data,
			x509->signature->length,
			cacert,
			cacertlen);
		X509_free(x509);
		if (lRet != SAR_OK)
			return SAR_CERTINVALIDErr;
	}

	return SAR_OK;
}

long MYTOOL_VerifyCertSign(unsigned char *cert, int certlen, char *cafile)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	X509_STORE *store = setup_verify(cafile, NULL);
	if (store == NULL)
	{
		X509_free(x509);
		return SAR_CERTENCODEErr;
	}
	int store_flags = 0;
	X509_STORE_set_flags(store, store_flags);

	X509_STORE_CTX *ctx = X509_STORE_CTX_new();
	if (ctx == NULL)
	{
		X509_free(x509);
		X509_STORE_free(store);
		return SAR_CERTINVALIDErr;
	}

	if (!X509_STORE_CTX_init(ctx, store, x509, NULL))
	{
		X509_free(x509);
		X509_STORE_free(store);
		X509_STORE_CTX_free(ctx);
		return SAR_CERTINVALIDErr;
	}

	int nRet = X509_verify_cert(ctx);
	X509_free(x509);
	X509_STORE_free(store);
	X509_STORE_CTX_free(ctx);
	if (nRet == 0)
		return SAR_CERTINVALIDErr;

	return SAR_OK;
}

long MYTOOL_VerifyCertValidity(unsigned char *cert, int certlen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	long lRet;
	time_t tmNow = time(NULL);
	ASN1_UTCTIME *utcNotBefore = ASN1_STRING_dup(X509_get_notBefore(x509)),
		*utcNotAfter = ASN1_STRING_dup(X509_get_notAfter(x509));
	if (ASN1_UTCTIME_cmp_time_t(utcNotBefore, tmNow) >= 0)
		lRet = SAR_CERTNOTYETVALIDErr;
	else if (ASN1_UTCTIME_cmp_time_t(utcNotAfter, tmNow) <= 0)
		lRet = SAR_CERTHASEXPIREDErr;
	else
		lRet = SAR_OK;

	M_ASN1_UTCTIME_free(utcNotBefore);
	M_ASN1_UTCTIME_free(utcNotAfter);
	X509_free(x509);

	return lRet;
}

long MYTOOL_VerifyCRLSign(unsigned char *crl, int crllen, unsigned char *cacert, int cacertlen)
{
	unsigned char *pszTmp = crl;
	X509_CRL *x509crl = d2i_X509_CRL(NULL, (const unsigned char **)&pszTmp, crllen);
	if (x509crl == NULL)
		return SAR_CRLENCODEErr;
	
	char szTmp[128];
	memset(szTmp, 0, sizeof(szTmp));
	OBJ_obj2txt(szTmp, sizeof(szTmp), x509crl->sig_alg->algorithm, 0);
	KTBOOL32 bIsSM2Cert = FALSE;
	if (strstr(szTmp, "RSA") == NULL &&
		strstr(szTmp, "rsa") == NULL &&
		strstr(szTmp, "Rsa") == NULL)
		bIsSM2Cert = TRUE;

	if (!bIsSM2Cert)
	{
		pszTmp = cacert;
		X509 *x509ca = d2i_X509(NULL, (const unsigned char **)&pszTmp, cacertlen);
		if (x509ca == NULL)
		{
			X509_CRL_free(x509crl);
			return SAR_CERTENCODEErr;
		}
		
		EVP_PKEY *pkey = X509_get_pubkey(x509ca);
		if (pkey == NULL)
		{
			X509_CRL_free(x509crl);
			X509_free(x509ca);
			return SAR_CERTENCODEErr;
		}

		int	nRet;
		nRet = X509_CRL_verify(x509crl, pkey);
		EVP_PKEY_free(pkey);
		X509_CRL_free(x509crl);
		X509_free(x509ca);
		if (nRet != 1)
			return SAR_CRLINVALIDErr;
	}
	else
	{
		// get cert info
		int nCrlInfoLen = i2d_X509_CRL_INFO(x509crl->crl, NULL);
		if (nCrlInfoLen <= 0)
		{
			X509_CRL_free(x509crl);
			return SAR_CRLENCODEErr;
		}

		ByteArray rBuf(nCrlInfoLen);
		memset(&rBuf[0], 0, rBuf.size());
		unsigned char *pbTmp = &rBuf[0];
		nCrlInfoLen = i2d_X509_CRL_INFO(x509crl->crl, &pbTmp);
		if (nCrlInfoLen <= 0)
		{
			X509_CRL_free(x509crl);
			return SAR_CRLENCODEErr;
		}

		long lRet = MYTOOL_ECCVerifySignedData(
			&rBuf[0],
			nCrlInfoLen,
			x509crl->signature->data,
			x509crl->signature->length,
			cacert,
			cacertlen);
		X509_CRL_free(x509crl);
		if (lRet != SAR_OK)
			return SAR_CRLINVALIDErr;
	}

	return SAR_OK;
}

long MYTOOL_VerifyCRLSign(char *crlfile, char *cafile)
{
	X509_CRL *x509crl = load_crl(crlfile, FORMAT_PEM);
	if (x509crl == NULL)
		return SAR_CRLENCODEErr;

	int	nRet;
	X509_STORE *store = setup_verify(cafile, NULL);
	if (store == NULL)
	{
		X509_CRL_free(x509crl);
		return SAR_CERTENCODEErr;
	}
	int store_flags = 0;
	X509_STORE_set_flags(store, store_flags);

	X509_STORE_CTX *ctx = X509_STORE_CTX_new();
	if (ctx == NULL)
	{
		X509_CRL_free(x509crl);
		X509_STORE_free(store);
		return SAR_CRLINVALIDErr;
	}

	if (!X509_STORE_CTX_init(ctx, store, NULL, NULL))
	{
		X509_CRL_free(x509crl);
		X509_STORE_free(store);
		X509_STORE_CTX_free(ctx);
		return SAR_CRLINVALIDErr;
	}

	X509_OBJECT xobj;
	nRet = X509_STORE_get_by_subject(ctx, X509_LU_X509, X509_CRL_get_issuer(x509crl), &xobj);
	if (nRet <= 0)
	{
		X509_CRL_free(x509crl);
		X509_STORE_free(store);
		X509_STORE_CTX_free(ctx);
		return SAR_CRLINVALIDErr;
	}

	EVP_PKEY *pkey = X509_get_pubkey(xobj.data.x509);
	X509_OBJECT_free_contents(&xobj);
	if (!pkey)
	{
		X509_CRL_free(x509crl);
		X509_STORE_free(store);
		X509_STORE_CTX_free(ctx);
		return SAR_CRLINVALIDErr;
	}

	nRet = X509_CRL_verify(x509crl, pkey);
	EVP_PKEY_free(pkey);
	X509_CRL_free(x509crl);
	X509_STORE_free(store);
	X509_STORE_CTX_free(ctx);
	if (nRet != 1)
		return SAR_CRLINVALIDErr;

	return SAR_OK;
}

long MYTOOL_VerifyCertWithCRL(unsigned char *cert, int certlen, unsigned char *crl, int crllen)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	pszTmp = crl;
	X509_CRL *x509crl = d2i_X509_CRL(NULL, (const unsigned char **)&pszTmp, crllen);
	if (x509crl == NULL)
	{
		X509_free(x509);
		return SAR_CRLENCODEErr;
	}
	
	int nRet;
	ASN1_INTEGER *serial = X509_get_serialNumber(x509);
	STACK_OF(X509_REVOKED) *revoked = x509crl->crl->revoked;
	int num = sk_X509_REVOKED_num(revoked);
	X509_REVOKED *rc;
	for (int i = 0; i < num; i++)
	{
		rc = sk_X509_REVOKED_pop(revoked);
		nRet = ASN1_INTEGER_cmp(serial, rc->serialNumber);
		X509_REVOKED_free(rc);
		if (nRet == 0)
		{
			X509_free(x509);
			X509_CRL_free(x509crl);
			return SAR_CERTREVOKEDErr;
		}
	}
	X509_free(x509);
	X509_CRL_free(x509crl);

	return SAR_OK;
}

long MYTOOL_VerifyCertWithCRL(unsigned char *cert, int certlen, char *crlfile)
{
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	X509_CRL *x509crl = load_crl(crlfile, FORMAT_PEM);
	if (x509crl == NULL)
	{
		X509_free(x509);
		return SAR_CRLENCODEErr;
	}

	int nRet;
	ASN1_INTEGER *serial = X509_get_serialNumber(x509);
	STACK_OF(X509_REVOKED) *revoked = x509crl->crl->revoked;
	int num = sk_X509_REVOKED_num(revoked);
	X509_REVOKED *rc;
	for (int i = 0; i < num; i++)
	{
		rc = sk_X509_REVOKED_pop(revoked);
		nRet = ASN1_INTEGER_cmp(serial, rc->serialNumber);
		X509_REVOKED_free(rc);
		if (nRet == 0)
		{
			X509_free(x509);
			X509_CRL_free(x509crl);
			return SAR_CERTREVOKEDErr;
		}
	}
	X509_free(x509);
	X509_CRL_free(x509crl);

	return SAR_OK;
}

long MYTOOL_VerifyCert(unsigned char *cert, int certlen, unsigned char *cacert, int cacertlen, unsigned char *crl, int crllen, KTBOOL32 bCheckCRL)
{
	long lRet = MYTOOL_VerifyCertSign(cert, certlen, cacert, cacertlen);
	if (lRet != SAR_OK)
		return lRet;

	lRet = MYTOOL_VerifyCertValidity(cert, certlen);
	if (lRet != SAR_OK)
		return lRet;

	if (bCheckCRL)
	{
		lRet = MYTOOL_VerifyCRLSign(crl, crllen, cacert, cacertlen);
		if (lRet != SAR_OK)
			return lRet;
	}

	lRet = MYTOOL_VerifyCertWithCRL(cert, certlen, crl, crllen);
	if (lRet != SAR_OK)
		return lRet;

	return SAR_OK;
}

long MYTOOL_VerifyCert(unsigned char *cert, int certlen, char *cafile, char *crlfile, KTBOOL32 bCheckCRL)
{
	long lRet = MYTOOL_VerifyCertSign(cert, certlen, cafile);
	if (lRet != SAR_OK)
		return lRet;

	lRet = MYTOOL_VerifyCertValidity(cert, certlen);
	if (lRet != SAR_OK)
		return lRet;

	if (bCheckCRL)
	{
		lRet = MYTOOL_VerifyCRLSign(crlfile, cafile);
		if (lRet != SAR_OK)
			return lRet;
	}

	lRet = MYTOOL_VerifyCertWithCRL(cert, certlen, crlfile);
	if (lRet != SAR_OK)
		return lRet;

	return SAR_OK;
}


// 7
KTBOOL32 MYTOOL_PKCS5Padding(BYTE *data, long & datalen, KTBOOL32 encrypt)
{
	BYTE pad;
	if (encrypt)
	{
		long mlen = (datalen / 16) * 16;
		mlen += 16;
		pad = (BYTE)(mlen - datalen);
		memset(data + datalen, pad, pad);
		datalen = mlen;
		return TRUE;
	}
	else
	{
		if (datalen % 16)
			return FALSE;

		pad = data[datalen - 1];
		if (pad == 0 || pad > 16)
			return FALSE;

		for (int i = 0; i < pad; i++)
		{
			if (data[datalen - 1 - i] != pad)
			{
				return FALSE;
			}
		}

		datalen -= pad;
		memset(data + datalen, 0, pad);
		return TRUE;
	}
}

