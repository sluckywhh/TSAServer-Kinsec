#ifndef __MYXMLSIGN_H__
#define __MYXMLSIGN_H__

// =================================================================================
//  Include Header Files
// xml_security_c
#include <xsec/framework/XSECProvider.hpp>
#include <xsec/dsig/DSIGReference.hpp>
#include <xsec/enc/OpenSSL/OpenSSLCryptoKeyRSA.hpp>
#include <xsec/framework/XSECException.hpp>
#include <xsec/dsig/DSIGKeyInfoX509.hpp>
#include <xsec/enc/OpenSSL/OpenSSLCryptoX509.hpp>
#include <xsec/enc/XSECCryptoException.hpp>
// xalan
#ifndef XSEC_NO_XALAN
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
XALAN_USING_XALAN(XalanTransformer)
#endif

// openssl
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

// other
#include "MyXml.h"
#include "MyToolLibFunc.h"
#include "MailCoder.h"

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#    ifdef _DEBUG
#      pragma comment(lib, "xsec_1D.lib")
#      pragma message("Automatically linking with xsec_1D.lib")
#    else
#      pragma comment(lib, "xsec_1.lib")
#      pragma message("Automatically linking with xsec_1.lib")
#    endif
#elif defined(_LINUX)
#endif

#define MYXML_SIZE_CERT	4096

typedef int (*FUNC_PRIV_ENC)(
	int,
	const unsigned char *,
	unsigned char *,
	RSA *,
	int);

// =================================================================================
//  Class Definition
class MyXmlSign : public MyXml
{
public :
	// = Default constructor.
	MyXmlSign()
	{
		// do the initialization outside this class
		// memory leak found
//		if (MyXmlSign::reference_ == 0)
//		{
//#ifndef XSEC_NO_XALAN
//			XalanTransformer::initialize();
//#endif
//			XSECPlatformUtils::Initialise();
//		}
//		MyXmlSign::reference_++;

		m_sig = NULL;
		m_funcold = NULL;
	}

	// = Default desctructor.
	virtual ~MyXmlSign()
	{
		if (m_sig != NULL)
		{
			RestoreSignFunc();
			m_prov.releaseSignature(m_sig);
			m_sig = NULL;
		}

		// do the finalization outside this class
//		MyXmlSign::reference_--;
//		if (MyXmlSign::reference_ == 0)
//		{
//#ifndef XSEC_NO_XALAN
//			XalanTransformer::Terminate();
//#endif
//			XSECPlatformUtils::Terminate();
//		}
	}

