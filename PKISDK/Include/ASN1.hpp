#if !defined(__ASN1_HPP_INCLUDED__)
#define __ASN1_HPP_INCLUDED__

/**
  *@Name: ASN1.hpp
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/25 14:46
  *@Description: ASN1 module interface file 
*/

#ifndef __ONLY_ASN1_HPP__
	#include "./Math.hpp"
#endif

#undef STATIC_LIB

//if this project is a dll, comment out the next line.
#define STATIC_LIB
#ifdef KT_WIN32
	#if _MSC_VER >= 1000
	#pragma once
	#endif // _MSC_VER >= 1000

	#if !defined(_MT)
		//#error Please use the /MD or /MT switch (multithreaded (DLL) C-runtime)
	#endif
	#if defined (_DEBUG)
		#if defined(_DLL)
			#pragma comment(lib,"KTASN1MDd.lib")
			#pragma message("Automatically linking with KTASN1MDd.lib")
		#else
			#pragma comment(lib,"KTASN1MTd.lib")
			#pragma message("Automatically linking with KTASN1MTd.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib,"KTASN1MD.lib")
			#pragma message("Automatically linking with KTASN1MD.lib")
		#else
			#pragma comment(lib,"KTASN1MT.lib")
			#pragma message("Automatically linking with KTASN1MT.lib")
		#endif
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

	#include "./ASN1/ASN1.h"
	#include "./ASN1/ASN1Helper.h"

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
///   1.0.0.0  2003/04/25 14:46  Hongwei Yu   created 
///   1.0.0.1  2003/07/01 12:50  Hongwei Yu   switch /MT and /MD case 
///   1.0.0.2  2003/10/16 14:30  Hongwei Yu   remove _DEBUG switch 
///   1.0.0.3  2003/10/23 11:55  Hongwei Yu   restore _DEBUG switch

#endif // _ASN1_HPP_INCLUDED
