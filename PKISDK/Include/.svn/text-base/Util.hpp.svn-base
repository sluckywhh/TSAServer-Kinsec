#if !defined(_KTUTIL_HPP_INCLUDED)
#define _KTUTIL_HPP_INCLUDED

/**
  * @Name: Util.hpp
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 14:03
  * @Description: Util module interface file 
*/

#include "./KTPredef.hpp"

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
			#pragma comment(lib,"KTUtilMDd.lib")
			#pragma message("Automatically linking with KTUtilMDd.lib")
		#else
			#pragma comment(lib,"KTUtilMTd.lib")
			#pragma message("Automatically linking with KTUtilMTd.lib")
		#endif
		
	#else
		#if defined(_DLL)
			#pragma comment(lib,"KTUtilMD.lib")
			#pragma message("Automatically linking with KTUtilMD.lib")
		#else
			#pragma comment(lib,"KTUtilMT.lib")
			#pragma message("Automatically linking with KTUtilMT.lib")
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

	#include "./Util/KTTIME.h"
	#include "./Util/KTUtil.h"
	#include "./Util/RandomPool.h"	
	#include "./Util/SoftRandomPool.h"
	#include "./Util/ParamParser.h"
	#include "./Util/KTMutex.h"	
	#include "./Util/AutoLock.h"	
	#include "./Interface/IPlugin.h"
	#include "./Util/PluginMgr.h"
	#include "./Util/OpenCRL.h"

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
///   1.0.0.0  2003/04/25 14:04  Hongwei Yu   created 
///   1.0.0.1  2003/07/01 14:26  Hongwei Yu   switch /MT and /MD case  
///   1.0.0.2  2003/10/16 14:22  Hongwei Yu   remove _DEBUG switch
///   1.0.0.3  2003/10/23 14:09  Hongwei Yu   restore _DEBUG switch
///	  1.0.0.4  2005/12/16 09:54  chenlc		  add openssl support for win32 environment
///   1.0.0.5  2007/01/16 09:20  chenlc       remove openssl support because of conflict

#endif // _KTUTIL_HPP_INCLUDED
