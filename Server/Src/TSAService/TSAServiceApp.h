// =================================================================================
//  File    : TSAServiceApp.h
//  Purpose : 时间戳服务器服务类
//  Author  : 林灶钢
//  Created : 2006-12-29
//  CopyRight (C) 林灶钢
// =================================================================================

#ifndef __TSASERVICEAPP_H__
#define __TSASERVICEAPP_H__

// =================================================================================
//  Include Header Files

#include "ASN1PKI.hpp"
#include "TSASvrServer.h"
#include "DK_ThreadPool.h"
#include "DK_SocketAcceptor.h"
#include <pthread.h>


// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  define SETTING_FILE "SysSetting\\TSASetting.xml"
#elif defined(_LINUX)
#  define SETTING_FILE "SysSetting/TSASetting.xml"
#endif

// =================================================================================
//  Class Definition
class CTSAServiceApp
{
public : 
	// = Default constructor.
	CTSAServiceApp();

	// = Default destructor.
	virtual ~CTSAServiceApp();

	// = Start.
	int start();

	// = Thread pool.
	DK_ThreadPool m_threadPool;

	// = Acceptor.
	DK_SocketAcceptor m_acceptor;

	// = TSA certificate.
	KTCertificate m_certificate;

	// = TSA private.
	KTByteArray m_rPriKey;

	// = Is using database.
	bool m_bUsingDb;
	
	X509* m_SerCert;
	
	EVP_PKEY* m_SerPriKey;
	
	X509* m_SerCertCa;
		
	X509* m_SerCertRoot;
	
	time_t m_CertBeginValid;
	
	time_t m_CertEndValid;
	
	pthread_mutex_t m_lock;
private :
	// = Service port.
	short m_nServicePort;

	// = Parallel count.
	int m_nParallelCount;
	
};
extern CTSAServiceApp g_app;

// =================================================================================
//  1.00    2006-12-29    Created By LinZaoGang
//  File End 
#endif  // __TSASERVICEAPP_H__  
