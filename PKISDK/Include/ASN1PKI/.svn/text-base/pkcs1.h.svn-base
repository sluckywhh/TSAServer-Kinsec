#ifndef _PKCS1_H_
#define _PKCS1_H_

/**
  * @Name: Pkcs#1.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/09 8:38
  * @Description: all pkcs#1 class define 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./ObjID.h"

///////////////////////////////////////////////////////////////////////////////
/* KTAlgorithmIdentifier pkcs1
--
-- When rsaEncryption is used in an AlgorithmIdentifier the parameters 
-- MUST be present and MUST be NULL.
--
rsaEncryption    OBJECT IDENTIFIER ::= { pkcs-1 1 }

--
-- When id-RSAES-OAEP is used in an AlgorithmIdentifier the parameters MUST 
-- be present and MUST be RSAES-OAEP-params.
--
id-RSAES-OAEP    OBJECT IDENTIFIER ::= { pkcs-1 7 }

--
-- When id-pSpecified is used in an AlgorithmIdentifier the parameters MUST 
-- be an OCTET STRING.
--
id-pSpecified    OBJECT IDENTIFIER ::= { pkcs-1 9 }

--
-- When id-RSASSA-PSS is used in an AlgorithmIdentifier the parameters MUST 
-- be present and MUST be RSASSA-PSS-params.
--
id-RSASSA-PSS    OBJECT IDENTIFIER ::= { pkcs-1 10 }

--
-- When the following OIDs are used in an AlgorithmIdentifier the parameters 
-- MUST be present and MUST be NULL.
--
md2WithRSAEncryption       OBJECT IDENTIFIER ::= { pkcs-1 2 }
md5WithRSAEncryption       OBJECT IDENTIFIER ::= { pkcs-1 4 }
sha1WithRSAEncryption      OBJECT IDENTIFIER ::= { pkcs-1 5 }
sha256WithRSAEncryption    OBJECT IDENTIFIER ::= { pkcs-1 11 }
sha384WithRSAEncryption    OBJECT IDENTIFIER ::= { pkcs-1 12 }
sha512WithRSAEncryption    OBJECT IDENTIFIER ::= { pkcs-1 13 }
*/

class KT_LIB_MODE KTAlgorithmIdentifier : public KTTwin
{
protected:
	KTObject* newObj(const char* pc_typeId) const;
};

/*
-- Allowed EME-OAEP and EMSA-PSS digest algorithms.
--
OAEP-PSSDigestAlgorithms    ALGORITHM-IDENTIFIER ::= {
    { OID id-sha1 PARAMETERS NULL   }|
    { OID id-sha256 PARAMETERS NULL }|
    { OID id-sha384 PARAMETERS NULL }|
    { OID id-sha512 PARAMETERS NULL }, 
    ...  -- Allows for future expansion --
}

--
-- Allowed EMSA-PKCS1-v1_5 digest algorithms.
--
PKCS1-v1-5DigestAlgorithms    ALGORITHM-IDENTIFIER ::= {
    { OID id-md2 PARAMETERS NULL    }|
    { OID id-md5 PARAMETERS NULL    }|
    { OID id-sha1 PARAMETERS NULL   }|
    { OID id-sha256 PARAMETERS NULL }|
    { OID id-sha384 PARAMETERS NULL }|
    { OID id-sha512 PARAMETERS NULL } 
}

sha1    HashAlgorithm ::= {
    algorithm   id-sha1, 
    parameters  SHA1Parameters : NULL
}
*/

typedef KTAlgorithmIdentifier KTHashAlgorithm;  //{OAEP-PSSDigestAlgorithms}

/*
-- Allowed mask generation function algorithms. 
-- If the identifier is id-mgf1, the parameters are a HashAlgorithm. 
--
PKCS1MGFAlgorithms    ALGORITHM-IDENTIFIER ::= {
    { OID id-mgf1 PARAMETERS HashAlgorithm }, 
    ...  -- Allows for future expansion --
}

--
-- Default AlgorithmIdentifier for id-RSAES-OAEP.maskGenAlgorithm and
-- id-RSASSA-PSS.maskGenAlgorithm.
--
mgf1SHA1    MaskGenAlgorithm ::= {
    algorithm   id-mgf1, 
    parameters  HashAlgorithm : sha1
}
*/

