#ifndef __RC4_ENGINE_H__
#define __RC4_ENGINE_H__

/**
  * @Name: RC4Engine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 04:16
  * @Description:  the RC4 engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_RC4_ALGO
#include "CipherEngineImpl.h"

class CKey;
class CAlgorithmParameter;
class CRC4Implement;
class CRC4Engine : public ICipherEngineImpl<CRC4Engine>
{
public:
	CRC4Engine();

	virtual ~CRC4Engine();

	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}
	
	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL);

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen);

	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}
	
private:
	static const KT_ALGORITHM_INFO s_algoInfo;	
	CRC4Implement	*m_pImpl;
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
///   1.0.0.0  2003/04/26 04:16  Hongwei Yu   created 

#endif 
