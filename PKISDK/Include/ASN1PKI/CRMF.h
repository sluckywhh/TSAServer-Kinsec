#ifndef __X509_Certificate_Request_Message_Format_H__
#define __X509_Certificate_Request_Message_Format_H__

/**
  * @Name: CRMF.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/11 16:50
  * @Description: all X.509 certificate Request Message Format class define
  * @Document: rfc2511.txt
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./X.509.h"
#include "./pkcs10.h"
#include "./pkcs7.h"

///////////////////////////////////////////////////////////
//KTOptionalValidity
class KT_LIB_MODE KTOptionalValidity : public KTSEQUENCE2 {
public:
	KTOptionalValidity()
    {
		PLACE2OBJS(m_notBefore, m_notAfter)

		m_notBefore.setOptional();
		m_notAfter.setOptional();
		m_notBefore.setTypeEx(0, TRUE);
		m_notBefore.setTypeEx(1, TRUE);
	}

	KTTime		m_notBefore;		//[0] Time OPTIONAL
	KTTime		m_notAfter;			//[1] Time OPTIONAL
};

///////////////////////////////////////////////////////////
//KTCertTemplate
class KT_LIB_MODE KTCertTemplate : public KTSEQUENCE10 {
public:
	KTCertTemplate()
    {
		PLACE10OBJS(m_version, m_serialNumber, m_signingAlg, m_issuer, m_validity,
			m_subject, m_publicKey, m_issuerUID, m_subjectUID, m_extensions)
        for(int i=0; i<10; i++)
        {
            m_ppObj[i]->setOptional();
            m_ppObj[i]->setTypeEx(i, TRUE);
        }
    }

	KTVersion				    m_version;		        // [0] OPTIONAL
	KTINTEGER				    m_serialNumber;	        // [1] OPTIONAL
	KTAlgorithmIdentifier	    m_signingAlg;	        // [2] OPTIONAL
	KTName					    m_issuer;		        // [3] OPTIONAL
	KTOptionalValidity		    m_validity;		        // [4] OPTIONAL
	KTName					    m_subject;		        // [5] OPTIONAL
	KTSubjectPublicKeyInfo	    m_publicKey;	        // [6] OPTIONAL
	KTUniqueIdentifier			m_issuerUID;	        // [7] OPTIONAL
	KTUniqueIdentifier			m_subjectUID;	        // [8] OPTIONAL
	KTExtensions				m_extensions;		    // [9] OPTIONAL
};

SEQUENCEOF(KTControls, KTAttributeTypeAndValue)
///////////////////////////////////////////////////////////
class KT_LIB_MODE KTCertRequest : public KTSEQUENCE3{
public:
	KTCertRequest()
    {
		PLACE3OBJS(m_certReqId, m_certTemplate, m_controls)
        m_controls.setOptional();
	}

	KTINTEGER				m_certReqId;	//-- ID for matching request and reply
	KTCertTemplate			m_certTemplate; //Selected fields of cert to be issued
	KTControls				m_controls;	    //OPTIONAL
};

//-- algorithm value shall be PasswordBasedMac {1 2 840 113533 7 66 13}
//-- parameter value is PBMParameter
class KT_LIB_MODE KTPKMACValue : public KTSEQUENCE2 {
public:
    KTPKMACValue()
    {
        PLACE2OBJS(m_algId, m_value);
    }

   KTAlgorithmIdentifier    m_algId;
   KTBITSTRING              m_value;
};

/*
GeneralName:
        -- used only if an authenticated identity has been
        -- established for the sender (e.g., a DN from a
        -- previously-issued and currently-valid certificate
PKMACValue:
        -- used if no authenticated GeneralName currently exists for
        -- the sender; publicKeyMAC contains a password-based MAC
        -- on the DER-encoded value of publicKey
*/
class KT_LIB_MODE KTAuthInfo : public KTCHOICE2 {
public:
    KTAuthInfo()
    {
        PLACE2OBJS(m_sender, m_publicKeyMAC);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_sender.setTypeEx(0, TRUE);
    }
        KTGeneralName       m_sender;              //[0] 
        KTPKMACValue        m_publicKeyMAC;
};

class KT_LIB_MODE KTPOPOSigningKeyInput  : public KTSEQUENCE2 {
public:
    KTPOPOSigningKeyInput()
    {
        PLACE2OBJS(m_authInfo, m_publicKey);
    }

    KTAuthInfo              m_authInfo; 
    KTSubjectPublicKeyInfo  m_publicKey;
};

class KT_LIB_MODE KTPOPOSigningKey : public KTToBeSigned {
public:
    KTPOPOSigningKey()
    {
		attachToBeSignedObj(&m_poposkInput);
        m_poposkInput.setOptional();
        m_poposkInput.setTypeEx(0, TRUE);
    }

    KTPOPOSigningKeyInput   m_poposkInput;           //[0]  OPTIONAL,           
};

