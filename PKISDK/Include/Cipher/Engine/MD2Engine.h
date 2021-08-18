#ifndef __MD2_ENGINE_H__
#define __MD2_ENGINE_H__

/**
  * @Name: MD2Engine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:49
  * @Description:  the MD2 engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_MD2_ALGO
#include "MDEngineImpl.h"

class CMD2Implement;

class CMD2Engine : public IMDEngineImpl<CMD2Engine>
{
public:
	CMD2Engine();
	virtual ~CMD2Engine();

	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen);

	KTINT32 final(KTBYTE* pOuput, KTUINT* pnOutputLen);

	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}
	
private:
	CMD2Implement *m_pImpl;
	static const KT_ALGORITHM_INFO s_algoInfo;

	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_MD2_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:50  Hongwei Yu   created 

#endif 
