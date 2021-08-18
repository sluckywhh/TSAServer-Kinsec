#ifndef _MWKEY_EKEY_PCSC_HPP_TAG
#define _MWKEY_EKEY_PCSC_HPP_TAG

#ifndef _DEBUG
	#pragma comment( lib, "xcsp_eclib.lib" )
	#pragma message( "Automatically linking with xcsp_eclib.lib(MWeKey PCSC)" )
#else
	#pragma comment( lib, "xcsp_eclib.lib" )
	#pragma message( "Automatically linking with xcsp_eclib.lib(MWeKey PCSC)" )
#endif
#include "./Key/MWeKeyPCSC/xcos_eclibt.h"
#include "./Key/MWeKeyPCSC/xcos_eclibf.h"

//
#endif
