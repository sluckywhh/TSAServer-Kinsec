#ifndef _PKCS10_H_
#define _PKCS10_H_

/**
  * @Name: Pkcs#10.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/09 11:01
  * @Description: all pkcs#10 class define 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./pkcs1.h"

class KT_LIB_MODE KTSubjectPublicKeyInfo : public KTSEQUENCE2
{
public:
	KTSubjectPublicKeyInfo()
	{
		PLACE2OBJS(m_algorithm, m_subjectPublicKey)
	}
	
	int  setRSAPublicKey(const KTRSAPublicKey& publicKey);
	int  getRSAPublicKey(KTRSAPublicKey& publicKey) const;
    void  setRSAPublicKey(const KTBYTE* pPubKey, KTUINT nLen);
    void  getRSAPublicKey(KTByteArray& rPubKey) const;

	KTBOOL IsSM2PublicKey()const;

	int  setSM2PublicKey(const KTSM2PublicKey& publicKey);
	int  getSM2PublicKey(KTSM2PublicKey& publicKey) const;
    void  setSM2PublicKey(const KTBYTE* pPubKey, KTUINT nLen);
    void  getSM2PublicKey(KTByteArray& rPubKey) const;

	KTAlgorithmIdentifier	m_algorithm;
	KTBITSTRING				m_subjectPublicKey;
};

///////////////////////////////////////////////////////////////////////////////
//KTenrolmentCSP
//# CAPI cert enrolment CSP, contains a BMPString describing the CAPI level and
//# a BIT STRING blob containing a key spec

class KT_LIB_MODE KTenrolmentCSP : public KTSEQUENCE3
{
public:
	KTenrolmentCSP(){ 
		PLACE3OBJS(m_version, m_level, m_spec);
	}

	KTINTEGER		m_version;
	KTBMPString		m_level;
	KTBITSTRING		m_spec;
};

//# Windows OS version
typedef KTIA5String KTosVersion;

// KTAttribute
SETOF(KTOCTETSTRINGSet, KTOCTETSTRING);
SETOF(KTBMPStringSet, KTBMPString);
SETOF(KTBITSTRINGSet, KTBITSTRING);
SETOF(KTIA5StringSet, KTIA5String);
SETOF(KTenrolmentCSPSet, KTenrolmentCSP);

/*Attribute       ::=     SEQUENCE {
        type            ATTRIBUTE.&id ({SupportedAttributes}),
        values  SET SIZE (1 .. MAX) OF ATTRIBUTE.&Type
                        ({SupportedAttributes}{@type})}*/
class KT_LIB_MODE KTAttribute : public KTTwin
{
protected:
	KTObject* newObj(const char* pc_typeId) const;
};

SETOF(KTAttributes, KTAttribute);

class KT_LIB_MODE KTAttributeTypeAndValue : public KTAttribute
{
public:
	int setTypeValue(const char* pc_attriId, const char* pc_attrValue, UINT unLen);
	int getValue(KTString& strAttribute, KTBOOL* pbBMP=NULL) const;
protected:
	KTObject* newObj(const char* pc_typeId) const;
};
///////////////////////////////////////////////////////////////////////////////
// KTRelativeDistinguishedName
SETOF(KTRelativeDistinguishedName, KTAttributeTypeAndValue)

///////////////////////////////////////////////////////////////////////////////
// KTRDNSequence 
SEQUENCEOF(KTRDNSequence0, KTRelativeDistinguishedName)
class KT_LIB_MODE KTRDNSequence : public KTRDNSequence0
{
public:
    int addItem(const char* szObjID, const char* pbValue, UINT unBytes);
    int getItem(const char* szObjID, KTString& rValue) const;
    KTBOOL operator == (const KTRDNSequence& otherRDNSequence)	const;
};

typedef KTRDNSequence KTDistinguishedName;

class KT_LIB_MODE KTName : public KTCHOICE1
{
public:
    KTName()
    {
        PLACE1OBJ(m_rdnSequence);
		OptionPlacedObjs(1);
		ChoiceFirstPlacedObj();
    }

    int addItem(const char* szObjID, const char* pbValue, UINT unBytes);
    int getItem(const char* szObjID, KTString& rValue) const;
    KTBOOL operator == (const KTName& otherName) const;

    KTRDNSequence   m_rdnSequence;
}; 

class KT_LIB_MODE KTCertificationRequestInfo : public KTSEQUENCE4
{
public:
	KTCertificationRequestInfo()
	{
		PLACE4OBJS(m_version, m_subject, m_subjectPKInfo, m_attributes)
		m_version = 0;			// CertificationRequestInfo Version 1 (0)
		m_attributes.setTypeEx(0, FALSE);
	}

	KTINTEGER				m_version;
	KTName					m_subject;
	KTSubjectPublicKeyInfo	m_subjectPKInfo;
	KTAttributes			m_attributes;				// IMPLICIT 0
};

///////////////////////////////////////////////////////////////////////////////
// KTToBeSigned
class KT_LIB_MODE KTToBeSigned : public KTSEQUENCE3
{
public:
	KTToBeSigned()
	{
		m_ppObj[0] = NULL;
		m_ppObj[1] = &m_signatureAlgorithm;
		m_ppObj[2] = &m_signature;
		m_pToBeSigned = NULL;
	}
	void attachToBeSignedObj(const KTObject* pObjToBeSigned)
	{
		m_ppObj[0] = m_pToBeSigned = (KTObject*) pObjToBeSigned;
	};

	KTObject*				m_pToBeSigned;
	KTAlgorithmIdentifier	m_signatureAlgorithm;
	KTBITSTRING				m_signature;
};

class KT_LIB_MODE KTCertificationRequest : public KTToBeSigned
{
public:
	KTCertificationRequest()
	{
        attachToBeSignedObj(&m_certificationRequestInfo);
    }
	
	KTCertificationRequestInfo  m_certificationRequestInfo;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/09 11:40   Hongwei Yu   created 

#endif //_PKCS#10_H_
