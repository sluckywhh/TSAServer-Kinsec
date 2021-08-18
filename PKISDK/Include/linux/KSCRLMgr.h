#ifndef __KS_CRL_MGR_H__
#define __KS_CRL_MGR_H__

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

#include <string>
#include <vector>

#include "KSAutoMutex.h"

typedef std::vector<X509_CRL *> KSCRLArray;

class CKSCRLMgr
{
public :
	CKSCRLMgr();
	virtual ~CKSCRLMgr();

	void Init();
	bool AddCRL(const char *pszCRLFile);
	bool Update(bool bForceUpdate, const char *pszIssuerCN, const char *pszCRLDir);

	bool CertInCRL(X509 *cert, time_t& timeRevoked);
	bool CertInCRL(const unsigned char *pbCert, int nCertLen, time_t& timeRevoked);

	void *m_hCRLMutex;

	char m_szIssuerCN[512];
	KSCRLArray m_crls;
};

#endif
