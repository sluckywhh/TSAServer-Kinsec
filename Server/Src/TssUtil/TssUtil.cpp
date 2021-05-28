#include "TssUtil.h"
#include "TssClient.h"
#include "MailCoder.h"
//#include "SMLib.h"
#include "MySMLibFunc.h"
#include "pk7_sm2_sign.h"
#include "MySMData.h"
#include "MySMObjID.h"
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/hmac.h>
#include <openssl/ts.h>
#if defined(_LINUX)
#include <sys/timeb.h>
#endif

typedef std::string					String;
typedef std::vector<char>			CharArray;
typedef std::vector<unsigned char>	ByteArray;
typedef std::vector<String>			StringArray;

#define NONCE_LENGTH              64
static bool g_init = false;
#if defined(_WIN32)  
static HANDLE *lock_cs;
#elif defined(_LINUX)
static pthread_mutex_t *lock_cs;
static long *lock_count;
#endif


#define g2(p) (((p)[0] - '0') * 10 + (p)[1] - '0')
#define g4(p) (((p)[0] - '0') * 1000 + ((p)[1] - '0') * 100 + ((p)[2] - '0') * 10 + (p)[3] - '0')
void CertTime2String(asn1_string_st *stTime, char *pszTime);
void locking_callback(int mode, int type, char *file, int line);
unsigned long thread_id();
void thread_setup(void);
void thread_cleanup(void);
int IsPKCS7SM2Sign(PKCS7_SM2_SIGNATURE *p7);


int mystr2bmpstr(const char *pcStr, unsigned int unStrSize, char *pcBMP, bool bBigEndian = true);
int mystr2utf8str(const char *pcStr, unsigned int unStrSize, char *pcUTF8);
int mybmpstr2str(const char *pcBMP, unsigned int unBMPSize, char *pcStr, bool bBigEndian = true);
int mybmpstr2utf8str(const char *pcBMP, unsigned int unBMPSize, char *pcUTF8, bool bBigEndian = true);
int myutf8str2str(const char *pcUTF8, unsigned int unUTF8Size, char *pcStr);
int myutf8str2bmpstr(const char *pcUTF8, unsigned int unUTF8Size, char *pcBMP, bool bBigEndian = true);
KTBOOL32 GetDNItemValue(char *dn, long dnlen, char *itemname, char *itemvalue, long *itemvaluelen);

// 字符串反向（用于DN：从大到小，从小到大）
void ReverseString(char *str)
{
	// 兼容linux
	char *pszTmp1 = str, *pszTmp2, szTmp;
//	CStringArray astrTmp;
//	astrTmp.RemoveAll();
	StringArray astrTmp;
	astrTmp.empty();
	while (1)
	{
		pszTmp2 = strstr(pszTmp1, SGD_STR_SEPARATOR);
		if (pszTmp2 != NULL)
		{
			szTmp = *pszTmp2;
			*pszTmp2 = 0;
			if (strlen(pszTmp1) > 0)
//				astrTmp.Add(pszTmp1);
				astrTmp.push_back(pszTmp1);
			*pszTmp2 = szTmp;
			pszTmp1 = pszTmp2 + strlen(SGD_STR_SEPARATOR);
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
				strcat(str, SGD_STR_SEPARATOR);
//			strcat(str, astrTmp.GetAt(i));
			strcat(str, astrTmp[i].c_str());
		}
	}
}

// 取证书DN
bool mycertname2string(X509_NAME *nm, char *pszDN)
{
	if (nm == NULL)
		return false;

	int num = X509_NAME_entry_count(nm);
	if (num <= 0)
		return false;

	// 兼容linux
//	USES_CONVERSION;
//	setlocale(LC_CTYPE, "");
//	char asndata2[1024];
//	wchar_t wdata[1024];
	int n;

	int fn_nid, asnlen, asntype;
	char szOut[1024];
	char szName[1024], szValue[1024];
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
				return false;
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
				1024);
			if (wdatalen <= 0)
				return false;
			int datalen = WideCharToMultiByte(
				CP_ACP,
				0,
				wdata,
				wdatalen,
				szValue,
				1024,
				NULL,
				NULL);
			if (datalen <= 0)
				return false;
