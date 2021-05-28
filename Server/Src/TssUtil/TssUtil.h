#ifndef __TSSUTIL_H_
#define __TSSUTIL_H_
#include "FJCAApiConst.h"
#include "TssDefine.h"

#if defined(_WIN32)
	#ifdef TSSUTIL_EXPORTS
		#define TSSUTIL_EXPORTS __declspec( dllexport )
	#else
		#define TSSUTIL_EXPORTS __declspec( dllimport )
	#endif
#elif defined(_LINUX)
	#define TSSUTIL_EXPORTS
#endif


TSSUTIL_EXPORTS SGD_UINT32 STF_InitEnvironment(void **phTSHandle);

TSSUTIL_EXPORTS SGD_UINT32 STF_ClearEnvironment(void* hTSHandle);

TSSUTIL_EXPORTS SGD_UINT32 STF_GetCurrentTime(SGD_UINT8 *url, SGD_UINT8 *currentTime );

TSSUTIL_EXPORTS SGD_UINT32 STF_CreateTSRequest( void* hTSHandle, 
	SGD_UINT8 *pucInData, 
	SGD_UINT32 uiInDataLength, 
	SGD_UINT32 uiReqType, 
	SGD_UINT8 *pucTSExt,
	SGD_UINT32 *TSExtLength,
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT8 *pucTSRequest, 
	SGD_UINT32 *puiTSRequestLength);

TSSUTIL_EXPORTS SGD_UINT32 STF_GetTSReponse (void* hTSHandle,
	SGD_UINT8 *url, 
	SGD_UINT8 *pucTSRequest,
	SGD_UINT32 uiTSRequestLength,
	SGD_UINT32 uiSignatureAlgID,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 *puiTSResponseLength);


TSSUTIL_EXPORTS SGD_UINT32 STF_VerifyTSValidity(void* hTSHandle,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT32 uiSignatureAlgID, 
	SGD_UINT8 *pucTSCert, 
	SGD_UINT32 uiTSCertLength);

TSSUTIL_EXPORTS SGD_UINT32 STF_GetTSInfo(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT8 *pucIssuerName, 
	SGD_UINT32 *puiIssuerNameLength, 
	SGD_UINT8 *pucTime, 
	SGD_UINT32 *puiTimeLength);

TSSUTIL_EXPORTS SGD_UINT32 STF_GetTSDetail(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiItemnumber, 
	SGD_UINT8 *pucItemValue, 
	SGD_UINT32 *puiItemValueLength);

#endif
