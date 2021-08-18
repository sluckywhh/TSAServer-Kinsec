#ifndef __B64_ENGINE_H__
#define __B64_ENGINE_H__

/**
  * @Name: B64Engine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:33
  * @Description:  the B64 code engine define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KL_NO_B64_ALGO

#include "CipherEngineImpl.h"

class CKey;
class CAlgorithmParameter;
class CB64Implement;


class CB64Engine : public ICipherEngineImpl<CB64Engine>
{
public:
	KTBOOL isAlgorithm(const KTCHAR* pAlgoInfo){return TRUE;}

	KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL);

	KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOuput, KTUINT* pnOutputLen);

	CB64Engine();

	virtual ~CB64Engine();
private:
	CB64Implement *m_pImpl;
	KT_DECLARE_INTERFACE()
};

#endif //KL_NO_B64_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:40  Hongwei Yu   created 

#endif
