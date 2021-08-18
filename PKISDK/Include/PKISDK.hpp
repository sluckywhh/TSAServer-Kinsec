#ifndef __PKISDK_HPP_INCLUDED__
#define __PKISDK_HPP_INCLUDED__

/**
  * @Name: PKISdk.hpp
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 14:14
  * @Description: all PKI module interface hpp file 
*/

#ifdef	__cplusplus
    #include "./PKIUtil.hpp"
#endif

#undef STATIC_LIB

//if this project is a not static lib, comment out the next line.
//#define STATIC_LIB
#ifdef KT_WIN32
	#if _MSC_VER >= 1000
	#pragma once
	#endif // _MSC_VER >= 1000

	#if !defined(_MT)
		//#error Please use the /MD or /MT switch (multithreaded (DLL) C-runtime)
	#endif
	#if defined(_DEBUG)
		#ifdef KT_NO_COM_TOKEN_SVR
			#pragma comment(lib,"PKISDKS10d.lib")
			#pragma message("Automatically linking with PKISDKS10d.dll")
		#else
			#pragma comment(lib,"PKISDK10d.lib")
			#pragma message("Automatically linking with PKISDK10d.dll")
		#endif
	#else
		#ifdef KT_NO_COM_TOKEN_SVR
			#pragma comment(lib,"PKISDKS10.lib")
			#pragma message("Automatically linking with PKISDKS10.dll")
		#else
			#pragma comment(lib,"PKISDK10.lib")
			#pragma message("Automatically linking with PKISDK10.dll")
		#endif
	#endif	
#endif

#undef	KT_LIB_MODE
#define KT_LIB_MODE
#ifndef STATIC_LIB //it is a dll project
    #if defined(KT_WIN32)
        //so we must change MFC extension dll definitions to AFX_CLASS_IMPORT to make this
        //dll importable.
        #undef KT_LIB_MODE
        #define KT_LIB_MODE __declspec(dllimport)
    #endif
#endif

#include "./PKIAPI/APIBase.h"
#include "./PKIAPI/APICert.h"
#include "./PKIAPI/PKISDK51.h"

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

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 14:15  Hongwei Yu   created 
///   1.0.0.1  2003/05/10 21:20  Hongwei Yu   move ASN1.hpp etc to front 
///   1.0.0.2  2003/05/12 23:03  Hongwei Yu   remove KT_LOGGER macro
///   1.0.0.3  2003/07/01 14:26  Hongwei Yu   switch /MT and /MD case
///   1.0.0.4  2003/10/16 14:36  Hongwei Yu   remove _DEBUG switch
///   1.0.0.5  2003/10/23 14:09  Hongwei Yu   restore _DEBUG switch 

#endif
