#ifndef __KT_EXCEPTION_H__
#define __KT_EXCEPTION_H__

/**
  *@Name: KTException.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/04 16:35
  *@Description: all error number 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include <stdio.h>
#include <stdarg.h>
#include "./KTLog.h"
#include "./KTType.h"
#include "./KTSTL.h"
#include "./KTErrNum.h"

class CKTEXCEPTION
{
public:
	CKTEXCEPTION(const KTCHAR* sErr, KTINT nErr = KT_ERR_UNKNOWN)
	{
   		m_strMessage = sErr;
    	m_nErrId = nErr;
  //  	KT_TRACE(m_strMessage.c_str());
	};
	CKTEXCEPTION(CKTEXCEPTION* e, const KTCHAR* sErr, KTINT nErr = KT_ERR_UNKNOWN)
	{
    	KTString s = sErr;
    	m_strMessage = s + "\n\t" + e->toString();
    	m_nErrId = nErr;
    //	KT_TRACE(m_strMessage.c_str());
	};
	CKTEXCEPTION(KTINT nErr,const KTCHAR* sFormat,...)
	{
		char buf[1024];
		va_list va;
		va_start(va,sFormat);
		vsprintf(buf,sFormat,va);
		va_end(va);
		m_strMessage = buf;
		m_nErrId = nErr;
	}
	//]
    virtual ~CKTEXCEPTION()
	{
	};

    inline void release();
    inline KTINT  getErrId();
    inline KTString getMessage();
    inline KTString toString();

public:
    inline void prompt();

protected:
    KTINT m_nErrId;
    KTString m_strMessage;
};

inline KTINT CKTEXCEPTION::getErrId()
{ 
	return m_nErrId; 
}

inline KTString CKTEXCEPTION::getMessage()
{ 
	return m_strMessage; 
}

inline void CKTEXCEPTION::prompt()
{
    //#ifdef KT_UNIX
    std::cout << (this->getMessage()).c_str() << std::endl;
    //#else
    //    MessageBox(NULL, this->GetMessage(), "REP", MB_OK);
    //#endif
}

inline KTString CKTEXCEPTION::toString()
{
    KTStrStream s;
    s << "´íÎó´úÂë: " << getErrId() << "\n´íÎóÐÅÏ¢: " << getMessage().c_str() << std::ends;
    KTString r; 
    r = s.str();
    return r;
}

inline void CKTEXCEPTION::release()
{
	delete this;
}

typedef CKTEXCEPTION KTEXCEPTION;

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/*
/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/03 16:36  Hongwei Yu   created 
**/

#endif
