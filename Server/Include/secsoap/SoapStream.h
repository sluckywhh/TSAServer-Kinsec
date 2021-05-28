// =================================================================================
//  File       : SoapStream.h
//  Purpose    : Socket Stream Class.
//  Author     : ¡÷‘Ó∏÷
//  Created    : 2006-10-18
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

#ifndef __SOAPSTREAM_H__
#define __SOAPSTREAM_H__

// =================================================================================
//  Include Header Files
#include "SoapSock.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class SS_EXPORT CSoapStream : public CSoapSock
{
public :
	// = Default constructor.
	CSoapStream();

	// = Default destructor.
	virtual ~CSoapStream();

	// = Set timeout seconds.
	void SetTimeout( long lSendTimeoutSecs = 15, long lRecvTimeoutSecs = 15 );

	// = Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
	long Send( const void * pBuff, long lSize ) const;

	// = Try to send exactly <lSize> bytes from <pBuff> to the connection socket.
	long Send_n( const void * pBuff, long lSize ) const;

	// = Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
	long Recv( void * pBuff, long lSize ) const;

	// = Try to recv exactly <lSize> bytes into <pBuff> from the connected socket.
	long Recv_n( void * pBuff, long lSize ) const;

private :
	// = Wait for send data.
	int WaitForSendData() const;

	// = Wait for recv data.
	int WaitForRecvData() const;

	// = Send timeout seconds.
	long send_timeout_;

	// = Recv timeout seconds.
	long recv_timeout_;
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif // __SOAPSTREAM_H__