typedef KTAlgorithmIdentifier   KTMaskGenAlgorithm; //{PKCS1MGFAlgorithms}

/*
-- Allowed algorithms for pSourceAlgorithm.
--
PKCS1PSourceAlgorithms    ALGORITHM-IDENTIFIER ::= {
    { OID id-pSpecified PARAMETERS EncodingParameters }, 
    ...  -- Allows for future expansion --
}
*/

typedef KTOCTETSTRING   KTEncodingParameters;   //(SIZE(0..MAX))

/* 
-- This identifier means that the label L is an empty string, so the digest 
-- of the empty string appears in the RSA block before masking.
--

pSpecifiedEmpty    PSourceAlgorithm ::= {
    algorithm   id-pSpecified, 
    parameters  EncodingParameters : emptyString
}
*/

typedef KTAlgorithmIdentifier   KTPSourceAlgorithm;     //{ {PKCS1PSourceAlgorithms} }

/*
emptyString    EncodingParameters ::= ''H 

--
-- Type identifier definitions for the PKCS #1 OIDs.
--
PKCS1Algorithms    ALGORITHM-IDENTIFIER ::= {
    { OID rsaEncryption              PARAMETERS NULL } |
    { OID md2WithRSAEncryption       PARAMETERS NULL } |
    { OID md5WithRSAEncryption       PARAMETERS NULL } |
    { OID sha1WithRSAEncryption      PARAMETERS NULL } |
    { OID sha256WithRSAEncryption    PARAMETERS NULL } |
    { OID sha384WithRSAEncryption    PARAMETERS NULL } |
    { OID sha512WithRSAEncryption    PARAMETERS NULL } |
    { OID id-RSAES-OAEP PARAMETERS RSAES-OAEP-params } | 
    PKCS1PSourceAlgorithms                             |
    { OID id-RSASSA-PSS PARAMETERS RSASSA-PSS-params } , 
    ...  -- Allows for future expansion --
}*/

///////////////////////////////////////////////////////////////////////////////
#define KT_SM2_LENGTH		32
#define KT_SM2_LENGTH_64	64
#define KT_SM2_LENGTH_MAX	64

#define KT_SM3_LENGTH		32

///////////////////////////////////////////////////////////////////////////////
// KTSM2PublicKey 
class KT_LIB_MODE KTSM2PublicKey : public KTBITSTRING	// 04||X||Y
{
public:
 	int getX(unsigned char *out, int *outlen) const;
	int getY(unsigned char *out, int *outlen) const;

	int getX(KTByteArray& out) const;
	int getY(KTByteArray& out) const;
};

///////////////////////////////////////////////////////////////////////////////
// KTSM2PrivateKey 
class KT_LIB_MODE KTSM2PrivateKey: public KTINTEGER
{
public:
	KTBOOL isPair(const KTSM2PublicKey& pubKey) const;

	int getD(unsigned char *out, int *outlen) const;
	int setD(unsigned char *in, int inlen);

	int getD(KTByteArray& out) const;
	int setD(KTByteArray& in);
};

///////////////////////////////////////////////////////////////////////////////
// KTSM2Cipher 加密数据
class KT_LIB_MODE KTSM2Cipher : public KTSEQUENCE4
{
public:
	KTSM2Cipher()
	{
		PLACE4OBJS(m_x, m_y, m_hash, m_cipherText)
	}

	int getX(unsigned char *out, int *outlen) const;
	int getY(unsigned char *out, int *outlen) const;
	int setX(unsigned char *in, int inlen);
	int setY(unsigned char *in, int inlen);

	int getX(KTByteArray& out) const;
	int getY(KTByteArray& out) const;
	int setX(KTByteArray& in);
	int setY(KTByteArray& in);

	KTINTEGER m_x;						//-- X分量
	KTINTEGER m_y;						//-- Y分量
	KTOCTETSTRING m_hash;				//-- 杂凑值
	KTOCTETSTRING m_cipherText;			//-- 密文
};

///////////////////////////////////////////////////////////////////////////////
// KTSM2Signature 签名数据
class KT_LIB_MODE KTSM2Signature : public KTSEQUENCE2
{
public:
	KTSM2Signature()
	{
		PLACE2OBJS(m_r, m_s)
	}

	int getR(unsigned char *out, int *outlen) const;
	int getS(unsigned char *out, int *outlen) const;
	int setR(unsigned char *in, int inlen);
	int setS(unsigned char *in, int inlen);

