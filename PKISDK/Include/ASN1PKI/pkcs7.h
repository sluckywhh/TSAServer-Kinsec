#ifndef __PKCS7_H__
#define __PKCS7_H__

/**
  * @Name: Pkcs#7.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/09 11:42
  * @Description: all pkcs#7 class define 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./pkcs1.h"
#include "./X.509.h"

typedef KTAlgorithmIdentifier   KTContentEncryptionAlgorithmIdentifier;   //{{ContentEncryptionAlgorithms}}

typedef KTAlgorithmIdentifier   KTDigestAlgorithmIdentifier; //{{DigestAlgorithms}}

typedef KTAlgorithmIdentifier   KTDigestEncryptionAlgorithmIdentifier; //{{DigestEncryptionAlgorithms}}

///KTExtendedCertificate in pkcs#6
typedef KTCertificate  KTExtendedCertificate;

class KT_LIB_MODE KTExtendedCertificateOrCertificate: public KTCHOICE2
{
public:
    KTExtendedCertificateOrCertificate()
    {
        PLACE2OBJS(m_certificate, m_extendedCertificate);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
		m_extendedCertificate.setTypeEx(0, FALSE);
    }

    KTCertificate           m_certificate;
    KTExtendedCertificate   m_extendedCertificate;
};

SETOF(KTExtendedCertificatesAndCertificates, KTExtendedCertificateOrCertificate)

class KT_LIB_MODE KTIssuerAndSerialNumber : public KTSEQUENCE2
{
public:
	KTIssuerAndSerialNumber()
	{
		PLACE2OBJS(m_issuer, m_serialNumber)
	}

	KTName  m_issuer;
	KTCertificateSerialNumber m_serialNumber;
};

typedef KTAlgorithmIdentifier   KTKeyEncryptionAlgorithmIdentifier;     //{{KeyEncryptionAlgorithms}}

/*
Contents CONTENTS ::= {
  {Data                    IDENTIFIED BY data}                   |
  {SignedData              IDENTIFIED BY signedData}             |
  {EnvelopedData           IDENTIFIED BY envelopedData}          |
  {SignedAndEnvelopedData  IDENTIFIED BY signedAndEnvelopedData} |
  {DigestedData            IDENTIFIED BY digestedData}           |
  {EncryptedData           IDENTIFIED BY encryptedData},
  ...  -- add any application-specific types/contents here
}*/
class KT_LIB_MODE KTContentInfo : public KTTwin
{	
protected:
	//the value is optional and explicit [0]
	KTObject* newObj(const char* pc_typeId) const;
};

typedef KTOCTETSTRING   KTData; 

typedef KTOCTETSTRING   KTDigest;

typedef KTDigest KTMessageDigest;

//////////////////////////////////////////////////////////////////////////////
// KTEncryptedDigest
typedef KTOCTETSTRING KTEncryptedDigest;

/*
Authenticated ATTRIBUTE ::= {
  contentType |
  messageDigest,
  ...,  -- add application-specific attributes here
  signingTime
}*/

SETOF(KTAttributeSet, KTAttribute)
SEQUENCEOF(KTAttributeSequence, KTAttribute)

class KT_LIB_MODE KTAuthenticatedAttributes : public KTCHOICE2 {
public:
    KTAuthenticatedAttributes()
    {
        PLACE2OBJS(m_aaSet, m_aaSequence);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_aaSet.setTypeEx(0, FALSE);
        m_aaSequence.setTypeEx(2, TRUE);
    }

    KTAttributeSet          m_aaSet;         //[0] IMPLICIT  {{Authenticated}},
    KTAttributeSequence     m_aaSequence;    //[2] EXPLICIT  {{Authenticated}
};

/*
Unauthenticated ATTRIBUTE ::= {
  ...,  -- add application-specific attributes here
  counterSignature
}*/
    
class KT_LIB_MODE KTUnauthenticatedAttributes : public KTCHOICE2 
{
public:
    KTUnauthenticatedAttributes()
    {
        PLACE2OBJS(m_uaSet, m_uaSequence);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_uaSet.setTypeEx(1, FALSE);
        m_uaSequence.setTypeEx(3, FALSE);
    }
    KTAttributeSet          m_uaSet;         //[1] IMPLICIT SET OF Attribute {{Unauthenticated}},
    KTAttributeSequence     m_uaSequence;    //[3] IMPLICIT SEQUENCE OF Attribute {{Unauthenticated}
};

