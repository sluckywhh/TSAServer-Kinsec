#ifndef _KT_BASIC_API_H_
#define _KT_BASIC_API_H_

/**
  *@Name: KTBasicAPI.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/03/24 22:01
  *@Description: encape all Kind PKI SDK function
*/

#ifdef __cplusplus
  extern "C"{	  
#endif

KT_LIB_MODE int KTBsc_GetLastErrNum();

KT_LIB_MODE KTDWORD KTBsc_GetResetTimeInterval();

KT_LIB_MODE void KTBsc_SetResetTimeInterval(KTDWORD nResetTimeInterval);

///initialize && finalize
KT_LIB_MODE const KTChar*  KTBsc_Initialize( void* pReserved );

KT_LIB_MODE const KTChar*  KTBsc_Finalize( void* pReserved );
/////////////////////////

////////session begin
KT_LIB_MODE const KTChar* KTBsc_GetSlotCount( UINT* punSlotCount, bool bResetSlot );

KT_LIB_MODE const KTChar* KTBsc_GetSlotInfo( UINT unSlotID, KT_SLOT_INFO* pSlotInfo );

KT_LIB_MODE const KTChar* KTBsc_GetTokenInfo( UINT unSlotID, KT_TOKEN_INFO* pTokenInfo );

KT_LIB_MODE const KTChar* KTBsc_Login( UINT unSlotID , const KTChar* pcUserNm, const KTChar* pcUserPIN,
                KT_SESSION_HANDLE* phSession);

KT_LIB_MODE const KTChar*   KTBsc_LoginPubSession( KT_SESSION_HANDLE * phSession );

KT_LIB_MODE const KTChar*   KTBsc_UILogin( HWND hParentWnd,		/////父窗口,可为NULL
				bool bPermitNewUser,		/////是否允许新用户登录
				bool bForceReset,				/////是否强制复位
				KT_SESSION_HANDLE* phSession );	/////session handle

KT_LIB_MODE const KTChar* KTBsc_GetSessionWithoutLogin(
				KT_SESSION_HANDLE* phSession);	/////session handle

KT_LIB_MODE const KTChar*   KTBsc_UILoginEx( HWND hParentWnd,  /////父窗口,可为NULL
					bool bPermitNewUser,  /////是否允许新用户登录
					bool bForceReset,    /////是否强制复位
					BYTE* pbUserNm,	////用户名
					int  nUserNmLen,	////用户名长度
					BYTE* pbUserPWD,	////用户口令
					int  nUserPWDLen,	////用户口令长度
					KT_SESSION_HANDLE* phSession );

KT_LIB_MODE  const KTChar*  KTBsc_UILoginEx2( HWND hParentWnd,		/////父窗口,可为NULL
				bool bPermitNewUser,		      /////是否允许新用户登录
				bool bForceResetAllSlot,		  /////是否强制复位所有Slot
				bool bUnusedSlotOnly,			  /////仅使用未使用的Slot				
				bool bUseLastLoginInfo,			  /////是否使用上次登录信息
				bool bCheckLoginResetTime,		  /////使用上次登录信息时是否检测引擎保持时间
				bool bShowLoginDlg,				  /////是否显示登陆对话框
				KT_SESSION_HANDLE* phSession);	  /////session handle

KT_LIB_MODE const KTChar*   KTBsc_GetSessionState( 
                          KT_SESSION_HANDLE hSession, 
						  bool* pbLogin, 
						  bool* pbPresent );

KT_LIB_MODE const KTChar*   KTBsc_GetLoginfo( 
                        KT_SESSION_HANDLE hSession,
                        unsigned long* plSlotID, 
					    unsigned long* plSlotType,
					    char pc_userName[65] );

KT_LIB_MODE const KTChar* KPBsc_SetPIN( KT_SESSION_HANDLE hSession, 
              const KTChar* const pcOldPIN, const KTChar* const pcNewPIN );

KT_LIB_MODE const KTChar*   KTBsc_Logout( KT_SESSION_HANDLE hSession );

KT_LIB_MODE const KTChar*   KTBsc_LockUILoginfo();

KT_LIB_MODE void  KTBsc_GetLoginSessionState(bool* pbLogin, 
						  bool* pbPresent );

////////session end


KT_LIB_MODE const KTChar*  KTBsc_SoftGenerateRandom( unsigned int  unLen, 
							   BYTE * pbBuf );

KT_LIB_MODE const KTChar*  KTBsc_GetAlgorithmInfo(KT_ALGORITHM_INFO* pAlgoInfo, UINT* punSize);

KT_LIB_MODE const KTChar*   KTBsc_B64_Code( bool bEncode,
					    const BYTE * pInput,
					    UINT  nInputLen,
					    BYTE * pOutput,
					    UINT  * pnOutputLen);

KT_LIB_MODE const KTChar*   KTBsc_ListData( KT_SESSION_HANDLE		hSession,
				        KT_TOKEN_OBJ_TYPE		nObjectType,
				        const BYTE*				pPropHash,	//default=NULL
				        UINT 					nPropHashLen,
				        KTHANDLE*			    phHandleList,
				        UINT *				    pnCount );

KT_LIB_MODE const KTChar*   KTBsc_GetDataSize(
                    KT_SESSION_HANDLE  hSession,    
                    KTHANDLE	handle,
                    UINT* punSize);
//获取数据级别
KT_LIB_MODE const KTChar*   KTBsc_GetDataTrustLevel(
                    KT_SESSION_HANDLE  hSession,    
                    KTHANDLE	handle,
                    UINT* punTrustLevel);

//获取数据别名
KT_LIB_MODE const KTChar*   KTBsc_GetDataAltName(
											   KT_SESSION_HANDLE  hSession,    
											   KTHANDLE	handle,
                    CHAR* punAltName);

//获取CA别名
KT_LIB_MODE const KTChar*   KTBsc_GetDataCAAltName(
												 KT_SESSION_HANDLE  hSession,    
												 KTHANDLE	handle,
                    CHAR* punCAaltName);

KT_LIB_MODE const KTChar*   KTBsc_GetData( KT_SESSION_HANDLE  hSession,    
                    KTHANDLE	handle,
                    BYTE*  pData,
                    UINT *  pnDataLen );

KT_LIB_MODE const KTChar*   KTBsc_RemoveData( KT_SESSION_HANDLE hSession,
					KTHANDLE handle );

KT_LIB_MODE const KTChar*   KTBsc_PutData( KT_SESSION_HANDLE  hSession,    
                    KT_TOKEN_OBJ_TYPE   nType,
                    const BYTE*         pData,
                    UINT                nDataLen,
                    const BYTE*         pPropHash,
                    UINT                nPropHashLen,
                    KTHANDLE*           phHandle );
//导入数据。增加了级别、别名和CA别名
KT_LIB_MODE const KTChar*   KTBsc_PutDataByTrustLevelCAAltName( KT_SESSION_HANDLE  hSession,    
										  KT_TOKEN_OBJ_TYPE   nType,
										  const BYTE*         pData,
										  UINT                nDataLen,
										  const BYTE*         pPropHash,
										  UINT                nPropHashLen,
										  KTUINT						nTrustLevel,//级别
										  CHAR*						nAltName,//别名
										  CHAR*						nCAAltName,//CA别名
										  KTHANDLE*           phHandle );

KT_LIB_MODE const KTChar*   KTBsc_MD5_PutData(	KT_SESSION_HANDLE			hSession,
				    KT_TOKEN_OBJ_TYPE			nType,
				    const BYTE*					pData,
				    UINT 						nDataLen,
				    KTHANDLE*				    phHandle );

KT_LIB_MODE const KTChar*   KTBsc_SHA1_PutData( KT_SESSION_HANDLE			hSession,
				    KT_TOKEN_OBJ_TYPE			nType,
				    const BYTE*			        pData,
				    UINT 						nDataLen,
				    KTHANDLE*			        phHandle );

              
#ifdef __cplusplus

#include "KTSTL.h"
class KTObject;
class KTCertificate;

KT_LIB_MODE const KTChar*   KTBsc_CertFindPrvKey(KT_SESSION_HANDLE hSession, 
                    const KTCertificate&    cert, 
                    KTHANDLE&               hPrvKey);

KT_LIB_MODE const KTChar*   KTBsc_GetObject( KT_SESSION_HANDLE  hSession,    
                    KTHANDLE	handle,
					KTObject*	pObj);
  }

#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/03/25 11:14  Hongwei Yu   Created 
///   1.0.0.1  2003/03/28 17:12  Hongwei Yu   added func KTBsc_GetAlgorithmInfo
///   1.0.0.2  2003/05/09 09:16  Hongwei Yu   move C++ API functions from KTBasic.h
///   1.0.0.3  2003/05/09 11:07  Hongwei Yu   added KTBsc_GetObject
///   1.0.0.4  2003/06/17 16:28  Hongwei Yu   KTBsc_GetSlotCount added param reset slot
///   1.0.0.5  2003/12/11 16:52  Hongwei Yu   added KTBsc_UILoginEx2

#endif
