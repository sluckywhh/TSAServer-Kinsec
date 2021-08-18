/************************************************************************/
/* 
	@Author			: chenlc
	@Date			: 2006-04-04
	@Copyright		: Kinsec
	@Description	: Downloading PKI objects from remote server	
*/
/************************************************************************/
#ifndef _IUIDOWNLOAD_H_ 
#define _IUIDOWNLOAD_H_ 

#include "PKISDK.hpp"
#include "UIBasic.hpp"

namespace KTUIDownload
{

#ifndef IN
# define IN
#endif

#ifndef OUT
# define OUT
#endif

#ifndef _T
# ifdef _UNICODE
#  define _T(x)		L##x
# else
#  define _T(x)		x
# endif
#endif
/* 
 * Call Type
 * 
 */
#define IUIDLCALL						_stdcall

/*
 * Error definitions
 *
 */
enum enumKTUIDLError{
	KTUIDL_OK		= 0,
	KTUIDL_FAIL		= -1,
	KTUIDL_NOTIMPL	= -2,
	KTUIDL_NOT_SUPPORT = -3,
};

/*
 * Library entry. 
 * Passing LoadLibrary API to get it.
 * 
 */
#define IUIDL_LIB_ENTRY					_T("IUIDLGetObject")

int IUIDLCALL IUIDLGetObject(
		int nCId, 
		int nIId, 
		void* pParam, 
		void** ppObj
		);

typedef int (IUIDLCALL *PIUIDLGetObject)(
		int nCId, 
		int nIId, 
		void* pParam, 
		void** ppObj
		);

/*
 * Usable class and interface identify
 *
 */
enum{
	OBJECT_CUIDownload				= 1,
	OBJECT_CUIConfiguration			= 2,
};
enum{
	INTERFACE_IUIDownload			= 1,
	INTERFACE_IUIConfiguration		= 2,
	INTERFACE_IUIConditionInput		= 3,
};

/*
 * Download Mode
 *
 */
enum{
	DOWNLOAD_MODE_SILENCE				= 1,
	DOWNLOAD_MODE_PROGRESS_BAR			= 2,
	// specified whether download program should import
	// download object to PKI session token.
	// if this flag didn't specified, 
	// we should use ExtensiableCall with CMD_GET_RESULT to
	// receive the temporary data.
	DOWNLOAD_MODE_AUTO_IMPORT			= 4,
	DOWNLOAD_MODE_CAN_CANCEL			= 8,
	DOWNLOAD_MODE_DISPLAY_ERROR			= 16,
	DOWNLOAD_MODE_ALTERNANE				= 32,
};

/*
 * ExtensibleCall Commands
 *
 */
enum{
	/*
	 * pIn : DWORD value. Specify which type of result should be return in pOut.
			if ITEM_TYPE_DWORD specified, pOut is the DWORD for getting the data size.
			if ITEM_TYPE_BINARY specified. pOut is the binary data buffer for receiving data.
			if ITEM_TYPE_STRING_A specified. pOut is the data buffer for receiving zero end string which is temporary file path.
	 * pOut : A buffer or object use to receive the result data.
	 */
	CMD_GET_RESULT,
};
/*
*Keyitem name.
*/
#define KEY_ITEM_DOWNLOAD_MODE	_T( "downloadmode" )
#define KEY_ITEM_HTTP_SERVER	_T( "httpserver" )
#define KEY_ITEM_HTTP_PORT		_T( "httpport" )
#define KEY_ITEM_LDAP_SERVER	_T( "ldapserver" )
#define KEY_ITEM_LDAP_PORT		_T( "ldapport" )
#define KEY_ITEM_USE_PROXY		_T( "useproxy" )
#define KEY_ITEM_PROXY_SERVER	_T( "proxyserver" )
#define KEY_ITEM_PROXY_PORT		_T( "proxyport" )

//
class IUIDownload;
class IUIConditionInput;

class IUIConditionInput{
public:
	virtual int IUIDLCALL SetCondition(const char* pKey, const char* pVal) = 0;
	virtual void IUIDLCALL Empty() = 0;
	virtual int IUIDLCALL RemoveCondition(const char* pKey) = 0;

	virtual int IUIDLCALL QueryInterface(int nIId, void** ppObj) = 0;
	virtual int IUIDLCALL AddRef() = 0;
	virtual int IUIDLCALL Release() = 0;
};

class IUIDownload{
public:
	virtual int IUIDLCALL SetSessionEnvir(KT_SESSION_HANDLE hSession) = 0;
	virtual int IUIDLCALL SetUIEnvir(HWND hParent) = 0;
	virtual int IUIDLCALL SetMode(int nMode) = 0;
	virtual int IUIDLCALL DownloadUserCertificates(KTHandleArray& rHandle) = 0;
	virtual int IUIDLCALL DownloadCRL(KTHandleArray& rHandle) = 0;
	virtual int IUIDLCALL DownloadCA(KTHandleArray& rHandle) = 0;
	virtual int IUIDLCALL DownloadRootCA(KTHandleArray& rHandle) = 0;
	virtual int IUIDLCALL ExtensibleCall(int nCmd, void* pIn, void* pOut) = 0;

	virtual int IUIDLCALL QueryInterface(int nIId, void** ppObj) = 0;
	virtual int IUIDLCALL AddRef() = 0;
	virtual int IUIDLCALL Release() = 0;
};

enum {
	PARAM_CAN_SET_DEFAULT	= 0x1,
	PARAM_READ_ONLY			= 0x2,
};

enum {
	ITEM_TYPE_DWORD			= 0x1,
	ITEM_TYPE_BINARY		= 0x2,
	ITEM_TYPE_STRING_W		= 0x3,
	ITEM_TYPE_STRING_A		= 0x4,
};

class IUIConfiguration
{
public:
	virtual int IUIDLCALL LoadDefaultSetting() = 0;
	virtual int IUIDLCALL DisplayUI(HWND hParent, int nParam) = 0;
	virtual int IUIDLCALL GetItem(LPCTSTR pKey, int *pType, BYTE* pSetting, int *pSize ) = 0;
	virtual int IUIDLCALL SetItem(LPCTSTR pKey, int nType, const BYTE* pSetting, int nSize ) = 0;

	virtual int IUIDLCALL AddRef() = 0;
	virtual int IUIDLCALL Release() = 0;
	virtual int IUIDLCALL QueryInterface(int nIId, void** ppObj) = 0;

};

} // namespace KTUIDownload

#endif

