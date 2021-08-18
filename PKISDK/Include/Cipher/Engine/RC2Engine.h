#ifndef __RC2_ENGINE_H__
#define __RC2_ENGINE_H__

/**
  * @Name: RC2Engine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 04:13
  * @Description:  the RC2 engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_RC2_ALGO

class CRC2Implement;
class CKey;
class CAlgorithmParameter;
#include "CipherEngineImpl.h"

class CRC2Engine : public ICipherEngineImpl<CRC2Engine>
{
public:
	CRC2Engine();

	virtual ~CRC2Engine();

	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}
	
	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL);

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen);

	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}
	
private:
	static const KT_ALGORITHM_INFO s_algoInfo;	
	KTBOOL m_bEncode;
	CRC2Implement	*m_pImpl;
	KTBOOL m_bCBC;
	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_RC2_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 04:14  Hongwei Yu   created 

#endif 
