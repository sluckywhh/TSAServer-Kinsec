#if !defined(_ICCARD_HPP_INCLUDED)
#define _ICCARD_HPP_INCLUDED

/**
  *@Name: ICCard.hpp
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/25 14:29
  *@Description: all ICCard modules interface file 
*/

#undef STATIC_LIB

//if this project is a not static lib, comment out the next line.
#define STATIC_LIB

#ifdef KT_WIN32
    #if _MSC_VER >= 1000
    #pragma once
    #endif // _MSC_VER >= 1000

    #if defined(STATIC_LIB)
      #if defined (_DEBUG)
        #pragma comment(lib,"PCSC32d.lib")
		#pragma message("Automatically linking with PCSC32d.lib")
		
		#pragma comment(lib,"ICCBased.lib")
        #pragma message("Automatically linking with KLICCBased.lib")

		#pragma comment(lib,"GPKCOSd.lib")
		#pragma message("Automatically linking with KLGPKCOSd.lib")

        #pragma comment(lib,"TIMECOSd.lib")
		#pragma message("Automatically linking with KTTIMECOSd.lib")

		#pragma comment(lib,"SmartCOSd.lib")
		#pragma message("Automatically linking with KLSmartCOSd.lib")
      #else
        #pragma comment(lib,"PCSC32.lib")
		#pragma message("Automatically linking with PCSC32.lib")
		
		#pragma comment(lib,"ICCBase.lib")
		#pragma message("Automatically linking with KLICCBase.lib")

		#pragma comment(lib,"GPKCOS.lib")
		#pragma message("Automatically linking with KLGPKCOS.lib")

        #pragma comment(lib,"TIMECOS.lib")
		#pragma message("Automatically linking with KTTIMECOS.lib") 

		#pragma comment(lib,"SmartCOS.lib")
		#pragma message("Automatically linking with KLSmartCOS.lib")       
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

#include "./Interface/ICommonICCHelper.h"
#include "./Interface/ICommonICCHelperPK.h"
#include "./Interface/ICommonPCSCHelper.h"
#include "./ICCard/PCSCHelper.h"
#include "./ICCard/SKPCSCHelper.h"
#include "./ICCard/GPKCOS.h"
#include "./ICCard/TIMECOS.h"
#include "./ICCard/SmartCOS.h"
#include "./ICCard/ICCDef.h"


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
///   1.0.0.0  2003/04/25 14:30  Hongwei Yu   created 

#endif // _ICCARD_HPP_INCLUDED