//////////////////////////////////////////////////////////////////////////////
// KTSignerInfo

class KT_LIB_MODE KTSignerInfo : public KTSEQUENCE7
{
public:
	KTSignerInfo()
	{
		m_version = 1; 
		m_authenticatedAttributes.setOptional();
		m_unauthenticatedAttributes.setOptional();

		PLACE7OBJS(m_version, m_issuerAndSerialNumber, m_digestAlgorithm, m_authenticatedAttributes,
			m_digestEncryptionAlgorithm, m_encryptedDigest,	m_unauthenticatedAttributes)
	}

	KTINTEGER				    m_version;							    
	KTIssuerAndSerialNumber	    m_issuerAndSerialNumber;
	KTAlgorithmIdentifier	    m_digestAlgorithm;
	KTAuthenticatedAttributes   m_authenticatedAttributes;				// OPTIONAL
	KTAlgorithmIdentifier	    m_digestEncryptionAlgorithm;
	KTEncryptedDigest		    m_encryptedDigest;
	KTUnauthenticatedAttributes	m_unauthenticatedAttributes;			//OPTIONAL

};

////////////////////////////////////////////////////////////////////////////
// KTSignerInfos

SEQUENCEOF(KTSignerInfoSequence, KTSignerInfo)
SETOF(KTSignerInfoSet, KTSignerInfo)

class KT_LIB_MODE KTSignerInfos: public KTCHOICE2
{
public:
    KTSignerInfos()
    {
        PLACE2OBJS(m_siSet, m_siSequence);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
    }

    KTSignerInfoSet         m_siSet;
    KTSignerInfoSequence    m_siSequence;
};

/////////////////////////////////////////////////////////////////////////////////////////////
// KTRecipientInfo

typedef KTOCTETSTRING		KTEncryptedKey;

class KT_LIB_MODE KTRecipientInfo: public KTSEQUENCE4
{
public:
	KTRecipientInfo()
	{
		m_version = 0;
		m_keyEncryptionAlgorithm.setTypeValue(ObjID_id_RSAES_OAEP);
		PLACE4OBJS(m_version, m_issuerAndSerialNumber, m_keyEncryptionAlgorithm, m_encryptedKey)
	}

	KTINTEGER					            m_version;
	KTIssuerAndSerialNumber		            m_issuerAndSerialNumber;
	KTKeyEncryptionAlgorithmIdentifier		m_keyEncryptionAlgorithm;
	KTEncryptedKey				            m_encryptedKey;
};

///////////////////////////////////////////////////////////////////////////////
// KTCertificates
SEQUENCEOF(KTCertificates0, KTCertificate)

class KT_LIB_MODE KTCertificates : public KTCertificates0
{
public:
	const KTCertificate* matchCert(const KTRSAPrivateKey	&privateKey) const; 
};

/////////////////////////////////////////////////////////////////////////////////////////////
// KTRecipientInfos
SEQUENCEOF(KTRecipientInfoSequence, KTRecipientInfo)
SETOF(KTRecipientInfoSet, KTRecipientInfo)

class KT_LIB_MODE KTRecipientInfos : public KTCHOICE2
{
public:
    KTRecipientInfos()
    {
        PLACE2OBJS(m_riSet, m_riSequence);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
    }

	//KTCertificate* retrieveRecipientCert(const KTCertificates& certChain);

    KTRecipientInfoSet      m_riSet;
    KTRecipientInfoSequence m_riSequence;
};

/////////////////////////////////////////////////////////////////////////////////////////////
// KTEncryptedContent
typedef KTOCTETSTRING	KTEncryptedContent;

///////////////////////////////////////////////////////////////////////////////
// KTDigestAlgorithmIdentifiers
SEQUENCEOF(KTDigestAlgorithmIdentifierSequence, KTAlgorithmIdentifier)
SETOF(KTDigestAlgorithmIdentifierSet, KTAlgorithmIdentifier)

