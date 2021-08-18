#if !defined(xml_HPP_INCLUDED)
#define xml_HPP_INCLUDED

/**
  * @Name: XML.hpp
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 13:40
  * @Description: XML module interface file 
*/

#undef STATIC_LIB

//if this project is a not static lib, comment out the next line.
#define STATIC_LIB

#ifdef _WIN32
    #if _MSC_VER >= 1000
    #pragma once
    #endif // _MSC_VER >= 1000

	#if !defined(_MT)
		//#error Please use the /MD or /MT switch (multithreaded (DLL) C-runtime)
	#endif
	#if defined(_DEBUG)
		#if defined(_DLL)
			#pragma comment(lib,"KTXMLMDd.lib")
			#pragma message("Automatically linking with KTXMLMDd.lib")
		#else
			#pragma comment(lib,"KTXMLMTd.lib")
			#pragma message("Automatically linking with KTXMLMTd.dll") 
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib,"KTXMLMD.lib")
			#pragma message("Automatically linking with KTXMLMD.lib")
		#else
			#pragma comment(lib,"KTXMLMT.lib")
			#pragma message("Automatically linking with KTXMLMT.dll") 
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

#include "./XML/xmlparse.h"
#include "./XML/XMLExparser.h"

#undef STATIC_LIB

#undef KT_LIB_MODE

#if defined(KT_LIB_STATIC)
    #define KT_LIB_MODE
#elif defined(KT_LIB_SHARED)
    #ifdef KT_WIN32
        #define KT_LIB_MODE AFX_CLASS_EXPORT
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
///   1.0.0.0  2003/04/25 13:41  Hongwei Yu   created 
///   1.0.0.1  2003/05/12 22:59  Hongwei Yu   remove KT_LOGGER macro
///   1.0.0.2  2003/07/01 14:27  Hongwei Yu   switch /MT and /MD case
///   1.0.0.3  2003/10/16 14:36  Hongwei Yu   remove _DEBUG switch
///   1.0.0.4  2003/10/23 14:09  Hongwei Yu   restore _DEBUG switch 

#endif // xml_HPP_INCLUDED