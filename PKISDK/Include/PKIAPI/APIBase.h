#ifndef __API_BASE_H__
#define __API_BASE_H__

/**
  * @Name: APIBase.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 11:55
  * @See: PKIDef.h
  * @Description: 基本应用接口 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KTType.h"
#include "PKIDef.h"

#undef EXPORTS

#ifdef WIN32
	#ifndef STATIC_LIB
		#ifdef KT_DLL // assume this is defined when we build the dll
			#define EXPORTS __declspec( dllexport)
		#else
			#define EXPORTS __declspec( dllimport)
		#endif
	#else
		#define EXPORTS  
	#endif
#else
	#define EXPORTS
#endif  //WIN32

#ifdef __cplusplus
    extern "C"{
#endif
//////////////////////////////////////////////////////////////////////////
//init and slot function
//初始化
EXPORTS KTINT32
KTPKI_Initialize(
	IN void*					pReserved
);

EXPORTS KTINT32
KTPKI_Finalize(
	IN void*					pReserved
);

EXPORTS KTINT32
KTPKI_Thread_Initialize();

EXPORTS KTINT32
KTPKI_Thread_Finalize();

EXPORTS const KTChar*
KTPKI_GetErrorString(
	IN	KTINT32		rv);

EXPORTS const KTChar*
KTPKI_GetErrorString_utf8(
	IN	KTINT32		rv);

EXPORTS bool KTPKI_IsBase64(unsigned char *in, int inlen);
EXPORTS bool KTPKI_ToDer(unsigned char *in, int inlen, unsigned char *out, int *outlen);

// #ifdef KT_WIN32
EXPORTS KTINT32
KTPKI_GetSlotCount(
	OUT KTUINT*				pnSlotCount,
	IN  KTBOOL				bResetSlot
);

EXPORTS KTINT32
KTPKI_GetSlotList(
   IN KTBOOL				tokenPresent,
   OUT KTHANDLE				*phSlotList,
   IN OUT KTUINT*		pnCount
);

EXPORTS KTINT32
KTPKI_GetSlotInfo(
	IN  KTINT						nIndex,
	OUT KT_SLOT_INFO_PTR			pSlotInfo
);

EXPORTS KTINT32
KTPKI_GetTokenInfo(
	IN	KTHANDLE					nSlotID,
	OUT KT_TOKEN_INFO_PTR			pTokenInfo
);

EXPORTS KTINT32
KTPKI_GetSession(
	IN const unsigned char* id, 
	IN const int size,
	IN KTBOOL32 bCreateIfNotExist, 
	OUT KT_SESSION_HANDLE *phSession
);

//////////////////////////////////////////////////////////////////////////
//session function

EXPORTS KTINT32
KTPKI_OpenSession(
	IN KTHANDLE						nSlotID,
	OUT KT_SESSION_HANDLE_PTR		phSession
);

EXPORTS KTINT32
KTPKI_CloseSession(
	IN KT_SESSION_HANDLE	hSession
);

EXPORTS KTINT32
KTPKI_CloseSessions(
);

EXPORTS KTINT32
KTPKI_GetSessionInfo(
	IN KT_SESSION_HANDLE			hSession,
	OUT KT_SESSION_INFO				*pSessionInfo
);

EXPORTS KTINT32
KTPKI_GetTokenState(
	IN  KT_SESSION_HANDLE   hSession,
	OUT	KT_TOKEN_STATE_PTR	pnState
);

EXPORTS KTINT32
KTPKI_Login(
	IN KT_SESSION_HANDLE			hSession,		
	IN const KTCHAR*				pUsername,
	IN const KTCHAR*				pPIN,
	OUT int*						pnRemainCount = NULL
);

EXPORTS KTINT32
KTPKI_Login2(
	IN KT_SESSION_HANDLE			hSession,
	IN const KTCHAR*				pUsername,
	IN const int					nKeepTime,
	IN const KTCHAR*				pPIN,
	OUT int*						pnRemainCount = NULL
);

EXPORTS KTINT32
KTPKI_Logout(
	IN KT_SESSION_HANDLE			hSession
);

EXPORTS KTINT32
KTPKI_SetPIN(		//change user pin
	IN KT_SESSION_HANDLE			hSession,		
	IN const KTCHAR*				pOldPIN,
	IN const KTCHAR*				pNewPIN,
	OUT int*						pnRemainCount = NULL
);


//////////////////////////////////////////////////////////////////////////
//I/O function	
EXPORTS KTINT32
KTPKI_GetData(    
	IN KT_SESSION_HANDLE			hSession,		
	IN KTHANDLE						hHandle,
	OUT BYTE*					pData,
	IN OUT	KTUINT*				pnDataLen
);
EXPORTS KTINT32
KTPKI_GetDataSize(    
				  IN KT_SESSION_HANDLE			hSession,		
				  IN KTHANDLE						hHandle,
				  OUT	KTUINT*					pnDataSize
);
//获取级别
EXPORTS KTINT32
KTPKI_GetDataTrustLevel(    
				  IN KT_SESSION_HANDLE			hSession,		
				  IN KTHANDLE						hHandle,
				  OUT	KTUINT*					pnDataTrustLevel
);
//获取别名
EXPORTS KTINT32
KTPKI_GetDataAltName(    
					IN KT_SESSION_HANDLE			hSession,		
					IN KTHANDLE						hHandle,
					OUT	char*					pnDataAltName
);
//获取CA别名
EXPORTS KTINT32
KTPKI_GetDataCAAltName(    
					 IN KT_SESSION_HANDLE			hSession,		
					 IN KTHANDLE						hHandle,
					 OUT	char*					pnDataCAAltName
);

EXPORTS KTINT32
KTPKI_GetDataPropHash(    
	IN KT_SESSION_HANDLE			hSession,		
	IN KTHANDLE						hHandle,
	OUT BYTE*					pData,
	IN OUT	KTUINT*				pnDataLen
);

EXPORTS KTINT32
KTPKI_GetDataType(    
	IN KT_SESSION_HANDLE			hSession,		
	IN KTHANDLE						hHandle,
	OUT	KT_TOKEN_OBJ_TYPE*	pnObjectType
);

EXPORTS KTINT32
KTPKI_PutData(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_TOKEN_OBJ_TYPE			nType,
	IN const BYTE*					pData,
	IN KTUINT						nDataLen,
	IN const BYTE*					pPropHash,   // NULL or buffer[KT_TOKEN_OBJ_HASH_LEN]
	IN KTUINT						nPropHashLen, // 0 or KT_TOKEN_OBJ_HASH_LEN
	OUT KTHANDLE*				phHandle
);
//导入证书,增加级别和别名和CA别名
EXPORTS KTINT32
KTPKI_PutDataByTrustLevelCAAltName(
			  IN KT_SESSION_HANDLE			hSession,
			  IN KT_TOKEN_OBJ_TYPE			nType,
			  IN const BYTE*					pData,
			  IN KTUINT						nDataLen,
			  IN const BYTE*					pPropHash,   // NULL or buffer[KT_TOKEN_OBJ_HASH_LEN]
			  IN KTUINT						nPropHashLen, // 0 or KT_TOKEN_OBJ_HASH_LEN
			  IN KTUINT						nTrustLevel,//级别
			  IN char*						nAltName,//别名
			  IN char*						nCAAltName,//CA别名
			  OUT KTHANDLE*				phHandle
);


EXPORTS KTINT32
KTPKI_ListData(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_TOKEN_OBJ_TYPE			nObjectType,
	IN const BYTE*					pObjectPropData,	//default=NULL
	IN KTUINT						nObjectPropDataLen,
	OUT KTHANDLE*				phHandleList,
	IN OUT KTUINT*				pnCount
);
//通过级别、类型，获取证书句柄
EXPORTS KTINT32
KTPKI_ListDataByTrustLevel(
			   IN KT_SESSION_HANDLE			hSession,
			   IN KT_TOKEN_OBJ_TYPE			nType,
			   IN KTUINT32					nObjectTrustLevel,
			   IN const BYTE*					pObjectPropData,	//default=NULL
			   IN KTUINT						nObjectPropDataLen,
			   OUT KTHANDLE*				phHandleList,
			   IN OUT KTUINT*				pnCount
);
//通过CA别名、类型，获取证书句柄
EXPORTS KTINT32
KTPKI_ListDataByCAAltName(
						   IN KT_SESSION_HANDLE			hSession,
						   IN KT_TOKEN_OBJ_TYPE			nType,
						   IN char*						nObjectCAAltName,
						   IN const BYTE*					pObjectPropData,	//default=NULL
						   IN KTUINT						nObjectPropDataLen,
						   OUT KTHANDLE*				phHandleList,
						   IN OUT KTUINT*				pnCount
);

EXPORTS KTINT32
KTPKI_UpdateData(
	IN KT_SESSION_HANDLE			hSession,
	IN KTHANDLE						hObject,
	IN const BYTE*					pData,
	IN KTUINT						nDataLen,
	IN const BYTE*					pPropHash,   // NULL or buffer[KT_TOKEN_OBJ_HASH_LEN]
	IN KTUINT						nPropHashLen // 0 or KT_TOKEN_OBJ_HASH_LEN
);

EXPORTS KTINT32
KTPKI_RemoveData(
	IN KT_SESSION_HANDLE			hSession,
	IN KTHANDLE						hHandle
);

//////////////////////////////////////////////////////////////////////////
///crypto function
EXPORTS KTINT32
KTPKI_GenerateKey(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_CIPHER_MECHANISM			nMech,
	IN KT_MECHANISM_PARAM_PTR		pParam,
	OUT KTHANDLE					*phKey
);

EXPORTS KTINT32
KTPKI_GenerateKeybyName(
	IN KT_SESSION_HANDLE			hSession,
    IN const KTCHAR*					pAlgoName,
	IN KT_MECHANISM_PARAM_PTR		pParam,
	OUT KTHANDLE					*phKey
);

EXPORTS KTINT32
KTPKI_GenerateKeyPair(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_CIPHER_MECHANISM			nMech,
	IN KT_MECHANISM_PARAM_PTR		pParam,
	OUT KTHANDLE					*phPublicKey,
	OUT KTHANDLE					*phPrivateKey
);

EXPORTS KTINT32
KTPKI_GenerateKeyPairbyName(
	IN KT_SESSION_HANDLE			hSession,
    IN const KTCHAR*					pAlgoName,
	IN KT_MECHANISM_PARAM_PTR		pParam,
	OUT KTHANDLE					*phPublicKey,
	OUT KTHANDLE					*phPrivateKey
);

EXPORTS KTINT32
KTPKI_GenerateKeyPair2(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_CIPHER_MECHANISM			nMech,
	IN KT_MECHANISM_PARAM_PTR		pParam,
	OUT KTHANDLE					*phPublicKey,
	OUT KTHANDLE					*phPrivateKey,
	OUT BYTE*					pPubKeyBuf,
	IN OUT KTUINT32					*pnPubKeyLen,
	OUT BYTE*					pPrvKeyBuf,
	IN OUT KTUINT32					*pnPrvKeyLen
);

EXPORTS KTINT32
KTPKI_FindPrvKey(
	IN KT_SESSION_HANDLE		hSession,
	IN KT_TOKEN_OBJ_TYPE		nKeyType,			//KTOT_RSA_PRIVATE_KEY
	IN const BYTE*				pPropData,			//DER code public key's 
	IN KTUINT					nPropDataLen,
	OUT KTHANDLE*			phPrv
);

EXPORTS KTINT32
KTPKI_ImportKey(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_TOKEN_OBJ_TYPE			nKeyType,
	IN const BYTE*					pKeybuf,
	IN KTUINT						nKeyLen,
	OUT	KTHANDLE*				phKey
);

EXPORTS KTINT32
KTPKI_SetKeyExtData(
	IN KT_SESSION_HANDLE			hSession,
	IN KTHANDLE						hKey,
	IN const BYTE*					pKeyExtData,
	IN KTUINT						nKeyExtDataLen
);

EXPORTS KTINT32
KTPKI_GetKeyExtData(
	IN KT_SESSION_HANDLE			hSession,
	IN KTHANDLE						hKey,
	OUT BYTE*					pKeyExtData,
	IN OUT KTUINT*				pnKeyExtDataLen
);

//通用加密算法
//provide encrypt/decrypt,signature/verify,digest,b64code
EXPORTS KTINT32
KTPKI_Cipher(
	IN KT_SESSION_HANDLE			hSession,
	IN KT_CIPHER_MECHANISM			nMech,
	IN KT_MECHANISM_PARAM_PTR		pParam,
	IN KTHANDLE						hKey,      //0 if digest or b64code
	IN const BYTE*					pInputData,
	IN KTUINT						nInputDataLen,
	OUT BYTE*					pOutputData,
	IN OUT KTUINT*				pnOutputDataLen
);

EXPORTS KTINT32
KTPKI_CipherbyName(
	IN KT_SESSION_HANDLE			hSession,
    IN const KTCHAR*					pAlgoName,
	IN KT_MECHANISM_PARAM_PTR		pParam,
	IN KTHANDLE						hKey,      //0 if digest or b64code
	IN const BYTE*					pInputData,
	IN KTUINT						nInputDataLen,
	OUT BYTE*					pOutputData,
	IN OUT KTUINT*				pnOutputDataLen
);


EXPORTS KTINT32
KTPKI_RSASign(
	IN KT_SESSION_HANDLE		hSession,
	IN KTHANDLE					hPrvKey,
	IN KT_CIPHER_MECHANISM		nSignInfo,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen,
	OUT BYTE*				pSignedData,
	IN OUT	KTUINT*			pnSignedDataLen
);

EXPORTS KTINT32
KTPKI_RSAVerifySignature(
	IN KT_SESSION_HANDLE		hSession,		// hSession can not be NULL
	IN const BYTE*				pModulus,
	IN KTUINT					nModulusLen,
	IN const BYTE*				pPubExp,
	IN KTUINT					nPubExpLen,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen,
	IN const BYTE*				pSignedData,
	IN KTUINT					nSignedDataLen,
	OUT KTBOOL*					pbResult
);

EXPORTS KTINT32
KTPKI_RSAVerifySignatureWithCert(
	IN KT_SESSION_HANDLE		hSession,
	IN const BYTE*				pCertData,
	IN KTUINT					nCertDataLen,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen,
	IN const BYTE*				pSignedData,
	IN KTUINT					nSignedDataLen,
	OUT KTBOOL*					pbResult
);

// SM2 sign
EXPORTS KTINT32
KTPKI_SM2Sign(
	IN KT_SESSION_HANDLE		hSession,
	IN const KTCertificate&		cert,
	IN KTHANDLE					hPrvKey,
	IN KT_CIPHER_MECHANISM		nSignInfo,
	IN const KTBYTE*			pData,
	IN KTUINT					nDataLen,
	OUT KTBYTE*					pSignedData,
	IN OUT	KTUINT*				pnSignedDataLen
);

EXPORTS KTINT32
KTPKI_SM2Sign2(
	IN KT_SESSION_HANDLE		hSession,
	IN CSM2PublicKey			*pPubKey,
	IN KTHANDLE					hPrvKey,
	IN KT_CIPHER_MECHANISM		nSignInfo,
	IN const KTBYTE*			pData,
	IN KTUINT					nDataLen,
	OUT KTBYTE*					pSignedData,
	IN OUT	KTUINT*				pnSignedDataLen
);

EXPORTS KTINT32
KTPKI_SM2VerifySignature(
	IN KT_SESSION_HANDLE		hSession,		// hSession can not be NULL
	IN const KTBYTE*			pPublicKeyX,
	IN KTUINT					nPublicKeyXLen,
	IN const KTBYTE*			pPublicKeyY,
	IN KTUINT					nPublicKeyYLen,
	IN const KTBYTE*			pData,
	IN KTUINT					nDataLen,
	IN const KTBYTE*			pSignedData,
	IN KTUINT					nSignedDataLen,
	OUT KTBOOL*					pbResult
);

EXPORTS KTINT32
KTPKI_SM2VerifySignatureWithCert(
	IN KT_SESSION_HANDLE		hSession,
	IN const KTBYTE*			pCertData,
	IN KTUINT					nCertDataLen,
	IN const KTBYTE*			pData,
	IN KTUINT					nDataLen,
	IN const KTBYTE*			pSignedData,
	IN KTUINT					nSignedDataLen,
	OUT KTBOOL*					pbResult
);

/*
EXPORTS KTINT32
KTPKI_Sign(
	IN KT_SESSION_HANDLE		hSession,
	IN KTHANDLE					hPrvKey,
	IN KT_CIPHER_MECHANISM		nSignInfo,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen,
	OUT BYTE*				pSignedData,
	IN OUT	KTUINT*			pnSignedDataLen
);

EXPORTS KTINT32
KTPKI_VerifySignature(
	IN KT_SESSION_HANDLE		hSession,		// hSession can not be NULL
	IN const BYTE*				pPubKeyData,
	IN KTUINT					nPubKeyDataLen,
	IN const BYTE*				pData,
	IN KTUINT					nDataLen,
	IN const BYTE*				pSignedData,
	IN KTUINT					nSignedDataLen,
	OUT KTBOOL*					pbResult
);
*/

