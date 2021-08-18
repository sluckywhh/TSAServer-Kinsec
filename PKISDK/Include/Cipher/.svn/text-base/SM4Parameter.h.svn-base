#ifndef __SM4_PARAMETER_H__
#define __SM4_PARAMETER_H__

/**
  * @Name: SM4Parameter.h
  * @Copyright: KTXA SoftWare
  * @Description:  the SM4 parameter define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_SM4_ALGO

#include "AlgorithmParameter.h"

class CSM4Parameter : public CAlgorithmParameter
{
public:
	CSM4Parameter()
	{
		m_nKeySize = 16;
		m_nEffectiveBits = 128;
	}
	CSM4Parameter(int nKeySize, int nEffectiveBits)
	{
		m_nKeySize = nKeySize;
		m_nEffectiveBits = nEffectiveBits;
	}
	int getKeySize() const
	{
		return m_nKeySize;
	}
	int getEffectiveBits() const
	{
		return m_nEffectiveBits;
	}
private:
	int m_nKeySize;
	int m_nEffectiveBits;
};

#endif //#ifndef KT_NO_SM4_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

#endif
