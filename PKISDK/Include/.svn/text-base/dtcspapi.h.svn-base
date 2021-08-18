/***************************************************************************
 * 文件名称:  dtcspapi.h
 * 作者:   	  LiuQiang
 * 创建日期:  2005.09.08
 *
 * 更新: Liuyong @ 2008.9.27
 * 版本信息： dtcsp 2.03.0050
 * 
 * 文件描述：	本文件为DTCSP(客户端)提供的入口函数的声明
 *
 *
 * 版权所有：	济南得安计算机技术有限公司 (C) 1997
***************************************************************************/

#ifndef _DTCSP_API_H_
#define _DTCSP_API_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int		DTCSP_UINT32;
typedef unsigned short		DTCSP_UINT16;
typedef unsigned char		DTCSP_UCHAR;
typedef char				DTCSP_CHAR;
typedef int					DTCSP_INT32;
typedef short				DTCSP_INT16;
typedef void				DTCSP_VOID;
typedef long				DTCSP_LONG;
typedef unsigned long		DTCSP_ULONG;
typedef DTCSP_UINT32*		DTCSP_UINT32_PTR;
typedef DTCSP_UINT16*		DTCSP_UINT16_PTR;
typedef DTCSP_UCHAR*		DTCSP_UCHAR_PTR;
typedef DTCSP_CHAR*			DTCSP_CHAR_PTR;
typedef DTCSP_INT32*		DTCSP_INT32_PTR;
typedef DTCSP_INT16*		DTCSP_INT16_PTR;
typedef DTCSP_VOID*			DTCSP_VOID_PTR;
typedef DTCSP_INT32			DTCSP_HANDLE;
typedef DTCSP_LONG *		DTCSP_LONG_PTR;
typedef DTCSP_ULONG *		DTCSP_ULONG_PTR;

#ifdef _DTCSP_WINDOWS_
typedef __int64         DTCSP_INT64;
#else
typedef long long       DTCSP_INT64;
#endif

//	RSA modulus
#define MAX_RSA_MODULUS_LEN		256		//2048 bits
#define MAX_RSA_PRIME_LEN		128
#define  MAX_ECC_PRIME_LEN		32	//256bits add by ChaiQing 2008.07.15
typedef struct
{
    unsigned int  bits;                 		/* length in bits of modulus */
    unsigned char modulus[MAX_RSA_MODULUS_LEN];  	/* modulus */
    unsigned char exponent[MAX_RSA_MODULUS_LEN]; 	/* public exponent */
} DTCSP_RSA_PUBLIC_KEY;

typedef struct
{
    unsigned int  bits;                           	/* length in bits of modulus */
    unsigned char modulus[MAX_RSA_MODULUS_LEN];       	/* n */
    unsigned char publicExponent[MAX_RSA_MODULUS_LEN];	/* e */
    unsigned char exponent[MAX_RSA_MODULUS_LEN];      	/* d */
    unsigned char prime[2][MAX_RSA_PRIME_LEN];        	/* p,q */
    unsigned char primeExponent[2][MAX_RSA_PRIME_LEN];	/* dp,dq */
    unsigned char coefficient[MAX_RSA_PRIME_LEN];      	/* qInv */
} DTCSP_RSA_PRIVATE_KEY;

typedef struct
{
    unsigned int  bits;                 		/* length in bits of modulus */
    unsigned char modulus[MAX_RSA_MODULUS_LEN];  	/* modulus */
    unsigned char publicExponent[MAX_RSA_MODULUS_LEN]; 	/* public exponent */
    unsigned char cipherPrivateKey[MAX_RSA_PRIME_LEN*7];
} DTCSP_RSA_CIPHER_PRIVATE_KEY;

typedef struct{
    DTCSP_CHAR DesKey[8];
    DTCSP_CHAR Iv[8];
    DTCSP_INT32 Flag;
}DTCSP_DES_CBC_CONTEXT,*DTCSP_DES_CBC_CONTEXT_PTR;

typedef struct{
    DTCSP_CHAR DesKey[24];
    DTCSP_CHAR Iv[8];
    DTCSP_INT32 Flag;
}DTCSP_3DES_CBC_CONTEXT,*DTCSP_3DES_CBC_CONTEXT_PTR;

typedef struct{
    DTCSP_CHAR AesKey[16];
    DTCSP_CHAR Iv[16];
    DTCSP_INT32 Flag;
}DTCSP_AES_CBC_CONTEXT,*DTCSP_AES_CBC_CONTEXT_PTR;

typedef struct{
    DTCSP_CHAR Key[16];
    DTCSP_CHAR Iv[16];
    DTCSP_INT32 Flag;
}DTCSP_SSF33_CBC_CONTEXT,*DTCSP_SSF33_CBC_CONTEXT_PTR;

typedef struct{
    DTCSP_UCHAR  SCB2Key[32];
    DTCSP_UINT16 InBuf[8096];
    DTCSP_INT32 KeyLen;
    DTCSP_INT32 Flag;
}DTCSP_SCB2_ECB_CONTEXT,*DTCSP_SCB2_ECB_CONTEXT_PTR;

typedef struct{
    DTCSP_UCHAR		SCB2Key[32];
    DTCSP_UCHAR		Iv[16];
    DTCSP_UINT16	InBuf[8096];
    DTCSP_INT32		KeyLen;
    DTCSP_INT32		Flag;
}DTCSP_SCB2_CBC_CONTEXT,*DTCSP_SCB2_CBC_CONTEXT_PTR;


typedef struct _item_
{
    int		popNum;
    float	prob;
}ITEM;


typedef struct _in_item_
{
    int popNum;
    unsigned long intProb;
    int isPop;
    unsigned long maxProb;
    unsigned long minProb;
    struct _in_item_ *pNext;
}IN_ITEM;

typedef struct{
    DTCSP_UCHAR		InBuf[64];
    DTCSP_INT32		nInBufLen;
    DTCSP_INT64     nMessageLen;
    DTCSP_INT32		InitFlag;
    DTCSP_INT32	    CardIndex;
} DTCSP_SHA_CONTEXT,*DTCSP_SHA_CONTEXT_PTR;

typedef struct{
    DTCSP_UCHAR		InBuf[64];
    DTCSP_INT32		nInBufLen;
    DTCSP_INT32		InitFlag;
    DTCSP_INT32		Flag;
    DTCSP_UCHAR		pID[64];
    DTCSP_INT32     nIDLen;
    DTCSP_INT32		CardIndex;
} DTCSP_SCH_CONTEXT,*DTCSP_SCH_CONTEXT_PTR;

typedef struct{
    DTCSP_UCHAR		primep[MAX_ECC_PRIME_LEN];//素数p
    DTCSP_UCHAR		a[MAX_ECC_PRIME_LEN];  //参数a
    DTCSP_UCHAR		b[MAX_ECC_PRIME_LEN];  //参数b
    DTCSP_UCHAR		gx[MAX_ECC_PRIME_LEN];  //参数Gx	/* x coordinate of the base poDTCSP_INT32 G */
    DTCSP_UCHAR		gy[MAX_ECC_PRIME_LEN];  //参数Gy	/* y coordinate of the base poDTCSP_INT32 G */
    DTCSP_UCHAR		n[MAX_ECC_PRIME_LEN];	//阶N	/* order n of the base poDTCSP_INT32 G */
    DTCSP_INT16		len;//参数位长Len，Len必须为160、192、224或256
    DTCSP_INT16		type;	/*对应芯片手册曲线类型,开始时为0*/
} DTCSP_ECC_CURVE, *DTCSP_ECC_CURVE_PTR;

