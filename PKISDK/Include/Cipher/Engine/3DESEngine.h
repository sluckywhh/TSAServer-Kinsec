#ifndef __3DES_ENGINE_H__
#define __3DES_ENGINE_H__

/**
  * @Name: 3DESEngine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:33
  * @Description:  the 3DES algorithm engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_3DES_ALGO

#include "CipherEngineImpl.h"

class C3DESImplement;
class CKey;
class CAlgorithmParameter;

class C3DESEngine : public ICipherEngineImpl<C3DESEngine>
{
public:
	C3DESEngine();

	virtual ~C3DESEngine();

	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}
	
	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL);

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen);
	
	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}
private:
	C3DESImplement *m_pImpl;
	KTBOOL m_bEncode;
	static const KT_ALGORITHM_INFO s_algoInfo;
	KTBOOL m_bCBC;

	KT_DECLARE_INTERFACE();	
};

#endif //KT_NO_3DES_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:38  Hongwei Yu   created 

#endif 