class KT_LIB_MODE KTDigestAlgorithmIdentifiers : public KTCHOICE2
{
public:
    KTDigestAlgorithmIdentifiers()
    {
        PLACE2OBJS(m_daSet, m_daSequence);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
    }

	int attachDigestAlgo(const char* pc_digestAlgoID, const KTObject* pObj=NULL); 		

    KTDigestAlgorithmIdentifierSet             m_daSet;
    KTDigestAlgorithmIdentifierSequence        m_daSequence;
};

class KT_LIB_MODE KTCertificatesChoice: public KTCHOICE2
{
public:    
    KTCertificatesChoice()
    {
        PLACE2OBJS(m_certSet, m_certSequence);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_certSet.setTypeEx(0, FALSE);
        m_certSequence.setTypeEx(2, FALSE);
    }

    KTExtendedCertificatesAndCertificates   m_certSet;
    KTCertificates                          m_certSequence;
};

///////////////////////////////////////////////////////////////////////////////////////
SETOF(KTCertificateRevocationLists, KTCertificateList)
SEQUENCEOF(KTCRLSequence, KTCertificateList)

class KT_LIB_MODE KTCRLsChoice: public KTCHOICE2
{
public:    
    KTCRLsChoice()
    {
        PLACE2OBJS(m_crlSet, m_crlSequence);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_crlSet.setTypeEx(1, FALSE);
        m_crlSequence.setTypeEx(3, FALSE);
    }

    KTCertificateRevocationLists   m_crlSet;
    KTCRLSequence                  m_crlSequence;
};

///////////////////////////////////////////////////////////////////////////////
// KTSignedData
class KT_LIB_MODE KTSignedData: public KTSEQUENCE6
{
public:
	KTSignedData()
	{	
		m_version = 2;
		m_certificates.setOptional();
        m_crls.setOptional();

		PLACE6OBJS(m_version, m_digestAlgorithms, m_contentInfo, 
			m_certificates,	m_crls,	m_signerInfos)
	}

	KTINTEGER		                m_version;
	KTDigestAlgorithmIdentifiers	m_digestAlgorithms;
	KTContentInfo	                m_contentInfo;
	KTCertificatesChoice	        m_certificates;		
	KTCRLsChoice	                m_crls;				     
	KTSignerInfos 	                m_signerInfos;		
};

//CBCParameter ::= IV
//IV ::= OCTET STRING  -- exactly 8 octets
typedef KTOCTETSTRING KTCBCParameter;
      
/////////////////////////////////////////////////////////////////////////////////////////////
// KTEncryptedcontentInfo
class KT_LIB_MODE KTEncryptedContentInfo : public KTSEQUENCE3
{
public:
	KTEncryptedContentInfo()
	{
		m_encryptedContent.setTypeEx(0, FALSE);
		m_encryptedContent.setOptional();

		PLACE3OBJS(m_contentType, m_contentEncryptionAlgorithm,	m_encryptedContent)
	}

	void setValue(const char* const pc_contentTypeID, const KTOCTETSTRING& content,
				 const char* const pc_algorithmID, const KTCBCParameter& parameter);

	KTContentType			                m_contentType;
	KTContentEncryptionAlgorithmIdentifier	m_contentEncryptionAlgorithm;
	KTEncryptedContent		                m_encryptedContent;				//IMPLICIT 0 OPTIONAL

};

/////////////////////////////////////////////////////////////////////////////////////////////
// KTEnvelopedData
class KT_LIB_MODE KTEnvelopedData : public KTSEQUENCE3
{
public:
	KTEnvelopedData()
	{		
		m_version = 1;
		PLACE3OBJS(m_version, m_recipientInfos,	m_encryptedContentInfo)
	}

	KTINTEGER				m_version;
	KTRecipientInfos		m_recipientInfos;
	KTEncryptedContentInfo	m_encryptedContentInfo;
};

class KT_LIB_MODE KTSignedAndEnvelopedData : public KTSEQUENCE7 {
public:
    KTSignedAndEnvelopedData()
    {
        PLACE7OBJS(m_version, m_recipientInfos, m_digestAlgorithms, m_encryptedContentInfo,
            m_certificates, m_crls, m_signerInfos)
        m_certificates.setOptional();
        m_crls.setOptional();
    }