// ECC公钥结构体
typedef struct{
    DTCSP_ECC_CURVE curve; /* ECC curve */	//外部曲线为用户指定的，其它要对len赋值
    DTCSP_UCHAR		qx[MAX_ECC_PRIME_LEN];	/* x coordinate of the poDTCSP_INT32 Q */
    DTCSP_UCHAR		qy[MAX_ECC_PRIME_LEN];	/* y coordinate of the poDTCSP_INT32 Q */
} DTCSP_ECC_PUBLIC_KEY, *DTCSP_ECC_PUBLIC_KEY_PTR;

// ECC私钥结构体
typedef struct{
    DTCSP_ECC_CURVE curve; /* ECC curve */  //外部曲线为用户指定的，其它要对len赋值
    DTCSP_UCHAR		qx[MAX_ECC_PRIME_LEN]; /* x coordinate of the poDTCSP_INT32 Q */
    DTCSP_UCHAR		qy[MAX_ECC_PRIME_LEN]; /* y coordinate of the poDTCSP_INT32 Q */
    DTCSP_UCHAR		d[MAX_ECC_PRIME_LEN];      /* d */
}DTCSP_ECC_PRIVATE_KEY, *DTCSP_ECC_PRIVATE_KEY_PTR;

typedef struct{
    DTCSP_ECC_CURVE	curve;
    DTCSP_UCHAR		EccKey[96];	//EccKey只需要包含qx\qy和d即可
    DTCSP_UCHAR		EccKeyMessage[32];//ch
}DTCSP_ECC_BKEY,*DTCSP_ECC_BKEY_PTR;

// 签名验证信息结构体
typedef  struct {
    DTCSP_UCHAR		Rdata[32];
    DTCSP_UCHAR		Sdata[32];
}DTCSP_ECC_SIG,*DTCSP_ECC_SIG_PTR;

// SCE加解密密文结构体
typedef  struct  {
    DTCSP_INT16		nC2Len;  //涉及c2
    DTCSP_UCHAR		c1[64];
    DTCSP_UCHAR		c2[136];
    DTCSP_UCHAR		c3[32];
}DTCSP_ECC_CIPHER,*DTCSP_ECC_CIPHER_PTR;



//tsa
int PrintHex(char *itemName,
	unsigned char *sourceData,
	unsigned int dataLength,
	unsigned int rowCount);

DTCSP_INT32 STF_CreateTSRequest(
	DTCSP_VOID_PTR	pContext,
	DTCSP_UCHAR *	pSubject,
	DTCSP_INT32		nSubjectLen,
	DTCSP_UCHAR_PTR	pIndata,
	DTCSP_INT32		nIndataLen,
	DTCSP_INT32		nReqtype,
	DTCSP_INT32		nHashId,
	DTCSP_UCHAR_PTR	pOutData,
	DTCSP_INT32_PTR	pOutDataLen
);

DTCSP_INT32 Analyze_errorcode(DTCSP_INT32 errorcode, DTCSP_UCHAR *errorString);



