// =================================================================================
//  File       : Blowfish.h
//  Purpose    : Blowfish Crypto Class.
//  Author     : ¡÷‘Ó∏÷
//  Created    : 2006-10-18
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

#ifndef __BLOWFISH_H__
#define __BLOWFISH_H__

// =================================================================================
//  Include Header Files
#include "SoapSock.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class SS_EXPORT CBlowfish
{
public :
	struct S_Bf_Key
	{
		unsigned __int32 P[16 + 2];
		unsigned __int32 S[4 * 256];
	};

	// = Default constructor.
	CBlowfish();

	// = Default destructor.
	virtual ~CBlowfish();

	// = Set key for encrypt and decrypt.
	void SetKey( unsigned char * pKey, int nKeySize );

	// = Encrypt data.
	void EncryptData( void * pData, int nLen );

	// = Decrypt data.
	void DecryptData( void * pData, int nLen );

private :
	// = Encrypt block.
	void EncryptBlock( unsigned __int32 * data );

	// = Decrypt block.
	void DecryptBlock( unsigned __int32 * data );

	// = Key for encrypt and derypt.
	S_Bf_Key ks_;
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif // __BLOWFISH_H__