/*    
encrCert (0),
-- requests that resulting certificate be encrypted for the
-- end entity (following which, POP will be proven in a
-- confirmation message)
challengeResp (1) }
-- requests that CA engage in challenge-response exchange with
-- end entity in order to prove private key possession
*/
enum enumSubsequentMessage
{
    SubsequentMessagEncrCert = 0,
    SubsequentMessagChallengeResp = 1,
};
typedef KTINTEGER KTSubsequentMessage;

/*
thisMessage       [0] BIT STRING,
-- possession is proven in this message (which contains the private
-- key itself (encrypted for the CA))
subsequentMessage [1] SubsequentMessage,
-- possession will be proven in a subsequent message
dhMAC             [2] BIT STRING }
-- for keyAgreement (only), possession is proven in this message
-- (which contains a MAC (over the DER-encoded value of the
-- certReq parameter in CertReqMsg, which MUST include both subject
-- and publicKey) based on a key derived from the end entity's
-- private DH key and the CA's public DH key);
-- the dhMAC value MUST be calculated as per the directions given
-- in Appendix A.
*/
class KT_LIB_MODE KTPOPOPrivKey  : public KTCHOICE3 {
public:
    KTPOPOPrivKey()
    {
        PLACE3OBJS(m_thisMessage, m_subsequentMessage, m_dhMAC);
		OptionPlacedObjs(3);
		ChoiceFirstPlacedObj();
        m_thisMessage.setTypeEx(0, TRUE);
        m_subsequentMessage.setTypeEx(1, TRUE);
        m_dhMAC.setTypeEx(2, TRUE);
    }
    
    KTBITSTRING             m_thisMessage;          //[0]
    KTSubsequentMessage     m_subsequentMessage;    //[1] ,
    KTBITSTRING             m_dhMAC;                //[2]  
};

///////////////////////////////////////////////////////////
class KT_LIB_MODE KTProofOfPossession : public KTCHOICE4 {
public:
	KTProofOfPossession()
    {
		PLACE4OBJS(m_raVerified, m_signature, m_keyEncipherment, m_keyEncipherment)
		OptionPlacedObjs(4);
		ChoiceFirstPlacedObj();
        m_raVerified.setTypeEx(0, TRUE);
        m_signature.setTypeEx(1, TRUE);
        m_keyEncipherment.setTypeEx(2, TRUE);
        m_keyAgreement.setTypeEx(3, TRUE);
	}
	
	KTNULL		        m_raVerified;        //[0],-- used if the RA has already verified that the requester is in possession of the private key
	KTPOPOSigningKey    m_signature;         //[1] ,
	KTPOPOPrivKey       m_keyEncipherment;   //[2] ,
	KTPOPOPrivKey       m_keyAgreement;      //[3]    
};

///////////////////////////////////////////////////////////
//KTCertReqMsg
SEQUENCEOF(KTAttributeTypeAndValues, KTAttributeTypeAndValue)

class KT_LIB_MODE KTCertReqMsg : public KTSEQUENCE3 {
public:
	KTCertReqMsg()
    {
		PLACE3OBJS(m_certReq, m_pop, m_regInfo)
		m_pop.setOptional();
		m_regInfo.setOptional();
	}
    
	KTCertRequest					m_certReq;
	KTProofOfPossession				m_pop;	    //OPTIONAL
	KTAttributeTypeAndValues		m_regInfo;  //SIZE(1..MAX)  OPTIONAL
	
};
SEQUENCEOF(KTCertReqMessages, KTCertReqMsg)

/////////////////////////////////////////////////////////////////////////////////
//////-- Registration Controls in CRMF

//id-regCtrl-regToken OBJECT IDENTIFIER ::= { id-regCtrl 1 }
typedef KTUTF8String    KTRegToken; 

//id-regCtrl-authenticator OBJECT IDENTIFIER ::= { id-regCtrl 2 }
typedef KTUTF8String    KTAuthenticator;

//id-regCtrl-pkiPublicationInfo OBJECT IDENTIFIER ::= { id-regCtrl 3 }
enum enumPubMethod
{
    PubMethodDontCare    = 0,
    PubMethodX500        = 1,
    PubMethodWeb         = 2,
    PubMethodLdap        = 3
};

class KT_LIB_MODE KTSinglePubInfo : public KTSEQUENCE2 {
public:
    KTSinglePubInfo()
    {
        PLACE2OBJS(m_pubMethod, m_pubLocation);
        m_pubLocation.setOptional();
    }

    KTINTEGER       m_pubMethod;
    KTGeneralName   m_pubLocation;   //OPTIONAL 
};

SEQUENCEOF(KTSinglePubInfoSequence, KTSinglePubInfo)

enum enumPubAction
{
    PubActionDontPublish    = 0,
    PubActionPleasePublish  = 1,
};

//-- pubInfos MUST NOT be present if action is "dontPublish"
//-- (if action is "pleasePublish" and pubInfos is omitted,
//-- "dontCare" is assumed)
class KT_LIB_MODE KTPKIPublicationInfo : public KTSEQUENCE2 {
public:
    KTPKIPublicationInfo()
    {
        PLACE2OBJS(m_action, m_pubInfos);
        m_pubInfos.setOptional();
    }

