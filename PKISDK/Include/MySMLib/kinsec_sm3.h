/************************************************************************
FileName:
SM3.h
Version:
SM3_V1.1
Date:
Sep 18,2016
Description:
This headfile provide macro defination, parameter definition
and function declaration needed in SM3 algorithm implement
Function List:
1.KINSEC_SM3_256 //calls SM3_init, SM3_process and SM3_done to calculate hash value
2.KINSEC_SM3_init //init the SM3 state
3.KINSEC_SM3_process //compress the the first len/64 blocks of the message
4.KINSEC_SM3_done //compress the rest message and output the hash value
5.KINSEC_SM3_compress //called by SM3_process and SM3_done, compress a single block of message
6.KINSEC_BiToW //called by SM3_compress,to calculate W from Bi
7.KINSEC_WToW1 //called by SM3_compress, calculate W' from W
8.KINSEC_CF //called by SM3_compress, to calculate KINSEC_CF function.
9.KINSEC_BigEndian //called by SM3_compress and SM3_done.GM/T 0004-2012 requires to use
big-endian.
//if CPU uses little-endian, BigEndian function is a necessary call to
change the
//little-endian format into big-endian format.
10.SM3_SelfTest //test whether the SM3 calculation is correct by comparing the hash result
with the standard data
History:
1. Date: Sep 18,2016
Author: Mao Yingying, Huo Lili
Modification: 1)add notes to all the functions
2)add SM3_SelfTest function
************************************************************************/
#include <string.h>
#define KINSEC_SM3_len 256
#define KINSEC_SM3_T1 0x79CC4519
#define KINSEC_SM3_T2 0x7A879D8A
#define KINSEC_SM3_IVA 0x7380166f
#define KINSEC_SM3_IVB 0x4914b2b9
#define KINSEC_SM3_IVC 0x172442d7
#define KINSEC_SM3_IVD 0xda8a0600
#define KINSEC_SM3_IVE 0xa96f30bc
#define KINSEC_SM3_IVF 0x163138aa
#define KINSEC_SM3_IVG 0xe38dee4d
#define KINSEC_SM3_IVH 0xb0fb0e4e
/* Various logical functions */
#define KINSEC_SM3_p1(x) (x^KINSEC_SM3_rotl32(x,15)^KINSEC_SM3_rotl32(x,23))
#define KINSEC_SM3_p0(x) (x^KINSEC_SM3_rotl32(x,9)^KINSEC_SM3_rotl32(x,17))
#define KINSEC_SM3_ff0(a,b,c) (a^b^c)
#define KINSEC_SM3_ff1(a,b,c) ((a&b)|(a&c)|(b&c))
#define KINSEC_SM3_gg0(e,f,g) (e^f^g)
#define KINSEC_SM3_gg1(e,f,g) ((e&f)|((~e)&g))
#define KINSEC_SM3_rotl32(x,n) ((((unsigned int) x) << n) | (((unsigned int) x) >> (32 - n)))
#define KINSEC_SM3_rotr32(x,n) ((((unsigned int) x) >> n) | (((unsigned int) x) << (32 - n)))
typedef struct {
	unsigned int state[8];
	unsigned int length;
	unsigned int curlen;
	unsigned char buf[64];
} KINSEC_SM3_STATE;
void KINSEC_BiToWj(unsigned int Bi[], unsigned int Wj[]);
void KINSEC_WjToWj1(unsigned int Wj[], unsigned int Wj1[]);
void KINSEC_CF(unsigned int Wj[], unsigned int Wj1[], unsigned int V[]);
void KINSEC_BigEndian(unsigned char src[], unsigned int bytelen, unsigned char des[]);
void KINSEC_SM3_init(KINSEC_SM3_STATE *md);
void KINSEC_SM3_compress(KINSEC_SM3_STATE * md);
void KINSEC_SM3_process(KINSEC_SM3_STATE * md, unsigned char buf[], int len);
void KINSEC_SM3_done(KINSEC_SM3_STATE *md, unsigned char *hash);
void KINSEC_SM3_256(unsigned char buf[], int len, unsigned char hash[]);
int KINSEC_SM3_SelfTest();