EXPORTS KTINT32
KTPKI_GetAlgorithmInfo(
	IN KT_SESSION_HANDLE	hSession,	// not used, can be NULL
	OUT KT_ALGORITHM_INFO	*pAlgoInfo,
	IN OUT KTUINT		*pnCount
);

EXPORTS KTINT32
KTPKI_GenerateRandom(
	IN  KT_SESSION_HANDLE	hSession,
	IN	KTINT				nRandomLen,
	OUT BYTE*			pOutput
);

EXPORTS KTINT32
KTPKI_Pem2Der(
	IN KTCHAR* pInput, 
	OUT KTBYTE* pOutput, 
	IN OUT KTUINT* punOutputLen );

enum enumPemTag
{
	PemTagCert,
	PemTagCRL,
	PemTagRSAPrivateKey,
};

EXPORTS KTINT32
KTPKI_Der2Pem(
	IN enumPemTag pemTag,
	IN KTBYTE* pInput, 
	IN KTUINT unInputLen, 
	OUT KTBYTE* pOutput, 
	IN OUT KTUINT* punOutputLen );

//////////////////////////////////////////////////////////////////////////
// 对称算法

// 导入会话密钥
// 成功后需调用KTPKI_CloseHandle关闭会话密钥句柄
EXPORTS KTINT32
KTPKI_ImportSessionKey(
	IN KT_SESSION_HANDLE	hSession,
	IN KTUINT				unAlgID,
	IN BYTE					*pbWrapedData,
	IN KTUINT				unWrapedLen);