	int getR(KTByteArray& out) const;
	int getS(KTByteArray& out) const;
	int setR(KTByteArray& in);
	int setS(KTByteArray& in);

	KTINTEGER m_r;						//-- 签名值的第一部分
	KTINTEGER m_s;						//-- 签名值的第二部分
};

///////////////////////////////////////////////////////////////////////////////
// KTSM2EnvelopedKey 密钥对保护数据
class KT_LIB_MODE KTSM2EnvelopedKey : public KTSEQUENCE4
{
public:
	KTSM2EnvelopedKey()
	{
		PLACE4OBJS(m_symAlgID, m_symEncryptedKey, m_sm2PublicKey, m_sm2EncryptedPrivateKey)
	}

	KTAlgorithmIdentifier m_symAlgID;		//-- 对称密码算法标识
	KTSM2Cipher m_symEncryptedKey;			//-- 对称密钥密文
	KTSM2PublicKey m_sm2PublicKey;			//-- SM2公钥
	KTBITSTRING m_sm2EncryptedPrivateKey;	//-- SM2私钥密文
};

///////////////////////////////////////////////////////////////////////////////
// KTRSAPublicKey 
class KT_LIB_MODE KTRSAPublicKey : public KTSEQUENCE2
{
public:
	KTRSAPublicKey()
	{
		PLACE2OBJS(m_modulus, m_publicExponent)
	}

	KTINTEGER m_modulus;            //-- n
	KTINTEGER m_publicExponent;     //-- e
};

///////////////////////////////////////////////////////////////////////////////
// KTSecretKey //对称密钥
class KT_LIB_MODE KTSecretKey : public KTSEQUENCE2
{
public:
	KTSecretKey()
	{
		PLACE2OBJS(m_type, m_value)
	}

	KTINTEGER	    m_type;
	KTOCTETSTRING   m_value;
};

//::=  { two-prime(0), multi(1) }(CONSTRAINED BY {-- version must be multi if otherPrimeInfos present --})
typedef KTINTEGER KTVersion;    

class KT_LIB_MODE KTOtherPrimeInfo: public KTSEQUENCE3  
{
public:
	KTINTEGER m_prime;				//ri
	KTINTEGER m_exponent;			//di
	KTINTEGER m_coefficient;		//ti

	KTOtherPrimeInfo()
	{
		PLACE3OBJS ( m_prime, m_exponent, m_coefficient)
	}
};

SEQUENCEOF(KTOtherPrimeInfos, KTOtherPrimeInfo)

class KT_LIB_MODE KTRSAPrivateKey: public KTSEQUENCE10
{
public:
	KTINTEGER	m_version;
	KTINTEGER	m_modulus;				// n = p * q
	KTINTEGER	m_publicExponent;		// e 
	KTINTEGER	m_privateExponent;		// d
	KTINTEGER	m_prime1;				// p	
	KTINTEGER	m_prime2;				// q, p > q	
	KTINTEGER	m_exponent1;			// d mod (p - 1) : d1
	KTINTEGER	m_exponent2;			// d mod (q - 1) : d2
	KTINTEGER	m_coefficient;			// (inverse of q) mod p 
	KTOtherPrimeInfos m_otherPrimeInfos;

	KTRSAPrivateKey()
	{
		m_version = 0;
		m_otherPrimeInfos.setOptional();

		PLACE10OBJS(m_version, m_modulus, m_publicExponent,	m_privateExponent, m_prime1,
			m_prime2, m_exponent1, m_exponent2,	m_coefficient, m_otherPrimeInfos)
	}

public:
	void swapPrimes( );
    KTBOOL isPair(const KTRSAPublicKey& pubKey) const;
};

/* 
-- Identifier for default RSAES-OAEP algorithm identifier.
-- The DER Encoding of this is in hexadecimal:
-- (0x)30 0D
--        06 09 
--           2A 86 48 86 F7 0D 01 01 07
--        30 00   
-- Notice that the DER encoding of default values is "empty". 
*/

