#if !defined _KT_CRL_PROP_H_
#define _KT_CRL_PROP_H_

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "./KTCertProp.h"

/**
  *@Name: KTCRLProp.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/03/25 17:22
  *@Description: all crl property name and value class define
  *@Notes: KLCRLProp and KLCertProp have common ground, need a base class later 
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KT_LIB_MODE KTCRLProp  
{
public:
	KTCRLProp();
	virtual ~KTCRLProp();
	
	static void getProp(const KTCRL* pCRL,
        KTStringArray& strArrBasicName, KTStringArray& strArrBasicValue,
        KTStringArray& strArrRevCertSN, KTStringArray& strArrRevDate,
        KTStringArray& strArrRevExtensions, 
        KTStringArray& strArrExName, KTStringArray& strArrExValue, KTBoolArray& bArrCritical);

	static void getBasicProp(const KTCRL* pCRL,
        KTStringArray& strArrBasicName, KTStringArray& strArrBasicValue);

	static void getEntryListProp(const KTCRL* pCRL,
        KTStringArray& strArrRevCertSN, KTStringArray& strArrRevDate,
        KTStringArray& strArrRevExtensions);	

    static void getExtensionProp(const KTCRL* pCRL,
        KTStringArray& strArrExName, KTStringArray& strArrExValue, KTBoolArray& bArrCritical);

	static const KTCHAR* getVersionPropName();
    static const KTCHAR* getVersionPropValue(const KTCRL* pCRL);

	static const KTCHAR* getSignaturePropName();
    static const KTCHAR* getSignaturePropValue(const KTCRL* pCRL);

    static const KTCHAR* getThisUpdatePropName();
    static const KTCHAR* getThisUpdatePropValue(const KTCRL* pCRL);
    static const KTCHAR* getNextUpdatePropName();
    static const KTCHAR* getNextUpdatePropValue(const KTCRL* pCRL);

    static void getThisUpdateTm(const KTCRL* pCRL, struct tm& thisTm);
    static void getNextUpdateTm(const KTCRL* pCRL, struct tm& nextTm);

    static void getIssuerProp(const KTCRL* pCRL,
                    KTStringArray& strArrPropName, KTStringArray& strArrPropValue);
  
    static const KTCHAR* getIssuerItemValue(const KTCRL* pCRL, const KTCHAR* strOID);
    static const KTCHAR* getExtensionItemValue(const KTCRL* pCRL, const KTCHAR* strOID,
        KTString& strPropName, bool& bCritical);

};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/03/25 17:26  Hongwei Yu   created
///   1.0.0.1  2003/03/26 21:51  Hongwei Yu   refactor  

#endif // _KT_CRL_PROP_H_
