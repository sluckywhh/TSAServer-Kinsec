#if !defined(_KT_CERT_PROP_H_)
#define _KT_CERT_PROP_H_

/**
  *@Name: KTCertProp.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/03/25 17:08
  *@Description: all cert property name and value class define
  *@Notes: KLCRLProp and KLCertProp have common ground, need a base class later 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./pkcs7.h"

class KT_LIB_MODE KTCertProp  
{
    friend class KTCRLProp;

public:
	KTCertProp();
	virtual ~KTCertProp();
  
  enum enumCertType
  {
    CertTypeUnknown,
    CertTypeRootCA,
    CertTypeCA,
    CertTypeNotCA,
  };

  enum enumCertValid
  {
    CertValidUnknown,
    CertValidBefore,
    CertValidOK,
    CertValidAfter,
  };
	
public:
  static void gmtTmToLocalTm(struct tm& time);  //not above 2038Y in linux
  static void localTmToGmtTm(struct tm& time);  //not above 2038Y in linux
  static void tmToString(const struct tm& time, KTString& strTm, bool bUTF8 = false);
  
  //< 0 time1 less than time2 
  //==0 time1 identical to time2 
  //> 0 time1 greater than time2 
  static int timecmp(const struct tm& time1, const struct tm& time2);

#ifdef KT_WIN32
  static void structTmToSysTm(const struct tm& structTm, SYSTEMTIME& sysTime);
  static void sysTmToStructTm(const SYSTEMTIME& sysTime, struct tm& structTm);
  static void gmtTmToLocalTm(SYSTEMTIME & time); 
  static void localTmToGmtTm(SYSTEMTIME & time);
#endif

  ////Translate binary to ASCII, add one space before each four bytes
  ////pc_ASCII al least unLen*5/2 +1
  static int binaryToASCII(const BYTE * pbData, UINT unLen, KTCHAR pc_ASCII[]);

private:
  static inline void KTTimeToTm(const KTTime& ktTime, struct tm& time)
  {
	ktTime.getValue(time);
  };
  static void gmtKTTimeToLocalTmStr(const KTTime& ktTime, KTString& strLocalTm, bool bUTF8 = false);  
  static const KTCHAR* binaryMemObjToASCII(const KTMemObject& memObj);
  
  static const KTCHAR* signArithmetricTransFromObjID(const KTObjectIdentifier& objID);
  static const KTCHAR* getAttributeOID(const KTAttributeTypeAndValue* pAttr);
  static const KTCHAR* getAttributeValue(const KTAttributeTypeAndValue* pAttr, KTUINT32* pBMPValueLen=NULL);
 
public:  
  static const KTCHAR* nameTransFromSubjectOID(const KTCHAR* strOID);
  static const KTCHAR* nameTransFromIssuerOID(const KTCHAR* strOID);

  static void getProp(const KTCertificate* pCert,
    KTStringArray& strArrBasicName, KTStringArray& strArrBasicValue,
    enumCertValid& certValid,
    KTStringArray& strArrHashName, KTStringArray& strArrHashValue, 
    KTStringArray& strArrExName, KTStringArray& strArrExValue, KTBoolArray& bArrCritical);

  static void getBasicProp(const KTCertificate* pCert,
    KTStringArray& strArrBasicName, KTStringArray& strArrBasicValue,
    enumCertValid& certValid);
	
  static void getHashProp(const KTCertificate* pCert,
    KTStringArray& strArrHashName, KTStringArray& strArrHashValue);
  static void getHashProp(UINT unDerCertDataLen, BYTE* pbDerCertData,
    KTStringArray& strArrHashName, KTStringArray& strArrHashValue);

	static void getExtensionProp(const KTCertificate* pCert,
    KTStringArray& strArrExName, KTStringArray& strArrExValue, KTBoolArray& bArrCritical);
	
public:
  static const KTCHAR* getVersionPropName();
  static const KTCHAR* getVersionPropValue(const KTCertificate* pCert);

  static const KTCHAR* getSerialNumberPropName();
  static const KTCHAR* getSerialNumberPropValue(const KTCertificate*  pCert);

  static const KTCHAR* getSignaturePropName();
  static const KTCHAR* getSignaturePropValue(const KTCertificate* pCert);

  static const KTCHAR* getValidityNotBeforePropName();
  static const KTCHAR* getValidityNotBeforePropValue(const KTCertificate * pCert);
  static const KTCHAR* getValidityNotAfterPropName();
  static const KTCHAR* getValidityNotAfterPropValue(const KTCertificate * pCert);
  
  static void getNotBeforeTm(const KTCertificate * pCert, struct tm& tmNotBefor);
  static void getNotAfterTm(const KTCertificate * pCert, struct tm& tmNotAfter);
  
  static KTBOOL  isFather(const KTCertificate * pCert, const KTCertificate * pFatherCert);
  static UINT  getKeyUsage(const KTCertificate * pCert);
  static enumCertType  getCertType(const KTCertificate * pCert);
  static enumCertValid getValidity(const KTCertificate * pCert);
  
  static const KTCHAR* getPublicKeyPropName();
  static const KTCHAR* getPublicKeyPropValue(const KTCertificate* pCert);

  static const KTCHAR* getIssuerUniqueIDPropName();
  ////option, if not exist return NULL
  static const KTCHAR* getIssuerUniqueIDPropValue(const KTCertificate* pCert);

  static const KTCHAR* getSubjectUniqueIDPropName();
  ////option, if not exist return NULL
  static const KTCHAR* getSubjectUniqueIDPropValue(const KTCertificate* pCert);

  static void getSubjectProp(const KTCertificate* pCert,
                    KTStringArray& strArrPropName, KTStringArray& strArrPropValue);
  static void getIssuerProp(const KTCertificate* pCert,
                    KTStringArray& strArrPropName, KTStringArray& strArrPropValue);
  
  static const KTCHAR* getSubjectItemValue(const KTCertificate* pCert, const KTCHAR* strOID, KTUINT32* pBMPValueLen=NULL);
  static const KTCHAR* getIssuerItemValue(const KTCertificate* pCert, const KTCHAR* strOID, KTUINT32* pBMPValueLen=NULL);
  static const KTCHAR* getExtensionItemValue(const KTCertificate* pCert, const KTCHAR* strOID,
      KTString& strPropName, bool& bCritical);

private:
  static const KTCHAR* parseKeyUsage (const KTMemObject& memObj, UINT* punKeyUsage);
  static const KTCHAR* parseSubjectKeyIdentifier(const KTMemObject& memObj);
  static const KTCHAR* parseBasicConstraints (const KTMemObject& memObj,
      enumCertType* pCertType, int* pnCASubTreeDeep);
  static const KTCHAR* parseCRLNumber(const KTMemObject& memObj);
  static const KTCHAR* parseAuthorityKeyIdentifier(const KTMemObject& memObj);
  static const KTCHAR* parseCRLDistributionPoints(const KTMemObject& memObj);
  static const KTCHAR* parseExtKeyUsage(const KTMemObject& memObj);
  static const KTCHAR* parseAuthorityInfoAccess(const KTMemObject& memObj);
  static const KTCHAR* parseNetscapeCertType(const KTMemObject& memObj);
  static const KTCHAR* parseCertificatePolicies(const KTMemObject& memObj);

  static const KTCHAR* getExtensionSubProp(const KTExtension * ppExten, 
      KTString& strPropName, bool& bCritical);
  static const KTCHAR* getExtensionsProp(const KTExtensions& extens);

private:
  static KTString m_str;
  static KTCharArray m_caTmp;

	// 2017-11
	// 使用静态成员变量时，多线程调用出错
	// 以下接口不再使用静态成员变量
private:
  static int binaryMemObjToASCII2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  
  static int signArithmetricTransFromObjID2(const KTObjectIdentifier& objID, KTString& strOut, bool bUTF8 = false);
  static int getAttributeOID2(const KTAttributeTypeAndValue* pAttr, KTString& strOut, bool bUTF8 = false);
  static int getAttributeValue2(const KTAttributeTypeAndValue* pAttr, KTString& strOut, KTUINT32* pBMPValueLen=NULL, bool bUTF8 = false);
 
public:  
  static int nameTransFromSubjectOID2(const KTCHAR* strOID, KTString& strOut, bool bUTF8 = false);
  static int nameTransFromIssuerOID2(const KTCHAR* strOID, KTString& strOut, bool bUTF8 = false);

  static void getProp2(const KTCertificate* pCert,
    KTStringArray& strArrBasicName, KTStringArray& strArrBasicValue,
    enumCertValid& certValid,
    KTStringArray& strArrHashName, KTStringArray& strArrHashValue, 
    KTStringArray& strArrExName, KTStringArray& strArrExValue, KTBoolArray& bArrCritical, bool bUTF8 = false);

  static void getBasicProp2(const KTCertificate* pCert,
    KTStringArray& strArrBasicName, KTStringArray& strArrBasicValue,
    enumCertValid& certValid, bool bUTF8 = false);
	
  static void getHashProp2(const KTCertificate* pCert,
    KTStringArray& strArrHashName, KTStringArray& strArrHashValue, bool bUTF8 = false);
  static void getHashProp2(UINT unDerCertDataLen, BYTE* pbDerCertData,
    KTStringArray& strArrHashName, KTStringArray& strArrHashValue, bool bUTF8 = false);

	static void getExtensionProp2(const KTCertificate* pCert,
    KTStringArray& strArrExName, KTStringArray& strArrExValue, KTBoolArray& bArrCritical, bool bUTF8 = false);
	
public:
  static int getVersionPropName2(KTString& strOut, bool bUTF8 = false);
  static int getVersionPropValue2(const KTCertificate* pCert, KTString& strOut, bool bUTF8 = false);

  static int getSerialNumberPropName2(KTString& strOut, bool bUTF8 = false);
  static int getSerialNumberPropValue2(const KTCertificate *pCert, KTString& strOut, bool bUTF8 = false);

  static int getSignaturePropName2(KTString& strOut, bool bUTF8 = false);
  static int getSignaturePropValue2(const KTCertificate* pCert, KTString& strOut, bool bUTF8 = false);

  static int getValidityNotBeforePropName2(KTString& strOut, bool bUTF8 = false);
  static int getValidityNotBeforePropValue2(const KTCertificate * pCert, KTString& strOut, bool bUTF8 = false);
  static int getValidityNotAfterPropName2(KTString& strOut, bool bUTF8 = false);
  static int getValidityNotAfterPropValue2(const KTCertificate * pCert, KTString& strOut, bool bUTF8 = false);
 
  static int getPublicKeyPropName2(KTString& strOut, bool bUTF8 = false);
  static int getPublicKeyPropValue2(const KTCertificate* pCert, KTString& strOut, bool bUTF8 = false);

  static int getIssuerUniqueIDPropName2(KTString& strOut, bool bUTF8 = false);
  ////option, if not exist return NULL
  static int getIssuerUniqueIDPropValue2(const KTCertificate* pCer, KTString& strOut, bool bUTF8 = false);

  static int getSubjectUniqueIDPropName2(KTString& strOut, bool bUTF8 = false);
  ////option, if not exist return NULL
  static int getSubjectUniqueIDPropValue2(const KTCertificate* pCert, KTString& strOut, bool bUTF8 = false);

  static void getSubjectProp2(const KTCertificate* pCert,
                    KTStringArray& strArrPropName, KTStringArray& strArrPropValue, bool bUTF8 = false);
  static void getIssuerProp2(const KTCertificate* pCert,
                    KTStringArray& strArrPropName, KTStringArray& strArrPropValue, bool bUTF8 = false);
  
  static int getSubjectItemValue2(const KTCertificate* pCert, const KTCHAR* strOID, KTString& strOut, KTUINT32* pBMPValueLen=NULL, bool bUTF8 = false);
  static int getIssuerItemValue2(const KTCertificate* pCert, const KTCHAR* strOID, KTString& strOut, KTUINT32* pBMPValueLen=NULL, bool bUTF8 = false);
  static int getExtensionItemValue2(const KTCertificate* pCert, const KTCHAR* strOID,
      KTString& strPropName, bool& bCritical, KTString& strOut, bool bUTF8 = false);

private:
  static int parseKeyUsage2(const KTMemObject& memObj, UINT* punKeyUsage, KTString& strOut, bool bUTF8 = false);
  static int parseSubjectKeyIdentifier2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  static int parseBasicConstraints2(const KTMemObject& memObj,
      enumCertType* pCertType, int* pnCASubTreeDeep, KTString& strOut, bool bUTF8 = false);
  static int parseCRLNumber2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  static int parseAuthorityKeyIdentifier2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  static int parseCRLDistributionPoints2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  static int parseExtKeyUsage2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  static int parseAuthorityInfoAccess2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  static int parseNetscapeCertType2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);
  static int parseCertificatePolicies2(const KTMemObject& memObj, KTString& strOut, bool bUTF8 = false);

  static int getExtensionSubProp2(const KTExtension * ppExten, 
      KTString& strPropName, bool& bCritical, KTString& strOut, bool bUTF8 = false);
  static int getExtensionsProp2(const KTExtensions& extens, KTString& strOut, bool bUTF8 = false);
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/03/25 17:09  Hongwei Yu   created 
///   1.0.0.1  2003/03/26 21:51  Hongwei Yu   refactor 
///   1.0.0.2  2003/03/29 21:24  Hongwei Yu   added keyusage and cert type enum  
///   1.0.0.3  2003/05/13 12:25  Hongwei Yu   inline mode KTTimeToTm 
///   1.0.0.4  2003/05/13 15:36  Hongwei Yu   added timecmp and system time functions
///   1.0.0.5  2004/01/09 14:01  Hongwei Yu   added parameter pBMPValeuLen  

#endif
