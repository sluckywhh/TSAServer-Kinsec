

/** 
 * File: CommonHelper.hpp 
 * 
 * Purpose: XML解析器类声明
 * 
 * 
 * @author linzongfang 
 * @version 1.0 2003/01/13  
 * 
 * Copyright (C) 2000, 2003, KOAL, Inc. 
 * 
 */


#ifndef __CPP_COMMONHELPER__
#define __CPP_COMMONHELPER__

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <list>
#include <string>
#include <iostream>
#include <strstream>
#include <vector>
#include <map>

using namespace std;

typedef std::list<std::string> MyStringList;

#define SAFE_DELETE(p) { if (NULL != p) delete p; p = NULL; }
#define SAFE_DELETEA(p) { if (NULL != p) delete[] p; p = NULL; }

class CommonHelper
{
    public:
        CommonHelper();
        ~CommonHelper();

    public:
        std::string GetLastErrorString();
        void SetLastErrorString(const char * str);
        void TRACE(const char * str = NULL);
        void TRACE(std::string str);
        void TRACE(std::string str, MyStringList* sl);
        virtual void Clear();

    private:
        std::string m_lastErrorString;
};

#endif



/** $Log: CommonHelper.hpp, v1.0 $
 * @ Revision history
 * @ -------------------------------------------------------------------------
 * @ Version      Date              Author               Note
 * @ -------------------------------------------------------------------------
 * @ 1.0          2003-01-10 14:09  linzongfang          创建版本
 * @ 2.0		  2012-11-01 09:47  yyx					 修改版本
 * @
 */


