#ifndef __X509_Certificate_Management_Protocols_H__
#define __X509_Certificate_Management_Protocols_H__

/**
  * @Name: CMP.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/11 14:16
  * @Description: all X.509 certificate Management Protocols class define
  * @Document: rfc2510.txt
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./X.509.h"
#include "./CRMF.h"
#include "./pkcs10.h"


typedef KTUTF8String    KTPKIFreeText; 
      //-- text encoded as UTF-8 String (note:  each UTF8String SHOULD
      //-- include an RFC 1766 language tag to indicate the language
      //-- of the contained text)

/*    
    SEQUENCE {
      m_infoType               KTOBJECTIDENTIFIER,
      m_infoValue              ANY DEFINED BY infoType  OPTIONAL
  }
  -- Example InfoTypeAndValue contents include, but are not limited to:
  --  { CAProtEncCert    = {id-it 1}, Certificate                     }
  --  { SignKeyPairTypes = {id-it 2}, SEQUENCE OF AlgorithmIdentifier }
  --  { EncKeyPairTypes  = {id-it 3}, SEQUENCE OF AlgorithmIdentifier }
  --  { PreferredSymmAlg = {id-it 4}, AlgorithmIdentifier             }
  --  { CAKeyUpdateInfo  = {id-it 5}, CAKeyUpdAnnContent              }
  --  { CurrentCRL       = {id-it 6}, CertificateList                 }
  -- where {id-it} = {id-pkix 4} = {1 3 6 1 5 5 7 4}
  -- This construct MAY also be used to define new PKIX Certificate
  -- Management Protocol request and response messages, or general-
  -- purpose (e.g., announcement) messages for future needs or for
  -- specific environments.
*/
SEQUENCEOF(KTAlgorithmIdentifierSequence, KTAlgorithmIdentifier)

class KT_LIB_MODE KTInfoTypeAndValue: public KTTwin
{
protected:
	KTObject* newObj(const char* pc_typeId) const;
};

SEQUENCEOF(KTGenMsgContent, KTInfoTypeAndValue);

class KT_LIB_MODE KTPKIHeader : public KTSEQUENCE12 {
public:
    KTPKIHeader()
    {
        PLACE12OBJS(m_pvno, m_sender, m_recipient, m_messageTime, 
            m_protectionAlg, m_senderKID, m_recipKID, m_transactionID,
            m_senderNonce, m_recipNonce, m_freeText, m_generalInfo);
        
        for(int i=3; i<12; i++)
        {
            m_ppObj[i]->setOptional();
            m_ppObj[i]->setTypeEx(i-3, TRUE);
        }
    }

      KTINTEGER             m_pvno;            //{ ietf-version2 (1) },
      KTGeneralName         m_sender;          //-- identifies the sender
      KTGeneralName         m_recipient;       //-- identifies the intended recipient
      KTGeneralizedTime     m_messageTime;     //[0]          OPTIONAL,
          //-- time of production of this message (used when sender
          //-- believes that the transport will be "suitable"; i.e.,
          //-- that the time will still be meaningful upon receipt)
      KTAlgorithmIdentifier m_protectionAlg;   //[1] OPTIONAL-- algorithm used for calculation of protection bits
      KTKeyIdentifier       m_senderKID;       //[2] OPTIONAL,
      KTKeyIdentifier       m_recipKID;        //[3] OPTIONAL, -- to identify specific keys used for protection
      KTOCTETSTRING         m_transactionID;   //[4] OPTIONAL,
          //-- identifies the transaction; i.e., this will be the same in
          //-- corresponding request, response and confirmation messages
      KTOCTETSTRING         m_senderNonce;     //[5] OPTIONAL,
      KTOCTETSTRING         m_recipNonce;      //[6] OPTIONAL,
          //-- nonces used to provide replay protection, senderNonce
          //-- is inserted by the creator of this message; recipNonce
          //-- is a nonce previously inserted in a related message by
          //-- the intended recipient of this message
      KTPKIFreeText         m_freeText;        //[7] OPTIONAL,
          //-- this may be used to indicate context-specific instructions
          //-- (this field is intended for human consumption)
      KTInfoTypeAndValue    m_generalInfo;     //[8] OPTIONAL
          //-- this may be used to convey context-specific information
          //-- (this field not primarily intended for human consumption)
  };

