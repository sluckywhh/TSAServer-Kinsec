#ifndef _KT_CGNT_API_H
#define _KT_CGNT_API_H

#ifndef HWND
typedef void *HWND;
#endif

#ifdef __cplusplus
	extern "C"{	  
#endif

  class KTCertificates;
  class KTCertificate;
  class KTCRLSequence;
  class KTCertificateList;
#ifndef KTCRL
  typedef KTCertificateList KTCRL;
#endif
  class KTSequenceOf;

KT_LIB_MODE int KTCgnt_GetLastErrNum();

KT_LIB_MODE int KTCgnt_Thread_Initialize();
KT_LIB_MODE int KTCgnt_Thread_Finalize();

///////////时间戳函数 begin
// 使用时间戳进行签名

KT_LIB_MODE const KTChar *KTCgnt_TimeStamp( 
				const char *szUrl,/*[in]时间戳服务地址*/
				const char *szSrc,/*[in]原文*/
				KTString *pstrCipher,/*[out]时间戳签名数据*/
				bool bUIShow,/*[in]是否显示窗口*/
				int &nRetSelID);/*[out]在选用UI下,返回对话框选择的按钮ID*/
KT_LIB_MODE const KTChar *KTCgnt_TimeStampEx( 
				const char *szUrl,/*[in]时间戳服务地址*/
				const unsigned char *pbSrc,/*[in]原文*/
				int nSrcLen,/*[in]原文长度*/
				unsigned char *pbCipher,/*[out]时间戳签名数据*/
				int *pnCipherLen);/*[out]时间戳签名数据长度*/				
KT_LIB_MODE const KTChar *KTCgnt_TimeStampEx_WithStatus( 
				const char *szUrl,/*[in]时间戳服务地址*/
				const unsigned char *pbSrc,/*[in]原文*/
				int nSrcLen,/*[in]原文长度*/
				unsigned char *pbCipher,/*[out]时间戳签名数据*/
				int *pnCipherLen);/*[out]时间戳签名数据长度*/

// 使用时间戳进行验证

KT_LIB_MODE const KTChar *KTCgnt_VerifyTimeStamp(
				const char *szUrl,/*[in]时间戳服务地址*/
				const char *szCipher,/*[in]时间戳签名数据*/
				const char *szSrc,/*[in]输入原文*/
				KTCertificate *pcertSign,/*[out]签名证书*/
				KTString *pstrStampTime,/*[out]时间戳时间(字符串)*/
				bool bUIShow = FALSE);
KT_LIB_MODE const KTChar *KTCgnt_VerifyTimeStampEx(
				const char *szUrl,/*[in]时间戳服务地址*/
				const unsigned char *pbCipher,/*[in]时间戳签名数据*/
				int nCipherLen,/*[in]时间戳签名数据长度*/
				const unsigned char *pbSrc,/*[in]原文*/
				int nSrcLen);/*[in]原文长度*/

//解析时间戳
KT_LIB_MODE const KTChar *KTCgnt_GetTimeStampInfoEx(
				const char *szUrl,/*[in]时间戳服务地址*/
				const unsigned char *pbCipher,/*[in]时间戳签名数据*/
				int nCipherLen,/*[in]时间戳签名数据长度*/
				int nFlag,/*[in]标志 0-时间  1-证书  2-签名数据  3-时间戳原文摘要  4-时间戳原文摘要算法  10-Z结尾的时间*/
				unsigned char *pbParseData,/*[in]输出数据*/
				int *pnParseDataLen);/*[in]输出数据长度*/

// 获取错误描述
KT_LIB_MODE int KTCgnt_GetErrDescriptionByTS(
				const char *szUrl,/*[in]时间戳服务地址*/
				int nError,/*[in]错误号*/
				KTString& strError);/*[out]错误描述*/
///////////时间戳函数 end

///////////文件下载函数 begin
KT_LIB_MODE const KTChar *KTCgnt_DownloadFile(
				const char *pszUrl,/*[in]文件下载地址*/
				const char *pszDestPath/*[in]文件保存路径*/
				);
KT_LIB_MODE const KTChar *KTCgnt_DownloadFileEx(
				const char *pszCAFile,/*[in]CA证书文件路径*/
				const char *pszUrl,/*[in]文件下载地址*/
				const char *pszDestPath/*[in]文件保存路径*/
				);
///////////文件下载函数 end

///////////验证证书有效性函数 begin
KT_LIB_MODE bool KTCgnt_IsGrandFatherCA(
				KT_SESSION_HANDLE hSession,
				const KTCertificate& cert,
				const KTCertificate& caCert);

KT_LIB_MODE bool KTCgnt_IsExistSuperiorCA(
				KT_SESSION_HANDLE hSession,
				const KTCertificate& cert,
				const KTCertificates& caCerts);

KT_LIB_MODE const KTChar *KTCgnt_GetCertTrain(
				KT_SESSION_HANDLE hSession,
				const KTCertificate *pCert,
				KTCertificates *pCertTrain);

KT_LIB_MODE const KTChar *KTCgnt_CheckCertsByCRL(
				KT_SESSION_HANDLE hSession,
				const KTCertificates *pCerts,
				int nCertCount,
				int *pnCertStatus,
				HWND pParentWnd);

KT_LIB_MODE const KTChar *KTCgnt_CheckCertByCRL(
				KT_SESSION_HANDLE hSession,
				const KTCertificate *pCert,
				int *pnCertStatus,
				HWND pParentWnd);

KT_LIB_MODE const KTChar *KTCgnt_CheckHandleCertByCRL(
				KT_SESSION_HANDLE hSession,
				KTHANDLE hCert,
				int *pnCertStatus,
				HWND pParentWnd,
				struct tm *rtm = NULL);

KT_LIB_MODE void KTCgnt_TranslateCertStatus(
				int nCertStatus,
				KTString& strStatus);

KT_LIB_MODE void KTCgnt_TransCertStatus(
				int nCertStatus,
				KTChar pc_certStatus[KT_MAX_PATH]);
///////////验证证书有效性函数 end

///////////显示证书属性函数 begin
KT_LIB_MODE const KTChar *KTCgnt_ShowCertProp(
				HWND pParentWnd,
				const KTCertificate *pCert);

KT_LIB_MODE const KTChar *KTCgnt_ShowCertitificateProp(
				HWND pParentWnd,
				UINT unDerCertLen,
				BYTE *pbDerCertData);

KT_LIB_MODE const KTChar *KTCgnt_ShowCertitificateProperty(
				HWND  pParentWnd,
				KT_SESSION_HANDLE hSession,
				KTHANDLE hCertHandle);

KT_LIB_MODE bool KTCgnt_GUI_IsTrustRootCert(
				HWND  pParentWnd,
				const KTCertificate *pCert);
///////////显示证书属性函数 end


///////////获取用于签名加密的证书 begin
// 得到用于签名的证书
KT_LIB_MODE const KTChar *KTCgnt_GUI_GetSignCertHandle( 
				const KTChar *pc_addresser,			//发件人
				bool bNotesAbv,
				KT_SESSION_HANDLE hSession,
				int *pnHandleCount,
				KTHANDLE *phHandles);

// 得到用于签名的证书
KT_LIB_MODE const KTChar *KTCgnt_GUI_GetSignCertHandleEx(
				const KTChar *pc_objID,				//if NULL, get from config info											  
				const KTChar *pc_addresser,			//发件人
				bool bNotesAbv,
				KT_SESSION_HANDLE hSession,
				KTHandleArray& rHandle);

// 得到用于加密的证书			
KT_LIB_MODE const KTChar *KTCgnt_GUI_GetEnvelopCertHandle(
				const KTChar *pc_addresser,		//发件人
				const KTChar *pc_addressee,		//收件人
				bool bNotesAbv,
				bool bSelfEnvelop,
				KT_SESSION_HANDLE hSession,
				int *pnHandleCount,
				KTHANDLE *phHandles);

// 得到用于加密的证书			
KT_LIB_MODE const KTChar *KTCgnt_GUI_GetEnvelopCertHandleEx(
				const KTChar *pc_objID,			//if NULL, get from config info
				const KTChar *pc_addresser,		//发件人
				const KTChar *pc_addressee,		//收件人
				bool bNotesAbv,
				bool bSelfEnvelop,
				KT_SESSION_HANDLE hSession,
				KTHandleArray& rHandle);

//得到用于加密签名的证书
//pc_addresser:		发件人地址(文件加解密时为"")
//pc_addressee:		收件人地址列表, 以",'或";"分隔(文件加解密时为"")
//bAbbreviation:	是否忽略Notes收件人地址中的@及其以后部分,一般只有在Notes应用中才会忽略
//bSelfEnvelop:		是否用自己的个人证书加密
//hSession:		打开的个人会话,可为空
//pnLen:		传入时是缓冲区长度地址,返回时是证书句柄字符串长度地址(缓冲区不够长时,返回需要的长度)	
//pc_certHanles:	证书句柄字符串			
KT_LIB_MODE const KTChar *KTCgnt_GUI_GetEnvelopSignCertHandle(
				const KTChar *pc_addresser,		//发件人
				const KTChar *pc_addressee,		//收件人
				bool bNotesAbv,
				bool bSelfEnvelop,
				KT_SESSION_HANDLE hSession,
				int *pnEnveHandleCount,
				KTHANDLE *phEnveHandles,
				int *pnSignHandleCount,
				KTHANDLE *phSignHandles);

//得到用于加密签名的证书
//Pc_objID:			发件人收件人地址在证书中对应的ID, 例如ObjID_emailAddress
//pc_addresser:		发件人地址(文件加解密时为"")
//pc_addressee:		收件人地址列表, 以",'或";"分隔(文件加解密时为"")
//bAbbreviation:	是否忽略Notes收件人地址中的@及其以后部分,一般只有在Notes应用中才会忽略
//bSelfEnvelop:		是否用自己的个人证书加密
//hSession:			打开的个人会话,可为空	
//rHandle:			返回的证书句柄列表			
KT_LIB_MODE const KTChar *KTCgnt_GUI_GetEnvelopSignCertHandleEx(
				const KTChar *pc_objID,			//if NULL, get from config info
				const KTChar *pc_addresser,		//发件人
				const KTChar *pc_addressee,		//收件人
				bool bNotesAbv,
				bool bSelfEnvelop,
				KT_SESSION_HANDLE hSession,
				KTHandleArray& hArrEnvCert,
				KTHandleArray& hArrSignCert);

KT_LIB_MODE KTBOOL32 KTCgnt_ShowCertSelectDlg(
				HWND pParent, 
				KTHANDLE *phHandles, 
				UINT *puSize, 
				LPCTSTR pcszCaption);

KT_LIB_MODE KTBOOL32 KTCgnt_ShowCertSelectDlgEx(
				HWND pParent,
				KTHandleArray& arr,
				LPCTSTR pcszCaption);
///////////获取用于签名加密的证书 end

///////////导入证书 begin
KT_LIB_MODE const KTChar *KTCgnt_PutCerts(
				KT_SESSION_HANDLE hSession,
				KT_TOKEN_OBJ_TYPE nType,
				const KTCertificates *pCerts);

KT_LIB_MODE const KTChar *KTCgnt_PutCertsEx(
				KT_SESSION_HANDLE hSession,
				KT_TOKEN_OBJ_TYPE nType,
				const KTCertificates *pCerts,
				KTHANDLE phHandles[],
				KTBOOL pbSuccess[]);

KT_LIB_MODE const KTChar *KTCgnt_PutCRLs(
				KT_SESSION_HANDLE hSession,
				const KTCRLSequence *pCRLs);

KT_LIB_MODE bool KTCgnt_B64CertOrCRLToDER(
				BYTE *pInput,
				UINT unInputLen,
				BYTE *pOutput,
				UINT *punOutputLen);
///////////导入证书 end

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

KT_LIB_MODE KTBOOL32 KTCgnt_OpenFileNormal(
				LPCTSTR szFileNm,
				bool bWait,
				LPCTSTR szCmdLnParam);

KT_LIB_MODE const KTChar *KTCgnt_RunPropSheet(
				KT_SESSION_HANDLE hSession,
				enumPropPage selectPage,
				HWND  pParentWnd,
				KTUINT unPropPages,
				const KTChar *pc_caption);

#ifdef __cplusplus
}
#endif

#endif