#ifndef __CIPHER_FACTORY_H__
#define __CIPHER_FACTORY_H__

/**
  * @Name: CipherFactory.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:26
  * @Description: 加密算法组件工厂头文件
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

//define algoritm info struct
//define algorithm type.
#include "Engine.h"
#include "KTInterface.h"

#include "./Engine/DESEngine.h"
#include "./Engine/3DESEngine.h"
#include "./Engine/RC2Engine.h"
#include "./Engine/RC4Engine.h"
#include "./Engine/RSAEngine.h"

#include "./Engine/MD2Engine.h"
#include "./Engine/MD5Engine.h"
#include "./Engine/SHA1Engine.h"
#include "./Engine/B64Engine.h"
#include "./Engine/PlugableCipherEngine.h"

#include "./KeyGenerator/RSAKeyGenerator.h"
#include "./KeyGenerator/DESKeyGenerator.h"
#include "./KeyGenerator/3DESKeyGenerator.h"
#include "./KeyGenerator/RC2KeyGenerator.h"
#include "./KeyGenerator/RC4KeyGenerator.h"
#include "./KeyGenerator/SM4KeyGenerator.h"
#include "./KeyGenerator/PlugableKeyGenerator.h"

//T:CXXEngine
//T1:IXXEngine
template <class T,class T1>
class CCipherFactory{
public:
	KTINT32 createInstance(const KTCHAR* pInfo,void** ppv){
		T *pT=new T;
		CHECK_NEW(pT);
		if(FALSE == pT->isAlgorithm(pInfo)){
			delete pT;
			return KT_ERR_CRNT_NOSUPPORT;
		}
		pT->addRef();
		*ppv=(T1*)pT;
		return KT_OK;
	}
	KTINT32 createInstance(void** ppv){
		T *pT=new T;
		CHECK_NEW(pT);
		pT->addRef();
		*ppv=(T1*)pT;
		return KT_OK;
	}
};

/* Description:
*	Get the kind of algorithm's infomation from cipher factory.
*
* Parameters: 
*   pAlgoInfo:[OUT] :the array of KT_ALGORITHM_INFO
*   pnCount:[IN ]:the size of the array.
*           [OUT]:the count of algorithm.
* Return : KT_OK if successed.
*/
KTINT32 K_GetAlgorithmsInfo(KT_ALGORITHM_INFO *pAlgoInfo,KTUINT *pnCount);

template<class T>
KTINT32 K_Digest(
			   CCipherFactory<T,IMDEngine> *pFactory,
			   const KTBYTE* pInput,KTUINT nInputLen,
			   KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	KTINT32 rv=KT_OK;
	IMDEngine *pEngine=NULL;
	try{
		ThrowIf(pFactory->createInstance((void**)&pEngine));
		ThrowIf(pEngine->update(pInput,nInputLen));
		ThrowIf(pEngine->final(pOutput,pnOutputLen));	
	}
	catch( enumKTError r){
		rv=r;
	}
	if(pEngine)
		pEngine->release();
	return rv;
}

template<class T>
KTINT32 K_Cipher(
			   CCipherFactory<T,ICipherEngine> *pFactory,
			   EM_MODE mode,
			   CKey *pKey,CAlgorithmParameter *pParam,
			   const KTBYTE* pInput,KTUINT nInputLen,
			   KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	KTINT32 rv=KT_OK;
	ICipherEngine *pEngine=NULL;
	try{
		ThrowIf(pFactory->createInstance((void**)&pEngine));
		ThrowIf(pEngine->init(mode,pKey,pParam));
		ThrowIf(pEngine->update(pInput,nInputLen,pOutput,pnOutputLen));
	}
	catch( enumKTError r){
		rv=r;
	}
	if(pEngine)
		pEngine->release();
	return rv;
}
template<class T>
KTINT32 K_Cipher(
			   CCipherFactory<T,ICipherEngine> *pFactory,
			   const KTCHAR* pInfo,
			   EM_MODE mode,
			   CKey *pKey,CAlgorithmParameter *pParam,
			   const KTBYTE* pInput,KTUINT nInputLen,
			   KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	KTINT32 rv=KT_OK;
	ICipherEngine *pEngine=NULL;
	try{
		ThrowIf(pFactory->createInstance(pInfo,(void**)&pEngine));
		ThrowIf(pEngine->init(mode,pKey,pParam));
		ThrowIf(pEngine->update(pInput,nInputLen,pOutput,pnOutputLen));
	}
	catch( enumKTError r){
		rv=r;
	}
	if(pEngine)
		pEngine->release();
	return rv;
}
template<class T>
KTINT32 K_KeyGenerator(
					 CCipherFactory<T,IKeyGenerator> *pFactory,
					 CAlgorithmParameter *pParam,CKey **ppKey)
{
	KTINT32 rv=KT_OK;
	IKeyGenerator *pGenerator=NULL;
	try{
		ThrowIf(pFactory->createInstance((void**)&pGenerator));
		ThrowIf(pGenerator->init(pParam));
		ThrowIf(pGenerator->generate(ppKey));
	}
	catch( enumKTError r){
		rv=r;
	}
	if(pGenerator)
		pGenerator->release();
	return rv;
}

