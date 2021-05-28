#ifndef _KSSIGNLIBFUNC_H
#define _KSSIGNLIBFUNC_H

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
#ifdef WIN32
	typedef unsigned long   DWORD;
#else
	typedef unsigned int    DWORD;
#endif
#endif
#ifndef HANDLE
typedef void *				HANDLE;
#endif

////////////////////////////////////////////////////////////////
// 回调函数

// 以下函数成功返回0，失败返回其它
// 私钥加密
typedef int (*KS_FUNC_PRIV_ENC)(
	void *h,
	unsigned char *in,
	int inlen,
	unsigned char *out,
	int *outlen);

// 私钥解密
typedef int (*KS_FUNC_PRIV_DEC)(
	void *h,
	unsigned char *in,
	int inlen,
	unsigned char *out,
	int *outlen);

// 用证书验证签名
typedef int (*KS_FUNC_CERT_VRF_SIGN)(
	void *h,
	bool bIsDigest,
	unsigned char *data,				// 原文或摘要（若为摘要，对于RSA算法，摘要为原文摘要的结果；对于SM2算法，摘要为原文签名预处理的结果）
	int datalen,
	unsigned char *sign,
	int signlen,
	unsigned char *cert,
	int certlen);

// 用证书加密
typedef int (*KS_FUNC_CERT_ENC)(
	void *h,
	unsigned char *in,
	int inlen,
	unsigned char *cert,
	int certlen,
	unsigned char *out,
	int *outlen);

// 对称加密
typedef int (*KS_FUNC_SYMM_ENC)(
	void *h,							// 设备句柄
	int algid,							// 对称算法标识
	bool padding,						// 是否填充
	unsigned char *key,					// 密钥
	int keylen,							// 密钥的长度
	bool keyEncrypted,					// 密钥是否为密文
	unsigned char *iv,					// 初始向量
	int ivlen,							// 初始向量的长度
	unsigned char *in,					// 输入数据
	int inlen,							// 输入数据的长度
	unsigned char *out,					// 输出数据
	int *outlen);						// 输出数据的长度

// 对称解密
typedef int (*KS_FUNC_SYMM_DEC)(
	void *h,							// 设备句柄
	int algid,							// 对称算法标识
	bool padding,						// 是否去填充
	unsigned char *key,					// 密钥
	int keylen,							// 密钥的长度
	bool keyEncrypted,					// 密钥是否为密文
	unsigned char *iv,					// 初始向量
	int ivlen,							// 初始向量的长度
	unsigned char *in,					// 输入数据
	int inlen,							// 输入数据的长度
	unsigned char *out,					// 输出数据
	int *outlen);						// 输出数据的长度

// 摘要
typedef int (*KS_FUNC_DIGEST)(
	void *h,							// 设备句柄
	int algid,							// 摘要算法标识
	unsigned char *in,					// 输入数据
	int inlen,							// 输入数据的长度
	unsigned char *cert,				// 证书（若证书为NULL，则进行摘要运算；若证书为SM2证书，且摘要算法为SM3，则进行SM2签名预处理运算）
	int certlen,						// 证书的长度
	unsigned char *out,					// 输出数据
	int *outlen);						// 输出数据的长度

// 文件摘要
typedef int (*KS_FUNC_DIGEST_FILE)(
	void *h,							// 设备句柄
	int algid,							// 摘要算法标识
	char *infile,						// 输入文件
	unsigned char *cert,				// 证书（若证书为NULL，则进行摘要运算；若证书为SM2证书，且摘要算法为SM3，则进行SM2签名预处理运算）
	int certlen,						// 证书的长度
	unsigned char *out,					// 输出数据
	int *outlen);						// 输出数据的长度

////////////////////////////////////////////////////////////////
// 初始化
int KSSIGN_Initialize();
int KSSIGN_Finalize();