enum enumPKIStatus
{
      PKIStatusGranted                = 0,
            //-- you got exactly what you asked for
      PKIStatusGrantedWithMods        = 1,
            //-- you got something like what you asked for; the
            //-- requester is responsible for ascertaining the differences
      PKIStatusRejection              = 2,
            //-- you don't get it, more information elsewhere in the message
      PKIStatusWaiting                = 3,
            //-- the request body part has not yet been processed,
            //-- expect to hear more later
      PKIStatusRevocationWarning      = 4,
            //-- this message contains a warning that a revocation is
            //-- imminent
      PKIStatusRevocationNotification = 5,
            //-- notification that a revocation has occurred
      PKIStatusKeyUpdateWarning       =  6,
            //-- update already done for the oldCertId specified in
            //-- CertReqMsg
};
typedef KTINTEGER   KTPKIStatus;

class KT_LIB_MODE KTCertOrEncCert : public KTCHOICE2 {
public:
    KTCertOrEncCert()
    {
        PLACE2OBJS(m_certificate, m_encryptedCert);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_certificate.setTypeEx(0, TRUE);
        m_encryptedCert.setTypeEx(1, TRUE);    
    }
      KTCertificate     m_certificate;     //[0] 
      KTEncryptedValue  m_encryptedCert;   //[1] 
  };

class KT_LIB_MODE KTCertifiedKeyPair : public KTSEQUENCE3 {
public:
    KTCertifiedKeyPair()
    {
        PLACE3OBJS(m_certOrEncCert, m_privateKey, m_publicationInfo)
        m_privateKey.setOptional();
        m_publicationInfo.setOptional();   
    }
      KTCertOrEncCert       m_certOrEncCert;
      KTEncryptedValue      m_privateKey;           //[0] OPTIONAL,
      KTPKIPublicationInfo  m_publicationInfo;      //[1] OPTIONAL
  };

SEQUENCEOF(KTCertifiedKeyPairSequence, KTCertifiedKeyPair)

enum enumPKIFailureInfo
{
  //-- since we can fail in more than one way!
  //-- More codes may be added in the future if/when required.
      PKIFailureInfoBadAlg           = 0x001,
            //-- unrecognized or unsupported Algorithm Identifier
      PKIFailureInfoBadMessageCheck  = 0x002,
            //-- integrity check failed (e.g., signature did not verify)
      PKIFailureInfoBadRequest       = 0x004,
            //-- transaction not permitted or supported
      PKIFailureInfoBadTime          = 0x008,
            //-- messageTime was not sufficiently close to the system time,
            //-- as defined by local policy
      PKIFailureInfoBadCertId        = 0x010,
            //-- no certificate could be found matching the provided criteria
      PKIFailureInfoBadDataFormat    = 0x020,
            //-- the data submitted has the wrong format
      PKIFailureInfoWrongAuthority   = 0x040,
            //-- the authority indicated in the request is different from the
            //-- one creating the response token
      PKIFailureInfoIncorrectData    = 0x080,
            //-- the requester's data is incorrect (for notary services)
      PKIFailureInfoMissingTimeStamp = 0x100,
            //-- when the timestamp is missing but should be there (by policy)
      PKIFailureInfoBadPOP           = 0x200
            //-- the proof-of-possession failed
  };
typedef KTBITSTRING KTPKIFailureInfo;  

class KT_LIB_MODE KTPKIStatusInfo : public KTSEQUENCE3 {
public:
    KTPKIStatusInfo()
    {
        PLACE3OBJS(m_status, m_statusString, m_failInfo)
        m_statusString.setOptional();
        m_failInfo.setOptional();
    }
      KTPKIStatus       m_status;        
      KTPKIFreeText     m_statusString;   //OPTIONAL,
      KTPKIFailureInfo  m_failInfo;       //OPTIONAL
};

class KT_LIB_MODE KTCertResponse : public KTSEQUENCE4 {
public:
    KTCertResponse()
    {
        PLACE4OBJS(m_certReqId, m_status, m_certifiedKeyPair, m_rspInfo)
        m_certifiedKeyPair.setOptional();
        m_rspInfo.setOptional();    
    }
      KTINTEGER             m_certReqId;
            //-- to match this response with corresponding request (a value
            //-- of -1 is to be used if certReqId is not specified in the
            //-- corresponding request)
      KTPKIStatusInfo       m_status; 
      KTCertifiedKeyPair    m_certifiedKeyPair;        //OPTIONAL,
      KTOCTETSTRING         m_rspInfo;                 //OPTIONAL
            //-- analogous to the id-regInfo-asciiPairs OCTET STRING defined
            //-- for regInfo in CertReqMsg [CRMF]
  };