*/			n = myutf8str2str(asndata, asnlen, szValue);
			if (n <= 0)
				return false;
		}
		else
			memcpy(szValue, asndata, asnlen);

		if (i > 0)
			strcat(szOut, SGD_STR_SEPARATOR);
		strcat(szOut, szName);
		strcat(szOut, "=");
		strcat(szOut, szValue);
	}

	if (strlen(szOut) == 0)
		return false;
	strcpy(pszDN, szOut);

	return true;
}


TSSUTIL_EXPORTS SGD_UINT32 STF_InitEnvironment(void **phTSHandle)
{
	if (g_init)
	{
		return STF_TS_OK;
	}
	g_init = true;
	OpenSSL_add_all_algorithms();
	ERR_load_ERR_strings();
	ERR_load_crypto_strings();
	thread_setup();
	*phTSHandle = malloc(4);
	return STF_TS_OK;
}

TSSUTIL_EXPORTS SGD_UINT32 STF_ClearEnvironment(void* hTSHandle)
{
	if (g_init)
	{
		g_init = false;
		thread_cleanup();
		CONF_modules_unload(1);
		EVP_cleanup();
		ENGINE_cleanup();
		CRYPTO_cleanup_all_ex_data();
		ERR_remove_state(0);
		ERR_free_strings();
		free(hTSHandle);
		return STF_TS_OK;
	}
	return STF_TS_OK;
}

TSSUTIL_EXPORTS SGD_UINT32 *STF_GetErrorString(SGD_UINT32 EerrorNum, SGD_UINT32 *errorInfo)
{
	return STF_TS_OK;
}

TSSUTIL_EXPORTS SGD_UINT32 STF_GetCurrentTime(SGD_UINT8 *url, SGD_UINT8 *currentTime )
{
	CTSClient cli;
	cli.Init(url);
	string CurrTime;
	int ret = cli.TS_Time(CurrTime);
	if (ret == 200)
	{
		strcpy(currentTime, CurrTime.c_str());
		return STF_TS_OK;
	}
	return ret;
}

bool HashData(unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
	SHA_CTX context;
	SHA1_Init(&context);
	SHA1_Update(&context, (const unsigned char *)in, inlen);
	SHA1_Final(out, &context);
	*outlen = 20;
	return true;
}

ASN1_INTEGER *create_nonce(int bits)
{
	unsigned char buf[20];
	ASN1_INTEGER *nonce = NULL;
	int len = (bits - 1) / 8 + 1;
	int i;

	/* Generating random byte sequence. */
	if (len > (int)sizeof(buf)) goto err;
	if (RAND_bytes(buf, len) <= 0) goto err;

	/* Find the first non-zero byte and creating ASN1_INTEGER object. */
	for (i = 0; i < len && !buf[i]; ++i);
	if (!(nonce = ASN1_INTEGER_new())) goto err;
	OPENSSL_free(nonce->data);
	/* Allocate at least one byte. */
	nonce->length = len - i;
	if (!(nonce->data = (unsigned char *)OPENSSL_malloc(nonce->length + 1))) goto err;
	memcpy(nonce->data, buf + i, nonce->length);

	return nonce;
err:
	//	BIO_printf(bio_err, "could not create nonce\n");
	ASN1_INTEGER_free(nonce);
	return NULL;
}


