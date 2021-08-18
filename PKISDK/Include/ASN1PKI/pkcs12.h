#ifndef		__PKCS12_H__
#define		__PKCS12_H__

/**
  * @Name: Pkcs#12.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2000/11/12 20:03
  * @Description: all pkcs#12 class define 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./pkcs1.h"
#include "./pkcs5.h"
#include "./pkcs7.h"

KT_BEGIN_NAMESPACE(KTPKI)

int HMAC(
	const KTBYTE*	pbMACKey,
	KTUINT			unKeyLen,	
	const KTBYTE*   pbText,
	KTUINT			unTextLen,
	HASH_FUNCTION	hashFunction,
	KTUINT			unHashLen,
	KTBYTE*			pbHMAC
);

int HMAC_SHA1(
	const KTBYTE*	pbMACKey,
	KTUINT			unMACKeyLen,	
	const KTBYTE*   pbText,
	KTUINT			unTextLen,
	KTBYTE			pbHMAC[20]
);

const KTBYTE pkcs12HMAC		    =	0x04;
const KTBYTE pbe128BitRC4	    =	0x11;
const KTBYTE pbe40BitRC4	    =	0x21;
const KTBYTE pbe3KeyDESCBC	    =   0x33;
const KTBYTE pbe2KeyDESCBC	    =   0x43; 
const KTBYTE pbe128BitRC2CBC    =   0x53;
const KTBYTE pbe40BitRC2CBC	    =   0x63;

KTBOOL GenarateKeyWithSHA1(
	KTBYTE					flag,
	const KTOCTETSTRING		&octSalt,
	const KTINTEGER			&integer,
	const KTBYTE*		    pPass,
	int						nPassLen,
	KTByteArray				&rKey,
	KTByteArray				&rIV,
	KTByteArray				&rMAC
);

void Modify(
	KTByteArray				&rI,
	const KTByteArray		&rB
);

void SetParityBit(
	KTByteArray				&rKey
);

void SetParityBit( KTBYTE* pbKey, KTUINT unKeyLen);				

int CryptContent(
	KTBOOL				    bEncrypt,
	const KTBYTE*			pPass,
	int						nPassLen, 
	const KTAlgorithmIdentifier	&algo,
	const KTOCTETSTRING		&oct,
	KTByteArray				&rResult
);

void GetOctetStringForEncrypted(
	const KTObject			&obj,
	KTOCTETSTRING			&oct,
	int						n
);

KT_END_NAMESPACE(KTPKI)


SEQUENCEOF(KTRSAPrivateKeySequence, KTRSAPrivateKey)
SEQUENCEOF(KTSM2PrivateKeySequence, KTSM2PrivateKey)

class KT_LIB_MODE KTMacData : public KTSEQUENCE3
{
public:
	KTMacData()
	{	
		m_iterations= 1;
		m_iterations.setOptional(); ////for old version
		PLACE3OBJS(m_mac, m_macSalt, m_iterations)
	}

	KTDigestInfo		m_mac;
	KTOCTETSTRING		m_macSalt;
	KTINTEGER			m_iterations;
};

/*
AuthenticatedSafe ::= SEQUENCE OF ContentInfo
	-- Data if unencrypted
	-- EncryptedData if password-encrypted
	-- EnvelopedData if public key-encrypted
*/

SEQUENCEOF(KTContentInfoSequence, KTContentInfo)
class KT_LIB_MODE KTAuthenticatedSafe: public KTContentInfoSequence
{
public:    
	int getValue(
		const KTBYTE*		pPass,
		int					nPassLen, 
		KTRSAPrivateKeySequence		&prvKeys, 
		KTSM2PrivateKeySequence		&sm2PrvKeys, 
		KTCertificates	    &certs,
		KTCRLSequence		&crls) const;


	int setValue(
		const KTBYTE*			pPass,
		int						nPassLen, 
		const char* const		pc_encryptAlgoID,
		const KTRSAPrivateKeySequence	&prvKeys, 
		const KTSM2PrivateKeySequence	&sm2PrvKeys, 
		const KTCertificates	&certs,
		const KTCRLSequence		&crls);
};

class KT_LIB_MODE KTPFX : public KTSEQUENCE3
{
public:
	KTPFX()
	{
		m_version=3;
		m_macData.setOptional();
		PLACE3OBJS(m_version, m_authSafe, m_macData)
	}

	KTINTEGER		m_version;
	KTContentInfo	m_authSafe;
	KTMacData		m_macData;
	
/*****function member****/
  	int	checkDigest(
		const KTBYTE*			pPass,
		int					nPassLen
	);

