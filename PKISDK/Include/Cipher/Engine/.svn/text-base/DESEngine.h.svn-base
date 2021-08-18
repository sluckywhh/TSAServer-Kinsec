#ifndef __DES_ENGINE_H__
#define __DES_ENGINE_H__

/**
  * @Name: DESEngine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:46
  * @Description:  the DES engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_DES_ALGO

#include "CipherEngineImpl.h"

class CKey;
class CAlgorithmParameter;
class CDESImplement;

class CDESEngine : public ICipherEngineImpl<CDESEngine>
{
public:
	CDESEngine();
	virtual ~CDESEngine();
	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}
	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL);
	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen);

	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}

private:
	CDESImplement *m_pDESImpl;
	KTBOOL		   m_bEncode;
	static const KT_ALGORITHM_INFO s_algoInfo;	
	KTBOOL		   m_bCBC;

	KT_DECLARE_INTERFACE()
};

#endif// KT_NO_DES_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:47  Hongwei Yu   created 

#endif
