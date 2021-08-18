#ifndef __SM_H__
#define __SM_H__

// KTINTEGER转换为数据串（长度为32倍数）
int KTINTEGER2SMData(const KTINTEGER& in, KTByteArray& out);
int KTINTEGER2SMData(const KTINTEGER& in, unsigned char *out, int *outlen);
// 数据串（长度为32倍数）转换为KTINTEGER
int SMData2KTINTEGER(KTByteArray& in, KTINTEGER& out);
int SMData2KTINTEGER(unsigned char *in, int inlen, KTINTEGER& out);

// 取公钥X分量
int GetXFromSM2PublicKey(const KTBITSTRING& in, KTByteArray& out);
int GetXFromSM2PublicKey(const KTBITSTRING& in, unsigned char *out, int *outlen);
// 取公钥Y分量
int GetYFromSM2PublicKey(const KTBITSTRING& in, KTByteArray& out);
int GetYFromSM2PublicKey(const KTBITSTRING& in, unsigned char *out, int *outlen);

// SM3杂凑算法
KTINT32 K_SM3_Digest(const KTBYTE *pInput, KTUINT nInputLen,
				KTBYTE *pOutput, KTUINT *pnOutputLen);

// SM2预处理
KTINT32 K_SM2_YCL(CSM2PublicKey *pKey,
				const KTBYTE *pInput, KTUINT nInputLen,
				KTBYTE *pOutput, KTUINT *pnOutputLen);

// SM2非对称算法，包括私钥签名和公钥验证签名、公钥加密和私钥解密
// 验证签名时，pInput为原文数据，pOutput为签名数据，均为输入数据
KTINT32 K_SM2_Cipher(EM_MODE mode, CKey *pKey,
				const KTBYTE *pInput, KTUINT nInputLen,
				KTBYTE *pOutput, KTUINT *pnOutputLen);

#endif
