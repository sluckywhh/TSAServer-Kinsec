// =================================================================================
//  File    : TSAEngine.h
//  Purpose : 时间戳服务器签名引擎类
//  Author  : 林灶钢
//  Created : 2006-11-21
//  CopyRight (C) Fujian Kinsec
// =================================================================================

#ifndef __TSAENGINE_H__
#define __TSAENGINE_H__

// =================================================================================
//  Include Header Files
#include "PKIDef.h"
#include "ASN1PKI.hpp"
#include <openssl/x509.h>

// =================================================================================
//  Macro Definitionclass 
class KTTimeStampResp : public KTSEQUENCE2
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

// =================================================================================
//  Class Definition
class CTSAEngine
{
public : 
	// = Default constructor.
	CTSAEngine();

	// = Default destructor.
	virtual ~CTSAEngine();

	// = Load certificate.
	int loadCertificate( const KTCertificate & cert, const KTByteArray & rPriKey );

	// = Get Tsa certificate.
	int getCertData( std::string & certData );

	// = Create time stamp.
	int createTimeStamp( const char * szReqData, std::string & serialNumber, 
		std::string & tsData, std::string & respData );
	
	void InitSerCer(X509* SerCert, EVP_PKEY* SerPriKey, X509* SerCertCa, X509* SerCertRoot, time_t CertBeginValid, time_t CertEndValid);
	
	int CreateTSReq(unsigned char *data, int datalen,	unsigned char *tsreq,	int *tsreqlen	);
		
	int CreateTSResp( unsigned char* tsreq, int tsreqlen,	X509* rootcacert, X509* cacert,
	X509* cert, EVP_PKEY* pkey,	unsigned char* tsresp, int* tsresplen,	unsigned char* tstoken,	int* tstokenlen, std::string& serialNumber);
	
	int VerifyTSToken( unsigned char* data, int datalen, X509* rootcacert,  X509* cacert, unsigned char* tstoken, int tstokenlen);
	
	int ParseTSToken(unsigned char* tstoken, int tstokenlen, char* pszRes, int flag);
	
	X509* GetSerCer(){ return m_SerCert; }
	
	EVP_PKEY* GetSerPriKey(){ return m_SerPriKey; }
	
	X509* GetSerCertCa(){ return m_SerCertCa; }
	
	X509* GetSerCertRoot(){ return m_SerCertRoot; }
	
	time_t m_CertBeginValid;
	
	time_t m_CertEndValid;
	
		
private :
	// = Base64 encode.
	void base64Encode( const unsigned char * pData, unsigned int unDataLen, std::string & data );

	// = Base64 decode.
	void base64Decode( const char * szData, unsigned int unDataLen, KTByteArray & rData );

	// = Set tst info.
	void setTSTInfo( const KTTimeStampReq & tsReq, KTTSTInfo * pTSTInfo );

	// = Set signer info.
	void setSignerInfo( const KTByteArray & rData, KTSignerInfos * pSignerInfos );

	// = tsa cert.
	KTCertificate m_cert;

	// = tsa cert private key object.
	CRSAPrivateKey * m_pPriKey;
	
	X509* m_SerCert;
	
	EVP_PKEY* m_SerPriKey;
	
	X509* m_SerCertCa;
		
	X509* m_SerCertRoot;
	

};

// =================================================================================
//  1.00    2006-11-21    Created By LinZaoGang
//  File End 
#endif  // __TSAENGINE_H__  