  KTINTEGER                     m_version;         //{seVer1(1), seVer2(2)} (seVer1 | seVer2),
  KTRecipientInfos              m_recipientInfos;  
  KTDigestAlgorithmIdentifiers  m_digestAlgorithms;
  KTEncryptedContentInfo        m_encryptedContentInfo;
  KTCertificatesChoice          m_certificates;     //OPTIONAL,
  KTCRLsChoice                  m_crls;             //OPTIONAL,
  KTSignerInfos                 m_signerInfos;     
};

///////////////////////////////////////////////////////////////////////////////
//	KTDigestedData
class KT_LIB_MODE KTDigestedData: public KTSEQUENCE4
{
public:
	KTDigestedData()
	{		
		m_version = 0;
		PLACE4OBJS(m_version, m_digestAlgorithm, m_contentInfo, m_digest)
	}

	KTINTEGER				    m_version;
	KTDigestAlgorithmIdentifier	m_digestAlgorithm;
	KTContentInfo			    m_contentInfo;
	KTDigest				    m_digest;
};

////in SetPKCS7Plus
///////////////////////////////////////////////////////////////////////////////
// KTDetachedDigest
// when the id of contenttype is supported by DigestedData, content can present or absent
// when the id of contenttype is not supported by DigestedData,( for Policy Extension), 
// content can absent or present with taged value is SNull
// above note is according gte-rca.der
typedef KTDigestedData KTDetachedDigest;



// -----------------------------------------------------------------------------
//  附加部分：
//      2005-06-23  林灶钢  添加以下代码
//      2005-10-09  林灶钢  修改以下代码
//  功能：
//      使 PKCS7 支持时间戳功能。
// -----------------------------------------------------------------------------
// =================================================================================
//  File    : TSP.hpp
//  Purpose : 时间戳协议基本类型定义
//  Author  : 林灶钢
//  Created : 2005-10-09
//  CopyRight (C) Fujian Kinsec
// =================================================================================

#ifndef __TSP_HPP__
#define __TSP_HPP__

// =================================================================================
//  Include Header Files

// =================================================================================
//  Class Definition

/*
id-ct-TSTInfo  OBJECT IDENTIFIER ::=  { 
    iso(1) member-body(2) us(840) rsadsi(113549) pkcs(1) pkcs-9(9) smime(16) ct(1) 4
}
*/
#ifndef ObjID_id_ct_TSTInfo
#define ObjID_id_ct_TSTInfo      "1 2 840 113549 1 9 16 1 4"
#endif


typedef KTObjectIdentifier KTTSAPolicyId;

/*
MessageImprint ::= SEQUENCE  {
    hashAlgorithm                 AlgorithmIdentifier, 
	hashedMessage                 OCTET STRING
}
*/
class KT_LIB_MODE KTMessageImprint : public KTSEQUENCE2
{
public :
    KTMessageImprint()
    {
        PLACE2OBJS( m_hashAlgorithm, m_hashedMessage )
    }

	KTAlgorithmIdentifier m_hashAlgorithm;
	KTMessageDigest		  m_hashedMessage;	
};

/*
TimeStampReq ::= SEQUENCE  {
    version                     INTEGER  { v1(1) },
	messageImprint              MessageImprint,
	    -- a hash algorithm OID and the hash value of the data to be
		-- time-stamped
	reqPolicy                   TSAPolicyId                OPTIONAL,
	nonce                       INTEGER                    OPTIONAL,
	certReq                     BOOLEAN                    DEFAULT FALSE,
	extensions                  [0] IMPLICIT Extensions    OPTIONAL
}
*/
class KT_LIB_MODE KTTimeStampReq : public KTSEQUENCE6
{
public:
	KTTimeStampReq()
	{
		m_version = 1;
		m_certReq = FALSE;

		m_reqPolicy.setOptional();
		m_nonce.setOptional();
		m_extensions.setOptional();

		PLACE6OBJS( m_version, m_messageImprint, m_reqPolicy, m_nonce, m_certReq, 
			m_extensions )
	}

