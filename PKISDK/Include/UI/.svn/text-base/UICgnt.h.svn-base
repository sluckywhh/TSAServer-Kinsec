#ifndef _KT_CGNT_API_H
#define _KT_CGNT_API_H

/**
  *@Name: KTCgntAPI.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/03/25 14:16
  *@Description: all Cgnt function envelope
*/
#ifdef __cplusplus
extern "C"	{
  class KTCertificates;
  class KTCertificate;
  class KTCRLSequence;
  class KTCertificateList;
#ifndef KTCRL
  typedef KTCertificateList KTCRL;
#endif
  class KTSequenceOf;
  class CWnd;
#else
  typedef void KTCertificates; 
  typedef void KTCertificate; 
  typedef void KTCRL; 
  typedef void CWnd;
  typedef void KTSequenceOf;
#endif

KT_LIB_MODE int KTCgnt_GetLastErrNum();

KT_LIB_MODE int KTCgnt_Thread_Initialize();
KT_LIB_MODE int KTCgnt_Thread_Finalize();

KT_LIB_MODE void KTCgnt_ShowVerifyOptIfNoCRLDlg();

//提供证书选择框供用户选择指定证书
KT_LIB_MODE BOOL KTCgnt_ShowCertSelectDlg( CWnd *pParent, 
				KTHANDLE *phHandles, 
				UINT *puSize, 
				LPCTSTR pcszCaption );

//提供对话框下载用户证书
KT_LIB_MODE BOOL KTCgnt_ShowUCHttpDownload( KT_SESSION_HANDLE hSession,
							    CWnd* pParent, 
								LPCTSTR pcszTitle, 
								LPCTSTR pcszServer, 
								LPCTSTR pcszReqPath, 
								int iPort,
								KTCertificates* pCerts
								);

//得到用户输入的用于对称加密的密码
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetEncryptPassWord(
				CWnd*	pParent,
				int*	pnLen,		//input, 缓冲区大小(至少64字节);output, 口令长度
				KTChar*	pc_pwd,		
				bool*	pbUseBakCert );	//是否使用备份证书 	

//得到用户输入的用于对称解密的密码
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetDecryptPassWord(
				CWnd*	pParent,					  
				int*	pnLen,
				KTChar*	pc_pwd, 
				bool*	pbUseBakCert );


//得到用于加密签名的证书
//pc_addresser:		发件人地址(文件加解密时为"")
//pc_addressee:		收件人地址列表, 以",'或";"分隔(文件加解密时为"")
//bAbbreviation:	是否忽略Notes收件人地址中的@及其以后部分,一般只有在Notes应用中才会忽略
//bSelfEnvelop:		是否用自己的个人证书加密
//hSession:		打开的个人会话,可为空
//pnLen:		传入时是缓冲区长度地址,返回时是证书句柄字符串长度地址(缓冲区不够长时,返回需要的长度)	
//pc_certHanles:	证书句柄字符串			
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetEnvelopSignCertHandle(
				const KTChar*	pc_addresser,		//发件人
				const KTChar*	pc_addressee,		//收件人
				bool	bNotesAbv,
				bool	bSelfEnvelop,
				KT_SESSION_HANDLE	hSession,
				int*	pnEnveHandleCount,
				KTHANDLE*	phEnveHandles,
				int*	pnSignHandleCount,
				KTHANDLE*	phSignHandles);

//得到用于加密的证书			
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetEnvelopCertHandle(
				const KTChar*	pc_addresser,		//发件人
				const KTChar*	pc_addressee,		//收件人
				bool	bNotesAbv,
				bool	bSelfEnvelop,
				KT_SESSION_HANDLE	hSession,
				int*	pnHandleCount,
				KTHANDLE*	phHandles );
										

//得到用于签名的证书
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetSignCertHandle( 
				const KTChar*	pc_addresser,			//发件人
				bool	bNotesAbv,
				KT_SESSION_HANDLE	hSession,
				int*	pnHandleCount,
				KTHANDLE*	phHandles );
	
/////设置算法函数 begin	

KT_LIB_MODE const KTChar* KTCgnt_GUI_UpdateAlgorithm(KT_SESSION_HANDLE hSession, CWnd* pParent,
                                      KT_ALGORITHM_INFO* pAlgoInfo);
/////设置算法函数 end

//////显示证书属性函数 begin
KT_LIB_MODE const KTChar* KTCgnt_ShowCertProp( CWnd* pParentWnd,		//CWnd*
						const KTCertificate* pCert );

KT_LIB_MODE const KTChar* KTCgnt_ShowCertitificateProp( CWnd* pParentWnd,   //CWnd*
						 UINT unDerCertLen,
						 BYTE* pbDerCertData );

KT_LIB_MODE const KTChar* KTCgnt_ShowCertitificateProperty( CWnd* pParentWnd,  //CWnd*
						KT_SESSION_HANDLE hSession,
						KTHANDLE hCertHandle );
//////显示证书属性函数 end

//////显示CRL属性函数 begin
KT_LIB_MODE const KTChar* KTCgnt_ShowCRLProp( CWnd* pParentWnd,		//CWnd*
						const KTCRL* pCRL );
KT_LIB_MODE const KTChar* KTCgnt_ShowCRLProp2( CWnd* pParentWnd,		//CWnd*
						KTByteArray& rCRL );
//////显示CRL属性函数 begin


///////////证证书有效性相关函数开始

//////是否指定证书的Root证书
KT_LIB_MODE bool KTI_IsGrandFatherCA( KT_SESSION_HANDLE hSession, 
				            const KTCertificate* pCert, 
				            const KTCertificate* pCACert );

//////验证当前用户是否有效
KT_LIB_MODE const KTChar* KTCgnt_GUI_CheckUser( KT_SESSION_HANDLE hSession,
							 bool bNotesAbv, 
							 const KTChar* pc_userID );

//////根据指定的CA证书验证当前用户是否有效
KT_LIB_MODE const KTChar* KTCgnt_GUI_CheckUserEx( KT_SESSION_HANDLE hSession,
							   bool bNotesAbv,
							   const KTChar*  pc_userID,
							   KTChar* pc_b64DERCACerts );

KT_LIB_MODE const KTChar* KTCgnt_GetCertTrain(KT_SESSION_HANDLE hSession, 
						const KTCertificate* pCert, 
						const KTCertificates* pCertTrain);

KT_LIB_MODE const KTChar* KTCgnt_GetCertChainAndCRL( KT_SESSION_HANDLE hSession,
						const KTCertificate* pCert,
						KTCertificates* pCertChain,
                        bool* pbExistCACRL,
						KTCRL* pCRL );

KT_LIB_MODE const KTChar* KTCgnt_CheckCertsByCRL( KT_SESSION_HANDLE hSession, 
					 const KTCertificates* pCerts,
					 int nCertCount,
					 int* pnCertStatus,
					 CWnd* pParentWnd );

KT_LIB_MODE const KTChar* KTCgnt_CheckCertByCRL( KT_SESSION_HANDLE hSession,
					const KTCertificate* pCert,
					int* pnCertStatus,
					CWnd* pParentWnd );

KT_LIB_MODE const KTChar* KTCgnt_CheckHandleCertByCRL( KT_SESSION_HANDLE hSession,
					KTHANDLE hCert,
					int* pnCertStatus,
					CWnd* pParentWnd,
					struct tm *rtm = NULL);
//////验证证书有效性函数结束


//////ldap下载相关函数 begin
KT_LIB_MODE const KTChar* KTCgnt_DownObj( 
					const KTChar* const pc_filter,
					const KTChar* const pc_attrNm,
					const KTChar* const pc_msg,
					CWnd* pParent, 
					bool bSilent /*= false */,
                    KTSequenceOf* pObjSequence);

KT_LIB_MODE const KTChar* KTCgnt_DownRootCertWithCommonName(
							const KTChar* const pc_commonName, ////if NULL, down all root cert 
							CWnd* pParent,
							bool bSilent,
                            KTCertificates* pCerts);

KT_LIB_MODE const KTChar* KTCgnt_DownInnerCACertWithCommonName(
							const KTChar* pc_commonName,  ////if NULL, down all CA cert
							CWnd* pParent,
							bool bSilent,
                            KTCertificates* pCerts);

//down root ca certs and inner cacerts
KT_LIB_MODE const KTChar* KTCgnt_DownCACertWithCommonName(
							const KTChar* const pc_commonName,  ////if NULL, down all CA cert
							CWnd* pParent,
							bool bSilent,
                            KTCertificates* pCerts);

KT_LIB_MODE const KTChar* KTCgnt_GUI_DownUserCert(
						    KTChar* pc_initEditText,
						    CWnd* pParent,
						    bool bSilent,
						    bool bOnlyCipherCert,
                            KTCertificates* pCerts);

KT_LIB_MODE const KTChar* KTCgnt_DownUserCertWithEmail(
							const KTChar* const pc_email,		//cann't be NULL
							CWnd* pParent,
							bool bSilent,
                            bool bOnlyCipherCert,
                            KTCertificates* pCerts );

KT_LIB_MODE const KTChar* KTCgnt_DownUserCertWithSerialNumber( 
							const KTChar* const pc_serialNumber, //cann't be NULL
							CWnd* pParent,
							bool bSilent,
                            bool bOnlyCipherCert,
                            KTCertificates* pCerts );

KT_LIB_MODE const KTChar* KTCgnt_DownUserCertWithCommonName(
							const KTChar* const pc_commonName, 
							CWnd* pParent,
							bool bSilent,
                            bool bOnlyCipherCert,
                            KTCertificates* pCerts);

KT_LIB_MODE const KTChar* KTCgnt_DownCRL(
						    CWnd* pParent,
						    bool bSilent,
                            KTCRLSequence* pCRLs );

KT_LIB_MODE const KTChar* KTCgnt_DownCRLAndImport_Open(
								KT_SESSION_HANDLE hSession,
								CWnd* pParent,
								bool bSilent,
								KTHANDLE& hCrl);
//////ldap下载相关函数 end

KT_LIB_MODE const KTChar* KTCgnt_PutCRLs(KT_SESSION_HANDLE hSession, const KTCRLSequence* pCRLs);
KT_LIB_MODE const KTChar* KTCgnt_PutCerts(KT_SESSION_HANDLE hSession,
					KT_TOKEN_OBJ_TYPE nType, const KTCertificates* pCerts);
KT_LIB_MODE const KTChar* KTCgnt_PutCertsEx(KT_SESSION_HANDLE hSession,
					KT_TOKEN_OBJ_TYPE nType, const KTCertificates* pCerts, KTHANDLE phHandles[], KTBOOL pbSuccess[]);

KT_LIB_MODE bool KTCgnt_GUI_IsTrustRootCert( CWnd* pParentWnd,
									const KTCertificate* pCert );

KT_LIB_MODE bool KTCgnt_B64CertOrCRLToDER( BYTE* pInput, UINT unInputLen, 
					 BYTE* pOutput, UINT* punOutputLen );

KT_LIB_MODE CWnd* KTCgnt_HWNDToCWnd(HWND hwnd);

enum enumPropPage
{
	PropPageLoginInfo=0x001,
	PropPageCertPersonal=0x002,
	PropPageCertUser=0x004,
	PropPageCertCA=0x008,
	PropPageCertRootCA=0x010,
	PropPageCRL=0x020,
	PropPageKey=0x040,
	PropPageSlot=0x080,
	PropPageCfgParam=0x100,
	PropPageSecSealManage=0x200,
	PropPageCfgParam2 = 0x400,
	PropPagesTssManage	= 0x800,		
	PropPagesCert=PropPageCertPersonal|PropPageCertUser|PropPageCertCA|PropPageCertRootCA,
	PropPagesPKIEgn=PropPagesCert|PropPageCRL|PropPageSlot|PropPageCfgParam|PropPageCfgParam2,
	PropPageCertThreeCA=0x009,
};

KT_LIB_MODE const KTChar* KTCgnt_RunPropSheet(KT_SESSION_HANDLE hSession, enumPropPage selectPage, CWnd* pParentWnd, 
									   KTUINT unPropPages, const KTChar* pc_caption);

KT_LIB_MODE BOOL KTCgnt_OpenFileNormal(LPCTSTR szFileNm, bool bWait, LPCTSTR szCmdLnParam);

/*
nOption  [7][6][5][4][3][2][1][0]	
	第0位 0=询问用户是否下载, 1=自动下载
	第1位 0=询问用户是否安装, 1=自动安装
	第2位 0=出错时提示用户，  1=自动退出

pc_updtSvrURL=NULL或"" 时自动从UICfg模块中读取
nProductVersion=-1 时自动从UICfg模块中读取
nOption=-1 时自动从UICfg模块中读取
*/
KT_LIB_MODE const KTChar* KTCgnt_UpdtProduct(const char* pc_updtSvrURL, const char* pc_productNm, int nProductVersion, 
				 int nOption, HWND hParentWnd, const char* pc_msgTitle, const char* pc_workFolder, bool bBackground,
				 bool bWaitSetup);

KT_LIB_MODE const KTChar* KTCgnt_UpdtAllProducts(char* pc_moduleNm, int nOption, HWND hParentWnd, char* pc_msgTitle,
												 bool bBackground, bool bWaitSetup);

KT_LIB_MODE const KTChar* KTCgnt_UpdtProductEx(const char* pc_updtSvrURL, const char* pc_productNm, int nProductVersion, 
				 int nOption, HWND hParentWnd, const char* pc_msgTitle, const char* pc_workFolder, bool bBackground,
				 bool bWaitSetup, KTCertificate* pCert);

KT_LIB_MODE const KTChar* KTCgnt_UpdtAllProductsEx(char* pc_moduleNm, int nOption, HWND hParentWnd, char* pc_msgTitle,
												 bool bBackground, bool bWaitSetup, KTCertificate* pCert);


KT_LIB_MODE void KTCgnt_TransCertStatus( int nCertStatus, KTChar pc_certStatus[KT_MAX_PATH]);

KT_LIB_MODE bool KTCgnt_GetSingleField(KTChar* singleField, KTChar* buffField );

KT_LIB_MODE bool KTCgnt_CompareEmailAddr(const KTChar* const pc_email, 
					  const KTChar* const pc_emails,
					  bool bEmailsNotesAbv ); 

#ifdef __cplusplus


KT_LIB_MODE bool KTCgnt_IsGrandFatherCA( KT_SESSION_HANDLE hSession, 
				        const KTCertificate& cert, 
				        const KTCertificate& caCert );

KT_LIB_MODE bool KTCgnt_IsExistSuperiorCA( KT_SESSION_HANDLE hSession, 
				        const KTCertificate& cert, 
				        const KTCertificates& caCerts );

KT_LIB_MODE const KTChar* KTCgnt_GUI_CheckUserEx2( KT_SESSION_HANDLE hSession,
							 bool bNotesAbv, 
							 const KTChar* pc_userID,
							 const KTCertificates* pCACerts );

#if defined(_DLL)

KT_LIB_MODE void KTCgnt_TranslateCertStatus( int nCertStatus, 
							   KTString& strStatus );

KT_LIB_MODE void KTCgnt_NotesAbvString( const KTChar* const szEmail,
					        KTString& strEmail );

KT_LIB_MODE BOOL KTCgnt_ShowCertSelectDlgEx( CWnd* pParent, 
							KTHandleArray& arr, 
							LPCTSTR pcszCaption );

KT_LIB_MODE void  KTCgnt_ConvertStringToArray(const KTChar* const lpszEmails , 
						   bool bEmailsNotesAbv,
						   KTStringArray& strArray );

KT_LIB_MODE bool KTCgnt_CompareEmailAddrEx( KTStringArray& strArrAddressee,
						const KTStringArray& strArrEmail,
						KTUintArray & dwArrOffset );

//得到用于加密签名的证书
//Pc_objID:			发件人收件人地址在证书中对应的ID, 例如ObjID_emailAddress
//pc_addresser:		发件人地址(文件加解密时为"")
//pc_addressee:		收件人地址列表, 以",'或";"分隔(文件加解密时为"")
//bAbbreviation:	是否忽略Notes收件人地址中的@及其以后部分,一般只有在Notes应用中才会忽略
//bSelfEnvelop:		是否用自己的个人证书加密
//hSession:			打开的个人会话,可为空	
//rHandle:			返回的证书句柄列表			
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetEnvelopSignCertHandleEx(
				const KTChar*	pc_objID,			//if NULL, get from config info
				const KTChar*	pc_addresser,		//发件人
				const KTChar*	pc_addressee,		//收件人
				bool	bNotesAbv,
				bool	bSelfEnvelop,
				KT_SESSION_HANDLE	hSession,
				KTHandleArray&	hArrEnvCert,
				KTHandleArray&	hArrSignCert);

//得到用于加密的证书			
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetEnvelopCertHandleEx(
				const KTChar*	pc_objID,			//if NULL, get from config info
				const KTChar*	pc_addresser,		//发件人
				const KTChar*	pc_addressee,		//收件人
				bool	bNotesAbv,
				bool	bSelfEnvelop,
				KT_SESSION_HANDLE	hSession,
				KTHandleArray&	rHandle);
										

//得到用于签名的证书
KT_LIB_MODE const KTChar* KTCgnt_GUI_GetSignCertHandleEx(
				const KTChar*	pc_objID,				//if NULL, get from config info											  
				const KTChar*	pc_addresser,			//发件人
				bool	bNotesAbv,
				KT_SESSION_HANDLE	hSession,
				KTHandleArray&	rHandle);

//使用时间戳进行签名
KT_LIB_MODE const KTChar* KTCgnt_TimeStamp( 
	const char *szUrl,/*[in]时间戳服务地址*/
	const char *szSrc,/*[in]原文*/
	KTString *pstrCipher,/*[out]时间戳签名数据*/
	bool bUIShow,/*[in]是否显示窗口*/
	int &nRetSelID/*[out]在选用UI下,返回对话框选择的按钮ID*/
	);
KT_LIB_MODE const KTChar* KTCgnt_TimeStampEx( 
	const char *szUrl,/*[in]时间戳服务地址*/
	const unsigned char *pbSrc,/*[in]原文*/
	int nSrcLen,/*[in]原文长度*/
	unsigned char *pbCipher,/*[out]时间戳签名数据*/
	int *pnCipherLen/*[out]时间戳签名数据长度*/
	);
KT_LIB_MODE const KTChar* KTCgnt_TimeStampEx_WithStatus( 
	const char *szUrl,/*[in]时间戳服务地址*/
	const unsigned char *pbSrc,/*[in]原文*/
	int nSrcLen,/*[in]原文长度*/
	unsigned char *pbCipher,/*[out]时间戳签名数据*/
	int *pnCipherLen/*[out]时间戳签名数据长度*/
	);
//使用时间戳进行验证
KT_LIB_MODE const KTChar* KTCgnt_VerifyTimeStamp(
	const char *szUrl,/*[in]时间戳服务地址*/
	const char *szCipher,/*[in]时间戳签名数据*/
	const char *szSrc,/*[in]输入原文*/
	KTCertificate *pcertSign,/*[out]签名证书*/
	KTString *pstrStampTime,/*[out]时间戳时间(字符串)*/
	bool bUIShow = FALSE
	);
KT_LIB_MODE const KTChar* KTCgnt_VerifyTimeStampEx(
	const char *szUrl,/*[in]时间戳服务地址*/
	const unsigned char *pbCipher,/*[in]时间戳签名数据*/
	int nCipherLen,/*[in]时间戳签名数据长度*/
	const unsigned char *pbSrc,/*[in]原文*/
	int nSrcLen/*[in]原文长度*/
	);
//解析时间戳
KT_LIB_MODE const KTChar* KTCgnt_GetTimeStampInfoEx(
	const char *szUrl,/*[in]时间戳服务地址*/
	const unsigned char *pbCipher,/*[in]时间戳签名数据*/
	int nCipherLen,/*[in]时间戳签名数据长度*/
	int nFlag,/*[in]标志 0-时间  1-证书  2-签名数据  3-时间戳原文摘要  4-时间戳原文摘要算法  10-Z结尾的时间*/
	unsigned char *pbParseData,/*[in]输出数据*/
	int *pnParseDataLen/*[in]输出数据长度*/
	);

//获取错误描述
KT_LIB_MODE int KTCgnt_GetErrDescriptionByTS(
	const char *szUrl,/*[in]时间戳服务地址*/
	int nError,/*[in]错误号*/
	KTString& strError/*[out]错误描述*/
	);

KT_LIB_MODE const KTChar* KTCgnt_DownloadFile(
	const char *pszUrl,/*[in]文件下载地址*/
	const char *pszDestPath/*[in]文件保存路径*/
	);

KT_LIB_MODE const KTChar* KTCgnt_DownloadFileEx(
	const char *pszCAFile,/*[in]CA证书文件路径*/
	const char *pszUrl,/*[in]文件下载地址*/
	const char *pszDestPath/*[in]文件保存路径*/
	);

KT_LIB_MODE const KTChar* KTCgnt_HTTPRequest(
	const char *pszCAFile,/*[in]CA证书文件路径*/
	const char *pszUrl,/*[in]请求地址*/
	const char *pszParam,/*[in]请求参数*/
	KTByteArray& rResult/*[out]返回数据*/
	);

//======================================
KT_LIB_MODE const KTChar*	KTBsc_CertFindPrvKeyEx(
					KT_SESSION_HANDLE hSession, 
					KTBYTE* pCert, 
					KTUINT nSize, 
					KTHANDLE& hPrvKey );

KT_LIB_MODE const KTChar*   KTBsc_ListData2( KT_SESSION_HANDLE		hSession,
				    KT_TOKEN_OBJ_TYPE		nObjectType,
				    const BYTE*				pPropHash,	//default=NULL
				    UINT 					nPropHashLen,
                    KTHandleArray&          handleArr);

KT_LIB_MODE const KTChar*   KTBsc_GetData2( KT_SESSION_HANDLE  hSession,    
                    KTHANDLE	            handle,
                    KTByteArray&            baData );
//由级别，获取证书句柄
KT_LIB_MODE const KTChar*   KTBsc_ListDataByTrustLevel( KT_SESSION_HANDLE		hSession,
											KT_TOKEN_OBJ_TYPE				nObjectType,
											KTUINT32						nObjectTrustLevel,
											const BYTE*				pPropHash,	//default=NULL
											UINT 					nPropHashLen,
                    KTHandleArray&          handleArr);
//由CA别名，获取证书句柄
KT_LIB_MODE const KTChar*   KTBsc_ListDataByCAAltName( KT_SESSION_HANDLE		hSession,
													   KT_TOKEN_OBJ_TYPE		nObjectType,
													   CHAR*			nObjectCAAltName,
													   const BYTE*				pPropHash,	//default=NULL
													   UINT 					nPropHashLen,
                    KTHandleArray&          handleArr);


} //extern "C"