DTCSP_INT32 STF_CreateTSResponse(
	DTCSP_VOID_PTR	pContext,
	DTCSP_UCHAR_PTR	pSubject,
	DTCSP_INT32		nSubjectLen,
	DTCSP_UCHAR_PTR	pIndata,
	DTCSP_INT32		nIndataLen,
	DTCSP_INT32		nSignatureAlgID,
	DTCSP_UCHAR_PTR	pOutData,
	DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 STF_VerifyTSValidity(
	DTCSP_VOID_PTR	pContext,
	DTCSP_UCHAR_PTR	pucTSResponse,
	DTCSP_INT32		uiTSResponseLength,
	DTCSP_INT32		nHashId,
	DTCSP_INT32		nSignatureAlgID,
	DTCSP_UCHAR_PTR	cert,
	DTCSP_INT32		certlen);

DTCSP_INT32 STF_GetTSInfo(
	DTCSP_VOID_PTR	pContext,
	DTCSP_UCHAR_PTR	pucTSResponse,
	DTCSP_INT32		uiTSResponseLength,
	DTCSP_UCHAR_PTR pucIssuerName,
	DTCSP_INT32_PTR puiIssuerNameLength,
	DTCSP_UCHAR_PTR pucTime,
	DTCSP_UINT32_PTR puiTimeLength
	);

DTCSP_INT32 STF_GetTSDetail(
	DTCSP_VOID_PTR	pContext,
	DTCSP_UCHAR_PTR	pucTSResponse,
	DTCSP_INT32		uiTSResponseLength,
	DTCSP_INT32 item,
	DTCSP_UCHAR_PTR pucItemValue,
	DTCSP_UINT32_PTR puiItemValueLength
	);
DTCSP_INT32	DTCSP_Init(
    DTCSP_VOID_PTR* pContext,
    DTCSP_CHAR_PTR 	pConfigureFileName,
    DTCSP_UCHAR_PTR pPassword);

DTCSP_INT32	 DTCSP_InitEx(
	DTCSP_VOID_PTR* pContext,
	DTCSP_UCHAR_PTR Password,
	DTCSP_CHAR_PTR 	pIp,
	DTCSP_INT32    nPort);

DTCSP_INT32 DTCSP_End(DTCSP_VOID_PTR *pContext);

DTCSP_INT32 DTCSP_GetDTCSPVersion(DTCSP_UCHAR_PTR pVersion);

DTCSP_INT32 DTCSP_InitFlash(DTCSP_VOID_PTR pContext);

DTCSP_INT32 DTCSP_DisableAllInit(DTCSP_VOID_PTR pContext);

DTCSP_INT32 DTCSP_GetCardVersion(
    DTCSP_VOID_PTR	pContext,
    DTCSP_UCHAR_PTR	pCardVersion);

DTCSP_INT32 DTCSP_GetCardRunStatus(
    DTCSP_VOID_PTR		pContext,
    DTCSP_UCHAR_PTR		pRunStatus);

DTCSP_INT32 DTCSP_GetCardRunStatusEx(
    DTCSP_VOID_PTR		pContext,
    DTCSP_UCHAR_PTR		pRunStatus);

DTCSP_INT32 DTCSP_GenerateTrueRandData(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nRandomDataLen,
    DTCSP_UCHAR_PTR	pRandomData);

DTCSP_INT32 DTCSP_GenerateSpecifyTrueRandNumber(
    DTCSP_VOID_PTR pContext,
    DTCSP_INT32 nMinNumber,
    DTCSP_INT32 nMaxNumber,
    DTCSP_INT32_PTR pRandomNumber
);

DTCSP_INT32 DTCSP_InitManagerBegin(
    DTCSP_VOID_PTR 	pContext,
    DTCSP_INT32  	nManagerCount);

DTCSP_INT32 DTCSP_InitManagerPassword(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nManagerNumber);

DTCSP_INT32 DTCSP_InitManagerEnd(DTCSP_VOID_PTR pContext);

DTCSP_INT32 DTCSP_GetManagerCount(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32_PTR  pManagerCount);

DTCSP_INT32 DTCSP_AddOneManager(
    DTCSP_VOID_PTR		pContext,
    DTCSP_INT32			nManagerNumber);

DTCSP_INT32 DTCSP_DelOneManager(
    DTCSP_VOID_PTR  pContext,
    DTCSP_INT32		nManagerNo);

DTCSP_INT32 DTCSP_ManagerLogin(
    DTCSP_VOID_PTR  	pContext,
    DTCSP_INT32_PTR  	pManagerNumber);

DTCSP_INT32 DTCSP_ManagerLogout(DTCSP_VOID_PTR pContext);

DTCSP_INT32 DTCSP_ChangeManagerPassword(
    DTCSP_VOID_PTR 	pContext,
    DTCSP_INT32_PTR	pManagerNumber);

DTCSP_INT32 DTCSP_InitOperatorPassword(DTCSP_VOID_PTR  pContext);

DTCSP_INT32 DTCSP_AddOneOperator(DTCSP_VOID_PTR  pContext);

DTCSP_INT32 DTCSP_OperatorLogin(DTCSP_VOID_PTR  pContext);

DTCSP_INT32 DTCSP_OperatorLogout(DTCSP_VOID_PTR  pContext);

DTCSP_INT32 DTCSP_InitICCard(DTCSP_VOID_PTR  pContext);

DTCSP_INT32 DTCSP_BackupCardInfoBegin(DTCSP_VOID_PTR pContext);

DTCSP_INT32 DTCSP_BackupCardInfoExportSecretKeyPart(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nICCardNumber);

DTCSP_INT32 DTCSP_BackupCardInfoEnd(
    DTCSP_VOID_PTR	pContext,
    DTCSP_CHAR_PTR	pBackupFileName);

DTCSP_INT32 DTCSP_RestoreCardInfoBegin(DTCSP_VOID_PTR	pContext);

DTCSP_INT32 DTCSP_RestoreCardInfoImportSecretKeyPart(
    DTCSP_VOID_PTR	pContext);

DTCSP_INT32 DTCSP_RestoreCardInfoEnd(
    DTCSP_VOID_PTR	pContext,
    DTCSP_CHAR_PTR	pRestoreFileName);

DTCSP_INT32 DTCSP_SetSystemMasterKeyBegin(DTCSP_VOID_PTR pContext);

DTCSP_INT32 DTCSP_SetSystemMasterKeyEnd(DTCSP_VOID_PTR pContext);

DTCSP_INT32 DTCSP_SetDeviceMasterKey(
    DTCSP_VOID_PTR	pContext,
    DTCSP_CHAR_PTR	pDeviceMasterKey,
    DTCSP_INT32		nDeviceMasterKeyLen);

DTCSP_INT32 DTCSP_EncMsgSystemMasterKeyCur(
    DTCSP_VOID_PTR	pContext,
    DTCSP_UCHAR_PTR	pIndata,
    DTCSP_INT32		nIndataLen,
    DTCSP_UCHAR_PTR	pOutdata,
    DTCSP_INT32_PTR	pOutdataLen);

DTCSP_INT32 DTCSP_DecMsgSystemMasterKeyCur(
    DTCSP_VOID_PTR	pContext,
    DTCSP_UCHAR_PTR	pIndata,
    DTCSP_INT32		nIndataLen,
    DTCSP_UCHAR_PTR	pOutdata,
    DTCSP_INT32_PTR	pOutdataLen);

DTCSP_INT32 DTCSP_EncMsgSystemMasterKeyNew (
    DTCSP_VOID_PTR	pContext,
    DTCSP_UCHAR_PTR	pIndata,
    DTCSP_INT32		nIndataLen,
    DTCSP_UCHAR_PTR	pOutdata,
    DTCSP_INT32_PTR	pOutdataLen);

DTCSP_INT32 DTCSP_DecMsgSystemMasterKeyNew (
    DTCSP_VOID_PTR	pContext,
    DTCSP_UCHAR_PTR	pIndata,
    DTCSP_INT32		nIndataLen,
    DTCSP_UCHAR_PTR	pOutdata,
    DTCSP_INT32_PTR	pOutdataLen);

DTCSP_INT32 DTCSP_EncMsgSystemMasterKeyOld (
    DTCSP_VOID_PTR	pContext,
    DTCSP_UCHAR_PTR	pIndata,
    DTCSP_INT32		nIndataLen,
    DTCSP_UCHAR_PTR	pOutdata,
    DTCSP_INT32_PTR	pOutdataLen);

DTCSP_INT32 DTCSP_DecMsgSystemMasterKeyOld (
    DTCSP_VOID_PTR	pContext,
    DTCSP_UCHAR_PTR	pIndata,
    DTCSP_INT32		nIndataLen,
    DTCSP_UCHAR_PTR	pOutdata,
    DTCSP_INT32_PTR	pOutdataLen);

DTCSP_INT32 DTCSP_SSF33Encrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		bKeyChangeFlag,
    DTCSP_UCHAR_PTR	pKey33,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SSF33Decrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		bKeyChangeFlag,
    DTCSP_UCHAR_PTR	pKey33,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SSF33EncryptStd(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		bKeyChangeFlag,
    DTCSP_UCHAR_PTR	pKey33,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SSF33DecryptStd(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		bKeyChangeFlag,
    DTCSP_UCHAR_PTR	pKey33,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_GenerateRSAKeyPair(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_INT32		nModulusLen,
    DTCSP_UCHAR_PTR	pPublicExponent,
    DTCSP_INT32		nPublicExponentLen,
    DTCSP_RSA_PUBLIC_KEY *	pPublicKey,
    DTCSP_RSA_PRIVATE_KEY *	pPrivateKey);

DTCSP_INT32 DTCSP_GenerateCRSAKeyPair(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_INT32		nModulusLen,
    DTCSP_UCHAR_PTR	pPublicExponent,
    DTCSP_INT32		nPublicExponentLen,
    DTCSP_RSA_PUBLIC_KEY *	pPublicKey,
    DTCSP_RSA_CIPHER_PRIVATE_KEY *	pCipherPrivateKey);

DTCSP_INT32	DTCSP_RSAPublicRaw(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PUBLIC_KEY*	pPublicKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPrivateRaw(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PRIVATE_KEY*	pPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_CRSAPrivateRaw(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_CIPHER_PRIVATE_KEY*	pCipherPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPublicKeyPkcs1Encrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PUBLIC_KEY* pPublicKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPublicKeyPkcs1Decrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PUBLIC_KEY*	pPublicKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPrivateKeyPkcs1Encrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PRIVATE_KEY*	pPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPrivateKeyPkcs1Decrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PRIVATE_KEY*	pPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_CRSAPrivateKeyPkcs1Encrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_CIPHER_PRIVATE_KEY*	pCipherPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32	DTCSP_CRSAPrivateKeyPkcs1Decrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_CIPHER_PRIVATE_KEY*	pCipherPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32	DTCSP_GetRSAPublicKey(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PUBLIC_KEY	*pPublicKey);

DTCSP_INT32 DTCSP_CopyRSAKeyPair(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nSrcKeyNumber,
    DTCSP_INT32		nDstKeyNumber);

DTCSP_INT32 DTCSP_DelRSAKeyPair(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber);

DTCSP_INT32 DTCSP_PutRSAKeyPair(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nDstKeyNumber,
    DTCSP_RSA_PUBLIC_KEY	PublicKey,
    DTCSP_RSA_PRIVATE_KEY	PrivateKey);

DTCSP_INT32	DTCSP_RSAPublicKeyEncrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PUBLIC_KEY	PublicKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPublicKeyDecrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PUBLIC_KEY	PublicKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPrivateKeyEncrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PRIVATE_KEY	PrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_RSAPrivateKeyDecrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_PRIVATE_KEY	PrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32 *	pOutDataLen);

DTCSP_INT32	DTCSP_CRSAPrivateKeyEncrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_CIPHER_PRIVATE_KEY	CipherPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32	DTCSP_CRSAPrivateKeyDecrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_RSA_CIPHER_PRIVATE_KEY	CipherPrivateKey,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_UserWriteFlash(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nFlashOffsetLong,
    DTCSP_INT32_PTR	pIndata,
    DTCSP_INT32		nIndataLenLong);

DTCSP_INT32 DTCSP_UserReadFlash(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nFlashOffsetLong,
    DTCSP_INT32_PTR	pOutdata,
    DTCSP_INT32		nOutdataLenLong);

DTCSP_INT32 DTCSP_PutDESKey(
    DTCSP_VOID_PTR    pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pDesKey,
    DTCSP_INT32            nDesKeyLen);

DTCSP_INT32 DTCSP_GetDESKey(
    DTCSP_VOID_PTR         pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pDesKey,
    DTCSP_INT32_PTR     pDesKeyLen);

DTCSP_INT32 DTCSP_DESEncrypt(
    DTCSP_VOID_PTR         pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pDesKey,
    DTCSP_INT32    nDesKeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32            nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR           pOutDataLen);

DTCSP_INT32 DTCSP_DESDecrypt(
    DTCSP_VOID_PTR         pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pDesKey,
    DTCSP_INT32            nDesKeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32            nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR           pOutDataLen);

DTCSP_INT32 DTCSP_3DESEncrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pDesKey,
    DTCSP_INT32  nDesKeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32  nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR pOutDataLen);

DTCSP_INT32 DTCSP_3DESDecrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pDesKey,
    DTCSP_INT32  nDesKeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32  nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR pOutDataLen);

DTCSP_INT32 DTCSP_SSF33CBCInit(
    DTCSP_VOID_PTR   pContext,
    DTCSP_SSF33_CBC_CONTEXT_PTR pSSF33Context,
    DTCSP_UCHAR_PTR  pIv,
    DTCSP_UCHAR_PTR  pKey,
    DTCSP_INT32      nKeyLen,
    DTCSP_INT32      nEncryptFlag);

DTCSP_INT32 DTCSP_SSF33CBCUpdate(
    DTCSP_VOID_PTR   pContext,
    DTCSP_SSF33_CBC_CONTEXT_PTR pSSF33Context,
    DTCSP_UCHAR_PTR  pInData,
    DTCSP_INT32      nInLen,
    DTCSP_UCHAR_PTR  pOutData,
    DTCSP_INT32_PTR  pnOutLen);

DTCSP_INT32 DTCSP_SSF33CBCFinalize(
    DTCSP_VOID_PTR   pContext,
    DTCSP_SSF33_CBC_CONTEXT_PTR pSSF33Context);

DTCSP_UINT32 DTCSP_DESCBCInit(
    DTCSP_VOID_PTR   pContext,
    DTCSP_DES_CBC_CONTEXT_PTR pDesContext,
    DTCSP_UCHAR_PTR  pIv,
    DTCSP_UCHAR_PTR  pKey,
    DTCSP_INT32      nKeyLen,
    DTCSP_INT32      nEncryptFlag);

DTCSP_INT32 DTCSP_DESCBCUpdate(
    DTCSP_VOID_PTR   pContext,
    DTCSP_DES_CBC_CONTEXT_PTR pDesContext,
    DTCSP_UCHAR_PTR  pInData,
    DTCSP_INT32      nInLen,
    DTCSP_UCHAR_PTR  pOutData,
    DTCSP_INT32_PTR  pnOutLen);

DTCSP_INT32 DTCSP_DESCBCFinalize(
    DTCSP_VOID_PTR   pContext,
    DTCSP_DES_CBC_CONTEXT_PTR pDesContext);

DTCSP_INT32 DTCSP_3DESCBCInit(
    DTCSP_VOID_PTR   pContext,
    DTCSP_3DES_CBC_CONTEXT_PTR p3DesContext,
    DTCSP_UCHAR_PTR  pIv,
    DTCSP_UCHAR_PTR  pKey,
    DTCSP_INT32      nKeyLen,
    DTCSP_INT32      nEncryptFlag);

DTCSP_INT32 DTCSP_3DESCBCUpdate(
    DTCSP_VOID_PTR   pContext,
    DTCSP_3DES_CBC_CONTEXT_PTR p3DesContext,
    DTCSP_UCHAR_PTR  pInData,
    DTCSP_INT32      nInLen,
    DTCSP_UCHAR_PTR  pOutData,
    DTCSP_INT32_PTR  pnOutLen);

DTCSP_INT32 DTCSP_3DESCBCFinalize(
    DTCSP_VOID_PTR   pContext,
    DTCSP_3DES_CBC_CONTEXT_PTR p3DesContext);

DTCSP_INT32 DTCSP_AESEncrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pAesKey,
    DTCSP_INT32  nAesKeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32  nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR pOutDataLen);

DTCSP_INT32 DTCSP_AESDecrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pAesKey,
    DTCSP_INT32  nAesKeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32  nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR pOutDataLen);

DTCSP_INT32 DTCSP_AESCBCInit( DTCSP_VOID_PTR   pContext,
    DTCSP_AES_CBC_CONTEXT_PTR pAesContext,
    DTCSP_UCHAR_PTR  pIv,
    DTCSP_UCHAR_PTR  pKey,
    DTCSP_INT32      nKeyLen,
    DTCSP_INT32      nEncryptFlag);
DTCSP_INT32 DTCSP_AESCBCUpdate(
    DTCSP_VOID_PTR   pContext,
    DTCSP_AES_CBC_CONTEXT_PTR pAesContext,
    DTCSP_UCHAR_PTR  pInData,
    DTCSP_INT32      nInLen,
    DTCSP_UCHAR_PTR  pOutData,
    DTCSP_INT32_PTR  pnOutLen);

DTCSP_INT32 DTCSP_AESCBCFinalize(
    DTCSP_VOID_PTR   pContext,
    DTCSP_AES_CBC_CONTEXT_PTR pAesContext);

DTCSP_INT32 DTCSP_GenerateRSAKeyPairByName(
    void *pContext,
    char * sKeyName,
    unsigned int nModulusLen,
    unsigned char *pPublicExponent,
    unsigned int nPublicExponentLen,
    DTCSP_RSA_PUBLIC_KEY *pPublicKey,
    DTCSP_RSA_PRIVATE_KEY *pPrivateKey);

DTCSP_INT32 DTCSP_DelRSAKeyPairByName(
    void *pContext,
    char *sKeyName);


DTCSP_INT32 DTCSP_PutRSAKeyPairByName(
    void *pContext,
    char *sKeyName,
    DTCSP_RSA_PUBLIC_KEY *PublicKey,
    DTCSP_RSA_PRIVATE_KEY *PrivateKey);

DTCSP_INT32 DTCSP_GetRSAPublicKeyByName(
    void *pContext,
    char *sKeyName,
    DTCSP_RSA_PUBLIC_KEY *pPublicKey);

DTCSP_INT32 DTCSP_RSAPrivateRawByName(
    void *pContext,
    char *sKeyName,
    DTCSP_RSA_PRIVATE_KEY*	pPrivateKey,
    unsigned char *pInData,
    unsigned int nInDataLen,
    unsigned char *pOutData,
    unsigned int *pnOutDataLen);

DTCSP_INT32 DTCSP_Hmac_MD5(
    DTCSP_VOID_PTR      pContext,
    DTCSP_UCHAR_PTR pMD5Key,
    DTCSP_INT32          nMD5KeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32           nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR  pOutDataLen);

DTCSP_INT32 DTCSP_Hmac_Sha1(
    DTCSP_VOID_PTR    pContext,
    DTCSP_UCHAR_PTR  pSha1Key,
    DTCSP_INT32            nSha1KeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_INT32            nInDataLen,
    DTCSP_UCHAR_PTR pOutData,
    DTCSP_INT32_PTR    pOutDataLen);
DTCSP_INT32 DTCSP_GetICCardState(
    DTCSP_VOID_PTR pContext,
    DTCSP_UINT32_PTR  pnICCardState,
    DTCSP_UINT32_PTR  pnICCardID);


DTCSP_INT32 DTCSP_SCB2ECBEncrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SCB2ECBDecrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SCB2CBCEncrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR pIv,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SCB2CBCDecrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR pIv,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_TC_Pop(
    DTCSP_VOID_PTR pContext,
    ITEM *pInList,
    DTCSP_INT32 pListNum,
    DTCSP_INT32 *pOutList);


DTCSP_INT32 DTCSP_GenerateMKEncKey(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	KeyEncwMK);

DTCSP_INT32 DTCSP_GenerateUKEncKey (
    DTCSP_VOID_PTR pContext,
    DTCSP_INT32 nRkEncwMKLen,
    DTCSP_UCHAR_PTR RkEncwMK,
    DTCSP_INT32 nKeyLen,
    DTCSP_UCHAR_PTR KeyEncwMK);

DTCSP_INT32 DTCSP_GenerateUKAndMKEncKey (
    DTCSP_VOID_PTR pContext,
    DTCSP_INT32 nKekAEncwMKLen,
    DTCSP_UCHAR_PTR KekAEncwMK,
    DTCSP_INT32 nKekBEncwMKLen,
    DTCSP_UCHAR_PTR KekBEncwMK,
    DTCSP_INT32 nKeyLen,
    DTCSP_UCHAR_PTR TekEncwKA,
    DTCSP_UCHAR_PTR TekEncwKB,
    DTCSP_UCHAR_PTR TekEncwMK);

DTCSP_INT32 DTCSP_GenUKEncRandom(
    DTCSP_VOID_PTR pContext,
    DTCSP_INT32 nKekEncwMKLen,
    DTCSP_UCHAR_PTR KekEncwMK,
    DTCSP_INT32 nRandomDataLen,
    DTCSP_UCHAR_PTR RanData);

DTCSP_INT32 DTCSP_SCB2Encrypt(
    DTCSP_VOID_PTR pContext,
    DTCSP_CHAR_PTR pKeyEncwMK,
    DTCSP_INT32 nKeyEncwMKLen,
    DTCSP_CHAR_PTR pInData,
    DTCSP_INT32 nInDataLen,
    DTCSP_CHAR_PTR  pOutData,
    DTCSP_INT32 * pOutDataLen);

DTCSP_INT32 DTCSP_SCB2Decrypt(
    DTCSP_VOID_PTR pContext,
    DTCSP_CHAR_PTR pKeyEncwMK,
    DTCSP_INT32 nKeyEncwMKLen,
    DTCSP_CHAR_PTR pInData,
    DTCSP_INT32 nInDataLen,
    DTCSP_CHAR_PTR  pOutData,
    DTCSP_INT32_PTR pOutDataLen);



//以字节为单位用户读写Flash的接口,用户Flash区可存放112K字节
DTCSP_INT32 DTCSP_UserWriteFlashByChar(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nOffset,
    DTCSP_UCHAR_PTR	pIndata,
    DTCSP_INT32		nIndataLen);
DTCSP_INT32 DTCSP_UserReadFlashByChar(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nOffset,
    DTCSP_UCHAR_PTR	pOutdata,
    DTCSP_INT32		nOutdataLen);

DTCSP_INT32 DTCSP_GetKeyStatus(
    DTCSP_VOID_PTR		pContext,
    DTCSP_INT32         nKeyType,
    DTCSP_INT32         nKeyNum,
    DTCSP_INT32_PTR     pKeyTag);

DTCSP_INT32 DTCSP_Mac_3Des(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32            nKeyNum,
    DTCSP_UCHAR_PTR pIv,
    DTCSP_UINT32   nIvLen,
    DTCSP_UCHAR_PTR pDesKey,
    DTCSP_UINT32  nDesKeyLen,
    DTCSP_UCHAR_PTR pInData,
    DTCSP_UINT32  nInDataLen,
    DTCSP_UCHAR_PTR pMacData,
    DTCSP_UINT32_PTR  pMacDataLen);

//add by lfj_jack 2008.3.25
DTCSP_INT32 DTCSP_SessionKeyEncrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32          nAlgorithmType,
    DTCSP_INT32          nKeyNum,
    DTCSP_UCHAR_PTR      pIv,
    DTCSP_UCHAR_PTR      pSessionKey,
    DTCSP_UINT32         nSessionKeyLen,
    DTCSP_UCHAR_PTR      pInData,
    DTCSP_INT32         nInDataLen,
    DTCSP_UCHAR_PTR      pOutData,
    DTCSP_INT32_PTR     nOutDataLen);

