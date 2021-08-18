#ifndef __AB_CONTROL_H__
#define __AB_CONTROL_H__

/**
  * @Name: ABControl.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 02:33
  * @Description: interface for the CABControl class 
*/ 

#include "AsmuthBloom.h"

class CABControl  
{
public:
	CABControl();
	virtual ~CABControl();
	int initSplit(int m, int n, int nBitNum, CBigInteger &bnM);
	int initRestore(int m, int n, const CBigInteger *bnD, const CBigInteger *bnK,
		const CBigInteger& bnR, const CBigInteger& bnP);
	int splitKey();
	int getKey(CBigInteger& bnKey);
	int getPublicNum(CBigInteger *bnR , CBigInteger *bnP);
	int getShadow(CBigInteger *bnK , CBigInteger *bnD);

private:
	CAsmuthBloom ab;
	CBigInteger m_bnKey;		//key, if restore key, get it 
	CBigInteger m_bnD[MAXSHADOWCOUNT];			//must open's bignum,correspond to m_bnK
	CBigInteger m_bnK[MAXSHADOWCOUNT];			//each one's private key 
	CBigInteger m_bnR;			//must open's middle bignum
	CBigInteger m_bnP;			//must open's middle bignum
};

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 02:40  Hongwei Yu   created 

#endif