TSSUTIL_EXPORTS SGD_UINT32 STF_CreateTSRequest( void* hTSHandle, 
	SGD_UINT8 *pucInData, 
	SGD_UINT32 uiInDataLength, 
	SGD_UINT32 uiReqType, 
	SGD_UINT8 *pucTSExt,
	SGD_UINT32 *TSExtLength,
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT8 *pucTSRequest, 
	SGD_UINT32 *puiTSRequestLength)
{
	if (uiInDataLength > 1024*1024*1024)
	{
		return STF_TS_INDATA_TOOLONG;
	}
	if (uiReqType != 1)
	{
		return STF_TS_INVALID_REQUEST;
	}
	if (pucInData == NULL || uiInDataLength == 0)
	{
		return STF_TS_INVALID_DATAFORMAT;
	}
	if (uiHashAlgID == SGD_SHA1)
	{
		return STF_TS_INVALID_ALG;
	}

	bool bSB3 = false;
	if (uiHashAlgID == SGD_SM3)
	{
		bSB3 = true;
	}
	else if (uiHashAlgID == SGD_SHA1)
	{
		bSB3 = false;
	}
	else
	{
		return STF_TS_INVALID_ALG;
	}

	if ( !(pucTSExt == NULL || strlen(pucTSExt) == 0) )
	{
		return STF_TS_UNACCEPTED_EXTENSION;
	}

	bool bRet = STF_TS_OK;
	int nRet = 0, RetVal = 0;
	TS_MSG_IMPRINT *msg_imprint = NULL;
	X509_ALGOR *x509_algor = NULL;
	ASN1_INTEGER *nonce_asn1 = NULL;

	TS_REQ *ts_req = NULL;

	// digest ********************************************************
	unsigned char byDigest[256];
	memset(byDigest, 0, sizeof(byDigest));
	long nDigestLen = 256;
	if (bSB3)
	{
		bRet = MYSM_SM3((unsigned char *)pucInData, uiInDataLength, byDigest, &nDigestLen);
		if (bRet != 0)
		{
			RetVal = SAR_HashErr;
			goto end;
		}
	}
	else
	{
		bRet = HashData((unsigned char *)pucInData, uiInDataLength, byDigest, (int *)&nDigestLen);
		if (!bRet)
		{
			RetVal = SAR_HashErr;
			goto end;
		}
	}

	// ts req ********************************************************
	ts_req = TS_REQ_new();
	if (!ts_req)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	// version
	long version;
	version = 1;
	nRet = TS_REQ_set_version(ts_req, version);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// messageImprint
	x509_algor = X509_ALGOR_new();
	if (!ts_req)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}
	if (bSB3)
	{
		x509_algor->algorithm = OBJ_txt2obj(OID_SM3, 0);
	}
	else
	{
		x509_algor->algorithm = OBJ_nid2obj(NID_sha1);
	}
	
	if (!(x509_algor->algorithm))
	{
		RetVal = STF_TS_INVALID_ALG;
		goto end;
	}
	x509_algor->parameter = ASN1_TYPE_new();
	if (!(x509_algor->parameter))
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}
	x509_algor->parameter->type = V_ASN1_NULL;

	msg_imprint = TS_MSG_IMPRINT_new();
	if (!msg_imprint)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}
	nRet = TS_MSG_IMPRINT_set_algo(msg_imprint, x509_algor);
	if (!nRet)
	{
		RetVal = STF_TS_INVALID_ALG;
		goto end;
	}
	nRet = TS_MSG_IMPRINT_set_msg(msg_imprint, byDigest, nDigestLen);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}
	nRet = TS_REQ_set_msg_imprint(ts_req, msg_imprint);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// nonce
	nonce_asn1 = create_nonce(NONCE_LENGTH);
	if (!nonce_asn1)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	nRet = TS_REQ_set_nonce(ts_req, nonce_asn1);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// certReq
	int cert_req;
	cert_req = 1;
	nRet = TS_REQ_set_cert_req(ts_req, cert_req);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// output
	unsigned char byOut[10240];
	memset(byOut, 0, sizeof(byOut));
	int nOutLen;
	unsigned char *pbTmp;
	pbTmp = byOut;
	nOutLen = i2d_TS_REQ(ts_req, &pbTmp);
	if (nOutLen <= 0)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}
	memcpy(pucTSRequest, byOut, nOutLen);
	*puiTSRequestLength = nOutLen;
// 	char szTSReqB64[10240] = {0};
// 	int nTSReqB64Len = CMailCoder::base64_encode((char *)byOut, nOutLen, szTSReqB64);
// 	memcpy(pucTSRequest, szTSReqB64, nTSReqB64Len);
// 	*puiTSRequestLength = nTSReqB64Len;

end:
	if (msg_imprint)
		TS_MSG_IMPRINT_free(msg_imprint);
	if (x509_algor)
		X509_ALGOR_free(x509_algor);
	if (nonce_asn1)
		ASN1_INTEGER_free(nonce_asn1);

	if (ts_req)
		TS_REQ_free(ts_req);

	return RetVal;
}

