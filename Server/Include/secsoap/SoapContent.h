// =================================================================================
//  File       : SoapContent.h
//  Purpose    : Soap Content Param List Class Definition.
//  Programmer : ¡÷‘Ó∏÷
//  Created    : 2006-10-18
//  CopyRight (C) Kinsec
// =================================================================================

#ifndef __SOAPCONTENT_H__
#define __SOAPCONTENT_H__

// =================================================================================
//  Include Header Files
#include "SoapSock.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class SS_EXPORT CSoapContent
{
public :
	// = Default constructor.
	CSoapContent();

	// = Default destructor.
	~CSoapContent();

	// = Get value.
	const char * GetValue( const char * szName );

	// = Add value.
	void AddValue( const char * szName, const char * szValue );

	// = Content value.
	char * content_;

private : 
	// = Parameter list.
	struct S_Parameter
	{
		char * name;
		char * value;
		S_Parameter * next;

		S_Parameter()
		{
			next = NULL;
			name = NULL;
			value = NULL;
		}
	} * header_;

	// = Null value.
	char null_[1];
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif  // __SOAPCONTENT_H__
