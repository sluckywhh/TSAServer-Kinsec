#ifndef __KEY_GENERATOR_IMPLEMENT_H__
#define __KEY_GENERATOR_IMPLEMENT_H__

/**
  * @Name: KeyGeneratorImpl.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:24
  * @Description:  the key generator implement define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "../KeyGenerator.h"

class CKey;
class CAlgorithmParameter;

template<class Generator>
class IKeyGeneratorImpl 
: public IKeyGenerator
{
public:
	IKeyGeneratorImpl()
	{
	}
	KTINT32 init(CAlgorithmParameter* pParam)
	{
		// TODO: Add your specialized code here.
		// NOTE: Requires a correct return value to compile.
		Generator *pT=(Generator*)this;
		return pT->init(pParam);
	}

	KTINT32 generate(CKey ** pKey)
	{
		// TODO: Add your specialized code here.
		// NOTE: Requires a correct return value to compile.
		assert(pKey);
		Generator *pT=(Generator*)this;
		return pT->generate(pKey);
	}
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:24  Hongwei Yu   created 

#endif
