#ifndef __PLUGABLE_KEY_GENERATOR_H__
#define __PLUGABLE_KEY_GENERATOR_H__

/**
  * @Name: PlugableKeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:28
  * @Description:  the plugable key generator define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KeyGeneratorImpl.h"

#ifndef KT_NO_PLUGABLE_ALGO

class CAlgorithmParameter;
class CKey;

class CPlugableKeyGenerator : public IKeyGeneratorImpl<CPlugableKeyGenerator>
{
public:
	CPlugableKeyGenerator();

	~CPlugableKeyGenerator();

	KTINT32 init(CAlgorithmParameter* pParam);
	
	KTINT32 init(const KTCHAR* pAlgo);
	
	KTINT32 generate(CKey ** pKey);
private:
	int m_nAlgoIndex;
	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_PLUGABLE_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:29  Hongwei Yu   created 

#endif 
