#ifndef	__OCSP_H__
#define __OCSP_H__

/**
  * @Name: OCSP.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/09 22:22
  * @Description: all OCSP protocol synax class define
  * @Document: rfc2560.txt  
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

///////////////////////////////////////////////////////////////////////////////
// KTCertID 
class KT_LIB_MODE KTCertID : public KTSEQUENCE4		//KTCertID is of type SEQUENCE and has four member variable
{
  public:
	KTCertID()
	{
		PLACE4OBJS(m_hashAlgorithm,	m_issuerNameHash, m_issuerKeyHash, m_serialNumber)
	}

	KTAlgorithmIdentifier 	    m_hashAlgorithm;
	KTOCTETSTRING 			    m_issuerNameHash;
	KTOCTETSTRING 			    m_issuerKeyHash;
	KTCertificateSerialNumber   m_serialNumber;
};

///////////////////////////////////////////////////////////////////////////////
// KTRequest 
class KT_LIB_MODE KTRequest : public KTSEQUENCE2
{
public:
	KTRequest()
	{
		PLACE2OBJS(m_reqCert, m_singleRequestExtensions)
		m_singleRequestExtensions.setOptional();
		m_singleRequestExtensions.setTypeEx(0, TRUE);
	}

	KTCertID		m_reqCert;
	KTExtensions	m_singleRequestExtensions;		//OPTIONAL
};

///////////////////////////////////////////////////////////////////////////////
// KTRequestSequence
SEQUENCEOF(KTRequestSequence, KTRequest)

//////////////////////////////////////////////////////////////////
// KTTBSRequest   //To be signed request
class KT_LIB_MODE KTTBSRequest : public KTSEQUENCE4
{
public:
	KTTBSRequest()
	{
		m_version = 0;
		m_version.setTypeEx(0, TRUE);
		m_requestorName.setOptional();
		m_requestorName.setTypeEx(1, TRUE);
		m_requestExtensions.setOptional();
		m_requestExtensions.setTypeEx(2, TRUE);

		PLACE4OBJS(m_version, m_requestorName, m_requestList, m_requestExtensions)
	}

	KTINTEGER				m_version;
	KTGeneralName			m_requestorName;
	KTRequestSequence		m_requestList;
	KTExtensions			m_requestExtensions;
};

//////////////////////////////////////////////////////////////////
// KTSignature
class KT_LIB_MODE KTSignature : public KTSEQUENCE3
{
public:
	KTSignature()
	{
		m_certs.setOptional();
		m_certs.setTypeEx(0, TRUE);

		PLACE3OBJS(m_algorithm, m_signature, m_certs)
	};

	KTAlgorithmIdentifier	m_algorithm;
	KTBITSTRING				m_signature;
	KTCertificates			m_certs;
};

///////////////////////////////////////////////////////////////////////////////
// KTOCSPReqest	: 
class KT_LIB_MODE KTOCSPRequest : public KTSEQUENCE2
{
public:
	KTOCSPRequest()
	{
		PLACE2OBJS(m_tbsRequest, m_optionalSignature)

		m_optionalSignature.setOptional();
		m_optionalSignature.setTypeEx(0, TRUE);
	}

	KTTBSRequest	    m_tbsRequest;
	KTSignature		    m_optionalSignature; //optional
};

//////////////////////////////////////////////////////////////////
// KTResponseBytes
class KT_LIB_MODE KTResponseBytes : public KTSEQUENCE2
{
public:
	KTResponseBytes()
	{
		PLACE2OBJS(m_responseType, m_response)
	}

	KTObjectIdentifier      m_responseType;
	KTOCTETSTRING           m_response; 
};

//////////////////////////////////////////////////////////////////
// KTResponderID         
class KT_LIB_MODE KTResponderID : public KTCHOICE2
{
public:
	KTResponderID()
	{
		PLACE2OBJS(m_byName, m_byKey);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_byName.setTypeEx(1, TRUE);
		m_byKey.setTypeEx(2, TRUE);
	};

	KTName				m_byName;
	KTOCTETSTRING		m_byKey;
};

typedef KTGeneralizedTime   KTArchiveCutoff; 

SEQUENCEOF(KTAcceptableResponses, KTObjectIdentifier)

///////////////////////////////////////////////////////////////
///
// KTRevokedInfo
// this is not stardard because reason is basicly not a String class KT_LIB_MODE??
class KT_LIB_MODE KTRevokedInfo : public KTSEQUENCE2
{
public:
	KTRevokedInfo()
	{
		PLACE2OBJS(m_revocationTime, m_revocationReason)

		m_revocationReason.setOptional();
		m_revocationReason.setTypeEx(0, TRUE);
	}

	KTGeneralizedTime	m_revocationTime;
	KTCRLReason			m_revocationReason; //explicit 0
};

//////////////////////////////////////////////////////////////////
// KTCertStatus
class KT_LIB_MODE KTCertStatus : public KTCHOICE3
{

public:
	KTCertStatus()
	{
		PLACE3OBJS(m_good, m_revoked, m_unknown);
		OptionPlacedObjs(3);
		ChoiceFirstPlacedObj();
        m_good.setTypeEx(0, FALSE);
		m_revoked.setTypeEx(1, FALSE);
		m_unknown.setTypeEx(2, FALSE);
	}

	KTNULL				m_good;
	KTRevokedInfo		m_revoked;
	KTNULL				m_unknown;
};

//////////////////////////////////////////////////////////////////
// KTSingleResponse
class KT_LIB_MODE KTSingleResponse : public KTSEQUENCE5
{
public:
	KTSingleResponse()
	{
		PLACE5OBJS(m_certID, m_certStatus,	m_thisUpdate,
			m_nextUpdate, m_singleExtensions)

		m_nextUpdate.setOptional();
		m_nextUpdate.setTypeEx(0, TRUE);
		m_singleExtensions.setOptional();
		m_singleExtensions.setTypeEx(1, TRUE);
	}

	KTCertID				m_certID;
	KTCertStatus			m_certStatus;
	KTGeneralizedTime 	    m_thisUpdate; 
	KTGeneralizedTime 	    m_nextUpdate;		//optional explicit 0
	KTExtensions			m_singleExtensions;	//optional explicit 1
};

//////////////////////////////////////////////////////////////////
// KTResponseSequence
SEQUENCEOF(KTResponseSequence, KTSingleResponse)

//////////////////////////////////////////////////////////////////
// KTResponseData
class KT_LIB_MODE KTResponseData : public KTSEQUENCE5
{
public:
	KTResponseData()
	{
		PLACE5OBJS(m_version, m_responderID, m_producedAt,
			m_responses, m_responseExtensions)

		m_version = 1;
		m_version.setTypeEx(0, TRUE);
		m_responseExtensions.setOptional();
		m_responseExtensions.setTypeEx(1, TRUE);
	}

	KTINTEGER			    m_version;				//explicit 0
	KTResponderID		    m_responderID;
	KTGeneralizedTime 	    m_producedAt; 
	KTResponseSequence	    m_responses;
	KTExtensions			m_responseExtensions;	//optional explicit 1
};

//////////////////////////////////////////////////////////////////
// KTBasicOCSPResponse
class KT_LIB_MODE KTBasicOCSPResponse : public KTSEQUENCE4
{
public:
    KTBasicOCSPResponse()
	{
		PLACE4OBJS(m_tbsResponseData, m_signatureAlgorithm, m_signature, m_certs)

		m_certs.setOptional();
		m_certs.setTypeEx(0, TRUE);
	}

	KTResponseData			m_tbsResponseData;
	KTAlgorithmIdentifier	m_signatureAlgorithm;
	KTBITSTRING		 		m_signature;
	KTCertificates      	m_certs;				//optional explicit 0
};

//////////////////////////////////////////////////////////////////
// KTOCSPResponseStatus
enum enumOCSPResponseStatus
{
	OCSPResStatusSuccessful = 0,
	OCSPResStatusMalformedRequest,
	OCSPResStatusInternalError,
	OCSPResStatusTryLater,
	OCSPResStatusNotUsedNow,
	OCSPResStatusSigRequired,
	OCSPResStatusUnauthorized
};
typedef KTEnumerated KTOCSPResponseStatus;

//////////////////////////////////////////////////////////////////
// KTOCSPResponse
class KT_LIB_MODE KTOCSPResponse : public KTSEQUENCE2
{
public:
	KTOCSPResponse()
	{
		PLACE2OBJS(m_responseStatus, m_responseBytes)

		m_responseBytes.setOptional();
		m_responseBytes.setTypeEx(0, TRUE);
	}
	
	KTOCSPResponseStatus    m_responseStatus;
	KTResponseBytes	        m_responseBytes; //optional explicit 0
};

//////////////////////////////////////////////////////////////////
// KTServiceLocator
// this is a single extension of the response
class KT_LIB_MODE KTServiceLocator : public KTSEQUENCE2
{
public:
    KTServiceLocator()
	{
		PLACE2OBJS(m_issuer, m_locator)
		m_locator.setOptional();
	}

	KTName						    m_issuer;
	KTAuthorityInfoAccessSyntax	    m_locator;	//    OPTIONAL
};

//////////////////////////////////////////////////////////////////
// KTCrlID
// this is a singleExtension of the response
class KT_LIB_MODE KTCrlID : public KTSEQUENCE3
{
public:
	KTCrlID()
	{
		PLACE3OBJS(m_crlUrl, m_crlNum, m_crlTime)

		m_crlUrl.setOptional();
		m_crlUrl.setTypeEx(0, TRUE);
		m_crlNum.setOptional();
		m_crlNum.setTypeEx(1, TRUE);
		m_crlTime.setOptional();
		m_crlTime.setTypeEx(2, TRUE);
	}

	KTIA5String			m_crlUrl;	// [0]     EXPLICIT IA5String OPTIONAL
	KTINTEGER			m_crlNum;	// [1]     EXPLICIT INTEGER OPTIONAL
	KTGeneralizedTime	m_crlTime;	// [2]     EXPLICIT GeneralizedTime OPTIONAL
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/10 00:31   Hongwei Yu   created 
///   1.0.0.1  2003/04/11 14:13   Hongwei Yu   refactory 
///   1.0.0.2  2003/06/30 22:40   Hongwei Yu   choiced first obj in all choice##n

#endif	//	__OCSP_H__
