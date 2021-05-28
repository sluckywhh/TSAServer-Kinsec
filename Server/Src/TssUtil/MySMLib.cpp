#include "MySMLibFunc.h"

#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/errno.h>
#include <unistd.h>
#endif

#ifndef _NOTUSEOPENSSL
//#include <openssl/ec.h>
//#include <openssl/ec_lcl.h>
//#include <openssl/ecdsa.h>
//#include <openssl/ecs_locl.h>
//#include <openssl/ecdh.h>
//#include <openssl/ech_locl.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/sha.h>
#endif

#define MYSMLEN_SM3HASH			32
#define MYSMLEN_DATABLOCK		4096

KTBOOL32 g_bFormal = TRUE;
long g_lReference = 0;

// =================================================================================
// ecc曲线参数
// 支持32 bytes(256 bits)
// 目前国密未发布64 bytes(512 bits)

// 正式曲线参数
/*
推荐使用素数域256位椭圆曲线。
椭圆曲线方程：y2 = x3 + ax + b。
曲线参数：
p =  FFFFFFFE FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 00000000 FFFFFFFF FFFFFFFF
a =  FFFFFFFE FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 00000000 FFFFFFFF FFFFFFFC
b =  28E9FA9E 9D9F5E34 4D5A9E4B CF6509A7 F39789F5 15AB8F92 DDBCBD41 4D940E93
Gx = 32C4AE2C 1F198119 5F990446 6A39C994 8FE30BBF F2660BE1 715A4589 334C74C7
Gy = BC3736A2 F4F6779C 59BDCEE3 6B692153 D0A9877C C62A4740 02DF32E5 2139F0A0
n =  FFFFFFFE FFFFFFFF FFFFFFFF FFFFFFFF 7203DF6B 21C6052B 53BBF409 39D54123
256的ECC，前32字节为0，后32字节有效
*/
unsigned char g_eccP[] = {
	0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char g_eccA[] = {
	0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC};
unsigned char g_eccB[] = {
	0x28, 0xE9, 0xFA, 0x9E, 0x9D, 0x9F, 0x5E, 0x34, 0x4D, 0x5A, 0x9E, 0x4B, 0xCF, 0x65, 0x09, 0xA7,
	0xF3, 0x97, 0x89, 0xF5, 0x15, 0xAB, 0x8F, 0x92, 0xDD, 0xBC, 0xBD, 0x41, 0x4D, 0x94, 0x0E, 0x93};
unsigned char g_eccGx[] = {
	0x32, 0xC4, 0xAE, 0x2C, 0x1F, 0x19, 0x81, 0x19, 0x5F, 0x99, 0x04, 0x46, 0x6A, 0x39, 0xC9, 0x94,
	0x8F, 0xE3, 0x0B, 0xBF, 0xF2, 0x66, 0x0B, 0xE1, 0x71, 0x5A, 0x45, 0x89, 0x33, 0x4C, 0x74, 0xC7};
unsigned char g_eccGy[] = {
	0xBC, 0x37, 0x36, 0xA2, 0xF4, 0xF6, 0x77, 0x9C, 0x59, 0xBD, 0xCE, 0xE3, 0x6B, 0x69, 0x21, 0x53,
	0xD0, 0xA9, 0x87, 0x7C, 0xC6, 0x2A, 0x47, 0x40, 0x02, 0xDF, 0x32, 0xE5, 0x21, 0x39, 0xF0, 0xA0};
unsigned char g_eccOrder[] = {
	0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x72, 0x03, 0xDF, 0x6B, 0x21, 0xC6, 0x05, 0x2B, 0x53, 0xBB, 0xF4, 0x09, 0x39, 0xD5, 0x41, 0x23};

// 测试算法用
unsigned char g_eccTestP[] = {
	0x85, 0x42, 0xD6, 0x9E, 0x4C, 0x04, 0x4F, 0x18, 0xE8, 0xB9, 0x24, 0x35, 0xBF, 0x6F, 0xF7, 0xDE,
	0x45, 0x72, 0x83, 0x91, 0x5C, 0x45, 0x51, 0x7D, 0x72, 0x2E, 0xDB, 0x8B, 0x08, 0xF1, 0xDF, 0xC3};
unsigned char g_eccTestA[] = {
	0x78, 0x79, 0x68, 0xB4, 0xFA, 0x32, 0xC3, 0xFD, 0x24, 0x17, 0x84, 0x2E, 0x73, 0xBB, 0xFE, 0xFF,
	0x2F, 0x3C, 0x84, 0x8B, 0x68, 0x31, 0xD7, 0xE0, 0xEC, 0x65, 0x22, 0x8B, 0x39, 0x37, 0xE4, 0x98};
unsigned char g_eccTestB[] = {
	0x63, 0xE4, 0xC6, 0xD3, 0xB2, 0x3B, 0x0C, 0x84, 0x9C, 0xF8, 0x42, 0x41, 0x48, 0x4B, 0xFE, 0x48,
	0xF6, 0x1D, 0x59, 0xA5, 0xB1, 0x6B, 0xA0, 0x6E, 0x6E, 0x12, 0xD1, 0xDA, 0x27, 0xC5, 0x24, 0x9A};
unsigned char g_eccTestGx[] = {
	0x42, 0x1D, 0xEB, 0xD6, 0x1B, 0x62, 0xEA, 0xB6, 0x74, 0x64, 0x34, 0xEB, 0xC3, 0xCC, 0x31, 0x5E,
	0x32, 0x22, 0x0B, 0x3B, 0xAD, 0xD5, 0x0B, 0xDC, 0x4C, 0x4E, 0x6C, 0x14, 0x7F, 0xED, 0xD4, 0x3D};
unsigned char g_eccTestGy[] = {
	0x06, 0x80, 0x51, 0x2B, 0xCB, 0xB4, 0x2C, 0x07, 0xD4, 0x73, 0x49, 0xD2, 0x15, 0x3B, 0x70, 0xC4,
	0xE5, 0xD7, 0xFD, 0xFC, 0xBF, 0xA3, 0x6E, 0xA1, 0xA8, 0x58, 0x41, 0xB9, 0xE4, 0x6E, 0x09, 0xA2};
unsigned char g_eccTestOrder[] = {
	0x85, 0x42, 0xD6, 0x9E, 0x4C, 0x04, 0x4F, 0x18, 0xE8, 0xB9, 0x24, 0x35, 0xBF, 0x6F, 0xF7, 0xDD,
	0x29, 0x77, 0x20, 0x63, 0x04, 0x85, 0x62, 0x8D, 0x5A, 0xE7, 0x4E, 0xE7, 0xC3, 0x2E, 0x79, 0xB7};
// =================================================================================

#ifndef _NOTUSEOPENSSL
bool g_bOpenSSLInited = 0;

//线程安全
#ifdef _LINUX
	static pthread_mutex_t *lock_cs;
#else
	static HANDLE *lock_cs;
#endif
static long *lock_count;

void pthreads_locking_callback(int mode, int type, char *file, int line)
{
	if (mode & CRYPTO_LOCK)
	{
#ifdef _LINUX
		pthread_mutex_lock(&(lock_cs[type]));
#else
		DWORD d = WaitForSingleObject(lock_cs[type], INFINITE);
		/// \todo check 'd' for result
#endif
		lock_count[type]++;
	}
	else
	{
#ifdef _LINUX
		pthread_mutex_unlock(&(lock_cs[type]));
#else
		::ReleaseMutex(lock_cs[type]);
#endif
	}
}

unsigned long pthreads_thread_id(void)
{
	unsigned long ret;
#ifdef _LINUX
	ret=(unsigned long)pthread_self();
#else
	ret = GetCurrentThreadId();
#endif
	return(ret);
}

/*************************************************************************
 Function : PKI_Initilze
 Purpose  : 初始化算法库、线程安全初始化
 Params   : 无
 Return   : void
*************************************************************************/
void PKI_Initilze()
{
	if( g_bOpenSSLInited )
		return;

	OpenSSL_add_all_algorithms();

	//线程安全
	int i = 0;

#ifdef _LINUX
	lock_cs = (pthread_mutex_t *)OPENSSL_malloc( CRYPTO_num_locks() * sizeof(pthread_mutex_t) );
#else
	lock_cs = (HANDLE *)OPENSSL_malloc( CRYPTO_num_locks() * sizeof(HANDLE) );
#endif
	lock_count = (long *)OPENSSL_malloc( CRYPTO_num_locks() * sizeof(long) );
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		lock_count[i]=0;
#ifdef _LINUX
		pthread_mutex_init(&(lock_cs[i]),NULL);
#else
		lock_cs[i] = ::CreateMutex(NULL, FALSE, NULL);
#endif
	}

	CRYPTO_set_id_callback((unsigned long (*)())pthreads_thread_id);
	CRYPTO_set_locking_callback((void (*)(int, int, const char*, int))pthreads_locking_callback);

	g_bOpenSSLInited = 1;
}