TSSUTIL_EXPORTS SGD_UINT32 STF_GetTSReponse (void* hTSHandle,
	SGD_UINT8 *url,
	SGD_UINT8 *pucTSRequest,
	SGD_UINT32 uiTSRequestLength,
	SGD_UINT32 uiSignatureAlgID,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 *puiTSResponseLength)
{
	if (pucTSRequest == NULL || uiTSRequestLength == 0)
	{
		return STF_TS_INVALID_DATAFORMAT;
	}
	if (uiSignatureAlgID != 1)
	{
		return STF_TS_INVALID_ALG;
	}
	CTSClient cli;
	cli.Init(url);
	string requestData;
	requestData.append(pucTSRequest, uiTSRequestLength);
	string respData;
	int ret = cli.TS_Resp(requestData, respData);
	if (ret == 200)
	{
		memcpy(pucTSResponse, respData.c_str(), respData.length());
		*puiTSResponseLength = respData.length();
		return STF_TS_OK;
	}
	return ret;
}

TSSUTIL_EXPORTS SGD_UINT32 STF_VerifyTSValidity(void* hTSHandle,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT32 uiSignatureAlgID, 
	SGD_UINT8 *pucTSCert, 
	SGD_UINT32 uiTSCertLength)
{
	int nRet = 0;
	int RetVal = STF_TS_OK;
	PKCS7 *ts_token = NULL;
	TS_VERIFY_CTX *ts_verify_ctx = NULL;

	// verify ts token ********************************************************
	// ts token
	unsigned char *pbTmp = (unsigned char *)pucTSResponse;
	ts_token = d2i_PKCS7(NULL, (const unsigned char **)&pbTmp, uiTSResponseLength);
	if (!ts_token)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}

	if (OBJ_obj2nid(ts_token->type) != NID_pkcs7_signed)
	{
		PKCS7_free(ts_token);
		char b64Response[4096] = {0};
		int b64len = CMailCoder::base64_encode(pucTSResponse, uiTSResponseLength, b64Response);

		return MYSIGN_VerifySignedDataByP7_SM2(b64Response, NULL, 0);
	}

	// verify ctx
	ts_verify_ctx = TS_VERIFY_CTX_new();
	if (!ts_verify_ctx)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	ts_verify_ctx->flags = TS_VFY_VERSION | TS_VFY_DATA | TS_VFY_SIGNATURE | TS_VFY_SIGNER;

	// data
	ts_verify_ctx->data = BIO_new(BIO_s_mem());
	if (!ts_verify_ctx->data)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	//	BIO_set_close(ts_verify_ctx->data, BIO_CLOSE); // BIO_free() free BUF_MEM
	if (BIO_write(ts_verify_ctx->data, pucTSResponse, uiTSResponseLength) != uiTSResponseLength)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}

	// x509 store
	ts_verify_ctx->store = X509_STORE_new();
	if (!ts_verify_ctx->store)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	// verify
	nRet = TS_RESP_verify_token(ts_verify_ctx, ts_token);
	if (!nRet)
	{
		RetVal = SAR_VERIFYTSErr;
		goto end;
	}

end:
	if (ts_token)
		PKCS7_free(ts_token);
	if (ts_verify_ctx)
		TS_VERIFY_CTX_free(ts_verify_ctx);

	return RetVal;
}


