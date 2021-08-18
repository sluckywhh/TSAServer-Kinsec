#ifndef __CIPHER_ENGINE_IMPL_H__
#define __CIPHER_ENGINE_IMPL_H__

/**
  * @Name: CipherEngineImpl.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:44
  * @Description:  the cipher engine base class define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

class CKey;
class CAlgorithmParameter;

template<class Engine>
class ICipherEngineImpl : public ICipherEngine
{
public:
	ICipherEngineImpl()
	{
		// ToDo: Add your specialized code here and/or call the base class
	}
	virtual ~ICipherEngineImpl()
	{
		// ToDo: Add your specialized code here and/or call the base class
	}

	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL)
	{
		Engine *pT=(Engine*)this;
		return pT->init(nMode,pKey,pParam);	
	}
	
	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen)
	{
		// ToDo: Add your specialized code here
		assert(pInput);
		assert(pOuput);
		assert(pnOutputLen);		
		if(0 == nInputLen)
			return KT_OK;		
		Engine *pT=(Engine*)this;
		return pT->update(pInput,nInputLen,pOuput,pnOutputLen);
	}
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:45  Hongwei Yu   created 

#endif 