/*************************************************************************
 Function : PKI_Finalize
 Purpose  : 释放算法库资源
 Params   : 无
 Return   : void
*************************************************************************/
void PKI_Finalize()
{
	if( g_bOpenSSLInited )
	{
		int i = 0;

		CRYPTO_set_locking_callback( 0 );
		for (i=0; i<CRYPTO_num_locks(); i++)
		{
#ifdef _LINUX
			pthread_mutex_destroy(&(lock_cs[i]));
#else
			::CloseHandle(lock_cs[i]);
#endif
		}

		OPENSSL_free(lock_cs);
		OPENSSL_free(lock_count);

		CONF_modules_unload(1);
		EVP_cleanup();
		ENGINE_cleanup();
		CRYPTO_cleanup_all_ex_data();
		ERR_remove_state(0);
		ERR_free_strings();

		g_bOpenSSLInited = 0;
	}
}
#endif

long MYSM_Initialize(KTBOOL32 bFormal)
{
	if (g_lReference == 0)
	{
#ifndef _NOTUSEOPENSSL
		PKI_Initilze();
		MYSM_SetParam(bFormal);
#endif
	}
	g_lReference++;

	return MYSMERR_SUCCESS;
}

long MYSM_Finalize()
{
	g_lReference--;
	if (g_lReference == 0)
	{
#ifndef _NOTUSEOPENSSL
		PKI_Finalize();
#endif
	}

	return MYSMERR_SUCCESS;
}

long MYSM_SetParam(KTBOOL32 bFormal)
{
	g_bFormal = bFormal;

	return MYSMERR_SUCCESS;
}

// ecc parameter
// 32(256 bits) is supported
// 64(512 bits) is not supported yet
long MYSM_ECCSetParam(
	int ecclen,
	unsigned char **eccP,
	unsigned char **eccA,
	unsigned char **eccB,
	unsigned char **eccGx,
	unsigned char **eccGy,
	unsigned char **eccOrder)
{
	if (ecclen != 32)
		return MYSMERR_PARAMETER;

	if (g_bFormal)
	{
		*eccP = g_eccP;
		*eccA = g_eccA;
		*eccB = g_eccB;
		*eccGx = g_eccGx;
		*eccGy = g_eccGy;
		*eccOrder = g_eccOrder;
	}
	else
	{
		*eccP = g_eccTestP;
		*eccA = g_eccTestA;
		*eccB = g_eccTestB;
		*eccGx = g_eccTestGx;
		*eccGy = g_eccTestGy;
		*eccOrder = g_eccTestOrder;
	}

	return MYSMERR_SUCCESS;
}

// octet string -> integer
DWORD MYSM_Str2Num(unsigned char *in)
{
	DWORD out = 0;

	if (in == NULL)
		return out;

	DWORD tmp = in[0];
	tmp <<= 24;
	out += tmp;

	tmp = in[1];
	tmp <<= 16;
	out += tmp;

	tmp = in[2];
	tmp <<= 8;
	out += tmp;

	out += in[3];

	return out;
}

// integer -> octet string
void MYSM_Num2Str(DWORD in, unsigned char *out)
{
	if (out == NULL)
		return;

	out[0] = (unsigned char)((in & 0xFF000000) >> 24);
	out[1] = (unsigned char)((in & 0x00FF0000) >> 16);
	out[2] = (unsigned char)((in & 0x0000FF00) >> 8);
	out[3] = (unsigned char)(in & 0x000000FF);
}

// 32bit循环移位
// bits：移动位数。小于0，左移；大于0，右移
DWORD MYSM_XHYW(DWORD in, int bits)
{
	// <<左移： 丢弃最高位，最低位补0
	// >>右移：
	//			无符号整数：丢弃最低位，最高位补0
	//			有符号整数：符号位向右移动后，正数补0，负数补1

	bits %= 32;

	int tmp, tmp2;
	DWORD out = 0;

	if (bits < 0)
	{
		tmp = -bits;
		tmp2 = 32 - tmp;
		out = (in << tmp) | (in >> tmp2);
	}
	else if (bits > 0)
	{
		tmp = bits;
		tmp2 = 32 - tmp;
		out = (in >> tmp) | (in << tmp2);
	}
	else
		out = in;

	return out;
}

// 布尔函数FF
DWORD MYSM_FF(int i, DWORD X, DWORD Y, DWORD Z)
{
	DWORD out = 0;

	if (i < 0 || i > 63)
		return out;

	if (i < 16)
		out = X ^ Y ^Z;
	else
		out = (X & Y) | (X & Z) | (Y & Z);

	return out;
}

// 布尔函数GG
DWORD MYSM_GG(int i, DWORD X, DWORD Y, DWORD Z)
{
	DWORD out = 0;

	if (i < 0 || i > 63)
		return out;

	if (i < 16)
		out = X ^ Y ^Z;
	else
		out = (X & Y) | (~X & Z);

	return out;
}

// 置换函数P0
DWORD MYSM_P0(DWORD in)
{
	DWORD out = in ^ MYSM_XHYW(in, -9) ^ MYSM_XHYW(in, -17);

	return out;
}

// 置换函数P1
DWORD MYSM_P1(DWORD in)
{
	DWORD out = in ^ MYSM_XHYW(in, -15) ^ MYSM_XHYW(in, -23);

	return out;
}

// 消息扩展
// BB：64字节
long MYSM_XXKZ(unsigned char *BB, DWORD W[68], DWORD WW[64])
{
	if (BB == NULL || W == NULL || WW == NULL)
		return MYSMERR_PARAMETER;

	int i;
	for (i = 0; i < 16; i++)
		W[i] = MYSM_Str2Num(BB + i * 4);

	for (i = 16; i < 68; i++)
		W[i] = MYSM_P1(W[i - 16] ^ W[i - 9] ^ MYSM_XHYW(W[i - 3], -15)) ^ MYSM_XHYW(W[i - 13], -7) ^ W[i - 6];

	for (i = 0; i < 64; i++)
		WW[i] = W[i] ^ W[i + 4];

	return MYSMERR_SUCCESS;
}

// 压缩函数
// BB：64字节
long MYSM_CF(unsigned char *BB, unsigned char *V)
{
	if (BB == NULL || V == NULL)
		return MYSMERR_PARAMETER;

	// 消息扩展
	DWORD W[68], WW[64];
	long lRet = MYSM_XXKZ(BB, W, WW);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// ABCDEFGH <- V
	DWORD A, B, C, D, E, F, G, H;
	A = MYSM_Str2Num(V);
	B = MYSM_Str2Num(V + 4);
	C = MYSM_Str2Num(V + 8);
	D = MYSM_Str2Num(V + 12);
	E = MYSM_Str2Num(V + 16);
	F = MYSM_Str2Num(V + 20);
	G = MYSM_Str2Num(V + 24);
	H = MYSM_Str2Num(V + 28);

	// 备份初始值
	DWORD A0, B0, C0, D0, E0, F0, G0, H0;
	A0 = A;
	B0 = B;
	C0 = C;
	D0 = D;
	E0 = E;
	F0 = F;
	G0 = G;
	H0 = H;

	// 循环计算
	DWORD T, SS1, SS2, TT1, TT2;
	for (int i = 0; i < 64; i++)
	{
		T = i < 16 ? 0x79cc4519 : 0x7a879d8a;

		SS1 = MYSM_XHYW(MYSM_XHYW(A, -12) + E + MYSM_XHYW(T, -(i % 32)), -7);

		SS2 = SS1 ^ MYSM_XHYW(A, -12);

		TT1 = MYSM_FF(i, A, B, C) + D + SS2 + WW[i];

		TT2 = MYSM_GG(i, E, F, G) + H + SS1 + W[i];

		D = C;
		C = MYSM_XHYW(B, -9);
		B = A;
		A = TT1;
		H = G;
		G = MYSM_XHYW(F, -19);
		F = E;
		E = MYSM_P0(TT2);
	}

	// ABCDEFGH = ABCDEFGH ⊕ V
	A ^= A0;
	B ^= B0;
	C ^= C0;
	D ^= D0;
	E ^= E0;
	F ^= F0;
	G ^= G0;
	H ^= H0;
	
	// V <- ABCDEFGH
	MYSM_Num2Str(A, V);
	MYSM_Num2Str(B, V + 4);
	MYSM_Num2Str(C, V + 8);
	MYSM_Num2Str(D, V + 12);
	MYSM_Num2Str(E, V + 16);
	MYSM_Num2Str(F, V + 20);
	MYSM_Num2Str(G, V + 24);
	MYSM_Num2Str(H, V + 28);

	return MYSMERR_SUCCESS;
}