SEQUENCEOF(KTCertResponseSequence, KTCertResponse)

  //-- One INTEGER per encryption key certification request (in the
  //-- same order as these requests appear in CertReqMessages).  The
  //-- retrieved INTEGER A (above) is returned to the sender of the
  //-- corresponding Challenge.

class KT_LIB_MODE KTCertRepMessage : public KTSEQUENCE2 {
public:
    KTCertRepMessage()
    {
        PLACE2OBJS(m_caPubs, m_response)
        m_caPubs.setOptional();
        m_caPubs.setTypeEx(1, TRUE);
    }
      KTCertificates            m_caPubs;       //[1]  OPTIONAL,
      KTCertResponseSequence    m_response;      
  };

class KT_LIB_MODE KTChallenge : public KTSEQUENCE3 {
public:
    KTChallenge()
    {
        PLACE3OBJS(m_owf, m_witness, m_challenge)
        m_owf.setOptional();
    }
      KTAlgorithmIdentifier m_owf;              //OPTIONAL,
            //-- MUST be present in the first Challenge; MAY be omitted in any
            //-- subsequent Challenge in POPODecKeyChallContent (if omitted,
            //-- then the owf used in the immediately preceding Challenge is
            //-- to be used).
      KTOCTETSTRING         m_witness;          //
            //-- the result of applying the one-way function (owf) to a
            //-- randomly-generated INTEGER, A.  [Note that a different
            //-- INTEGER MUST be used for each Challenge.]
      KTOCTETSTRING         m_challenge;           
            //-- the encryption (under the public key for which the cert.
            //-- request is being made) of Rand, where Rand is specified as
  };

SEQUENCEOF(KTPOPODecKeyChallContent, KTChallenge);

  //-- One Challenge per encryption key certification request (in the
  //-- same order as these requests appear in CertReqMessages).

SEQUENCEOF(KTPOPODecKeyRespContent, KTINTEGER);

class KT_LIB_MODE KTKeyRecRepContent : public KTSEQUENCE4 {
public:
    KTKeyRecRepContent()
    {
        PLACE4OBJS(m_status, m_newSigCert, m_caCerts, m_keyPairHist)
        
        for(int i=1; i<4; i++)
        {
            m_ppObj[i]->setOptional();
            m_ppObj[i]->setTypeEx(i-1, TRUE);
        };      
    }

      KTPKIStatusInfo               m_status;                  
      KTCertificate                 m_newSigCert;          //[0] OPTIONAL,
      KTCertificates                m_caCerts;             //[1] OPTIONAL,
      KTCertifiedKeyPairSequence    m_keyPairHist;         //[2] OPTIONAL
  };

class KT_LIB_MODE KTRevDetails : public KTSEQUENCE4 {
public:
    KTRevDetails()
    {
        PLACE4OBJS(m_certDetails, m_revocationReason, m_badSinceDate, m_crlEntryDetails)
        for(int i=1; i<4; i++)
            m_ppObj[i]->setOptional();
    }
      KTCertTemplate    m_certDetails;
            //-- allows requester to specify as much as they can about
            //-- the cert. for which revocation is requested
            //-- (e.g., for cases in which serialNumber is not available)
      KTReasonFlags     m_revocationReason;      //OPTIONAL,
            //-- the reason that revocation is requested
      KTGeneralizedTime m_badSinceDate;          //OPTIONAL,
            //-- indicates best knowledge of sender
      KTExtensions      m_crlEntryDetails;       //OPTIONAL
            //-- requested crlEntryExtensions
  };

SEQUENCEOF(KTRevReqContent, KTRevDetails);

SEQUENCEOF(KTPKIStatusInfoSequence, KTPKIStatusInfo)

SEQUENCEOF(KTCertIdSequence, KTCertId)

SEQUENCEOF(KTCertificateListSequence, KTCertificateList)

class KT_LIB_MODE KTRevRepContent : public KTSEQUENCE3 {
public:
    KTRevRepContent()
    {
        PLACE3OBJS(m_status, m_revCerts, m_crls);
        for(int i=1; i<3; i++)
        {
            m_ppObj[i]->setOptional();
            m_ppObj[i]->setTypeEx(i-1, TRUE);
        }
    }

      KTPKIStatusInfoSequence   m_status;
            //-- in same order as was sent in RevReqContent
      KTCertIdSequence          m_revCerts;     //[0] OPTIONAL,
            //-- IDs for which revocation was requested (same order as status)
      KTCertificateListSequence m_crls;         //[1] OPTIONAL
            //-- the resulting CRLs (there may be more than one)
  };

