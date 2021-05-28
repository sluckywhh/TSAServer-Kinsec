// =================================================================================
//  File       : SoapSock.h
//  Purpose    : Socket Base Class.
//  Programmer : ¡÷‘Ó∏÷
//  Created    : 2006-10-18
//  CopyRight (C) ¡÷‘Ó∏÷
// =================================================================================

#ifndef __SOAPSOCK_H__
#define __SOAPSOCK_H__

// =================================================================================
//  Include Header Files

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  ifdef SECSOAP_EXPORTS
#    define SS_EXPORT __declspec( dllexport )
#  else
#    define SS_EXPORT __declspec( dllimport )
#    ifdef _DEBUG
#      pragma comment( lib, "SecSoapD.lib" )
#      pragma message( "Automatically linking with SecSoapD.lib" )
#    else
#      pragma comment( lib, "SecSoap.lib" )
#      pragma message( "Automatically linking with SecSoap.lib" )
#    endif
#  endif
#elif defined(_LINUX)
#  define SS_EXPORT
#endif
#define ERROR_INFO_NAME "ErrorInfo" 

// =================================================================================
//  Class Definition
class SS_EXPORT CSoapSock  
{
public:
	// = Default constructor.
	CSoapSock();

	// = Default destructor.
	virtual ~CSoapSock();

	// = Cloee a socket.
	void Close();

	// = Socket handle.
#if defined( _WIN32 )
	unsigned int socket_;
#elif defined( _LINUX )
	int socket_;
#endif

	// = Socket reference.
	static long reference_;
};

// =================================================================================
//  1.00    2006-10-18    Created By LinZaoGang
//  File End 
#endif // __SOAPSOCK_H__
