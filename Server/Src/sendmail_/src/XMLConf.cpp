#include "CommonHelper.h"
#include "XMLConf.h"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
//#include <xercesc/dom/DOMWriter.hpp>

XMLConf::XMLConf()
{
	m_parser = NULL;
	m_errHandler = NULL;
}

XMLConf::~XMLConf()
{
	Clear();
}

void XMLConf::Clear()
{
	SAFE_DELETE(m_parser);
	SAFE_DELETE(m_errHandler);
}

bool XMLConf::Load(std::string fname)
{
       try 
       {
            XMLPlatformUtils::Initialize();
       }
       catch (const XMLException& toCatch) 
       {
            char* message = XMLString::transcode(toCatch.getMessage());
            TRACE(std::string("XMLConf::Load ") + fname + " Message: " + message);
            delete [] message;
            return false;
        }

	Clear();
	
	m_parser = new XercesDOMParser();
       m_errHandler = (ErrorHandler*) new HandlerBase();
       m_parser->setErrorHandler(m_errHandler);

      	try 
      	{
            	m_parser->parse(fname.c_str());            	
       }
       catch (const XMLException& toCatch) 
       {
            	char* message = XMLString::transcode(toCatch.getMessage());
            	TRACE(std::string("XMLConf::Load ") + fname + " Message: " + message);
            	delete [] message;
            	return false;
        }
       catch (const DOMException& toCatch) 
       {
            	char* message = XMLString::transcode(toCatch.msg);
            	TRACE(std::string("XMLConf::Load ") + fname + " Message: " + message);
            	delete [] message;
            	return false;
        }
        catch (...) 
        {
            	TRACE(std::string("XMLConf::Load ") + fname + " Message: Unexpected Exception while loading.");
            	return false;
        }

	return true;
}

bool XMLConf::Save(std::string fname)
{
	try
	{
		// get a serializer, an instance of DOMWriter
		XMLCh tempStr[100];
		XMLString::transcode("LS", tempStr, 99);
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

		DOMLSOutput *theOutput = impl->createLSOutput();	//modified
//		DOMWriter *theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
		XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(fname.c_str());
		// get the DOM representation
		DOMNode *doc = m_parser->getDocument();
		theOutput->setByteStream(myFormTarget);
//		theSerializer->writeNode(myFormTarget, *doc);
		DOMLSSerializer *theSerializer = impl->createLSSerializer();
		theSerializer->write(doc, theOutput);
		
		delete myFormTarget;
		delete theSerializer;
		delete theOutput;		//added
		return true;
	}
	catch (const XMLException& toCatch) 
       {
            char* message = XMLString::transcode(toCatch.getMessage());
            TRACE(std::string("Save the changes to ") + fname + " Message: " + message);
            delete [] message;
            return false;
        }
}

DOMDocument* XMLConf::GetDocument()
{
	assert(m_parser != NULL);
	return m_parser->getDocument ();
}

bool XMLConf::splitXPath(std::string xpath, MyStringList* sl)
{
        assert(xpath[0] == '/');
        
        //strip last '/'
        if ('/' == xpath[xpath.size() -1])
        	xpath = xpath.substr(0, xpath.size() -1);

        int npos = 1;
        do
        {
                int nsize = xpath.find("/", npos);
                if (nsize < 0)
                        nsize = xpath.size();
                //cout << nsize << " " << npos << endl;
                std::string sub = xpath.substr(npos, nsize - npos);
                //cout << sub << endl;
                sl->push_back(sub);
                npos = nsize + 1;
        }
        while (npos < xpath.size());

        return true;
}

DOMNodeList *XMLConf::GetNodeList(DOMElement * parent, const char *tagName)
{
	if( NULL == tagName || 0 >= strlen(tagName) )
		return NULL;

	XMLCh* xtn = XMLString::transcode(tagName);
	DOMNodeList * nl = parent->getElementsByTagName(xtn);
	delete xtn;

	return nl;
}

DOMNodeList * XMLConf::GetNodeList(std::string xpath)
{
	MyStringList sl;
	splitXPath(xpath, &sl);

	DOMNodeList*  nl = NULL;
	if (sl.size() > 0)
	{
		DOMElement* nd = (DOMElement *)GetDocument()->getDocumentElement();
		assert(nd != NULL);

		//compare root tag name
		XMLCh* xtn = XMLString::transcode(sl.front().c_str());
		if (0 != XMLString::compareIString(nd->getTagName(), xtn))
		{
			SetLastErrorString("root tagname is not compatible!");
			delete xtn;
			return NULL;
		}
		sl.pop_front();
		delete xtn;
		
		while (sl.size() > 0)
		{
			std::string tag = sl.front();

			XMLCh* xch = XMLString::transcode(tag.c_str());
			nl = nd->getElementsByTagName(xch);
			delete xch;
			
			if (NULL == nl || 0 == nl->getLength())
			{
				return NULL;
			}
			//only handle the first item;
			nd = (DOMElement*)(nl->item(0));

			//char* ch = XMLString::transcode(nd->getNodeName());
			//cout << "node name: " << ch << endl;
			//delete ch;

			sl.pop_front();
		}
	}
		
	return nl;
}

DOMElement * XMLConf::GetNodeByTag(DOMElement * parent, const char * tabName)
{
	DOMNodeList * pNodeList = NULL;
	DOMElement * pElement;
	char *name;

	pNodeList = parent->getChildNodes();

	for (int i = 0; i < pNodeList->getLength(); i++)
	{
		pElement = (DOMElement *)pNodeList->item(i);
		if (pElement)
		{
			name = XMLString::transcode(pElement->getNodeName());
			if (!strcasecmp(name, tabName))
			{
				delete [] name;
				return pElement;
			}
			delete [] name;
		}
	}

	return NULL;
}

