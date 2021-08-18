#ifndef __BIG_INTEGER_H__
#define __BIG_INTEGER_H__

/**
  * @Name: BigInteger.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 02:27
  * @Description: define some big integer class 
*/ 

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

typedef unsigned int DIGIT;

#define MAX_DIGITS 144	// 256		// can be defined other value
#pragma warning(disable:4251)

class CDecBigInteger;
class KT_LIB_MODE CBigInteger  
{
friend class CBigConvert;
public:
//***************
//construct:

	CBigInteger();

	/**
	* construct from the un
	*/
//	CBigInteger(KTUINT32 un);

	virtual ~CBigInteger();
//***************
//attribute operator:
	/**
	* Set BigInteger constant zero.
	*/
	void setZero();  
	
	/**
	* Returns the number of bits.
	* <p>
	* @return the number of bits.
	*/
	int getBitCount()const;    

	/**
	* Returns the number of byte.
	* <p>
	* @return the number of byte.
	*/
	int getByteCount()const;   

	/**
	* Set the n-th bit to value
	*<p>
	*@param n:the n-th bit
	*/
	void setBit(int n);        
	
	/**
	* Get the n-th bit to value
	* <p>
	* @param n:the n-th bit
	* <p>
	* @return the n-th bit,
	*/
	KTBOOL getBit(int n);      
	
	/**
	* TRUE if it is odd
	* <p>
	* @return TRUE if it is odd
	*/
	KTBOOL isOdd();   

//***************
//base operator:
	const CBigInteger& operator=(const CBigInteger& bn);
	const CBigInteger& operator = (const CDecBigInteger& dbn);
	
	void operator+=(const CBigInteger& bn);
	void operator+=(DIGIT un);

	void operator-=(const CBigInteger& bn);
	void operator-=(DIGIT un);

	void operator*=(const CBigInteger& bn);

	void operator/=(const CBigInteger& bn);
	
	void operator%=(const CBigInteger& bn);

	void operator<<=(unsigned int n);
	void operator>>=(unsigned int n);
	const DIGIT operator[](unsigned int n);
	
    KTBYTE* getPoint();

//***************	
//other operator:

	/**
	* hi * 2 ^ nBits + *this
	*/
	void cut(CBigInteger& hi, int nBits);  

	/**
	* *this=bnDiv%bnDivRev
	*/
	void mod(const CBigInteger& bnDiv, const CBigInteger& bnDivRev);  

	/**
	* get reciprocal of this to bnRec
	* param bnRec:result
	*/
	void reciprocal(CBigInteger& bnRec) const;
	
	/**
	* bnQuotient=*this/bnDiv
	*/
	void modDiv(const CBigInteger& bnDiv, CBigInteger& bnQuotient);

	/**
	* *this=*this/2
	*/
	void half();	// not asm, use / 2

	/**
	* *this=*this*2   //asm
	*/
	void doubl();	

	/**
	* *this=*this*(*this)   //asm essential
	*/
	void power();	

#ifdef NO_INTEL_32BITS
	void copyTo(CBigInteger& dest);
	void copyFrom(const CBigInteger source);
#endif

	/**
	* the following 2 functions:
	* Compares this BigInteger with the specified BigInteger or the DIGIT
	* @param bn:the specified BigInteger
	*		 un:the specified DIGIT
	* @return  0  if *this==bn 
	*          >0 if *this>bn
	*          <0 if *this<bn 
	*/
	int compare(const CBigInteger& bn) const;
	int compare(const DIGIT un) const;


//***************
//input/output
	/**
	* the following 4 function
	* Put a byte array containing the two's-complement representation of this BigInteger
	*	or Get this BigInteger from a byte array containing the two's-complement.
	* the two's-complement is HEX type
	* example: 0x0a09   ==>  0x0a and 0x09
	* <p>
	* @param rSource/rResult :a byte array containing the two's-complement
	*       strSource/strResult:a byte point containing the two's-complement
	*       nLen:length.
	*       bReverse:TRUE if it need reverse.
	*/
	void translateFrom(const KTBYTE* strSource,int nLen,KTBOOL bReverse = TRUE);
	void translateFrom(const KTByteArray& rSource, KTBOOL bReverse = TRUE);
	KTINT32 translateTo(KTBYTE* strResult,int nExpectedLen,KTBOOL bReverse = TRUE );
	KTINT32 translateTo(KTByteArray& rResult, KTBOOL bReverse = TRUE );

	
	/**
	* Put the binary String representation of this BigInteger
	* <p>
	* @param output :the binary String representation of this BigInteger
	*        nMinSize:min size of rOutput
	*		 nMaxSize:max size of rOutput
	*/
	void output(KTByteArray& rOuput,int nMinSize,int nMaxSize);		

//**************
//other
	/**
	* the following 3 functions:
	* Create random big integer.
	* <p>
	* @param nMinDigits/nMaxdigits: nMinDigits<m_nDigits<nMaxDigits;
	*        bnLessThan:*this<bnLessThan
	*        nBits:the count of bits = nBits
	*			   base 1, because it's bit number, not index
	*/
	void createRandom(int nMinDigits, int nMaxDigits);
	void createRandom(const CBigInteger& bnLessThan);
	void createRandom(int nBits);	
	
