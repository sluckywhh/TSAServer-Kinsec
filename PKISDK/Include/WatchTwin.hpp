#if !defined(_WATCHTWIN_HPP_INCLUDED)
#define _WATCHTWIN_HPP_INCLUDED

/**
  * @Name: WatchTwin.hpp
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/06/21 08:38
  * @Description: WatchTwin module interface file 
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
	#if defined(_TWIN_CERT)&&defined(_WESTONE)
		#if defined(_DEBUG)
			#pragma comment(lib,"WestoneTwnd.lib")
			#pragma message("Automatically linking with WestoneTwnd.lib")
		#else
			#pragma comment(lib,"WestoneTwn.lib")
			#pragma message("Automatically linking with WestoneTwn.lib")
		#endif
	#elif defined(_TWIN_CERT)&&defined(_ZC)
		#if defined(_DEBUG)
			#pragma comment(lib,"ZCTwnd.lib")
			#pragma message("Automatically linking with ZCTwnd.lib")
		#else
			#pragma comment(lib,"ZCTwn.lib")
			#pragma message("Automatically linking with ZCTwn.lib")
		#endif
	#elif defined(_TWIN_CERT)&&defined(_HB)
		#if defined(_DEBUG)
			#pragma comment(lib,"HBTwnd.lib")
			#pragma message("Automatically linking with HBTwnd.lib")
		#else
			#pragma comment(lib,"HBTwn.lib")
			#pragma message("Automatically linking with HBTwn.lib")
		#endif
	#elif defined(_TWIN_CERT)&&defined(_DEAN)
		#if defined(_DEBUG)
			#pragma comment(lib,"DeanTwnd.lib")
			#pragma message("Automatically linking with DeanTwnd.lib")	
		#else
			#pragma comment(lib,"DeanTwn.lib")
			#pragma message("Automatically linking with DeanTwn.lib")	
		#endif
	#elif defined(_TWIN_CERT)&&defined(_MW_PCSC)
		#if defined(_DEBUG)
			#pragma comment(lib,"MW_pcscTwnd.lib")
			#pragma message("Automatically linking with MW_pcscTwnd.lib")	
		#else
			#pragma comment(lib,"MW_pcscTwn.lib")
			#pragma message("Automatically linking with MW_pcscTwn.lib")	
		#endif	
	#elif defined(_TWIN_CERT)&&defined(_MW)
		#if defined(_DEBUG)
			#pragma comment(lib,"MWTwnd.lib")
			#pragma message("Automatically linking with MWTwnd.lib")
		#else
			#pragma comment(lib,"MWTwn.lib")
			#pragma message("Automatically linking with MWTwn.lib")
		#endif
	#elif defined(_TWIN_CERT)
		#if defined(_DEBUG)
			#pragma comment(lib,"WatchTwnd.lib")
			#pragma message("Automatically linking with WatchTwnd.lib")
		#else
			#pragma comment(lib,"WatchTwn.lib")
			#pragma message("Automatically linking with WatchTwn.lib")
		#endif		
	#elif defined(_WESTONE)
		#if defined(_DEBUG)
			#pragma comment(lib,"WestoneKeyd.lib")
			#pragma message("Automatically linking with WestoneKeyd.lib")
		#else
			#pragma comment(lib,"WestoneKey.lib")
			#pragma message("Automatically linking with WestoneKey.lib")
		#endif
	#elif defined(_DEAN)
		#if defined(_DEBUG)
			#pragma comment(lib,"DeanKeyd.lib")
			#pragma message("Automatically linking with DeanKeyd.lib")
		#else
			#pragma comment(lib,"DeanKey.lib")
			#pragma message("Automatically linking with DeanKey.lib")
		#endif
	#elif defined(_MW_PCSC)
		#if defined(_DEBUG)
			#pragma comment(lib,"MW_pcscKeyd.lib")
			#pragma message("Automatically linking with MW_pcscKeyd.lib")
		#else
			#pragma comment(lib,"MW_pcscKey.lib")
			#pragma message("Automatically linking with MW_pcscKey.lib")
		#endif
	#else
		#if defined(_DEBUG)
			#pragma comment(lib,"WatchKeyd.lib")
			#pragma message("Automatically linking with WatchKeyd.lib")
		#else
			#pragma comment(lib,"WatchKey.lib")
			#pragma message("Automatically linking with WatchKey.lib")
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

	#include "./Interface/ICommonKeyMangr.h"	
	#include "./Interface/ICommonICCHelper.h"
	#include "./Interface/ICommonICCHelperPK.h"
	#include "./Key/WatchTwin.h"
	
#undef STATIC_LIB

#undef KT_LIB_MODE

#if defined(KL_LIB_STATIC)
    #define KT_LIB_MODE
#elif defined(KL_LIB_SHARED)
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
///   1.0.0.1  2003/05/12 23:08  Hongwei Yu   remove KT_LOGGER macro
///   1.0.0.2  2003/07/01 14:29  Hongwei Yu   switch /MT and /MD case
///   1.0.0.3  2003/10/23 14:09  Hongwei Yu   restore _DEBUG switch

#endif // _WATCHTWIN_HPP_INCLUDED
