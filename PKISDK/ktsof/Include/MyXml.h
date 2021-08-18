#ifndef __MYXML_H__
#define __MYXML_H__

// =================================================================================
//  Include Header Files
// xerces_c
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <string>
#include <vector>

XERCES_CPP_NAMESPACE_USE

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  define XML_ENCODING "gbk"
#  define XML_HEADER "<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n"
#    ifdef _DEBUG
#      pragma comment(lib, "xerces-c_3D.lib")
#      pragma message("Automatically linking with xerces-c_3D.lib")
#    else
#      pragma comment(lib, "xerces-c_3.lib")
#      pragma message("Automatically linking with xerces-c_3.lib")
#    endif
#elif defined(_LINUX)
#  define XML_ENCODING "gb18030"  
#  define XML_HEADER "<?xml version=\"1.0\" encoding=\"gb18030\"?>\r\n"
#endif

// =================================================================================
//  Class Definition
class AStr
{
public :
	// = Default constructor.
	AStr(const char * const toTranscode)
	{ 
		fUnicodeForm = XMLString::transcode(toTranscode); 
	}

	// = Default destructor.
	~AStr()
	{ 
		XMLString::release(&fUnicodeForm); 
	}
	
	// = Get string.
	const XMLCh *unicodeForm() const
	{ 
		return fUnicodeForm; 
	}
	
private :
	XMLCh *fUnicodeForm;
};	
#define U(str) AStr(str).unicodeForm()

class UStr
{
public :
	// = Default constructor.
    UStr(const XMLCh * const toTranscode)
    {
        fLocalForm = XMLString::transcode(toTranscode);
    }

	// = Default destructor.
    ~UStr()
    {
        XMLString::release(&fLocalForm);
    }

	// = Get string.
    const char *localForm() const
    {
        return fLocalForm;
    }

private :
    char *fLocalForm;
};
#define A(str) UStr(str).localForm()

class MyXml
{
public :
	// = Default constructor.
	MyXml()
	{
		// do the initialization outside this class
//		if (MyXml::reference_ == 0)
//			XMLPlatformUtils::Initialize(XML_ENCODING);
//		MyXml::reference_++;

		m_pXmlParser = NULL;
		m_pXmlDocument = NULL;
	}

	// = Default desctructor.
	virtual ~MyXml()
	{
		if (m_pXmlParser != NULL)
		{
			delete m_pXmlParser;
			m_pXmlParser = NULL;
		}
		else if (m_pXmlDocument != NULL)
			m_pXmlDocument->release();
		m_pXmlDocument = NULL;

		// do the finalization outside this class
//		MyXml::reference_--;
//		if (MyXml::reference_ == 0)
//			XMLPlatformUtils::Terminate();
	}

	// = Load a xml file.
	KTBOOL32 load(const char *szFileName)
	{
		if (m_pXmlParser != NULL)
		{
			delete m_pXmlParser;
			m_pXmlParser = NULL;
		}
		else if (m_pXmlDocument != NULL)
			m_pXmlDocument->release();
		m_pXmlDocument = NULL;

		m_pXmlParser = new XercesDOMParser();
		if (m_pXmlParser == NULL)
			return FALSE;

		m_pXmlParser->parse(szFileName);
		if (m_pXmlParser->getErrorCount() > 0 || m_pXmlParser->getDocument() == NULL)
		{
			delete m_pXmlParser;
			m_pXmlParser = NULL;
			return FALSE;
		}
		m_pXmlDocument = m_pXmlParser->getDocument();

		return TRUE;
	}

	// = Save to a xml file.
	KTBOOL32 save(const char *szFileName, DOMNode *pNode = NULL)
	{
		DOMNode *pNodeToSave;
		if (pNode != NULL)
			pNodeToSave = pNode;
		else
			pNodeToSave = (DOMNode *)m_pXmlDocument;
		if (pNodeToSave == NULL)
			return FALSE;

		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(U("Core"));

		XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(szFileName);
		if (myFormTarget == NULL)
			return FALSE;

//		DOMWriter * theSerializer = impl->createDOMWriter();
//		theSerializer->writeNode(myFormTarget, *pNodeToSave);
//		theSerializer->release();

		DOMLSOutput *theOutput = impl->createLSOutput();
		if (theOutput == NULL)
		{
			delete myFormTarget;
			return FALSE;
		}
		theOutput->setByteStream(myFormTarget);

		DOMLSSerializer *theSerializer = impl->createLSSerializer();
		if (theSerializer == NULL)
		{
			theOutput->release();
			delete myFormTarget;
			return FALSE;
		}
//		theSerializer->setNewLine(XMLString::transcode("\r\n")); 

		bool bRet = theSerializer->write(pNodeToSave, theOutput);

		theSerializer->release();
		theOutput->release();
		delete myFormTarget;

		return bRet;
	}

