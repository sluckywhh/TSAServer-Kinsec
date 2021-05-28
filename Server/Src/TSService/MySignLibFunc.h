#ifndef _MYSIGNLIBFUNC_H
#define _MYSIGNLIBFUNC_H

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/sha.h>

#include <string>
#include <vector>
using namespace std;

#undef TRUE
#undef FALSE
#define TRUE				0x00000001	//布尔值为真
#define FALSE				0x00000000	//布尔值为假

#ifndef BYTE
typedef unsigned char       BYTE;
#endif
#ifndef KTBOOL32
typedef int					KTBOOL32;
#endif
#ifndef WORD
typedef unsigned short int	WORD;
#endif
#ifndef DWORD
#	ifdef WIN32
        typedef unsigned long   DWORD;
#	else
        typedef unsigned int    DWORD;
#	endif
#endif
#ifndef HANDLE
typedef void *				HANDLE;
#endif

#ifndef ByteArray
typedef std::vector<BYTE> ByteArray;
#endif

// 私钥加密
typedef int (*FUNC_PRIV_ENC)(
	int flen,
	const unsigned char *from,
	unsigned char *to,
	RSA *rsa,
	int padding);
// 私钥解密
typedef int (*FUNC_PRIV_DEC)(
	int flen,
	const unsigned char *from,
	unsigned char *to,
	RSA *rsa,
	int padding);

// 对称加密或解密数据
// 失败返回0，成功返回1
typedef int (*FUNC_SYMM_ENC_OR_DEC_DATA)(
	bool bEncrypt,						// 是否加密
	int nAlgID,							// 对称算法标识
	unsigned char *pbKey,				// 密钥
	int nKeyLen,						// 密钥的长度
	bool bKeyEncrypted,					// 密钥是否为密文
	unsigned char *pbIV,				// 初始向量
	int nIVLen,							// 初始向量的长度
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

long MYSIGN_Initialize();
long MYSIGN_Finalize();

// PKCS7签名
long MYSIGN_SignDataByP7(
	unsigned char *pbIn,				// 输入数据
	long lInLen,						// 输入数据的长度
	char *pszCert,						// 签名证书
	char *pszKey,						// 签名私钥（若使用硬件设备，应为NULL）
	FUNC_PRIV_ENC funcPrivEnc,			// 私钥加密函数（若使用软私钥，应为NULL）
	char *pszOut,						// 输出数据
	bool bAttached = true);				// 是否包含原文
// PKCS7验证签名
long MYSIGN_VerifySignedDataByP7(
	char *pszSign,
	unsigned char *pbData = NULL,
	long lDataLen = 0);
// 获取PKCS7签名信息
long MYSIGN_GetP7SignDataInfo(
	char *pszIn,
	long lType,
	unsigned char *pbOut,
	long *plOutLen);

// XML签名
long MYSIGN_SignDataXML(
	unsigned char *pbIn,				// 输入数据
	long lInLen,						// 输入数据的长度
	char *pszCert,						// 签名证书
	char *pszKeyName,					// KeyName（可为签名证书CN或NULL）
	char *pszKey,						// 签名私钥（若使用硬件设备，应为NULL）
	FUNC_PRIV_ENC funcPrivEnc,			// 私钥加密函数（若使用软私钥，应为NULL）
	char *pszOut);						// 输出数据
// XML验证签名
long MYSIGN_VerifySignedDataXML(
	char *pszIn);
// 获取XML签名信息
long MYSIGN_GetXMLSignatureInfo(
	char *pszIn,
	long lType,
	unsigned char *pbOut,
	long *plOutLen);

// PKCS7数字信封
// lAlgID取值如下：
//   SGD_SM1_ECB
//   SGD_SM1_CBC
//   SGD_SSF33_ECB
//   SGD_SSF33_CBC
//   SGD_SM4_ECB
//   SGD_SM4_CBC
//   其它			3DES
// 若对称算法使用软算法，则funcEncOrDecData应为NULL。
long MYSIGN_EnvelopeDataByP7(
	unsigned char *pbIn,						// 输入数据
	long lInLen,								// 输入数据的长度
	char *pszRcptCerts[],						// 加密证书
	long lRcptCount,							// 加密证书的个数
	long lAlgID,								// 对称算法
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	char *pszOut);								// 输出数据

// 解密PKCS7数字信封
// 若非对称算法使用软算法，则funcPrivDec应为NULL；否则，pszKey应为NULL。
// 若对称算法使用软算法，则funcEncOrDecData应为NULL。
long MYSIGN_UnenvelopeDataByP7(
	char *pszIn,								// 输入数据
	char *pszCert,								// 解密证书
	char *pszKey,								// 解密私钥
	FUNC_PRIV_DEC funcPrivDec,					// 私钥解密函数
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	unsigned char *pbOut,						// 输出数据
	long *plOutLen);							// 输出数据的长度

long MYSIGN_CipherDataByP7( 
	unsigned char *in, 
	long inlen,
	int cipher,
	char *cert,
	char *key,
	FUNC_PRIV_ENC funcprivenc,
	char *recipcerts,
	ByteArray &rOut );
long MYSIGN_DecipherDataByP7( 
	unsigned char *in, 
	long inlen,
	char *cert,
	char *key,
	FUNC_PRIV_DEC funcprivdec,
	char *chaincerts,
	char *outsigncert,
	ByteArray &rOut );

#endif
