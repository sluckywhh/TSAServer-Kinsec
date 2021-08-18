#ifndef __LDAP_HPP__
#define __LDAP_HPP__

/**
  *@Name: KTLdapAPI.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date: 2003/03/10 16:55
  *@Description: API  function  for openLdap
  *@Usage: KTLdap_Initialize¡¢KTLdap_DownFirst¡¢KTLdap_DownNextObj¡¢KTLdap_Finalize
  *@Notes: if you initialize sucess, you must finalize
*/

#undef STATIC_LIB

//if this project is a dll, comment out the next line.
#define STATIC_LIB
#ifdef _WIN32
	#if _MSC_VER >= 1000
	#pragma once
	#endif // _MSC_VER >= 1000

	#if !defined(_MT)
		//#error Please use the /MD or /MT switch (multithreaded (DLL) C-runtime)
	#endif
    #pragma comment(lib,"ws2_32.lib")
	#pragma message("Automatically linking with ws2_32.lib")
	#pragma comment(lib,"Advapi32.lib")
	#pragma message("Automatically linking with Advapi32.lib")	

	#if defined(_DLL)
		#if defined(_DEBUG)
			#pragma comment(lib,"KTLdapMDd.lib")
			#pragma message("Automatically linking with KTLdapMDd.lib")
		#else
			#pragma comment(lib,"KTLdapMD.lib")
			#pragma message("Automatically linking with KTLdapMD.lib")
		#endif
		#pragma comment(lib,"olber32md.lib")
		#pragma message("Automatically linking with olber32md.lib")
		#pragma comment(lib,"oldap32md.lib")
		#pragma message("Automatically linking with oldap32md.lib")	
		//#pragma comment(lib,"libsaslmd.lib")
		//#pragma message("Automatically linking with libsaslmd.lib")
	#else
		#if defined(_DEBUG)
			#pragma comment(lib,"KTLdapMTd.lib")
			#pragma message("Automatically linking with KTLdapMTd.lib")
		#else
			#pragma comment(lib,"KTLdapMT.lib")
			#pragma message("Automatically linking with KTLdapMT.lib")
		#endif
		#pragma comment(lib,"olber32.lib")
		#pragma message("Automatically linking with olber32.lib")
		#pragma comment(lib,"oldap32.lib")
		#pragma message("Automatically linking with oldap32.lib")
		//#pragma comment(lib,"libsasl.lib")
		//#pragma message("Automatically linking with libsasl.lib")
	#endif

#endif

#undef	KT_LIB_MODE
#define KT_LIB_MODE
#ifndef STATIC_LIB //dll
    #if defined(KT_WIN32)
        #undef KT_LIB_MODE
        #define KT_LIB_MODE __declspec(dllimport)
   #endif
#endif

#ifdef __cplusplus
	#include "./Ldap/KTLdap.h"
#endif
	#include "./Ldap/KTLdapAPI.h"

#undef STATIC_LIB

#undef KT_LIB_MODE

#if defined(KT_LIB_STATIC)
    #define KT_LIB_MODE
#elif defined(KT_LIB_SHARED)
    #ifdef KT_WIN32
        #define KT_LIB_MODE __declspec(dllexport)
	#else
        //i do not know how to make a unix shared lib to export class, is this enough?
        #define KT_LIB_MODE
    #endif
#else
    #define KT_LIB_MODE
#endif

#endif //__LDAP_HPP__

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/03/10 17:00  Hongwei Yu   Created 
///   1.0.0.1  2003/03/30 12:32  Hongwei Yu   added KTLDAP_ERR_XX & KTLDAP_AUTH
///   1.0.0.2  2003/04/29 17:02  Hongwei Yu   added auto link lib pragma
///   1.0.0.3  2003/06/18 11:20  Hongwei Yu   move to PKI_SDK
///   1.0.0.4  2003/08/29 19:09  Hongwei Yu   support binary value download 
///   1.0.0.5  2003/10/12 14:51  Hongwei Yu   use openldap-2.1.22, move to linux
///   1.0.0.6  2003/10/16 14:32  Hongwei Yu   remove _DEBUG switch
///   1.0.0.7  2003/10/23 11:55  Hongwei Yu   restore _DEBUG switch     