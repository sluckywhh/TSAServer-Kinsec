#ifndef _MYSIGNLIBFUNC_H
#define _MYSIGNLIBFUNC_H

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
	unsigned char *pbCert,				// 签名证书
	long lCertLen,						// 签名证书的长度
	unsigned char *pbKey,				// 签名私钥（若使用硬件设备，应为NULL）
	long lKeyLen,						// 签名私钥的长度（若使用硬件设备，应为0）
	FUNC_PRIV_ENC funcPrivEnc,			// 私钥加密函数（若使用软私钥，应为NULL）
	unsigned char *pbOut,				// 输出数据
	long *plOutLen,						// 输出数据的长度
	bool bAttached = true,				// 是否包含原文
	bool bHasAttr = false,				// 是否包含签名属性
	bool bWithVerificationCode = false);// 是否带校验码
long MYSIGN_SignDataByP7(
	unsigned char *pbIn,				// 输入数据
	long lInLen,						// 输入数据的长度
	char *pszCert,						// 签名证书（base64编码）
	char *pszKey,						// 签名私钥（base64编码。若使用硬件设备，应为NULL）
	FUNC_PRIV_ENC funcPrivEnc,			// 私钥加密函数（若使用软私钥，应为NULL）
	char *pszOut,						// 输出数据（base64编码）
	bool bAttached = true,				// 是否包含原文
	bool bHasAttr = false,				// 是否包含签名属性
	bool bWithVerificationCode = false);// 是否带校验码
// PKCS7验证签名
long MYSIGN_VerifySignedDataByP7(
	unsigned char *pbP7Sign,			// PKCS7签名
	long lP7SignLen,					// PKCS7签名的长度
	unsigned char *pbData = NULL,		// 原文
	long lDataLen = 0);					// 原文的长度
long MYSIGN_VerifySignedDataByP7(
	char *pszP7Sign,					// PKCS7签名（base64编码）
	unsigned char *pbData = NULL,		// 原文
	long lDataLen = 0);					// 原文的长度
// 获取PKCS7签名信息
long MYSIGN_GetP7SignDataInfo(
	unsigned char *pbIn,				// 输入数据
	long lInLen,						// 输入数据的长度
	long lType,							// 类型：1－原文，2－签名证书，3－签名值，4－签名算法，5－原文摘要，6－摘要算法，7－签名数据版本，8－签名时间
	unsigned char *pbOut,				// 输出数据
	long *plOutLen);					// 输出数据的长度
long MYSIGN_GetP7SignDataInfo(
	char *pszIn,						// 输入数据（base64编码）
	long lType,							// 类型：1－原文，2－签名证书，3－签名值，4－签名算法，5－原文摘要，6－摘要算法，7－签名数据版本，8－签名时间
	unsigned char *pbOut,				// 输出数据（签名证书、签名值、摘要值为base64编码）
	long *plOutLen);					// 输出数据的长度

// XML签名
long MYSIGN_SignDataXML(
	unsigned char *pbIn,				// 输入数据
	long lInLen,						// 输入数据的长度
	char *pszCert,						// 签名证书（base64编码）
	char *pszKeyName,					// KeyName（可为签名证书CN或NULL）
	char *pszKey,						// 签名私钥（base64编码。若使用硬件设备，应为NULL）
	FUNC_PRIV_ENC funcPrivEnc,			// 私钥加密函数（若使用软私钥，应为NULL）
	char *pszOut);						// 输出数据（base64编码）
// XML验证签名
long MYSIGN_VerifySignedDataXML(
	char *pszIn);						// 输入数据（base64编码）
// 获取XML签名信息
long MYSIGN_GetXMLSignatureInfo(
	char *pszIn,						// 输入数据（base64编码）
	long lType,							// 类型：1－xml原文，2－摘要，3－签名值，4－签名证书，5－摘要算法，6－签名算法
	unsigned char *pbOut,				// 输出数据（摘要、签名证书和签名值为base64编码）
	long *plOutLen);					// 输出数据的长度

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
	char *pszEncCerts[],						// 加密证书（base64编码）
	long lEncCertsCount,						// 加密证书的个数
	long lAlgID,								// 对称算法
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	unsigned char *pbOut,						// 输出数据
	long *plOutLen,								// 输出数据的长度
	bool bWithVerificationCode = false);		// 是否带校验码
long MYSIGN_EnvelopeDataByP7(
	unsigned char *pbIn,						// 输入数据
	long lInLen,								// 输入数据的长度
	char *pszEncCerts[],						// 加密证书（base64编码）
	long lEncCertsCount,						// 加密证书的个数
	long lAlgID,								// 对称算法
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	char *pszOut,								// 输出数据（base64编码）
	bool bWithVerificationCode = false);		// 是否带校验码