	KTINTEGER                 m_version;
	KTMessageImprint          m_messageImprint;
	KTTSAPolicyId             m_reqPolicy;           // OPTIONAL
	KTINTEGER                 m_nonce;               // OPTIONAL
	KTBOOLEAN                 m_certReq;
	KTExtensions              m_extensions;          // OPTIONAL
};

/*
Accuracy ::= SEQUENCE {
    seconds                INTEGER               OPTIONAL,
	millis             [0] INTEGER  (1..999)     OPTIONAL,
	micros             [1] INTEGER  (1..999)     OPTIONAL 
}
*/
class KT_LIB_MODE KTAccuracy : public KTSEQUENCE3
{
public :
    KTAccuracy()
    {
        m_seconds.setOptional();
        m_millis.setOptional();
        m_micros.setOptional();

        PLACE3OBJS( m_seconds, m_millis, m_micros )
    }

    KTINTEGER              m_seconds;        // OPTIONAL               - 秒
    KTINTEGER              m_millis;         // [0] OPTIONAL  1..999   - 毫秒
    KTINTEGER              m_micros;         // [1] OPTIONAL  1..999   - 微秒
};

/*
TSTInfo ::= SEQUENCE  {
    version                INTEGER    { v1(1) },
	policy                 TSAPolicyId, 
	messageImprint         MessageImprint, 
	    -- MUST have the same value as the similar field in 
		-- TimeStampReq
	serialNumber           INTEGER,
	    -- Time-Stamping users MUST be ready to accommodate integers
		-- up to 160 bits.
	genTime                 GeneralizedTime,
	accuracy                Accuracy                  OPTIONAL,
	ordering                BOOLEAN                   DEFAULT FALSE,
	nonce                   INTEGER                   OPTIONAL,
	    -- MUST be present if the similar field was present
		-- in TimeStampReq.  In that case it MUST have the same value.
	tsa                 [0] GeneralName               OPTIONAL,
	extensions          [1] IMPLICIT Extensions       OPTIONAL
}
*/
class KT_LIB_MODE KTTSTInfo : public KTSEQUENCE10
{
public :
	KTTSTInfo()
	{
		m_version = 1;
		m_ordering = FALSE;

		m_accuracy.setOptional();
		m_nonce.setOptional();
		m_tsa.setOptional();
		m_extensions.setOptional();

		PLACE10OBJS( m_version, m_policy, m_messageImprint, m_serialNumber,
			m_genTime, m_accuracy, m_ordering, m_nonce, m_tsa, m_extensions )
	}

	KTINTEGER               m_version;
	KTTSAPolicyId           m_policy;
	KTMessageImprint        m_messageImprint;
	KTINTEGER               m_serialNumber;
	KTGeneralizedTime       m_genTime;
	KTAccuracy              m_accuracy;           // OPTIONAL
	KTBOOLEAN               m_ordering;           // DEFAULT FALSE
	KTINTEGER               m_nonce;              // OPTIONAL
	KTGeneralName           m_tsa;                // OPTIONAL
	KTExtensions            m_extensions;         // IMPLICIT OPTIONAL
};

