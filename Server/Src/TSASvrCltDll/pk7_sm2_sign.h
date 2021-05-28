#ifndef _PK7_SM2_SIGN_H
#define _PK7_SM2_SIGN_H

#include "MySignLibFunc.h"

// PKCS7签名
long MYSIGN_SignDataByP7_SM2(
	unsigned char *pbIn,				// 输入数据
	long lInLen,						// 输入数据的长度
	char *pszCert,						// 签名证书
	char *pszKey,						// 签名私钥（若使用硬件设备，应为NULL）
	FUNC_PRIV_ENC funcPrivEnc,			// 私钥加密函数（若使用软私钥，应为NULL）
	char *pszOut,						// 输出数据
	bool bAttached = true);				// 是否包含原文

// PKCS7验证签名
long MYSIGN_VerifySignedDataByP7_SM2(
	char *pszSign,
	unsigned char *pbData = NULL,
	long lDataLen = 0);

// PKCS7验证签名
long MYSIGN_VerifySignedDataByP7_SM2_Ex(
	char *pszSign,
	unsigned char *pbData = NULL,
	long lDataLen = 0,
	unsigned char *pbCACert = 0,
	long lCACertLen = 0);

// 获取PKCS7签名信息
long MYSIGN_GetP7SignDataInfo_SM2(
	char *pszIn,
	long lType,
	unsigned char *pbOut,
	long *plOutLen);


#endif

