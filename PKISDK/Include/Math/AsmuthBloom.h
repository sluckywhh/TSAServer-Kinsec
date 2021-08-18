#ifndef __ASMUTH_BLOOM_H__
#define __ASMUTH_BLOOM_H__

/**
  * @Name: AsmuthBloom.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 02:33
  * @Description: define class CAsmuthBloom 
*/ 

#define MAXINTEGERBITNUM    1024
#define MAXSHADOWCOUNT      10

#include "BigInteger.h"

class CAsmuthBloom
{
public:
	int m_nShadowCount;
	int m_nThreshold;
	int m_nKeyBitNum;

	CBigInteger m_bnK[MAXSHADOWCOUNT];		//shadow 
	CBigInteger m_bnM;			//be divided key
	CBigInteger m_bnEM;			//be restored key

//	CBigInteger m_bnU;			//restore key middle num
	CBigInteger m_bnR;			//restore key middle num
	CBigInteger m_bnP;			//random num
	CBigInteger m_bnD[MAXSHADOWCOUNT];		//public num

public:
	CAsmuthBloom();
	int preDivKey();
	int divideKey();
	int restoreKey(CBigInteger *bnK , CBigInteger *bnD);
	void init(int m,int n);
	void setKey(CBigInteger &nbkey);

protected:
	int conditionVerify();

private:
	CBigInteger m_bnBM;			//BM=M+RP

};

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 02:38  Hongwei Yu   created 

#endif
