#if !defined(_SLOT_HPP_INCLUDED)
#define _SLOT_HPP_INCLUDED

#undef STATIC_LIB
//if this project is a not static lib, comment out the next line.
#define STATIC_LIB

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

#include "./TokenSvr/ISlot.h"	
#include "./TokenSvr/ISlotImple.h"	

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

#endif // _SLOT_HPP_INCLUDED
