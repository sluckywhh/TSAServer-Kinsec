#ifndef __KT_LOG_H__
#define __KT_LOG_H__

/**
  *@Name: KTLog.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/02 20:46
  *@Description: logger header file 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KTType.h"
#include "KTSTL.h"
#include "KTMacro.h"
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

/*
#include "KTUnknow.h"
//define ILoger interface.
class KT_LIB_MODE ILog : public IKTUnknow{
public:
	virtual void _cdecl write(int nLogLevel,KTLOG_TYPE nLogType,const KTCHAR* pDesc,KTULONG lParam=0)=0;
	virtual void _cdecl write(int nLogLevel,KTLOG_TYPE nLogType,const KTCHAR* pSource,const KTCHAR* pDesc,KTULONG lParam=0)=0;
	virtual void flush()=0;
};
*/

#ifdef _WIN32
    #include "windows.h"
#endif

#ifndef _WIN32
    #define _cdecl
#endif

#define KT_DECALRE_LOG(source,fname,FilterLogType,Level)
#define KT_DECALRE_USE_LOG(source,fname,FilterLogType,Level)
#define KT_USE_LOG(source)
#define KT_TRACE
/*//define event type.
enum KTLOG_TYPE{
	KTLOG_ERROR_TYPE			=0x0001,		//error event
	KTLOG_WARNING_TYPE			=0x0002,		//warning event
	KTLOG_INFORMATION_TYPE		=0x0004,		//information event
	KTLOG_AUDIT_SUCCESS			=0x0008,		//success audit event
	KTLOG_AUDIT_FAILURE			=0x0010,		//failure audit event
	KTLOG_AUDIT_REFUSE			=0x0020,		//failure audit event
	KTLOG_DEBUG					=0x8000,		//for debug
    KTLOG_ALL_TYPE              =0xFFFF,        //
};

const KTUINT KT_LOGGER_LEVEL			=0x0001;
const KTUINT KT_INFORMATON_LEVEL	=0x0003;	
const KTUINT KT_WARNING_LEVEL		=0x0005;
const KTUINT KT_ERROR_LEVEL			=0x0007;
const KTUINT KT_AUDIT_LEVEL			=0x0009;

#if defined(_RDEBUG) || defined(_DEBUG)
	//define DebugLog class.
	#define KT_DECALRE_LOG(source,fname,FilterLogType,Level) \
		CDebugLog source(fname,FilterLogType,Level);

	#define KT_DECALRE_USE_LOG(source,fname,FilterLogType,Level) \
		CDebugLog source(fname,FilterLogType,Level);	\
		static CDebugLog* g_pLoger=&source;

	#define KT_USE_LOG(source) \
		extern CDebugLog source;	\
		static CDebugLog* g_pLoger=&source;

	#define KT_TRACE			g_pLoger->put
		
#else
	#define KT_DECALRE_LOG(source,fname,FilterLogType,Level)
	#define KT_DECALRE_USE_LOG(source,fname,FilterLogType,Level)
	#define KT_USE_LOG(source)
	#define KT_TRACE
#endif //_DEBUG

#if defined(_RDEBUG) || defined(_DEBUG)

class CDebugLog{ //: public ILog{
public:
	CDebugLog(KTCHAR *fname,int nFilterType,int nLevel);
	~CDebugLog();
	virtual void _cdecl write(int nLogLevel,KTLOG_TYPE nLogType,const KTCHAR* pDesc,KTULONG lParam=0);
	virtual void flush(){}

	void _cdecl put(const KTCHAR *format,...);
	void _cdecl put(KTLOG_TYPE nLogType,const KTCHAR *format,...);
	void _cdecl put(int nLogLevel,KTLOG_TYPE nLogType,const KTCHAR *format,...);
	void _cdecl put(const KTByteArray& v);

protected:
	KTOutFileStream m_os;
	int m_nFilterType;
	int	m_nLevel;
    enum {
        MAX_BUFFER_LEN = 10240,
    };
};

inline CDebugLog::CDebugLog(KTCHAR *fname,int nFilterType,int nLevel)
{
	int			w;
	KTCHAR		f;
    time_t      timeNow=time(NULL);
    tm*         pTmNow= localtime(&timeNow);    

    m_os.open(fname, std::ios::out|std::ios::app);
	m_nFilterType=nFilterType;
	m_nLevel=nLevel;
	w = m_os.width();
	f = m_os.fill();
	m_os<<std::endl<<_KT("Log begin at ")<<pTmNow->tm_year+1900<<'/'<<std::setw(2)<<std::setfill('0')
        <<pTmNow->tm_mon+1<<'/'<<pTmNow->tm_mday
		<<' '<<pTmNow->tm_hour<<':'<<pTmNow->tm_min<<':'<<pTmNow->tm_sec<<std::setw(w)<<std::setfill(f)<<std::endl;
	m_os.flush();
}
inline CDebugLog::~CDebugLog()
{
	m_os.close();
}
inline void _cdecl CDebugLog::put(const KTCHAR *format,...)
{
	char buf[MAX_BUFFER_LEN];
	va_list va;
	va_start(va,format);
	vsprintf(buf,format,va);
	va_end(va);
	write(KT_INFORMATON_LEVEL,KTLOG_INFORMATION_TYPE,buf);
}
inline void _cdecl CDebugLog::put(KTLOG_TYPE nLogType,const KTCHAR *format,...)
{
	char buf[MAX_BUFFER_LEN];
	va_list va;
	va_start(va,format);
	vsprintf(buf,format,va);
	va_end(va);
	write(KT_INFORMATON_LEVEL,nLogType,buf);
}
inline void _cdecl CDebugLog::put(int nLogLevel,KTLOG_TYPE nLogType,const KTCHAR *format,...)
{
	char buf[MAX_BUFFER_LEN];
	va_list va;
	va_start(va,format);
	vsprintf(buf,format,va);
	va_end(va);
	write(nLogLevel,nLogType,buf);
}

inline void _cdecl CDebugLog::write(int nLogLevel,KTLOG_TYPE nLogType,const KTCHAR* pDesc,KTULONG lParam)
{
	if(nLogLevel>m_nLevel)
	{
		if(nLogType & m_nFilterType)
		{
			switch(nLogType)
			{
				case KTLOG_ERROR_TYPE:
					m_os<<"[ERROR]:";
#ifdef THIS_FILE
//					m_os<<"["<<THIS_FILE<<"]===>";
#endif //THIS_FILE
					break;
				case KTLOG_WARNING_TYPE:
					m_os<<"[WARNING]:";
					break;
				case KTLOG_INFORMATION_TYPE:
					m_os<<"[INFORMATION]:";
#ifdef THIS_FILE
//					m_os<<"["<<THIS_FILE<<"]===>";
#endif //THIS_FILE
					break;
				case KTLOG_AUDIT_SUCCESS:
					m_os<<"[SUCCESS]:";
					break;
				case KTLOG_AUDIT_FAILURE:
					m_os<<"[FAILURE]:";
					break;
				default:
					m_os<<"[DEBUG]:";
#ifdef THIS_FILE
//					m_os<<"["<<THIS_FILE<<"]===>";
#endif //THIS_FILE
					
					break;

			}
			m_os<<pDesc<<std::endl;
			m_os.flush();
		}
	}
}

inline void _cdecl CDebugLog::put(const KTByteArray& v)
{
	for(KTByteArray::size_type i=0;i<v.size();i++)
	{
		m_os<<std::setfill('0')<<std::setw(2)<<std::hex<<(int)v[i]<<" ";
	}
	m_os<<std::endl;
	m_os.flush();
}
#endif //_DEBUG
*/
#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/03 15:44  Hongwei Yu   created 
///   1.0.0.1  2003/05/12 23:06  Hongwei Yu   remove KT_LOGGER macro

#endif