/*
PKIStatusInfo ::= SEQUENCE  {
    status                       PKIStatus, 
	statusString                 PKIFreeText          OPTIONAL,
	failInfo                     PKIFailureInfo       OPTIONAL
}

From RFC 1510  -  section 3.1.1:
PKIFreeText :: SEQUENCE SIZE (1..MAX) OF UTF8String
    -- text encoded as UTF-8 String (note: each UTF8String SHOULD 
	-- include an RFC 1766 language tag to indicate the language
	-- of the contained text)
*/
/* 在 include\ASN1PKI\cmd.h 中定义

// Possible values for status. 
#define	TS_STATUS_GRANTED		    	    0
#define	TS_STATUS_GRANTED_WITH_MODS		    1
#define	TS_STATUS_REJECTION			        2
#define	TS_STATUS_WAITING			        3
#define	TS_STATUS_REVOCATION_WARNING		4
#define	TS_STATUS_REVOCATION_NOTIFICATION	5

// Possible values for failure_info. 
#define	TS_INFO_BAD_ALG		            	0
#define	TS_INFO_BAD_REQUEST	        	    2
#define	TS_INFO_BAD_DATA_FORMAT	    	    5
#define	TS_INFO_TIME_NOT_AVAILABLE   	    14
#define	TS_INFO_UNACCEPTED_POLICY   	    15
#define	TS_INFO_UNACCEPTED_EXTENSION	    16
#define	TS_INFO_ADD_INFO_NOT_AVAILABLE	    17
#define	TS_INFO_SYSTEM_FAILURE		        25

typedef KTINTEGER   KTPKIStatus;
typedef KTBITSTRING KTPKIFailureInfo;

SETOF(KTPKIFreeTextSet, KTUTF8String)
SEQUENCEOF(KTPKIFreeTextSequence, KTUTF8String)

class KT_LIB_MODE KTPKIFreeText : public KTCHOICE2 
{
public :
	KTPKIFreeText()
	{
		PLACE2OBJS( m_aaSet, m_aaSequence );
		OptionPlacedObjs( 2 );
		ChoiceFirstPlacedObj();
		m_aaSet.setTypeEx( 0, FALSE );
		m_aaSequence.setTypeEx( 2, TRUE );
	}

	KTPKIFreeTextSet        m_aaSet;         //[0] IMPLICIT  {{Authenticated}},
	KTPKIFreeTextSequence   m_aaSequence;    //[2] EXPLICIT  {{Authenticated}
};

class KT_LIB_MODE KTPKIStatusInfo : public KTSEQUENCE10
{
public :
	KTPKIStatusInfo()
	{
		m_statusString.setOptional();
		m_failInfo.setOptional();

		PLACE3OBJS( m_status, m_statusString, m_failInfo )
	}

	KTPKIStatus             m_status;
	KTPKIFreeText           m_statusString;           // OPTIONAL
	KTPKIFailureInfo        m_failInfo;               // OPTIONAL
};
*/

/*
TimeStampToken ::= ContentInfo 
     -- contentType is id-signedData ([CMS])
     -- content is SignedData ([CMS])
*/
typedef KTContentInfo KTTimeStampToken;

/*
TimeStampResp ::= SEQUENCE  {
    status                       PKIStatusInfo,
	timeStampToken               TimeStampToken       OPTIONAL
}
*/
/*
class KT_LIB_MODE KTTimeStampResp : public KTSEQUENCE2
{
public :
	KTTimeStampResp()
	{
		m_timeStampToken.setOptional();

		PLACE2OBJS( m_status, m_timeStampToken )
	}

	KTPKIStatusInfo         m_status;
	KTTimeStampToken        m_timeStampToken;        // OPTIONAL
};
*/

/*
IssuerSerial ::= SEQUENCE  {
    issuer                      GeneralNames, 
	serialNumber                CertificateSerialNumber
}
*/
class KT_LIB_MODE KTIssuerSerial : public KTSEQUENCE2
{
public :
	KTIssuerSerial()
	{
		PLACE2OBJS( m_issuer, m_serialNumber )
	}

	KTName                     m_issuer;
	KTCertificateSerialNumber  m_serialNumber;
};

/*
ESSCertID ::= SEQUENCE  {
    certHash                 Hash,
	issuerSerial             IssuerSerial           OPTIONAL
}
*/
class KT_LIB_MODE KTESSCertID : public KTSEQUENCE2
{
public :
	KTESSCertID()
	{
		PLACE2OBJS( m_certHash, m_issuerSerial )
	}

    KTOCTETSTRING     m_certHash;           // Always SHA-1 digest
	KTIssuerSerial    m_issuerSerial;
};

SETOF(KTESSCertIDSet, KTESSCertID)
SEQUENCEOF(KTESSCertIDSequence, KTESSCertID)

class KT_LIB_MODE KTESSCertIDs : public KTCHOICE2
{
public :
	KTESSCertIDs()
	{
		PLACE2OBJS( m_aaSet, m_aaSequence );
		OptionPlacedObjs( 2 );
		ChoiceFirstPlacedObj();
		m_aaSet.setTypeEx( 0, FALSE );
		m_aaSequence.setTypeEx( 2, TRUE );
	}

	KTESSCertIDSet        m_aaSet;         //[0] IMPLICIT  {{Authenticated}},
	KTESSCertIDSequence   m_aaSequence;    //[2] EXPLICIT  {{Authenticated}
};