	void createReverse(int nLength, const void * pvBuffer);
	void create(int nLength, const void * pvBuffer);
	/**
	* set upper bound, and use high digit mask
	* this function is useful to generate RSA key with exact bit number, 
	* <p>
	* @param 
	*/
	void bound(int nDigits, DIGIT unSignificantMask);	// 
				

	/**
	* bn1 <==> bn2
	*/
	void static convert(CBigInteger& bn1, CBigInteger& bn2);		

	/**
	* Returns a BigInteger whose value is the greatest common divisor of abs(bn1) and abs(bn2).
	* @bnGCD:result
	*/
	void static gCD(const CBigInteger& bn1, const CBigInteger& bn2, CBigInteger& bnGCD);

	/**
	* Search a prime of nDigits bits
	* @param bnPrime:result
	*/
	void static searchPrime(CBigInteger& bnPrime, int nDigits);
	
	/**
	* the following 2 functions:
	* Search a strong prime of nDigits bits
	* @bnPrime:result
	*/
	void static searchStrongPrime(CBigInteger& bnPrime, int nDigits);
	void static searchStrongPrime(CBigInteger& bnPrime, CBigInteger& bnComFactor, CBigInteger& bnPrimeFactor, int nDigits);

	/**
	* if bnMod >= bnNormal
	*	 bnInverse = (bnNormal **-1) MOD bnMod
	*/
	static void inverse(const CBigInteger& bnNormal, const CBigInteger& bnMod, CBigInteger& bnInverse);
		
	/**
	* Get a BigInteger whose value is (bnBase ** bnPower mod bnMod).
	* @	bnBase < bnMod &&  bnMod > 0
	*	bnResult= bnBase ** bnPower mod bnMod
	*/
	static KTBOOL modPower(const CBigInteger& bnBase, 
			const CBigInteger& bnPower, 
			const CBigInteger& bnMod, 
			CBigInteger& bnResult);
protected:
	int realDigits(int nMaxDigits = MAX_DIGITS) const;

	// note : all ModDiv__ function don't adjust and check digits number
	// because they are just intermidiate functions.
	// they all are inline assemble(or inline) functions

	// the following 6 functions dependent on ModDiv__ functions
	void multiDigitsDiv(const CBigInteger& bnDiv);
	void singleDigitDiv(DIGIT nDiv);	
	void multiDigitsMod(const CBigInteger& bnDiv);
	void singleDigitMod(DIGIT nDiv);	
	void multiDigitsModDiv(const CBigInteger& bnDiv, CBigInteger& bnQuotient);
	void singleDigitModDiv(DIGIT nDiv, CBigInteger& bnQuotient);	

	void modDiv__MulSub(DIGIT* pToSub, const DIGIT* pSub, 
						int nSubDigits, DIGIT nMul);	// essential
	void modDiv__Sub(DIGIT* pToSub, const DIGIT* pSub,
						int nSubDigits);	// equal above while nMul = 1
	int  modDiv__Compare(const DIGIT* pToCompare, const DIGIT* pCompare,
						int nDigits);
	void modDiv__IntAdd(DIGIT* pToAdd, DIGIT nAdd); 

#ifdef _DEBUG
    public:
	    static void test(char *cond);
#endif
        
protected:
	int m_nDigits;
	DIGIT m_uDigits[MAX_DIGITS];	
	
	DIGIT m_uBE_Digits[MAX_DIGITS];	// for BIG_ENDIAN machine
};


inline int CBigInteger::getByteCount()const
{
	return m_nDigits;
}


///////////////////////////////////////////////////////////////////////////////
// CDecBigInteger

#define MAX_DECDIGITS 2048
#define DECDIGIT int

class KT_LIB_MODE CDecBigInteger
{
public:
	CDecBigInteger();

	void setZero();
    
	void setOne();

	int isOdd();
	
	KTBOOL isValidDecNumber() const;

    const CDecBigInteger& operator = (const CDecBigInteger& dbn);
	const CDecBigInteger& operator = (const CBigInteger& bnToConvert);
	
	void operator += (const CDecBigInteger& dbn);
	int operator > (DECDIGIT dValue);	

	void half();
	void doubl();

	void translateTo(char* strResult, KTBOOL bReverse = TRUE) const;
	void translateFrom(const char* strSourse, KTBOOL bReverse = TRUE);

	int digits() const;

protected:
	int m_nDecDigits;
	DECDIGIT m_baDecNumber[MAX_DECDIGITS];
};
inline CDecBigInteger::CDecBigInteger()
{ 
	setZero(); 
}
inline void CDecBigInteger::setZero()
{	
	memset(m_baDecNumber, 0, sizeof(DECDIGIT) * MAX_DECDIGITS);
	m_nDecDigits = 0;
}
inline void CDecBigInteger::setOne()	
{ 	
	setZero();
	m_baDecNumber[0] = 1;
	m_nDecDigits = 1;
}
inline int CDecBigInteger::isOdd()
{
	return m_baDecNumber[0]%2; 
}
inline int CDecBigInteger::digits() const 
{
	return m_nDecDigits; 
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 02:26  Hongwei Yu   created 

#endif
