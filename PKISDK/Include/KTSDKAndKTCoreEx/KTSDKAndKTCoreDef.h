#ifndef _KTSDKANDKTCORE_DEF_H
#define _KTSDKANDKTCORE_DEF_H

#ifndef _IBM_AIX
#pragma pack(push, 1)
#else
#pragma pack(1)
#pragma options align=packed
#endif /*_IBM_AIX*/

#define MAX_FILEPATH		260
#define MAX_SHOW_SIGN_NUM   10
#define MAX_SIGNER_NUM      10

typedef struct _KTSDKANDKTCORE_SIGNER_INFO
{
	//unsigned int unSize;//support TSS
	//int nFlags;//support TSS,reference enumTimeTypeFlags; 
	struct tm	tmGMT;
//	KTHANDLE	hCertHandle;
	KTByteArray	rCert;
	int			nCertStatus;
}KTSDKANDKTCORE_SIGNER_INFO;

typedef struct _KTSDKANDKTCORE_SIGN_INFO
{
	char            pc_signFileNm[MAX_FILEPATH];
	int             nSigner;			//传入时指可以容纳的KT_SIGNER_INFO的个数
	KTSDKANDKTCORE_SIGNER_INFO  pSignerInfo[MAX_SIGNER_NUM];

}KTSDKANDKTCORE_SIGN_INFO;  

#ifndef _IBM_AIX
#pragma pack(pop)
#else
#pragma pack()
#pragma options align=reset
#endif

#endif
