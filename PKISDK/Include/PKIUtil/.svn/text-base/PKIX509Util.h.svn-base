#ifndef __PKI_X509_CERT_UTIL_H__
#define __PKI_X509_CERT_UTIL_H__

/**
  * @Name: PKIX509Util.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 01:56
  * @Description: API functions about X.509 cert 
*/ 

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#undef EXPORTS
#ifdef WIN32
	#ifndef STATIC_LIB
		#ifdef KT_DLL // assume this is defined when we build the dll
			#define EXPORTS __declspec( dllexport)
		#else
			#define EXPORTS __declspec( dllimport)
		#endif
	#else
		#define EXPORTS  
	#endif
#else
	#define EXPORTS
#endif  //WIN32

class KTCertificate;
class KTCertificates;
class KTEnvelopedData;
class KTCertificationRequest;
class CKey;

EXPORTS KTINT32 
KTPKI_Sign(
		IN KT_SESSION_HANDLE		hSession,
		IN KT_CIPHER_MECHANISM		nSignInfo,
		IN const KTCertificate&		cert,
		IN const BYTE*				pData,
		IN KTUINT					nDataLen,
		OUT BYTE*				pSignedData,
		IN OUT	KTUINT*			pnSignedDataLen
);

EXPORTS KTINT32 
KTPKI_SignCert(
		IN KT_SESSION_HANDLE		hSession,
		IN KT_CIPHER_MECHANISM		nSignInfo,		
			// KMT_SHA1_RSA_PKCS | KMT_SIGNATURE or KMT_MD5_RSA_PKCS | KMT_SIGNATURE
		IN const KTCertificate&		issuerCert,
		IN OUT	KTCertificate&		cert
);

EXPORTS KTINT32 
KTPKI_SignCRL(
		IN KT_SESSION_HANDLE		hSession,
		IN KT_CIPHER_MECHANISM		nSignInfo,		
			// KMT_SHA1_RSA_PKCS | KMT_SIGNATURE or KMT_MD5_RSA_PKCS | KMT_SIGNATURE
		IN const KTCertificate&		cert,
		IN OUT KTCRL&				crl
);

EXPORTS KTINT32 
KTPKI_Verify(
		IN KT_SESSION_HANDLE		hSession,
		IN const KTCertificate&		cert,
		IN const BYTE*				pData,
		IN KTUINT					nDataLen,
		IN const BYTE*				pSignedData,
		IN KTUINT					nSignedDataLen
);


EXPORTS KTINT32 
KTPKI_CertFindPrvKey(
		IN KT_SESSION_HANDLE		hSession,
		IN const KTCertificate&		cert,
		KTHANDLE*				phPrv
);



/////////////////////////////////////////////////////////////////////
//PKCS10 request
EXPORTS KTINT32
KTPKI_CreatePKCS10Request(
	IN KT_SESSION_HANDLE			hSession,
	IN KTINT						nKeyLen,
	IN KT_CIPHER_MECHANISM			nMech,	//KMT_SHA1_RSA_PKCS or KMT_MD5_RSA_PKCS
	IN OUT KTCertificationRequest&	CertReq,
	OUT KTHANDLE *					phPrvKey
);

EXPORTS KTINT32
KTPKI_CreatePKCS10Request2(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_CIPHER_MECHANISM			nMech,	//KMT_SHA1_RSA_PKCS or KMT_MD5_RSA_PKCS
	IN KTHANDLE						hPubKey,
	IN OUT KTCertificationRequest&	CertReq
);        

EXPORTS KTINT32
KTPKI_CreatePKCS10RequestExportPrvKey(
	IN KT_SESSION_HANDLE			hSession,
	IN KTINT						nKeyLen,
	IN KT_CIPHER_MECHANISM			nMech,	//KMT_SHA1_RSA_PKCS or KMT_MD5_RSA_PKCS
	OUT BYTE*					pPubKey,
	IN OUT KTUINT32	*				pnPubKeyLen,
	OUT BYTE*					pPrvKey,
	IN OUT KTUINT32 *				pnPrvKeyLen,
	IN OUT KTCertificationRequest&	CertReq,
	IN KTBOOL						bKeepKey = FALSE,
	OUT KTHANDLE *					phPrvKey = NULL
);

