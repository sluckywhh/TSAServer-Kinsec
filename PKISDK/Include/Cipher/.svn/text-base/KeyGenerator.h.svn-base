#ifndef __KEY_GENERATOR_H__
#define __KEY_GENERATOR_H__

/**
  * @Name: KeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:30
  * @Description: 对称密钥生成器接口
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KTUnknow.h"
class CAlgorithmParameter;
class CKey;
class IKeyGenerator : public IKTUnknow
{
public:
    /* Description: 
	*	initializing  parameter for generator.
	* Parameters: 
	*   pParam:the base class pointer to class parameter.[IN]
	*		   pParam=NULL if no parameter.
	* Return : 
	*	KT_OK if successed.
	*/
	virtual KTINT32 init(CAlgorithmParameter* pParam) = 0;

    /* Description: 
	*	generate symmetric key.
	* Parameters: 
	*   ppKey:the address of key's pointer.[OUT]
	*		  note. the key must be delete.e.g.
	*		      CKey *pKey=NULL;
	*	 	      ...
	*	              pGenerator->generator(&pKey);
	*		      ...
	*		      pKey->Release();
	*				
	* Return : 
	*	KT_OK if successed.
	*/
	virtual KTINT32 generate(CKey ** ppKey) = 0;

};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:32  Hongwei Yu   created 

#endif 