TSSUTIL_EXPORTS SGD_UINT32 STF_GetTSInfo(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT8 *pucIssuerName, 
	SGD_UINT32 *puiIssuerNameLength, 
	SGD_UINT8 *pucTime, 
	SGD_UINT32 *puiTimeLength)
{
	string temp;
	int RetVal = STF_TS_OK;
	PKCS7 *ts_token = NULL;
	TS_TST_INFO *ts_tst_info = NULL;
	ASN1_GENERALIZEDTIME *gentime;
	string::size_type pos;
	char szDNname[256] = {0};
	GENERAL_NAME *tsa_name;
	PKCS7_SM2_SIGNATURE *p7 = NULL;
	// parse ts token ********************************************************
	// ts token
	unsigned char *pbTmp;
	pbTmp = (unsigned char *)pucTSResponse;
   	ts_token = d2i_PKCS7(NULL, (const unsigned char **)&pbTmp, uiTSResponseLength);
	if (!ts_token)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}

	//PKCS7_SM2_SIGNATURE *p7 = NULL;
	// tst info
	int nTSTInfoLen;
	if (OBJ_obj2nid(ts_token->type) != NID_pkcs7_signed)
	{
		pbTmp = (unsigned char *)pucTSResponse;
		p7 = d2i_PKCS7_SM2_SIGNATURE(NULL, (const unsigned char **)&pbTmp, uiTSResponseLength);
		if (p7 == NULL)
			return STF_TS_MALFORMAT;
		if (!IsPKCS7SM2Sign(p7))
		{
			RetVal = STF_TS_MALFORMAT;
			goto end;
		}
		if (p7->signedData->contentInfo->data == NULL)
		{
			RetVal = SAR_ECCVERIFYSIGNErr;
			goto end;
		}
		pbTmp = p7->signedData->contentInfo->data->data;
		nTSTInfoLen = p7->signedData->contentInfo->data->length;
		if (nTSTInfoLen <= 0)
		{
			RetVal = SAR_ECCVERIFYSIGNErr;
			goto end;
		}
		ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, nTSTInfoLen);
	}
	else
	{
		nTSTInfoLen = ts_token->d.sign->contents->d.other->value.octet_string->length;
		pbTmp =	ts_token->d.sign->contents->d.other->value.octet_string->data;
		ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, nTSTInfoLen);
	}
	if (!ts_tst_info)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}
	tsa_name = TS_TST_INFO_get_tsa(ts_tst_info);
	//char szDNname[256] = {0};
	mycertname2string(tsa_name->d.directoryName, szDNname);
	temp = szDNname;
	pos = temp.find("CN="); 
	temp = temp.substr(pos + strlen("CN="));
	strcpy(pucIssuerName, temp.c_str());
	*puiIssuerNameLength = temp.length();

	gentime = (ASN1_GENERALIZEDTIME *)TS_TST_INFO_get_time(ts_tst_info);
	CertTime2String(gentime, pucTime);
	*puiTimeLength = strlen(pucTime);

end:
	if (ts_token)
		PKCS7_free(ts_token);
	if (ts_tst_info)
		TS_TST_INFO_free(ts_tst_info);
	if (p7)
		PKCS7_SM2_SIGNATURE_free(p7);
	return RetVal;
}

int GetTimeOfStamp(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength);
int GetSignerCN(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength);
int GetOriginalData(PKCS7_SM2_SIGNATURE *p7, char *pucItemValue, int *puiItemValueLength);
int GetCertOfTsserver(PKCS7_SM2_SIGNATURE *p7, char *pucItemValue, int *puiItemValueLength);
int GetPrecision(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength);
int GetSignerCountry(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength);
int GetSignerOrg(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength);
int GetSignerCity(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength);
int GetSignerEmail(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength);



