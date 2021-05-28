#if defined(_WIN32)
#  include <crtdbg.h>
#endif
#include "XmlDocument.h"


class XStr
{
public :
	XStr( const char * const toTranscode )
	{ 
		fUnicodeForm = XMLString::transcode( toTranscode ); 
	}
	~XStr()
	{ 
		XMLString::release( &fUnicodeForm ); 
	}
	
	// = Get string.
	const XMLCh * unicodeForm() const
	{ 
		return fUnicodeForm; 
	}
	
private :
	XMLCh * fUnicodeForm;
};	
#define X( str ) XStr( str ).unicodeForm()

class UStr
{
public :
    UStr( const XMLCh * const toTranscode )
    {
        fLocalForm = XMLString::transcode( toTranscode );
    }

    ~UStr()
    {
        XMLString::release( &fLocalForm );
    }

	// = Get string.
    const char * localForm() const
    {
        return fLocalForm;
    }

private :
    char * fLocalForm;
};
#define U( str ) UStr( str ).localForm()

CXmlDocument::CXmlDocument()
{
	XMLPlatformUtils::Initialize ();
	this->parser_ = NULL;
	memset( this->file_, 0, sizeof( this->file_ ) );
}

CXmlDocument::~CXmlDocument()
{
	if( this->parser_ )
		delete this->parser_;
	XMLPlatformUtils::Terminate();
}

int CXmlDocument::Load( const char * szFileName )
{
	
	
#if defined(_WIN32)
	_ASSERT( szFileName );
#endif
	strcpy( this->file_, szFileName );

    
	if( this->parser_ ) 
	{
		delete this->parser_;
	}
	this->parser_ = new XercesDOMParser();
    
	try
	{
		this->parser_->parse( this->file_ );
		if( this->parser_->getDocument() == NULL )
		{
			printf( "[Error] Loading (%s) Failed: Invalid XML Document.\n", this->file_ );
			return -1;
		}
	}
	catch( ... )
	{
		printf( "[Error] Loading (%s) Failed: Unexpected Exception While Loading.\n" );
		return -1;
	}
	return 0;
	
}

int CXmlDocument::Save( const char * szFileName /* = NULL */ )
{	
	if( szFileName )
		strcpy( this->file_, szFileName );

	try
	{
		DOMImplementation * impl = DOMImplementationRegistry::getDOMImplementation( X( "Core" ) );
		XMLFormatTarget * myFormTarget = new LocalFileFormatTarget( this->file_ );
		DOMLSOutput *theOutput = impl->createLSOutput();
		DOMNode *doc = this->parser_->getDocument();
		theOutput->setByteStream(myFormTarget);
		DOMLSSerializer *theSerializer = impl->createLSSerializer();
		theSerializer->write(doc, theOutput);
		delete theSerializer;
		delete theOutput;
		delete myFormTarget;
	}
	catch( const XMLException & toCatch )
	{
		printf( "[Error] Saving (%s) Failed: %s.\n", this->file_, U( toCatch.getMessage() ) );
		return -1;
	}	
	return 0;
}

DOMElement * CXmlDocument::GetRootElement()
{
#if defined(_WIN32)
	_ASSERT( this->parser_ );
	_ASSERT( this->parser_->getDocument() );
#endif
	return (DOMElement *)parser_->getDocument()->getDocumentElement();
}


DOMElement * CXmlDocument::GetParentElement( DOMElement * xmlElem )
{
	return xmlElem ? (DOMElement *)xmlElem->getParentNode() : NULL;
}


DOMElement * CXmlDocument::GetFirstChild( DOMElement * xmlElem, const char * szName /* = "*" */ )
{
	DOMNodeList * xmlList = GetChildren( xmlElem, szName );
	if( xmlList && xmlList->getLength() > 0 )
		return (DOMElement *)xmlList->item( 0 );
	else
		return NULL;
}


DOMNodeList * CXmlDocument::GetChildren( DOMElement * xmlElem, const char * szName /* = "*" */ )
{
	return xmlElem ? xmlElem->getElementsByTagName( X( szName ? szName : "*" ) ) : NULL;
}


DOMElement * CXmlDocument::AppendChild( DOMElement * xmlParent, const char * szName )
{
#if defined(_WIN32)
	_ASSERT( this->parser_ );
	_ASSERT( this->parser_->getDocument() );
#endif
	if( xmlParent )
	{
		DOMElement * xmlElem = parser_->getDocument()->createElement( X( szName ) );
		if( xmlElem )
			return (DOMElement *)xmlParent->appendChild( xmlElem );
	}
	return NULL;
}


void CXmlDocument::RemoveElement( DOMElement * xmlElem )
{
	if( xmlElem )
	{
		DOMNode * xmlParent = xmlElem->getParentNode();
		if( xmlParent )
		{
			xmlParent->removeChild( xmlElem );
			xmlElem->release();
		}
	}
}

void CXmlDocument::RemoveChildren( DOMElement * xmlParent )
{
	if( xmlParent )
	{
		DOMNodeList * xmlList = xmlParent->getChildNodes();
		int nCount = xmlList->getLength();
		for( int i = 0; i < nCount; i++ )
		{
			DOMNode * xmlNode = xmlList->item( 0 );
			xmlParent->removeChild( xmlNode );
			xmlNode->release();
		}
	}
}


std::string CXmlDocument::GetElementName( DOMElement * xmlElem )
{
	return xmlElem ? U( xmlElem->getNodeName() ) : "";
}

void CXmlDocument::GetElementName( DOMElement * xmlElem, std::string & name )
{
	name = xmlElem ? U( xmlElem->getNodeName() ) : "";
}

std::string CXmlDocument::GetElementContent( DOMElement * xmlElem )
{
	return xmlElem ? U( xmlElem->getTextContent() ) : "";
}

void CXmlDocument::GetElementContent( DOMElement * xmlElem, std::string & content )
{
	content = xmlElem ? U( xmlElem->getTextContent() ) : "";
}

void CXmlDocument::SetElementContent( DOMElement * xmlElem, const char * szContent )
{
#if defined(_WIN32)
	_ASSERT( szContent );
#endif
	if( xmlElem )
		xmlElem->setTextContent( X( szContent ) );
}

std::string CXmlDocument::GetElementAttribute( DOMElement * xmlElem, const char * szAttrName )
{
#if defined(_WIN32)
	_ASSERT( szAttrName );
#endif
	return xmlElem ? U( xmlElem->getAttribute( X( szAttrName ) ) ) : "";
}

void CXmlDocument::GetElementAttribute( DOMElement * xmlElem, const char * szAttrName, 
									   std::string & attrValue )
{
#if defined(_WIN32)
	_ASSERT( szAttrName );
#endif
	attrValue = xmlElem ? U( xmlElem->getAttribute( X( szAttrName ) ) ) : "";
}

void CXmlDocument::SetElementAttribute( DOMElement * xmlElem, const char * szAttrName, 
									   const char * szAttrValue )
{
#if defined(_WIN32)
	_ASSERT( szAttrName );
	_ASSERT( szAttrValue );
#endif
	if( xmlElem )
		xmlElem->setAttribute( X( szAttrName ), X( szAttrValue ) );
}


