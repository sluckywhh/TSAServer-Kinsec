#ifndef __TSENGINE_H__
#define __TSENGINE_H__
#include <openssl/x509.h>
#include <string>
using std::string;


class CTSEngine
{
public : 
	CTSEngine();
	virtual ~CTSEngine();
	void InitSerCer(X509 *RsaCert, EVP_PKEY *RsaPKey, X509 *RsaCa, X509 *RsaRootCa, 
	time_t RsaBeforeValid, time_t RsaAfterValid, char *Sm2Cert, char *Sm2PKey, 
	X509 *Sm2Ca, X509 *Sm2RootCa, time_t Sm2BeforeValid, time_t Sm2AfterValid);
	int CreateTSReq(unsigned char *data, int datalen, unsigned char *tsreq,	int *tsreqlen	);
	int CreateTSResp(unsigned char *tsreq, int tsreqlen, X509 *rootcacert, X509 *cacert,
	X509 *cert, EVP_PKEY *pkey,	unsigned char *tsresp, int *tsresplen,	unsigned char *tstoken,	int *tstokenlen, std::string &serialNumber);
	int CreateSM2TSResp(unsigned char *tsreq, int tsreqlen, X509 *rootcacert, X509 *cacert,
	char *cert, char *pkey,	unsigned char *tsresp, int* tsresplen,	unsigned char *tstoken,	int *tstokenlen, std::string &serialNumber);
	int VerifyTSToken(unsigned char *data, int datalen, X509 *rootcacert,  X509 *cacert, unsigned char *tstoken, int tstokenlen);
	int ParseTSToken(unsigned char *tstoken, int tstokenlen, char *pszRes, int flag);
	
public:	
	X509 *m_RsaCert;
	EVP_PKEY *m_RsaPkey;
	X509 *m_RsaCa;
	X509 *m_RsaRootCa;
	time_t m_RsaBeforeValid;
	time_t m_RsaAfterValid;
	char *m_Sm2Cert;
	char *m_Sm2Pkey;
	X509 *m_Sm2Ca;
	X509 *m_Sm2RootCa;
	time_t m_Sm2BeforeValid;
	time_t m_Sm2AfterValid;
};

#endif

