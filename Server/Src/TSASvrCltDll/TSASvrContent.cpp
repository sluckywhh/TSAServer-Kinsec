// =================================================================================
//  File       : TSASvrContent.cpp
//  Programmer : ¡÷‘Ó∏÷
//  Created    : 2006-11-28
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

// =================================================================================
//  Include Header Files
#include <stdio.h>
#include <string.h>
#include "TSASvrContent.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
// = Default constructor.
CTSASvrContent::CTSASvrContent()
{
	this->header_ = NULL;
	this->content_ = NULL;
	memset( this->null_, 0, sizeof( this->null_ ) );
}

// = Default destructor.
CTSASvrContent::~CTSASvrContent()
{
	if( this->content_ )
		delete [] this->content_;
	while( this->header_ )
	{
		S_Parameter * pTemp = this->header_;
		this->header_ = this->header_->next;
		delete [] pTemp->name;
		delete [] pTemp->value;
		delete pTemp;
	}
}

// = Get value.
const char * CTSASvrContent::GetValue( const char * szName )
{
	if( this->content_ == NULL || szName == NULL )
		return this->null_;
	else if( this->header_ )
	{
		// Find had exist.
		S_Parameter * pTemp = this->header_;
		while( 1 )
		{
			if( strcmp( pTemp->name, szName ) == 0 && pTemp->value )
				return pTemp->value;
			if( pTemp->next == NULL )
				break;
			pTemp = pTemp->next;
		}
		
		// Add into list.
		char * pBegin = strstr( this->content_, szName );
		while( pBegin )
		{
			if( pBegin != this->content_ && *( pBegin - 1 ) != ';' )
			{
				pBegin = strstr( pBegin + 1, szName );
				continue;
			}
			if( *( pBegin + strlen( szName ) ) != '|' )
			{
				pBegin = strstr( pBegin + 1, szName );
				continue;
			}

			pBegin += strlen( szName ) + 1; // 1 = strlen( "|" );
			char * pEnd = strstr( pBegin, ";" );
			if( pEnd )
			{
				// Save name.
				S_Parameter * newValue = new S_Parameter;
				newValue->name = new char[strlen( szName ) + 1];
				strcpy( newValue->name, szName );

				// Save value.
				int iSize = pEnd - pBegin;
				newValue->value = new char[iSize + 1];
				memcpy( newValue->value, pBegin, iSize );
				newValue->value[iSize] = '\0';
				pTemp->next = newValue;
				return newValue->value;
			}
		}
	}
	else
	{
		char * pBegin = strstr( this->content_, szName );
		while( pBegin )
		{
			if( pBegin != this->content_ && *( pBegin - 1 ) != ';' )
			{
				pBegin = strstr( pBegin + 1, szName );
				continue;
			}
			if( *( pBegin + strlen( szName ) ) != '|' )
			{
				pBegin = strstr( pBegin + 1, szName );
				continue;
			}

			pBegin += strlen( szName ) + 1; // 1 = strlen( "|" );
			char * pEnd = strstr( pBegin, ";" );
			if( pEnd )
			{
				// Save name.
				this->header_ = new S_Parameter;
				this->header_->name = new char[strlen( szName ) + 1];
				strcpy( this->header_->name, szName );

				// Save value.
				int iSize = pEnd - pBegin;
				this->header_->value = new char[iSize + 1];
				memcpy( this->header_->value, pBegin, iSize );
				this->header_->value[iSize] = '\0';
				return this->header_->value;
			}
		}
	}
	return this->null_;
}

// = Add value.
void CTSASvrContent::AddValue( const char * szName, const char * szValue )
{
	if( szName && szValue )
	{
		if( this->content_ )
		{
			char * pTemp = this->content_;
			this->content_ = new char[strlen( pTemp ) + strlen( szName ) + strlen( szValue ) + 3];
			sprintf( this->content_, "%s%s|%s;", pTemp, szName, szValue );
			delete [] pTemp;
		}
		else
		{
			this->content_ = new char[strlen( szName ) + strlen( szValue ) + 3];
			sprintf( this->content_, "%s|%s;", szName, szValue );
		}
	}
}


// =================================================================================
//  1.00    2006-11-28    Created By LinZaoGang
//  File End 
