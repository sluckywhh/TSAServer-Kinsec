#ifndef __PKI_BASE_UTIL_H__
#define __PKI_BASE_UTIL_H__

/**
  * @Name: PKIOCSPUtil.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 02:00
  * @Description: PKI basic API functions  
*/ 

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

class ITokenSign;

KTINT32
KTPKI_SignObject(
		IN OUT KTToBeSigned				*pToBeSigned,
		IN ITokenSign				*pTokenSign,
		IN KTHANDLE					hPrvKey,
		IN const char* const		pcAlgId
);

KTINT32 
KTPKI_VerifyObject(
		IN const KTToBeSigned				*pToBeSigned,
		IN const CRSAPublicKey&		publicKey,
		IN ITokenSign			*pTokenSign=NULL
);

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 02:02  Hongwei Yu   created 

#endif
