#ifndef __KEY_PAIR_GENERATOR_IMPLEMENT_H__
#define __KEY_PAIR_GENERATOR_IMPLEMENT_H__

/**
  * @Name: KeyPairGeneratorImpl.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:25
  * @Description:  the key pair generator implement define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "../KeyPairGenerator.h"

class CKey;

template<class Generator> 
class IKeyPairGeneratorImpl : public IKeyPairGenerator
{
public:
	IKeyPairGeneratorImpl()
	{
	}
	KTINT32 init(CAlgorithmParameter* pParam)
	{
		// TODO: Add your specialized code here.
		// NOTE: Requires a correct return value to compile.
		Generator *pT=(Generator*)this;
		return pT->init(pParam);
	}


	KTINT32 generate(CKey** pPubKey, CKey** pPrvKey)
	{
		// TODO: Add your specialized code here.
		// NOTE: Requires a correct return value to compile.
		assert(pPubKey);
		assert(pPrvKey);
		Generator *pT=(Generator*)this;
		return pT->generate(pPubKey,pPrvKey);
	}
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:27  Hongwei Yu   created 

#endif