template<class T>
KTINT32 K_KeyPairGenerator(
						 CCipherFactory<T,IKeyPairGenerator> *pFactory,
						 CAlgorithmParameter *pParam,CKey **ppPubKey,CKey **ppPrvKey)
{
	KTINT32 rv=KT_OK;
	IKeyPairGenerator *pGenerator=NULL;
	try{
		ThrowIf(pFactory->createInstance((void**)&pGenerator));
		ThrowIf(pGenerator->init(pParam));
		ThrowIf(pGenerator->generate(ppPubKey,ppPrvKey));
	}
	catch( enumKTError r){
		rv=r;
	}
	if(pGenerator)
		pGenerator->release();
	return rv;
}
//*******************************************************************************************
inline KTINT32 K_RSA_Generator(CAlgorithmParameter *pParam,
							 CKey **ppPubKey,CKey** ppPrvKey)
{
	CCipherFactory<CRSAKeyGenerator,IKeyPairGenerator> factory;
	return K_KeyPairGenerator<CRSAKeyGenerator>(&factory,pParam,ppPubKey,ppPrvKey);
}

//*******************************************************************************************

inline KTINT32 K_DES_Generator(CAlgorithmParameter *pParam,
							 CKey **ppKey)
{
	CCipherFactory<CDESKeyGenerator,IKeyGenerator> factory;
	return K_KeyGenerator<CDESKeyGenerator>(&factory,pParam,ppKey);
}
inline KTINT32 K_3DES_Generator(CAlgorithmParameter *pParam,
							 CKey **ppKey)
{
	CCipherFactory<C3DESKeyGenerator,IKeyGenerator> factory;
	return K_KeyGenerator<C3DESKeyGenerator>(&factory,pParam,ppKey);
}
inline KTINT32 K_RC2_Generator(CAlgorithmParameter *pParam,
							 CKey **ppKey)
{
	CCipherFactory<CRC2KeyGenerator,IKeyGenerator> factory;
	return K_KeyGenerator<CRC2KeyGenerator>(&factory,pParam,ppKey);
}
inline KTINT32 K_RC4_Generator(CAlgorithmParameter *pParam,
							 CKey **ppKey)
{
	CCipherFactory<CRC4KeyGenerator,IKeyGenerator> factory;
	return K_KeyGenerator<CRC4KeyGenerator>(&factory,pParam,ppKey);
}
inline KTINT32 K_SM4_Generator(CAlgorithmParameter *pParam,
							 CKey **ppKey)
{
	CCipherFactory<CSM4KeyGenerator,IKeyGenerator> factory;
	return K_KeyGenerator<CSM4KeyGenerator>(&factory,pParam,ppKey);
}

//*******************************************************************************************

