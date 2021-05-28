#ifndef __XMLDOCUMENT_H__
#define __XMLDOCUMENT_H__

#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <string>
#include <vector>

XERCES_CPP_NAMESPACE_USE

#if defined(_WIN32)
#  ifdef _DEBUG
#  define XML_ENCODING "gb2312"
#  define XML_HEADER "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
#elif defined(_LINUX)
#  define XML_ENCODING "GB2312"  
#  define XML_HEADER "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
#endif
#endif

class CXmlDocument
{
public :
	CXmlDocument();
	~CXmlDocument();
	int Load( const char * szFileName );
	int Save( const char * szFileName = NULL );
	DOMElement * GetRootElement();
	DOMElement * GetParentElement( DOMElement * xmlElem );
	DOMElement * GetFirstChild( DOMElement * xmlElem, const char * szName = "*" );
	DOMNodeList * GetChildren( DOMElement * xmlElem, const char * szName = "*" );
	DOMElement * AppendChild( DOMElement * xmlParent, const char * szName );
	void RemoveElement( DOMElement * xmlElem );
	void RemoveChildren( DOMElement * xmlParent );
	std::string GetElementName( DOMElement * xmlElem );
	void GetElementName( DOMElement * xmlElem, std::string & name );
	std::string GetElementContent( DOMElement * xmlElem );
	void GetElementContent( DOMElement * xmlElem, std::string & content );
	void SetElementContent( DOMElement * xmlElem, const char * szContent );
	std::string GetElementAttribute( DOMElement * xmlElem, const char * szAttrName );
	void GetElementAttribute( DOMElement * xmlElem, const char * szAttrName, 
		std::string & attrValue );
	void SetElementAttribute( DOMElement * xmlElem, const char * szAttrName, 
		const char * szAttrValue );

private :
	char file_[512];
	XercesDOMParser * parser_;
};

#endif  
