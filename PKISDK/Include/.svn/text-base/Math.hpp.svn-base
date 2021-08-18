#if !defined(_KTMATH_HPP_INCLUDED)
#define _KTMATH_HPP_INCLUDED

/**
  * @Name: Math.cpp
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 14:23
  * @Description: Math module interface file
*/

#include "./Util.hpp"

#undef STATIC_LIB

//if this project is a not static lib, comment out the next line.
#define STATIC_LIB
#ifdef KT_WIN32
	#if _MSC_VER >= 1000
	#pragma once
	#endif // _MSC_VER >= 1000

	#if !defined(_MT)
		//#error Please use the /MD or /MT switch (multithreaded (DLL) C-runtime)
	#endif
	#if defined(_DEBUG)
		#if defined(_DLL)
			#pragma comment(lib,"KTMathMDd.lib")
			#pragma message("Automatically linking with KTMathMDd.lib")
		#else
			#pragma comment(lib,"KTMathMTd.lib")
			#pragma message("Automatically linking with KTMathMTd.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib,"KTMathMD.lib")
			#pragma message("Automatically linking with KTMathMD.lib")
		#else
			#pragma comment(lib,"KTMathMT.lib")
			#pragma message("Automatically linking with KTMathMT.lib")
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

	#include "./Math/BigInteger.h"	
	#include "./Math/AsmuthBloom.h"
	#include "./Math/ABControl.h"

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
///   1.0.0.0  2003/04/25 14:23  Hongwei Yu   created
///   1.0.0.1  2003/10/16 14:30  Hongwei Yu   remove _DEBUG switch
///   1.0.0.2  2003/10/23 14:09  Hongwei Yu   restore _DEBUG switch 

#endif // _KTMATH_HPP_INCLUDED
