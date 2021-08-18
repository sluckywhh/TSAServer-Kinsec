#ifndef __RC2_PARAMETER_H__
#define __RC2_PARAMETER_H__

/**
  * @Name: RC2Parameter.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:42
  * @Description:  the RC2 parameter define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_RC2_ALGO

#include "AlgorithmParameter.h"

class CRC2Parameter : public CAlgorithmParameter
{
public:
	CRC2Parameter()
	{
		m_nKeySize=16;
		m_nEffectiveBits=128;
	}
	CRC2Parameter(int nKeySize, int nEffectiveBits)
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

#endif //#ifndef KT_NO_RC2_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:42  Hongwei Yu   created 

#endif
