#ifndef _PKCS5_H_
#define _PKCS5_H_

/**
  * @Name: Pkcs#5.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/07/03 14:12
  * @Description: all pkcs#5 class define 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./ObjID.h"
#include "./pkcs1.h"

///////////////////////////////////////////////////////////////////////////////
/* 
PKCS5v2-0 {iso(1) member-body(2) us(840) rsadsi(113549)
  pkcs(1) pkcs-5(5) modules(16) pkcs5v2-0(1)}

DEFINITIONS ::= BEGIN

-- Basic object identifiers

rsadsi OBJECT IDENTIFIER ::= 
  {iso(1) member-body(2) us(840) 113549}
pkcs OBJECT IDENTIFIER ::= {rsadsi 1}
pkcs-5 OBJECT IDENTIFIER ::= {pkcs 5}

-- Basic types and classes

AlgorithmIdentifier { ALGORITHM-IDENTIFIER:InfoObjectSet } ::= 
SEQUENCE {
  algorithm ALGORITHM-IDENTIFIER.&id({InfoObjectSet}),
  parameters ALGORITHM-IDENTIFIER.&Type({InfoObjectSet}
    {@algorithm}) OPTIONAL }

ALGORITHM-IDENTIFIER ::= TYPE-IDENTIFIER

-- PBKDF2

PBKDF2Algorithms ALGORITHM-IDENTIFIER ::= 
  { {PBKDF2-params IDENTIFIED BY id-PBKDF2}, ...}

id-PBKDF2 OBJECT IDENTIFIER ::= {pkcs-5 12}

algid-hmacWithSHA1 AlgorithmIdentifier {{PBKDF2-PRFs}} ::=
  {algorithm id-hmacWithSHA1, parameters NULL : NULL}

PBKDF2-params ::= SEQUENCE {
  salt CHOICE {
    specified OCTET STRING,
    otherSource AlgorithmIdentifier {{PBKDF2-SaltSources}}
  },
  iterationCount INTEGER (1..MAX),
  keyLength INTEGER (1..MAX) OPTIONAL,
  prf AlgorithmIdentifier {{PBKDF2-PRFs}} DEFAULT
    algid-hmacWithSHA1 }
*/
KT_BEGIN_NAMESPACE(KTPKI)

/*PBKDF1 (P, S, c, dkLen)
Options:	Hash	underlying hash function
Input:	P	password, an octet string
	S	salt, an eight-octet string
	c	iteration count, a positive integer
	dkLen	intended length in octets of derived key, a positive integer, at most 16 for MD2 or MD5 and 20 for SHA-1
Output:	DK	derived key, a dkLen-octet string
*/
typedef int (*HASH_FUNCTION)(const KTBYTE* pbBuf, KTUINT unBufLen, KTBYTE* pbHash, KTUINT* punHashLen);
int PBKDF1(HASH_FUNCTION hashFunction, KTUINT unHashLen, 
		   const KTBYTE* pbPassword, KTUINT unPasswordLen,
		   const KTBYTE* pbSalt, KTUINT unSaltLen, int nIterationCount,
		   KTUINT unDKLen, KTBYTE* pbDerivedkey);

/*PBKDF2 (P, S, c, dkLen)
Options:	PRF	underlying pseudorandom function (hLen denotes the length in octets of the pseudorandom function output)
Input:	P	password, an octet string
	S	salt, an octet string
	c	iteration count, a positive integer
	dkLen	intended length in octets of the derived key, a positive integer, at most (232 - 1) ¡ä hLen
Output:	DK	derived key, a dkLen-octet string
*/
typedef int (* PSEUDORANDOM_FUNCTION)(const KTBYTE* pbMACKey,  KTUINT unMACKeyLen, const KTBYTE* pbText,  KTUINT unTextLen,
										KTBYTE	pbHMAC[]);
int PBKDF2(PSEUDORANDOM_FUNCTION PRF, KTUINT unPseudorandomLen, 
		   const KTBYTE* pbPassword, KTUINT unPasswordLen,
		   const KTBYTE* pbSalt, KTUINT unSaltLen, int nIterationCount,
		   KTUINT unDKLen, KTBYTE* pbDerivedkey);

int GetPaddingString(KTUINT unMessageLen, KTBYTE* punPSLen, KTBYTE* pbPS);

