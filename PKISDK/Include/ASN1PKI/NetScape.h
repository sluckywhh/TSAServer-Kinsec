#ifndef _NETSCAPE_H_INCLUDE
#define _NETSCAPE_H_INCLUDE

/**
  * @Name: NetScape.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/09 21:07
  * @Description: all NetScape class define 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

//#include "./PkcsBase.h"
#include "./X.509.h"
//////////////////////////////////////////////////////////////////
//KTPublicKeyAndChallenge  for NetScape Certfication Request.
class KT_LIB_MODE KTPublicKeyAndChallenge : public KTSEQUENCE2
{
public:
	KTPublicKeyAndChallenge()
	{
		PLACE2OBJS(m_spki, m_challenge)
	}

	KTSubjectPublicKeyInfo	m_spki;
	KTIA5String				m_challenge;
};

//////////////////////////////////////////////////////////////////
//KTSignedPublicKeyAndChallenge
class KT_LIB_MODE KTSignedPublicKeyAndChallenge : public KTSEQUENCE3
{
public:
	KTSignedPublicKeyAndChallenge()
	{
        PLACE3OBJS(m_publicKeyAndChallenge, m_signatureAlgorithm, m_signature)
    }
	
    KTPublicKeyAndChallenge     m_publicKeyAndChallenge;
	KTAlgorithmIdentifier	    m_signatureAlgorithm;
	KTBITSTRING				    m_signature;    
};

//////////////////////////////////////////////////////////////////
//KTNetScapeCertType
//bit-0	SSL client - this cert is certified for SSL client authentication use
//bit-1	SSL server - this cert is certified for SSL server authentication use
//bit-2	S/MIME - this cert is certified for use by clients(New in PR3)
//bit-3	Object Signing - this cert is certified for signing objects such as Java applets and plugins(New in PR3)
//bit-4	Reserved - this bit is reserved for future use
//bit-5	SSL CA - this cert is certified for issuing certs for SSL use
//bit-6	S/MIME CA - this cert is certified for issuing certs for S/MIME use(New in PR3)
//bit-7	Object Signing CA - this cert is certified for issuing certs for Object Signing(New in PR3)

typedef KTBITSTRING KTNetScapeCertType;
enum enumNetScapeCertType
{
    NSCertTypeNone              = 0x000,
    NSCertTypeSslClient         = 0x001,
    NSCertTypeSslServer         = 0x002,
    NSCertTypeS_Mime            = 0x004,
    NSCertTypeObjectSigning     = 0x008,
    NSCertTypeReserved          = 0x010,
    NSCertTypeSslCA             = 0x020,
    NSCertTypeS_MimeCA          = 0x040,
    NSCertTypeObjectSigningCA   = 0x080,
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/09 21:53   Hongwei Yu   created 

#endif  //_NETSCAPE_H_INCLUDE
