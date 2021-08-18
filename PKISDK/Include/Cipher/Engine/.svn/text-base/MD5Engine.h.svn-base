#ifndef __MD5_ENGINE_H__
#define __MD5_ENGINE_H__

/**
  * @Name: MD5Engine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 04:01
  * @Description:  the MD5 engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_MD5_ALGO
#include "MDEngineImpl.h"

class CMD5Implement;
 
class CMD5Engine : public IMDEngineImpl<CMD5Engine>
{
public:
	CMD5Engine();
	virtual ~CMD5Engine();

	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen);

	KTINT32 final(KTBYTE* pOuput, KTUINT* pnOutputLen);

	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}
	
private:
	CMD5Implement *m_pImpl;
	static const KT_ALGORITHM_INFO s_algoInfo;
	KT_DECLARE_INTERFACE()	
};

#endif //KT_NO_MD5_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 04:08  Hongwei Yu   created 

#endif
