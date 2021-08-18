#if !defined(__FILETOKEN_HPP_INCLUDED__)
#define __FILETOKEN_HPP_INCLUDED__

/**
  *@Name: FileToken.hpp
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/25 14:42
  *@Description: FileToken module interface file 
*/

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
	#if defined (_DEBUG)
		#pragma comment(lib,"KTFlTokend.lib")
		#pragma message("Automatically linking with KTFileTokend.lib")
	#else
		#pragma comment(lib,"KTFlToken.lib")
		#pragma message("Automatically linking with KTFileToken.lib")	
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

	#include "./TokenSvr/FileToken.h"

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
///   1.0.0.0  2003/04/25 14:43  Hongwei Yu   created 
///   1.0.0.1  2003/05/12 23:08  Hongwei Yu   remove KT_LOGGER macro
///   1.0.0.2  2003/07/01 14:25  Hongwei Yu   switch /MT and /MD case  
///   1.0.0.3  2003/10/16 14:32  Hongwei Yu   remove _DEBUG switch
///   1.0.0.4  2003/10/23 11:55  Hongwei Yu   restore _DEBUG switch 

#endif
