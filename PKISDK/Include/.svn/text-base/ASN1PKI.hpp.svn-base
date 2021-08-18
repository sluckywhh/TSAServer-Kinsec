#if !defined(_PKISPEC_HPP_INCLUDED)
#define _PKISPEC_HPP_INCLUDED

/**
  *@Name: ASN1PKI.hpp
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/25 14:45
  *@Description: ASN1PKI module interface file 
*/

#ifndef __ONLY_ASN1PKI_HPP__
	#include "./Cipher.hpp"
#endif

#undef STATIC_LIB

//if this project is a not static lib, comment out the next line.
#define STATIC_LIB

#ifdef KT_WIN32
	#if _MSC_VER >= 1000
	#pragma once
	#endif // _MSC_VER >= 1000

	#if !defined(_MT)
		#error Please use the /MD or /MT switch (multithreaded (DLL) C-runtime)
	#endif
	#if defined(_DLL)
	  #if defined (_DEBUG)
		 #pragma comment(lib,"ASN1PKIMDd.lib")
		 #pragma message("Automatically linking with ASN1PKIMDd.lib")
	  #else
         #pragma comment(lib,"ASN1PKIMD.lib")
		 #pragma message("Automatically linking with ASN1PKIMD.lib")   
	  #endif
	#elif defined(_DEBUG)
		 #pragma comment(lib,"ASN1PKIMTd.lib")
		 #pragma message("Automatically linking with ASN1PKIMTd.lib") 
	#else
         #pragma comment(lib,"ASN1PKIMT.lib")
		 #pragma message("Automatically linking with ASN1PKIMT.lib")
	#endif
#endif //KT_WIN32

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

	#include "./ASN1PKI/ObjID.h"
	#include "./ASN1PKI/pkcs1.h"
	#include "./ASN1PKI/pkcs5.h"
	#include "./ASN1PKI/pkcs7.h"
	#include "./ASN1PKI/pkcs10.h"
	#include "./ASN1PKI/pkcs12.h"
	#include "./ASN1PKI/X.509.h"
	#include "./ASN1PKI/cmp.h"
	#include "./ASN1PKI/CRMF.h"	
	#include "./ASN1PKI/OCSP.h"
	#include "./ASN1PKI/NetScape.h"
	#include "./ASN1PKI/KTCRLProp.h"
	#include "./ASN1PKI/KTCertProp.h" 	

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
///   1.0.0.0  2003/04/25 14:45  Hongwei Yu   created
///   1.0.0.1  2003/07/01 14:25  Hongwei Yu   switch /MT and /MD case  

#endif