DTCSP_INT32 DTCSP_SessionKeyDecrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32          nAlgorithmType,
    DTCSP_INT32          nKeyNum,
    DTCSP_UCHAR_PTR      pIv,
    DTCSP_UCHAR_PTR      pSessionKey,
    DTCSP_UINT32         nSessionKeyLen,
    DTCSP_UCHAR_PTR      pInData,
    DTCSP_UINT32         nInDataLen,
    DTCSP_UCHAR_PTR      pOutData,
    DTCSP_UINT32_PTR     nOutDataLen);

DTCSP_INT32 DTCSP_GenMacUseMasterKey(
    DTCSP_VOID_PTR  pContext,
    DTCSP_UCHAR_PTR Plain,
    DTCSP_INT32     PlainLen,
    DTCSP_UCHAR_PTR Cipher,
    DTCSP_UINT32_PTR CipherLen);

DTCSP_INT32 DTCSP_SDBIECBEncrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32          nKeyNum,
    DTCSP_UCHAR_PTR      pSessionKey,
    DTCSP_UINT32         nSessionKeyLen,
    DTCSP_UCHAR_PTR      pInData,
    DTCSP_UINT32         nInDataLen,
    DTCSP_UCHAR_PTR      pOutData,
    DTCSP_UINT32_PTR     nOutDataLen);