// 设置明文对称密钥
// 成功后需调用KTPKI_CloseHandle关闭会话密钥句柄
EXPORTS KTINT32
KTPKI_SetSymmKey(
	IN KT_SESSION_HANDLE	hSession,
	IN KTUINT				unAlgID,
	IN BYTE					*pbKey,
	IN KTUINT				unKeyLen);

// 关闭会话密钥、杂凑、消息认证码句柄
EXPORTS KTINT32
KTPKI_CloseHandle(
	IN KT_SESSION_HANDLE	hSession);

// 数据加密初始化
EXPORTS KTINT32
KTPKI_EncryptInit(
	IN KT_SESSION_HANDLE	hSession,
	IN BYTE					*pbIV,
	IN KTUINT				unIVLen,
	IN KTUINT				unPaddingType,
	IN KTUINT				unFeedBitLen);

// 多个分组数据的加密操作
EXPORTS KTINT32
KTPKI_EncryptUpdate(
	IN KT_SESSION_HANDLE	hSession,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

// 结束多个分组数据的加密
EXPORTS KTINT32
KTPKI_EncryptFinal(
	IN KT_SESSION_HANDLE	hSession,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

// 数据解密初始化
EXPORTS KTINT32
KTPKI_DecryptInit(
	IN KT_SESSION_HANDLE	hSession,
	IN BYTE					*pbIV,
	IN KTUINT				unIVLen,
	IN KTUINT				unPaddingType,
	IN KTUINT				unFeedBitLen);

// 多个分组数据的解密操作
EXPORTS KTINT32
KTPKI_DecryptUpdate(
	IN KT_SESSION_HANDLE	hSession,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

// 结束多个分组数据的解密
EXPORTS KTINT32
KTPKI_DecryptFinal(
	IN KT_SESSION_HANDLE	hSession,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

// PKCS5数据填充或去填充
EXPORTS KTINT32
KTPKI_PKCS5Padding(
	IN OUT BYTE				*data,
	IN OUT KTUINT &			datalen,
	IN KTBOOL32				encrypt);

// 单组数据加解密
EXPORTS KTINT32
KTPKI_EncryptOrDecrypt(
	IN KT_SESSION_HANDLE	hSession,
	IN KTBOOL32				bEncrypt,				// 是否加密
	IN KTUINT				unAlgID,				// 对称算法标识
	IN BYTE					*pbKey,					// 密钥
	IN KTUINT				unKeyLen,				// 密钥的长度
	IN KTBOOL32				bKeyEncrypted,			// 密钥是否为密文
	IN BYTE					*pbIV,					// 初始向量
	IN KTUINT				unIVLen,				// 初始向量的长度
	IN BYTE					*pbIn,					// 输入数据
	IN KTUINT				unInLen,				// 输入数据的长度
	OUT BYTE				*pbOut,					// 输出数据
	IN OUT KTUINT			*punOutLen);			// 输出数据的长度

// #endif

#ifndef _WIN32
EXPORTS KTINT32
KTPKI_InitializeEx(
	IN void*					pReserved
);

EXPORTS KTINT32
KTPKI_FinalizeEx(
	IN void*					pReserved
);

// 导入会话密钥
// 成功后需调用KTPKI_CloseHandle关闭会话密钥句柄
EXPORTS KTINT32
KTPKI_ImportSessionKeyEx(
	IN KT_SESSION_HANDLE	hSession,
	IN KTUINT				unAlgID,
	IN BYTE					*pbWrapedData,
	IN KTUINT				unWrapedLen,
	OUT HANDLE				*phKey);

// 设置明文对称密钥
// 成功后需调用KTPKI_CloseHandle关闭会话密钥句柄
EXPORTS KTINT32
KTPKI_SetSymmKeyEx(
	IN KT_SESSION_HANDLE	hSession,
	IN KTUINT				unAlgID,
	IN BYTE					*pbKey,
	IN KTUINT				unKeyLen,
	OUT HANDLE				*phKey);

// 关闭会话密钥、杂凑、消息认证码句柄
EXPORTS KTINT32
KTPKI_CloseHandleEx(
	IN KT_SESSION_HANDLE	hSession,
	IN HANDLE				hKey);

// 数据加密初始化
EXPORTS KTINT32
KTPKI_EncryptInitEx(
	IN KT_SESSION_HANDLE	hSession,
	IN HANDLE				hKey,
	IN BYTE					*pbIV,
	IN KTUINT				unIVLen,
	IN KTUINT				unPaddingType,
	IN KTUINT				unFeedBitLen);

// 多个分组数据的加密操作
EXPORTS KTINT32
KTPKI_EncryptUpdateEx(
	IN KT_SESSION_HANDLE	hSession,
	IN HANDLE				hKey,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

// 结束多个分组数据的加密
EXPORTS KTINT32
KTPKI_EncryptFinalEx(
	IN KT_SESSION_HANDLE	hSession,
	IN HANDLE				hKey,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

// 数据解密初始化
EXPORTS KTINT32
KTPKI_DecryptInitEx(
	IN KT_SESSION_HANDLE	hSession,
	IN HANDLE				hKey,
	IN BYTE					*pbIV,
	IN KTUINT				unIVLen,
	IN KTUINT				unPaddingType,
	IN KTUINT				unFeedBitLen);

// 多个分组数据的解密操作
EXPORTS KTINT32
KTPKI_DecryptUpdateEx(
	IN KT_SESSION_HANDLE	hSession,
	IN HANDLE				hKey,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

// 结束多个分组数据的解密
EXPORTS KTINT32
KTPKI_DecryptFinalEx(
	IN KT_SESSION_HANDLE	hSession,
	IN HANDLE				hKey,
	IN BYTE					*pbIn,
	IN KTUINT				unInLen,
	OUT BYTE				*pbOut,
	IN OUT KTUINT			*punOutLen);

#endif

#ifdef __cplusplus
    }
#endif

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 11:59  Hongwei Yu   created 
///   1.0.0.1  2003/05/12 17:42  Hongwei Yu   modified KTPKI_GetErrorString declaration
///   1.0.0.2  2003/09/16 16:24  Hongwei Yu   added KTPKI_Pem2Der and KTPKI_Der2Pem

#endif