class KT_LIB_MODE KTCAKeyUpdAnnContent : public KTSEQUENCE3 {
public:
    KTCAKeyUpdAnnContent()
    {
        PLACE3OBJS(m_oldWithNew, m_newWithOld, m_newWithNew)
    }
      KTCertificate m_oldWithNew;          //-- old pub signed with new priv
      KTCertificate m_newWithOld;          //-- new pub signed with old priv
      KTCertificate m_newWithNew;          //-- new pub signed with new priv
  };

typedef KTCertificate   KTCertAnnContent; 

class KT_LIB_MODE KTRevAnnContent : public KTSEQUENCE5 {
public:
    KTRevAnnContent()
    {
        PLACE5OBJS(m_status, m_certId, m_willBeRevokedAt, m_badSinceDate, m_certId)
        m_crlDetails.setOptional();
    }
      KTPKIStatus       m_status;
      KTCertId          m_certId; 
      KTGeneralizedTime m_willBeRevokedAt;
      KTGeneralizedTime m_badSinceDate;
      KTExtensions      m_crlDetails;       //OPTIONAL
            //-- extra CRL details(e.g., crl number, reason, location, etc.)
};

SEQUENCEOF(KTCRLAnnContent, KTCertificateList);

typedef KTNULL  KTPKIConfirmContent; 

typedef KTBITSTRING KTPKIProtection; 

class KTPKIBody;

class KT_LIB_MODE KTPKIMessage : public KTSEQUENCE4 {
public:
    KTPKIMessage()
    {
        PLACE4OBJS(m_header, m_header, m_protection, m_extraCerts);
        m_ppObj[1] = NULL;
        m_pBody = NULL;
        m_protection.setOptional();
        m_extraCerts.setOptional();
        m_protection.setTypeEx(0, TRUE);
        m_extraCerts.setTypeEx(1, TRUE);
    }
      KTPKIHeader       m_header;           
      KTPKIBody*        m_pBody;             
      KTPKIProtection   m_protection;   //[0]  OPTIONAL,
      KTCertificates    m_extraCerts;   //[1]  OPTIONAL
  };

typedef KTPKIMessage    KTNestedMessageContent; 

/*    
  -- May be sent by EE, RA, or CA (depending on message content).
  -- The OPTIONAL infoValue parameter of InfoTypeAndValue will typically
  -- be omitted for some of the examples given above.  The receiver is
  -- free to ignore any contained OBJ. IDs that it does not recognize.
  -- If sent from EE to CA, the empty set indicates that the CA may send
  -- any/all information that it wishes.
*/
SEQUENCEOF(KTGenRepContent, KTInfoTypeAndValue);

  //-- The receiver is free to ignore any contained OBJ. IDs that it does
  //-- not recognize.

class KT_LIB_MODE KTErrorMsgContent : public KTSEQUENCE3 {
public:
    KTErrorMsgContent()
    {
        PLACE3OBJS(m_pKIStatusInfo, m_errorCode, m_errorDetails)
        m_errorCode.setOptional();
        m_errorDetails.setOptional();
    }
      KTPKIStatusInfo   m_pKIStatusInfo;
      KTINTEGER          m_errorCode;        //OPTIONAL,
            //-- implementation-specific error codes
      KTPKIFreeText     m_errorDetails;     //OPTIONAL
            //-- implementation-specific error details
  };

class KT_LIB_MODE KTPKIBody : public KTCHOICE24 {       //-- message-specific body elements
public:
    KTPKIBody()
    {
        PLACE24OBJS(m_ir, m_ip, m_cr, m_cp, m_p10cr, m_popdecc, m_popdecr, m_kur, m_kup,
            m_krr, m_krp, m_rr, m_rp, m_ccr, m_ccp, m_ckuann, m_cann, m_rann,
            m_crlann, m_conf, m_nested, m_genm, m_genp, m_error);
		OptionPlacedObjs(24);
		SetTypeExPlacedObjs(24, TRUE);
		ChoiceFirstPlacedObj();
    }