DTCSP_INT32 DTCSP_SDBIECBDecrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32          nKeyNum,
    DTCSP_UCHAR_PTR      pSessionKey,
    DTCSP_UINT32         nSessionKeyLen,
    DTCSP_UCHAR_PTR      pInData,
    DTCSP_UINT32         nInDataLen,
    DTCSP_UCHAR_PTR      pOutData,
    DTCSP_UINT32_PTR     nOutDataLen);

DTCSP_INT32 DTCSP_SDBICBCEncrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32          nKeyNum,
    DTCSP_UCHAR_PTR      pSessionKey,
    DTCSP_UINT32         nSessionKeyLen,
    DTCSP_UCHAR_PTR      pIv,
    DTCSP_UCHAR_PTR      pInData,
    DTCSP_UINT32         nInDataLen,
    DTCSP_UCHAR_PTR      pOutData,
    DTCSP_UINT32_PTR     nOutDataLen);

DTCSP_INT32 DTCSP_SDBICBCDecrypt(
    DTCSP_VOID_PTR       pContext,
    DTCSP_INT32          nKeyNum,
    DTCSP_UCHAR_PTR      pSessionKey,
    DTCSP_UINT32         nSessionKeyLen,
    DTCSP_UCHAR_PTR      pIv,
    DTCSP_UCHAR_PTR      pInData,
    DTCSP_UINT32         nInDataLen,
    DTCSP_UCHAR_PTR      pOutData,
    DTCSP_UINT32_PTR     nOutDataLen);

DTCSP_INT32	DTCSP_ECC_Initialize(
	 DTCSP_VOID_PTR	pContext,
	 DTCSP_INT32         nEccCurveFlag,
	 DTCSP_ECC_CURVE_PTR   pEccCurve);

DTCSP_INT32	DTCSP_ECC_GetCurveFlag(
   DTCSP_VOID_PTR	pContext,
   DTCSP_INT32         nEccKeyNum,
   DTCSP_INT32_PTR   pEccCurveFlag);

DTCSP_INT32	DTCSP_ECC_GetPubKey(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_ECC_PUBLIC_KEY_PTR pECCPublicKey);

DTCSP_INT32	DTCSP_ECC_GenKeyPair (
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeyNumber,
    DTCSP_ECC_PUBLIC_KEY_PTR   pEccPublicKey,
    DTCSP_ECC_PRIVATE_KEY_PTR  pEccPrivateKey);

