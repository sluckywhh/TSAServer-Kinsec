#ifndef _KT_TYPE_H_
#define _KT_TYPE_H_

/**
  *@Name: KTType.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/03/24 14:23
  *@Description: all Kind Type 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include <limits.h>

#ifndef IN
    #define IN
#endif
#ifndef OUT
    #define OUT
#endif

#ifndef TRUE
	#define TRUE	1
#endif
#ifndef FALSE
	#define FALSE	0
#endif

#ifdef KT_UNICODE
  #define _KT(x)      L ## x
#else
  #define _KT(x)      x
#endif

/* KTINT8, KTUINT8 */
#if UCHAR_MAX == 0xff
	typedef signed char		KTINT8;
    typedef unsigned char   KTUINT8;
	#define KTINT8_MAX		SCHAR_MAX
	#define KTUINT8_MAX		UCHAR_MAX
	typedef KTUINT8			KTBYTE;	

#endif

/* KTINT16, KTUINT16 */
#if USHRT_MAX == 0xffff
	typedef short			KTINT16;
	typedef unsigned short  KTUINT16;
	#define KTINT16_MAX	    SHRT_MAX
	#define KTUINT16_MAX	USHRT_MAX

#elif UINT_MAX == 0xffff
	typedef int				KTINT16;
	typedef unsigned int	KTUINT16;
	#define KTINT16_MAX		INT_MAX
	#define KTUINT16_MAX	UINT_MAX

#endif


/* KTINT32, KTUINT32 */
#if USHRT_MAX == 0xfffffffful
	typedef short			  KTINT32;
	typedef unsigned short	  KTUINT32;
	#define KTINT32_MAX		  SHRT_MAX
	#define KTUINT32_MAX	  USHRT_MAX

#elif UINT_MAX == 0xfffffffful
	typedef int				KTINT32;
	typedef unsigned int	KTUINT32;
	#define KTINT32_MAX		INT_MAX
	#define KTUINT32_MAX	UINT_MAX

#elif ULONG_MAX == 0xfffffffful
	typedef long			KTINT32;
	typedef unsigned long	KTUINT32;
	#define KTINT32_MAX		LONG_MAX
	#define KTUINT32_MAX	ULONG_MAX

#endif

#define KTWORD			KTUINT16
#define KTDWORD			KTUINT32		  
typedef unsigned char	KTBOOL;	
typedef int				KTBOOL32;	
typedef char			KTCHAR;
typedef int 			KTINT;
typedef unsigned int	KTUINT;
typedef long			KTLONG;
typedef unsigned long	KTULONG;
typedef KTUINT		    KTHANDLE;
typedef KTBYTE          KTByte;
typedef KTCHAR          KTChar;

#ifndef BYTE
	#define BYTE KTBYTE
#endif
#ifndef UINT
	#define UINT KTUINT
#endif

//define algoritm info struct
//define algorithm type.
const KTBYTE	KTA_SYMMETRIC_TYPE		=1;
const KTBYTE	KTA_UNSYMMETRIC_TYPE	=2;	
const KTBYTE	KTA_DIGEST_TYPE			=3;			
typedef struct KT_ALGORITHM_INFO
{
  KTCHAR    szName[32];
  KTCHAR    szDescription[64];
  KTULONG	unVersion;
  KTBYTE	unType;
  KTUINT	unPacketLen;
  KTBYTE	pbReserved[64];
}KT_ALGORITHM_INFO, *KT_ALGORITHM_INFO_PTR;
	
#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/03/25 11:13  Hongwei Yu   created 

#endif
