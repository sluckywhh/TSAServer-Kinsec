#ifndef _KT_MACRO_H_
#define _KT_MACRO_H_
/**
  *@Name: KTMacro.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/03 15:43
  *@Description: all macro define 
*/
#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include <assert.h>
#include "KTType.h"
#include "KTErrNum.h"

#if !defined(KT_LIB_MODE)
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
#endif


#define safe_delete(p)  delete (p); (p) = NULL;

#ifdef	__cplusplus

	inline KTBOOL IsError(KTINT32 ret)
	{
		return (KT_OK != ret);
	};

	inline KTBOOL IsOK(KTINT32 ret)
	{
		return (KT_OK == ret);
	};

	inline void ThrowIf(KTINT32 ret)
	{
		if (IsError(ret))
			throw static_cast<enumKTError>(ret);
	};

#endif
    
#define KT_BEGIN_NAMESPACE(x)  namespace x{
#define KT_END_NAMESPACE(x)	   };
	
#define THIS_FILE           __FILE__

#ifndef KT_MAX_PATH
    #define KT_MAX_PATH		256
#endif

#define CHECK_NEW(x)			\
	if(x == NULL){			\
	    return KT_ERR_NOENOUGH_MEM;	\
	}

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/03 15:38  Hongwei Yu   created 

#endif