inline 
KTINT32 K_DES_Cipher(EM_MODE mode,
				   CKey *pKey,CAlgorithmParameter *pParam,
				   const KTBYTE* pInput,KTUINT nInputLen,
				   KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<CDESEngine,ICipherEngine> factory;
	return K_Cipher<CDESEngine>(&factory,mode,pKey,pParam,pInput,nInputLen,
					pOutput,pnOutputLen);
}
inline 
KTINT32 K_3DES_Cipher(EM_MODE mode,
				   CKey *pKey,CAlgorithmParameter *pParam,
				   const KTBYTE* pInput,KTUINT nInputLen,
				   KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<C3DESEngine,ICipherEngine> factory;		
	return K_Cipher<C3DESEngine>(&factory,mode,pKey,pParam,pInput,nInputLen,
		pOutput,pnOutputLen);
}
#define KTSYMM_ALGO_DES_EDE3_CBC		1
#define KTSYMM_ALGO_RC2_40_CBC			2
KTINT32 Openssl_EncryptOrDecrypt(
	bool bEncrypt,
	int nAlgo,
	unsigned char *pbKey,
	KTUINT nKeyLen,
	unsigned char *pbIV,
	KTUINT nIVLen,
	unsigned char *pbIn,
	KTUINT nInLen,
	unsigned char *pbOut,
	KTUINT *pnOutLen);
inline 
KTINT32 K_RC2_Cipher(EM_MODE mode,
				   CKey *pKey,CAlgorithmParameter *pParam,
				   const KTBYTE* pInput,KTUINT nInputLen,
				   KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<CRC2Engine,ICipherEngine> factory;		
	return K_Cipher<CRC2Engine>(&factory,mode,pKey,pParam,pInput,nInputLen,
		pOutput,pnOutputLen);
}
inline 
KTINT32 K_RC4_Cipher(EM_MODE mode,
				   CKey *pKey,CAlgorithmParameter *pParam,
				   const KTBYTE* pInput,KTUINT nInputLen,
				   KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<CRC4Engine,ICipherEngine> factory;		
	return K_Cipher<CRC4Engine>(&factory,mode,pKey,pParam,pInput,nInputLen,
		pOutput,pnOutputLen);
}

KTINT32 K_RSA_Cipher(EM_MODE mode,
				   CKey *pKey,
				   const KTBYTE* pInput,KTUINT nInputLen,
				   KTBYTE* pOutput,KTUINT* pnOutputLen);

KTINT32 K_Cipher(const KTCHAR* pAlgoName,
			   EM_MODE mode,
			   CKey *pKey,CAlgorithmParameter *pParam,
			   const KTBYTE* pInput,KTUINT nInputLen,
			   KTBYTE* pOutput,KTUINT* pnOutputLen);

KTINT32 K_KeyGenerator(const KTCHAR* pAlgoName,
					 CAlgorithmParameter *pParam,
					 CKey **ppKey);

KTINT32 K_KeyPairGenerator(const KTCHAR* pAlgoName,
					 CAlgorithmParameter *pParam,
					 CKey **ppPubKey,
					 CKey **ppPrvKey);


inline
KTINT32 K_B64_Code(EM_MODE mode,const KTBYTE* pInput,KTUINT nInputLen,
						KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<CB64Engine,ICipherEngine> factory;	
	return K_Cipher<CB64Engine>(&factory,mode,NULL,NULL,pInput,nInputLen,
		pOutput,pnOutputLen);	
}

//*******************************************************************************************
inline 
KTINT32 K_SHA1_Digest(const KTBYTE* pInput,KTUINT nInputLen,
					KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<CSHA1Engine,IMDEngine> factory;	
	return K_Digest<CSHA1Engine>(&factory,pInput,nInputLen,pOutput,pnOutputLen);
}
KTINT32 K_SHA256_Digest(const KTBYTE* pInput,KTUINT nInputLen,
					KTBYTE* pOutput,KTUINT* pnOutputLen);
inline 
KTINT32 K_MD5_Digest(const KTBYTE* pInput,KTUINT nInputLen,
					KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<CMD5Engine,IMDEngine> factory;	
	return K_Digest<CMD5Engine>(&factory,pInput,nInputLen,pOutput,pnOutputLen);
}
inline 
KTINT32 K_MD2_Digest(const KTBYTE* pInput,KTUINT nInputLen,
					KTBYTE* pOutput,KTUINT* pnOutputLen)
{
	CCipherFactory<CMD2Engine,IMDEngine> factory;	
	return K_Digest<CMD2Engine>(&factory,pInput,nInputLen,pOutput,pnOutputLen);
}

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 03:25  Hongwei Yu   created 

#endif