std::string XMLConf::GetTextOf(std::string xpath)
{
	std::string tt;
	MyStringList sl;
	if (GetTextsOf(xpath, &sl, 1) <= 0)
		return tt;

	if (sl.size() > 0)
		return sl.front();
	else
		return tt;
}

int XMLConf::GetTextsOf(std::string xpath, MyStringList* slv, int nCount)		
{
	DOMNodeList *nl = GetNodeList(xpath);
	if (NULL == nl)
		return -1;

	return getTextsOf(nl, slv, nCount);
}

int XMLConf::GetTextsOf(DOMElement * parent, const char* tagName, MyStringList* slv, int nCount)
{
	assert(parent != NULL);

	XMLCh* xtn = XMLString::transcode(tagName);
	DOMNodeList * nl = parent->getElementsByTagName(xtn);
	delete xtn;

	return getTextsOf(nl, slv, nCount);
}

std::string XMLConf::GetTextOf(DOMElement * parent, const char* tagName)
{
	MyStringList sl;
	std::string temp;
	if (GetTextsOf(parent, tagName, &sl, 1) <= 0)
		return temp;

	if (sl.size() > 0)
		return sl.front();
	else
		return temp;
}

std::string XMLConf::GetTextOf(DOMNode *pNode)
{
	std::string tmp("");
	if (NULL == pNode) return tmp;
	DOMNode *pChild = pNode->getFirstChild();
	if (NULL == pChild) return tmp;
	char *s = XMLString::transcode(pChild->getNodeValue());
	tmp = s;
	delete[] s;
	
	return tmp;
}

int XMLConf::getTextsOf(DOMNodeList *nl, MyStringList* slv, int nCount)
{
	if (NULL == nl || NULL == slv)
		return -1;

	int n = 0;
	nCount = (nCount < 0) ? nl->getLength() : nCount;
	for (n = 0; n < nCount && n < nl->getLength(); n ++)
	{
		// assert(nl->item(n)->hasChildNodes() && dynamic_cast<DOMText *>(nl->item(n)->getFirstChild()) != NULL);
		char * ch = NULL;
		if (nl->item(n))
		{
			if (nl->item(n)->hasChildNodes() && 
				dynamic_cast<DOMText *>(nl->item(n)->getFirstChild()))
			{
				ch = XMLString::transcode(nl->item(n)->getFirstChild()->getNodeValue());
			}
		}
		if (ch)
		{
			slv->push_back(ch);
			delete [] ch;
		}
	}
	return n;
}

void XMLConf::SetValue(std::string xpath, const char *value)
{
	DOMNodeList *nl = GetNodeList(xpath);
	assert(nl->getLength() == 1);
	DOMNode *pNode = nl->item(0)->getFirstChild();
	XMLCh *xvalue = XMLString::transcode(value);
	if(pNode != NULL)
	{
		pNode->setNodeValue(xvalue);
	}
	else
	{
		DOMText *t = m_parser->getDocument()->createTextNode(xvalue);
		nl->item(0)->appendChild(t);
	}
	
	delete xvalue;
}

void XMLConf::SetValue(DOMElement *parent, const char *tagName, const char *value)
{
	XMLCh *xtag = XMLString::transcode(tagName);
	DOMNodeList *nl = parent->getElementsByTagName(xtag);
	delete xtag;

	assert(nl->getLength() == 1);
	DOMNode *pNode = nl->item(0)->getFirstChild();
	XMLCh *xvalue = XMLString::transcode(value);
	if(pNode != NULL)
	{
		pNode->setNodeValue(xvalue);
	}
	else
	{
		DOMText *t = m_parser->getDocument()->createTextNode(xvalue);
		nl->item(0)->appendChild(t);
	}
	
	delete xvalue;
}

DOMElement* XMLConf::AppendItem(DOMElement *parent, const char *tagName, const char *description, const char *value)
{
	DOMDocument *pDoc = m_parser->getDocument();
	
	XMLCh *xtag = XMLString::transcode(tagName);
	DOMElement *pElement = pDoc->createElement(xtag);
	delete xtag;

	XMLCh *xdes = XMLString::transcode("description");
	XMLCh *xdesValue = XMLString::transcode(description);
	pElement->setAttribute(xdes, xdesValue);
	delete xdes;
	delete xdesValue;
	
	if (value != "")
	{
		XMLCh *xvalue = XMLString::transcode(value);
		DOMText *pText = pDoc->createTextNode(xvalue);
		delete xvalue;
		
		pElement->appendChild(pText);
	}
	
	return (DOMElement *)parent->appendChild(pElement);
}

void XMLConf::SetElementAtt(DOMElement *e, const char *AttName, const char *AttValue)
{
	XMLCh *name = XMLString::transcode(AttName);
	XMLCh *value = XMLString::transcode(AttValue);
	e->setAttribute(name, value);
	delete name;
	delete value;
}

/** $Log: XMLConf.cpp, v1.0 $
 * @ Revision history
 * @ -------------------------------------------------------------------------
 * @ Version      Date              Author               Note
 * @ -------------------------------------------------------------------------
 * @ 1.0          2003-01-10 14:09  linzongfang          ´´½¨°æ±¾
 * @
 * @
 */