/*
SigningCertificate ::= SEQUENCE  {
    certs                             SEQUENCE OF ESSCertID,
	policies                          SEQUENCE OF PolicyInformation OPTIONAL
}
*/
class KT_LIB_MODE KTESSSigningCertificate : public KTSEQUENCE2
{
public :
	KTESSSigningCertificate()
	{
		m_policies.setOptional();

		PLACE2OBJS( m_certs, m_policies )
	}

	KTESSCertIDs                   m_certs;
	KTCertificatePolicies          m_policies;
};

// =================================================================================
//  1.00    2005-10-09    Created By LinZaoGang
//  File End 
#endif   

#ifndef ObjID_timeStampData
#define ObjID_timeStampData                       ObjID_pkcs_7" 7"
#endif
#ifndef ObjID_signedDataAndTimeStamp
#define ObjID_signedDataAndTimeStamp              ObjID_pkcs_7" 8"
#endif
#ifndef ObjID_signedAndEnvelopedDataAndTimeStamp
#define ObjID_signedAndEnvelopedDataAndTimeStamp  ObjID_pkcs_7" 9"
#endif

// -----------------------------------------------------------------------
//  Data structure definition
// -----------------------------------------------------------------------

/*************************************************************************
 Class   : KTTimeStampData
 Purpose : 时间戳内容数据结构
 Members : m_version           - 版本
           m_digestAlgorithms  - 摘要算法
           m_TSTInfo           - 封装信息值
           m_certificates      - 签名证书
           m_crls              - 
           m_signerInfos       - 签名者信息
*************************************************************************/
class KT_LIB_MODE KTTimeStampData : public KTSEQUENCE6
{
public:
	KTTimeStampData()
	{	
        m_version = 1;
        m_certificates.setOptional();
        m_crls.setOptional();

		PLACE6OBJS( m_version, m_digestAlgorithms, m_TSTInfo, 
            m_certificates, m_crls, m_signerInfos )
	}

	KTINTEGER		                m_version;
	KTDigestAlgorithmIdentifiers	m_digestAlgorithms;
	KTContentInfo	                m_TSTInfo;	//存放封装内容。与KTSignedData不同。
    KTCertificatesChoice	        m_certificates;		
	KTCRLsChoice	                m_crls;				     
	KTSignerInfos 	                m_signerInfos;		
};


/*************************************************************************
 Class   : KTSignedDataAndTimeStamp
 Purpose : 数字签名（含时间戳）
 Members : m_signedData    - 签名数据
           m_timeStamp     - 时间戳数据
*************************************************************************/
class KT_LIB_MODE KTSignedDataAndTimeStamp : public KTSEQUENCE2
{
public :
	KTSignedDataAndTimeStamp()
	{	
        PLACE2OBJS( m_signedData, m_timeStamp )
	}

	KTContentInfo	                m_signedData;
	KTContentInfo	                m_timeStamp;
};

/*************************************************************************
 Class   : KTSignedAndEnvelopedDataAndTimeStamp
 Purpose : 数字签名并数字信封（含时间戳）
 Members : m_signedData    - 签名数据
           m_timeStamp     - 时间戳数据
*************************************************************************/
class KT_LIB_MODE KTSignedAndEnvelopedDataAndTimeStamp : public KTSEQUENCE2
{
public :
	KTSignedAndEnvelopedDataAndTimeStamp()
	{	
        PLACE2OBJS( m_signedAndEnvelopedData, m_timeStamp )
	}

	KTContentInfo	                m_signedAndEnvelopedData;
	KTContentInfo	                m_timeStamp;
};

// -----------------------------------------------------------------------
//  Version            Date              Author             Note
// ----------     --------------     ---------------     ----------- 
//  1.0.0.0         2005-06-23            林灶钢            添加
//  1.1.0.0         2005-10-09            林灶钢            修改
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------------



#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ---------------------------------------------------------------------------- 
///   1.0.0.0  2003/04/11 11:53   Hongwei Yu   created 
///   1.0.0.1  2003/06/30 22:34   Hongwei Yu   choiced first obj in all choice##n

#endif
