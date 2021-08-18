#ifndef __UILOGIN_HPP_INCLUDED__
#define __UILOGIN_HPP_INCLUDED__

/**
  * @Name: UILogin.hpp
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 14:07
  * @Description: UILogin module interface file 
*/

#include "KTPredef.hpp"

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
		#pragma comment(lib,"UILogind.lib")
		#pragma message("Automatically linking with UILogind.dll")
	#else
		#pragma comment(lib,"UILogin.lib")
		#pragma message("Automatically linking with UILogin.dll")
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

#include "PKIDef.h"

	#include "./UI/UILogin.h"	

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
///   1.0.0.0  2003/04/25 14:06  Hongwei Yu   created 
///   1.0.0.1  2003/07/01 14:30  Hongwei Yu   switch /MT and /MD case
///   1.0.0.2  2003/10/16 15:31  Hongwei Yu   remove _DEBUG switch
///   1.0.0.3  2003/10/23 15:00  Hongwei Yu   restore _DEBUG switch

#endif // __UILOGIN_HPP_INCLUDED__
