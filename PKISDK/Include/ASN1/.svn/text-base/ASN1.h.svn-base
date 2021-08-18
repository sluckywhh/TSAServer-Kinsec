#ifndef __ASN1_H__
#define __ASN1_H__

/**
  * @Name: ASN1.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/07 18:52
  * @Description: all asn1 define class 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include <time.h>

//#pragma warning(disable:4251)
class KT_LIB_MODE CBigInteger;

enum TagClass {    
            UniversalTagClass =0x01,
            ApplicationTagClass = 0x10,
            ContextSpecificTagClass = 0x20,
            PrivateTagClass = 0x40,
};
    
enum UniversalTypes {
            InvalidUniversalTag,
            UniversalBoolean,
            UniversalInteger,
            UniversalBitString,
            UniversalOctetString,
            UniversalNull,
            UniversalObjectId,
            UniversalObjectDescriptor,
            UniversalExternalType,
            UniversalReal,
            UniversalEnumeration,
            UniversalEmbeddedPDV,
            UniversalUTF8String,
            UniversalSequence = 16,
            UniversalSet,
            UniversalNumericString,
            UniversalPrintableString,
            UniversalTeletexString,
            UniversalVideotexString,
            UniversalIA5String,
            UniversalUTCTime,
            UniversalGeneralizedTime,
            UniversalGraphicString,
            UniversalVisibleString,
            UniversalGeneralString,
            UniversalUniversalString,
            UniversalBMPString = 30
    };

#define CHECK_LENGTH_COMPLETE  \
		{\
			if(0 != m_nContentToReadCount)\
				return KT_ERR_ASN_LEN_CONFLICT;\
			return KT_OK;\
		}

#define ASSERT_LENGTH_COMPLETE {assert(m_head.m_nLength == m_nOutputedCount);}

class KT_LIB_MODE KTHead
{
public:
	int		m_nType;
	int		m_nLength;
	KTBYTE	m_nTag;
	int		getHeadLength() const;
};
int ASN_GetHeadInfo(const BYTE *pbSrc, int *pnType, int *pnHeadLen, int *pnDataLen);

///////////////////////////////////////////////////////////////////////////////
// KTObject define root class KT_LIB_MODE of SET class KT_LIB_MODE library in ASN.1 and DER encoding

class KT_LIB_MODE KTObject		
{
public:
	KTObject(); 
	virtual ~KTObject(){};								//must be virtual
	int		cloneContent(const KTObject& src);			//clone content only, not include head
	
	int		create(const BYTE* pbFrom, int nMaxBytes);	//include head to be created
	void	output(BYTE* pbTo) const;
	void	outputContent(BYTE* pbTo) const;				

	int		tagClass() const;
	int		type() const;
	int		extenType() const;
		
	int		contentCount() const;
	int		totalCount() const;

	void	setComposite(KTBOOL bComposite);
	void	setType(int nType);
	void	setTypeEx(int nType, KTBOOL bIsExplicit);	//from normal to implicit or explicit
	
	//get property
	KTBOOL	optional() const;
	KTBOOL	exist() const;
	KTBOOL	choice() const;
	KTBOOL	explict() const;
	KTBOOL	implict() const;

	//set property
	void	setOptional();
	void	setExist(KTBOOL bExist = TRUE);
	void	setChoice(KTBOOL bChoice = TRUE);
	void	setExplicit(KTBOOL bExplicit = TRUE);
   
private:
    KTObject(KTObject& obj);					// forbid use this function
	KTObject& operator = (KTObject& obj);		// forbid use this function

public:
	virtual int	modifyLen();

protected:
	virtual int createContent() = 0;
	virtual void outputContentIn() const= 0;

	void	setTypeEx(int nType);				//from implicit or explicit to normal

	int		peekLen( ) const;
	void	setContentCount(int nContentOctetCount);

	int		memberCreate(KTObject* pObjToCreated);
	void	memberOutput(const KTObject* pObjToOutput) const;	

	void	setSrc(const BYTE* pbSrc);
	void	setDst(BYTE* pbDst) const;
	
	int		createHeader(int nMaxOctets, KTBOOL bIsExplicit = FALSE);
	void	outputHeader(const KTBOOL bIsExplicit=FALSE) const;

	KTHead	m_head;
	KTHead	m_extenHead;
	
	int	m_nContentToReadCount;
	mutable int	m_nOutputedCount;
	
    const BYTE*	m_pbSrc;
	mutable BYTE*	m_pbDst;

private:
	KTBYTE	m_nStatus;
};

///////////////////////////////////////////////////////////////////////////////////
// KTChoice : Choice, choices >= 1

class KT_LIB_MODE KTChoice : public KTObject
{
public:
	KTChoice();
	
	KTObject* choiceObj(KTObject* pObj);	// return Previous attached object pointer
	const KTObject* choicedObj() const;
	
	int modifyLen();
protected:
	void outputContentIn() const;

private:
	KTObject* m_pObj;
};

///////////////////////////////////////////////////////////////////////////////////
// KTSequence

class KT_LIB_MODE KTSequence : public KTObject		
{
public:
	KTSequence()
	{
		setType(UniversalSequence); 
		setComposite(TRUE); 
	}
};

class KT_LIB_MODE KTSet : public KTObject			
{
public:
	KTSet() 
	{
		setType(UniversalSet);
		setComposite(TRUE);
	}
};

///////////////////////////////////////////////////////////////////////////////////
//	KTObjectList

struct KTNode
{
	KTNode()
	{
		m_pObj = NULL;
		m_pNodeNext = NULL;
	}

	KTObject* m_pObj;
	KTNode* m_pNodeNext;
};

//////////////////////////////////////////////////////////////////////////////////
// KTSequenceOf

class KT_LIB_MODE KTSequenceOf : public KTObject
{
public:
	KTSequenceOf();
	~KTSequenceOf();
	
	void clear();
	int objCount() const;
	const KTObject* indexObj(int nIndex) const;

	const KTObject* attach(const KTObject* pObjToAttach, int nIndex = -1);
	void detach(int nIndex);
	void detach(const KTObject *pObj);
	
	virtual KTObject* newObj() const= 0;
	KTObject* increaseNode(int nIndex = -1);
	
	int modifyLen();
protected:
	int createContent();
	void outputContentIn() const;

private:
	int     m_nCount;
	KTNode  m_nodeHead;
};

///////////////////////////////////////////////////////////////////////////////////
// KTSet

class KT_LIB_MODE KTSetOf : public KTSequenceOf			
{
public:
	KTSetOf() 
	{
		setType(UniversalSet);
		setComposite(TRUE);
	}
};

///////////////////////////////////////////////////////////////////////////////////
// KTMemObject : Ojbect need memory alloc
#define MEM_OBJ_INIT_BUF_SIZE 64
class KT_LIB_MODE KTMemObject:public KTObject		// Abstact class KT_LIB_MODE
{
public:
	KTMemObject();
	~KTMemObject();
	const BYTE* contentData() const { return m_pbContent; }

	KTBOOL operator == (const KTMemObject& other) const;
	KTBOOL operator != (const KTMemObject& other) const { return !(*this == other); }

protected:
	int createContent();
	void outputContentIn() const;

	BYTE* m_pbContent;

	virtual int checkContent() const= 0;
	void safeAlloc(int nSize);

private:
	void safeDelete();
	BYTE  m_baBuf[MEM_OBJ_INIT_BUF_SIZE];
};

///////////////////////////////////////////////////////////////////////////////////
// KTBOOLEAN

class KT_LIB_MODE KTBOOLEAN:public KTObject
{
public:
	KTBOOLEAN();
    
    void setValue(KTBOOL bValue);
    void getValue(KTBOOL& bValue) const;

    void operator = (KTBOOL bValue);
    KTBOOL value() const;

protected:
	int createContent();
	void outputContentIn() const;

private:
	BYTE m_byContent;
};

///////////////////////////////////////////////////////////////////////////////////
// KTNULL

class KT_LIB_MODE KTNULL: public KTObject
{
public:
	KTNULL() { setType(UniversalNull);}

protected:
	int createContent() 
	{ 
		if(0 != m_head.m_nLength)
			return KT_ERR_ASN_LEN_CONFLICT;
		return KT_OK;
	}
	
	void outputContentIn() const { assert(0 == m_head.m_nLength);}
};

///////////////////////////////////////////////////////////////////////////////
// KTINTEGER
class KT_LIB_MODE KTINTEGER : public KTMemObject
{
public:
	KTINTEGER();
	
    void operator = (const CBigInteger& bigInt ){ setValue(bigInt); }; 
    void operator = (const KTString& strInt ){ setValue(strInt); }; 
    void operator = (int nValue){ setValue(nValue); };
	
    void setValue(const CBigInteger& bigInt, KTBOOL bMinus= FALSE);
    void setValue(const KTString& strInt);
    void setValue(int nInt);
    
    int value() const;				    // retrieve value
    void getValue(int& nInt) const{ nInt = value(); };
    void getValue(KTString& strInt) const;
    void getValue(CBigInteger& bigInt, KTBOOL& bMinus) const;

    KTBOOL isOverMaxInt() const; //???

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTEnumerated

class KT_LIB_MODE KTEnumerated : public KTINTEGER
{
public:
	KTEnumerated() { setType(UniversalEnumeration); }
};

//note: KTReal depend on i386 cpu
class KT_LIB_MODE KTReal : public KTMemObject
{
public:
	KTReal();

    void operator = (double dbValue){ setValue(dbValue); }; 
	double value();
    
    void setValue(double dbValue);
    void getValue(double& dbValue){ dbValue = value(); };

protected:
	int checkContent() const;
	KTBOOL m_bIsOverBound;  //checked after Value(); 1999.1.7
};


///////////////////////////////////////////////////////////////////////////////
// KTAbstractString: parent class of SPrintableString, SIA5String, ST61String....

class KT_LIB_MODE KTAbstractString : public KTMemObject		
{
public:
	KTAbstractString() { setContentCount(0); }

	int setValue(const char* strSrc, int nLength=-1);
	virtual void getValue(KTString &strDst) const;
};

///////////////////////////////////////////////////////////////////////////////
// KTOCTETSTRING

class KT_LIB_MODE KTOCTETSTRING : public KTAbstractString
{
public:
	KTOCTETSTRING();
	
    void setValue(const KTObject& obj);				// content present whole DER encode of obj
	void setValue(const BYTE* pbFrom, int nSize);	// memory exception
	//void setValue(const CBigInteger& bigInt, int nMinSize = 0, int nMaxSize = INT_MAX); // memory exception
	
    int getValue(KTObject& obj) const;
    void getValue(KTByteArray& rData) const;	
   // void getValue(CBigInteger& bigInt) const;

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTBITSTRING

class KT_LIB_MODE KTBITSTRING : public KTAbstractString
{
public:
	KTBITSTRING();
    
    void setValue(const KTOCTETSTRING& octStr);		// memory exception, convert OctetString to BitString
	void getValue(KTOCTETSTRING& octStr) const;		// memory exception, convert BitString to OctetString
	
    void setValue(KTUINT32 unValue);
    KTUINT32 getValue() const;

    void createBits(int nBitCount);
    int  bitCount() const	
	{ 
		assert(m_head.m_nLength > 0); 
		return (m_head.m_nLength - 1)* 8 - (*m_pbContent);
	}

	void setBit(int nPos, KTBOOL bSet = TRUE);
    void setBitEx(const int nPos); //nPos can more than bitCount
    KTBOOL isBitSet(int nPos) const;
	
    int firstSetBit() const;  //if no, return -1
    void clear();										// clear all Bits Set

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTPrintableString

class KT_LIB_MODE KTPrintableString : public KTAbstractString
{
public:
	KTPrintableString()	{ setType(UniversalPrintableString); }

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTIA5String

class KT_LIB_MODE KTIA5String : public KTAbstractString
{
public:
	KTIA5String() { setType(UniversalIA5String); }

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTTeletexString

class KT_LIB_MODE KTTeletexString : public KTAbstractString
{
public:
	KTTeletexString() { setType(UniversalTeletexString); }

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTBMPString

class KT_LIB_MODE KTBMPString : public KTOCTETSTRING
{
public:
	KTBMPString()
	{
		setType(UniversalBMPString);
	}
	virtual void getValue(KTString &strBMP) const;
	
	////return convert character from bmpstring
	static int str2bmpstr(const KTCHAR* pcStr, KTUINT unSTRSize, KTByteArray& rOutput);
	static int bmpstr2str(const KTCHAR* pcBMP, KTUINT unBMPSize, KTCHAR str[]);
	static int utf8str2str(const KTCHAR* pcUTF8, KTUINT unUTF8Size, KTCHAR str[]);

	// 2014-11
	static int ktstr2bmpstr(const char *pcStr, unsigned int unStrSize, char *pcBMP, bool bBigEndian = true);
	static int ktstr2utf8str(const char *pcStr, unsigned int unStrSize, char *pcUTF8);
	static int ktbmpstr2str(const char *pcBMP, unsigned int unBMPSize, char *pcStr, bool bBigEndian = true);
	static int ktbmpstr2utf8str(const char *pcBMP, unsigned int unBMPSize, char *pcUTF8, bool bBigEndian = true);
	static int ktutf8str2str(const char *pcUTF8, unsigned int unUTF8Size, char *pcStr);
	static int ktutf8str2bmpstr(const char *pcUTF8, unsigned int unUTF8Size, char *pcBMP, bool bBigEndian = true);

protected:
	int checkContent() const;
};

void ks_gbk_to_utf8(KTString& str);
void ks_utf8_to_gbk(KTString& str);

int ks_get_text_encoding(unsigned char *pbIn, int nLen);
bool ks_is_text_gb18030(unsigned char *pbIn, int nLen);
bool ks_is_text_utf8(unsigned char *pbIn, int nLen);

///////////////////////////////////////////////////////////////////////////////
// KTVisibleString

class KT_LIB_MODE KTVisibleString : public KTAbstractString
{
public:
	KTVisibleString() { setType(UniversalVisibleString); }

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTNumericString

class KT_LIB_MODE KTNumericString : public KTAbstractString
{
public:
	KTNumericString() { setType(UniversalNumericString); }

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTUTF8String

class KT_LIB_MODE KTUTF8String : public KTOCTETSTRING
{
public:
	KTUTF8String() { setType(UniversalUTF8String); }

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTUniversalString
class KT_LIB_MODE KTUniversalString : public KTOCTETSTRING
{
public:
	KTUniversalString() { setType(UniversalUniversalString);}

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTUTCTime

#define MAX_UTC_LEN 20
#define TIME_YEAR	0
#define TIME_MONTH	1
#define TIME_DAY	2
#define TIME_HOUR	3
#define TIME_MINUTE	4
#define TIME_SECOND	5

class KT_LIB_MODE KTUTCTime : public KTMemObject
{
public:
	KTUTCTime()
	{
		setType(UniversalUTCTime);	
	}
    
    KTBOOL isLocalForm(){ return FALSE; };
    void setLocalForm(){ assert(FALSE); };

	void setValue(int nYear, int nMonth, int nDay, int nHour, int nMinutes,
				KTBOOL bSecondsExist, int nSeconds,
				KTBOOL bOffsetExist, int nOffset);
	void getValue(int& nYear, int& nMonth, int& nDay, int& nHour, int& nMinute,
				KTBOOL& bSecondsExist, int& nSeconds,
				KTBOOL& bOffsetExist, int& nOffset)const;

	void setValue(const struct tm& tm);
	void getValue(struct tm& tm)const;
	
protected:
	int checkContent() const;
};

////////////////////////////////////////////////////////////////////////////////
// KTGeneralizedTime

#define MAX_GENERALIZED_TIME_LEN	32

class KT_LIB_MODE KTGeneralizedTime : public KTMemObject
{
public:
	KTGeneralizedTime()
	{
		setType(UniversalGeneralizedTime);
	}
	
    KTBOOL isLocalForm(){ return FALSE; };
    void setLocalForm(){ assert(FALSE); };

	// note: because the encode of GeneralizeTime is not sure, 
	// so MUST transfer paras as SetTime(year, month, day, hour, minutes, TRUE, seconds);
	int setValue(int nYear, int nMonth, int nDay, int nHour, int nMinutes,
				KTBOOL bSecondsExist = FALSE, int nSeconds = 0,
				KTBOOL bFractionExist = FALSE, int nFraction = 0, int nLeadZero = 0,
				KTBOOL bOffsetExist = FALSE, int nOffset = 0);
	int getValue(int& nYear, int& nMonth, int& nDay, int& nHour, int& nMinute,
				KTBOOL& bSecondsExist, int& nSeconds,
				KTBOOL& bFractionExist, int& nFraction, int& nLeadZero,
				KTBOOL& bOffsetExist, int& nOffset) const;

    int setValue(const struct tm& tm);
	int getValue(struct tm& tm)const;

protected:
	int checkContent() const;
};

///////////////////////////////////////////////////////////////////////////////
// KTObjectIdentifier

#define MAX_OCTET_EACH_OID	4

class KT_LIB_MODE KTObjectIdentifier : public KTMemObject
{
public:
	KTObjectIdentifier();
    
    void setValue(const int* pnValue, int nCount);
	void setValue(const char* pc_objID);
	void getValue(KTString &strOID) const;
    KTBOOL operator == (const char* const pc_objID) const;
    const char* operator = (const char* pc_objID);

private:
	int indexNum(int nPos) const;
    
protected:
	int checkContent() const;

private:
    mutable int m_nNumCount;
};
typedef KTObjectIdentifier KTOBJECTIDENTIFIER;

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/07 22:53  Hongwei Yu   created 
///   1.0.0.1  2003/06/10 11:45  Hongwei Yu   refactory 

#endif

