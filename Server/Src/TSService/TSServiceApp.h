#ifndef __TSSERVICEAPP_H__
#define __TSSERVICEAPP_H__

#include "DK_ThreadPool.h"
#include "DK_SocketAcceptor.h"
#include <openssl/asn1.h>
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
#include <openssl/pkcs12.h>
#include <pthread.h>


class CTSServiceApp
{
public : 
	CTSServiceApp();
	virtual ~CTSServiceApp();
	int start();
	DK_ThreadPool m_threadPool;
	DK_SocketAcceptor m_acceptor;
	
public :
	X509* m_RsaCert;
	EVP_PKEY* m_RsaPKey;
	X509* m_RsaCa;
	X509* m_RsaRootCa;
	time_t m_RsaBeforeValid;
	time_t m_RsaAfterValid;
	char m_Sm2Cert[4096];
	char m_Sm2PKey[1024];
	X509* m_Sm2Ca;
	X509* m_Sm2RootCa;
	time_t m_Sm2BeforeValid;
	time_t m_Sm2AfterValid;
	pthread_mutex_t m_lock;
	bool m_bDb;
	
private :
	short m_nServicePort;
	int m_nParallelCount;
	
};


#endif 
