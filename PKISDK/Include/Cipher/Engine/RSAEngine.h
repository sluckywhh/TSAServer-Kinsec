#ifndef __RSA_ENGINE_H__
#define __RSA_ENGINE_H__

/**
  * @Name: RSAEngine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 04:16
  * @Description:  the RSA engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_RSA_ALGO
#include "CipherEngineImpl.h"

class CKey;
class CAlgorithmParameter;
class CRSAImplement;
class CRSAEngine : public ICipherEngineImpl<CRSAEngine>
{
public:
	CRSAEngine();

	virtual ~CRSAEngine();

	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}
	
	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen);

	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL);

	static const KT_ALGORITHM_INFO* getAlgorithmInfo(){return &s_algoInfo;}
	
private:
	KTUINT32 m_nMode;
	CRSAImplement *m_pImpl;
private:
	static const KT_ALGORITHM_INFO s_algoInfo;	
	KT_DECLARE_INTERFACE()
};


#endif //#ifndef KT_NO_RSA_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 04:17  Hongwei Yu   created 

#endif