#else

} //extern "C"

#include "./UICfg.hpp"
#include "./UIBasic.hpp"

KT_LIB_MODE const KTChar*	KTBsc_CertFindPrvKeyEx(
					KT_SESSION_HANDLE hSession, 
					KTBYTE* pCert, 
					KTUINT nSize, 
					KTHANDLE& hPrvKey );

inline const KTChar*   KTBsc_ListData2( KT_SESSION_HANDLE		hSession,
				KT_TOKEN_OBJ_TYPE		nObjectType,
				const BYTE*				pPropHash,	//default=NULL
				UINT 					nPropHashLen,
                KTHandleArray& handleArr)
{	
	int rv =0;
	KTUINT unSize=0;
	do{
		unSize = handleArr.size()+KT_MAX_PATH;
		handleArr.resize(unSize);
		rv = KTPKI_ListData( hSession, nObjectType, pPropHash, nPropHashLen, 
				&handleArr[0], &unSize );
	}while(rv==KT_ERR_BUFFER_SMALL);

	if( rv!=0 ) 
		return KTPKI_UI_SetErr( "不能从证书存储设备中读取数据列表,%s!\n\n当前会话%d,当前数据类型%d!", 
								 KTPKI_GetErrorString(rv), hSession, nObjectType );
	handleArr.resize(unSize);
	return NULL;	
}

