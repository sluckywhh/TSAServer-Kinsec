// =================================================================================
//  File    : TSASvrServer.h
//  Author  : ¡÷‘Ó∏÷
//  Created : 2006-12-29
//  CopyRight (C) ¡÷‘Ó∏÷.
// =================================================================================

#ifndef __TSASVRSERVER_H__
#define __TSASVRSERVER_H__

// =================================================================================
//  Include Header Files
#include "DK_Worker.h"
#include "DK_SocketParam.h"
#include "DK_SocketStream.h"
#include "TSAEngine.h"

// =================================================================================
//  Macro Definition 
#define ERROR_INFO_NAME "ErrorInfo"
#define ERROR_INFO_CODE "ErrorCode"  

// =================================================================================
//  Class Definition
class CTSASvrServer : public DK_Worker
{
public :
	// = Default constructor.
	CTSASvrServer( const KTCertificate & cert, const KTByteArray & rPriKey, bool bUsingDb, 
	X509* SerCert, EVP_PKEY* SerPriKey, X509* SerCertCa, X509* SerCertRoot, 
	time_t CertBeginValid, time_t CertEndValid, pthread_mutex_t* plock);

	// = Default destructor.
	virtual ~CTSASvrServer();

	// = Socket stream.
	DK_SocketStream m_stream;

protected :
	// = Process request.
	virtual int processRequest( const DK_Request & request );

	// = Send response data.
	int sendResponse( const DK_SocketParam & resp, bool bOk );

	// = Recv request data.
	int recvRequest( DK_SocketParam & req );

private :
	// = TSAEngine.
	CTSAEngine m_engine;

	// = Using database.
	bool m_bUsingDb;
	
	pthread_mutex_t* m_plock;
	
};

// =================================================================================
//  1.00    2006-12-29    Created By LinZaoGang
//  File End 
#endif  // __TSASVRSERVER_H__