	// = Load a xml string.
	KTBOOL32 loadFromString(const unsigned char *pbXmlString, long lLen)
	{
		if (m_pXmlParser != NULL)
		{
			delete m_pXmlParser;
			m_pXmlParser = NULL;
		}
		else if (m_pXmlDocument != NULL)
			m_pXmlDocument->release();
		m_pXmlDocument = NULL;

		MemBufInputSource *memBufIS = new MemBufInputSource(
			(const XMLByte *)pbXmlString,
			lLen,
			"XSECMem",
			false);
		if (memBufIS == NULL)
			return FALSE;

		m_pXmlParser = new XercesDOMParser();
		if (m_pXmlParser == NULL)
		{
			delete memBufIS;
			return FALSE;
		}

		m_pXmlParser->setDoNamespaces(true);
		m_pXmlParser->setCreateEntityReferenceNodes(true);
		m_pXmlParser->setDoSchema(true);
		m_pXmlParser->parse(*memBufIS);
		delete memBufIS;
		if (m_pXmlParser->getErrorCount() > 0 || m_pXmlParser->getDocument() == NULL)
		{
			delete m_pXmlParser;
			m_pXmlParser = NULL;
			return FALSE;
		}
		m_pXmlDocument = m_pXmlParser->getDocument();

		return TRUE;
	}

	// = Save to a xml string.
	KTBOOL32 saveToString(std::string & strXml, DOMNode *pNode = NULL)
	{
		DOMNode *pNodeToSave;
		if (pNode != NULL)
			pNodeToSave = pNode;
		else
			pNodeToSave = (DOMNode *)m_pXmlDocument;
		if (pNodeToSave == NULL)
			return FALSE;

 /*		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(U("Core"));

		DOMLSSerializer *theSerializer = impl->createLSSerializer();
		if (theSerializer == NULL)
			return FALSE;

		XMLCh *ustrXml =  theSerializer->writeToString(pNodeToSave);
		theSerializer->release();
		if (ustrXml == NULL)
			return FALSE;
		strXml = A(ustrXml);
		XMLString::release(&ustrXml);

		std::string strEnc = "GBK";
		if (pNodeToSave == m_pXmlDocument)
		{
			const XMLCh *ustrEnc = m_pXmlDocument->getXmlEncoding();
			if (ustrEnc)
				strEnc = A(ustrEnc);
		}
		std::string::size_type pos = strXml.find("UTF-16");
		if (pos != std::string::npos)
			strXml.replace(pos, strlen("UTF-16"), strEnc);
*/
		KTBOOL32 bRet = FALSE;

		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(U("Core"));

		DOMLSSerializer *theSerializer = NULL;
		DOMLSOutput *theOutputDesc = NULL;
		XMLFormatTarget *memTarget = NULL;
		
		theSerializer = impl->createLSSerializer();
		if (theSerializer == NULL)
			goto EXIT;

		theOutputDesc = impl->createLSOutput();
		if (theOutputDesc == NULL)
			goto EXIT;

		XMLCh encodeStr[100];
		XMLString::transcode("UTF-8", encodeStr, 99);
		theOutputDesc->setEncoding(encodeStr);

		memTarget = new MemBufFormatTarget();
		if (memTarget == NULL)
			goto EXIT;

		theOutputDesc->setByteStream(memTarget);
		bRet = theSerializer->write(pNodeToSave, theOutputDesc);
		if (!bRet)
			goto EXIT;

		int nRawLen;
		nRawLen = ((MemBufFormatTarget *)memTarget)->getLen();
		const unsigned char *pbRaw;
		pbRaw = ((MemBufFormatTarget *)memTarget)->getRawBuffer();
		if (nRawLen <= 0 || pbRaw == NULL)
			goto EXIT;

		strXml.assign((const char *)pbRaw, nRawLen);

		bRet = TRUE;

EXIT:
		if (memTarget != NULL)
			delete memTarget;
		if (theSerializer != NULL)
			theSerializer->release();
		if (theOutputDesc != NULL)
			theOutputDesc->release();

		return bRet;
	}

	// = Create document.
	KTBOOL32 createDocument(const char *szName)
	{
		if (m_pXmlParser != NULL)
		{
			delete m_pXmlParser;
			m_pXmlParser = NULL;
		}
		else if (m_pXmlDocument != NULL)
			m_pXmlDocument->release();
		m_pXmlDocument = NULL;

		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(U("Core"));

		m_pXmlDocument = impl->createDocument(0, U(szName), NULL);  

		return TRUE;
	}

	// = Get document.
	xercesc::DOMDocument *getDocument()
	{
		return m_pXmlDocument;
	}

