#ifndef __SOFTRANDOMPOOL_H__
#define __SOFTRANDOMPOOL_H__

/**
  * @Name: SoftRandomPool.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 00:09
  * @Description: class CSoftRandomPool define
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "RandomPool.h"

class CSoftRandomPool : public CRandomPool  
{
public:
	CSoftRandomPool();
	virtual ~CSoftRandomPool();
	virtual void setSeed(long seed){};
protected:
	KTINT32 doGenerate(KTUINT nLen,KTBYTE* pData);
};

inline KTINT32 K_SoftGenerateRandom(KTUINT nLen,KTBYTE* pData){
	CSoftRandomPool pool;
	return pool.generate(nLen,pData);
}

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 00:09  Hongwei Yu   created 

#endif
