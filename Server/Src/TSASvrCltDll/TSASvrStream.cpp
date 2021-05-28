// =================================================================================
//  File       : TSASvrStream.cpp
//  Author     : ¡÷‘Ó∏÷
//  Created    : 2006-11-28
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

// =================================================================================
//  Include Header Files
#if defined(_WIN32)
#  include <winsock2.h>
#elif defined(_LINUX)
#  include <unistd.h>
#  include <pthread.h>
#  include <arpa/inet.h>
#  include <sys/socket.h>
#endif
#include "TSASvrStream.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
// = Default constructor.
CTSASvrStream::CTSASvrStream()
{
#if defined(_WIN32)
	static bool bInit = false;
	if( bInit == false )
	{
		WSADATA wsa_data;
		WSAStartup( 0x202, &wsa_data );
	}
	this->socket_ = (unsigned int)socket( PF_INET, SOCK_STREAM, 0 );
#elif defined(_LINUX)
	this->socket_ = (int)socket( PF_INET, SOCK_STREAM, 0 );
#endif
}

// = Default destructor.
CTSASvrStream::~CTSASvrStream()
{
	if( this->socket_ != -1 )
	{
#if defined(_WIN32)
		closesocket( this->socket_ );
#elif defined(_LINUX)
		close( this->socket_ );
#endif
		this->socket_ = -1;
	}
}

// = Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
long CTSASvrStream::Send( const void * pBuff, long lSize ) const
{
	int nRet = this->WaitForSendData();
	if( nRet < 0 )
		return -1;
	else if( nRet == 0 )
		return 0;
	else
		return send( this->socket_, (char *)pBuff, lSize, 0 );
}

// = Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
long CTSASvrStream::Send_n( const void * pBuff, long lSize ) const
{
	long bt = 0;
	while( bt < lSize )
	{
		// Try to transfer as much of the remaining data as possible.
		int nRet = this->WaitForSendData();
		if( nRet < 0 )
			return -1;
		else if( nRet == 0 )
			break; // timeout.
		long bytes_send = send( this->socket_, (const char *)pBuff + bt, lSize - bt, 0 );
		if( bytes_send > 0 )
			bt += bytes_send;
		else
			return -1;
	}

	return bt;
}

// = Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
long CTSASvrStream::Recv( void * pBuff, long lSize ) const
{
	int nRet = this->WaitForRecvData();
	if( nRet < 0 )
		return -1;
	else if( nRet == 0 )
		return 0;
	return recv( this->socket_, (char *)pBuff, lSize, 0 );
}

// = Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
long CTSASvrStream::Recv_n( void * pBuff, long lSize ) const
{
	long bt = 0;
	while( bt < lSize )
	{
		// Try to transfer as much of the remaining data as possible.
		int nRet = this->WaitForRecvData();
		if( nRet < 0 )
			return -1;
		else if( nRet == 0 )
			break;
		long bytes_recv = recv( this->socket_, (char *)pBuff + bt, lSize - bt, 0 );
		if( bytes_recv > 0 )
			bt += bytes_recv;
		else
			return -1;
	}
	return bt;
}

// = Wait for send data.
int CTSASvrStream::WaitForSendData() const
{
	fd_set handle_set;
	FD_ZERO( &handle_set );
	FD_SET( this->socket_, &handle_set );

	struct timeval tv;
	tv.tv_usec = 0;
	tv.tv_sec = 60;
	int nRet = select( this->socket_ + 1, 0, &handle_set, 0, &tv );
	if( nRet > 0 && FD_ISSET( this->socket_, &handle_set ) )
		FD_CLR( this->socket_, &handle_set );
	return nRet;
}

// = Wait for recv data.
int CTSASvrStream::WaitForRecvData() const
{
	fd_set handle_set;
	FD_ZERO( &handle_set );
	FD_SET( this->socket_, &handle_set );

	struct timeval tv;
	tv.tv_usec = 0;
	tv.tv_sec = 60;
	int nRet = select( this->socket_ + 1, &handle_set, 0, 0, &tv );
	if( nRet > 0 && FD_ISSET( this->socket_, &handle_set ) )
		FD_CLR( this->socket_, &handle_set );
	return nRet;
}


// =================================================================================
//  1.00    2006-11-28    Created By LinZaoGang
//  File End 
