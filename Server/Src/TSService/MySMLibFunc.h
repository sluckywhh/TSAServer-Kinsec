#ifndef _MYSMLIBFUNC_H
#define _MYSMLIBFUNC_H

#define MYSMERR_SUCCESS			0
#define MYSMERR_PARAMETER		1
#define MYSMERR_MEMORY			2
#define MYSMERR_NOTSUPPORT		3
#define MYSMERR_BNOPERATION		4
#define MYSMERR_ECCOPERATION	5
#define MYSMERR_CERT			6
#define MYSMERR_NOTECCCERT		7
#define MYSMERR_FILEOPERATION	8
#define MYSMERR_ECCSIGN			11
#define MYSMERR_ECCVERIFYSIGN	12
#define MYSMERR_ECCENCRYPT		13
#define MYSMERR_ECCDECRYPT		14
#define MYSMERR_ECCKEYEXCHANGE	21

#undef TRUE
#undef FALSE
#define TRUE				0x00000001	//布尔值为真
#define FALSE				0x00000000	//布尔值为假

#ifndef BYTE
typedef unsigned char       BYTE;
#endif
#ifndef KTBOOL32
typedef int					KTBOOL32;
#endif
#ifndef WORD
typedef unsigned short int	WORD;
#endif
#ifndef DWORD
#	ifdef WIN32
        typedef unsigned long   DWORD;
#	else
        typedef unsigned int    DWORD;
#	endif
#endif
#ifndef HANDLE
typedef void *				HANDLE;
#endif

/************************************************************************/
/*	初始化		 														*/
/************************************************************************/
long MYSM_Initialize(KTBOOL32 bFormal = TRUE);	// TRUE使用正式曲线，FALSE使用测试曲线
long MYSM_Finalize();
long MYSM_SetParam(KTBOOL32 bFormal = TRUE);

/************************************************************************/
/*	SM3密码杂凑算法														*/
/************************************************************************/
long MYSM_SM3Init(
	HANDLE			*phHandle);		// OUT

long MYSM_SM3Update(
	HANDLE			hHandle,		// IN
	unsigned char	*in,			// IN
	long			inlen);			// IN

long MYSM_SM3Final(
	HANDLE			hHandle,		// IN
	unsigned char	*out,			// OUT
	long			*outlen);		// IN OUT

long MYSM_SM3(
	unsigned char	*in,			// IN
	long			inlen,			// IN
	unsigned char	*out,			// OUT
	long			*outlen);		// IN OUT

/************************************************************************/
/*	SM2椭圆曲线公钥密码算法 											*/
/************************************************************************/
// 从证书获取ECC公钥数据
long MYSM_ECCGetPubKeyFromCert(
	unsigned char	*cert,			// IN
	long			certlen,		// IN
	unsigned char	*pubkey_x,		// OUT
	long			*pubkeylen_x,	// IN OUT
	unsigned char	*pubkey_y,		// OUT
	long			*pubkeylen_y);	// IN OUT

// 根据ECC私钥计算ECC公钥
long MYSM_ECCCalcPubKey(
	unsigned char	*prvkey,		// IN
	unsigned char	*pubkey_x,		// OUT
	unsigned char	*pubkey_y,		// OUT
	long			ecclen = 32);	// IN 256bits

// 生成ECC密钥对
long MYSM_ECCGenKeyPair(
	unsigned char	*prvkey,		// OUT
	unsigned char	*pubkey_x,		// OUT
	unsigned char	*pubkey_y,		// OUT
	long			ecclen = 32);	// IN 256bits

// 预处理1
// Z = SM3(ENTL‖ID‖a‖b‖Gx‖Gy‖Px‖Py)
long MYSM_ECCGetZ(
	unsigned char	*userid,		// IN
	long			useridlen,		// IN
	unsigned char	*pubkey_x,		// IN
	unsigned char	*pubkey_y,		// IN
	unsigned char	*out,			// OUT
	long			*outlen,		// IN OUT
	long			ecclen = 32);	// IN 256bits

// 预处理2
// H = SM3(Z‖M)
long MYSM_ECCGetH(
	unsigned char	*z,				// IN
	long			zlen,			// IN
	unsigned char	*in,			// IN
	long			inlen,			// IN
	unsigned char	*out,			// OUT
	long			*outlen);		// IN OUT

// 预处理2
// H = SM3(Z‖M)
long MYSM_ECCGetFileH(
	unsigned char	*z,				// IN
	long			zlen,			// IN
	char			*infile,		// IN
	unsigned char	*out,			// OUT
	long			*outlen);		// IN OUT

