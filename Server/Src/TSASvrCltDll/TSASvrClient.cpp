// =================================================================================
//  File       : TSASvrClient.cpp
//  Programmer : 林灶钢
//  Created    : 2006-11-28
//  CopyRight (C) 林灶钢
// =================================================================================

// =================================================================================
//  Include Header Files
#include <stdio.h>
#if defined( _WIN32 )
#  include <winsock2.h>
#elif defined( _LINUX )
#  include <unistd.h>
#  include <pthread.h>
#  include <arpa/inet.h>
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#  include <string.h>
#  include <stdlib.h>
#endif
#include "TSASvrClient.h"
#include "TSErr.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
// = Initialize soap client interface.
int CTSASvrClient::Initialize( const char * szUrl, long lConnTimeoutSecs /* = 20 */ )
{
	const char * pBegin = strstr( szUrl, "http://" ) ? szUrl + strlen( "http://" ) : szUrl;
	const char * pEnd = strstr( pBegin, ":" );
	if( pEnd == NULL )
		return TSERR_INVALID_URL;
	memset( this->remote_ip_, 0, sizeof( this->remote_ip_ ) );
	memcpy( this->remote_ip_, pBegin, pEnd - pBegin );
	this->remote_port_ = atoi( pEnd + 1 );
	this->conn_timeout_ = lConnTimeoutSecs;
	return 0;
}

// = Get current time.
int CTSASvrClient::GetTSATime( std::string & curTime )
{
	CTSASvrContent request;
	CTSASvrContent response;
	int nErr = this->CallRemoteMethod( "GetTSATime", request, response );
	if( nErr != 0 )
		return nErr;
	curTime = response.GetValue( "CurrentTime" );
	return 0;
}

// = Get tsa cert.
int CTSASvrClient::GetTSACert( std::string & certData )
{
	CTSASvrContent request;
	CTSASvrContent response;
	int nErr = this->CallRemoteMethod( "GetTSACert", request, response );
	if( nErr != 0 )
		return nErr;


	certData = response.GetValue( "CertData" );
	return 0;
}

// = Issuer time stamp.
int CTSASvrClient::IssuerTimeStamp( const char * szSrc, std::string & respData )
{
	CTSASvrContent request;
	CTSASvrContent response;
	request.AddValue( "SrcData", szSrc );
	int nErr = this->CallRemoteMethod( "IssuerTimeStamp", request, response );
	if( nErr != 0 )
		return nErr;
	respData = response.GetValue( "ResponseData" );
	return 0;
}

// = Call remote method.
int CTSASvrClient::CallRemoteMethod( const char * szMethodName, CTSASvrContent & request, 
									CTSASvrContent & response )
{
	request.AddValue( "RemoteFnName", szMethodName );

	CTSASvrStream stream;
	struct sockaddr_in saddr = { 0 };
	saddr.sin_addr.s_addr = inet_addr( this->remote_ip_ );
	saddr.sin_port = htons( this->remote_port_ );
	saddr.sin_family = PF_INET;

#if defined(_WIN32)
	BOOL bDontLinger = FALSE;
	if( ::setsockopt( stream.socket_, SOL_SOCKET, SO_DONTLINGER, (char *)&bDontLinger, 
		sizeof( bDontLinger ) ) < 0 )
		return -1;
#elif defined(_LINUX)
#endif

	struct linger ling;
	ling.l_onoff = 1;
	ling.l_linger = 0;   
#if defined(_WIN32)
	if( ::setsockopt( stream.socket_, SOL_SOCKET, SO_LINGER, (char *)&ling, 
		sizeof( ling ) ) < 0 )
#elif defined(_LINUX)
	if( ::setsockopt( stream.socket_, SOL_SOCKET, SO_LINGER, &ling, sizeof( ling ) ) < 0 )
#endif
		return -1;

	bool bRet = false;
	unsigned long ul = 1;
#if defined(_WIN32)
	ioctlsocket( stream.socket_, FIONBIO, &ul ); // 设置为非阻塞模式
#elif defined(_LINUX)
	ioctl( stream.socket_, FIONBIO, &ul ); // 设置为非阻塞模式
#endif

	if( connect( stream.socket_, (struct sockaddr *)&saddr, sizeof( saddr ) ) == -1 )
	{
		fd_set set;
		FD_ZERO( &set );
		FD_SET( stream.socket_, &set );

		timeval tm;
		tm.tv_sec = this->conn_timeout_;
		tm.tv_usec = 0;

		if( select( stream.socket_ + 1, NULL, &set, NULL, &tm ) > 0 )
		{
#if defined(_WIN32)
			bRet = true;
#elif defined(_LINUX)
			int nErr = -1, nLen = sizeof(nErr);
			getsockopt( stream.socket_, SOL_SOCKET, SO_ERROR, &nErr, (socklen_t *)&nLen );
			bRet = nErr == 0 ? true : false;
#endif
		}
		else
			bRet = false;
	}
	else
		bRet = true;
	if( bRet == false )
		return TSERR_INVALID_CONNECT;

	ul = 0;
#if defined(_WIN32)
 	ioctlsocket( stream.socket_, FIONBIO, &ul ); // 设置为非阻塞模式
#elif defined(_LINUX)
	ioctl( stream.socket_, FIONBIO, &ul ); // 设置为非阻塞模式
#endif

	int nErr = this->SendRequestData( stream, request );
	if( nErr != 0 )
		return nErr;
	return this->RecvResponseData( stream, response );
}

