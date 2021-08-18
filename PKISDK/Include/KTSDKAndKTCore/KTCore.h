#ifndef _KTCORE_H
#define _KTCORE_H

#include "KTCoreDef.h"

int KTCore_Initialize();
int KTCore_Finalize();

// 软对称算法
#define SOFT_3DES			0x00
#define SOFT_CEA			0x01
// 国密对称算法标识
#define SGD_SM1_ECB						0x00000101	// SM1算法ECB加密模式
#define SGD_SM1_CBC						0x00000102	// SM1算法CBC加密模式
#define SGD_SM1_CFB						0x00000104	// SM1算法CFB加密模式
#define SGD_SM1_OFB						0x00000108	// SM1算法OFB加密模式
#define SGD_SM1_MAC						0x00000110	// SM1算法MAC加密模式
#define SGD_SSF33_ECB					0x00000201	// SSF33算法ECB加密模式
#define SGD_SSF33_CBC					0x00000202	// SSF33算法CBC加密模式
#define SGD_SSF33_CFB					0x00000204	// SSF33算法CFB加密模式
#define SGD_SSF33_OFB					0x00000208	// SSF33算法OFB加密模式
#define SGD_SSF33_MAC					0x00000210	// SSF33算法MAC加密模式
#define SGD_SM4_ECB						0x00000401	// SM4算法ECB加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_CBC						0x00000402	// SM4算法CBC加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_CFB						0x00000404	// SM4算法CFB加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_OFB						0x00000408	// SM4算法OFB加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_MAC						0x00000410	// SM4算法MAC加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_ZUC_EEA3					0x00000801	// ZUC祖冲之机密性算法128-EEA3算法	// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_ZUC_EIA3					0x00000802	// ZUC祖冲之完整性算法128-EIA3算法	// 《密码应用标识规范》（GM/T 0006-2012）新加
int KTCore_SetEncryptMethod(int nEncryptMethod);
int KTCore_GetEncryptMethod();

const KTChar *KTCore_Sign(
	const char *pszPlainFile,			// 原文件（待签名文件）绝对路径
	const char *pszSignatureFile,		// 数字签名文件绝对路径
	bool bPlainInSignature);			// 签名中是否包含原文件内容

const KTChar *KTCore_VerifySign(
	const char *pszSignatureFile,		// 数字签名文件绝对路径
	const char *pszPlainFile,			// 原文件（待验证签名文件）绝对路径。若签名中包含原文件内容，则将原文件输出
	KT_SIGN_INFO *pSignInfo);			// 签名信息

const KTChar *KTCore_Envelop(
	const char *pszPlainFiles,			// 原文件（待加密文件或目录）绝对路径，以'|'分隔
	const char *pszCipherFile,			// 数字信封文件绝对路径
	const char *pszEnvelopCerts);		// 加密证书，以'|'分隔

const KTChar *KTCore_Develop(
	const char *pszCipherFile,			// 数字信封文件绝对路径
	const char *pszPlainFileOrFolder);	// 原文件（解密文件或目录）绝对路径

const KTChar *KTCore_EnvelopSign(
	const char *pszPlainFiles,			// 原文件（待加密签名文件或目录）绝对路径，以'|'分隔
	const char *pszCipherSignatureFile, // 加密签名文件绝对路径
	const char *pszEnvelopCerts);		// 加密证书，以'|'分隔

const KTChar *KTCore_GetDecryptedFileNames(
	int *pnLen,							// 取解密文件名的缓冲区大小
	char *pszFileName);					// 取解密文件名的缓冲区

#endif
