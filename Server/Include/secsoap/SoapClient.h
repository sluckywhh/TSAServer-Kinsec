// =================================================================================
//  File       : SoapClient.h
//  Purpose    : Soap Client Class Definition.
//  Programmer : ¡÷‘Ó∏÷
//  Created    : 2006-10-18
//  CopyRight (C) Kinsec
// =================================================================================

#ifndef __SOAPCLIENT_H__
#define __SOAPCLIENT_H__

// =================================================================================
//  Include Header Files
#if defined(_WIN32)
#  include <windows.h>
#endif
#include "SoapContent.h"
#include "SoapConnector.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class SS_EXPORT CSoapClient
{
public :
	// = Default constructor.
	CSoapClient();
	
	// = Default destructor.
	virtual ~CSoapClient();

	// = Get error info.
	const char * GetErrorInfo();

	// = Initialize soap client interface.
	int Initialize( const char * szIPAddr, short nPort, long lConnTimeout = 8, 
		long lSendTimeout = 15, long lRecvTimeout = 15 );

	// = Set soft cert.
	int SetSoftCert( const char * szCertPath, const char * szPin );

	// = Get current certificate serial number.
	const char * GetSerialNumber();

	// = Execute system command on server.
	// @szCommand : command string.
	// return 0 for succession.
	int SystemCommand( const char * szCommand );

#if defined(_WIN32)
	// = Upload a file with window message handle.
	int UploadFile( const char * szLocalPath, const char * szRemotePath, HWND hWnd = 0, 
		int nMessageID = 0 );

	// = Download a file with window message handle.
	int DownloadFile( const char * szLocalPath, const char * szRemotePath, HWND hWnd = 0, 
		int nMessageID = 0 );
#elif defined(_LINUX)
	// = Upload a file to remote server.
	int UploadFile( const char * szLocalPath, const char * szRemotePath );

	// = Download a file from remote server.
	int DownloadFile( const char * szLocalPath, const char * szRemotePath );
#endif

	// = Remote ip address.
	char remote_ip_[64];

	// = Remote port.
	short remote_port_;

protected :
	// = Call remote method.
	int CallRemoteMethod( const char * szMethodName, CSoapContent & request, 
		CSoapContent & response );

	// = Send request data.
	int SendRequestData( const CSoapStream & stream, CBlowfish & bf, CSoapContent & request );

	// = Recv response data.
	int RecvResponseData( const CSoapStream & stream, CBlowfish & bf, CSoapContent & response );

	// = Error string.
	char error_[256];

	// = Connect timeout seconds.
	long conn_timeout_;

	// = Send timeout seconds.
	long send_timeout_;

	// = Recv timeout seconds.
	long recv_timeout_;

private :
	// = Cert.
	void * cert_;

	// = Private key.
	void * pri_key_;
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif  // __SOAPCLIENT_H__