int PBES1(const KTBYTE* pbPassword, KTUINT unPasswordLen,
		  const KTBYTE* pbMessage, KTUINT unMessageLen,
		  const KTChar* pc_pbeOID, KTUINT* punCipherTextLen, KTBYTE* pbCipherText );

//PBES2

//PBMAC1


KT_END_NAMESPACE(KTPKI)

class KT_LIB_MODE KTSalt : public KTCHOICE2 {
public:
	KTSalt()
	{
		PLACE2OBJS(m_specified, m_otherSource);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
	}
    KTOCTETSTRING m_specified;
    KTAlgorithmIdentifier m_otherSource;
  };

class KT_LIB_MODE KTPBKDF2_params :public KTSEQUENCE4 {
public:
	KTPBKDF2_params()
	{
		PLACE4OBJS(m_salt, m_iterationCount, m_keyLength, m_prf);
		m_keyLength.setOptional();
		m_prf.setTypeValue(ObjID_id_hmacWithSHA1);
	}

  KTSalt		m_salt;
  KTINTEGER		m_iterationCount;	//(1..MAX),
  KTINTEGER		m_keyLength;		//(1..MAX) OPTIONAL,
  KTAlgorithmIdentifier m_prf;		//{{PBKDF2-PRFs}} DEFAULT algid-hmacWithSHA1 
};

/*
PBKDF2-SaltSources ALGORITHM-IDENTIFIER ::= { ... }

PBKDF2-PRFs ALGORITHM-IDENTIFIER ::=
  { {NULL IDENTIFIED BY id-hmacWithSHA1}, ... }

 -- PBES1

PBES1Algorithms ALGORITHM-IDENTIFIER ::= 
  { {PBEParameter IDENTIFIED BY pbeWithMD2AndDES-CBC} |
    {PBEParameter IDENTIFIED BY pbeWithMD2AndRC2-CBC} |
    {PBEParameter IDENTIFIED BY pbeWithMD5AndDES-CBC} |
    {PBEParameter IDENTIFIED BY pbeWithMD5AndRC2-CBC} |
    {PBEParameter IDENTIFIED BY pbeWithSHA1AndDES-CBC} |
    {PBEParameter IDENTIFIED BY pbeWithSHA1AndRC2-CBC}, ...}

pbeWithMD2AndDES-CBC OBJECT IDENTIFIER ::= {pkcs-5 1}
pbeWithMD2AndRC2-CBC OBJECT IDENTIFIER ::= {pkcs-5 4}
pbeWithMD5AndDES-CBC OBJECT IDENTIFIER ::= {pkcs-5 3}
pbeWithMD5AndRC2-CBC OBJECT IDENTIFIER ::= {pkcs-5 6}
pbeWithSHA1AndDES-CBC OBJECT IDENTIFIER ::= {pkcs-5 10}
pbeWithSHA1AndRC2-CBC OBJECT IDENTIFIER ::= {pkcs-5 11}

PBEParameter ::= SEQUENCE {
  salt OCTET STRING (SIZE(8)),
  iterationCount INTEGER }
*/

class KT_LIB_MODE KTPBEParameter :public KTSEQUENCE2 {
public:
	KTPBEParameter()
	{
		PLACE2OBJS(m_salt, m_iterationCount);
	}
  KTOCTETSTRING m_salt;		//(SIZE(8)),
  KTINTEGER		m_iterationCount;
};

/*
-- PBES2

PBES2Algorithms ALGORITHM-IDENTIFIER ::= 
  { {PBES2-params IDENTIFIED BY id-PBES2}, ...}

id-PBES2 OBJECT IDENTIFIER ::= {pkcs-5 13}

PBES2-params ::= SEQUENCE {
  keyDerivationFunc AlgorithmIdentifier {{PBES2-KDFs}},
  encryptionScheme AlgorithmIdentifier {{PBES2-Encs}} }
*/
class KT_LIB_MODE KTPBES2_params :public KTSEQUENCE2 {
public:
	KTPBES2_params()
	{
		PLACE2OBJS(m_keyDerivationFunc, m_encryptionScheme);	
	}
  KTAlgorithmIdentifier	m_keyDerivationFunc;	//{{PBES2-KDFs}},
  KTAlgorithmIdentifier m_encryptionScheme;		//{{PBES2-Encs}} 
};

