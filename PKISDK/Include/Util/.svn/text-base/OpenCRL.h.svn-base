// OpenCRL.h: interface for the COpenCRL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENCRL_H__052C236D_F83E_4E60_A1A0_DBEB7FD2012C__INCLUDED_)
#define AFX_OPENCRL_H__052C236D_F83E_4E60_A1A0_DBEB7FD2012C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KTCertificate;

class COpenCRL  
{
public:
	COpenCRL(){}
	virtual ~COpenCRL(){}

public:
	// Constructing a CRL from a continued data block of DER coded.
	int create(const void* pData, unsigned int nSize){
		return 0;
	}

	// Searching certificate in CRL. If it found target 
	// then it return the revoked time of the certificate ( GMT Time ).
	bool isCertIn(const KTCertificate& cert, struct tm *rtm = NULL, bool* bGetTime = NULL) const{
		return false;
	}
	bool isCertIn(const void* pData, unsigned int nSize, struct tm *rtm = NULL, bool* bGetTime = NULL) const{
		return false;
	}

	// Receiving the CRL last update time
	bool getLastUpdate( struct tm *rtm ) const{
		return false;
	}

	// Receiving the CRL next update time
	bool getNextUpdate( struct tm *rtm ) const{
		return false;
	}

	// Receiving the CRL issue ( string format )
	// The return value according to KTErr.h
	int getIssue( char* pIssue, unsigned int nLen ) const{
		return 0;
	}
	int getIssue( KTString& sIssue ) const{
		return 0;
	}

	// Comparing the issue between specify certificate and the CRL.
	bool isSameIssue( const KTCertificate& cert ) const{
		return false;
	}
	// pData : DER coded certificate data.
	bool isSameIssue( const void* pData, unsigned int nSize ) const{
		return false;
	}
	bool isSameIssue( const COpenCRL* pOpenCRL ) const{
		return false;
	}

	// Verify Sign
	bool VerifySign( const KTCertificate& cert ) const{
		return false;
	}
	// pData : DER coded single X509 certificate
	bool VerifySign( const void* pData, unsigned int nSize ) const{
		return false;
	}


private:
	bool m_bCreated;
	//X509_CRL* m_pCRL;
};

#endif // !defined(AFX_OPENCRL_H__052C236D_F83E_4E60_A1A0_DBEB7FD2012C__INCLUDED_)
