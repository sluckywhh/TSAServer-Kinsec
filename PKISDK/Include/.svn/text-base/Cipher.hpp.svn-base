#if !defined(__CIPHER_HPP_INCLUDED__)
#define __CIPHER_HPP_INCLUDED__

/**
  *@Name: Cipher.hpp
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/25 14:44
  *@Description: Cipher module interface file 
*/

#include "./ASN1.hpp"
#include "./MySMLib.hpp"

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
			#pragma comment(lib,"KTCipherMDd.lib")
			#pragma message("Automatically linking with KTCipherMDd.lib")
		#else
			#pragma comment(lib,"KTCipherMTd.lib")
			#pragma message("Automatically linking with KTCipherMTd.lib")
		#endif
	#else
		#if defined(_DLL)
			#pragma comment(lib,"KTCipherMD.lib")
			#pragma message("Automatically linking with KTCipherMD.lib")
		#else
			#pragma comment(lib,"KTCipherMT.lib")
			#pragma message("Automatically linking with KTCipherMT.lib")
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

	#include "./Cipher/Engine.h"	
	#include "./Cipher/Engine/CipherEngineImpl.h"	
	#include "./Cipher/Engine/MDEngineImpl.h"
	#include "./Cipher/Engine/DESEngine.h"
	#include "./Cipher/Engine/3DESEngine.h"
	#include "./Cipher/Engine/RC2Engine.h"
	#include "./Cipher/Engine/RC4Engine.h"
	#include "./Cipher/Engine/RSAEngine.h"
	#include "./Cipher/Engine/MD2Engine.h"
	#include "./Cipher/Engine/MD5Engine.h"
	#include "./Cipher/Engine/SHA1Engine.h"
	#include "./Cipher/Engine/B64Engine.h"
	#include "./Cipher/Engine/PlugableCipherEngine.h"

	#include "./Cipher/KeyGenerator/KeyGeneratorImpl.h"
	#include "./Cipher/KeyGenerator/KeyPairGeneratorImpl.h"
	#include "./Cipher/KeyGenerator/DESKeyGenerator.h"
	#include "./Cipher/KeyGenerator/3DESKeyGenerator.h"
	#include "./Cipher/KeyGenerator/RC2KeyGenerator.h"
	#include "./Cipher/KeyGenerator/RC4KeyGenerator.h"
	#include "./Cipher/KeyGenerator/SM4KeyGenerator.h"
	#include "./Cipher/KeyGenerator/PlugableKeyGenerator.h"
	#include "./Cipher/KeyGenerator/RSAKeyGenerator.h"

	#include "./Cipher/CipherFactory.h"	
	#include "./Cipher/KeyGenerator.h"	
	#include "./Cipher/KeyPairGenerator.h"	
	
	#include "./Cipher/Key.h"
	#include "./Cipher/DESKey.h"
	#include "./Cipher/3DESKey.h"
	#include "./Cipher/RC2Key.h"
	#include "./Cipher/RC4Key.h"
	#include "./Cipher/SM4Key.h"
	#include "./Cipher/RSAPublicKey.h"
	#include "./Cipher/RSAPrivateKey.h"
	#include "./Cipher/PlugableKey.h"
	#include "./Cipher/SM2PublicKey.h"
	#include "./Cipher/SM2PrivateKey.h"

	#include "./Cipher/AlgorithmParameter.h"
	#include "./Cipher/RSAParameter.h"
	#include "./Cipher/IVParameter.h"
	#include "./Cipher/RC2Parameter.h"
	#include "./Cipher/SM4Parameter.h"

	#include "./Cipher/SM/SM.h"

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
///   1.0.0.0  2003/04/25 14:44  Hongwei Yu   created 
///   1.0.0.1  2003/07/01 14:25  Hongwei Yu   switch /MT and /MD case  
///   1.0.0.2  2003/10/16 14:32  Hongwei Yu   remove _DEBUG switch 
///   1.0.0.3  2003/10/23 11:55  Hongwei Yu   restore _DEBUG switch

#endif
