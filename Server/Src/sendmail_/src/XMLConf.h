

/** 
 * File: XMLConf.hpp 
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


#ifndef __CPP_XMLCONF__
#define __CPP_XMLCONF__

#include "CommonHelper.h"
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>


//using namespace xercesc;
XERCES_CPP_NAMESPACE_USE

class XMLConf : public CommonHelper
{
    public:
        XMLConf();
        ~XMLConf();

    public:
        virtual bool Load(std::string fname);
        virtual bool Save(std::string fname);
        void Initialize() { XMLPlatformUtils::Initialize(); }
        void Terminate(){ XMLPlatformUtils::Terminate(); }
        DOMDocument* GetDocument();

        //xpath is something like /root/tag1/tag2
        std::string GetTextOf(std::string xpath);
        //get the value of parent's child elements
        std::string GetTextOf(DOMElement * parent, const char* tagName);

        int GetTextsOf(std::string xpath, MyStringList* slv, int nCount = -1);

        int GetTextsOf(DOMElement * parent, const char* tagName, MyStringList* slv, int nCount = -1);

		DOMNodeList * GetNodeList(DOMElement * parent, const char *tagName);
        DOMNodeList * GetNodeList(std::string xpath);
        DOMElement * GetNodeByTag(DOMElement * parent, const char * tabName); 
        
        std::string GetTextOf(DOMNode *pNode);
        //set the value
        void SetValue(std::string xpath, const char *value);
        void SetValue(DOMElement *parent, const char *tagName, const char *value);
        //append child item
        DOMElement *AppendItem(DOMElement *parent, const char *tagName, const char *description, const char *value = "");
        //set element attribute
        void SetElementAtt(DOMElement *e, const char *AttName, const char *AttValue);

    protected:
        int getTextsOf(DOMNodeList *nl, MyStringList* slv, int nCount = -1);

        virtual void Clear();

        XercesDOMParser* m_parser;
        bool splitXPath(std::string xpath, MyStringList* sl);

        ErrorHandler* m_errHandler;
};

#endif



/** $Log: XMLConf.hpp, v1.0 $
 * @ Revision history
 * @ -------------------------------------------------------------------------
 * @ Version      Date              Author               Note
 * @ -------------------------------------------------------------------------
 * @ 1.0          2003-01-10 14:09  linzongfang          创建版本
 * @ 2.0		  2011-09-28 11:32	yeyouxiao			 修改版本
 * @
 */