class KT_LIB_MODE KTRSAES_OAEP_params : public KTSEQUENCE3 {
public:
    KTRSAES_OAEP_params()
    {
        PLACE3OBJS(m_hashAlgorithm, m_maskGenAlgorithm, m_pSourceAlgorithm);
        
        m_hashAlgorithm.setTypeEx(0, TRUE);
        m_maskGenAlgorithm.setTypeEx(1, TRUE);
        m_pSourceAlgorithm.setTypeEx(2, TRUE);
        
        m_hashAlgorithm.setTypeValue(ObjID_id_sha1);
        m_maskGenAlgorithm.setTypeValue(ObjID_id_mgf1, &m_hashAlgorithm);
        m_pSourceAlgorithm.setTypeValue(ObjID_id_pSpecified);
    }

    KTHashAlgorithm     m_hashAlgorithm;      //[0]   DEFAULT sha1, 
    KTMaskGenAlgorithm  m_maskGenAlgorithm;   //[1]   DEFAULT mgf1SHA1, 
    KTPSourceAlgorithm  m_pSourceAlgorithm;   //[2]   DEFAULT pSpecifiedEmpty 
};

/*
rSAES-OAEP-Default-Identifier    RSAES-AlgorithmIdentifier ::= {
    algorithm   id-RSAES-OAEP, 
    parameters  RSAES-OAEP-params : {
        hashAlgorithm       sha1, 
        maskGenAlgorithm    mgf1SHA1, 
        pSourceAlgorithm    pSpecifiedEmpty
    }
}
*/

typedef KTAlgorithmIdentifier   KTRSAES_AlgorithmIdentifier;    //{ {PKCS1Algorithms} }


/* 
-- AlgorithmIdentifier.parameters for id-RSASSA-PSS.
-- Note that the tags in this Sequence are explicit.
-- 
-- Identifier for default RSASSA-PSS algorithm identifier
-- The DER Encoding of this is in hexadecimal:
-- (0x)30 0D
--        06 09 
--           2A 86 48 86 F7 0D 01 01 0A
--        30 00   
-- Notice that the DER encoding of default values is "empty". 
*/

typedef KTINTEGER   KTTrailerField;         //{ trailerFieldBC(1) }

class KT_LIB_MODE KTRSASSA_PSS_params : public KTSEQUENCE4 {
public:
    KTRSASSA_PSS_params()
    {
        PLACE4OBJS(m_hashAlgorithm, m_maskGenAlgorithm, m_saltLength, m_trailerField)
        
        m_hashAlgorithm.setTypeEx(0, TRUE);
        m_maskGenAlgorithm.setTypeEx(1, TRUE);
        m_saltLength.setTypeEx(2, TRUE);
        m_trailerField.setTypeEx(3, TRUE);

        m_hashAlgorithm.setTypeValue(ObjID_id_sha1);
        m_maskGenAlgorithm.setTypeValue(ObjID_id_mgf1, &m_hashAlgorithm);
        m_saltLength = 20;
        m_trailerField = 1;
    }
    KTHashAlgorithm     m_hashAlgorithm;      //[0]    DEFAULT sha1, 
    KTMaskGenAlgorithm  m_maskGenAlgorithm;   //[1]    DEFAULT mgf1SHA1, 
    KTINTEGER           m_saltLength;         //[2]    DEFAULT 20,
    KTTrailerField      m_trailerField;       //[3]    DEFAULT trailerFieldBC
};

/*
rSASSA-PSS-Default-Identifier    RSASSA-AlgorithmIdentifier ::= {
    algorithm   id-RSASSA-PSS, 
    parameters  RSASSA-PSS-params : {
        hashAlgorithm       sha1, 
        maskGenAlgorithm    mgf1SHA1, 
        saltLength          20,
        trailerField        trailerFieldBC
    }
}
*/

typedef KTAlgorithmIdentifier   KTDigestAlgorithm;      //{ {PKCS1-v1-5DigestAlgorithms} }

class KT_LIB_MODE KTDigestInfo : public KTSEQUENCE2 {
public:
    KTDigestInfo()
    {
        PLACE2OBJS(m_digestAlgorithm, m_digest)
    }

    KTBOOL equal(const KTDigestInfo& digestInfo) const;  

    KTDigestAlgorithm   m_digestAlgorithm;
    KTOCTETSTRING       m_digest;  
};


#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/09 10:00   Hongwei Yu   created 
///   1.0.0.1  2003/04/11 09:39   Hongwei Yu   refactory
///   1.0.0.2  2003/12/22 16:58   Hongwei Yu   RSAPrivateKey default version from 1 to 0 

#endif //_PKCS1_H_
