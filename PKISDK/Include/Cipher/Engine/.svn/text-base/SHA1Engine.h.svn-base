#ifndef __SHA1_ENGINE_H__
#define __SHA1_ENGINE_H__

/**
  * @Name: SHA1Engine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 09:58
  * @Description:  the SHA1 engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_SHA1_ALGO
#include "MDEngineImpl.h"

class CSHA1Implement;

class CSHA1Engine : public IMDEngineImpl<CSHA1Engine>
{
public:
	CSHA1Engine();
	virtual ~CSHA1Engine();

	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen);

	KTINT32 final(KTBYTE* pOuput, KTUINT* pnOutputLen);

	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}
private:
	CSHA1Implement *m_pImpl;
	static const KT_ALGORITHM_INFO s_algoInfo;	

	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_SHA1_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 09:59  Hongwei Yu   created 

#endif
