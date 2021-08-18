#ifndef _KT_CONFIG_API_H_
#define _KT_CONFIG_API_H_

#ifdef __cplusplus

class KT_LIB_MODE CUICfgLog
{

private:
	static FILE * w_pf;

public:
	static bool open( const KTChar* szFileNameTag, KTString& strLogFileNm );
	static bool lineFeedWrite( const KTChar* szLog );
	static bool write( const KTChar* szFormat, const KTChar* szLog );
	static bool write( const KTChar* szLog );

	~CUICfgLog();
};

//#if defined(_DLL)	  
KT_LIB_MODE const KTChar* KTPKI_UI_SetErr(const KTChar pc_format[], ...);
KT_LIB_MODE const KTChar* KTPKI_UI_SetErr(int nErr, const KTChar pc_format[], ...);
//#endif

KT_LIB_MODE const KTChar* KTPKI_UI_SetErr2(int nErr, const KTChar pc_err[]);
KT_LIB_MODE int KTPKI_UI_GetLastErrNum();
KT_LIB_MODE const KTChar* KTPKI_UI_GetLastErrInfo();

KT_LIB_MODE void UICfg_GetCertParam( DWORD& dwMatchCertTag,
							  DWORD& dwEnveCertTag,
							  DWORD& dwSignCertTag );

KT_LIB_MODE void UICfg_SetCertParam( DWORD dwMatchCertTag,
							  DWORD dwEnveCertTag,
							  DWORD dwSignCertTag );

KT_LIB_MODE void UICfg_GetShowInfoParam( DWORD& dwShowSignInfo,
                          DWORD& dwShowEnveInfo );

KT_LIB_MODE void UICfg_SetShowInfoParam( DWORD dwShowSignInfo,
                          DWORD dwShowEnveInfo );

KT_LIB_MODE void UICfg_GetCertPublishServer( KTString& strHostNm );
KT_LIB_MODE void UICfg_SetCertPublishServer( const KTChar* pcHostNm );

//1, 2, 3
KT_LIB_MODE bool UICfg_GetCertPublishVirtualCA(DWORD& dwPublishVirtualCA);
KT_LIB_MODE void UICfg_SetCertPublishVirtualCA(DWORD dwPublishVirtualCA);


//0, Ldap(default); 1, http
KT_LIB_MODE DWORD UICfg_GetCertPublishProtocol();
KT_LIB_MODE void UICfg_SetCertPublishProtocol(DWORD dwPublishProtocol);



KT_LIB_MODE void UICfg_GetLdapParam( KTString& strHostNm,
						KTString& strSearchBase,
						DWORD& dwPort,
						DWORD& dwVersion );

KT_LIB_MODE void UICfg_GetLdapUserNmPwd( KTString& strUserNm,
						   KTString& strPwd );

KT_LIB_MODE void UICfg_SetLdapParam( const KTChar* strHostNm,
						const KTChar* strSearchBase,
						DWORD dwPort,
						DWORD dwVersion );

KT_LIB_MODE const KTChar* UICfg_GetLdapUserCertArributeName();
KT_LIB_MODE const KTChar* UICfg_GetLdapCACertArributeName();
KT_LIB_MODE const KTChar* UICfg_GetLdapCRLArributeName();

KT_LIB_MODE const KTChar* UICfg_GetLdapUserCertObjectClassFilter();
KT_LIB_MODE const KTChar* UICfg_GetLdapCACertObjectClassFilter();
KT_LIB_MODE const KTChar* UICfg_GetLdapCRLObjectClassFilter();

////0 or default, no code; 1, UTF8, 2, B64
KT_LIB_MODE DWORD UICfg_GetLdapCertCRLAttributeType();



////0 or default, no code; 1, UTF8, use only for email value
KT_LIB_MODE DWORD UICfg_GetLdapCodeRule();

KT_LIB_MODE void UICfg_SetLdapUserNmPwd( const KTChar* strUserNm,
						   const KTChar* strPwd );

KT_LIB_MODE void UICfg_GetOcspParam( KTString& strOcspURL,
							  KTString& strOcspAgent,
							  KTString& strUserNm,
							  KTString& strUserPwd,
							  KTString& strOcspProxy,
							  KTString& strProxyByPass,
							  DWORD& dwOCSPPort,
							  DWORD& dwAccessType );