TSSUTIL_EXPORTS SGD_UINT32 STF_GetTSDetail(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiItemnumber, 
	SGD_UINT8 *pucItemValue, 
	SGD_UINT32 *puiItemValueLength)
{
	string temp;
	int RetVal = STF_TS_OK;
	PKCS7 *ts_token = NULL;
	TS_TST_INFO *ts_tst_info = NULL;
	PKCS7_SM2_SIGNATURE *p7 = NULL;
	// parse ts token ********************************************************
	// ts token
	unsigned char *pbTmp;
	pbTmp = (unsigned char *)pucTSResponse;
	ts_token = d2i_PKCS7(NULL, (const unsigned char **)&pbTmp, uiTSResponseLength);
	if (!ts_token)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}

	// tst info
	int nTSTInfoLen;
	if (OBJ_obj2nid(ts_token->type) != NID_pkcs7_signed)
	{
		pbTmp = (unsigned char *)pucTSResponse;
		p7 = d2i_PKCS7_SM2_SIGNATURE(NULL, (const unsigned char **)&pbTmp, uiTSResponseLength);
		if (p7 == NULL)
			return STF_TS_MALFORMAT;
		if (!IsPKCS7SM2Sign(p7))
		{
			RetVal = STF_TS_MALFORMAT;
			goto end;
		}
		if (p7->signedData->contentInfo->data == NULL)
		{
			RetVal = SAR_ECCVERIFYSIGNErr;
			goto end;
		}
		pbTmp = p7->signedData->contentInfo->data->data;
		nTSTInfoLen = p7->signedData->contentInfo->data->length;
		if (nTSTInfoLen <= 0)
		{
			RetVal = SAR_ECCVERIFYSIGNErr;
			goto end;
		}
		ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, nTSTInfoLen);
	}
	else
	{
		nTSTInfoLen = ts_token->d.sign->contents->d.other->value.octet_string->length;
		pbTmp =	ts_token->d.sign->contents->d.other->value.octet_string->data;
		ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, nTSTInfoLen);
	}
	if (!ts_tst_info)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}
	switch(uiItemnumber)
	{
	case STF_TIME_OF_STAMP:
		RetVal = GetTimeOfStamp(ts_tst_info, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_CN_OF_TSSIGNER:
		RetVal = GetSignerCN(ts_tst_info, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_ORIGINAL_DATA:
		RetVal = GetOriginalData(p7, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_CERT_OF_TSSERVER:
		RetVal = GetCertOfTsserver(p7, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_CERTCHAIN_OF_TSSERVER: 
		RetVal = STF_TS_INVALID_ITEM;
		break;
	case STF_SOURCE_OF_TIME: 
		RetVal = STF_TS_INVALID_ITEM;
		break;
	case STF_TIME_PRECISION:  
		RetVal = GetPrecision(ts_tst_info, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_RESPONSE_TYPE: 
		RetVal = STF_TS_INVALID_ITEM;
		break;
	case STF_SUBJECT_COUNTRY_OF_TSSIGNER:
		RetVal = GetSignerCountry(ts_tst_info, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_SUBJECT_ORGNIZATION_OF_TSSIGNER:
		RetVal = GetSignerOrg(ts_tst_info, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_SUBJECT_CITY_OF_TSSIGNER:
		RetVal = GetSignerCity(ts_tst_info, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	case STF_SUBJECT_EMIAL_OF_TSSIGNER:
		RetVal = GetSignerEmail(ts_tst_info, pucItemValue, puiItemValueLength);
		if (RetVal != STF_TS_OK)
		{
			goto end;
		}
		break;
	default:
		RetVal = STF_TS_INVALID_ITEM;
		break;
	}
end:
	if (ts_token)
		PKCS7_free(ts_token);
	if (ts_tst_info)
		TS_TST_INFO_free(ts_tst_info);
	if (p7)
		PKCS7_SM2_SIGNATURE_free(p7);
	return RetVal;
}


int GetTimeOfStamp(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength)
{
	// get time
	int RetVal= STF_TS_OK;
	ASN1_GENERALIZEDTIME *generalized_time;
	generalized_time = (ASN1_GENERALIZEDTIME *)TS_TST_INFO_get_time(ts_tst_info);
	if (!generalized_time)
	{
		RetVal = STF_TS_MALFORMAT;
	}
	CertTime2String(generalized_time, pucItemValue);
	*puiItemValueLength = strlen(pucItemValue);
	return RetVal;
}

int GetSignerCN(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength)
{
	GENERAL_NAME *tsa_name = TS_TST_INFO_get_tsa(ts_tst_info);
	char szDNname[256] = {0};
	mycertname2string(tsa_name->d.directoryName, szDNname);
	char ItemValue[32];
	long ItenLen;
	KTBOOL32 bRet = GetDNItemValue(szDNname, strlen(szDNname), "CN", ItemValue, &ItenLen);
	if (bRet)
	{
		strcpy(pucItemValue, ItemValue);
		*puiItemValueLength = ItenLen;
		return STF_TS_OK;
	}
	else
	{
		return STF_TS_MALFORMAT;
	}
}

int GetOriginalData(PKCS7_SM2_SIGNATURE *p7, char *pucItemValue, int *puiItemValueLength)
{
	if (p7->signedData->contentInfo->data == NULL)
	{
		return SAR_ECCVERIFYSIGNErr;
	}
	unsigned char *pbOriginalData = p7->signedData->contentInfo->data->data;
	long lOriginalDataLen = p7->signedData->contentInfo->data->length;
	if (lOriginalDataLen <= 0)
	{
		return SAR_ECCVERIFYSIGNErr;
	}
	memcpy(pucItemValue, pbOriginalData, lOriginalDataLen);
	*puiItemValueLength = lOriginalDataLen;
	return STF_TS_OK;
}

int GetCertOfTsserver(PKCS7_SM2_SIGNATURE *p7, char *pucItemValue, int *puiItemValueLength)
{
	int RetVal= STF_TS_OK; 
	STACK_OF(X509) *signcerts = NULL;
	unsigned char byCert[SGD_SIZE_CERT] = {0};
	long lCertLen = 0;
	signcerts = p7->signedData->certificates;
	if (signcerts == NULL)
	{
		return  SAR_PKCS7ENCODEErr;
	}
	for (int i = 0; i < sk_X509_num(signcerts); i++)
	{
		X509 *signcert = sk_X509_value(signcerts, i);
		unsigned char *pbTmp = byCert;
		lCertLen = i2d_X509(signcert, &pbTmp);
		if (lCertLen > 0)
			break;
	}
	if (lCertLen <= 0)
	{
		return SAR_PKCS7ENCODEErr;
	}
	char B64Data[4096] = {0};
	int B64len = CMailCoder::base64_encode((char *)byCert, lCertLen, B64Data);
	strcpy(pucItemValue, B64Data);
	*puiItemValueLength = lCertLen;
	return STF_TS_OK;
}

int GetPrecision(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength)
{
	TS_ACCURACY *ac = TS_TST_INFO_get_accuracy(ts_tst_info);
	if (ac == NULL)
	{
		return STF_TS_MALFORMAT;
	}

	ASN1_INTEGER *ans1_sec = ac->seconds;
	BIGNUM *bn_sec = ASN1_INTEGER_to_BN(ans1_sec, NULL);
	char* secBuf = BN_bn2dec(bn_sec);

	ASN1_INTEGER *ans1_millis = ac->millis;
	BIGNUM *bn_millis = ASN1_INTEGER_to_BN(ans1_millis, NULL);
	char* secMillis = BN_bn2dec(bn_millis);

	ASN1_INTEGER *ans1_micros = ac->micros;
	BIGNUM *bn_micros = ASN1_INTEGER_to_BN(ans1_micros, NULL);
	char* secMicros = BN_bn2dec(bn_micros);

	char temp[64] = {0};
	sprintf(temp, "secs:%s, millisecs:%s, microsecs:%s.", secBuf, secMillis, secMicros);
	strcpy(pucItemValue, temp);
	*puiItemValueLength = strlen(pucItemValue);
	return STF_TS_OK;
}

int GetSignerCountry(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength)
{
	GENERAL_NAME *tsa_name = TS_TST_INFO_get_tsa(ts_tst_info);
	char szDNname[256] = {0};
	mycertname2string(tsa_name->d.directoryName, szDNname);
	char ItemValue[32];
	long ItenLen;
	KTBOOL32 bRet = GetDNItemValue(szDNname, strlen(szDNname), "C", ItemValue, &ItenLen);
	if (bRet)
	{
		strcpy(pucItemValue, ItemValue);
		*puiItemValueLength = ItenLen;
		return STF_TS_OK;
	}
	else
	{
		return STF_TS_MALFORMAT;
	}
}

int GetSignerOrg(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength)
{
	GENERAL_NAME *tsa_name = TS_TST_INFO_get_tsa(ts_tst_info);
	char szDNname[256] = {0};
	mycertname2string(tsa_name->d.directoryName, szDNname);
	char ItemValue[32];
	long ItenLen;
	KTBOOL32 bRet = GetDNItemValue(szDNname, strlen(szDNname), "O", ItemValue, &ItenLen);
	if (bRet)
	{
		strcpy(pucItemValue, ItemValue);
		*puiItemValueLength = ItenLen;
		return STF_TS_OK;
	}
	else
	{
		return STF_TS_MALFORMAT;
	}
}

int GetSignerCity(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength)
{
	GENERAL_NAME *tsa_name = TS_TST_INFO_get_tsa(ts_tst_info);
	char szDNname[256] = {0};
	mycertname2string(tsa_name->d.directoryName, szDNname);
	char ItemValue[32];
	long ItenLen;
	KTBOOL32 bRet = GetDNItemValue(szDNname, strlen(szDNname), "L", ItemValue, &ItenLen);
	if (bRet)
	{
		strcpy(pucItemValue, ItemValue);
		*puiItemValueLength = ItenLen;
		return STF_TS_OK;
	}
	else
	{
		return STF_TS_MALFORMAT;
	}
}

int GetSignerEmail(TS_TST_INFO *ts_tst_info, char *pucItemValue, int *puiItemValueLength)
{
	GENERAL_NAME *tsa_name = TS_TST_INFO_get_tsa(ts_tst_info);
	char szDNname[256] = {0};
	mycertname2string(tsa_name->d.directoryName, szDNname);
	char ItemValue[32];
	long ItenLen;
	KTBOOL32 bRet = GetDNItemValue(szDNname, strlen(szDNname), "E", ItemValue, &ItenLen);
	if (bRet)
	{
		strcpy(pucItemValue, ItemValue);
		*puiItemValueLength = ItenLen;
		return STF_TS_OK;
	}
	else
	{
		return STF_TS_MALFORMAT;
	}
}


void locking_callback(int mode, int type, char *file, int line)
{
#if defined(_WIN32)
	if (mode & CRYPTO_LOCK)
	{
		WaitForSingleObject(lock_cs[type],INFINITE);
	}
	else
	{
		ReleaseMutex(lock_cs[type]);
	}
#elif defined(_LINUX)
	if (mode & CRYPTO_LOCK)
	{
		pthread_mutex_lock(&(lock_cs[type]));
		lock_count[type]++;
	}
	else
	{
		pthread_mutex_unlock(&(lock_cs[type]));
	}
#endif
}

unsigned long thread_id()
{
#if defined(_WIN32)
	return GetCurrentThreadId();
#elif defined(_LINUX)
	return (unsigned long)pthread_self();
#endif
}

void thread_setup(void)
{
#if defined(_WIN32)

	int i;
	lock_cs = (HANDLE*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(HANDLE));
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		lock_cs[i]=CreateMutex(NULL,FALSE,NULL);
	}
	CRYPTO_set_locking_callback((void (*)(int,int,const char *,int))locking_callback);

#elif defined(_LINUX)

	int i;
	lock_cs=(pthread_mutex_t*) OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
	lock_count=(long*) OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		lock_count[i]=0;
		pthread_mutex_init(&(lock_cs[i]),NULL);
	}

	CRYPTO_set_id_callback((unsigned long (*)())thread_id);
	CRYPTO_set_locking_callback((void (*)(int,int,const char *,int))locking_callback);
#endif

}

void thread_cleanup(void)
{
#if defined(_WIN32)

	int i;
	CRYPTO_set_locking_callback(NULL);
	for (i=0; i<CRYPTO_num_locks(); i++)
		CloseHandle(lock_cs[i]);
	OPENSSL_free(lock_cs);	
#elif defined(_LINUX)

	int i;
	CRYPTO_set_locking_callback(NULL);
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		pthread_mutex_destroy(&(lock_cs[i]));
	}
	OPENSSL_free(lock_cs);
	OPENSSL_free(lock_count);
#endif

}

void CertTime2String(asn1_string_st *stTime, char *pszTime)
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

	struct tm tmTmp;
	memset(&tmTmp, 0, sizeof(tm));
	tmTmp.tm_year = year - 1900;
	tmTmp.tm_mon = month - 1;
	tmTmp.tm_mday = day;
	tmTmp.tm_hour = hour;
	tmTmp.tm_min = minute;
	tmTmp.tm_sec = second;
	time_t timeTmp = mktime(&tmTmp);
	timeTmp -= nBias * 60;
	tmTmp = *localtime(&timeTmp);
	sprintf(pszTime, "%d-%02d-%02d %02d:%02d:%02d",
		tmTmp.tm_year + 1900, tmTmp.tm_mon + 1, tmTmp.tm_mday,
		tmTmp.tm_hour, tmTmp.tm_min, tmTmp.tm_sec);

	M_ASN1_UTCTIME_free(utcTime);
}