/*
PBES2-KDFs ALGORITHM-IDENTIFIER ::=
  { {PBKDF2-params IDENTIFIED BY id-PBKDF2}, ... }

PBES2-Encs ALGORITHM-IDENTIFIER ::= { ... }

-- PBMAC1

PBMAC1Algorithms ALGORITHM-IDENTIFIER ::= 
  { {PBMAC1-params IDENTIFIED BY id-PBMAC1}, ...}

id-PBMAC1 OBJECT IDENTIFIER ::= {pkcs-5 14}

PBMAC1-params ::=  SEQUENCE {
  keyDerivationFunc AlgorithmIdentifier {{PBMAC1-KDFs}},
  messageAuthScheme AlgorithmIdentifier {{PBMAC1-MACs}} }*/

class KT_LIB_MODE KTPBMAC1_params :public KTSEQUENCE2 {
public:
	KTPBMAC1_params()
	{
		PLACE2OBJS(m_keyDerivationFunc, m_messageAuthScheme);
	}
		
  KTAlgorithmIdentifier m_keyDerivationFunc;  //{{PBMAC1-KDFs}},
  KTAlgorithmIdentifier m_messageAuthScheme;  //{{PBMAC1-MACs}} 
};

/*
PBMAC1-KDFs ALGORITHM-IDENTIFIER ::=
  { {PBKDF2-params IDENTIFIED BY id-PBKDF2}, ... }

PBMAC1-MACs ALGORITHM-IDENTIFIER ::= { ... }

-- Supporting techniques

digestAlgorithm OBJECT IDENTIFIER     ::= {rsadsi 2}
encryptionAlgorithm OBJECT IDENTIFIER ::= {rsadsi 3}

SupportingAlgorithms ALGORITHM-IDENTIFIER ::=
  { {NULL IDENTIFIED BY id-hmacWithSHA1} |
    {OCTET STRING (SIZE(8)) IDENTIFIED BY desCBC} |
    {OCTET STRING (SIZE(8)) IDENTIFIED BY des-EDE3-CBC} |
    {RC2-CBC-Parameter IDENTIFIED BY rc2CBC} |
    {RC5-CBC-Parameters IDENTIFIED BY rc5-CBC-PAD}, ... }

id-hmacWithSHA1 OBJECT IDENTIFIER ::= {digestAlgorithm 7}

desCBC OBJECT IDENTIFIER ::=
  {iso(1) identified-organization(3) oiw(14) secsig(3)
    algorithms(2) 7} -- from OIW

des-EDE3-CBC OBJECT IDENTIFIER ::= {encryptionAlgorithm 7}
*/

/*
rc2CBC OBJECT IDENTIFIER ::= {encryptionAlgorithm 2}

RC2-CBC-Parameter ::= SEQUENCE {
  rc2ParameterVersion INTEGER OPTIONAL,
  iv OCTET STRING (SIZE(8)) }
*/
class KT_LIB_MODE KTRC2_CBC_Parameter :public KTSEQUENCE2 {
public:
	KTRC2_CBC_Parameter()
	{
		PLACE2OBJS(m_rc2ParameterVersion, m_iv);
		m_rc2ParameterVersion.setOptional();
	}
  KTINTEGER		m_rc2ParameterVersion;  //OPTIONAL,
  KTOCTETSTRING	m_iv;  //(SIZE(8)) 
};

/*
rc5-CBC-PAD OBJECT IDENTIFIER ::= {encryptionAlgorithm 9} 

RC5-CBC-Parameters ::= SEQUENCE {
  version INTEGER {v1-0(16)} (v1-0),
  rounds INTEGER (8..127),
  blockSizeInBits INTEGER (64 | 128),
  iv OCTET STRING OPTIONAL }
*/
class KT_LIB_MODE KTRC5_CBC_Parameters :public KTSEQUENCE4 {
public:
	KTRC5_CBC_Parameters()
	{
		PLACE4OBJS(m_version, m_rounds, m_blockSizeInBits, m_iv);
		m_version=16;
		m_iv.setOptional();
	}
  KTINTEGER		m_version;			//{v1-0(16)} (v1-0),
  KTINTEGER		m_rounds;			//(8..127),
  KTINTEGER		m_blockSizeInBits;	// (64 | 128),
  KTOCTETSTRING	m_iv;				//OPTIONAL 
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/07/03 14:12   Hongwei Yu   created 

#endif //_PKCS5_H_
