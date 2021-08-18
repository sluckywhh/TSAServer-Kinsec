#if !defined(__MYSMLIB_HPP_INCLUDED__)
#define __MYSMLIB_HPP_INCLUDED__

/**
  *@Name: MySMLib.hpp
  *@Copyright: KTXA SoftWare
  *@Author: 
  *@Date:  2013
  *@Description: SM algorithm module interface file 
*/

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
			#pragma comment(lib,"libeay32.lib")
			#pragma comment(lib,"MySMLibD.lib")
			#pragma message("Automatically linking with MySMLibd.lib")
		#else
			#pragma comment(lib,"libeay32.lib")
			#pragma comment(lib,"MySMLibD.lib")
			#pragma message("Automatically linking with MySMLibD.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib,"libeay32.lib")
			#pragma comment(lib,"MySMLib.lib")
			#pragma message("Automatically linking with MySMLib.lib")
		#else
			#pragma comment(lib,"libeay32.lib")
			#pragma comment(lib,"MySMLib.lib")
			#pragma message("Automatically linking with MySMLib.lib")
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

	#include "./MySMLib/MySMLibFunc.h"
	#include "./MySMLib/mysm_sm3.h"
	#include "./MySMLib/sms4.h"

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

#endif // _SM_HPP_INCLUDED