inline const KTChar*   KTBsc_GetData2( KT_SESSION_HANDLE  hSession,    
              KTHANDLE	    handle,
              KTByteArray&  baData )
{
	KTUINT unSize=0;
    const KTChar* pc_err = KTBsc_GetDataSize(hSession, handle, &unSize);
    if(pc_err)  return pc_err;

    baData.resize(unSize);
    return KTBsc_GetData(hSession, handle, &baData[0], &unSize);
}

#endif

#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/03/25 14:23  Hongwei Yu   Created 
///   1.0.0.1  2003/05/09 09:23  Hongwei Yu   move C++ API functions from KTCgnt.h
///   1.0.0.2  2003/06/03 16:46  Hongwei Yu   added KTCgnt_GUI_Get***HandleEx functions
///   1.0.0.3  2003/06/11 21:35  Hongwei Yu   added const in some function paramters
///   1.0.0.4  2003/08/29 12:26  Hongwei Yu   added KTBsc* and KTCgnt_DownObj
///   1.0.0.5  2003/10/16 15:34  Hongwei Yu   supported KTBsc* in MT and auto update
///   1.0.0.6  2003/12/10 16:48  ChenLingChan added KTCgnt_ShowCertSelectDlg**
///   1.0.0.7  2003/12/11 14:55  Hongwei Yu   added KTCgnt_TransCertStatus
///   1.0.0.8  2003/12/29 18:58  Chenlc		  added KTCgnt_ShowUCHttpDownload
///   1.0.0.9  2003/12/30 17:19  Hongwei Yu   added KTCgnt_OpenFileNormal

#endif