   KTINTEGER                m_action;     
   KTSinglePubInfoSequence  m_pubInfos;  //SEQUENCE SIZE (1..MAX) OF SinglePubInfo OPTIONAL 
};


//id-regCtrl-pkiArchiveOptions     OBJECT IDENTIFIER ::= { id-regCtrl 4 }
class KT_LIB_MODE KTEncryptedValue  : public KTSEQUENCE6 {
public:
    KTEncryptedValue()
    {
        PLACE6OBJS(m_intendedAlg, m_symmAlg, m_encSymmKey,
            m_keyAlg, m_valueHint, m_encValue);

        m_intendedAlg.setOptional();
        m_symmAlg.setOptional();
        m_encSymmKey.setOptional();
        m_keyAlg.setOptional();
        m_valueHint.setOptional();

        m_intendedAlg.setTypeEx(0, TRUE);
        m_symmAlg.setTypeEx(1, TRUE);
        m_encSymmKey.setTypeEx(2, TRUE);
        m_keyAlg.setTypeEx(3, TRUE);
        m_valueHint.setTypeEx(4, TRUE);
    }

    KTAlgorithmIdentifier   m_intendedAlg;   //[0]   OPTIONAL, -- the intended algorithm for which the value will be used
    KTAlgorithmIdentifier   m_symmAlg;       //[1]   OPTIONAL, -- the symmetric algorithm used to encrypt the value
    KTBITSTRING             m_encSymmKey;    //[2]   OPTIONAL, -- the (encrypted) symmetric key used to encrypt the value
    KTAlgorithmIdentifier   m_keyAlg;        //[3]   OPTIONAL, -- algorithm used to encrypt the symmetric key
    KTOCTETSTRING           m_valueHint;     //[4]   OPTIONAL,
        //-- a brief description or identifier of the encValue content
        //-- (may be meaningful only to the sending entity, and used only
        //-- if EncryptedValue might be re-examined by the sending entity
        //-- in the future)
    KTBITSTRING             m_encValue;       //-- the encrypted value itself
};

class KT_LIB_MODE KTEncryptedKeyChoice  : public KTCHOICE2 {
public:
    KTEncryptedKeyChoice()
    {
        PLACE2OBJS(m_encryptedValue, m_envelopedData);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_envelopedData.setTypeEx(0, TRUE);
    }

    KTEncryptedValue    m_encryptedValue;
    KTEnvelopedData     m_envelopedData;     //[0]  
        //-- The encrypted private key MUST be placed in the envelopedData
        //-- encryptedContentInfo encryptedContent OCTET STRING.
};

typedef KTOCTETSTRING   KTKeyGenParameters;

class KT_LIB_MODE KTPKIArchiveOptions  : public KTCHOICE3 {
public:
    KTPKIArchiveOptions()
    {
        PLACE3OBJS(m_encryptedPrivKey, m_keyGenParameters, m_archiveRemGenPrivKey);
		OptionPlacedObjs(3);
		ChoiceFirstPlacedObj();
        m_encryptedPrivKey.setTypeEx(0, TRUE);
        m_keyGenParameters.setTypeEx(1, TRUE);
        m_archiveRemGenPrivKey.setTypeEx(2, TRUE);
    }

    KTEncryptedKeyChoice        m_encryptedPrivKey;     //[0] , -- the actual value of the private key
    KTKeyGenParameters          m_keyGenParameters;     //[1] ,-- parameters which allow the private key to be re-generated
    KTBOOLEAN                   m_archiveRemGenPrivKey;         //[2] 
        //-- set to TRUE if sender wishes receiver to archive the private
        //-- key of a key pair which the receiver generates in response to
        //-- this request; set to FALSE if no archival is desired.
};

//id-regCtrl-oldCertID          OBJECT IDENTIFIER ::= { id-regCtrl 5 }

///not same to with KTCertID in OCSP.h
class KT_LIB_MODE KTCertId : public KTSEQUENCE2 {
public:
    KTCertId()
    {
        PLACE2OBJS(m_issuer, m_serialNumber);
    }

    KTGeneralName   m_issuer;
    KTINTEGER       m_serialNumber;
};

typedef KTCertId    KTOldCertId; 

//id-regCtrl-protocolEncrKey    OBJECT IDENTIFIER ::= { id-regCtrl 6 }
typedef KTSubjectPublicKeyInfo  KTProtocolEncrKey; 

//-- Registration Info in CRMF
//id-regInfo-utf8Pairs    OBJECT IDENTIFIER ::= { id-regInfo 1 }
typedef KTUTF8String    KTUTF8Pairs;
 
//id-regInfo-certReq       OBJECT IDENTIFIER ::= { id-regInfo 2 }
typedef KTCertRequest  KTCertReq; 

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/11 16:51   Hongwei Yu   created 
///   1.0.0.1  2003/06/30 22:40   Hongwei Yu   choiced first obj in all choice##n

#endif