////////////////////////////////////////////////////////////////
// PKCS7签名
// nDigestAlgID取值如下：
//   SGD_SHA1
//   SGD_SHA256
//   SGD_SM3
int KSSIGN_SignDataByP7(
	void *hDevice,						// 设备句柄
	KS_FUNC_PRIV_ENC funcPrivEnc,		// 私钥加密函数（若使用软私钥，应为NULL）
	KS_FUNC_DIGEST funcDigest,			// 摘要函数（若使用软算法，应为NULL）
	bool bAttached,						// 是否包含原文
	bool bCert,							// 签名是否包含证书
	bool bHasAttr,						// 是否包含签名属性（authenticated attributes）
	bool bIsDigest,						// 输入数据是否为摘要（对于SM2算法，如果签名不包含签名属性，摘要为原文签名预处理的结果；否则为原文SM3摘要的结果）
	int nDigestAlgID,					// 摘要算法
	bool bWithVerificationCode,			// 是否带校验码
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	unsigned char *pbCert,				// 签名证书
	int nCertLen,						// 签名证书的长度
	unsigned char *pbKey,				// 签名私钥（若使用硬件设备，应为NULL）
	int nKeyLen,						// 签名私钥的长度（若使用硬件设备，应为0）
	unsigned char *pbTS,				// 时间戳数据
	int nTSLen,							// 时间戳数据的长度
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

// PKCS7验证签名
int KSSIGN_VerifySignedDataByP7(
	void *hDevice,						// 设备句柄
	KS_FUNC_CERT_VRF_SIGN funcCVS,		// 验证签名函数（若使用软算法，应为NULL）
	KS_FUNC_DIGEST funcDigest,			// 摘要函数（若使用软算法，应为NULL）
	unsigned char *pbP7Sign,			// PKCS7签名
	int nP7SignLen,						// PKCS7签名的长度
	unsigned char *pbCert,				// 签名证书
	int nCertLen,						// 签名证书的长度
	unsigned char *pbData = NULL,		// 原文
	int nDataLen = 0,					// 原文的长度
	bool bIsDigest = false);			// 原文是否为摘要（对于SM2算法，如果签名不包含签名属性，摘要为原文签名预处理的结果；否则为原文SM3摘要的结果）

// 获取PKCS7签名信息
int KSSIGN_GetP7SignDataInfo(
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	int nType,							// 类型：1－原文，2－签名证书，3－签名值，4－签名算法，5－原文摘要，6－摘要算法，7－签名数据版本，8－签名时间，9－时间戳
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

////////////////////////////////////////////////////////////////
// PKCS7数字信封
// nSymmAlgID取值如下：
//   SGD_SM1_ECB
//   SGD_SM1_CBC
//   SGD_SSF33_ECB
//   SGD_SSF33_CBC
//   SGD_SM4_ECB
//   SGD_SM4_CBC
//   其它			3DES
int KSSIGN_EnvelopeDataByP7(
	void *hDevice,						// 设备句柄
	KS_FUNC_CERT_ENC funcCertEnc,		// 证书加密函数（若使用软算法，应为NULL）
	KS_FUNC_SYMM_ENC funcEnc,			// 对称加密函数（若使用软算法，应为NULL）
	bool bWithVerificationCode,			// 是否带校验码
	int nSymmAlgID,						// 对称算法
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	char *pszEncCerts[],				// 加密证书（base64编码）
	int nEncCertsCount,					// 加密证书的个数
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

// PKCS7解密数字信封
int KSSIGN_UnenvelopeDataByP7(
	void *hDevice,						// 设备句柄
	KS_FUNC_PRIV_DEC funcPrivDec,		// 私钥解密函数（若使用软私钥，应为NULL）
	KS_FUNC_SYMM_DEC funcDec,			// 对称解密函数（若使用软算法，应为NULL）
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	unsigned char *pbCert,				// 解密证书
	int nCertLen,						// 解密证书的长度
	unsigned char *pbKey,				// 解密私钥（若使用硬件设备，应为NULL）
	int nKeyLen,						// 解密私钥的长度（若使用硬件设备，应为0）
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

// 获取PKCS7数字信封信息
int KSSIGN_GetP7EnvelopeDataInfo(
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	int nType,							// 类型：1－接收者证书的颁发者DN和序列号（DN1;序列号1|DN2;序列号2...），2－非对称算法，3－非对称密文，4－对称算法，5－对称密文
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

////////////////////////////////////////////////////////////////
// PKCS7签名及数字信封
// nDigestAlgID取值如下：
//   SGD_SHA1
//   SGD_SHA256
//   SGD_SM3
// nSymmAlgID取值如下：
//   SGD_SM1_ECB
//   SGD_SM1_CBC
//   SGD_SSF33_ECB
//   SGD_SSF33_CBC
//   SGD_SM4_ECB
//   SGD_SM4_CBC
//   其它			3DES
int KSSIGN_SignAndEnvelopeDataByP7(
	void *hDevice,						// 设备句柄
	KS_FUNC_PRIV_ENC funcPrivEnc,		// 私钥加密函数（若使用软私钥，应为NULL）
	KS_FUNC_DIGEST funcDigest,			// 摘要函数（若使用软算法，应为NULL）
	KS_FUNC_CERT_ENC funcCertEnc,		// 证书加密函数（若使用软算法，应为NULL）
	KS_FUNC_SYMM_ENC funcEnc,			// 对称加密函数（若使用软算法，应为NULL）
	bool bWithVerificationCode,			// 是否带校验码
	int nDigestAlgID,					// 摘要算法
	int nSymmAlgID,						// 对称算法
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	unsigned char *pbSignCert,			// 签名证书
	int nSignCertLen,					// 签名证书的长度
	unsigned char *pbSignKey,			// 签名私钥（若使用硬件设备，应为NULL）
	int nSignKeyLen,					// 签名私钥的长度（若使用硬件设备，应为0）
	char *pszEncCerts[],				// 加密证书（base64编码）
	int nEncCertsCount,					// 加密证书的个数
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

// PKCS7解密数字信封及验证签名
int KSSIGN_UnenvelopeAndVerifyDataByP7(
	void *hDevice,						// 设备句柄
	KS_FUNC_PRIV_DEC funcPrivDec,		// 私钥解密函数（若使用软私钥，应为NULL）
	KS_FUNC_SYMM_DEC funcDec,			// 对称解密函数（若使用软算法，应为NULL）
	KS_FUNC_CERT_VRF_SIGN funcCVS,		// 验证签名函数（若使用软算法，应为NULL）
	KS_FUNC_DIGEST funcDigest,			// 摘要函数（若使用软算法，应为NULL）
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	unsigned char *pbEncCert,			// 解密证书
	int nEncCertLen,					// 解密证书的长度
	unsigned char *pbEncKey,			// 解密私钥（若使用硬件设备，应为NULL）
	int nEncKeyLen,						// 解密私钥的长度（若使用硬件设备，应为0）
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

// 获取PKCS7签名及数字信封信息
int KSSIGN_GetP7SignAndEnvelopeDataInfo(
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	int nType,							// 类型：1－接收者证书的颁发者DN和序列号（DN1;序列号1|DN2;序列号2...），2－签名证书，3－签名值，4－签名算法，5－原文摘要，6－摘要算法，7－版本，8－签名时间
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

////////////////////////////////////////////////////////////////
// 时间戳请求
int KSSIGN_CreateTimeStampReq(
	void *hDevice,						// 设备句柄
	KS_FUNC_DIGEST funcDigest,			// 摘要函数（若使用软算法，应为NULL）
	int nDigestAlgID,					// 摘要算法
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

// 时间戳
int KSSIGN_CreateTimeStamp(
	void *hDevice,						// 设备句柄
	KS_FUNC_PRIV_ENC funcPrivEnc,		// 私钥加密函数（若使用软私钥，应为NULL）
	KS_FUNC_DIGEST funcDigest,			// 摘要函数（若使用软算法，应为NULL）
	int nDigestAlgID,					// 摘要算法
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	unsigned char *pbCert,				// 时间戳证书
	int nCertLen,						// 时间戳证书的长度
	unsigned char *pbKey,				// 时间戳私钥（若使用硬件设备，应为NULL）
	int nKeyLen,						// 时间戳私钥的长度（若使用硬件设备，应为0）
	char *pszSN,						// 时间戳序列号
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

// 验证时间戳
int KSSIGN_VerifyTimeStamp(
	void *hDevice,						// 设备句柄
	KS_FUNC_CERT_VRF_SIGN funcCVS,		// 验证签名函数（若使用软算法，应为NULL）
	KS_FUNC_DIGEST funcDigest,			// 摘要函数（若使用软算法，应为NULL）
	unsigned char *pbTS,				// 时间戳数据
	int nTSLen,							// 时间戳数据的长度
	unsigned char *pbCert,				// 时间戳证书
	int nCertLen,						// 时间戳证书的长度
	unsigned char *pbData = NULL,		// 原文
	int nDataLen = 0,					// 原文的长度
	bool bIsDigest = false);			// 原文是否为摘要

// 获取RSA时间戳信息
int KSSIGN_GetTimeStampInfo(
	unsigned char *pbIn,				// 输入数据
	int nInLen,							// 输入数据的长度
	int nType,							// 类型：1－原文，2－时间戳证书，3－签名值，4－签名算法，5－原文摘要，6－摘要算法，7－版本，8－时间戳时间
	unsigned char *pbOut,				// 输出数据
	int *pnOutLen);						// 输出数据的长度

#endif
