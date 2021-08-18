/************************************************************************/
/* 		
*/
/************************************************************************/
#ifndef ADVTRACE_H
#define ADVTRACE_H


#include "tchar.h"
#include "DMTraceDef.h"

namespace maze{

class CWndTrace
{
public:
	CWndTrace( LPCTSTR szWindow );
	~CWndTrace();
public:
#ifndef _ATL_MIN_CRT
	VOID operator()( LPCTSTR szFormat, ... );
#else
	VOID operator()( LPCTSTR szMsg );
#endif // _ATL_MIN_CRT
private:
	BOOL Initialize(void);
	VOID Finalize(void);
	inline BOOL ValidWnd(){
#ifdef ENABLE_TRACE_ANYTIME
		if ( this == NULL ){
			return FALSE;
		}
		if (! IsWindow( _hWnd )){
			_hWnd = FindWindow( NULL, _szWindow );
			if (_hWnd == NULL || _hWnd == INVALID_HANDLE_VALUE){
				_hWnd = FindWindow(NULL, _T("GENERAL"));
			}
		}
		if (! _bInit){
			_bInit = Initialize();
		}
		return _bInit;
#endif
		return (this != NULL) && IsWindow( _hWnd ) && _bInit == TRUE;
	}
	VOID PrintMessage( LPCTSTR szMsg );
public:
	BOOL _bInit;
	HWND _hWnd;

#ifdef ENABLE_TRACE_ANYTIME
	char _szWindow[MAX_PATH];
#endif
};


inline CWndTrace::CWndTrace( LPCTSTR szWindow )
{

#ifdef ENABLE_TRACE_ANYTIME
	strcpy( _szWindow, szWindow );
#endif
	_hWnd	= FindWindow( NULL, szWindow );
	if (_hWnd == NULL || _hWnd == INVALID_HANDLE_VALUE){
		_hWnd = FindWindow(NULL, _T("GENERAL"));
	}
	_bInit	= Initialize();

}

inline CWndTrace::~CWndTrace()
{
	Finalize();
}
#ifndef _ATL_MIN_CRT

inline VOID CWndTrace::operator()( LPCTSTR szFormat, ... )
{
	if ( ! ValidWnd() )
		return;
	va_list va;
	va_start( va, szFormat );
	TCHAR szMsg[ADVTRACE_MAXMSG] = {0};
#ifdef STRSAFE
	StringCchPrintf( szMsg, ADVTRACE_MAXMSG, szFormat, va );
#else
	sprintf( szMsg, szFormat, va );
#endif // STRSAFE
	va_end( va );
	PrintMessage( szMsg );
}
#else

inline VOID CWndTrace::operator()( LPCTSTR szMsg )
{
	if ( ! ValidWnd() )
		return;
	PrintMessage( szMsg );
}
#endif	// _ATL_MIN_CRT

inline BOOL CWndTrace::Initialize(void)
{
	if ( ! IsWindow( _hWnd ) )
		return FALSE;
	return SendMessage( _hWnd, WMTRACE_INITIALIZE, GetCurrentProcessId(), CWNDTRACE_VERSION ) == TRACERET_OK;
}

inline VOID CWndTrace::Finalize(void)
{
	if ( IsWindow( _hWnd ) )
		SendMessage( _hWnd, WMTRACE_FINALIZE, GetCurrentProcessId(), 0 );
}

inline VOID CWndTrace::PrintMessage( LPCTSTR szMsg )
{
	DWORD dwLength = 0;
#ifdef STRSAFE
	if ( S_OK != StringCchLength( szMsg, ADVTRACE_MAXMSG, (size_t*)&dwLength ) )
		return;
#else
	dwLength = _tcslen( szMsg );
#endif
		
	COPYDATASTRUCT data;
	DWORD dwSize = sizeof(TRACEDATA) + sizeof(TCHAR) * dwLength;
	LPBYTE pByte = new BYTE[dwSize];
	LPTRACEDATA pData = ( LPTRACEDATA )pByte;
	pData->dwProcess = GetCurrentProcessId();
	pData->dwThread = GetCurrentThreadId();
	pData->dwSize = dwSize;
#ifdef STRSAFE
	StringCchCopy( pData->szMessage, dwLength+1, szMsg );
#else
	_tcscpy( pData->szMessage, szMsg );
#endif
	data.dwData = WMTRACE_MESSAGE;
	data.cbData = dwSize;
	data.lpData = pData;
	SendMessage( _hWnd, WM_COPYDATA, (WPARAM)_hWnd, (LPARAM)(&data) );
	delete[] pByte;
}

class CInOutTrace
{
public:
	CInOutTrace( CWndTrace *pTrace, LPCTSTR name){
		_outm = "<<< "; _outm += name;
		_inm = ">>> "; _inm += name;
		_pTrace = pTrace;
		(*_pTrace)(_inm.c_str());		
	}
	~CInOutTrace(){
		(*_pTrace)(_outm.c_str());
	}
private:
	std::string _inm;
	std::string _outm;
	CWndTrace* _pTrace;
};


}; // namespace maze

#ifdef ENABLE_REALTIME_TRACE
#  define USES_REALTIME_TRACE(name) namespace maze { static CWndTrace __xxxAdvTrace(#name); }
#  define RTTRACE	maze::__xxxAdvTrace
#  define RTTRACE_BUG RTTRACE
#  define RTTRACE_ERROR RTTRACE
#  ifdef ENABLE_TRACE_INOUT
#    define RTTRACEINOUT(name)	maze::CInOutTrace __iot_x(&g_wndTrace, #name);
#  else
#    define RTTRACEINOUT(name)
#  endif // _USES_FUNCTION_INOUT_TRACE
#else
#  define USES_REALTIME_TRACE(name)
#  define RTTRACE(x)
#  define RTTRACE_BUG RTTRACE
#  define RTTRACE_ERROR RTTRACE
#  define RTTRACEINOUT(name)
#  pragma warning( disable : 4002 )
#endif // _USES_REALTIME_TRACE




//////////////////////////////////////////////////////////////////////////
//
//		可以在相关需要的地方加入以下代码来使用即时跟踪功能
//		宏 : 
//				名称						功能						示例

//			_USES_REALTIME_TRACE		启用调试器				可以在工程设置中进行定义

//			_USES_FUNCTION_INOUT_TRACE	启用函数调用调试器		可以在工程设置中进行定义

//			_RTTRACE_TRACE_ANYTIME		允许在任何时候输出信息	可以在工程设置中进行定义

//			RTTRACE(x,...)				输出跟踪信息			RTTRACE("Hello World! Process = 0x%xH", GetCurrentProcessId());

//			RTTRACEINOUT(in,out)		自动输出函数调用状态	RTTRACEINOUT("Function A In","Function A Out");

//			
//		重要说明 :
		
//			当使用下面的定义时，要注意修改%TRACE_WND_NAME%为程序所需要的名称。
//			这个名称应该与调试输出程序一致才能收到跟踪信息。
//

#endif // ! ADVTRACE_H