// =================================================================================
//  File    : XmlDocument.h
//  Purpose : XML文件操作类
//  Author  : 林灶钢
//  Created : 2006-09-21
//  CopyRight (C) Fujian Kinsec
// =================================================================================

#ifndef __XMLDOCUMENT_H__
#define __XMLDOCUMENT_H__

// =================================================================================
//  Include Header Files
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <stdio.h>
#include <string>
#include <vector>

XERCES_CPP_NAMESPACE_USE

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  ifdef _DEBUG
#    pragma comment( lib, "xerces-c_2D.lib" )
#    pragma message( "Automatically linking with xerces-c_2D.lib" )
#  else
#    pragma comment( lib, "xerces-c_2.lib" )
#    pragma message( "Automatically linking with xerces-c_2.lib" )
#  endif
#  define XML_ENCODING "gb2312"
#  define XML_HEADER "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
#elif defined(_LINUX)
//#  define XML_ENCODING "gb18030"  
//#  define XML_HEADER "<?xml version=\"1.0\" encoding=\"gb18030\"?>\r\n"
#  define XML_ENCODING "GB2312"  
#  define XML_HEADER "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
#endif

// =================================================================================
//  Class Definition
class CXmlDocument
{
public :
	// = Default constructor.
	CXmlDocument();

	// = Default desctructor.
	~CXmlDocument();

	// = Load a xml file.
	int Load( const char * szFileName );

	// = Save to a xml file.
	int Save( const char * szFileName = NULL );

	// = Get root element.
	DOMElement * GetRootElement();

	// = Get parent element.
	DOMElement * GetParentElement( DOMElement * xmlElem );

	// = Get first child.
	DOMElement * GetFirstChild( DOMElement * xmlElem, const char * szName = "*" );

	// = Get child list.
	DOMNodeList * GetChildren( DOMElement * xmlElem, const char * szName = "*" );

	// = Append a child.
	DOMElement * AppendChild( DOMElement * xmlParent, const char * szName );

	// = Remove a element.
	void RemoveElement( DOMElement * xmlElem );

	// = Remove children.
	void RemoveChildren( DOMElement * xmlParent );

	// = Get element name.
	std::string GetElementName( DOMElement * xmlElem );
	void GetElementName( DOMElement * xmlElem, std::string & name );

	// = Get string content.
	std::string GetElementContent( DOMElement * xmlElem );
	void GetElementContent( DOMElement * xmlElem, std::string & content );

	// = Set string content.
	void SetElementContent( DOMElement * xmlElem, const char * szContent );

	// = Get attribute.
	std::string GetElementAttribute( DOMElement * xmlElem, const char * szAttrName );
	void GetElementAttribute( DOMElement * xmlElem, const char * szAttrName, 
		std::string & attrValue );

	// = Set attribute.
	void SetElementAttribute( DOMElement * xmlElem, const char * szAttrName, 
		const char * szAttrValue );

private :
	// = File name.
	char file_[512];

	// = XML parser.
	XercesDOMParser * parser_;
};

// =================================================================================
//  1.00    2006-09-21    Created By LinZaoGang
//  File End 
#endif  // __XMLDOCUMENT_H__  
