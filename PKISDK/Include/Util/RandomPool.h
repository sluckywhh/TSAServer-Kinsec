#ifndef __RANDOMPOOL_H__
#define __RANDOMPOOL_H__

/**
  * @Name: RandomPool.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 00:05
  * @Description: class CRandomPool define
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

class KT_LIB_MODE CRandomPool 
{
  public:
      virtual ~CRandomPool();
      KTINT32 generate(KTUINT nLen,KTBYTE* pData);
      virtual void setSeed(long seed)=0;
  protected:
      CRandomPool();
      virtual KTINT32 doGenerate(KTUINT nLen,KTBYTE* pData)=0;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 00:05  Hongwei Yu   created 

#endif