KT_LIB_MODE void UICfg_SetOcspParam( const KTChar* strOcspURL,
							  const KTChar* strOcspAgent,
							  const KTChar* strUserNm,
							  const KTChar* strUserPwd,
							  const KTChar* strOcspProxy,
							  DWORD dwOCSPPort,
							  DWORD dwAccessType );

KT_LIB_MODE void UICfg_GetOcspServer( KTString& strOcspURL);
KT_LIB_MODE void UICfg_SetOcspServer( const KTCHAR* strOcspURL);

//dwCheckCertTag, 0, CRL(default), 1, ocsp
KT_LIB_MODE void UICfg_GetMixedParam( DWORD& dwIsSelMailClt,
							  DWORD& dwCheckCertTag,
							  DWORD& dwMailPEnveSignTag );

KT_LIB_MODE void UICfg_SetMixedParam( DWORD dwIsSelMailClt,
							  DWORD dwCheckCertTag,
							  DWORD dwMailPEnveSignTag );


KT_LIB_MODE void UICfg_GetLogParam( DWORD& dwEnableLog,  
							 KTString& strLogPath, //no end with '\\'
							  DWORD& dwLogMonth );
KT_LIB_MODE void UICfg_SetLogParam( DWORD dwEnableLog,  
							 const KTChar* strLogPath, 
							  DWORD dwLogMonth ); 

KT_LIB_MODE void UICfg_GetLoginResetTime( DWORD& dwResetTime );
KT_LIB_MODE int UICfg_SetLoginResetTime( DWORD dwResetTime );
KT_LIB_MODE void UICfg_GetLoginTime( DWORD& dwLoginTime );
KT_LIB_MODE int UICfg_SetLoginTime( DWORD dwLoginTime );

KT_LIB_MODE void UICfg_GetMsgTitle(KTString& strMsgTitle);
KT_LIB_MODE void UICfg_SetMsgTitle(const KTChar* szMsgTitle);

KT_LIB_MODE bool UICfg_GetInstallPath( KTString& strInstPath ); //end with '\\'

KT_LIB_MODE bool UICfg_IsUseBakDisk();

KT_LIB_MODE DWORD UICfg_GetCRLNextUpdateTime();
KT_LIB_MODE void UICfg_SetCRLNextUpdateTime(DWORD dwNextUpdateTime);

KT_LIB_MODE bool UICfg_GetBakCertName( KTString& strCertName );

//#if defined(_DLL)
KT_LIB_MODE bool UICfg_GetMailCltNames(KTStringArray& strArrMailCltNames);
//#endif
KT_LIB_MODE void UICfg_GetDefaultMailCltName(KTString& strMailCltName);
KT_LIB_MODE void UICfg_SetDefaultMailCltName(const KTChar* strMailCltName);
KT_LIB_MODE DWORD UICfg_GetIsNeedSelMailClt();
KT_LIB_MODE void UICfg_SetIsNeedSelMailClt(DWORD dwIsNeedSelMailClt);
KT_LIB_MODE bool UICfg_GetMailToCmdLn(const KTChar* strMailCltName, KTString& strMaiToCmdLn);

KT_LIB_MODE DWORD UICfg_GetGenRSACert();

KT_LIB_MODE DWORD UICfg_GetLogoUIid();
KT_LIB_MODE void UICfg_GetValidSlot(KTBYTE* pbValidSlot, DWORD& dwSize);
KT_LIB_MODE void UICfg_SetValidSlot(KTBYTE* pbValidSlot, DWORD dwSize);

KT_LIB_MODE const KTChar* UICfg_GetCertNameID();
KT_LIB_MODE const KTChar* UICfg_GetCertSendeeID();

KT_LIB_MODE void UICfg_GetShellMenuText(KTString& strMenuText);
//#if defined(_DLL)
KT_LIB_MODE void UICfg_GetShellMenuItem(KTIntArray& rMenuItemID, KTStringArray&  rMenuItemText );
//#endif

KT_LIB_MODE bool UICfg_GetDefaultLoginSlot(DWORD* pdwSlotType);
KT_LIB_MODE void UICfg_SetDefaultLoginSlot(DWORD dwSlotType);

