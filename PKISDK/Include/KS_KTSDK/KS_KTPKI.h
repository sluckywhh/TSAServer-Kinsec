#ifndef _KS_KTPKI__H
#define _KS_KTPKI__H

// PKISDK½Ó¿Ú
//const KTChar *KTPKI_GetErrorString(KTINT32 rv);

bool KSSetLogFilePath(char *pszLogFilePath);
void KSWriteLog(const char *format, ...);
void KSWriteLog2(char *name, unsigned char *data, int len);

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

#endif