	KTBOOL32 InitSign(bool bSM2, bool bRSA2048, char *pszPrefix = NULL)
	{
//		if (bSM2)
//			return FALSE;

		if (m_sig != NULL)
		{
			RestoreSignFunc();
			m_prov.releaseSignature(m_sig);
			m_sig = NULL;
		}

		xercesc::DOMDocument *doc = getDocument();
		DOMElement *rootElem = getRoot();
		if (doc == NULL || rootElem == NULL)
			return FALSE;

		KTBOOL32 bRet = FALSE;
		try
		{
			// create a signature object
			m_sig = m_prov.newSignature();
			if (pszPrefix != NULL && strlen(pszPrefix) > 0)
				m_sig->setDSIGNSPrefix(MAKE_UNICODE_STRING(pszPrefix));

			// use it to create a blank signature DOM structure from the doc
			DOMElement *sigNode;
//			sigNode = m_sig->createBlankSignature(doc, CANON_C14N_COM, SIGNATURE_RSA, HASH_SHA1);
// 			sigNode = m_sig->createBlankSignature(
// 				doc,
// 				MAKE_UNICODE_STRING("http://www.w3.org/TR/2001/REC-xml-c14n-20010315"),
// 				MAKE_UNICODE_STRING("http://www.w3.org/2000/09/xmldsig#rsa-sha1"));
			sigNode = m_sig->createBlankSignature(
				doc,
				CANON_C14N_NOC,
				bSM2 ? SIGNATURE_SM2 : SIGNATURE_RSA,
				bSM2 ? HASH_SM3 : (bRSA2048 ? HASH_SHA256 : HASH_SHA1));

			// inser the signature DOM nodes into the doc
//			rootElem->appendChild(doc->createTextNode(MAKE_UNICODE_STRING("\n")));
			rootElem->appendChild(sigNode);
			rootElem->appendChild(doc->createTextNode(MAKE_UNICODE_STRING("\n")));

			// create an envelope reference for the text to be signed
//			DSIGReference *ref = m_sig->createReference(MAKE_UNICODE_STRING(""));
			DSIGReference *ref = m_sig->createReference(
				&chNull,
				bSM2 ? DSIGConstants::s_unicodeStrURISM3 : (bRSA2048 ? DSIGConstants::s_unicodeStrURISHA256 : DSIGConstants::s_unicodeStrURISHA1));
			ref->appendEnvelopedSignatureTransform();

			bRet = TRUE;
		}
		catch (XSECException &e)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during signature initialization\nError Type = %d, Error Message = %s",
				e.getType(), e.getMsg());
		}

		return bRet;
	}

	KTBOOL32 SetSignKeyAndFunc(
		char *pszPrivateKey,
		FUNC_PRIV_ENC funcPrivEnc)
	{
		if (m_sig == NULL ||
			(pszPrivateKey != NULL && (strlen(pszPrivateKey) == 0 || strlen(pszPrivateKey) >= MYXML_SIZE_CERT * 2)))
			return FALSE;

		m_funcold = NULL;

		char szPrvKeyB64[MYXML_SIZE_CERT * 2];
		memset(szPrvKeyB64, 0, sizeof(szPrvKeyB64));
		if (pszPrivateKey != NULL)
			strcpy(szPrvKeyB64, pszPrivateKey);
		else
			// use a fake private key
			strcpy(szPrvKeyB64, "MIICXgIBAAKBgQD2QLfzDYC/VOnv7jJT8UGIax1yStNspke//cMpEwgHfMwnN7Ok\
78JEaoGp8aVqqbYgSGhUjfXPRed1R9oz4d6AhlBtrKDTGVagdYGPzQR9XfSa58OB\
EDn01x6SSEDGR+3FBPA/a5gLQfJxFxMdKUNfFvstR1+JCa4GE9HnC5iwhQIDAQAB\
AoGANL1fqDp4HII/KBfHx5qgAmuOfu/voBFsxH3A0oc07QWZY7zWSLjgzm5RHuiW\
GOYID5nyXkmZH69FByVAYmQ5lrmgHjCvUkFxmGhuCRhLhD0HFuNaqWCfqRe4U1SW\
6Gn7LKDB6tKH90twj+7we9HdQ3CZx/GiGgeA09rWrrsXl3ECQQD9LqOBqI8Y1pLb\
oM8Xxmh4MuBchkPOF236cJSlfMp0cBbaPzblVYF04yC7geiX/NvRh0mSTCmsVqpL\
sK8CYZkzAkEA+P5WOjCbPhPA9gx/hDSWdMLlwwWPo0rW+mtmYgNvvF8jMDCRFqkU\
f/LfbpnWMYnTok1un/RMu1QWVZ3e7N2/ZwJBALCJUac54jV+gL41vdElHnkZeInr\
cOIGQkBLOcq+Xeics8KQK3XzjiXzrUIxjf5UaO4LuEgTZUdu0qWeWBl/9jMCQQCy\
3cpFnPs4eepJFqtX5osar5NsKIeJz05lsFdpG3zd/h4CFTF07NtMGsXE5wAGsDL2\
ydcpUs74VyyALpug3p8LAkEAk7Pu2KPW0RxOLlLL9Jres6hqpQbbOJVZWjBvx+xG\
wXLVJRUwr5iw+fYc2RTeN6Bpqdq7G95IgJG+bsmTQikMUg==");

		// Base64 decode
		unsigned char byPrvKey[MYXML_SIZE_CERT];
		memset(byPrvKey, 0, sizeof(byPrvKey));
		long lPrvKeyLen = CMailCoder::base64_decode(szPrvKeyB64, strlen(szPrvKeyB64), (char *)byPrvKey);
		if (lPrvKeyLen <= 0)
			return FALSE;

		// load private key
		unsigned char *pbTmp = byPrvKey;
		EVP_PKEY *pkey = d2i_AutoPrivateKey(NULL, (const unsigned char **)&pbTmp, lPrvKeyLen);
		if (pkey == NULL)
			return FALSE;

		KTBOOL32 bRet = FALSE;
		OpenSSLCryptoKeyRSA *rsaKey = NULL;
		try
		{
			// set the sign Key
			rsaKey = new OpenSSLCryptoKeyRSA(pkey);
			if (rsaKey == NULL)
				goto EXIT;

			// set the function of private key encryption
			if (funcPrivEnc != NULL)
			{
				RSA *rsa = rsaKey->getOpenSSLRSA();
				RSA_METHOD *meth = (RSA_METHOD *)rsa->meth;
				m_funcold = meth->rsa_priv_enc;
				meth->rsa_priv_enc = funcPrivEnc;
			}

			m_sig->setSigningKey(rsaKey);

			bRet = TRUE;
		}
		catch (XSECException &e)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during a sign key load\nError Type = %d, Error Message = %s",
				e.getType(), e.getMsg());
		}

