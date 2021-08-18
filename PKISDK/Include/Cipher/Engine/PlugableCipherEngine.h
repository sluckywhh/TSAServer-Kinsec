#ifndef __PLUGABLE_CIPHER_ENGINE_H__
#define __PLUGABLE_CIPHER_ENGINE_H__

/**
  * @Name: PlugableCipherEngine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 04:12
  * @Description:  the plugable cipher engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_PLUGABLE_ALGO

class CKey;
class CAlgorithmParameter;
class CPlugableAlgorithm;
#include "CipherEngineImpl.h"

class CPlugableCipherEngine : public ICipherEngineImpl<CPlugableCipherEngine>
{
public:
	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL);

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen);
	
	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo);

	CPlugableCipherEngine(){
		KT_INIT_INTERFACE();
		m_nAlgoIndex=-1;
	}
	virtual ~CPlugableCipherEngine(){};

	static void getAlgorithmInfo(KTAlgoInfoArray& rAlgoInfo);
	
private:
	int  m_nAlgoIndex;
	KTUINT32 m_nMode;
	KTByteArray m_rKey;
	KT_DECLARE_INTERFACE()
};
#endif //#ifndef KT_NO_PLUGABLE_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 04:13  Hongwei Yu   created 

#endif
