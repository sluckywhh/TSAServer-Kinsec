// =================================================================================
//  File    : TSASoapServer.h
//  Purpose : Soap Server Class Definition.
//  Author  : ¡÷‘Ó∏÷
//  Created : 2006-11-18
//  CopyRight (C) ¡÷‘Ó∏÷.
// =================================================================================

#ifndef __TSASOAPSERVER_H__
#define __TSASOAPSERVER_H__

// =================================================================================
//  Include Header Files
#include <stdio.h>
#include <string.h>
#include "secsoap/SoapServer.h"

// =================================================================================
//  Macro Definition        

// =================================================================================
//  Class Definition
class CTSASoapServer : public CSoapServer
{
protected :
	// = Find function and execute.
	virtual int FindFnDoHandle( const char * szMethod );
};

// =================================================================================
//  1.00    2006-11-18    Created By LinZaoGang
//  File End 
#endif  // __TSASOAPSERVER_H__