EXIT:
		if (!bRet)
		{
			if (rsaKey)
				delete rsaKey;
		}
		if (pkey)
			EVP_PKEY_free(pkey);

		return bRet;
	}

	void RestoreSignFunc()
	{
		if (m_sig == NULL || m_funcold == NULL)
			return;

		try
		{
			// restore the function of private key encryption
			OpenSSLCryptoKeyRSA *rsaKey = (OpenSSLCryptoKeyRSA *)m_sig->getSigningKey();
			if (rsaKey != NULL)
			{
				RSA *rsa = rsaKey->getOpenSSLRSA();
				RSA_METHOD *meth = (RSA_METHOD *)rsa->meth;
				meth->rsa_priv_enc = m_funcold;
				m_funcold = NULL;
			}
		}
		catch (XSECException &e)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during restoring sign function\nError Type = %d, Error Message = %s",
				e.getType(), e.getMsg());
		}

	}

	KTBOOL32 SetSignCert(
		char *pszCert,
		char *pszKeyName = NULL)
	{
		if (m_sig == NULL ||
			pszCert == NULL || strlen(pszCert) == 0 || strlen(pszCert) >= MYXML_SIZE_CERT * 2)
			return FALSE;

		// Base64 decode
		unsigned char pbCert[MYXML_SIZE_CERT];
		memset(pbCert, 0, sizeof(pbCert));
		long lCertLen = CMailCoder::base64_decode(pszCert, strlen(pszCert), (char *)pbCert);
		if (lCertLen <= 0)
			return FALSE;

		// used for sm2 sign preprocessing
		bool bSM2 = MYTOOL_IsECCCert(pbCert, lCertLen);
		if (bSM2)
			m_sig->SetSignCert(pbCert, lCertLen);

		// load cert
		unsigned char *pbTmp = (unsigned char *)pbCert;
		X509 *signcert = d2i_X509(NULL, (const unsigned char **)&pbTmp, lCertLen);
		if (signcert == NULL)
			return FALSE;
		X509_free(signcert);

		// Base64 encode
		char szCertB64[MYXML_SIZE_CERT * 2];
		memset(szCertB64, 0, sizeof(szCertB64));
		long lCertB64Len = CMailCoder::base64_encode((char *)pbCert, lCertLen, szCertB64);
		if (lCertB64Len <= 0)
			return FALSE;

		KTBOOL32 bRet = FALSE;
		try
		{
			// add a KeyInfo element
			if (pszKeyName != NULL && strlen(pszKeyName) > 0)
				m_sig->appendKeyName(U(pszKeyName));

			DSIGKeyInfoX509 *x509data = m_sig->appendX509Data();
			x509data->appendX509Certificate(U(szCertB64));

			bRet = TRUE;
		}
		catch (XSECException &e)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during a sign cert load\nError Type = %d, Error Message = %s",
				e.getType(), e.getMsg());
		}

		return bRet;
	}

	KTBOOL32 Sign()
	{
		if (m_sig == NULL)
			return FALSE;

		KTBOOL32 bRet = FALSE;
		try
		{
			// sign
			m_sig->sign();

			// restore the function of private key encryption
			RestoreSignFunc();

			bRet = TRUE;
		}
		catch (XSECException &e)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during signature calculation\nError Type = %d, Error Message = %s",
				e.getType(), e.getMsg());
		}

		return bRet;
	}

	KTBOOL32 Verify()
	{
		if (m_sig != NULL)
		{
			RestoreSignFunc();
			m_prov.releaseSignature(m_sig);
			m_sig = NULL;
		}

		xercesc::DOMDocument *doc = getDocument();
		DOMElement *rootElem = getRoot();
		if (doc == NULL || rootElem == NULL)
			return FALSE;

		KTBOOL32 bRet = FALSE;
		try
		{
			m_sig = m_prov.newSignatureFromDOM(doc);
			m_sig->load();

			// get sign cert
			XSECCryptoX509 *x509 = NULL;
			const XMLCh *x509Data = NULL;
			DSIGKeyInfoList *kinfList = m_sig->getKeyInfoList();
			if (kinfList == NULL)
				return FALSE;

			DSIGKeyInfoList::size_type i, kinfSize;
			kinfSize = kinfList->getSize();
			for (i = 0; i < kinfSize; i++)
			{
				DSIGKeyInfo *kinf = kinfList->item(i);
				if (kinf->getKeyInfoType() == DSIGKeyInfo::KEYINFO_X509)
				{
					DSIGKeyInfoX509 *kinfX509 = (DSIGKeyInfoX509 *)kinf;
					int nCertListSize = kinfX509->getCertificateListSize();
					if (nCertListSize > 0)
					{
						x509 = kinfX509->getCertificateCryptoItem(0);
						x509Data = kinfX509->getCertificateItem(0);
						break;
					}
				}
			}
			if (x509 == NULL || x509Data == NULL)
				return FALSE;

			// Base64 decode
			std::string strCert = A(x509Data);

			unsigned char pbCert[MYXML_SIZE_CERT];
			memset(pbCert, 0, sizeof(pbCert));
			long lCertLen = CMailCoder::base64_decode((char *)strCert.c_str(), strCert.size(), (char *)pbCert);
			if (lCertLen <= 0)
				return FALSE;

			bool bSM2 = MYTOOL_IsECCCert(pbCert, lCertLen);
			if (!bSM2)
				m_sig->setSigningKey(x509->clonePublicKey());
			else
			{
				// used for sm2 sign preprocessing
				m_sig->SetSignCert(pbCert, lCertLen);

				// use a fake private key
				char szPrvKeyB64[MYXML_SIZE_CERT * 2];
				memset(szPrvKeyB64, 0, sizeof(szPrvKeyB64));
				strcpy(szPrvKeyB64, "MIICXgIBAAKBgQD2QLfzDYC/VOnv7jJT8UGIax1yStNspke//cMpEwgHfMwnN7Ok\
78JEaoGp8aVqqbYgSGhUjfXPRed1R9oz4d6AhlBtrKDTGVagdYGPzQR9XfSa58OB\
EDn01x6SSEDGR+3FBPA/a5gLQfJxFxMdKUNfFvstR1+JCa4GE9HnC5iwhQIDAQAB\
AoGANL1fqDp4HII/KBfHx5qgAmuOfu/voBFsxH3A0oc07QWZY7zWSLjgzm5RHuiW\
GOYID5nyXkmZH69FByVAYmQ5lrmgHjCvUkFxmGhuCRhLhD0HFuNaqWCfqRe4U1SW\
6Gn7LKDB6tKH90twj+7we9HdQ3CZx/GiGgeA09rWrrsXl3ECQQD9LqOBqI8Y1pLb\
oM8Xxmh4MuBchkPOF236cJSlfMp0cBbaPzblVYF04yC7geiX/NvRh0mSTCmsVqpL\
sK8CYZkzAkEA+P5WOjCbPhPA9gx/hDSWdMLlwwWPo0rW+mtmYgNvvF8jMDCRFqkU\
f/LfbpnWMYnTok1un/RMu1QWVZ3e7N2/ZwJBALCJUac54jV+gL41vdElHnkZeInr\
cOIGQkBLOcq+Xeics8KQK3XzjiXzrUIxjf5UaO4LuEgTZUdu0qWeWBl/9jMCQQCy\
3cpFnPs4eepJFqtX5osar5NsKIeJz05lsFdpG3zd/h4CFTF07NtMGsXE5wAGsDL2\
ydcpUs74VyyALpug3p8LAkEAk7Pu2KPW0RxOLlLL9Jres6hqpQbbOJVZWjBvx+xG\
wXLVJRUwr5iw+fYc2RTeN6Bpqdq7G95IgJG+bsmTQikMUg==");

				// Base64 decode
				unsigned char byPrvKey[MYXML_SIZE_CERT];
				memset(byPrvKey, 0, sizeof(byPrvKey));
				long lPrvKeyLen = CMailCoder::base64_decode(szPrvKeyB64, strlen(szPrvKeyB64), (char *)byPrvKey);
				if (lPrvKeyLen <= 0)
					return FALSE;

				// load private key
				unsigned char *pbTmp = byPrvKey;
				EVP_PKEY *pkey = d2i_AutoPrivateKey(NULL, (const unsigned char **)&pbTmp, lPrvKeyLen);
				if (pkey == NULL)
					return FALSE;

				// set the sign Key
				OpenSSLCryptoKeyRSA *rsaKey = new OpenSSLCryptoKeyRSA(pkey);
				EVP_PKEY_free(pkey);
				if (rsaKey == NULL)
					return FALSE;

				m_sig->setSigningKey(rsaKey);
			}

			// verify sign
			if (!m_sig->verify())
				return FALSE;

			bRet = TRUE;
		}
		catch (XSECException &e)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during signature verification\nError Type = %d, Error Message = %s",
				e.getType(), e.getMsg());
		}
		catch (XSECCryptoException &e2)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during signature verification\nError Type = %d, Error Message = %s",
				e2.getType(), e2.getMsg());
		}

		return bRet;
	}

	KTBOOL32 GetInfo(long lType, std::string & strInfo)
	{
		if (m_sig != NULL)
		{
			RestoreSignFunc();
			m_prov.releaseSignature(m_sig);
			m_sig = NULL;
		}

		xercesc::DOMDocument *doc = getDocument();
		DOMElement *rootElem = getRoot();
		if (doc == NULL || rootElem == NULL)
			return FALSE;

		KTBOOL32 bRet = FALSE;
		try
		{
			m_sig = m_prov.newSignatureFromDOM(doc);
			m_sig->load();

			KTBOOL32 bFound = FALSE;
			if (lType == 1)
			{
				// get data
				std::string strXml;
				if (!saveToString(strXml))
					return FALSE;

				MyXml xmlclone;
				if (!xmlclone.loadFromString((const unsigned char *)(strXml.c_str()), strXml.size()))
					return FALSE;

//				DOMElement *sigNode = xmlclone.getFirstChild(xmlclone.getRoot(), "Signature");
//				if (sigNode == NULL)
//					return FALSE;
//				xmlclone.removeElement(sigNode);

				DOMNodeList *nodeList = xmlclone.getRoot()->getChildNodes();
				if (nodeList == NULL)
					return FALSE;

				XMLSize_t nodeSize = nodeList->getLength();
				for (int i = nodeSize - 1; i >= 0; i--)
				{
					DOMNode *node = nodeList->item(i);
					std::string strName = A(node->getNodeName());
					if ((i == nodeSize - 1 && strName == "#text") || strName == "Signature" || strName == "ds:Signature")
					{
						xmlclone.getRoot()->removeChild(node);
						node->release();
					}
				}

				if (!xmlclone.saveToString(strInfo))
					return FALSE;
			}
			else if (lType == 2 || lType == 5)
			{
				DSIGReferenceList *refList = m_sig->getReferenceList();
				if (refList == NULL)
					return FALSE;

				DSIGReferenceList::size_type i, refSize;
				refSize = refList->getSize();
				for (i = 0; i < refSize; i++)
				{
					DSIGReference *ref = refList->item(i);

					// get digest
					if (lType == 2)
					{
						XMLByte byHash[128];
						memset(byHash, 0, sizeof(byHash));
						unsigned int nRead = ref->readHash(byHash, 128);
						if (nRead > 0)
						{
							char szHashB64[256];
							memset(szHashB64, 0, sizeof(szHashB64));
							long lHashB64Len = CMailCoder::base64_encode((char *)byHash, nRead, szHashB64);
							if (lHashB64Len > 0)
							{
								strInfo = szHashB64;
								bFound = TRUE;
								break;
							}
						}
					}
					// get digest method
					else
					{
						hashMethod hashAlgo = ref->getHashMethod();
						if (hashAlgo == HASH_SHA1)
							strInfo = "SHA1";
						else if (hashAlgo == HASH_MD5)
							strInfo = "MD5";
						else if (hashAlgo == HASH_SHA224)
							strInfo = "SHA224";
						else if (hashAlgo == HASH_SHA256)
							strInfo = "SHA256";
						else if (hashAlgo == HASH_SHA384)
							strInfo = "SHA384";
						else if (hashAlgo == HASH_SHA512)
							strInfo = "SHA512";
						else if (hashAlgo == HASH_SM3)
							strInfo = "SM3";
						else
							strInfo = "Undefined";

						bFound = TRUE;
						break;
					}
				}

				if (!bFound)
					return FALSE;
			}
			else if (lType == 3)
			{
				// get sign value
				const XMLCh *ustrSign = m_sig->getSignatureValue();
				if (ustrSign == NULL)
					return FALSE;
				strInfo = A(ustrSign);
			}
			else if (lType == 4)
			{
				// get sign cert
				DSIGKeyInfoList *kinfList = m_sig->getKeyInfoList();
				if (kinfList == NULL)
					return FALSE;

				DSIGKeyInfoList::size_type i, kinfSize;
				kinfSize = kinfList->getSize();
				for (i = 0; i < kinfSize; i++)
				{
					DSIGKeyInfo *kinf = kinfList->item(i);
					if (kinf->getKeyInfoType() == DSIGKeyInfo::KEYINFO_X509)
					{
						DSIGKeyInfoX509 *kinfX509 = (DSIGKeyInfoX509 *)kinf;
						int nCertListSize = kinfX509->getCertificateListSize();
						if (nCertListSize > 0)
						{
							const XMLCh *ustrCert = kinfX509->getCertificateItem(0);
							if (ustrCert != NULL)
							{
								strInfo = A(ustrCert);
								bFound = TRUE;
								break;
							}
						}
					}
				}

				if (!bFound)
					return FALSE;
			}
/*			else if (lType == 5)
			{
				// get hash method
				hashMethod hashAlgo = m_sig->getHashMethod();
				if (hashAlgo == HASH_SHA1)
					strInfo = "SHA1";
				else if (hashAlgo == HASH_MD5)
					strInfo = "MD5";
				else if (hashAlgo == HASH_SHA224)
					strInfo = "SHA224";
				else if (hashAlgo == HASH_SHA256)
					strInfo = "SHA256";
				else if (hashAlgo == HASH_SHA384)
					strInfo = "SHA384";
				else if (hashAlgo == HASH_SHA512)
					strInfo = "SHA512";
				else
					strInfo = "No method defined";
			}
*/			else
			{
				// get sign method
/*				signatureMethod signAlgo = m_sig->getSignatureMethod();
				if (signAlgo == SIGNATURE_DSA)
					strInfo = "DSA";
				else if (signAlgo == SIGNATURE_HMAC)
					strInfo = "Hash MAC";
				else if (signAlgo == SIGNATURE_RSA)
					strInfo = "RSA";
				else
					strInfo = "No method defined";
*/	
				DOMElement *node1 = getFirstChild(getRoot(), "Signature");
				if (node1 == NULL)
				{
					node1 = getFirstChild(getRoot(), "ds:Signature");
					if (node1 == NULL)
						return FALSE;
				}
				DOMElement *node2 = getFirstChild(node1, "SignedInfo");
				if (node2 == NULL)
				{
					node2 = getFirstChild(node1, "ds:SignedInfo");
					if (node2 == NULL)
						return FALSE;
				}
				node1 = getFirstChild(node2, "SignatureMethod");
				if (node1 == NULL)
				{
					node1 = getFirstChild(node2, "ds:SignatureMethod");
					if (node1 == NULL)
						return FALSE;
				}

				std::string strAlgo = "";
				getAttribute(node1, "Algorithm", strAlgo);
				if (strAlgo == URI_ID_DSA_SHA1)
					strInfo = "SHA1_DSA";
				else if (strAlgo == URI_ID_HMAC_SHA1)
					strInfo = "SHA1_HMAC";
				else if (strAlgo == URI_ID_HMAC_SHA224)
					strInfo = "SHA224_HMAC";
				else if (strAlgo == URI_ID_HMAC_SHA256)
					strInfo = "SHA256_HMAC";
				else if (strAlgo == URI_ID_HMAC_SHA384)
					strInfo = "SHA384_HMAC";
				else if (strAlgo == URI_ID_HMAC_SHA512)
					strInfo = "SHA512_HMAC";
				else if (strAlgo == URI_ID_RSA_SHA1)
					strInfo = "SHA1_RSA";
				else if (strAlgo == URI_ID_RSA_SHA224)
					strInfo = "SHA224_RSA";
				else if (strAlgo == URI_ID_RSA_SHA256)
					strInfo = "SHA256_RSA";
				else if (strAlgo == URI_ID_RSA_SHA384)
					strInfo = "SHA384_RSA";
				else if (strAlgo == URI_ID_RSA_SHA512)
					strInfo = "SHA512_RSA";
				else if (strAlgo == URI_ID_RSA_MD5)
					strInfo = "MD5_RSA";
				else if (strAlgo == URI_ID_SM2_SM3)
					strInfo = "SM3_SM2";
				else
					strInfo = "Undefined";
			}

			bRet = TRUE;
		}
		catch (XSECException &e)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during getting signature information\nError Type = %d, Error Message = %s",
				e.getType(), e.getMsg());
		}
		catch (XSECCryptoException &e2)
		{
			char szMsg[512];
			sprintf(szMsg, "Error occured during getting signature information\nError Type = %d, Error Message = %s",
				e2.getType(), e2.getMsg());
		}

		return bRet;
	}

private:
	// = reference.
//	static long reference_;

	XSECProvider m_prov;
	DSIGSignature *m_sig;
	FUNC_PRIV_ENC m_funcold;
};

//long MyXmlSign::reference_ = 0;

#endif  // __MYXMLSIGN_H__  