DTCSP_INT32 DTCSP_ECC_GenKeyPairEx(
   DTCSP_VOID_PTR				pContext,
   DTCSP_INT32					nStoreLocation,
   DTCSP_INT32  		nEccCurveFlag,
   DTCSP_ECC_CURVE_PTR	pEccCurve, 
   DTCSP_ECC_PUBLIC_KEY_PTR	pEccPublicKey,
   DTCSP_ECC_PRIVATE_KEY_PTR	pEccPrivateKey);

DTCSP_INT32	DTCSP_ECDSA_Sign(
    DTCSP_VOID_PTR 	pContext,
    DTCSP_INT32			nKeyNumber,
    DTCSP_ECC_PRIVATE_KEY_PTR pECCPrivatecKey,
    DTCSP_UCHAR_PTR		pInDataMes,
    DTCSP_INT32				nInDataMesLen,
    DTCSP_ECC_SIG_PTR	pOutDataSign);

DTCSP_INT32 DTCSP_ECDSA_Verify(
    DTCSP_VOID_PTR	    pContext,
    DTCSP_INT32  		nKeyNumber,
    DTCSP_ECC_PUBLIC_KEY_PTR   pEccPublicKey,
    DTCSP_UCHAR_PTR		pInDataMes,
    DTCSP_INT32				nInDataMesLen,
    DTCSP_ECC_SIG_PTR	pInDataSign,
    DTCSP_INT32_PTR     result);

DTCSP_INT32 DTCSP_SCE_Sign(
    DTCSP_VOID_PTR	   pContext,
    DTCSP_INT32		   nKeyNumber,
    DTCSP_ECC_PRIVATE_KEY_PTR   pEccPrivateKey,
    DTCSP_UCHAR_PTR		pInDataMes,
    DTCSP_INT32				nInDataMesLen,
    DTCSP_ECC_SIG_PTR		pOutDataSign);

DTCSP_INT32 DTCSP_SCE_Verify(
    DTCSP_VOID_PTR	    pContext,
    DTCSP_INT32  		nKeyNumber,
    DTCSP_ECC_PUBLIC_KEY_PTR   pEccPublicKey,
    DTCSP_UCHAR_PTR		pInDataMes,
    DTCSP_INT32			nInDataMesLen,
    DTCSP_ECC_SIG_PTR	pInDataSign,
    DTCSP_INT32_PTR     result);

DTCSP_INT32 DTCSP_SCE_Encrypt(
    DTCSP_VOID_PTR		pContext,
    DTCSP_INT32  		nKeyNumber ,
    DTCSP_ECC_PUBLIC_KEY_PTR   pEccPublicKey,
    DTCSP_UCHAR_PTR			pInData,
    DTCSP_INT32				nInDataLen,
    DTCSP_ECC_CIPHER_PTR	pOutData);

DTCSP_INT32 DTCSP_SCE_Decrypt(
    DTCSP_VOID_PTR		pContext,
    DTCSP_INT32  		nKeyNumber,
    DTCSP_ECC_PRIVATE_KEY_PTR  pEccPrivateKey,
    DTCSP_ECC_CIPHER_PTR	pInData,
    DTCSP_UCHAR_PTR			pOutData,
    DTCSP_INT32_PTR  		pOutDataLen);

DTCSP_INT32  DTCSP_ECDH_KeyAgreement (
    DTCSP_VOID_PTR	  pContext,
    DTCSP_INT32       nKeyNumber,
    DTCSP_ECC_PRIVATE_KEY_PTR  pOrgTempPriKey,
    DTCSP_ECC_PUBLIC_KEY_PTR   pRespoTempPubKey,
    DTCSP_UCHAR_PTR   pOutKey,
    DTCSP_INT32_PTR   pOutKeyLen);

DTCSP_INT32 DTCSP_SCE_KeyAgreement (
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32  		nKeyNumber,
    DTCSP_ECC_PRIVATE_KEY_PTR  pOrgEccPrivateKey,
    DTCSP_ECC_PRIVATE_KEY_PTR  pOrgTempPriKey,
    DTCSP_ECC_PUBLIC_KEY_PTR   pRespoPubKey,
    DTCSP_ECC_PUBLIC_KEY_PTR   pRespoTempPubKey,
    DTCSP_INT32      nflag,
    DTCSP_INT32      nKeyLen,
    DTCSP_UCHAR_PTR  pOrgID,
    DTCSP_INT32      nOrgIDLen,
    DTCSP_UCHAR_PTR  pRespoID,
    DTCSP_INT32      nRespoIDLen,
    DTCSP_UCHAR_PTR  pOutKey);

DTCSP_INT32 DTCSP_SCH_Hash (
    DTCSP_VOID_PTR		pContext,
    DTCSP_INT32			nKeyNum,
    DTCSP_ECC_PUBLIC_KEY_PTR	pPublicKey,
    DTCSP_UCHAR_PTR		pInData,
    DTCSP_INT32  		nInDataLen,
    DTCSP_INT32			pOutDataLen,
    DTCSP_UCHAR_PTR		pOutData);

DTCSP_INT32 DTCSP_SHA1_Hash(
    DTCSP_VOID_PTR		pContext,
    DTCSP_UCHAR_PTR  	pInData,
    DTCSP_INT32  	   	nInDataLen,
    DTCSP_UCHAR_PTR		pOutData,
    DTCSP_INT32_PTR     pOutDataLen);


DTCSP_INT32 DTCSP_SHA256_Hash (
    DTCSP_VOID_PTR		pContext,
    DTCSP_UCHAR_PTR		pInData,
    DTCSP_INT32  		nInDataLen,
    DTCSP_UCHAR_PTR		pOutData,
    DTCSP_INT32_PTR		pOutDataLen);

DTCSP_INT32 DTCSP_SM1ECBEncrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SM1ECBDecrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SM1CBCEncrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR pIv,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SM1CBCDecrypt(
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32		nKeynum,
    DTCSP_UCHAR_PTR	pKey,
    DTCSP_INT32		nKeyLen,
    DTCSP_UCHAR_PTR pIv,
    DTCSP_UCHAR_PTR	pInData,
    DTCSP_INT32		nInDataLen,
    DTCSP_UCHAR_PTR	pOutData,
    DTCSP_INT32_PTR	pOutDataLen);

DTCSP_INT32 DTCSP_SM2_1_Sign(
    DTCSP_VOID_PTR	   pContext,
    DTCSP_INT32		   nKeyNumber,
    DTCSP_ECC_PRIVATE_KEY_PTR   pEccPrivateKey,
    DTCSP_UCHAR_PTR		pInDataMes, 
    DTCSP_INT32			nInDataMesLen,
    DTCSP_ECC_SIG_PTR		pOutDataSign);

DTCSP_INT32 DTCSP_SM2_1_Verify(
    DTCSP_VOID_PTR	    pContext,
    DTCSP_INT32  		nKeyNumber,
    DTCSP_ECC_PUBLIC_KEY_PTR   pEccPublicKey,
    DTCSP_UCHAR_PTR		pInDataMes, 
    DTCSP_INT32			nInDataMesLen,
    DTCSP_ECC_SIG_PTR	pInDataSign,
    DTCSP_INT32_PTR			result);

DTCSP_INT32 DTCSP_SM2_3_Encrypt(
    DTCSP_VOID_PTR		pContext,
    DTCSP_INT32  		nKeyNumber ,
    DTCSP_ECC_PUBLIC_KEY_PTR   pEccPublicKey,
    DTCSP_UCHAR_PTR			pInData,
    DTCSP_INT32				nInDataLen,  
    DTCSP_ECC_CIPHER_PTR	pOutData);

DTCSP_INT32 DTCSP_SM2_3_Decrypt(
    DTCSP_VOID_PTR		pContext,
    DTCSP_INT32  		nKeyNumber,
    DTCSP_ECC_PRIVATE_KEY_PTR  pEccPrivateKey,
    DTCSP_ECC_CIPHER_PTR	pInData,
    DTCSP_UCHAR_PTR			pOutData,
    DTCSP_INT32_PTR  		pOutDataLen);

