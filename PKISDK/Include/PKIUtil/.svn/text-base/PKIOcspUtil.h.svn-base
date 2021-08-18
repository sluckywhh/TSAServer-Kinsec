#ifndef	__PKI_OCSP_UTIL_H__
#define	__PKI_OCSP_UTIL_H__

/**
  * @Name: PKIOCSPUtil.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 02:00
  * @Description: API functions about OCSP protocol 
*/ 

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#undef EXPORTS
#ifdef WIN32
	#ifndef STATIC_LIB
		#ifdef KT_DLL // assume this is defined when we build the dll
			#define EXPORTS __declspec( dllexport)
		#else
			#define EXPORTS __declspec( dllimport)
		#endif
	#else
		#define EXPORTS  
	#endif
#else
	#define EXPORTS
#endif  //WIN32

const int	REQUEST_ITEM_TYPE_EXTENSION	= 1;
const int	REQUEST_ITEM_TYPE_SGLREQ	= 2;
const int	REQUEST_ITEM_TYPE_VERSION	= 3;
const int	REQUEST_ITEM_TYPE_REQUESTOR	= 4;
const int	REQUEST_ITEM_TYPE_CERTID	= 5;
const int	REQUEST_ITEM_TYPE_SIGN		= 6;

class CERTSTATUS{
public:
	KTCertificateSerialNumber 	m_serialNumber;
	int							m_certStatus;  //0 1 2
	KTCRLReason					m_CRLReason;
};
typedef std::vector<CERTSTATUS*> CERTSTATUS_PTR_ARRAY;

class CERTSTATUSLIST{
public:
	~CERTSTATUSLIST();
	void add(const KTCertificateSerialNumber& serialNumber,int certStatus, const KTCRLReason& reason);
	int getCount()const;
	CERTSTATUS& operator[](int n);
protected:
	CERTSTATUS_PTR_ARRAY m_CertStatusArray;
};

inline CERTSTATUSLIST::~CERTSTATUSLIST()
{
	for(int i=0;i<m_CertStatusArray.size();i++)
	{
		delete m_CertStatusArray[i];
	}
}
inline void CERTSTATUSLIST::add(const KTCertificateSerialNumber& serialNumber, int certStatus, const KTCRLReason& reason)
{
	CERTSTATUS *pCertStatus=new CERTSTATUS;
	pCertStatus->m_serialNumber.cloneContent(serialNumber);
	pCertStatus->m_certStatus = certStatus;
	pCertStatus->m_CRLReason.setValue(reason.value());
	m_CertStatusArray.push_back(pCertStatus);
}
inline int CERTSTATUSLIST::getCount()const
{
	return m_CertStatusArray.size();
}
inline CERTSTATUS& CERTSTATUSLIST::operator[](int n)
{
	assert(n>0);
	assert(n<getCount());
	return *m_CertStatusArray[n];
}


EXPORTS KTINT32
KTPKI_GenOCSPRequest(
	IN  KT_SESSION_HANDLE	hSession,	
	IN  KTGeneralName&		requestor,		
	IN  KTCertificates&		certs,
	IN  KTCertificate*		pUserCert,		//NULL if not sign.
	OUT KTOCSPRequest& 		ocspReq
);

EXPORTS KTINT32
KTPKI_ValidateOCSPResponse(
	IN  KT_SESSION_HANDLE	hSession,	
	IN  KTOCSPResponse&		ocspResponse,
	OUT KTGeneralizedTime&	nonce,
	OUT KTGeneralizedTime&	ArchiveCutoff,
	OUT CERTSTATUSLIST&		statusList,
	OUT int*				pOcspStatus
);

EXPORTS KTINT32
KTPKI_ConstructOCSPSingleResponse(
	IN  KT_SESSION_HANDLE	hSession,	
	IN	KTCertID	*			pCertID,
	IN	KTCertStatus&		CertStatus,
	IN	KTGeneralizedTime& 	ThisUpdate,
	IN	KTGeneralizedTime& 	NextUpdate,
	IN	KTExtensions&		Extensions,
	OUT	KTSingleResponse&	SglResp
);

EXPORTS KTINT32
KTPKI_ConstructOBRNew(
	IN  KT_SESSION_HANDLE	hSession,	
	IN	int					nVersion,
	IN	const KTByteArray&	baResponderID,
	IN	KTSingleResponse *	pSglResp,
	IN	KTExtensions&		Extensions,
	IN	OUT	KTResponseData&	ocspResponse
);

EXPORTS KTINT32
KTPKI_ConstructOBRMid(
	IN  KT_SESSION_HANDLE	hSession,	
	IN	KTSingleResponse *	pSglResp,
	IN	OUT	KTResponseData&	ocspResponse
);

EXPORTS KTINT32
KTPKI_ConstructOBREnd(
	IN  KT_SESSION_HANDLE		hSession,
	IN	KTResponseData			*pocspResponse,
	IN	const KTCHAR*				pchSignAlgo,
	IN	KTCertificates&			Certs,
	IN	OUT KTBasicOCSPResponse& boResponse
);

EXPORTS KTINT32
KTPKI_ConstructOCSPResponse(
	IN  KT_SESSION_HANDLE	hSession,	
	IN	KTResponseBytes& 	RespBytes,
	IN	int					nRespStatus,
	OUT	KTOCSPResponse&		ocspResponse
);

EXPORTS KTINT32
KTPKI_ConstructOCSPExtendResponse(
	IN  KT_SESSION_HANDLE	hSession
);

//get any item from a OCSP Request

EXPORTS KTINT32
KTPKI_ParseOCSPRequest(
	IN  KT_SESSION_HANDLE	hSession,	
	IN	KTOCSPRequest *		pRequest,
	IN	int					nObjectType,
	IN	int					nIndex,
	OUT KTByteArray&		baValue
);

EXPORTS KTINT32
KTPKI_ParseOCSPSingleRequest(
	IN  KT_SESSION_HANDLE	hSession,	
	IN	KTRequest			*pSingleRequest,
	IN	int					nObjectType,
	IN	int					nIndex,
	OUT KTByteArray&		baValue
);

EXPORTS KTINT32 
KTPKI_GetCertID(
	IN  KT_SESSION_HANDLE	hSession,
	IN  KTCertificate*		pCert, 
	OUT KTCertID *			pCertID
);
					
#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 01:58  Hongwei Yu   created 

#endif