// PKCS7解密数字信封
// 若对称算法使用软算法，则funcEncOrDecData应为NULL。
// 若为国密SKF接口的硬件设备，则不需要传入funcPrivDec，funcEncOrDecData包含了导入对称密钥和对称解密。
long MYSIGN_UnenvelopeDataByP7(
	unsigned char *pbIn,						// 输入数据
	long lInLen,								// 输入数据的长度
	unsigned char *pbCert,						// 解密证书
	long lCertLen,								// 解密证书的长度
	unsigned char *pbKey,						// 解密私钥（若使用硬件设备，应为NULL）
	long lKeyLen,								// 解密私钥的长度（若使用硬件设备，应为0）
	FUNC_PRIV_DEC funcPrivDec,					// 私钥解密函数（若使用软私钥，应为NULL）
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	unsigned char *pbOut,						// 输出数据
	long *plOutLen);							// 输出数据的长度
long MYSIGN_UnenvelopeDataByP7(
	char *pszIn,								// 输入数据（base64编码）
	char *pszCert,								// 解密证书（base64编码）
	char *pszKey,								// 解密私钥（base64编码。若使用硬件设备，应为NULL）
	FUNC_PRIV_DEC funcPrivDec,					// 私钥解密函数（若使用软私钥，应为NULL）
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	unsigned char *pbOut,						// 输出数据
	long *plOutLen);							// 输出数据的长度
// 获取PKCS7数字信封信息
long MYSIGN_GetP7EnvelopeDataInfo(
	unsigned char *pbIn,						// 输入数据
	long lInLen,								// 输入数据的长度
	long lType,									// 类型：1－接收者证书的颁发者DN和序列号（DN1;序列号1|DN2;序列号2...）
	unsigned char *pbOut,						// 输出数据
	long *plOutLen);							// 输出数据的长度
long MYSIGN_GetP7EnvelopeDataInfo(
	char *pszIn,								// 输入数据（base64编码）
	long lType,									// 类型：1－接收者证书的颁发者DN和序列号（DN1;序列号1|DN2;序列号2...）
	unsigned char *pbOut,						// 输出数据
	long *plOutLen);							// 输出数据的长度

// PKCS7签名及数字信封
// lAlgID取值如下：
//   SGD_SM1_ECB
//   SGD_SM1_CBC
//   SGD_SSF33_ECB
//   SGD_SSF33_CBC
//   SGD_SM4_ECB
//   SGD_SM4_CBC
//   其它			3DES
// 若对称算法使用软算法，则funcEncOrDecData应为NULL。
long MYSIGN_SignAndEnvelopeDataByP7(
	unsigned char *pbIn,						// 输入数据
	long lInLen,								// 输入数据的长度
	unsigned char *pbSignCert,					// 签名证书
	long lSignCertLen,							// 签名证书的长度
	unsigned char *pbSignKey,					// 签名私钥（若使用硬件设备，应为NULL）
	long lSignKeyLen,							// 签名私钥的长度（若使用硬件设备，应为0）
	FUNC_PRIV_ENC funcPrivEnc,					// 私钥加密函数（若使用软私钥，应为NULL）
	char *pszEncCerts[],						// 加密证书（base64编码）
	long lEncCertsCount,						// 加密证书的个数
	long lAlgID,								// 对称算法
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	unsigned char *pbOut,						// 输出数据
	long *plOutLen,								// 输出数据的长度
	bool bWithVerificationCode = false);		// 是否带校验码
// PKCS7解密数字信封及验证签名
// 若对称算法使用软算法，则funcEncOrDecData应为NULL。
// 若为国密SKF接口的硬件设备，则不需要传入funcPrivDec，funcEncOrDecData包含了导入对称密钥和对称解密。
long MYSIGN_UnenvelopeAndVerifyDataByP7(
	unsigned char *pbIn,						// 输入数据
	long lInLen,								// 输入数据的长度
	unsigned char *pbEncCert,					// 解密证书
	long lEncCertLen,							// 解密证书的长度
	unsigned char *pbEncKey,					// 解密私钥（若使用硬件设备，应为NULL）
	long lEncKeyLen,							// 解密私钥的长度（若使用硬件设备，应为0）
	FUNC_PRIV_DEC funcPrivDec,					// 私钥解密函数（若使用软私钥，应为NULL）
	FUNC_SYMM_ENC_OR_DEC_DATA funcEncOrDecData,	// 对称加密或解密数据函数
	unsigned char *pbOut,						// 输出数据
	long *plOutLen);							// 输出数据的长度
// 获取PKCS7签名及数字信封信息
long MYSIGN_GetP7SignAndEnvelopeDataInfo(
	unsigned char *pbIn,						// 输入数据
	long lInLen,								// 输入数据的长度
	long lType,									// 类型：2－签名证书，3－签名值
	unsigned char *pbOut,						// 输出数据
	long *plOutLen);							// 输出数据的长度

#endif
