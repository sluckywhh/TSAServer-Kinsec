/*
FILENAME :             MD5.H 
ESTABLISHED DATE:      1998/9/9
*/

/*COPYRIGHT (C) 1998-2000.STATE KEY LIBARY OF INFORMATION SECURITY. 
ALL RIGHTS RESERVED.
*/ 

/* Description: This is the header file for md5.c  
*/

/*MD5 context */


//#include "basic_type.h"

#ifndef  _MDX_SHA1_
#define  _MDX_SHA1_
typedef unsigned long       UINT4; 
typedef unsigned int        UINT;

typedef struct
{
	UINT4 state[4];              
	UINT4 count[2];
	BYTE  buffer[64];
}MD5_CTX;

void MD5_Init( MD5_CTX * );
void MD5_Update( MD5_CTX*,const BYTE *,UINT );
void MD5_Final( BYTE[16],MD5_CTX* );
void MD5_All( BYTE[16],const BYTE *,UINT );

/* SHA-1 context */

typedef struct
{
	UINT4 state[5];
	UINT4 count[2];
	BYTE buffer[64];
}SHA1_CTX;

void SHA1_Init( SHA1_CTX * );
void SHA1_Update( SHA1_CTX *,const BYTE *,UINT );
void SHA1_Final( BYTE [20], SHA1_CTX * );
void SHA1_All( BYTE[20],const BYTE *,UINT );

#endif
 