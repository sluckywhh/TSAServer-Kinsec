#ifndef __KEY_PAIR_GENERATOR_H__
#define __KEY_PAIR_GENERATOR_H__

/**
  * @Name: KeyPairGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:33
  * @Description: 非对称密钥生成器接口
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KTUnknow.h"

class CKey;
class CAlgorithmParameter;

class IKeyPairGenerator :public IKTUnknow
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
	*	generate unsymmetric key.
	* Parameters: 
	*   ppPubKey:the address of public key's pointer.[OUT]
	*   ppPrvKey:the address of private key's pointer.[OUT]
	*		  note. the key must be delete.e.g.
	*		      CKey *pPubKey=NULL;
	*			  CKey *pPrvKey=NULL;
	*	 	      ...
	*	          pGenerator->generator(&pPubKey,&prvKey);
	*		      ...
	*		      pPubKey->Release();
	*			  pPrvKey->Release();	
	*				
	* Return : 
	*	KT_OK if successed.
	*/
	virtual KTINT32 generate(CKey** ppPubKey, CKey** ppPrvKey) = 0;
	
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:34  Hongwei Yu   created 

#endif
