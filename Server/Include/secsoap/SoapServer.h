// =================================================================================
//  File    : SoapServer.h
//  Purpose : Soap Server Class Definition.
//  Author  : ¡÷‘Ó∏÷
//  Created : 2006-10-18
//  CopyRight (C) KinSec.
// =================================================================================

#ifndef __SOAPSERVER_H__
#define __SOAPSERVER_H__

// =================================================================================
//  Include Header Files
#include "Blowfish.h"
#include "SoapStream.h"
#include "SoapContent.h"
#include <string.h>

// =================================================================================
//  Macro Definition  
#define SECSOAP_CLIENT_IP          "KINSEC_CLIENT_IP"
#define SECSOAP_CERT_SERIAL_NUMBER "KINSEC_CERT_SERIAL_NUMBER"
#define SECSOAP_CERT_CN            "KINSEC_CERT_CN"
#define SECSOAP_CERT_C             "KINSEC_CERT_C"
#define SECSOAP_CERT_ST            "KINSEC_CERT_ST"
#define SECSOAP_CERT_L             "KINSEC_CERT_L"
#define SECSOAP_CERT_O             "KINSEC_CERT_O"
#define SECSOAP_CERT_OU            "KINSEC_CERT_OU"
#define SECSOAP_CERT_E             "KINSEC_CERT_E"
#define SECSOAP_CERT_ISSUER_CN     "KINSEC_CERT_ISSUER_CN"
#define SECSOAP_CERT               "KINSEC_CERT"

struct S_TrustCert
{
	int len_;
	unsigned char data_[2048];
	S_TrustCert * next_;

	S_TrustCert()
	{
		len_ = 0;
		next_ = NULL;
		memset( data_, 0, 2048 );
	}
};

// =================================================================================
//  Class Definition
class SS_EXPORT CSoapServer
{
	friend class CSoapAcceptor;
public :
	// = Default constructor.
	CSoapServer();

	// = Default destructor.
	virtual ~CSoapServer();

	// = Handle http request.
	virtual int Handle();

	// = Initialize soap server interface.
	int Initialize( long lSendTimeout = 30, long lRecvTimeout = 30 );

	// = Add allow ca.
	int AddRootCert( unsigned char * pData, int nLen );

protected :
	// = Find function and execute.
	virtual int FindFnDoHandle( const char * szMethod );

	// = Synchronous.
	int Synchronous();

	// = Execute system command.
	int SystemCommand( const char * szCommand );

	// = Handle upload file.
	int UploadFile( const char * szSavePath, __int64 lFileSize );

	// = Handle download file.
	int DownloadFile( const char * szFromPath );

	// = Send response data.
	int SendResponseData( bool bOk );

	// = Recv request data.
	int RecvRequestData();

	// = Crypt.
	CBlowfish bf_;

	// = Stream.
	CSoapStream stream_;

	// = Request content.
	CSoapContent request_;

	// = Response content.
	CSoapContent response_;

	// = Client cert info.
	CSoapContent cert_info_;

	// = Client ip.
	char url_[64];

	// = Send timeout seconds.
	long send_timeout_;

	// = Recv timeout seconds.
	long recv_timeout_;

	// = Trust cert list.
	S_TrustCert * trust_certs_;
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif  // __SOAPSERVER_H__
