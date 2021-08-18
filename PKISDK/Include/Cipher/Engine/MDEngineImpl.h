#ifndef __MD_ENGINE_IMPLEMENT_H__
#define __MD_ENGINE_IMPLEMENT_H__

/**
  * @Name: MDEngineImpl.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:49
  * @Description:  the MD engine implement define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

template<class Engine>
class IMDEngineImpl 
: public IMDEngine
{
public:
	IMDEngineImpl()
	{
	}
	virtual ~IMDEngineImpl()
	{

	}
	inline KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen)
	{
		// ToDo: Add your specialized code here
		assert(pInput);
		if(nInputLen)
			return KT_OK;
		Engine *pT=(Engine*)this;
		return pT->update(pInput,nInputLen);
	}


	inline KTINT32 final(KTBYTE* pOutput, KTUINT* pnOutputLen)
	{
		// TODO: Add your specialized code here.
		// NOTE: Requires a correct return value to compile.
		assert(pnOutputLen);
		assert(pOutput);
		Engine *pT=(Engine*)this;
		return pT->final(pOutput,pnOutputLen);
	}
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:34  Hongwei Yu   created 

#endif 
