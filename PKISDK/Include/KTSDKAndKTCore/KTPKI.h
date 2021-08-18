#ifndef _KTPKI_H
#define _KTPKI_H

// PKISDK接口
//const KTChar *KTPKI_GetErrorString(KTINT32 rv);

const KTChar *KTPKI_UI_SetErr(int nErr, const KTChar pc_format[], ...);
const KTChar *KTPKI_UI_SetErr(const KTChar pc_format[], ...);

KTINT32 KTPKI_Verify(
	KTCertificate& signCert,
	unsigned char *pbData,
	KTUINT32 unDataLen,
	unsigned char *pbSign,
	KTUINT32 unSignLen);

KTINT32 KTPKI_VerifyCert(
	const KTCertificate& cert,
	const KTCertificate& issuerCert,
	KTCRL *pCRL,						//may be NULL
	enumCertStatus *pnCertStatus);

KTINT32 KTPKI_VerifyCert(
	const KTCertificate& cert,
	const KTCertificates& certs,
	KTCRL *pCRL,						//may be NULL
	enumCertStatus *pnCertStatus);

void KTPKI_TransCertStatus(
	int nCertStatus,
	KTChar *pszCertStatus);

KTINT32 KTPKI_GetCertChain(
	KTCertificate& cert,
	KTCertificates& certs);

KTINT32 KTPKI_GenerateRandom(
	KTINT nRandomLen,
	KTBYTE *pOutput);

KTINT32 KTPKI_PKCS5Padding(
	BYTE *data,
	KTUINT& datalen,
	KTBOOL32 encrypt);

KTINT32 KTPKI_EncryptOrDecrypt(
	IN KTBOOL32				bEncrypt,				// 是否加密
	IN KTUINT				unAlgID,				// 对称算法标识
	IN BYTE					*pbKey,					// 密钥
	IN KTUINT				unKeyLen,				// 密钥的长度
	IN KTBOOL32				bKeyEncrypted,			// 密钥是否为密文
	IN BYTE					*pbIV,					// 初始向量
	IN KTUINT				unIVLen,				// 初始向量的长度
	IN BYTE					*pbIn,					// 输入数据
	IN KTUINT				unInLen,				// 输入数据的长度
	OUT BYTE				*pbOut,					// 输出数据
	IN OUT KTUINT			*punOutLen);			// 输出数据的长度

typedef std::vector<KTByteArray> 	KTByteArrayArray;

bool KTSDK_LoadCACerts2();

KTINT32 KTPKI_GetCertChain2(
	unsigned char *cert,
	int certlen,
	unsigned char *cacert,
	int *cacertlen);

KTINT32 KTPKI_GetCertChain2(
	unsigned char *cert,
	int certlen,
	KTByteArrayArray& certs);

KTINT32 KTPKI_VerifyRootCert(
	unsigned char *cert,
	int certlen);

KTINT32 KTPKI_VerifyCert2(
	unsigned char *cert,
	int certlen,
	unsigned char *cacert,
	int cacertlen,
	enumCertStatus *pnCertStatus);

#endif