	int getValue(KTAuthenticatedSafe& authSafe) const;
	int getValue(
		const KTBYTE*			    pPass,
		int					        nPassLen, 
		KTRSAPrivateKeySequence		&prvKeys, 
		KTSM2PrivateKeySequence		&sm2PrvKeys, 
		KTCertificates		        &certs,
		KTCRLSequence               &crls) const;

	int setValue(
		const KTBYTE*			pPass,
		int					    nPassLen,
		KTAuthenticatedSafe	    &authSafe);

	int setValue(
		const KTBYTE*			pPass,
		int						nPassLen,
		const char* const		pc_encryptAlgoID,
		const KTRSAPrivateKeySequence	&prvKeys,
		const KTSM2PrivateKeySequence	&sm2PrvKeys,
		const KTCertificates	&certs,
		const KTCRLSequence		&crls);
};

/*
PKCS12AttrSet ATTRIBUTE ::= {
	friendlyName |
	localKeyId,
	... -- Other attributes are allowed
}
*/
class KT_LIB_MODE KTPKCS12AttributeSet : public KTAttributeSet
{
public:
	int createKeyAttributeSet();
	int createUserCertAttributeSet();
};

class KT_LIB_MODE KTThree : public KTSequence
{
public:
	KTThree(){ m_pValue=NULL; m_attributes.setOptional(); };
	virtual ~KTThree(){ delete m_pValue; };

	KTBOOL isValueExist() const { return m_pValue!=NULL; };
	const KTObjectIdentifier& typeObj() const { return m_type; };
	const KTObject* valueObj() const { return m_pValue; };
	const KTPKCS12AttributeSet& attributesObj() const { return m_attributes; };

	KTPKCS12AttributeSet& attributesObj() { return m_attributes; };
	int setTypeValue(const char* pc_objId, const KTObject* pObj=NULL);

	int modifyLen();	

protected:
	virtual KTObject* newObj(const char* pc_typeId) const=0;
	int createContent();
	void outputContentIn() const;

	KTObjectIdentifier		m_type;
	KTObject*				m_pValue;
	KTPKCS12AttributeSet	m_attributes;
};

class KT_LIB_MODE KTGMSM2PrivateKey: public KTSEQUENCE4
{
public:
	KTGMSM2PrivateKey()
	{	
		m_version = 1;
		m_privateKeyAlgorithmParameter.setTypeEx(0, TRUE);
		m_privateKeyAlgorithmParameter.setOptional();
		m_gmsm2publicKey.setTypeEx(1, TRUE);
		m_gmsm2publicKey.setOptional();
		PLACE4OBJS(m_version, m_gmsm2privateKey, m_privateKeyAlgorithmParameter, m_gmsm2publicKey)
	}

	KTINTEGER				m_version;
	KTOCTETSTRING			m_gmsm2privateKey;
	KTObjectIdentifier		m_privateKeyAlgorithmParameter;
	KTBITSTRING				m_gmsm2publicKey;
};

//define in PKCS#8
class KT_LIB_MODE KTPrivateKeyInfo: public KTSEQUENCE4
{
public:
	KTPrivateKeyInfo()
	{	
		m_version= 0;
		m_attributes.setTypeEx(0, FALSE);
		m_attributes.setOptional();
		PLACE4OBJS(m_version, m_privateKeyAlgorithm, m_privateKey, m_attributes)
	}

	KTINTEGER				m_version;
	KTAlgorithmIdentifier	m_privateKeyAlgorithm;
	KTOCTETSTRING			m_privateKey;
	KTPKCS12AttributeSet	m_attributes;
};

class KT_LIB_MODE KTKeyBag: public KTPrivateKeyInfo
{ 
public:
	int setValue(const KTRSAPrivateKey &privateKey); 
	int setValue(const KTSM2PrivateKey &privateKey); 
};

class KT_LIB_MODE KTEncryptedPrivateKeyInfo: public KTSEQUENCE2 
{
public:
	KTEncryptedPrivateKeyInfo()
	{
		PLACE2OBJS(m_encryptionAlgorithm, m_encryptedData)
	}

	KTAlgorithmIdentifier		m_encryptionAlgorithm; 
	KTOCTETSTRING				m_encryptedData;
};

class KT_LIB_MODE KTPKCS8ShroudedKeyBag: public KTEncryptedPrivateKeyInfo
{
public:
	int setValue(
		const KTBYTE*			pPass,
		int						nPassLen,
		const char* const	    pc_encryptAlgoID,
		const KTKeyBag&			keyBag
	);
};

