#ifndef __API_CERT_H__
#define __API_CERT_H__

/**
  * @Name: APICert.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 12:00
  * @Description: 证书管理及应用接口 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KTType.h"
#include "PKIDef.h"

#undef EXPORTS

#ifdef WIN32
	#ifndef STATIC_LIB
		#ifdef KT_DLL // assume this is defined when we build the dll
			#define EXPORTS __declspec( dllexport)
		#else
			#define EXPORTS __declspec( dllimport)
		#endif
	#else
		#define EXPORTS  
	#endif
#else
	#define EXPORTS
#endif  //WIN32

//////////////////////////////////////////////////////////////////////////
//init and slot function
//初始化

#ifdef __cplusplus
    extern "C"{
#endif

//#ifdef WIN32
/////////////////////////////////////////////////////////////////////////////////////////////////
//导入/导出证书
EXPORTS KTINT32 
KTPKI_CU_ExportCert(
	  IN  KT_SESSION_HANDLE			hSession,
	  IN  KTHANDLE					hCert,
	  OUT BYTE*					pCertData,  //DER code data
	  IN OUT KTUINT*				pnCertDataLen
);

EXPORTS KTINT32 
KTPKI_CU_ImportCert(
	  IN KT_SESSION_HANDLE			hSession,
	  IN const BYTE*					pCertData,	//DER code data
	  IN KTUINT						nCertDataLen,
	  OUT KTHANDLE					*phCert,	//can be NULL
	  OUT KT_TOKEN_OBJ_TYPE				*pnCertType	//can be NULL					
);
//命令行导入，增加级别和别名
EXPORTS KTINT32 
KTPKI_CU_ImportCertByTrustLevel(
					IN KT_SESSION_HANDLE			hSession,
					IN const BYTE*					pCertData,	//DER code data
					IN KTUINT						nCertDataLen,
					IN KTUINT						nTrustLevel,//级别
					IN char*						nAltName,//别名
					IN char*					nCAaltName,//证书别名
					OUT KTHANDLE					*phCert,	//can be NULL
					OUT KT_TOKEN_OBJ_TYPE				*pnCertType	//can be NULL					
);

//////////////////////////////////////////////////////////////////////////////////////////////////
//

/*EXPORTS KTINT32 
KTPKI_CU_ListCert(
	IN KT_SESSION_HANDLE 			hSession,
	IN KT_TOKEN_OBJ_TYPE				nType,
	OUT KTHANDLE*				pnList,
	IN OUT	KTUINT*				pnCount
);
*/

//////////////////////////////////////////////////////////////////////////////////////////////////
//导入/导出CRL，在共有Token中存放CRLS
EXPORTS KTINT32
KTPKI_CU_ImportCRL(
	  IN KT_SESSION_HANDLE			hSession,
	  IN const BYTE*					pCrlData,	//DER code data
	  IN KTUINT						nCrlDataLen,
	  IN KTBOOL						bCheckSign	//检查签名，需要token2.pub中包含CA证书
);

EXPORTS KTINT32
KTPKI_CU_ExportCRL(
	  IN KT_SESSION_HANDLE			hSession,
	  IN const BYTE*					pCACertData,	//DER code data
	  IN KTUINT						nCACertDataLen,
	  OUT BYTE*					pCrlData,  //DER code data
	  IN OUT KTUINT*				pnCrlDataLen
);

#if defined(KT_WIN32) && defined(KT_USES_OPENSSL)
EXPORTS KTINT32
KTPKI_CU_ImportCRL_Open(
	  IN KT_SESSION_HANDLE			hSession,
	  IN const KTBYTE*				pCrlData,	//DER code data
	  IN KTUINT						nCrlDataLen,
	  IN KTBOOL						bCheckSign
);
#endif
//#endif

#ifdef __cplusplus
    }
#endif

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 11:59  Hongwei Yu   created 

#endif