      KTCertReqMessages         m_ir;      //[0]  --Initialization Request
      KTCertRepMessage          m_ip;      //[1]  --Initialization Response
      KTCertReqMessages         m_cr;      //[2]  --Certification Request
      KTCertRepMessage          m_cp;      //[3]  --Certification Response
      KTCertificationRequest    m_p10cr;   //[4]  --imported from [PKCS10]
      KTPOPODecKeyChallContent  m_popdecc; //[5]  --pop Challenge
      KTPOPODecKeyRespContent   m_popdecr; //[6]  --pop Response
      KTCertReqMessages         m_kur;     //[7]  --Key Update Request
      KTCertRepMessage          m_kup;     //[8]  --Key Update Response
      KTCertReqMessages         m_krr;     //[9]  --Key Recovery Request
      KTKeyRecRepContent        m_krp;     //[10] --Key Recovery Response
      KTRevReqContent           m_rr;      //[11] --Revocation Request
      KTRevRepContent           m_rp;      //[12] --Revocation Response  
      KTCertReqMessages         m_ccr;     //[13] --Cross-Cert. Request
      KTCertRepMessage          m_ccp;     //[14] --Cross-Cert. Response
      KTCAKeyUpdAnnContent      m_ckuann;  //[15] --CA Key Update Ann.
      KTCertAnnContent          m_cann;    //[16] --Certificate Ann.
      KTRevAnnContent           m_rann;    //[17] --Revocation Ann.
      KTCRLAnnContent           m_crlann;  //[18] --CRL Announcement
      KTPKIConfirmContent       m_conf;    //[19] --Confirmation
      KTNestedMessageContent    m_nested;  //[20] --Nested Message
      KTGenMsgContent           m_genm;    //[21] --General Message
      KTGenRepContent           m_genp;    //[22] --General Response
      KTErrorMsgContent         m_error;   //[23] --Error Message
  };

class KT_LIB_MODE KTProtectedPart : public KTSEQUENCE2 {
public:
    KTProtectedPart()
    {
        PLACE2OBJS(m_header, m_body);
    }
      KTPKIHeader   m_header; 
      KTPKIBody     m_body;      
  };

  //PasswordBasedMac ::= OBJECT IDENTIFIER --{1 2 840 113533 7 66 13}

class KT_LIB_MODE KTPBMParameter : public KTSEQUENCE4 {
public:
    KTPBMParameter()
    {
        PLACE4OBJS(m_salt, m_owf, m_iterationCount, m_mac)
    }
      KTOCTETSTRING         m_salt;
      KTAlgorithmIdentifier m_owf;                  //-- AlgId for a One-Way Function (SHA-1 recommended)
      KTINTEGER             m_iterationCount;       //-- number of times the OWF is applied
      KTAlgorithmIdentifier m_mac;                  //-- the MAC AlgId (e.g., DES-MAC, Triple-DES-MAC [PKCS11],
};      //-- or HMAC [RFC2104, RFC2202])

  //DHBasedMac ::= OBJECT IDENTIFIER --{1 2 840 113533 7 66 30}

class KT_LIB_MODE KTDHBMParameter : public KTSEQUENCE2 {
public:
    KTDHBMParameter()
    {
        PLACE2OBJS(m_owf, m_mac)
    }
      KTAlgorithmIdentifier m_owf;  //-- AlgId for a One-Way Function (SHA-1 recommended)
      KTAlgorithmIdentifier m_mac;  //-- the MAC AlgId (e.g., DES-MAC, Triple-DES-MAC [PKCS11],
  };    //-- or HMAC [RFC2104, RFC2202])

typedef KTCertificate   KTOOBCert; 

class KT_LIB_MODE KTOOBCertHash : public KTSEQUENCE3 {
public:
    KTOOBCertHash()
    {
        PLACE3OBJS(m_hashAlg, m_certId, m_hashVal)
        m_hashVal.setOptional();
        m_certId.setOptional();
        m_hashVal.setTypeEx(0, TRUE);
        m_certId.setTypeEx(1, TRUE);
    }
      KTAlgorithmIdentifier m_hashAlg;     //[0] OPTIONAL,
      KTCertId              m_certId;      //[1] OPTIONAL,
      KTBITSTRING           m_hashVal;         
            //-- hashVal is calculated over DER encoding of the
            //-- subjectPublicKey field of the corresponding cert.
};

class KT_LIB_MODE KTRand : public KTSEQUENCE2 {
public:
    KTRand()
    {
        PLACE2OBJS(m_int, m_sender)
    }
      KTINTEGER     m_int;      //--       - the randomly-generated INTEGER A (above)
      KTGeneralName m_sender;   // --      - the sender's name (as included in PKIHeader)
};

#ifdef KT_WIN32
    #pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/11 20:44   Hongwei Yu   created 
///   1.0.0.1  2003/06/30 22:40   Hongwei Yu   choiced first obj in all choice##n

#endif 