// 密钥派生函数
long MYSM_KDF(
	unsigned char *Z,
	long zlen,
	unsigned char *K,
	long klen)			// in byte
{
	if (Z == NULL || zlen <= 0 ||
		K == NULL || klen <= 0)
		return MYSMERR_PARAMETER;
	if (zlen > 1024)
		return MYSMERR_PARAMETER;

	long i, lTimes, lRemains, lRet;
	lTimes = klen / MYSMLEN_SM3HASH;
	lRemains = klen % MYSMLEN_SM3HASH;
	if (lRemains != 0)
		lTimes += 1;

	unsigned char byData[1024 + 10], byDigest[MYSMLEN_SM3HASH];
	long lDataLen, lDigestLen;
	memset(byData, 0, sizeof(byData));
	memcpy(byData, Z, zlen);
	lDataLen = zlen + 4;

	long lTmp = 0;
	for (i = 1; i <= lTimes; i++)
	{
		MYSM_Num2Str((DWORD)i, byData + zlen);

		memset(byDigest, 0, sizeof(byDigest));
		lDigestLen = MYSMLEN_SM3HASH;
		lRet = MYSM_SM3(
			byData,
			lDataLen,
			byDigest,
			&lDigestLen);
		if (lRet != MYSMERR_SUCCESS)
			return lRet;

		if (i < lTimes || lRemains == 0)
			memcpy(K + lTmp, byDigest, lDigestLen);
		else
			memcpy(K + lTmp, byDigest, lRemains);
		lTmp += lDigestLen;
	}

	return MYSMERR_SUCCESS;
}

#pragma pack(1)

// SM3杂凑算法
typedef struct tagMYSM3Context{
	unsigned char BB[64];
	long lBBLen;
	unsigned char V[MYSMLEN_SM3HASH];
	long lTotalLen;
} MYSM3CONTEXT, *PMYSM3CONTEXT;

#pragma pack()

long MYSM_SM3Init(
	HANDLE			*phHandle)
{
	if (phHandle == NULL)
		return MYSMERR_PARAMETER;

	PMYSM3CONTEXT pMySM3Context = new MYSM3CONTEXT;
	if (pMySM3Context == NULL)
		return MYSMERR_MEMORY;

	memset(pMySM3Context, 0, sizeof(MYSM3CONTEXT));
	memcpy(pMySM3Context->V, "\
\x73\x80\x16\x6f\x49\x14\xb2\xb9\x17\x24\x42\xd7\xda\x8a\x06\x00\
\xa9\x6f\x30\xbc\x16\x31\x38\xaa\xe3\x8d\xee\x4d\xb0\xfb\x0e\x4e", MYSMLEN_SM3HASH);

	*phHandle = (HANDLE)pMySM3Context;

	return MYSMERR_SUCCESS;
}

long MYSM_SM3Update(
	HANDLE			hHandle,
	unsigned char	*in,
	long			inlen)
{
	if (inlen == 0)
		return MYSMERR_SUCCESS;

	PMYSM3CONTEXT pMySM3Context = (PMYSM3CONTEXT)hHandle;
	if (pMySM3Context == NULL || in == NULL || inlen < 0)
	{
		if (pMySM3Context != NULL)
			delete pMySM3Context;
		return MYSMERR_PARAMETER;
	}

	long lRet, lLen, lTimes, lRemains;
	lLen = pMySM3Context->lBBLen + inlen;
	lTimes = lLen / 64;
	lRemains = lLen % 64;

	unsigned char byData[64];
	for (long i = 0; i < lTimes; i++)
	{
		memset(byData, 0, sizeof(byData));
		if (i == 0 && pMySM3Context->lBBLen > 0)
		{
			memcpy(byData, pMySM3Context->BB, pMySM3Context->lBBLen);
			memcpy(byData + pMySM3Context->lBBLen, in, 64 - pMySM3Context->lBBLen);
		}
		else
			memcpy(byData, in + i * 64 - pMySM3Context->lBBLen, 64);

		lRet = MYSM_CF(byData, pMySM3Context->V);
		if (lRet != MYSMERR_SUCCESS)
		{
			delete pMySM3Context;
			return lRet;
		}
	}

	if (lRemains > 0)
	{
		if (lTimes == 0)
			memcpy(pMySM3Context->BB + pMySM3Context->lBBLen, in, inlen);
		else
		{
			memset(pMySM3Context->BB, 0, sizeof(pMySM3Context->BB));
			memcpy(pMySM3Context->BB, in + lTimes * 64 - pMySM3Context->lBBLen, lRemains);
		}
	}
	else
		memset(pMySM3Context->BB, 0, sizeof(pMySM3Context->BB));
	pMySM3Context->lBBLen = lRemains;
	pMySM3Context->lTotalLen += inlen;

	return MYSMERR_SUCCESS;
}

long MYSM_SM3Final(
	HANDLE			hHandle,
	unsigned char	*out,
	long			*outlen)
{
	PMYSM3CONTEXT pMySM3Context = (PMYSM3CONTEXT)hHandle;
	if (pMySM3Context == NULL || out == NULL || outlen == NULL)
	{
		if (pMySM3Context != NULL)
			delete pMySM3Context;
		return MYSMERR_PARAMETER;
	}

	long lRet, lLen, lPadded;
	lLen = pMySM3Context->lBBLen;
	lPadded = 64;
	if (lLen > 64 - 8 - 1)
		lPadded = 128;

	// 填充
	// 剩余data || 0x80 || ... || 8字节data长度(in bit)
	unsigned char byData[128];
	memset(byData, 0, sizeof(byData));
	if (lLen > 0)
		memcpy(byData, pMySM3Context->BB, lLen);
	memcpy(byData + lLen, "\x80", 1);
	MYSM_Num2Str(pMySM3Context->lTotalLen * 8, byData + lPadded - 4);

	for (long i = 0; i < lPadded / 64; i++)
	{
		lRet = MYSM_CF(byData + i * 64, pMySM3Context->V);
		if (lRet != MYSMERR_SUCCESS)
		{
			delete pMySM3Context;
			return lRet;
		}
	}

	memcpy(out, pMySM3Context->V, MYSMLEN_SM3HASH);
	*outlen = MYSMLEN_SM3HASH;
	delete pMySM3Context;

	return MYSMERR_SUCCESS;
}

