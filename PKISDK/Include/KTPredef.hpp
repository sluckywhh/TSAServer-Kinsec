#if !defined _KT_PREDEF_HPP_
#define _KT_PREDEF_HPP_

/**
  *@Name: KTPredef.hpp
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/17 10:51
  *@Description: all pre define macro¡¢class and err number 
*/

#ifdef WIN32
    #ifndef KT_UNIX
		#ifndef KT_WIN32
			#define KT_WIN32
		#endif
    #endif
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./KTType.h"
#include "./KTSTL.h"
#include "./KTMacro.h"
#include "./KTErrNum.h"
#include "./KTLog.h"
#include "./KTException.h"
#include "./KTUnknow.h"
#include "./KTInterface.h"

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/17 10:56  Hongwei Yu   created 

#endif