/////////////////////////////////////////////////////////////////
EXPORTS KTINT32 
KTPKI_BasicEnvelop(
	IN KT_SESSION_HANDLE		hSession,
	IN const KTCertificate&		cert,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen,
	OUT BYTE*				pEnvelopedData,
	IN OUT	KTUINT*			pnEnvelopedDataLen
);

EXPORTS KTINT32
KTPKI_BasicUnEnvelop(
	IN KT_SESSION_HANDLE		hSession,
	IN const KTCertificate&		cert,
	IN const BYTE*				pEnvelopedData,
	IN KTUINT					nEnvelopedDataLen,
	OUT BYTE*				pData,
	IN OUT	KTUINT*			pnDataLen
);

/////////////////////////////////////////////////////////////////////
//PKCS7

//数字签名
EXPORTS KTINT32
KTPKI_Pkcs7Sign(
	IN KT_SESSION_HANDLE		hSession,
	IN KTCertificates			&certs,
	IN KT_MECHANISM_TYPE		nDigestMech,
	IN const char* const		pcDataType,
	IN KTObject					&dataObj,
	OUT KTSignedData			&sSignedData
);

//验证数字签名
EXPORTS KTINT32
KTPKI_Pkcs7Verify(
	IN KT_SESSION_HANDLE		hSession,
	IN KTCertificates			&certs,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen
);

//加数字信封
EXPORTS KTINT32
KTPKI_Pkcs7Envelop(
	IN KT_SESSION_HANDLE		hSession,
	IN const KTCertificates&	certs,
	IN KT_CIPHER_MECHANISM		nMech,		//	对称加密算法模式，目前支持DES算法CBS模式
											//	应该传入KMT_DES_CBC|KMT_ENCRYPT
	IN const char* const	    pcDataType,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen,
	OUT KTEnvelopedData&		sEnvelopData,
	IN CKey						*pKey = NULL
	//	由于仅支持DES算法CBS模式，应该是指向CDESKey的指针
	//	根据Pkcs#7规范，内容加密密钥应该每次随机生成，故调用此函数时应尽量不传pKey参数
);

//解信封
EXPORTS KTINT32
KTPKI_Pkcs7UnEnvelop(
	IN KT_SESSION_HANDLE		hSession,
	IN KTCertificate&			cert,
	IN const BYTE*				pEnvelopedData,
	IN KTUINT					nEnvelopedDataLen,
	OUT BYTE*				    pData,
	IN OUT KTUINT*			    pnDataLen
);	


/////////////////////////////////////////////////////////////////////
// 证书使用
/* Description: 获取证书链
 * Params: 
 *	hSession - [in] 
 *	Cert - [in] 要搜索的证书
 *	Certs - [out] 返回的证书链，其顺序为：Cert，Cert parent, ...
 * Return:
 *	KT_OK : 成功；else: 失败
 */
EXPORTS KTINT32
KTPKI_GetCertChain(
  IN KT_SESSION_HANDLE	hSession,
  IN const KTCertificate& Cert,
  OUT KTCertificates& Certs
);

//设置证书链更新状态，判断是否要清空缓存更新
EXPORTS KTINT32
KTPKI_SetCertChainUpdataState(
  IN KTINT iState
);

EXPORTS KTINT32
KTPKI_GetCertTrustLevel(
  IN KT_SESSION_HANDLE	    hSession,
  IN const KTCertificate&   Cert,
  OUT KTUINT*			    pnTrustLevel);

enum enumCertStatus
{
	StatusOK,    			// 证书没有问题
	StatusBeforeinEffect, 	// 证书尚未生效
	StatusExpired,   		// 证书已经过期
	StatusTimeBeforeIssuer, // 证书比发行者先生效
	StatusTimeAfterIssuer,	// 证书比发行者后作废
	StatusChainBad,   		// 没有有效证书链
	StatusUnAuthorized,  	// 签名验证失败
	StatusParentCertError, 	// 上级证书未通过验证
	StatusRevoked,   		// 证书已被废除
	StatusUnspecify,		// 未指定错误
	StatusOKWithTSS,		// 证书没有问题,并且支持时间戳
	StatusTSSCertNotTrust	// 时间戳证书不受信任
	
};