KT_LIB_MODE bool UICfg_GetLoginPfxName(KTChar pc_pfxName[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetLoginPfxName(const KTChar pc_pfxName[]);

KT_LIB_MODE bool UICfg_GetLoginUserName(KTChar pc_userName[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetLoginUserName(const KTChar pc_userName[]);

KT_LIB_MODE bool UICfg_GetUpdtSvrURL(KTChar pc_updtSvrURL[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetUpdtSvrURL(const KTChar pc_updtSvrURL[]);

KT_LIB_MODE bool UICfg_GetUpdtUserId(KTUINT32* pnUserId);
KT_LIB_MODE void UICfg_SetUpdtUserId(DWORD dwUserId);

KT_LIB_MODE bool UICfg_GetUpdtAdminId(KTUINT32* pnAdminId);
KT_LIB_MODE void UICfg_SetUpdtAdminId(DWORD dwAdminId);

//default 2
KT_LIB_MODE DWORD UICfg_GetUpdtOption();
KT_LIB_MODE void UICfg_SetUpdtOption(DWORD dwUpdtOption);

//default 5
KT_LIB_MODE DWORD UICfg_GetUpdtThread();
KT_LIB_MODE void UICfg_SetUpdtThread(DWORD dwUpdtThread);

//default 3600 second
KT_LIB_MODE DWORD UICfg_GetUpdtInterval();
KT_LIB_MODE void UICfg_SetUpdtInterval(DWORD dwUpdtInterval);

//\HKEY_CURRENT_USER\SoftWare\KinSec\Products\*
KT_LIB_MODE bool UICfg_GetSofts(KTStringArray& strArrSoft);
KT_LIB_MODE void UICfg_DelSoft(const char* pc_SoftNm);

//default 0
KT_LIB_MODE DWORD UICfg_GetSoftCreatedTime(const char* pc_SoftNm, bool* pbError=NULL);
KT_LIB_MODE void UICfg_SetSoftCreatedTime(const char* pc_SoftNm, DWORD dwCreatedTime);

//default 1000, \HKEY_CURRENT_USER\SoftWare\KinSec\Products\pc_softNm:version
KT_LIB_MODE DWORD UICfg_GetSoftVersion(const char* pc_softNm, bool* pbError=NULL);
KT_LIB_MODE void UICfg_SetSoftVersion(const char* pc_softNm, DWORD dwVersion);

//\HKEY_CURRENT_USER\SoftWare\KinSec\Products\pc_softNm:setupDIR
KT_LIB_MODE bool UICfg_GetSoftSetupDir(const char* pc_softNm, char pc_setupDIR[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetSoftSetupDir(const char* pc_softNm, const char pc_setupDIR[]);

//default 0xFFFFFFFF
KT_LIB_MODE DWORD UICfg_GetProductThreadOffset(const char* pc_softNm, const char* pc_productNm, int nThreadIndex);
KT_LIB_MODE void  UICfg_SetProductThreadOffset(const char* pc_softNm, const char* pc_productNm, int nThreadIndex, DWORD dwOffset);

//default 5, last error thread
KT_LIB_MODE DWORD  UICfg_GetProductThread(const char* pc_softNm, const char* pc_productNm);
KT_LIB_MODE void  UICfg_SetProductThread(const char* pc_softNm, const char* pc_productNm, DWORD dwThread);

//default 0
KT_LIB_MODE DWORD  UICfg_GetProductDownSpeed(const char* pc_softNm, const char* pc_productNm, bool bDelProductKey=false);
KT_LIB_MODE void  UICfg_SetProductDownSpeed(const char* pc_softNm, const char* pc_productNm, DWORD dwSpeed);

//last error product thread
KT_LIB_MODE bool  UICfg_ExistProductThreadInfo(const char* pc_softNm, const char* pc_productNm);
KT_LIB_MODE void  UICfg_RemoveProductThreadInfo(const char* pc_softNm, const char* pc_productNm);

#ifdef _WIN32
KT_LIB_MODE LONG UICfg_GetRegValue( HKEY hOpenKey,
		const KTChar* szKeyNm, const KTChar* szRegNm,
		DWORD* pdwType, void* pbData, DWORD* pdwSize );

KT_LIB_MODE LONG UICfg_GetSZRegValue( HKEY hOpenKey,
		const KTChar* szKeyNm, const KTChar* szRegNm,
		char pc_value[KT_MAX_PATH], const char* pc_defaultValue );

KT_LIB_MODE LONG UICfg_GetDWRegValue( HKEY hOpenKey,
		const KTChar* szKeyNm, const KTChar* szRegNm,
		DWORD* pdwValue, DWORD dwDefautValue );

KT_LIB_MODE LONG UICfg_SetRegValue( HKEY hOpenKey,
		const KTChar* szKeyNm, const KTChar* szRegNm, 
		DWORD dwType, const void* pbData, DWORD dwSize );

KT_LIB_MODE LONG UICfg_DelRegValue( HKEY hOpenKey,
		const KTChar* szKeyNm, const KTChar* szRegNm ); 
#else
LONG UICfg_GetSZRegValue(const KTChar *szRegNm, char *pszData);
LONG UICfg_GetDWRegValue(const KTChar *szRegNm, DWORD *pdwData);
LONG UICfg_SetSZRegValue(const KTChar *szRegNm, const char *pszData);
LONG UICfg_SetDWRegValue(const KTChar *szRegNm, DWORD dwData);
LONG UICfg_DelRegValue(const KTChar *szRegNm);

LONG UICfg_GetSZRegValue_Products(const KTChar *szProductNm, const KTChar *szRegNm, char *pszData);
LONG UICfg_GetDWRegValue_Products(const KTChar *szProductNm, const KTChar *szRegNm, DWORD *pdwData);
LONG UICfg_SetSZRegValue_Products(const KTChar *szProductNm, const KTChar *szRegNm, const char *pszData);
LONG UICfg_SetDWRegValue_Products(const KTChar *szProductNm, const KTChar *szRegNm, DWORD dwData);
LONG UICfg_DelRegValue_Products(const KTChar *szProductNm);
LONG UICfg_GetProducts(KTStringArray& strArrProduct);
#endif

#define CHECK_OPT_INGORE 1
#define CHECK_OPT_RETRY 2
#define CHECK_OPT_NEW 3
#define CHECK_OPT_FAILED 4

KT_LIB_MODE BOOL UICfg_IsShowVerifyOptIfNoCRL();
KT_LIB_MODE int UICfg_GetVerifyOptIfNoCRL();
KT_LIB_MODE void UICfg_SetShowVerifyOptInNoCRL(BOOL bShow);
KT_LIB_MODE void UICfg_SetVerifyOptIfNoCRL(int iOpt);
KT_LIB_MODE BOOL UICfg_GetEnableCRL();
KT_LIB_MODE void UICfg_SetEnableCRL( BOOL bEnable );
KT_LIB_MODE BOOL UICfg_GetRunCrlUpdate();
KT_LIB_MODE void UICfg_SetRunCrlUpdate( BOOL bRun );
KT_LIB_MODE BOOL UICfg_IsDownloadingCRL();
KT_LIB_MODE DWORD UICfg_EnterDownloadCRL();
KT_LIB_MODE void UICfg_LevelDownloadCRL(DWORD dwCookie);

KT_LIB_MODE BOOL UICfg_IsPersonalCertManageVisible();
KT_LIB_MODE BOOL UICfg_VisiblePersonalCertManage( BOOL bVisible );

//SecSeal Switch tag.
KT_LIB_MODE BOOL UICfg_IsSecSealManageVisible();
KT_LIB_MODE BOOL UICfg_VisibleSecSealManage( BOOL bVisible );

KT_LIB_MODE BOOL UICfg_IsConfigureExVisible();
KT_LIB_MODE BOOL UICfg_VisibleConfigureEx( BOOL bVisible );

KT_LIB_MODE DWORD UICfg_GetTssServerProvider();
KT_LIB_MODE void UICfg_SetTssServerProvider(DWORD dwValue);

KT_LIB_MODE bool UICfg_GetTssServer(KTChar pc_str[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetTssServer(const KTChar pc_str[]);

KT_LIB_MODE bool UICfg_GetTssPort(KTChar pc_str[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetTssPort(const KTChar pc_str[]);

KT_LIB_MODE bool UICfg_GetTssApplicationName(KTChar pc_str[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetTssApplicationName(const KTChar pc_str[]);

KT_LIB_MODE BOOL UICfg_GetTssStartup();
KT_LIB_MODE BOOL UICfg_SetTssStartup(BOOL bState);

KT_LIB_MODE bool UICfg_GetTssCertIssuer(KTChar pc_str[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetTssCertIssuer(const KTChar pc_str[]);

KT_LIB_MODE bool UICfg_GetTssCertKeyWord(KTChar pc_str[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetTssCertKeyWord(const KTChar pc_str[]);

KT_LIB_MODE BOOL UICfg_IsCrlManageVisible();
KT_LIB_MODE BOOL UICfg_VisibleCrlManage( BOOL bVisible );

KT_LIB_MODE BOOL UICfg_IsTssManageVisible();
KT_LIB_MODE BOOL UICfg_VisibleTssManage( BOOL bVisible );

KT_LIB_MODE BOOL UICfg_IsSecondaryCertManageVisible();
KT_LIB_MODE BOOL UICfg_VisibleSecondaryCertManage( BOOL bVisible );

KT_LIB_MODE BOOL UICfg_IsRootCertManageVisible();
KT_LIB_MODE BOOL UICfg_VisibleRootCertManage( BOOL bVisible );

KT_LIB_MODE BOOL UICfg_IsPasswordManageVisible();
KT_LIB_MODE BOOL UICfg_VisiblePasswordManage( BOOL bVisible );

KT_LIB_MODE BOOL UICfg_IsSysConfiguationManageVisible();
KT_LIB_MODE BOOL UICfg_VisibleSysConfiguationManage( BOOL bVisible );

KT_LIB_MODE BOOL UICfg_DefaultLoginReset();

KT_LIB_MODE BOOL UICfg_GetStartupSimpleMode();
KT_LIB_MODE BOOL UICfg_SetStartupSimpleMode(BOOL bState);

KT_LIB_MODE BOOL UICfg_GetUseProxy();
KT_LIB_MODE BOOL UICfg_SetUseProxy( BOOL bUse );

KT_LIB_MODE bool UICfg_GetProxyServer(KTChar pc_str[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetProxyServer(const KTChar pc_str[]);

KT_LIB_MODE bool UICfg_GetProxyPort(KTChar pc_str[KT_MAX_PATH]);
KT_LIB_MODE void UICfg_SetProxyPort(const KTChar pc_str[]);

KT_LIB_MODE BOOL UICfg_GetShowTimeLeft();
KT_LIB_MODE int UICfg_SetShowTimeLeft(BOOL bShow);

KT_LIB_MODE BOOL UICfg_GetRunCertProxyUpdate();
KT_LIB_MODE int UICfg_SetRunCertProxyUpdate(BOOL bRunCertProxyUpdate);

KT_LIB_MODE void UICfg_GetDaysToPromptBeforeExpired( DWORD& dwDaysToPromptBeforeExpired );
KT_LIB_MODE int UICfg_SetDaysToPromptBeforeExpired( DWORD dwDaysToPromptBeforeExpired );

KT_LIB_MODE BOOL UICfg_GetShowProcessDlg();
KT_LIB_MODE int UICfg_SetShowProcessDlg(BOOL bShow);

#define UICfg_DelReg(hOpenKey, szKeyNm) RegDeleteKey(hOpenKey, szKeyNm)

//constant variable define
extern const char gpc_REGK_PKICLT_CURVER[];

extern const char gpc_REGS_PKICLT_UpCertURL[];
extern const char gpc_REGS_PKICLT_UpCertEmailURL[];

extern const char gpc_DEFAULT_UpCertURL[];
extern const char gpc_DEFAULT_UpCertEmailURL[];

KT_LIB_MODE void UICfg_GetDeviceIndex( DWORD& dwDeviceIndex );
KT_LIB_MODE int UICfg_SetDeviceIndex( DWORD dwDeviceIndex );
KT_LIB_MODE BOOL UICfg_IsShowTwn();
KT_LIB_MODE int UICfg_SetShowTwn(BOOL bShow);
KT_LIB_MODE void UICfg_GetSymmetricAlgorithm( DWORD& dwSymmetricAlgorithm );
KT_LIB_MODE int UICfg_SetSymmetricAlgorithm( DWORD dwSymmetricAlgorithm );

KT_LIB_MODE void UICfg_GetLoginKeepTime( DWORD& dwLoginKeepTime );
KT_LIB_MODE int UICfg_SetLoginKeepTime( DWORD dwLoginKeepTime );

#endif

#endif



