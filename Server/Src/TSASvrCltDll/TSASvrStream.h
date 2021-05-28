// =================================================================================
//  File       : TSASvrStream.h
//  Author     : ¡÷‘Ó∏÷
//  Created    : 2006-11-28
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

#ifndef __TSASVRSTREAM_H__
#define __TSASVRSTREAM_H__

// =================================================================================
//  Include Header Files

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class CTSASvrStream
{
public :
	// = Default constructor.
	CTSASvrStream();

	// = Default destructor.
	virtual ~CTSASvrStream();

	// = Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
	long Send( const void * pBuff, long lSize ) const;

	// = Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
	long Send_n( const void * pBuff, long lSize ) const;

	// = Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
	long Recv( void * pBuff, long lSize ) const;

	// = Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
	long Recv_n( void * pBuff, long lSize ) const;

	// = Socket handle.
#if defined( _WIN32 )
	unsigned int socket_;
#elif defined( _LINUX )
	int socket_;
#endif

private :
	// = Wait for send data.
	int WaitForSendData() const;

	// = Wait for recv data.
	int WaitForRecvData() const;
};

// =================================================================================
//  1.00    2006-11-28    Created By LinZaoGang
//  File End 
#endif // __TSASVRSTREAM_H__
