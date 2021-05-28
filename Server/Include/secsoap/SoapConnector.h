// =================================================================================
//  File       : SoapConnector.h
//  Purpose    : Connector Class.
//  Author     : ¡÷‘Ó∏÷
//  Created    : 2006-10-18
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

#ifndef __SOAPCONNECTOR_H__
#define __SOAPCONNECTOR_H__

// =================================================================================
//  Include Header Files
#include "Blowfish.h"
#include "SoapStream.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class SS_EXPORT CSoapConnector
{
public:
	// = Default constructor.
	CSoapConnector();

	// = Default destructor.
	virtual ~CSoapConnector();

#if defined(_WIN32)
	// = Actively connect to a peer, producing a connected stream and key.
	int Connect( const char * szIP, short nPort, CSoapStream & stream, CBlowfish & bf, 
		long lSendTimeoutSecs = 6 );
#elif defined(_WIN32)
#endif

	// = Actively connect to a peer, producing a connected stream and key.
	int Connect( const char * szIP, short nPort, CSoapStream & stream, CBlowfish & bf, 
		const void * pCert, const void * pPriKey, long lSendTimeoutSecs = 6 );
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif // __SOAPCONNECTOR_H__
