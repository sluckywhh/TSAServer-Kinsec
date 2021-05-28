// =================================================================================
//  File       : SoapAcceptor.h
//  Purpose    : Acceptor Class.
//  Programmer : ¡÷‘Ó∏÷
//  Created    : 2006-10-18
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

#ifndef __SOAPACCEPTOR_H__
#define __SOAPACCEPTOR_H__

// =================================================================================
//  Include Header Files
#include "SoapServer.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class SS_EXPORT CSoapAcceptor : public CSoapSock
{
public :
	// = Default constructor.
	CSoapAcceptor();

	// = Default destructor.
	virtual ~CSoapAcceptor();

	// = Open a listen socket.
	int Open( short nLocalPort, int nBacklog, char *pAddressIP = 0 );

	// = Accept a client connection.
	int Accept( CSoapServer * pSoap );
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif // __SOAPACCEPTOR_H__
