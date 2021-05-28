#include <stdio.h>
#include <string.h>
#include "CommonHelper.h"

CommonHelper::CommonHelper()
{
    m_lastErrorString = "";
}

CommonHelper::~CommonHelper()
{
    Clear();
}

std::string CommonHelper::GetLastErrorString()
{
    return m_lastErrorString;
}

void CommonHelper::SetLastErrorString(const char * str)
{
    m_lastErrorString = str;
    TRACE();
}

void CommonHelper::TRACE(const char * str)
{
    if (NULL == str)
        std::cout << "TRACE: " << m_lastErrorString << std::endl;
    else
        std::cout << "TRACE: " << str << std::endl;
}

void CommonHelper::TRACE(std::string str)
{
    TRACE(str.c_str());
}

void CommonHelper::TRACE(std::string str, MyStringList* sl)
{
    // Iterate through list and output each element.
    int i = 0;
    MyStringList::const_iterator iter;
    for (iter = sl->begin(); iter != sl->end(); iter ++)
    {
        std::strstream ss;
        ss << str << " [" << (i++) << "] " << (*iter) << ends;
        TRACE(ss.str());
    }
}

void CommonHelper::Clear()
{
}




/** $Log: CommonHelper.cpp, v1.0 $
 * @ Revision history
 * @ -------------------------------------------------------------------------
 * @ Version      Date              Author               Note
 * @ -------------------------------------------------------------------------
 * @ 1.0          2003-01-10 14:09  linzongfang          创建版本
 * @ 2.0		  2012-11-01 09:47  yyx					 修改版本
 * @
 */