DTCSP_INT32 DTCSP_SM2_2_KeyAgreement (
    DTCSP_VOID_PTR	pContext,
    DTCSP_INT32  		nKeyNumber, 
    DTCSP_ECC_PRIVATE_KEY_PTR  pOrgEccPrivateKey,
    DTCSP_ECC_PRIVATE_KEY_PTR  pOrgTempPriKey,
    DTCSP_ECC_PUBLIC_KEY_PTR   pRespoPubKey,
    DTCSP_ECC_PUBLIC_KEY_PTR   pRespoTempPubKey,
    DTCSP_INT32      nflag,
    DTCSP_INT32      nKeyLen,  
    DTCSP_UCHAR_PTR  pOrgID,
    DTCSP_INT32      nOrgIDLen,  
    DTCSP_UCHAR_PTR  pRespoID,
    DTCSP_INT32      nRespoIDLen,  
    DTCSP_UCHAR_PTR  pOutKey);

DTCSP_INT32 DTCSP_SCH_Initialize (
    DTCSP_VOID_PTR			pContext, 
    DTCSP_SCH_CONTEXT_PTR	pSCHContext,
    DTCSP_INT32				nFlag,      
    DTCSP_UCHAR_PTR 			pID, 
    DTCSP_INT32           	nIDLen,
    DTCSP_INT32              nStoreLocation,
    DTCSP_INT32  			nEccCurveFlag,
    DTCSP_ECC_PUBLIC_KEY_PTR	pEccPublicKey);

DTCSP_INT32 DTCSP_SCH_Update(
    DTCSP_VOID_PTR			pContext,
    DTCSP_SCH_CONTEXT_PTR		pSCHContext,
    DTCSP_UCHAR_PTR 			pInData,       	
    DTCSP_INT32        		nInDataLen);

DTCSP_INT32 DTCSP_SCH_UpdateEx (
    DTCSP_VOID_PTR			pContext,
    DTCSP_SCH_CONTEXT_PTR	pSCHContext,
    DTCSP_UCHAR_PTR 		pInData,       	
    DTCSP_INT32        		nInDataLen);

DTCSP_INT32 DTCSP_SCH_Finalize(
    DTCSP_VOID_PTR 	pContext, 
    DTCSP_SCH_CONTEXT_PTR				pSCHContext,
    DTCSP_INT32 						nOutDataLen,
    DTCSP_UCHAR_PTR						pOutData);

DTCSP_INT32 DTCSP_SM3_Initialize (
    DTCSP_VOID_PTR			pContext, 
    DTCSP_SCH_CONTEXT_PTR	pSCHContext,
    DTCSP_INT32				nFlag,      
    DTCSP_UCHAR_PTR 			pID, 
    DTCSP_INT32           	nIDLen,
    DTCSP_INT32              nStoreLocation,
    DTCSP_INT32  			nEccCurveFlag,
    DTCSP_ECC_PUBLIC_KEY_PTR	pEccPublicKey);
DTCSP_INT32 DTCSP_SM3_Update(
    DTCSP_VOID_PTR			pContext,
    DTCSP_SCH_CONTEXT_PTR		pSCHContext,
    DTCSP_UCHAR_PTR 			pInData,       	
    DTCSP_INT32        		nInDataLen);

DTCSP_INT32 DTCSP_SM3_UpdateEx (
    DTCSP_VOID_PTR			pContext,
    DTCSP_SCH_CONTEXT_PTR	pSCHContext,
    DTCSP_UCHAR_PTR 		pInData,       	
    DTCSP_INT32        		nInDataLen);

DTCSP_INT32 DTCSP_SM3_Finalize(
    DTCSP_VOID_PTR 	pContext, 
    DTCSP_SCH_CONTEXT_PTR				pSCHContext,
    DTCSP_INT32 						nOutDataLen,
    DTCSP_UCHAR_PTR						pOutData);

DTCSP_INT32 DTCSP_SHA1_Initialize(
    DTCSP_VOID_PTR 	pContext,
    DTCSP_SHA_CONTEXT_PTR pSHA1Context);

DTCSP_INT32 DTCSP_SHA1_Update (
    DTCSP_VOID_PTR		pContext,
    DTCSP_SHA_CONTEXT_PTR	pSHA1Context,
    DTCSP_UCHAR_PTR 	pInData,       	
    DTCSP_INT32         nInDataLen);

DTCSP_INT32 DTCSP_SHA1_UpdateEx (
    DTCSP_VOID_PTR				pContext,
    DTCSP_SHA_CONTEXT_PTR	pSHA1Context,
    DTCSP_UCHAR_PTR 			pInData,       	
    DTCSP_INT32        		nInDataLen);

DTCSP_INT32 DTCSP_SHA1_Finalize(
    DTCSP_VOID_PTR 		pContext, 
    DTCSP_SHA_CONTEXT_PTR	pSHA1Context,
    DTCSP_UCHAR_PTR		pOutData,     
    DTCSP_INT32 *	pOutDataLen );

DTCSP_INT32 DTCSP_SHA256_Initialize(
    DTCSP_VOID_PTR 	pContext,
    DTCSP_SHA_CONTEXT_PTR pSHA1Context);

DTCSP_INT32 DTCSP_SHA256_Update (
    DTCSP_VOID_PTR		pContext,
    DTCSP_SHA_CONTEXT_PTR	pSHA1Context,
    DTCSP_UCHAR_PTR 	pInData,       	
    DTCSP_INT32         nInDataLen);

DTCSP_INT32 DTCSP_SHA256_UpdateEx (
    DTCSP_VOID_PTR				pContext,
    DTCSP_SHA_CONTEXT_PTR	pSHA1Context,
    DTCSP_UCHAR_PTR 			pInData,       	
    DTCSP_INT32        		nInDataLen);

DTCSP_INT32 DTCSP_SHA256_Finalize(
    DTCSP_VOID_PTR 		pContext, 
    DTCSP_SHA_CONTEXT_PTR	pSHA1Context,
    DTCSP_UCHAR_PTR		pOutData,     
    DTCSP_INT32 *	pOutDataLen );

////////////////////Error code//////////////////////////////////////
#define DTCSP_CARD_OK                 	0x7e0f
#define DTCSP_CARD_ERROR              	0x7ef0
#define DTCSP_SUCCESS			0
#define DTCSP_ERROR	    		1
/***********************net error code*****************************/
#define INIT_SOCKET_ERROR		0xf100
#define CLOSE_SOCKET_ERROR		0xf101
#define CONNECT_ERROR			0xf102
#define SEND_ERROR			0xf103
#define RECV_ERROR			0xf104
#define SELECT_ERROR			0xf105
#define REPAIR_SOCKET_ERROR		0xf106
#define SERVER_LOG_MSG_ERROR		0xf107
#define SERVER_COUNT_ERROR		0xf108
#define GETSERVERIP_ERROR		0xf109
/***********************pack error code****************************/
#define PACK_VALUE_ERROR 		0xf201
#define UNPACK_VALUE_ERROR		0xf202
#define PACK_DATA_ERROR 		0xf203
#define UNPACK_DATA_ERROR 		0xf204
/*********************shm sem error code***************************/
#define CREATE_SEM_ERROR		0xf301
#define CREATE_SHM_ERROR		0xf302
#define SEM_P_ERROR			0xf303
#define SEM_V_ERROR			0xf304
#define MAP_SHM_ERROR			0xf305
#define UNMAP_SHM_ERROR			0xf306
/***********************init error code****************************/
#define READ_CONFIG_FILE_ERROR		0xf400
#define DEVICE_TYPE_ERROR		0xf401
#define PASSWD_ERROR			0xf402
#define CHECK_VERSION_ERROR		0xf403
/******************Load Balance error code*************************/
#define LOAD_BALANCE_START_ERROR	0xf500
#define LOAD_BALANCE_END_ERROR		0xf501
/***********************other error code***************************/
#define INPUT_LEN_ERROR			0xf600
#define OUTPUT_LEN_ERROR		0xf601
//#define SSF33_KEY_LEN_ERROR		0xf602
#define KEY_LEN_ERROR		0xf602
#define RSA_MODULUS_LEN_ERROR		0xf603
#define RSA_KEY_NUMBER_ERROR		0xf604
#define RSA_KEY_NAME_ERROR          0xf605