// ECC签名算法
long MYSM_ECCSign(
	unsigned char	*dgst,			// IN
	long			dgstlen,		// IN
	unsigned char	*sign_r,		// OUT
	unsigned char	*sign_s,		// OUT
	unsigned char	*prvkey,		// IN
	long			ecclen = 32);	// IN 256bits

// ECC验证签名算法
long MYSM_ECCVerifySign(
	unsigned char	*dgst,			// IN
	long			dgstlen,		// IN
	unsigned char	*sign_r,		// IN
	unsigned char	*sign_s,		// IN
	unsigned char	*pubkey_x,		// IN
	unsigned char	*pubkey_y,		// IN
	long			ecclen = 32);	// IN 256bits

// ECC加密算法
long MYSM_ECCEncrypt(
	unsigned char	*in,			// IN
	long			inlen,			// IN
	unsigned char	*out_c1,		// OUT
	unsigned char	*out_c2,		// OUT
	unsigned char	*out_c3,		// OUT
	unsigned char	*pubkey_x,		// IN
	unsigned char	*pubkey_y,		// IN
	long			ecclen = 32);	// IN 256bits

// ECC解密算法
long MYSM_ECCDecrypt(
	unsigned char	*in_c1,			// IN
	unsigned char	*in_c2,			// IN
	long			inlen_c2,		// IN
	unsigned char	*in_c3,			// IN
	unsigned char	*out,			// OUT
	long			*outlen,		// IN OUT
	unsigned char	*prvkey,		// IN
	long			ecclen = 32);	// IN 256bits

// ECC密钥协商
/*
 *	使用ECC密钥协商算法，产生协商参数并计算会话密钥，输出临时ECC密钥对公钥，并返回产生的密钥句柄
 *	B方运算
 *	A_id			A方ID
 *	A_idlen			A方ID长度
 *	A_pubkey_x 		A方公钥x分量
 *	A_pubkey_y		A方公钥y分量
 *	A_temppubkey_x 	A方临时公钥x分量
 *	A_temppubkey_y	A方临时公钥y分量
 *	B_id			B方ID
 *	B_idlen 		B方ID长度
 *	B_prvkey 		B方私钥
 *	B_pubkey_x 		B方公钥x分量
 *	B_pubkey_y		B方公钥y分量
 *	B_temppubkey_x 	B方临时公钥x分量
 *	B_temppubkey_y	B方临时公钥y分量
 *	algid			会话密钥算法标识
 *	key				会话密钥
 *	keylen			会话密钥长度
 */
long MYSM_ECCKeyExchangeB(
	unsigned char	*A_id,				// IN
	long			A_idlen,			// IN
	unsigned char	*A_pubkey_x,		// IN
	unsigned char	*A_pubkey_y,		// IN
	unsigned char	*A_temppubkey_x,	// IN
	unsigned char	*A_temppubkey_y,	// IN
	unsigned char	*B_id,				// IN
	long			B_idlen,			// IN
	unsigned char	*B_prvkey,			// IN
	unsigned char	*B_pubkey_x,		// IN
	unsigned char	*B_pubkey_y,		// IN
	unsigned char	*B_temppubkey_x,	// OUT
	unsigned char	*B_temppubkey_y,	// OUT
	long			algid,				// IN
	unsigned char	*key,				// OUT
	long			keylen,				// IN
	long			ecclen = 32);		// IN 256bits

/*
 *	使用ECC密钥协商算法，产生协商参数并计算会话密钥，输出临时ECC密钥对公钥，并返回产生的密钥句柄
 *	A方运算
 *	A_id			A方ID
 *	A_idlen			A方ID长度
 *	A_prvkey 		A方私钥
 *	A_pubkey_x 		A方公钥x分量
 *	A_pubkey_y		A方公钥y分量
 *	A_tempprvkey 	A方临时私钥
 *	A_temppubkey_x 	A方临时公钥x分量
 *	A_temppubkey_y	A方临时公钥y分量
 *	B_id			B方ID
 *	B_idlen 		B方ID长度
 *	B_pubkey_x 		B方公钥x分量
 *	B_pubkey_y		B方公钥y分量
 *	B_temppubkey_x 	B方临时公钥x分量
 *	B_temppubkey_y	B方临时公钥y分量
 *	algid			会话密钥算法标识
 *	key				会话密钥
 *	keylen			会话密钥长度
 */