/* Description: 验证证书
 * Params:
 *	hSession - [in] 
 *	Cert - [in] 需要验证的证书
 *	pCerts - [in] 发行者所在的证书链，若为NULL, 将从TOKEN中得到整个证书链
 *	pCRL - [in] 黑名单。若为NULL，不检查这一项
 *	bChainVerify - [in] TRUE:验证整个证书链；FALSE:只验证Cert
 *	pnCertStatus - [out] 验证结果，见enumCertStatus定义
 * Return:
 *	KT_OK : 成功，不管证书状态如何，一般总返回此值；
 *	Else: 程序失败，而不是证书验证失败。
 */
EXPORTS KTINT32 
KTPKI_VerifyCert(
  IN KT_SESSION_HANDLE	hSession,
  IN const KTCertificate& Cert,
  IN KTCertificates* const pCerts,	// may be=NULL, use Certs in hSession
  IN KTCRL *const pCRL,				// may be=NULL
  IN const KTBOOL bChainVerify,		// verify cert chain
  OUT enumCertStatus *pnCertStatus
);
 
/* Description: 验证证书
 * Params:
 *	hSession - [in] 
 *	Cert - [in] 需要验证的证书
 *	pCerts - [in] 发行者所在的证书链，若为NULL, 将从TOKEN中得到整个证书链
 *	bCheckCRL - [in] 是否检查CRL
 *	bChainVerify - [in] TRUE:验证整个证书链；FALSE:只验证Cert
 *	pnCertStatus - [out] 验证结果，见enumCertStatus定义
 * Return:
 *	KT_OK : 成功，不管证书状态如何，一般总返回此值；
 *	Else: 程序失败，而不是证书验证失败。
 */
EXPORTS KTINT32
KTPKI_VerifyCert2(
  IN KT_SESSION_HANDLE	hSession,
  IN const KTCertificate& Cert,
  IN KTCertificates* const pCerts,
  IN const KTBOOL bCheckCRL,
  IN const KTBOOL bChainVerify,
  OUT enumCertStatus *pnCertStatus
);

/* Description: 从hSession中得到CA证书的黑名单（CRL）
 * Params:
 *	hSession - [in] 
 *	Cert - [in] 发布CRL的证书
 *	crl - [out] 找到的黑名单
 * Return:
 *	KT_OK : 成功，找到黑名单
 *	KT_ERR_OBJ_NOT_FOUND: 黑名单未找到
 *	Else: 程序失败
 */
EXPORTS KTINT32
KTPKI_GetCACRL(
  IN KT_SESSION_HANDLE	hSession,
  IN const KTCertificate& CACert,
  OUT KTCRL& crl
);


EXPORTS KTINT32
KTPKI_GetCACRL_Open(
  IN KT_SESSION_HANDLE hSession,
  IN const KTCertificate& CACert,
  OUT COpenCRL& OpenCRL
);

EXPORTS KTINT32
KTPKI_VerifyCert_Open(
  IN KT_SESSION_HANDLE hSession,
  IN const KTCertificate& Cert,
  IN const KTCertificates* pCerts,
  IN const COpenCRL* pOpenCRL,
  IN const KTBOOL bChainVerify,		// verify cert chain
  OUT enumCertStatus *pnCertStatus
);

EXPORTS KTINT32
KTPKI_VerifyCert2_Open(
  IN KT_SESSION_HANDLE	hSession,
  IN const KTCertificate& Cert,
  IN const KTCertificates* pCerts,
  IN const KTBOOL bCheckCRL,
  IN const KTBOOL bChainVerify,
  OUT enumCertStatus *pnCertStatus
);

EXPORTS KTINT32
KTPKI_Util_Thread_Initialize();

EXPORTS KTINT32
KTPKI_Util_Thread_Finalize();

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 01:56  Hongwei Yu   created 

#endif
