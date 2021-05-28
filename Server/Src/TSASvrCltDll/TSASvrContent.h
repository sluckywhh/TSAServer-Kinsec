// =================================================================================
//  File       : TSASvrContent.h
//  Programmer : ¡÷‘Ó∏÷
//  Created    : 2006-11-28
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

#ifndef __TSASVRCONTENT_H__
#define __TSASVRCONTENT_H__

// =================================================================================
//  Include Header Files

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class CTSASvrContent
{
public :
	// = Default constructor.
	CTSASvrContent();

	// = Default destructor.
	virtual ~CTSASvrContent();

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
//  1.00    2006-11-28    Created By LinZaoGang
//  File End 
#endif  // __TSASVRCONTENT_H__