//#define DTCSP_SUCCESS					0x0000
#define DTCSP_ERR_COMMAND_CODE			0xEEEE
#define DTCSP_ERR_LOADBALANCE_INIT		0xEE01
#define DTCSP_ERR_LOADBALANCE_GET		0xEE02
#define DTCSP_ERR_LOADBALANCE_REL		0xEE03
#define DTCSP_ERR_LOADBALANCE_GETALL	0xEE04
#define DTCSP_ERR_LOADBALANCE_RELALL	0xEE05
#define DTCSP_ERR_FAILED				0xFFFF

//	flash
//#define	DTCSP_ERR_FLASH_ADDRESS		0xEE10

//	communication
#define DTCSP_ERR_PARAMENT				0xEE20
#define	DTCSP_ERR_OPRN_FILE				0xEE21

//	For Config File
#define	DTCSP_ERR_CONFIG_FILE			0xEE30
#define	DTCSP_ERR_CONFIG_KEY			0xEE31

//  for Card Version
#define DTCSP_ERR_DSPVERSION_DIFFER    0xEE40
#define DTCSP_ERR_DSPVERSION_LOW       0xEE41

//all operation
#define DTCSP_ERR_MANAGEMENT_DENY              0xE000
#define DTCSP_ERR_OPERATION_DENY               0xE001
#define DTCSP_ERR_MNG_NOT_INITIALIZED          0xE050	// 管理员未初始化
#define DTCSP_ERR_MNG_ADD_OVERFLOW             0xE051	//  管理员已经有5个还要增加
#define DTCSP_ERR_MNG_ALREADY_EXIST            0xE057	//  增加原本存在的管理员
#define DTCSP_ERR_MNG_NOT_EXIST                0xE052	//  管理员不存在（登录不存在的管理员）、删除不存在的管理员
#define DTCSP_ERR_MNG_DEL_UNDERFLOW            0xE053	//  管理员数目为3时不可删除
#define DTCSP_ERR_ACCESS_VIOLATION             0xE054	//  管理权限不足
#define DTCSP_ERR_OPR_NOT_EXIST                0xE055	//  操作员不存在（未增加操作员时调用操作员登录，出错）
#define DTCSP_ERR_MNG_INIT_FORBID              0xE056	//  初始化被禁止（包括初始化flash、管理员）
#define DTCSP_ERR_MNG_COUNT_FLOW               0xE058	//  管理员数目溢出（<3 | >5)
#define	DTCSP_ERR_PASSWORD_LEN                 0xE059	//  操作员管理员口令必须为16字节（口令控制的密码卡）
#define DTCSP_ERR_FLASH_INIT_FORBID            0xE060	//  禁止初始化FLASH
#define DTCSP_ERR_RSAKEY_NOT_EXIST             0xE061	//  要读取的 rsa 密钥对不存在
#define DTCSP_ERR_MNG_NOT_LOGIN                0xE062	//
#define DTCSP_ERR_MASTERKEY_NOT_EXIST          0xE063	//	SYSTEM MK OR DEVICE MK NOT EXIST
#define DTCSP_ERR_OPRPASS_NOT_EXIST            0xE064	//  没初始化操作员口令
#define DTCSP_ERR_MNG_NUMBER_ILLEGAL           0xE065 //  管理员号码不合法

//for RSA cipher
#define DTCSP_ERR_RSA_KEYNUMBER                0xE100 // RSA key number error
#define DTCSP_ERR_RSA_MODULUSLENGTH            0xE101 // RSA modulus length error
#define ERR_SSX04_WAIT_N_BUSY_TIMEOUT          0xE102 // SSX04 chipset no response after a long time
#define ERR_SSX04_WAIT_READY_TIMEOUT           0xE103 //
#define ERR_SSX04_ME                           0xE104 // Modulus or Exponent error

//MPU
#define DTCSP_ERR_WRITE_MPU_TIMEOUT            0xE200
#define DTCSP_ERR_READ_MPU_TIMEOUT             0xE201
#define DTCSP_ERR_READ_MPU_LENGTH              0xE202

//DPRAM33
#define DTCSP_ERR_DPRAM33_LENGTH_LIMITED       0xE300

//Key flash operation error
#define	DTCSP_ERR_FLASH_ADDRESS                0xE400
#define DTCSP_ERR_USER_FLASH_BLOCK_ID          0xE401
#define DTCSP_ERR_USER_FLASH_BLOCK_OFFSET      0xE402
#define DTCSP_ERR_USER_FLASH_BLOCK_LENGTH      0xE403
#define DTCSP_ERR_FLAHS_BLOCK_NUMBER           0xE404
#define DTCSP_ERR_FLASH_BLOCK_PART_NUMBER      0xE405

//IC Card Read/Write
#define DTCSP_ERR_READ_IC_CARD                 0xE500
#define DTCSP_ERR_READ_MNG_IC_CARD             0xE501
#define DTCSP_ERR_READ_OPR_IC_CARD             0xE502
#define DTCSP_ERR_WRITE_MNG_IC_CARD            0xE503
#define DTCSP_ERR_WRITE_OPR_IC_CARD            0xE504

//Control Style
#define DTCSP_ERR_OPERATE_FORBID               0xE600 // 没有权限控制的密码卡在操作（初始化管理员等）中直接返回

//OTHERS
#define DTCSP_ERR_CHECK_SUM                    0xE700

///////////////////////////////  for 吉大  ////////////////////////////////////////
#define ERR_NOT_MANA_OPER                0xE701  // IC卡既不是管理员卡也不是操作员卡
#define ERR_MNG_IC_CARD                  0xE511  // 错误的管理员卡
#define ERR_MPU_STATUS                   0xE505  // MPU内状态错误: 吉大专用卡
////////////////////////////////////end//////////////////////////////////////////////

//Backup restore
#define DTCSP_ERR_BACKUPRESTORE_SHARE_NUMBER            0xE800
#define DTCSP_ERR_BACKUPRESTORE_PREVIOUS_STEP           0xE801
#define DTCSP_ERR_BACKUPRESTORE_FBN                     0xE802
#define DTCSP_ERR_BACKUPRESTORE_BPN                     0xE803
#define DTCSP_ERR_BACKUPRESTORE_NO_NEED_ANOTHER_PIECES  0xE804
#define DTCSP_ERR_BACKUPRESTORE_NEED_ANOTHER_PIECES     0xE805
#define DTCSP_ERR_BACKUPRESTORE_SECRETSHARE             0xE806
#define DTCSP_ERR_BACKUPRESTORE_SECRETRECONSTRUCT       0xE807

//PKCS#1 pad
#define DTCSP_ERR_PKCS1_BTERR			0xF030
#define DTCSP_ERR_PKCS1_BLOCKTYPE		0xF031
#define DTCSP_ERR_DATA_TOO_LONG			0xF032
#define DTCSP_ERR_NOERROR				0xF033
#define DTCSP_ERR_IVALID_PKCS1BLOCK		0xF034
#define DTCSP_ERR_PSERROR				0xF035
#define DTCSP_ERR_SPERROR				0xF036
#define DTCSP_ERR_DATA_LENGTH			0xF038


#ifdef __cplusplus
}
#endif

#endif
