#ifndef __ENGINE_H__
#define __ENGINE_H__

/**
  * @Name: Engine.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:26
  * @Description: 加密算法接口
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

/*
* define the engine's mode.
*/
enum EM_MODE{
		EM_ENCRYPT,
		EM_DECRYPT,
		EM_SIGNATURE,
		EM_VERIFY,
		EM_CODE,
		EM_DECODE
};

/*
* define the algorithm type.
*/
const KTBYTE	KLA_SYMMETRIC_TYPE		=1;
const KTBYTE	KLA_UNSYMMETRIC_TYPE	=2;	
const KTBYTE	KLA_DIGEST_TYPE			=3;			

/*
* define the interface of cipher.
*/
class ICipherEngine : public IKTUnknow
{
public:
   /* Description: 
	*	initalizing.
	* Parameters: 
	*	nMode:the mode of cipher's engine.[IN]
	*		EM_ENCRYPT,EM_SIGNATURE, etc.
	*	pKey:the pointer of key.		  [IN]
	*		 it's may be NULL
	*	pParam:the parameter of algorithm.[IN]
	*		 it's may be NULL
	* Return : 
	*/
	virtual KTINT32 init(KTUINT32 nMode, CKey* pKey = NULL, CAlgorithmParameter* pParam = NULL) = 0;

    /* Description: 
	*	process data.
	* Parameters: 
	*	pInput:the inputed data.[IN]
	*	nInputData:the length of inputed data. [IN]
	*	pOutput:the outputed data.[OUT]
	*	pnOutputLen:the length of output buffer [IN]
	*			    the lenght of output data.  [OUT]
	* Return :
	*	KT_OK if successed.
	*/
	virtual KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen, KTBYTE* pOutput, KTUINT* pnOutputLen) = 0;

};
/*
* define the interface of message digest.
*/
class IMDEngine : public IKTUnknow
{
public:
    /* Description: 
	*	process data.
	* Parameters: 
	*	pInput:the inputed data.[IN]
	*	nInputData:the length of inputed data. [IN]
	* Return :
	*	KT_OK if successed.
	*/
	virtual KTINT32 update(const KTBYTE* pInput, KTUINT nInputLen) = 0;
	
    /* Description: 
	*	get message digest data.
	* Parameters: 
	*	pOutput:the outputed data.[OUT]
	*	pnOutputLen:the length of output buffer [IN]
	*			    the lenght of output data.  [OUT]
	* Return :
	*	KT_OK if successed.
	*/
	virtual KTINT32 final(KTBYTE* pOuput, KTUINT* pnOutputLen) = 0;
	
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:29  Hongwei Yu   created 

#endif 