	// = Get root element.
	DOMElement *getRoot()
	{
		if (m_pXmlDocument)
			return (DOMElement *)m_pXmlDocument->getDocumentElement();
		return NULL;
	}

	// = Get parent element.
	DOMElement *getParent(DOMElement *xmlElem)
	{
		return xmlElem ? (DOMElement *)xmlElem->getParentNode() : NULL;
	}

	// = Get child list by attribute.
	DOMElement *getFirstChildByAttr(DOMElement *xmlElem, const char *szName, 
		const char *szAttrName, const char *szAttrValue)
	{
		if (xmlElem)
		{
			DOMNodeList *xmlList = xmlElem->getElementsByTagName(U(szName));
			for (int i = 0; i < xmlList->getLength(); i++)
			{
				DOMElement *xmlTemp = (DOMElement *)xmlList->item(i);
				if (strcmp(this->getAttribute(xmlTemp, szAttrName).c_str(), szAttrValue) == 0)
					return xmlTemp;
			}
		}
		return NULL;
	}

	// = Get first child.
	DOMElement *getFirstChild(DOMElement *xmlElem, const char *szName)
	{
		if (xmlElem)
		{
			DOMNodeList *xmlList = xmlElem->getElementsByTagName(U(szName));
			if (xmlList->getLength() > 0)
				return (DOMElement *)xmlList->item(0);
		}
		return NULL;
	}

	// = Get child list.
	DOMNodeList *getChildren(DOMElement *xmlElem, const char *szName)
	{
		return xmlElem ? xmlElem->getElementsByTagName(U(szName)) : NULL;
	}

	// = Append a text child.
	DOMText *appendTextChild(DOMElement *xmlParent, const char *szName)
	{
		if (m_pXmlDocument && xmlParent)
		{
			DOMText *xmlText = m_pXmlDocument->createTextNode(U(szName));
			return xmlText ? (DOMText *)xmlParent->appendChild(xmlText) : NULL;
		}
		return NULL;
	}

	// = Append a child.
	DOMElement *appendChild(DOMElement *xmlParent, const char *szName)
	{
		if (m_pXmlDocument && xmlParent)
		{
			DOMElement *xmlElem = m_pXmlDocument->createElement(U(szName));
			return xmlElem ? (DOMElement *)xmlParent->appendChild(xmlElem) : NULL;
		}
		return NULL;
	}

	// = Remove a element.
	void removeElement(DOMElement *xmlElem)
	{
		if (xmlElem)
		{
			DOMNode *xmlParent = xmlElem->getParentNode();
			if (xmlParent)
			{
				xmlParent->removeChild(xmlElem);
				xmlElem->release();
			}
		}
	}

	// = Remove children.
	void removeChildren(DOMElement *xmlParent)
	{
		if (xmlParent)
		{
			DOMNodeList *xmlList = xmlParent->getChildNodes();
			for (int i = xmlList->getLength() - 1; i >= 0; i--)
			{
				DOMNode *xmlNode = xmlList->item(i);
				xmlParent->removeChild(xmlNode);
				xmlNode->release();
			}
		}
	}

	// = Get element name.
	std::string getName(DOMElement *xmlElem)
	{
		return xmlElem ? A(xmlElem->getNodeName()) : "";
	}

	// = Get element name.
	void getName(DOMElement *xmlElem, std::string & name)
	{
		name = xmlElem ? A(xmlElem->getNodeName()) : "";
	}

	// = Get string content.
	std::string getContent(DOMElement *xmlElem)
	{
		return xmlElem ? A(xmlElem->getTextContent()) : "";
	}

	// = Get string content.
	void getContent(DOMElement *xmlElem, std::string & content)
	{
		content = xmlElem ? A(xmlElem->getTextContent()) : "";
	}

	// = Set string content.
	void setContent(DOMElement *xmlElem, const char *szContent)
	{
		if (xmlElem)
			xmlElem->setTextContent(U(szContent));
	}

	// = Get attribute.
	std::string getAttribute(DOMElement *xmlElem, const char *szName)
	{
		return xmlElem ? A(xmlElem->getAttribute(U(szName))) : "";
	}

	// = Get attribute.
	void getAttribute(DOMElement *xmlElem, const char *szName, std::string & value)
	{
		value = xmlElem ? A(xmlElem->getAttribute(U(szName))) : "";
	}

	// = Set attribute.
	void setAttribute(DOMElement *xmlElem, const char *szName, const char *szValue)
	{
		if (xmlElem)
			xmlElem->setAttribute(U(szName), U(szValue));
	}

private:
	// = reference.
//	static long reference_;

	// = XML parser.
	XercesDOMParser *m_pXmlParser;

	// = XML Document
	xercesc::DOMDocument *m_pXmlDocument;
};

//long MyXml::reference_ = 0;

#endif  // __MYXML_H__  
