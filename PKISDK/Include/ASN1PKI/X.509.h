#ifndef _X509CERT_H_
#define _X509CERT_H_

/**
  * @Name: X.509.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/10 00:53
  * @Description: all X.509 certificate about class define
  * @Document: rfc2511.txt
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./pkcs1.h"
#include "./pkcs10.h"

class KT_LIB_MODE KTX520name : public KTCHOICE5
{
public:
    KTX520name()
    {
        PLACE5OBJS(m_teletexString, m_printableString, m_universalString, m_utf8String, m_bmpString);
		OptionPlacedObjs(5);
		//ChoiceFirstPlacedObj();				// uncompatible with chinese language
		choiceObj(m_ppObj[4]);
    }
    
    KTTeletexString     m_teletexString;          //(SIZE (1..ub-name)),
    KTPrintableString   m_printableString;        //(SIZE (1..ub-name)),
    KTUniversalString   m_universalString;        //(SIZE (1..ub-name)),
    KTUTF8String        m_utf8String;             //(SIZE (1..ub-name)),
    KTBMPString         m_bmpString;              //(SIZE (1..ub-name)),
};

typedef KTX520name KTX520CommonName;
typedef KTX520name KTX520LocalityName;
typedef KTX520name KTX520StateOrProvinceName;
typedef KTX520name KTX520OrganizationName;
typedef KTX520name KTX520OrganizationalUnitName;
typedef KTX520name KTX520Title;

//typedef KTX520name KTX520dnQualifier;
typedef KTX520name KTX520countryName; 
typedef KTPrintableString KTX520dnQualifier;
//typedef KTPrintableString KTX520countryName;
typedef KTX520name KTDirectoryString;

#define JIT_ERR_Pkcs9email
#ifndef JIT_ERR_Pkcs9email
	typedef KTIA5String KTPkcs9email;
	//typedef KTX520name KTPkcs9email;
#else
	class KT_LIB_MODE KTPkcs9email : public KTCHOICE6
	{
	public:
		KTPkcs9email()
		{
			PLACE6OBJS(m_ia5String, m_teletexString, m_printableString, m_universalString, m_utf8String, m_bmpString);
			OptionPlacedObjs(6);
			ChoiceFirstPlacedObj();
		}
    	
		KTIA5String			m_ia5String;
		KTTeletexString     m_teletexString;          //(SIZE (1..ub-name)),
		KTPrintableString   m_printableString;        //(SIZE (1..ub-name)),
		KTUniversalString   m_universalString;        //(SIZE (1..ub-name)),
		KTUTF8String        m_utf8String;             //(SIZE (1..ub-name)),
		KTBMPString         m_bmpString;              //(SIZE (1..ub-name)),
	};
#endif

///////////////////////////////////////////////////////////////////////////////
//KTTime
class KT_LIB_MODE KTTime : public KTCHOICE2
{
public:
	KTTime()
    {
		PLACE2OBJS(m_utcTime, m_generalTime);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
	}

	void setValue(int nYear, int nMonth, int nDay, int nHour, int nMinutes,
				KTBOOL bSecondsExist, int nSeconds,
				KTBOOL bOffsetExist, int nOffset);
	void getValue(int& nYear, int& nMonth, int& nDay, int& nHour, int& nMinute,
				KTBOOL& bSecondsExist, int& nSeconds,
				KTBOOL& bOffsetExist, int& nOffset)const;

	void setValue(const struct tm& tm);
	void getValue(struct tm& tm)const;

	KTUTCTime			m_utcTime;
	KTGeneralizedTime	m_generalTime;
};

SETOF(KTTimeSet, KTTime);

///////////////////////////////////////////////////////////////////////////////
// KTValidity
class KT_LIB_MODE KTValidity : public KTSEQUENCE2
{
public:
	KTValidity()
	{
		PLACE2OBJS(m_notBefore, m_notAfter)
	}

	KTTime m_notBefore;
	KTTime m_notAfter;
};

///////////////////////////////////////////////////////////////////////////////
// KTContentType
typedef KTObjectIdentifier KTContentType;

///////////////////////////////////////////////////////////////////////////////
// KTCertificateSerialNumber
typedef KTINTEGER KTCertificateSerialNumber;

///////////////////////////////////////////////////////////////////////////////
// KTExtension
class KT_LIB_MODE KTExtension : public KTSEQUENCE3
{
public:
	KTExtension() 
	{
		m_critical.setOptional();
		PLACE3OBJS(m_extnId, m_critical, m_extnValue)
	}

    KTBOOL isCritical() const
	{ 
		if(m_critical.exist())
			return m_critical.value();
		return FALSE;
	};
	
	KTObjectIdentifier	m_extnId;
	KTBOOLEAN			m_critical;
	KTOCTETSTRING		m_extnValue;
};

///////////////////////////////////////////////////////////////////////////////
// KTExtensions
SEQUENCEOF(KTExtensions0, KTExtension)
class KT_LIB_MODE KTExtensions : public KTExtensions0
{
public:
	KTBOOL isCritical(int nIndex) const
	{ 
		return indexObj(nIndex)->isCritical(); 
	}
	void getExtnId(int nIndex, KTString& strExtnId) const 
	{
		indexObj(nIndex)->m_extnId.getValue(strExtnId); 
	}
	const KTOCTETSTRING& getExtnValue(int nIndex) const 
	{
		return indexObj(nIndex)->m_extnValue; 
	}

	const KTExtension* idExtn(const char* const pc_extnId) const; 

	void attach(const char* const pc_extnId, const BYTE* pbData, UINT unLen, KTBOOL bCritical = FALSE);
	void attach(const char* const pc_extnId, const KTObject& objExtn, KTBOOL bCritical = FALSE);
};

SETOF(KTExtensionsSet, KTExtensions);

///////////////////////////////////////////////////////////////////////////////
///KTUniqueIdentifier
typedef KTBITSTRING KTUniqueIdentifier;

////define digest function format
typedef int (*DIGEST_FUNCTION)(const KTBYTE* pbBuf, KTUINT unBufLen, KTBYTE* pbHash, KTUINT* punHashLen);

///////////////////////////////////////////////////////////////////////////////
// KTTBSCertificate 
// to be signed certificate
 
class KT_LIB_MODE KTTBSCertificate : public KTSEQUENCE10
{
public:
	KTTBSCertificate()
	{
		PLACE10OBJS(m_version, m_serialNumber, m_signature,	m_issuer, m_validity, 
            m_subject, m_subjectPublicKeyInfo, m_issuerUniqueID, m_subjectUniqueID, m_extensions)
		
        m_version = 0;
		m_version.setOptional();
		m_issuerUniqueID.setOptional();
		m_subjectUniqueID.setOptional();
		m_extensions.setOptional();

        m_version.setTypeEx(0, TRUE);
		m_issuerUniqueID.setTypeEx(1, FALSE);		
		m_subjectUniqueID.setTypeEx(2, FALSE);
		m_extensions.setTypeEx(3, TRUE);
	}

public:    
    void setVersion(int nVersion);
    int getVersion() const;
	KTBOOL match(const KTRSAPrivateKey	&privateKey) const; 
    int getDigest(DIGEST_FUNCTION digestFunction, KTBYTE* pbHash, KTUINT& unHashLen) const;

	KTBOOL match(const KTSM2PrivateKey	&privateKey) const; 
	KTBOOL IsSM2Certificate()const;

public:
	KTINTEGER				    m_version;
	KTCertificateSerialNumber	m_serialNumber;
	KTAlgorithmIdentifier	    m_signature;
	KTName					    m_issuer;
	KTValidity				    m_validity;
	KTName					    m_subject;
	KTSubjectPublicKeyInfo	    m_subjectPublicKeyInfo;

	KTUniqueIdentifier			m_issuerUniqueID;				// implicit 1, OPTIONAL
	KTUniqueIdentifier		    m_subjectUniqueID;				// implicit 2, OPTIONAL
	KTExtensions				m_extensions;
};

//////////////////////////////////////////////////////////////////
// KTCertificate
class KT_LIB_MODE KTCertificate : public KTToBeSigned
{
public:
	KTCertificate()
	{
		attachToBeSignedObj(&m_tbsCertificate);
	}
	
	int	checkValidity(const KTCertificate &issuerCert)const;
	///run 6628659 times in 1 second at pentium 4 1.8GHz
	int	checkName(const KTCertificate &issuerCert)const;
	///run 0011988 times in 1 second at pentium 4 1.8GHz
    int checkSignature(const KTCertificate &issuerCert)const;
    int checkSignatureSM2(const KTCertificate &issuerCert)const;
    ///if absent KeyID, return KT_ERR_CERT_NO_KEY_ID
    int	checkKeyID(const KTCertificate &issuerCert)const;

	KTBOOL IsSM2Certificate()const;

public:
	KTTBSCertificate        m_tbsCertificate;
};

//////////////////////////////////////////////////////////////////////////////
//KTDigest
typedef KTOCTETSTRING KTDigest;

///////////////////////////////////////////////////////////////////////////////
// KTCertThumbs
class KT_LIB_MODE KTCertThumb: public KTSEQUENCE2
{
public:
	KTCertThumb()
	{
		m_digestAlgorithm.setTypeValue(ObjID_id_sha1); 
		PLACE2OBJS(m_digestAlgorithm, m_thumbprint)
	}

	KTAlgorithmIdentifier	m_digestAlgorithm;
	KTDigest			    m_thumbprint;

};


//////////////////////////////////////////////////////////////////////
//KTCRLEntry
class KT_LIB_MODE KTCRLEntry : public KTSEQUENCE3
{
public:
	KTCRLEntry()
	{	
		m_crlEntryExtensions.setOptional();
		PLACE3OBJS(m_userCertificate, m_revocationDate, m_crlEntryExtensions)
	}

	KTCertificateSerialNumber   m_userCertificate;
	KTTime		                m_revocationDate;
	KTExtensions		        m_crlEntryExtensions;			// OPTIONAL
};

///////////////////////////////////////////////////////////////////////////////
//KTCRLEntrySequence
SEQUENCEOF(KTCRLEntrySequence, KTCRLEntry)

///////////////////////////////////////////////////////////////////////////////
//KTTBSCertList
class KT_LIB_MODE KTTBSCertList : public KTSEQUENCE7
{
public:
	KTTBSCertList()
	{
		PLACE7OBJS(m_version, m_signature, m_issuer, m_thisUpdate,
			m_nextUpdate, m_revokedCertificates, m_crlExtensions)
            
        m_version.setOptional();
		m_revokedCertificates.setOptional();
		m_crlExtensions.setOptional();
		m_crlExtensions.setTypeEx(0, TRUE);
	}
    
	KTBOOL isCertIn(const KTCertificate &cert) const;

	KTINTEGER				    m_version;
	KTAlgorithmIdentifier	    m_signature;
	KTName					    m_issuer;
	KTTime				        m_thisUpdate;
	KTTime				        m_nextUpdate;
	KTCRLEntrySequence			m_revokedCertificates;		//OPTIONAL
	KTExtensions				m_crlExtensions;			//OPTIONAL
};

//////////////////////////////////////////////////////////////////
// KTCertificateList
class KT_LIB_MODE KTCertificateList : public KTToBeSigned
{
public:
	KTCertificateList()
	{
		attachToBeSignedObj(&m_tbsCertList);
	}

	KTTBSCertList           m_tbsCertList;
};

typedef KTCertificateList KTCertificateRevocationList;
typedef KTCertificateRevocationList KTCRL;

class KT_LIB_MODE KTDss_Sig_Value  : public  KTSEQUENCE2  
{
public:
    KTDss_Sig_Value()
    {
        PLACE2OBJS(m_r, m_s)
    }
    
    KTINTEGER     m_r;       
    KTINTEGER     m_s;         
};

class KT_LIB_MODE KTValidationParms : public  KTSEQUENCE2
{
public:
    KTValidationParms()
    {
        PLACE2OBJS(m_seed, m_pgenCounter)
    }
    
    KTBITSTRING    m_seed;
    KTINTEGER      m_pgenCounter;       
};

class KT_LIB_MODE KTDomainParameters : public  KTSEQUENCE5
{
public:
    KTDomainParameters()
    {
        PLACE5OBJS(m_p, m_g, m_q, m_j, m_validationParms)
        m_j.setOptional();
        m_validationParms.setOptional();
    }

    KTINTEGER          m_p;        //INTEGER, -- odd prime, p=jq +1
    KTINTEGER          m_g;        //INTEGER, -- generator, g
    KTINTEGER          m_q;        //INTEGER, -- factor of p-1
    KTINTEGER          m_j;        //INTEGER OPTIONAL, -- subgroup factor, j>= 2
    KTValidationParms  m_validationParms;   //OPTIONAL 
};

class KT_LIB_MODE KTDss_Parms  : public  KTSEQUENCE3
{
public:
    KTDss_Parms()
    {
        PLACE3OBJS(m_p, m_q, m_g)
    }

    KTINTEGER   m_p;
    KTINTEGER   m_q;
    KTINTEGER   m_g;
};

class KT_LIB_MODE KTCountryName : public  KTCHOICE2
{
public:
    KTCountryName()
    {
        PLACE2OBJS(m_x121_dcc_code, m_iso_3166_alpha2_code);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
    }

    KTNumericString     m_x121_dcc_code;            //(SIZE (ub-country-name-numeric-length)),
    KTPrintableString   m_iso_3166_alpha2_code;     //(SIZE (ub-country-name-alpha-length)) 
};

class KT_LIB_MODE KTAdministrationDomainName : public  KTCHOICE2
{
public:
    KTAdministrationDomainName()
    {
        PLACE2OBJS(m_numeric, m_printable);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
    }

    KTNumericString     m_numeric;      //(SIZE (0..ub-domain-name-length)),
    KTPrintableString   m_printable;    //(SIZE (0..ub-domain-name-length)) 
};

typedef KTNumericString KTTX121Address;
typedef KTTX121Address KTNetworkAddress;

typedef KTPrintableString KTTerminalIdentifier;

typedef KTAdministrationDomainName KTPrivateDomainName;

typedef KTPrintableString KTOrganizationName;

typedef KTNumericString KTNumericUserIdentifier;

class KT_LIB_MODE KTPersonalName : public  KTSEQUENCE4
{
public:
    KTPersonalName()
    {
        setType(UniversalSet);
        PLACE4OBJS(m_surname, m_given_name, m_initials, m_generation_qualifier)
        
        m_surname.setTypeEx(0, FALSE);
        m_given_name.setTypeEx(1, FALSE);
        m_initials.setTypeEx(2, FALSE);
        m_generation_qualifier.setTypeEx(3, FALSE);
        
        m_given_name.setOptional();
        m_initials.setOptional();
        m_generation_qualifier.setOptional();
    }

	void getValue(KTString& str) const;

    KTPrintableString   m_surname; //[0]  (SIZE (1..ub-surname-length)),
    KTPrintableString   m_given_name; //[1] PrintableString (SIZE (1..ub-given-name-length)) OPTIONAL,
    KTPrintableString   m_initials;   // [2] PrintableString (SIZE (1..ub-initials-length)) OPTIONAL,
    KTPrintableString   m_generation_qualifier; //[3] PrintableString (SIZE (1..ub-generation-qualifier-length)) OPTIONAL 
}; 

typedef KTPrintableString KTOrganizationalUnitName;
SEQUENCEOF(KTOrganizationalUnitNames, KTOrganizationalUnitName) 

class KT_LIB_MODE KTBuiltInStandardAttributes : public  KTSEQUENCE9 
{
public:
   KTBuiltInStandardAttributes()
   {
       setType(UniversalSet);
       PLACE9OBJS(m_country_name, m_administration_domain_name, m_network_address,
           m_terminal_identifier, m_private_domain_name, m_organization_name,
           m_numeric_user_identifier, m_personal_name, m_organizational_unit_names)

       m_country_name.setOptional();
       m_administration_domain_name.setOptional();
       m_network_address.setOptional();
       m_terminal_identifier.setOptional();
       m_private_domain_name.setOptional();
       m_organization_name.setOptional();
       m_numeric_user_identifier.setOptional();
       m_personal_name.setOptional();
       m_organizational_unit_names.setOptional();

       m_network_address.setTypeEx(0, FALSE);
       m_terminal_identifier.setTypeEx(1, FALSE);
       m_private_domain_name.setTypeEx(2, FALSE);
       m_organization_name.setTypeEx(3, FALSE);
       m_numeric_user_identifier.setTypeEx(4, FALSE);
       m_personal_name.setTypeEx(5, FALSE);
       m_organizational_unit_names.setTypeEx(6, FALSE);
   }
	
    void getValue(KTString& str) const;

    KTCountryName                   m_country_name;                 //OPTIONAL,
    KTAdministrationDomainName      m_administration_domain_name;  //OPTIONAL,
    KTNetworkAddress                m_network_address;      //[0]  OPTIONAL,
    KTTerminalIdentifier            m_terminal_identifier;  //[1]  OPTIONAL,
    KTPrivateDomainName             m_private_domain_name;  //[2]  OPTIONAL,
    KTOrganizationName              m_organization_name;   //[3]  OPTIONAL,
    KTNumericUserIdentifier         m_numeric_user_identifier;      //[4]  OPTIONAL,
    KTPersonalName                  m_personal_name;        //[5]  OPTIONAL,-- see also teletex-personal-name
    KTOrganizationalUnitNames       m_organizational_unit_names;    //[6]  OPTIONAL-- see also teletex-organizational-unit-names -- 
};

class KT_LIB_MODE KTBuiltInDomainDefinedAttribute : public  KTSEQUENCE2
{
public:
    KTBuiltInDomainDefinedAttribute()
    {
        PLACE2OBJS(m_type, m_value)
    }

    KTPrintableString m_type;  //(SIZE(1..ub-domain-defined-attribute-type-length)),
    KTPrintableString m_value; //(SIZE(1..ub-domain-defined-attribute-value-length))
};
SEQUENCEOF(KTBuiltInDomainDefinedAttributes, KTBuiltInDomainDefinedAttribute)

class KT_LIB_MODE KTTeletexPersonalName : public  KTSEQUENCE4
{
public:
    KTTeletexPersonalName()
    {
        setType(UniversalSet);
        PLACE4OBJS(m_surname, m_given_name, m_initials, m_generation_qualifier)
        
        m_surname.setTypeEx(0, FALSE);
        m_given_name.setTypeEx(1, FALSE);
        m_initials.setTypeEx(2, FALSE);
        m_generation_qualifier.setTypeEx(3, FALSE);
        
        m_given_name.setOptional();
        m_initials.setOptional();
        m_generation_qualifier.setOptional();
    }
	
	void getValue(KTString& str) const;

    KTTeletexString   m_surname; //[0]  TeletexString (SIZE (1..ub-surname-length)),
    KTTeletexString   m_given_name; //[1] TeletexString (SIZE (1..ub-given-name-length)) OPTIONAL,
    KTTeletexString   m_initials;   // [2] TeletexString (SIZE (1..ub-initials-length)) OPTIONAL,
    KTTeletexString   m_generation_qualifier; //[3] TeletexString (SIZE (1..ub-generation-qualifier-length)) OPTIONAL
};

typedef KTTeletexString KTTeletexOrganizationalUnitName; 
SEQUENCEOF(KTTeletexOrganizationalUnitNames, KTTeletexOrganizationalUnitName)

class KT_LIB_MODE KTTeletexDomainDefinedAttribute : public  KTSEQUENCE2
{
public:
    KTTeletexDomainDefinedAttribute()
    {
        PLACE2OBJS(m_type, m_value)
    }

    KTTeletexString m_type;  //(SIZE(1..ub-domain-defined-attribute-type-length)),
    KTTeletexString m_value; //(SIZE(1..ub-domain-defined-attribute-value-length))
};
SEQUENCEOF(KTTeletexDomainDefinedAttributes, KTTeletexDomainDefinedAttribute)

class KT_LIB_MODE KTPhysicalDeliveryCountryName : public  KTCHOICE2
{
public:
    KTPhysicalDeliveryCountryName()
    {
        PLACE2OBJS(m_x121dcc_code, m_iso_3166_alpha2_code);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
    }

    KTNumericString     m_x121dcc_code;    //(SIZE (ub-country-name-numeric-length)),
    KTPrintableString   m_iso_3166_alpha2_code; //(SIZE (ub-country-name-alpha-length)) 
};

class KT_LIB_MODE KTPostalCode : public  KTCHOICE2
{
public:
    KTPostalCode()
    {
        PLACE2OBJS(m_numeric_code, m_printable_code);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
    }

    KTNumericString     m_numeric_code;     //(SIZE (1..ub-postal-code-length)),
    KTPrintableString   m_printable_code;   //(SIZE (1..ub-postal-code-length)) 
};

SEQUENCEOF(KTPrintableStringSequence, KTPrintableString)

class KT_LIB_MODE KTUnformattedPostalAddress : public  KTSEQUENCE2
{
public:
    KTUnformattedPostalAddress()
    {
        setType(UniversalSet);
        PLACE2OBJS(m_printable_address, m_teletex_string)
        m_printable_address.setOptional();
        m_teletex_string.setOptional();
    }

	void getValue(KTString& str) const;

    KTPrintableStringSequence   m_printable_address;    //SEQUENCE SIZE (1..ub-pds-physical-address-lines) OF PrintableString (SIZE (1..ub-pds-parameter-length)) OPTIONAL,
    KTTeletexString             m_teletex_string;       //(SIZE (1..ub-unformatted-address-length)) OPTIONAL 
};

class KT_LIB_MODE KTPDSParameter : public  KTSEQUENCE2
{
public:
    KTPDSParameter()
    {
        setType(UniversalSet);
        PLACE2OBJS(m_printable_string, m_teletex_string)
        m_printable_string.setOptional();
        m_teletex_string.setOptional();
    }

    KTPrintableString   m_printable_string;     //(SIZE(1..ub-pds-parameter-length)) OPTIONAL,
    KTTeletexString     m_teletex_string;       //(SIZE(1..ub-pds-parameter-length)) OPTIONAL 
};

class KT_LIB_MODE KTe163_4_address : public  KTSEQUENCE2
{
public:
    KTe163_4_address()
    {
        PLACE2OBJS(m_number, m_sub_address)
        m_number.setTypeEx(0, FALSE);
        m_sub_address.setTypeEx(1, FALSE);
        m_sub_address.setOptional();
    }

    KTNumericString m_number;       //[0]  (SIZE (1..ub-e163-4-number-length)),
    KTNumericString m_sub_address;  //[1]  (SIZE (1..ub-e163-4-sub-address-length)) OPTIONAL 
};

SEQUENCEOF(KTe163_4_addressSequence, KTe163_4_address)

class KT_LIB_MODE KTPresentationAddress : public  KTSEQUENCE4
{
public:
    KTPresentationAddress()
    {
        PLACE4OBJS(m_pSelector, m_sSelector, m_tSelector, m_nAddresses)
        m_pSelector.setOptional();
        m_sSelector.setOptional();
        m_tSelector.setOptional();

        m_pSelector.setTypeEx(0, TRUE);
        m_sSelector.setTypeEx(1, TRUE);
        m_tSelector.setTypeEx(2, TRUE);
        m_nAddresses.setTypeEx(3, TRUE);
    }
	
	void getValue(KTString& str) const;

    KTOCTETSTRING       m_pSelector;       //[0] EXPLICIT OPTIONAL,
    KTOCTETSTRING       m_sSelector;       //[1] EXPLICIT OPTIONAL,
    KTOCTETSTRING       m_tSelector;       //[2] EXPLICIT OPTIONAL,
    KTOCTETSTRINGSet    m_nAddresses;      //[3] EXPLICIT SET SIZE (1..MAX) OF OCTET STRING 
};

class KT_LIB_MODE KTExtendedNetworkAddress : public  KTCHOICE2
{
public:
    KTExtendedNetworkAddress()
    {
        PLACE2OBJS(m_e163_4_address, m_psap_address);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_psap_address.setTypeEx(0, FALSE);
    }
	
	void getValue(KTString& str) const;

    KTe163_4_addressSequence    m_e163_4_address;   //OPTIONAL
    KTPresentationAddress       m_psap_address;     //[0] 
};

class KT_LIB_MODE KTTerminalType: public KTINTEGER
{
public:
	const KTChar* terminalType() const;
};

enum enumTerminalType
{
    TerminalTypeTelex = 3,
    TerminalTypeTeletex = 4,
    TerminalTypeG3_facsimile = 5,
    TerminalTypeG4_facsimile = 6,
    TerminalTypeIa5_terminal = 7,
    TerminalTypeVideotex = 8,
};

class KT_LIB_MODE KTExtensionAttribute : public  KTSequence 
{
public:
    enum enumExtnAttrType
    {
        common_name = 1,    //CommonName ::= PrintableString (SIZE (1..ub-common-name-length))
        teletex_common_name = 2,    //TeletexCommonName ::= TeletexString (SIZE (1..ub-common-name-length))
        teletex_organization_name = 3,  //TeletexOrganizationName ::= TeletexString (SIZE (1..ub-organization-name-length))
        teletex_personal_name = 4, // TeletexPersonalName 
        teletex_organizational_unit_names = 5,   //TeletexOrganizationalUnitNames
        teletex_domain_defined_attributes = 6,  //TeletexDomainDefinedAttributes 
        pds_name = 7,   //PDSName ::= PrintableString (SIZE (1..ub-pds-name-length))
        physical_delivery_country_name = 8, //PhysicalDeliveryCountryName
        postal_code = 9,    //PostalCode
        physical_delivery_office_name = 10, //PhysicalDeliveryOfficeName ::= PDSParameter
        physical_delivery_office_number = 11, //PhysicalDeliveryOfficeNumber ::= PDSParameter
        extension_OR_address_components = 12,   //ExtensionORAddressComponents ::= PDSParameter
        physical_delivery_personal_name = 13,   //PhysicalDeliveryPersonalName ::= PDSParameter
        physical_delivery_organization_name = 14,   //PhysicalDeliveryOrganizationName ::= PDSParameter
        extension_physical_delivery_address_components = 15,    //ExtensionPhysicalDeliveryAddressComponents ::= PDSParameter
        unformatted_postal_address = 16,    //UnformattedPostalAddress
        street_address = 17,    //StreetAddress ::= PDSParameter
        post_office_box_address = 18,   //PostOfficeBoxAddress ::= PDSParameter
        poste_restante_address = 19,   //PosteRestanteAddress ::= PDSParameter
        unique_postal_name = 20, //UniquePostalName ::= PDSParameter
        local_postal_attributes = 21, //LocalPostalAttributes ::= PDSParameter
        extended_network_address = 22,  //ExtendedNetworkAddress
        terminal_type  = 23, //TerminalType ::= INTEGER
    };

public:
    KTExtensionAttribute()
    {
        m_xtension_attribute_type.setTypeEx(0, FALSE);
		m_xtension_attribute_value=NULL;
    }
	virtual ~KTExtensionAttribute()
	{
		delete m_xtension_attribute_value;
	}
    
    KTINTEGER       m_xtension_attribute_type;
    KTObject*       m_xtension_attribute_value;
	
	const KTChar* attribute_type() const;
	void attribute_value(KTString& str) const;

	int modifyLen();
	
protected:
	int createContent();
	void outputContentIn() const;
    
	KTObject* newObj(int nType);
};

SETOF(KTExtensionAttributes, KTExtensionAttribute)
 
class KT_LIB_MODE KTORAddress : public  KTSEQUENCE3 
{
public:
    KTORAddress()
    {
        PLACE3OBJS(m_built_in_standard_attributes, m_built_in_domain_defined_attributes, m_extension_attributes)
        m_built_in_domain_defined_attributes.setOptional();
        m_extension_attributes.setOptional();
    }
    
	void getValue(KTString& str) const;

    KTBuiltInStandardAttributes         m_built_in_standard_attributes;
    KTBuiltInDomainDefinedAttributes    m_built_in_domain_defined_attributes; //OPTIONAL,                         
    KTExtensionAttributes               m_extension_attributes;  //OPTIONAL 
};


//id-ce-authorityKeyIdentifier OBJECT IDENTIFIER ::=  { id-ce 35 }

typedef KTOCTETSTRING KTKeyIdentifier;

typedef KTAttribute KTAnotherName;

class KT_LIB_MODE KTEDIPartyName : public KTSEQUENCE2
{
public:
    KTEDIPartyName()
    {
        PLACE2OBJS(m_nameAssigner, m_partyName)
        m_nameAssigner.setOptional();
        m_nameAssigner.setTypeEx(0, FALSE);
        m_partyName.setTypeEx(1, FALSE);
    }

    KTDirectoryString   m_nameAssigner;     //[0]      OPTIONAL,
    KTDirectoryString   m_partyName;        //[1]      
};

class KT_LIB_MODE KTGeneralName : public KTCHOICE9
{
public:
    KTGeneralName()
    {
        PLACE9OBJS(m_otherName, m_rfc822Name, m_dNSName,
            m_x400Address, m_directoryName, m_ediPartyName,
            m_uniformResourceIdentifier, m_iPAddress, m_registeredID);
		OptionPlacedObjs(9);
		SetTypeExPlacedObjs(9, TRUE);
		ChoiceFirstPlacedObj();
    }

	void getValue(KTString& str) const;
    
	KTAnotherName       m_otherName;
    KTIA5String         m_rfc822Name;
    KTIA5String         m_dNSName;
    KTORAddress         m_x400Address;
    KTName              m_directoryName; 
    KTEDIPartyName      m_ediPartyName;
    KTIA5String         m_uniformResourceIdentifier;
    KTOCTETSTRING       m_iPAddress;
    KTObjectIdentifier  m_registeredID;   
};

SEQUENCEOF(KTGeneralNames, KTGeneralName)

///////////////////////////////////////////////////////////////////////////////
//KTAuthorityKeyIdentifier 
class KT_LIB_MODE KTAuthorityKeyIdentifier : public KTSEQUENCE3
{
public:
	KTAuthorityKeyIdentifier()
	{
		PLACE3OBJS(m_keyIdentifier, m_authorityCertIssuer, m_authorityCertSerialNumber)

		m_keyIdentifier.setTypeEx(0, FALSE);
		m_keyIdentifier.setOptional();
		m_authorityCertIssuer.setTypeEx(1, FALSE);
		m_authorityCertIssuer.setOptional();
		m_authorityCertSerialNumber.setTypeEx(2, FALSE);
		m_authorityCertSerialNumber.setOptional();
	}

	void setKeyIdentifier(const KTByteArray& rKeyIdentifier);
	KTBOOL getKeyIdentifier(KTByteArray& rKeyIdentifier) const;

	KTKeyIdentifier				m_keyIdentifier;
    KTGeneralNames				m_authorityCertIssuer;
    KTCertificateSerialNumber	m_authorityCertSerialNumber;
};

//id-ce-subjectKeyIdentifier OBJECT IDENTIFIER ::=  { id-ce 14 }
typedef KTKeyIdentifier KTSubjectKeyIdentifier;

//id-ce-keyUsage OBJECT IDENTIFIER ::=  { id-ce 15 }
typedef KTBITSTRING KTKeyUsage; 
enum enumKeyUsage
{
    KeyUsageNone              = 0x000,
    KeyUsageDigitalSignature  = 0x001,
    KeyUsageNonRepudiation    = 0x002,
    KeyUsageKeyEncipherment   = 0x004,
    KeyUsageDataEncipherment  = 0x008,
    KeyUsageKeyAgreement      = 0x010,
    KeyUsageKeyCertSign       = 0x020,
    KeyUsageCRLSign           = 0x040,
    KeyUsageEncipherOnly      = 0x080,
    KeyUsageDecipherOnly      = 0x100,
};

//id-ce-privateKeyUsagePeriod OBJECT IDENTIFIER ::=  { id-ce 16 }
class KT_LIB_MODE KTPrivateKeyUsagePeriod: public KTSEQUENCE2
{
public:
	KTPrivateKeyUsagePeriod()
	{
		PLACE2OBJS(m_notBefore, m_notAfter)

        m_notBefore.setOptional();
        m_notBefore.setTypeEx(0, FALSE);
		m_notAfter.setOptional();
        m_notAfter.setTypeEx(1, FALSE);
	}

	KTGeneralizedTime  m_notBefore;		//implicited 0 optional
	KTGeneralizedTime  m_notAfter;		//implicited 1 optional
};

//id-ce-certificatePolicies OBJECT IDENTIFIER ::=  { id-ce 32 }
typedef KTObjectIdentifier KTCertPolicyId;  //( id-qt-cps | id-qt-unotice )

typedef KTAttribute KTPolicyQualifierInfo; 

SEQUENCEOF(KTPolicyQualifierInfoSequence, KTPolicyQualifierInfo)

class KT_LIB_MODE KTPolicyInformation : public KTSEQUENCE2
{
public:
    KTPolicyInformation()
    {
        PLACE2OBJS(m_policyIdentifier, m_policyQualifiers)
        m_policyQualifiers.setOptional();
    }

    KTCertPolicyId                      m_policyIdentifier;
    KTPolicyQualifierInfoSequence       m_policyQualifiers;
};

SEQUENCEOF(KTCertificatePolicies, KTPolicyInformation)

typedef KTIA5String KTCPSuri;

class KT_LIB_MODE KTDisplayText : public KTCHOICE3
{
public:
    KTDisplayText()
    {
        PLACE3OBJS(m_visibleString, m_bmpString, m_utf8String);
		OptionPlacedObjs(3);
		ChoiceFirstPlacedObj();
    }
    
    KTVisibleString     m_visibleString;        //(SIZE (1..200)),
    KTBMPString         m_bmpString;            //(SIZE (1..200)),
    KTUTF8String        m_utf8String;           //(SIZE (1..200))
};

SEQUENCEOF(KTINTEGERSequence, KTINTEGER)

class KT_LIB_MODE KTNoticeReference : public KTSEQUENCE2
{
public:
    KTNoticeReference()
    {
        PLACE2OBJS(m_organization, m_noticeNumbers);
    }

    KTDisplayText      m_organization;
    KTINTEGERSequence  m_noticeNumbers;    
};

class KT_LIB_MODE KTUserNotice : public KTSEQUENCE2
{
public:
    KTUserNotice()
    {
        PLACE2OBJS(m_noticeRef, m_explicitText);
        m_noticeRef.setOptional();
        m_explicitText.setOptional();
    }

    KTNoticeReference   m_noticeRef;         //OPTIONAL
    KTDisplayText       m_explicitText;      //OPTIONAL
};

//id-ce-policyMappings OBJECT IDENTIFIER ::=  { id-ce 33 }
class KT_LIB_MODE KTPolicyMappings : public KTSEQUENCE2
{
public:
    KTPolicyMappings()
    {
        PLACE2OBJS(m_issuerDomainPolicy, m_subjectDomainPolicy);
    }

    KTCertPolicyId    m_issuerDomainPolicy;
    KTCertPolicyId    m_subjectDomainPolicy;      
};

//id-ce-subjectAltName OBJECT IDENTIFIER ::=  { id-ce 17 }
typedef KTGeneralNames  KTSubjectAltName; 

//id-ce-issuerAltName OBJECT IDENTIFIER ::=  { id-ce 18 }
typedef KTGeneralNames  KTIssuerAltName;

//id-ce-subjectDirectoryAttributes OBJECT IDENTIFIER ::=  { id-ce 9 }
SEQUENCEOF(KTSubjectDirectoryAttributes, KTAttribute) 

//id-ce-basicConstraints OBJECT IDENTIFIER ::=  { id-ce 19 }
class KT_LIB_MODE KTBasicConstraintsSyntax : public KTSEQUENCE2
{
public:
	KTBasicConstraintsSyntax()
	{
		PLACE2OBJS(m_cA, m_pathLenConstraint)
        m_cA = FALSE;
        m_pathLenConstraint.setOptional();
	}

	KTBOOLEAN   m_cA;						//default FALSE
	KTINTEGER   m_pathLenConstraint;	    //optional (0 ... MAX)
};


//-- name constraints extension OID and syntax
//id-ce-nameConstraints OBJECT IDENTIFIER ::=  { id-ce 30 }
typedef KTINTEGER KTBaseDistance;

class KT_LIB_MODE KTGeneralSubtree : public KTSEQUENCE3
{
public:
    KTGeneralSubtree()
    {
        PLACE3OBJS(m_base, m_minimum, m_maximum);
        m_minimum = 0;
        m_minimum.setTypeEx(0, FALSE);
        m_maximum.setOptional();
        m_maximum.setTypeEx(1, FALSE);
    }

    KTGeneralName       m_base;
    KTBaseDistance      m_minimum;         //[0]      DEFAULT 0,
    KTBaseDistance      m_maximum;         //[1]      OPTIONAL 
};

SEQUENCEOF(KTGeneralSubtrees, KTGeneralSubtree)

class KT_LIB_MODE KTNameConstraints : public KTSEQUENCE2
{
public:
    KTNameConstraints()
    {
        PLACE2OBJS(m_permittedSubtrees, m_excludedSubtrees);

        m_permittedSubtrees.setOptional();
        m_permittedSubtrees.setTypeEx(0, FALSE);
        m_excludedSubtrees.setOptional();
        m_excludedSubtrees.setTypeEx(1, FALSE);
    }

     KTGeneralSubtrees  m_permittedSubtrees;       //[0]      OPTIONAL,
     KTGeneralSubtrees  m_excludedSubtrees;        //[1]      OPTIONAL 
};

//-- policy constraints extension OID and syntax
//id-ce-policyConstraints OBJECT IDENTIFIER ::=  { id-ce 36 }

typedef KTINTEGER   KTSkipCerts;

class KT_LIB_MODE KTPolicyConstraints : public KTSEQUENCE2
{
public:
    KTPolicyConstraints()
    {
        PLACE2OBJS(m_requireExplicitPolicy, m_inhibitPolicyMapping);
        m_requireExplicitPolicy.setOptional();
        m_requireExplicitPolicy.setTypeEx(1, FALSE);
        m_inhibitPolicyMapping.setOptional();
        m_inhibitPolicyMapping.setTypeEx(1, FALSE);
    }

     KTSkipCerts    m_requireExplicitPolicy;           //[0]  OPTIONAL,
     KTSkipCerts    m_inhibitPolicyMapping;            //[1]  OPTIONAL
};

//-- CRL distribution points extension OID and syntax
//id-ce-cRLDistributionPoints     OBJECT IDENTIFIER  ::=  {id-ce 31}

enum enumReasonFlags
{
     ReasonFlagsUnused                  =0x01,
     ReasonFlagsKeyCompromise           =0x02,
     ReasonFlagsCACompromise            =0x04,
     ReasonFlagsAffiliationChanged      =0x08,
     ReasonFlagsSuperseded              =0x10,
     ReasonFlagsCessationOfOperation    =0x20,
     ReasonFlagsCertificateHold         =0x40, 
};

class KT_LIB_MODE KTReasonFlags: public KTBITSTRING
{
public:
	void reasonFlags(KTString& str) const;
	static const KTChar* reasonFlags(enumReasonFlags enumRF);
	static const KTChar* reasonFlags(int nIndex);
};

class KT_LIB_MODE KTDistributionPointName : public KTCHOICE2
{
public:
    KTDistributionPointName()
    {
        PLACE2OBJS(m_fullName, m_nameRelativeToCRLIssuer);
		OptionPlacedObjs(2);
		ChoiceFirstPlacedObj();
        m_fullName.setTypeEx(0, FALSE);
        m_nameRelativeToCRLIssuer.setTypeEx(1, FALSE);
    }

	void getValue(KTString& str) const;

     KTGeneralNames                 m_fullName;                //[0]     ,
     KTRelativeDistinguishedName    m_nameRelativeToCRLIssuer; //[1]     
};

class KT_LIB_MODE KTDistributionPoint : public KTSEQUENCE3
{
public:
    KTDistributionPoint()
    {
        PLACE3OBJS(m_distributionPoint, m_reasons, m_cRLIssuer);
        m_distributionPoint.setOptional();
        m_reasons.setOptional();
        m_cRLIssuer.setOptional();
        m_distributionPoint.setTypeEx(0, FALSE);
        m_reasons.setTypeEx(1, FALSE);
        m_cRLIssuer.setTypeEx(2, FALSE);
    }
		
	 void getValue(KTString& str) const;

     KTDistributionPointName    m_distributionPoint;       //[0]      OPTIONAL,
     KTReasonFlags              m_reasons;                 //[1]      OPTIONAL,
     KTGeneralNames             m_cRLIssuer;               //[2]      OPTIONAL
};

SEQUENCEOF(KTCRLDistPointsSyntax, KTDistributionPoint)


//-- extended key usage extension OID and syntax
//id-ce-extKeyUsage OBJECT IDENTIFIER ::= {id-ce 37}
typedef KTObjectIdentifier  KTKeyPurposeId;

SEQUENCEOF(KTExtKeyUsageSyntax, KTKeyPurposeId)


//-- authority info access
//id-pe-authorityInfoAccess OBJECT IDENTIFIER ::= { id-pe 1 }
class KT_LIB_MODE KTAccessDescription : public KTSEQUENCE2
{
public:
    KTAccessDescription()
    {
        PLACE2OBJS(m_accessMethod, m_accessLocation);
    }

    KTObjectIdentifier  m_accessMethod;
    KTGeneralName       m_accessLocation;        
};

SEQUENCEOF(KTAuthorityInfoAccessSyntax, KTAccessDescription)

//-- CRL number extension OID and syntax
//id-ce-cRLNumber OBJECT IDENTIFIER ::= { id-ce 20 }
typedef KTINTEGER   KTCRLNumber;  //(0..MAX)

//-- issuing distribution point extension OID and syntax
//id-ce-issuingDistributionPoint OBJECT IDENTIFIER ::= { id-ce 28 }

class KT_LIB_MODE KTIssuingDistributionPoint : public KTSEQUENCE5
{
public:
    KTIssuingDistributionPoint()
    {
        PLACE5OBJS(m_distributionPoint, m_onlyContainsUserCerts, m_onlyContainsCACerts,
            m_onlySomeReasons, m_indirectCRL);
        
        m_distributionPoint.setTypeEx(0, FALSE);
        m_onlyContainsUserCerts.setTypeEx(1, FALSE);
        m_onlyContainsCACerts.setTypeEx(2, FALSE);
        m_onlySomeReasons.setTypeEx(3, FALSE);
        m_indirectCRL.setTypeEx(4, FALSE);

        m_distributionPoint.setOptional();
        m_onlySomeReasons.setOptional();
    }

     KTDistributionPointName    m_distributionPoint;       //[0]  OPTIONAL,
     KTBOOLEAN                  m_onlyContainsUserCerts;   //[1]  DEFAULT FALSE,
     KTBOOLEAN                  m_onlyContainsCACerts;     //[2]  DEFAULT FALSE,
     KTReasonFlags              m_onlySomeReasons;         //[3]  OPTIONAL,
     KTBOOLEAN                  m_indirectCRL;             //[4]  DEFAULT FALSE 
};

//id-ce-deltaCRLIndicator OBJECT IDENTIFIER ::= { id-ce 27 }
//-- deltaCRLIndicator ::= BaseCRLNumber
typedef KTCRLNumber KTBaseCRLNumber; 

//-- CRL reasons extension OID and syntax
//id-ce-cRLReasons OBJECT IDENTIFIER ::= { id-ce 21 }
enum eumCRLReason
{
	CRLReasonUnspecified = 0,
	CRLReasonKeyCompromise,
	CRLReasonCACompromise,
	CRLReasonAffiliationChanged,
	CRLReasonSuperseded,
	CRLReasonCessationOfOperation,
	CRLReasonCertificateHold,
	CRLReasonRemoveFromCRL = 8,
};
typedef KTEnumerated KTCRLReason;

//-- certificate issuer CRL entry extension OID and syntax
//id-ce-certificateIssuer OBJECT IDENTIFIER ::= { id-ce 29 }
typedef KTGeneralNames  KTCertificateIssuer; 

//-- hold instruction extension OID and syntax
//id-ce-holdInstructionCode OBJECT IDENTIFIER ::= { id-ce 23 }
typedef KTObjectIdentifier  KTHoldInstructionCode;

//-- invalidity date CRL entry extension OID and syntax
//id-ce-invalidityDate OBJECT IDENTIFIER ::= { id-ce 24 }
typedef KTGeneralizedTime KTInvalidityDate;

typedef KTIA5String  KTPHGString;  

SEQUENCEOF(KTExtendedKeyUsage, KTObjectIdentifier)

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/11 02:25   Hongwei Yu   created 
///   1.0.0.1  2003/06/19 15:11   Hongwei Yu   change KTPkcs9email to support JIT err pkcs9email  
///   1.0.0.2  2003/06/30 22:40   Hongwei Yu   choiced first obj in all choice##n
///   1.0.0.3  2004/01/02 16:03   Hongwei Yu   first choice IA5String in JIT err pkcs9email

#endif //_X509CERT_H_
