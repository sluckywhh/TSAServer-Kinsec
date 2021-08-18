#ifndef _UI_CONFIG_EX_H_
#define _UI_CONFIG_EX_H_


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UICFGEX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UICFGEX_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef UICFGEX_EXPORTS
#define UICFGEX_API __declspec(dllexport)
#else
#define UICFGEX_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
//
#ifndef UICFGEX_EXPORTS
# ifdef _DEBUG
#  pragma comment( lib, "UICfgExd.lib" )
#  pragma message( "Automatic linking to UICfgExd.lib" )
# else
#  pragma comment( lib, "UICfgEx.lib" )
#  pragma message( "Automatic linking to UICfgEx.lib" )
# endif
#endif

//////////////////////////////////////////////////////////////////////////
// + Application-Context Management
// Note :
//	1. the context depends on thread context.
//	2. each thread has its own context, but shared.

//////////////////////////////////////////////////////////////////////////
////// + To Get/Set Encryption Algorithm for context.
// Note : 
//	1. That will effect for KTCore Envelope Functions
//	2. If the context has not SET algorithm, it will use 
//	    the default algorithm be specified in REGISTRY, 
//	    and check out by UICfg_GetSymmetricAlgorithm.
// i.e.
//  void UsesAlgorithmOfSCB2_Envelop(...){
//		DWORD nAlg = 0;
//		UICfg_Context_GetSymmetricAlgorithm( nAlg );
//		if ( nAlg != SCB2 ){
//			UICfg_Context_SetSymmetricAlgorithm( SCB2 );			
//		}
//		KTCore_Envelop(...); // uses SCB2 algorithm to encrypt.
//  }

UICFGEX_API bool UICfg_Context_SetSymmetricAlgorithm( DWORD nAlgorithm );
UICFGEX_API bool UICfg_Context_GetSymmetricAlgorithm( DWORD& nAlgorithm );

//
//
//
// - Application-Context Management
//////////////////////////////////////////////////////////////////////////


#endif // ! _UI_CONFIG_EX_H_