long MYSM_SM3(
	unsigned char	*in,
	long			inlen,
	unsigned char	*out,
	long			*outlen)
{
	if (in == NULL || inlen <= 0 ||
		out == NULL || outlen == NULL)
		return MYSMERR_PARAMETER;

	HANDLE hHandle = NULL;
	long lRet = MYSM_SM3Init(&hHandle);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	lRet = MYSM_SM3Update(hHandle, in, inlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	lRet = MYSM_SM3Final(hHandle, out, outlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	return MYSMERR_SUCCESS;
}

// 从证书获取公钥数据
long MYSM_ECCGetPubKeyFromCert(
	unsigned char	*cert,
	long			certlen,
	unsigned char	*pubkey_x,
	long			*pubkeylen_x,
	unsigned char	*pubkey_y,
	long			*pubkeylen_y)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	// x509 cert
	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return MYSMERR_CERT;

	// public key algo
	int algo = OBJ_obj2nid(x509->cert_info->key->algor->algorithm);
	if (algo != NID_X9_62_id_ecPublicKey)
	{
		X509_free(x509);
		return MYSMERR_NOTECCCERT;
	}

	// get public key
	long pubkeylen = x509->cert_info->key->public_key->length;
	if (pubkeylen == 0 || pubkeylen > 513)
	{
		X509_free(x509);
		return MYSMERR_CERT;
	}

	pszTmp = x509->cert_info->key->public_key->data;
	*pubkeylen_x = (pubkeylen - 1) / 2;
	memcpy(pubkey_x, pszTmp + 1, *pubkeylen_x);
	*pubkeylen_y = *pubkeylen_x;
	memcpy(pubkey_y, pszTmp + 1 + (*pubkeylen_x), *pubkeylen_y);

	X509_free(x509);

	return MYSMERR_SUCCESS;
#endif
}

// 根据ECC私钥计算ECC公钥
long MYSM_ECCCalcPubKey(
	unsigned char	*prvkey,		// IN
	unsigned char	*pubkey_x,		// OUT
	unsigned char	*pubkey_y,		// OUT
	long			ecclen)			// IN 256bits
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (prvkey == NULL ||
		pubkey_x == NULL || pubkey_y == NULL)
		return MYSMERR_PARAMETER;

	// ecc parameter
	unsigned char *eccP = NULL, *eccA = NULL, *eccB = NULL;
	unsigned char *eccGx = NULL, *eccGy = NULL, *eccOrder = NULL;
	long lRet = MYSM_ECCSetParam(
		ecclen,
		&eccP,
		&eccA,
		&eccB,
		&eccGx,
		&eccGy,
		&eccOrder);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	BN_CTX	 *ctx = NULL;
	BIGNUM 	 *p = NULL, *a = NULL, *b = NULL, *gx = NULL, *gy = NULL, *order = NULL;
	BIGNUM 	 *x = NULL, *y = NULL, *d = NULL;
	EC_POINT *g = NULL, *P = NULL;
	EC_GROUP *group = NULL;
	int		 len;
	long	 ret = MYSMERR_SUCCESS;
	unsigned char bin[128];
	unsigned char dgst[128], sign_r[128], sign_s[128];
	long dgstlen;

	if ((ctx = BN_CTX_new()) == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	//////////////////////////// ec group and public key ////////////////////////////
	// p, a, b
	if (!(p = BN_bin2bn(eccP, ecclen, NULL)) ||
		!(a = BN_bin2bn(eccA, ecclen, NULL)) ||
		!(b = BN_bin2bn(eccB, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// group
	if (!(group = EC_GROUP_new_curve_GFp(p, a, b, ctx)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// g
	if (!(g = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(gx = BN_bin2bn(eccGx, ecclen, NULL)) ||
		!(gy = BN_bin2bn(eccGy, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, g, gx, gy, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, g, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// order
	if (!(order = BN_bin2bn(eccOrder, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	if (!EC_GROUP_set_generator(group, g, order, BN_value_one()))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	//////////////////////////// calculate public key ////////////////////////////
	// private key
	if (!(d = BN_bin2bn(prvkey, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// 点P(x, y) = [d]G
	if (!(P = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_mul(group, P, d, NULL, NULL, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, P, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	if (!(x = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(y = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!EC_POINT_get_affine_coordinates_GFp(group,	P, x, y, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// output
	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(d, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(prvkey + ecclen - len, bin, len);

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(x, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(pubkey_x + ecclen - len, bin, len);

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(y, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(pubkey_y + ecclen - len, bin, len);

	// 椭圆曲线签名算法
	memset(dgst, 0, sizeof(dgst));
	dgstlen = MYSMLEN_SM3HASH;
	memset(dgst, '1', dgstlen);
	memset(sign_r, 0, sizeof(sign_r));
	memset(sign_s, 0, sizeof(sign_s));
	lRet = MYSM_ECCSign(
		dgst,
		dgstlen,
		sign_r,
		sign_s,
		prvkey,
		ecclen);
	if (lRet != 0)
	{
		ret = MYSMERR_ECCSIGN;
		goto err;
	}

	// 椭圆曲线验证签名算法
	lRet = MYSM_ECCVerifySign(
		dgst,
		dgstlen,
		sign_r,
		sign_s,
		pubkey_x,
		pubkey_y,
		ecclen);
	if (lRet != 0)
	{
		ret = MYSMERR_ECCVERIFYSIGN;
		goto err;
	}

err:
	if (ctx)
		BN_CTX_free(ctx);

	if (p)
		BN_free(p);
	if (a)
		BN_free(a);
	if (b)
		BN_free(b);
	if (gx)
		BN_free(gx);
	if (gy)
		BN_free(gy);
	if (order)
		BN_free(order);

	if (x)
		BN_free(x);
	if (y)
		BN_free(y);
	if (d)
		BN_free(d);

	if (g)
		EC_POINT_free(g);
	if (P)
		EC_POINT_free(P);

	if (group)
		EC_GROUP_free(group);

	return ret;
#endif
}

// 生成ECC密钥对
long MYSM_ECCGenKeyPair(
	unsigned char	*prvkey,
	unsigned char	*pubkey_x,
	unsigned char	*pubkey_y,
	long			ecclen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (prvkey == NULL ||
		pubkey_x == NULL || pubkey_y == NULL)
		return MYSMERR_PARAMETER;

	// ecc parameter
	unsigned char *eccP = NULL, *eccA = NULL, *eccB = NULL;
	unsigned char *eccGx = NULL, *eccGy = NULL, *eccOrder = NULL;
	long lRet = MYSM_ECCSetParam(
		ecclen,
		&eccP,
		&eccA,
		&eccB,
		&eccGx,
		&eccGy,
		&eccOrder);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	BN_CTX	 *ctx = NULL;
	BIGNUM 	 *p = NULL, *a = NULL, *b = NULL, *gx = NULL, *gy = NULL, *order = NULL;
	BIGNUM 	 *x = NULL, *y = NULL, *d = NULL, *top = NULL;
	EC_POINT *g = NULL, *P = NULL;
	EC_GROUP *group = NULL;
	int		 len;
	long	 ret = MYSMERR_SUCCESS;
	unsigned char bin[128];
	unsigned char dgst[128], sign_r[128], sign_s[128];
	long dgstlen;

	if ((ctx = BN_CTX_new()) == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	//////////////////////////// ec group and public key ////////////////////////////
	// p, a, b
	if (!(p = BN_bin2bn(eccP, ecclen, NULL)) ||
		!(a = BN_bin2bn(eccA, ecclen, NULL)) ||
		!(b = BN_bin2bn(eccB, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// group
	if (!(group = EC_GROUP_new_curve_GFp(p, a, b, ctx)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// g
	if (!(g = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(gx = BN_bin2bn(eccGx, ecclen, NULL)) ||
		!(gy = BN_bin2bn(eccGy, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, g, gx, gy, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, g, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// order
	if (!(order = BN_bin2bn(eccOrder, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	if (!EC_GROUP_set_generator(group, g, order, BN_value_one()))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	//////////////////////////// gen key pair ////////////////////////////
	// top = order - 1
	if (!(top = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!BN_usub(top, order, BN_value_one()))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// d ∈[1, order - 2]
	if (!(d = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	while (1)
	{
		// 0 <= d < order - 1
		if (!BN_rand_range(d, top))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		if (!BN_is_zero(d))
			break;
	}

	// 点P(x, y) = [d]G
	if (!(P = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_mul(group, P, d, NULL, NULL, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, P, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	if (!(x = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(y = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!EC_POINT_get_affine_coordinates_GFp(group,	P, x, y, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// output
	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(d, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(prvkey + ecclen - len, bin, len);

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(x, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(pubkey_x + ecclen - len, bin, len);

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(y, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(pubkey_y + ecclen - len, bin, len);

	// 椭圆曲线签名算法
	memset(dgst, 0, sizeof(dgst));
	dgstlen = MYSMLEN_SM3HASH;
	memset(dgst, '1', dgstlen);
	memset(sign_r, 0, sizeof(sign_r));
	memset(sign_s, 0, sizeof(sign_s));
	lRet = MYSM_ECCSign(
		dgst,
		dgstlen,
		sign_r,
		sign_s,
		prvkey,
		ecclen);
	if (lRet != 0)
	{
		ret = MYSMERR_ECCSIGN;
		goto err;
	}

	// 椭圆曲线验证签名算法
	lRet = MYSM_ECCVerifySign(
		dgst,
		dgstlen,
		sign_r,
		sign_s,
		pubkey_x,
		pubkey_y,
		ecclen);
	if (lRet != 0)
	{
		ret = MYSMERR_ECCVERIFYSIGN;
		goto err;
	}

err:
	if (ctx)
		BN_CTX_free(ctx);

	if (p)
		BN_free(p);
	if (a)
		BN_free(a);
	if (b)
		BN_free(b);
	if (gx)
		BN_free(gx);
	if (gy)
		BN_free(gy);
	if (order)
		BN_free(order);

	if (x)
		BN_free(x);
	if (y)
		BN_free(y);
	if (d)
		BN_free(d);
	if (top)
		BN_free(top);

	if (g)
		EC_POINT_free(g);
	if (P)
		EC_POINT_free(P);

	if (group)
		EC_GROUP_free(group);

	return ret;
#endif
}

// 预处理1
// Z = SM3(ENTL‖ID‖a‖b‖Gx‖Gy‖Px‖Py)
long MYSM_ECCGetZ(
	unsigned char	*userid,
	long			useridlen,
	unsigned char	*pubkey_x,
	unsigned char	*pubkey_y,
	unsigned char	*out,
	long			*outlen,
	long			ecclen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (userid == NULL || useridlen <= 0 ||
		pubkey_x == NULL || pubkey_y == NULL ||
		out == NULL || outlen == NULL)
		return MYSMERR_PARAMETER;

	// ecc parameter
	unsigned char *eccP = NULL, *eccA = NULL, *eccB = NULL;
	unsigned char *eccGx = NULL, *eccGy = NULL, *eccOrder = NULL;
	long lRet = MYSM_ECCSetParam(
		ecclen,
		&eccP,
		&eccA,
		&eccB,
		&eccGx,
		&eccGy,
		&eccOrder);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// init
	HANDLE hHandle = NULL;
	lRet = MYSM_SM3Init(&hHandle);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// update
	// ENTL
	unsigned char byTmp[128];
	memset(byTmp, 0, sizeof(byTmp));
	long lLen = useridlen * 8;
	byTmp[0] = (unsigned char)((lLen & 0x0000FF00) >> 8);
	byTmp[1] = (unsigned char)lLen;
	lRet = MYSM_SM3Update(hHandle, byTmp, 2);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// ID
	lRet = MYSM_SM3Update(hHandle, userid, useridlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// a
	lRet = MYSM_SM3Update(hHandle, eccA, ecclen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// b
	lRet = MYSM_SM3Update(hHandle, eccB, ecclen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// Gx
	lRet = MYSM_SM3Update(hHandle, eccGx, ecclen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// Gy
	lRet = MYSM_SM3Update(hHandle, eccGy, ecclen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// Px
	lRet = MYSM_SM3Update(hHandle, pubkey_x, ecclen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// Py
	lRet = MYSM_SM3Update(hHandle, pubkey_y, ecclen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// final
	lRet = MYSM_SM3Final(hHandle, out, outlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	return MYSMERR_SUCCESS;
#endif
}

// 预处理2
// H = SM3(Z‖M)
long MYSM_ECCGetH(
	unsigned char	*z,
	long			zlen,
	unsigned char	*in,
	long			inlen,
	unsigned char	*out,
	long			*outlen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (z == NULL || zlen <= 0 ||
		in == NULL || inlen <= 0 ||
		out == NULL || outlen == NULL)
		return MYSMERR_PARAMETER;

	// init
	HANDLE hHandle = NULL;
	long lRet = MYSM_SM3Init(&hHandle);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// update
	// Z
	lRet = MYSM_SM3Update(hHandle, z, zlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// M
	lRet = MYSM_SM3Update(hHandle, in, inlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	// final
	lRet = MYSM_SM3Final(hHandle, out, outlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	return MYSMERR_SUCCESS;
#endif
}

// 预处理2
// H = SM3(Z‖M)
long MYSM_ECCGetFileH(
	unsigned char	*z,
	long			zlen,
	char			*infile,
	unsigned char	*out,
	long			*outlen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (z == NULL || zlen <= 0 ||
		infile == NULL || strlen(infile) == 0 ||
		out == NULL || outlen == NULL)
		return MYSMERR_PARAMETER;

	FILE *f = fopen(infile, "rb");
	if (f == NULL)
		return MYSMERR_FILEOPERATION;
	fseek(f, 0, SEEK_END);
	long lLen = ftell(f);
	if (lLen == 0)
	{
		fclose(f);
		return MYSMERR_FILEOPERATION;
	}
	fseek(f, 0, SEEK_SET);

	// init
	HANDLE hHandle = NULL;
	long lRet = MYSM_SM3Init(&hHandle);
	if (lRet != MYSMERR_SUCCESS)
	{
		fclose(f);
		return lRet;
	}

	// update
	// Z
	lRet = MYSM_SM3Update(hHandle, z, zlen);
	if (lRet != MYSMERR_SUCCESS)
	{
		fclose(f);
		return lRet;
	}

	unsigned char byTmp[MYSMLEN_SM3HASH];
	memset(byTmp, 0, sizeof(byTmp));
	long lTmpLen;

	unsigned char byData[MYSMLEN_DATABLOCK];
	while (!feof(f))
	{
		memset(byData, 0, sizeof(byData));
		lLen = fread(byData, 1, MYSMLEN_DATABLOCK, f);
		if (ferror(f))
		{
			fclose(f);
			MYSM_SM3Final(hHandle, byTmp, &lTmpLen);
			return MYSMERR_FILEOPERATION;
		}

		// update
		// M
		if (lLen > 0)
		{
			lRet = MYSM_SM3Update(hHandle, byData, lLen);
			if (lRet != MYSMERR_SUCCESS)
			{
				fclose(f);
				return lRet;
			}
		}
	}
	fclose(f);

	// final
	lRet = MYSM_SM3Final(hHandle, out, outlen);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	return MYSMERR_SUCCESS;
#endif
}

// 椭圆曲线签名算法
long MYSM_ECCSign(
	unsigned char	*dgst,
	long			dgstlen,
	unsigned char	*sign_r,
	unsigned char	*sign_s,
	unsigned char	*prvkey,
	long			ecclen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (dgst == NULL || dgstlen <= 0 ||
		sign_r == NULL || sign_s == NULL ||
		prvkey == NULL)
		return MYSMERR_PARAMETER;

	// ecc parameter
	unsigned char *eccP = NULL, *eccA = NULL, *eccB = NULL;
	unsigned char *eccGx = NULL, *eccGy = NULL, *eccOrder = NULL;
	long lRet = MYSM_ECCSetParam(
		ecclen,
		&eccP,
		&eccA,
		&eccB,
		&eccGx,
		&eccGy,
		&eccOrder);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	BN_CTX	 *ctx = NULL;
	BIGNUM 	 *p = NULL, *a = NULL, *b = NULL, *gx = NULL, *gy = NULL, *order = NULL;
	BIGNUM 	 *d = NULL, *e = NULL, *k = NULL, *r = NULL, *s = NULL, *x1 = NULL, *y1 = NULL, *tmp = NULL, *tmp2 = NULL;
	EC_POINT *g = NULL, *P1 = NULL;
	EC_GROUP *group = NULL;
	long	 ret = MYSMERR_SUCCESS;
	unsigned char bin[128];
	int		 len;

	if ((ctx = BN_CTX_new()) == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	//////////////////////////// ec group and private key ////////////////////////////
	// p, a, b
	if (!(p = BN_bin2bn(eccP, ecclen, NULL)) ||
		!(a = BN_bin2bn(eccA, ecclen, NULL)) ||
		!(b = BN_bin2bn(eccB, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// group
	if (!(group = EC_GROUP_new_curve_GFp(p, a, b, ctx)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// g
	if (!(g = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(gx = BN_bin2bn(eccGx, ecclen, NULL)) ||
		!(gy = BN_bin2bn(eccGy, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, g, gx, gy, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, g, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// order
	if (!(order = BN_bin2bn(eccOrder, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	if (!EC_GROUP_set_generator(group, g, order, BN_value_one()))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// private key
	if (!(d = BN_bin2bn(prvkey, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	//////////////////////////// sign ////////////////////////////
	// e
	if (!(e = BN_bin2bn(dgst, dgstlen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	
	if (!(k = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(r = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(s = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	if (!(P1 = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(x1 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(y1 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	if (!(tmp = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(tmp2 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	while (1)
	{
		// k ∈[1, order - 1]
		while (1)
		{
			// 0 <= k < order
			if (!BN_rand_range(k, order))
			{
				ret = MYSMERR_BNOPERATION;
				goto err;
			}
			if (!BN_is_zero(k))
				break;
		}
/*		// used for test
		if (k)
		{
			BN_free(k);
			k = NULL;
		}
		if (!(k = BN_bin2bn((unsigned char *)"\
\x6C\xB2\x8D\x99\x38\x5C\x17\x5C\x94\xF9\x4E\x93\x48\x17\x66\x3F\
\xC1\x76\xD9\x25\xDD\x72\xB7\x27\x26\x0D\xBA\xAE\x1F\xB2\xF9\x6F", ecclen, NULL)))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
*/
		// 点P1(x1, y1) = [k]G
		if (!EC_POINT_mul(group, P1, k, NULL, NULL, ctx))
		{
			ret = MYSMERR_ECCOPERATION;
			goto err;
		}
		if (!EC_POINT_is_on_curve(group, P1, ctx))
		{
			ret = MYSMERR_ECCOPERATION;
			goto err;
		}
		if (!EC_POINT_get_affine_coordinates_GFp(group,	P1, x1, y1, ctx))
		{
			ret = MYSMERR_ECCOPERATION;
			goto err;
		}

		// r = (e + x1) mod order
		if (!BN_mod_add(r, e, x1, order, ctx))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		// 若 r = 0 或 r + k = order 则返回循环
		if (BN_is_zero(r))
			continue;
		if (!BN_uadd(tmp, r, k))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		if (BN_ucmp(tmp, order) == 0)
			continue;

		// tmp = (k - r * dA) mod order
		if (!BN_mod_mul(s, r, d, order, ctx))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		if (!BN_mod_sub(tmp, k, s, order, ctx))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}

		// tmp2 = (1 / (1 + dA)) mod order
		if (!BN_mod_add(s, BN_value_one(), d, order, ctx))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		if (!BN_mod_inverse(tmp2, s, order, ctx))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}

		// s = ((k - r * d) / (1 + d)) mod order
		if (!BN_mod_mul(s, tmp, tmp2, order, ctx))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		// 若 s = 0 则返回循环
		if (BN_is_zero(s))
			continue;

		break;
	}

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(r, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(sign_r + ecclen - len, bin, len);

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(s, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(sign_s + ecclen - len, bin, len);

err:
	if (ctx)
		BN_CTX_free(ctx);
	if (p)
		BN_free(p);
	if (a)
		BN_free(a);
	if (b)
		BN_free(b);
	if (gx)
		BN_free(gx);
	if (gy)
		BN_free(gy);
	if (order)
		BN_free(order);
	if (d)
		BN_free(d);
	if (e)
		BN_free(e);
	if (k)
		BN_free(k);
	if (r)
		BN_free(r);
	if (s)
		BN_free(s);
	if (x1)
		BN_free(x1);
	if (y1)
		BN_free(y1);
	if (tmp)
		BN_free(tmp);
	if (tmp2)
		BN_free(tmp2);
	if (g)
		EC_POINT_free(g);
	if (P1)
		EC_POINT_free(P1);
	if (group)
		EC_GROUP_free(group);

	return ret;
#endif
}

// 椭圆曲线验证签名算法
long MYSM_ECCVerifySign(
	unsigned char	*dgst,
	long			dgstlen,
	unsigned char	*sign_r,
	unsigned char	*sign_s,
	unsigned char	*pubkey_x,
	unsigned char	*pubkey_y,
	long			ecclen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (dgst == NULL || dgstlen <= 0 ||
		sign_r == NULL || sign_s == NULL ||
		pubkey_x == NULL || pubkey_y == NULL)
		return MYSMERR_PARAMETER;

	// ecc parameter
	unsigned char *eccP = NULL, *eccA = NULL, *eccB = NULL;
	unsigned char *eccGx = NULL, *eccGy = NULL, *eccOrder = NULL;
	long lRet = MYSM_ECCSetParam(
		ecclen,
		&eccP,
		&eccA,
		&eccB,
		&eccGx,
		&eccGy,
		&eccOrder);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	BN_CTX	 *ctx = NULL;
	BIGNUM 	 *p = NULL, *a = NULL, *b = NULL, *gx = NULL, *gy = NULL, *order = NULL;
	BIGNUM 	 *e = NULL, *r = NULL, *s = NULL, *t = NULL, *R = NULL, *x = NULL, *y = NULL, *x1 = NULL, *y1 = NULL;
	EC_POINT *g = NULL, *pubkey = NULL, *P1 = NULL;
	EC_GROUP *group = NULL;
	long	 ret = MYSMERR_SUCCESS;

	// r', s'
	if (!(r = BN_bin2bn(sign_r, ecclen, NULL)) ||
		!(s = BN_bin2bn(sign_s, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// order
	if (!(order = BN_bin2bn(eccOrder, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// r ∈[1, order - 1] ?
	if (BN_is_zero(r) ||
		BN_ucmp(r, order) >= 0)
	{
		ret = MYSMERR_ECCVERIFYSIGN;
		goto err;
	}
	// s ∈[1, order - 1] ?
	if (BN_is_zero(s) ||
		BN_ucmp(s, order) >= 0)
	{
		ret = MYSMERR_ECCVERIFYSIGN;
		goto err;
	}

	if ((ctx = BN_CTX_new()) == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	//////////////////////////// ec group and public key ////////////////////////////
	// p, a, b
	if (!(p = BN_bin2bn(eccP, ecclen, NULL)) ||
		!(a = BN_bin2bn(eccA, ecclen, NULL)) ||
		!(b = BN_bin2bn(eccB, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// group
	if (!(group = EC_GROUP_new_curve_GFp(p, a, b, ctx)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// g
	if (!(g = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(gx = BN_bin2bn(eccGx, ecclen, NULL)) ||
		!(gy = BN_bin2bn(eccGy, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, g, gx, gy, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, g, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	if (!EC_GROUP_set_generator(group, g, order, BN_value_one()))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// public key point
	if (!(pubkey = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(x = BN_bin2bn(pubkey_x, ecclen, NULL)) ||
		!(y = BN_bin2bn(pubkey_y, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, pubkey, x, y, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, pubkey, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	//////////////////////////// verify ////////////////////////////
	// t = (r' + s') mod order
	if (!(t = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!BN_mod_add(t, r, s, order, ctx))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	// t = 0 ?
	if (BN_is_zero(t))
	{
		ret = MYSMERR_ECCVERIFYSIGN;
		goto err;
	}

	// 点P1(x1', y1') = [s']G + [t]PA
	if (!(P1 = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
/** Computes r = g * n + q * m
 *  \param  group  underlying EC_GROUP object
 *  \param  r      EC_POINT object for the result
 *  \param  n      BIGNUM with the multiplier for the group g (optional)
 *  \param  q      EC_POINT object with the first factor of the second summand
 *  \param  m      BIGNUM with the second factor of the second summand
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occured
 */	if (!EC_POINT_mul(group, P1, s, pubkey, t, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, P1, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	if (!(x1 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(y1 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!EC_POINT_get_affine_coordinates_GFp(group, P1, x1, y1, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// e'
	if (!(e = BN_bin2bn(dgst, dgstlen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// R = (e' + x1') mod order
	if (!(R = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!BN_mod_add(R, e, x1, order, ctx))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	// R = r' ?
	if (BN_ucmp(R, r) != 0)
	{
		ret = MYSMERR_ECCVERIFYSIGN;
		goto err;
	}

err:
	if (ctx)
		BN_CTX_free(ctx);
	if (p)
		BN_free(p);
	if (a)
		BN_free(a);
	if (b)
		BN_free(b);
	if (gx)
		BN_free(gx);
	if (gy)
		BN_free(gy);
	if (order)
		BN_free(order);
	if (e)
		BN_free(e);
	if (r)
		BN_free(r);
	if (s)
		BN_free(s);
	if (t)
		BN_free(t);
	if (R)
		BN_free(R);
	if (x)
		BN_free(x);
	if (y)
		BN_free(y);
	if (x1)
		BN_free(x1);
	if (y1)
		BN_free(y1);
	if (g)
		EC_POINT_free(g);
	if (pubkey)
		EC_POINT_free(pubkey);
	if (P1)
		EC_POINT_free(P1);
	if (group)
		EC_GROUP_free(group);

	return ret;
#endif
}

// 椭圆曲线加密算法
long MYSM_ECCEncrypt(
	unsigned char	*in,
	long			inlen,
	unsigned char	*out_c1,
	unsigned char	*out_c2,
	unsigned char	*out_c3,
	unsigned char	*pubkey_x,
	unsigned char	*pubkey_y,
	long			ecclen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (in == NULL || inlen <= 0 ||
		out_c1 == NULL || out_c2 == NULL || out_c3 == NULL ||
		pubkey_x == NULL || pubkey_y == NULL)
		return MYSMERR_PARAMETER;

	// ecc parameter
	unsigned char *eccP = NULL, *eccA = NULL, *eccB = NULL;
	unsigned char *eccGx = NULL, *eccGy = NULL, *eccOrder = NULL;
	long lRet = MYSM_ECCSetParam(
		ecclen,
		&eccP,
		&eccA,
		&eccB,
		&eccGx,
		&eccGy,
		&eccOrder);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	BN_CTX	 *ctx = NULL;
	BIGNUM 	 *p = NULL, *a = NULL, *b = NULL, *gx = NULL, *gy = NULL, *order = NULL;
	BIGNUM 	 *k = NULL, *x = NULL, *y = NULL, *x1 = NULL, *y1 = NULL, *x2 = NULL, *y2 = NULL;
	EC_POINT *g = NULL, *pubkey = NULL, *S = NULL, *C1 = NULL, *P2 = NULL;
	EC_GROUP *group = NULL;
	long	 ret = MYSMERR_SUCCESS, i, lTmp;
	unsigned char *t = NULL, x2y2[256];
	unsigned char bin[128];
	int		 len;

	if ((ctx = BN_CTX_new()) == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	//////////////////////////// ec group and public key ////////////////////////////
	// p, a, b
	if (!(p = BN_bin2bn(eccP, ecclen, NULL)) ||
		!(a = BN_bin2bn(eccA, ecclen, NULL)) ||
		!(b = BN_bin2bn(eccB, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// group
	if (!(group = EC_GROUP_new_curve_GFp(p, a, b, ctx)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// g
	if (!(g = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(gx = BN_bin2bn(eccGx, ecclen, NULL)) ||
		!(gy = BN_bin2bn(eccGy, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, g, gx, gy, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, g, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// order
	if (!(order = BN_bin2bn(eccOrder, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	if (!EC_GROUP_set_generator(group, g, order, BN_value_one()))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// public key point
	if (!(pubkey = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(x = BN_bin2bn(pubkey_x, ecclen, NULL)) ||
		!(y = BN_bin2bn(pubkey_y, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, pubkey, x, y, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, pubkey, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// 点S = [h]PB
	// 推荐h = 1
	if (!(S = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_mul(group, S, NULL, pubkey, BN_value_one(), ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	// S = 0(无穷远点) ?
	if (EC_POINT_is_at_infinity(group, S))
	{
		ret = MYSMERR_ECCENCRYPT;
		goto err;
	}

	//////////////////////////// encrypt ////////////////////////////
	if (!(k = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	if (!(P2 = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(x2 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(y2 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	t = new unsigned char [inlen + ecclen * 2];
	if (t == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	while (1)
	{
		// k ∈[1, order - 1]
		while (1)
		{
			// 0 <= k < order
			if (!BN_rand_range(k, order))
			{
				ret = MYSMERR_BNOPERATION;
				goto err;
			}
			if (!BN_is_zero(k))
				break;
		}
/*		// used for test
		if (k)
		{
			BN_free(k);
			k = NULL;
		}
		if (!(k = BN_bin2bn((unsigned char *)"\
\x4C\x62\xEE\xFD\x6E\xCF\xC2\xB9\x5B\x92\xFD\x6C\x3D\x95\x75\x14\
\x8A\xFA\x17\x42\x55\x46\xD4\x90\x18\xE5\x38\x8D\x49\xDD\x7B\x4F", ecclen, NULL)))
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
*/
		// 点P2(x2, y2) = [k]PB
		if (!EC_POINT_mul(group, P2, NULL, pubkey, k, ctx))
		{
			ret = MYSMERR_ECCOPERATION;
			goto err;
		}
		if (!EC_POINT_is_on_curve(group, P2, ctx))
		{
			ret = MYSMERR_ECCOPERATION;
			goto err;
		}
		if (!EC_POINT_get_affine_coordinates_GFp(group,	P2, x2, y2, ctx))
		{
			ret = MYSMERR_ECCOPERATION;
			goto err;
		}

		memset(x2y2, 0, sizeof(x2y2));

		memset(bin, 0, sizeof(bin));
		len = BN_bn2bin(x2, bin);
		if (len == 0 || len > ecclen)
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		memcpy(x2y2 + ecclen - len, bin, len);

		memset(bin, 0, sizeof(bin));
		len = BN_bn2bin(y2, bin);
		if (len == 0 || len > ecclen)
		{
			ret = MYSMERR_BNOPERATION;
			goto err;
		}
		memcpy(x2y2 + ecclen * 2 - len, bin, len);

		// t = KDF(x2 || y2, klen)
		memset(t, 0, inlen + ecclen * 2);
		lRet = MYSM_KDF(
			x2y2,
			ecclen * 2,
			t,
			inlen);
		if (lRet != MYSMERR_SUCCESS)
		{
			ret = lRet;
			goto err;
		}

		for (i = 0; i < inlen; i++)
		{
			if (t[i] != 0)
				break;
		}
		if (i != inlen)
			break;
	}

	// 点C1(x1, y1) = [k]G
	if (!(C1 = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_mul(group, C1, k, NULL, NULL, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, C1, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	if (!(x1 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(y1 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!EC_POINT_get_affine_coordinates_GFp(group,	C1, x1, y1, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

//	out_c1[0] = 04;

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(x1, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
//	memcpy(out_c1 + 1 + ecclen - len, bin, len);
	memcpy(out_c1 + ecclen - len, bin, len);

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(y1, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
//	memcpy(out_c1 + 1 + ecclen * 2 - len, bin, len);
	memcpy(out_c1 + ecclen * 2 - len, bin, len);

	// C2 = M ⊕ t
	for (i = 0; i < inlen; i++)
		out_c2[i] = in[i] ^ t[i];

	// C3 = Hash(x2 || M || y2)
	memset(t, 0, inlen + ecclen * 2);
	memcpy(t, x2y2, ecclen);
	memcpy(t + ecclen, in, inlen);
	memcpy(t + ecclen + inlen, x2y2 + ecclen, ecclen);
	lTmp = MYSMLEN_SM3HASH;
	lRet = MYSM_SM3(
		t,
		inlen + ecclen * 2,
		out_c3,
		&lTmp);
	if (lRet != MYSMERR_SUCCESS)
	{
		ret = lRet;
		goto err;
	}

err:
	if (ret != MYSMERR_SUCCESS)
	{
//		memset(out_c1, 0, 1 + ecclen * 2);
		memset(out_c1, 0, ecclen * 2);
		memset(out_c2, 0, inlen);
		memset(out_c3, 0, MYSMLEN_SM3HASH);
	}
	if (t)
		delete [] t;
	if (ctx)
		BN_CTX_free(ctx);
	if (p)
		BN_free(p);
	if (a)
		BN_free(a);
	if (b)
		BN_free(b);
	if (gx)
		BN_free(gx);
	if (gy)
		BN_free(gy);
	if (order)
		BN_free(order);
	if (k)
		BN_free(k);
	if (x)
		BN_free(x);
	if (y)
		BN_free(y);
	if (x1)
		BN_free(x1);
	if (y1)
		BN_free(y1);
	if (x2)
		BN_free(x2);
	if (y2)
		BN_free(y2);
	if (g)
		EC_POINT_free(g);
	if (pubkey)
		EC_POINT_free(pubkey);
	if (S)
		EC_POINT_free(S);
	if (C1)
		EC_POINT_free(C1);
	if (P2)
		EC_POINT_free(P2);
	if (group)
		EC_GROUP_free(group);

	return ret;
#endif
}

// 椭圆曲线解密算法
long MYSM_ECCDecrypt(
	unsigned char	*in_c1,
	unsigned char	*in_c2,
	long			inlen_c2,
	unsigned char	*in_c3,
	unsigned char	*out,
	long			*outlen,
	unsigned char	*prvkey,
	long			ecclen)
{
#ifdef _NOTUSEOPENSSL
	return MYSMERR_NOTSUPPORT;
#else
	if (in_c1 == NULL || in_c3 == NULL ||
		in_c2 == NULL || inlen_c2 <= 0 ||
		out == NULL || outlen == NULL ||
		prvkey == NULL)
		return MYSMERR_PARAMETER;

	// ecc parameter
	unsigned char *eccP = NULL, *eccA = NULL, *eccB = NULL;
	unsigned char *eccGx = NULL, *eccGy = NULL, *eccOrder = NULL;
	long lRet = MYSM_ECCSetParam(
		ecclen,
		&eccP,
		&eccA,
		&eccB,
		&eccGx,
		&eccGy,
		&eccOrder);
	if (lRet != MYSMERR_SUCCESS)
		return lRet;

	BN_CTX	 *ctx = NULL;
	BIGNUM 	 *p = NULL, *a = NULL, *b = NULL, *gx = NULL, *gy = NULL, *order = NULL;
	BIGNUM 	 *d = NULL, *x1 = NULL, *y1 = NULL, *x2 = NULL, *y2 = NULL;
	EC_POINT *g = NULL, *S = NULL, *C1 = NULL, *P2 = NULL;
	EC_GROUP *group = NULL;
	long	 ret = MYSMERR_SUCCESS, i, lTmp;
	unsigned char *t = NULL, x2y2[256], u[MYSMLEN_SM3HASH];
	unsigned char bin[128];
	int		 len;

	if ((ctx = BN_CTX_new()) == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	//////////////////////////// ec group and private key ////////////////////////////
	// p, a, b
	if (!(p = BN_bin2bn(eccP, ecclen, NULL)) ||
		!(a = BN_bin2bn(eccA, ecclen, NULL)) ||
		!(b = BN_bin2bn(eccB, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	// group
	if (!(group = EC_GROUP_new_curve_GFp(p, a, b, ctx)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// g
	if (!(g = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!(gx = BN_bin2bn(eccGx, ecclen, NULL)) ||
		!(gy = BN_bin2bn(eccGy, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, g, gx, gy, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, g, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// order
	if (!(order = BN_bin2bn(eccOrder, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	if (!EC_GROUP_set_generator(group, g, order, BN_value_one()))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	// private key
	if (!(d = BN_bin2bn(prvkey, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}

	//////////////////////////// decrypt ////////////////////////////
	// C1
	if (!(C1 = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
//	if (!(x1 = BN_bin2bn(in_c1 + 1, ecclen, NULL)) ||
//		!(y1 = BN_bin2bn(in_c1 + 1 + ecclen, ecclen, NULL)))
	if (!(x1 = BN_bin2bn(in_c1, ecclen, NULL)) ||
		!(y1 = BN_bin2bn(in_c1 + ecclen, ecclen, NULL)))
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, C1, x1, y1, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, C1, ctx))
	{
		ret = MYSMERR_ECCDECRYPT;
		goto err;
	}

	// 点S = [h]C1
	// 推荐h = 1
	if (!(S = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_mul(group, S, NULL, C1, BN_value_one(), ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	// S = 0(无穷远点) ?
	if (EC_POINT_is_at_infinity(group, S))
	{
		ret = MYSMERR_ECCDECRYPT;
		goto err;
	}

	// P2(x2, y2) = [dB]C1
	if (!(P2 = EC_POINT_new(group)))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_mul(group, P2, NULL, C1, d, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}
	if (!EC_POINT_is_on_curve(group, P2, ctx))
	{
		ret = MYSMERR_ECCDECRYPT;
		goto err;
	}

	if (!(x2 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!(y2 = BN_new()))
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}
	if (!EC_POINT_get_affine_coordinates_GFp(group,	P2, x2, y2, ctx))
	{
		ret = MYSMERR_ECCOPERATION;
		goto err;
	}

	memset(x2y2, 0, sizeof(x2y2));

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(x2, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(x2y2 + ecclen - len, bin, len);

	memset(bin, 0, sizeof(bin));
	len = BN_bn2bin(y2, bin);
	if (len == 0 || len > ecclen)
	{
		ret = MYSMERR_BNOPERATION;
		goto err;
	}
	memcpy(x2y2 + ecclen * 2 - len, bin, len);

	// t = KDF(x2 || y2, klen)
	t = new unsigned char [inlen_c2 + ecclen * 2];
	if (t == NULL)
	{
		ret = MYSMERR_MEMORY;
		goto err;
	}

	memset(t, 0, inlen_c2 + ecclen * 2);
	lRet = MYSM_KDF(
		x2y2,
		ecclen * 2,
		t,
		inlen_c2);
	if (lRet != MYSMERR_SUCCESS)
	{
		ret = lRet;
		goto err;
	}

	for (i = 0; i < inlen_c2; i++)
	{
		if (t[i] != 0)
			break;
	}
	if (i == inlen_c2)
	{
		ret = MYSMERR_ECCDECRYPT;
		goto err;
	}

	// M = C2 ⊕ t
	for (i = 0; i < inlen_c2; i++)
		out[i] = in_c2[i] ^ t[i];

	// u = Hash(x2 || M || y2)
	memset(t, 0, inlen_c2 + ecclen * 2);
	memcpy(t, x2y2, ecclen);
	memcpy(t + ecclen, out, inlen_c2);
	memcpy(t + ecclen + inlen_c2, x2y2 + ecclen, ecclen);
	memset(u, 0, sizeof(u));
	lTmp = MYSMLEN_SM3HASH;
	lRet = MYSM_SM3(
		t,
		inlen_c2 + ecclen * 2,
		u,
		&lTmp);
	if (lRet != MYSMERR_SUCCESS)
	{
		ret = lRet;
		goto err;
	}

	// u = C3 ?
	if (memcmp(u, in_c3, MYSMLEN_SM3HASH) != 0)
	{
		ret = MYSMERR_ECCDECRYPT;
		goto err;
	}

	*outlen = inlen_c2;

err:
	if (ret != MYSMERR_SUCCESS)
		memset(out, 0, inlen_c2);
	if (t)
		delete [] t;
	if (ctx)
		BN_CTX_free(ctx);
	if (p)
		BN_free(p);
	if (a)
		BN_free(a);
	if (b)
		BN_free(b);
	if (gx)
		BN_free(gx);
	if (gy)
		BN_free(gy);
	if (order)
		BN_free(order);
	if (d)
		BN_free(d);
	if (x1)
		BN_free(x1);
	if (y1)
		BN_free(y1);
	if (x2)
		BN_free(x2);
	if (y2)
		BN_free(y2);
	if (g)
		EC_POINT_free(g);
	if (S)
		EC_POINT_free(S);
	if (C1)
		EC_POINT_free(C1);
	if (P2)
		EC_POINT_free(P2);
	if (group)
		EC_GROUP_free(group);

	return ret;
#endif
}
