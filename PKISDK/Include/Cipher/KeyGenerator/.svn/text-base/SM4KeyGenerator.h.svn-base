#ifndef __SM4_KEY_GENERATOR_H__
#define __SM4_KEY_GENERATOR_H__

/**
  * @Name: SM4KeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Description:  the SM4 key generator define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_SM4_ALGO

class CKey;
class CAlgorithmParameter;

class CSM4KeyGenerator : public IKeyGeneratorImpl<CSM4KeyGenerator>
{
public:
	CSM4KeyGenerator();

	~CSM4KeyGenerator();

	KTINT32 init(CAlgorithmParameter* pParam);

	KTINT32 generate(CKey ** pKey);

private:
	int m_nKeySize;
	int m_nEffectiveBits;
	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_SM4_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

#endif