/*
x509Certificate BAG-TYPE ::=
	{OCTET STRING IDENTIFIED BY {certTypes 1}}
	-- DER-encoded X.509 certificate stored in OCTET STRING
sdsiCertificate BAG-TYPE ::=
	{IA5String IDENTIFIED BY {certTypes 2}}
	-- Base64-encoded SDSI certificate stored in IA5String
*/
class KT_LIB_MODE KTCertBag: public KTContentInfo
{
public:
	int getValue(KTCertificate &cert) const;
	int setValue(const KTCertificate& cert);
};

/*
x509CRL BAG-TYPE ::=
	{OCTET STRING IDENTIFIED BY {crlTypes 1}}
	-- DER-encoded X.509 CRL stored in OCTET STRING
*/
class KT_LIB_MODE KTCRLBag: public KTContentInfo
{
public:
    int setValue(const KTCRL& crl);    
	int getValue(KTCRL &crl) const;
};

class KT_LIB_MODE KTSecretBag: public KTContentInfo
{
public:
    int setvalue(const KTSecretKey& secretKey);    //	for future extensions
    int getValue(KTSecretKey& secretKey) const;    //	for future extensions
};

/*
keyBag 	  BAG-TYPE ::= 
	{KeyBag IDENTIFIED BY {bagtypes 1}}
pkcs8ShroudedKeyBag BAG-TYPE ::=
	{PKCS8ShroudedKeyBag IDENTIFIED BY {bagtypes 2}}
certBag BAG-TYPE ::= 
	{CertBag IDENTIFIED BY {bagtypes 3}}
crlBag BAG-TYPE ::=
	{CRLBag IDENTIFIED BY {bagtypes 4}}
secretBag BAG-TYPE ::=    
	{SecretBag IDENTIFIED BY {bagtypes 5}}
safeContentsBag BAG-TYPE ::=
	{SafeContents IDENTIFIED BY {bagtypes 6}}
*/
class KT_LIB_MODE KTSafeBag : public KTThree
{
public:
	int getValue(
		const KTBYTE*			    pPass,
		int					        nPassLen,
		KTRSAPrivateKeySequence		&prvKeys, 
		KTSM2PrivateKeySequence		&sm2PrvKeys, 
		KTCertificates		        &certs,
		KTCRLSequence		        &crls) const;

	int setValue(const KTKeyBag& keyBag );
	int setValue(const KTPKCS8ShroudedKeyBag& shroudedKeyBag);
	int setValue(const KTCertBag& certBag);
	int setValue(const KTCRLBag& crlBag);
protected:
	KTObject* newObj(const char* pc_typeId) const;

};

SEQUENCEOF(KTSafeBagSequence,KTSafeBag)
class KT_LIB_MODE KTSafeContents: public KTSafeBagSequence
{
public:
	int getValue(
		const KTBYTE*			    pPass,
		int					        nPassLen,
		KTRSAPrivateKeySequence		&prvKeys, 
		KTSM2PrivateKeySequence		&sm2PrvKeys, 
		KTCertificates		        &certs,
		KTCRLSequence  		        &crls
	) const;

	int setValue(
		const KTBYTE*			pPass,
		int						nPassLen,
		const char* const		pc_encryptAlgoID,
		const KTRSAPrivateKeySequence	&prvKeys,
		const KTSM2PrivateKeySequence	&sm2PrvKeys
	);

	int setValue(const KTCertificates &certs);
    int setValue(const KTCRLSequence &crls);
};

class KT_LIB_MODE KTEncryptedData : public KTSEQUENCE2
{
public:
	KTEncryptedData()
	{	
		m_version = 0;
		PLACE2OBJS(m_version, m_encryptedContentInfo)
	}

	KTINTEGER				m_version;
	KTEncryptedContentInfo	m_encryptedContentInfo;

	int setValue(
		const KTBYTE*		pPass,
		int					nPassLen,
		const char* const	pc_encryptAlgoID,
		const KTSafeContents &safeContents
	);

};

///for encrypt and decrypt
class KT_LIB_MODE KTPKCS12PbeParams: public KTSEQUENCE2
{
 public:
 	KTPKCS12PbeParams()
 	{
 		PLACE2OBJS(m_salt, m_iterations)
 	}

	KTOCTETSTRING	m_salt;
	KTINTEGER		m_iterations;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/09 20:58   Hongwei Yu   created 
///   1.0.0.1  2003/07/08 21:59   Hongwei Yu   m_attributes in PrivateKeyInfo is implicit 
///   1.0.0.1  2003/12/27 20:12   Hongwei Yu   added last pop paragma for push first 
	
#endif	//	__PKCS12_H__