// = Send request data.
int CTSASvrClient::SendRequestData( const CTSASvrStream & stream, CTSASvrContent & request )
{
	char szBuff[512] = { 0 };
	long lContentLen = strlen( request.content_ ? request.content_ : "" );
	sprintf( szBuff, "POST / HTTP/1.1\r\nContent-Length: %ld\r\n\r\n", lContentLen );
	if( stream.Send_n( szBuff, strlen( szBuff ) ) <= 0 )
		return TSERR_SEND_TIMEOUT;
	if( lContentLen > 0 && stream.Send_n( request.content_, lContentLen ) <= 0 )
		return TSERR_SEND_TIMEOUT;
	return 0;
}

// = Recv response data.
int CTSASvrClient::RecvResponseData( const CTSASvrStream & stream, CTSASvrContent & response )
{
	char szBuff[2048] = { 0 };
	long lRecvSize = stream.Recv( szBuff, 2047 );
	if( lRecvSize <= 0 )
		return TSERR_RECV_TIMEOUT;
	szBuff[lRecvSize] = '\0';
	char * pBegin = strstr( szBuff, "Content-Length: " );
	if( pBegin == NULL )
		return TSERR_RECV_TIMEOUT;
	pBegin += strlen( "Content-Length: " );
	char * pEnd = strstr( pBegin, "\r\n\r\n" );
	if( pEnd == NULL )
		return TSERR_RECV_TIMEOUT;
	*pEnd = '\0';
	long lContentLen = atol( pBegin );
	*pEnd = '\r';

	if( lContentLen == 0 )
	{
		if( strstr( szBuff, "200 OK" ) )
			return 0;
		else
			return TSERR_RECV_TIMEOUT;
	}
	if( response.content_ )
		delete [] response.content_;
	response.content_ = new char[lContentLen + 1];
	response.content_[lContentLen] = '\0';
	pBegin = pEnd + strlen( "\r\n\r\n" );
	long lHadRecvLen = lRecvSize - ( pBegin - szBuff );
	if( lHadRecvLen > 0 )
		memcpy( response.content_, pBegin, lHadRecvLen );
	if( lHadRecvLen < lContentLen )
	{
		if( stream.Recv_n( response.content_ + lHadRecvLen, lContentLen - lHadRecvLen ) <= 0 )
			return TSERR_RECV_TIMEOUT;
// 		if( strstr( szBuff, "200 OK" ) )
// 			return 0;
// 		else
// 			return TSERR_RECV_TIMEOUT;
	}
	return 0;
}

int CTSASvrClient::TS_Req( const char * SrcData, std::string & ReqData )
{
	CTSASvrContent request;
	CTSASvrContent response;
	request.AddValue( "SrcData", SrcData );
	int nErr = this->CallRemoteMethod( "TSReq", request, response );
	if( nErr != 0 )
		return nErr;
	ReqData = response.GetValue( "TSReqData" );
	if (ReqData.size() <= 0 )
	{
		ReqData = response.GetValue(ERROR_INFO_CODE);
		if(0 == ReqData.length())
			return TSERR_INVALID_METHOD;
		nErr = atoi( ReqData.c_str() );
		return nErr;
	}
	return 0;
}

int CTSASvrClient::TS_Resp( const char * ReqData, std::string & RespData )
{
	CTSASvrContent request;
	CTSASvrContent response;
	request.AddValue( "ReqData", ReqData );
	int nErr = this->CallRemoteMethod( "TSResp", request, response );
	if( nErr != 0 )
		return nErr;
	RespData = response.GetValue( "TSRespData" );
	if (RespData.size() <=0 )
	{
		RespData = response.GetValue(ERROR_INFO_CODE);
		if(0 == RespData.length())
			return TSERR_INVALID_METHOD;
		nErr = atoi( RespData.c_str() );
		return nErr;
	}
	return 0;
}


int CTSASvrClient::TS_Verify( const char* SrcData, const char* RespData)
{
	CTSASvrContent request;
	CTSASvrContent response;
	request.AddValue( "SrcData", SrcData );
	request.AddValue( "RespData", RespData );
	int nErr = this->CallRemoteMethod( "TSVerify", request, response );
	if( nErr != 0 )
		return nErr;
	std::string VerifyData = response.GetValue( "TSVerifyData" );
	if (VerifyData.size() <=0 )
	{
		VerifyData = response.GetValue(ERROR_INFO_CODE);
		if(0 == VerifyData.length())
			return TSERR_INVALID_METHOD;
		nErr = atoi( VerifyData.c_str() );
		return nErr;
	}	
	if ( 0 != atoi( VerifyData.c_str() ) )
	{
		return TS_VerifyErr;
	}
	return 0;
}

int CTSASvrClient::TS_Parse( const char* RespData, std::string& ParseData, int flag)
{
	CTSASvrContent request;
	CTSASvrContent response;
	request.AddValue( "RespData", RespData );
	if ( 0 == flag)
	{
		request.AddValue("Flag","0");
	}
	else
	{
		request.AddValue("Flag","1");
	}
	int nErr = this->CallRemoteMethod( "TSParse", request, response );
	if( nErr != 0 )
		return nErr;
	ParseData = response.GetValue( "TSParseData" );
	if (ParseData.size() <= 0)
	{
		ParseData = response.GetValue(ERROR_INFO_CODE);
		if(0 == ParseData.length())
			return TSERR_INVALID_METHOD;		
		nErr = atoi( ParseData.c_str() );
		return nErr;
	}
	return 0;
	
}



// =================================================================================
//  1.00    2006-11-28    Created By LinZaoGang
//  File End 