long MYSM_ECCKeyExchangeA(
	unsigned char	*A_id,				// IN
	long			A_idlen,			// IN
	unsigned char	*A_prvkey,			// IN
	unsigned char	*A_pubkey_x,		// IN
	unsigned char	*A_pubkey_y,		// IN
	unsigned char	*A_tempprvkey,		// IN
	unsigned char	*A_temppubkey_x,	// IN
	unsigned char	*A_temppubkey_y,	// IN
	unsigned char	*B_id,				// IN
	long			B_idlen,			// IN
	unsigned char	*B_pubkey_x,		// IN
	unsigned char	*B_pubkey_y,		// IN
	unsigned char	*B_temppubkey_x,	// IN
	unsigned char	*B_temppubkey_y,	// IN
	long			algid,				// IN
	unsigned char	*key,				// OUT
	long			keylen,				// IN
	long			ecclen = 32);		// IN 256bits

/************************************************************************/
/*	SM4分组密码算法														*/
/************************************************************************/
// SM4运算初始化（密钥扩展）
// 密钥为16字节
// 加密：dwFlag = 0
// 解密：dwFlag = 1
long MYSM_SM4Init(
	unsigned char	*key,				// IN
	DWORD			dwFlag,				// IN
	HANDLE			*phHandle);			// OUT

// SM4运算，可循环多次调用
// 原文长度为16倍数
long MYSM_SM4Update(
	HANDLE			hHandle,			// IN
	unsigned char	*in,				// IN
	long			inlen,				// IN
	unsigned char	*out,				// OUT
	long			*outlen);			// IN OUT

// 结束SM4运算
// 运算包含填充和去填充
long MYSM_SM4Final(
	HANDLE			hHandle,			// IN
	unsigned char	*in,				// IN
	long			inlen,				// IN
	unsigned char	*out,				// OUT
	long			*outlen);			// IN OUT

// SM4运算
// 运算包含填充和去填充
// 密钥为16字节
// 加密：dwFlag = 0
// 解密：dwFlag = 1
long MYSM_SM4(
	unsigned char	*key,				// IN
	DWORD			dwFlag,				// IN
	unsigned char	*in,				// IN
	long			inlen,				// IN
	unsigned char	*out,				// OUT
	long			*outlen);			// IN OUT

///////////////////////////////////////////////////////////
// gmssl

// SM4运算初始化
// 密钥为16字节
// cbc模式iv为16字节
// ecb模式：dwMode = 1
// cbc模式：dwMode = 2
// 加密：dwFlag = 0
// 解密：dwFlag = 1
long MYSM_SMS4Init(
	unsigned char	*key,				// IN
	unsigned char	*iv,				// IN
	DWORD			dwMode,				// IN
	DWORD			dwFlag,				// IN
	HANDLE			*phHandle);			// OUT

// SM4运算，可循环多次调用
// 原文长度为16倍数
long MYSM_SMS4Update(
	HANDLE			hHandle,			// IN
	unsigned char	*in,				// IN
	long			inlen,				// IN
	unsigned char	*out,				// OUT
	long			*outlen);			// IN OUT

// 结束SM4运算
// 运算包含填充和去填充
long MYSM_SMS4Final(
	HANDLE			hHandle,			// IN
	unsigned char	*in,				// IN
	long			inlen,				// IN
	unsigned char	*out,				// OUT
	long			*outlen);			// IN OUT

// SM4运算
// 运算包含填充和去填充
// 密钥为16字节
// cbc模式iv为16字节
// ecb模式：dwMode = 1
// cbc模式：dwMode = 2
// 加密：dwFlag = 0
// 解密：dwFlag = 1
long MYSM_SMS4(
	unsigned char	*key,				// IN
	unsigned char	*iv,				// IN
	DWORD			dwMode,				// IN
	DWORD			dwFlag,				// IN
	unsigned char	*in,				// IN
	long			inlen,				// IN
	unsigned char	*out,				// OUT
	long			*outlen);			// IN OUT

/************************************************************************/
/*	证书解析															*/
/************************************************************************/
KTBOOL32 MYTOOL_ParseExtCertificatePolicies(unsigned char *extvalue, long extvaluelen, char *out, long *outlen);
KTBOOL32 MYTOOL_ParseExtExtKeyUsage(unsigned char *extvalue, long extvaluelen, DWORD *extkeyusage, char *out, long *outlen);
KTBOOL32 MYTOOL_ParseExtCrlDistributionPoints(unsigned char *extvalue, long extvaluelen, char *out, long *outlen);
KTBOOL32 MYTOOL_ParseExtNetScapeCertType(unsigned char *extvalue, long extvaluelen, char *out, long *outlen);
KTBOOL32 MYTOOL_ParseExtAuthorityInfoAccess(unsigned char *extvalue, long extvaluelen, char *out, long *outlen);

#endif
