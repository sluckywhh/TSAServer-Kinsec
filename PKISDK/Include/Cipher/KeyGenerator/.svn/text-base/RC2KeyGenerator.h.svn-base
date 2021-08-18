#ifndef __RC2_KEY_GENERATOR_H__
#define __RC2_KEY_GENERATOR_H__

/**
  * @Name: RC2KeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:32
  * @Description:  the RC2 key generator define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_RC2_ALGO

class CKey;
class CAlgorithmParameter;


class CRC2KeyGenerator : public IKeyGeneratorImpl<CRC2KeyGenerator>
{
public:
	CRC2KeyGenerator();

	~CRC2KeyGenerator();

	KTINT32 init(CAlgorithmParameter* pParam);

	KTINT32 generate(CKey ** pKey);

private:
	int m_nKeySize;
	int m_nEffectiveBits;
	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_RC2_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:33  Hongwei Yu   created 

#endif
