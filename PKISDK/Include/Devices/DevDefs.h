#pragma once

#pragma pack(push,1)

#define SGD_SM1_ECB		0x00000101	//SM1算法ECB加密模式
#define SGD_SM1_CBC		0x00000102	//SM1算法CBC加密模式
#define SGD_SM1_CFB		0x00000104	//SM1算法CFB加密模式
#define SGD_SM1_OFB		0x00000108	//SM1算法OFB加密模式
#define SGD_SM1_MAC		0x00000110	//SM1算法MAC运算
#define SGD_SSF33_ECB	0x00000201	//SSF33算法ECB加密模式
#define SGD_SSF33_CBC	0x00000202	//SSF33算法CBC加密模式
#define SGD_SSF33_CFB	0x00000204	//SSF33算法CFB加密模式
#define SGD_SSF33_OFB	0x00000208	//SSF33算法OFB加密模式
#define SGD_SSF33_MAC	0x00000210	//SSF33算法MAC运算
#define SGD_SMS4_ECB	0x00000401	//SMS4算法ECB加密模式
#define SGD_SMS4_CBC	0x00000402	//SMS4算法CBC加密模式
#define SGD_SMS4_CFB	0x00000404	//SMS4算法CFB加密模式
#define SGD_SMS4_OFB	0x00000408	//SMS4算法OFB加密模式
#define SGD_SMS4_MAC	0x00000410	//SMS4算法MAC运算
#define SGD_SM4_ECB		0x00000401	// SM4算法ECB加密模式
#define SGD_SM4_CBC		0x00000402	// SM4算法CBC加密模式
#define SGD_SM4_CFB		0x00000404	// SM4算法CFB加密模式
#define SGD_SM4_OFB		0x00000408	// SM4算法OFB加密模式
#define SGD_SM4_MAC		0x00000410	// SM4算法MAC加密模式
#define SGD_ZUC_EEA3	0x00000801	// ZUC祖冲之机密性算法128-EEA3算法
#define SGD_ZUC_EIA3	0x00000802	// ZUC祖冲之完整性算法128-EIA3算法
/*	0x00000400-0x800000xx	为其它分组密码算法预留	*/

#define SGD_RSA			0x00010000	//RSA算法
#define SGD_SM2_1		0x00020100	//椭圆曲线签名算法
#define SGD_SM2_2		0x00020200	//椭圆曲线密钥交换协议
#define SGD_SM2_3		0x00020400	//椭圆曲线加密算法
/*	0x00000400～0x800000xx	为其它非对称密码算法预留	*/

#define SGD_SM3			0x00000001	//SM3杂凑算法
#define SGD_SHA1		0x00000002	//SHA1杂凑算法
#define SGD_SHA256		0x00000004	//SHA256杂凑算法
/*	0x00000010～0x000000FF	为其它密码杂凑算法预留	*/

#define MAX_IV_LEN			32		//初始化向量的最大长度
#define	MAX_FILE_NAME_LEN	32		//文件名最大长度
#define MAX_CONTAINER_NAME_LEN	128	//容器名最大长度
#define MIN_PIN_LEN			6		//最小的PIN长度

#define MAX_RSA_MODULUS_LEN 256		//RSA算法模数的最大长度
#define MAX_RSA_EXPONENT_LEN 4		//RSA算法指数的最大长度

#define ECC_MAX_XCOORDINATE_BITS_LEN 512	//ECC算法X座标的最大长度
#define ECC_MAX_YCOORDINATE_BITS_LEN 512	//ECC算法Y座标的最大长度
#define ECC_MAX_MODULUS_BITS_LEN 512		//ECC算法模数的最大长度

typedef struct Struct_BLOCKCIPHERPARAM {
	BYTE	IV[MAX_IV_LEN];			//初始向量，MAX_IV_LEN为初始向量的最大长度
	ULONG	IVLen;					//初始向量实际长度，按字节计算
	ULONG	PaddingType;			//填充方式，0表示不填充，1表示按照PKCS#5方式进行填充
	ULONG	FeedBitLen;				//反馈值的位长度，按字节计算，只针对OFB、CFB模式
} BLOCKCIPHERPARAM, *PBLOCKCIPHERPARAM;

typedef struct st_device_state{
	BOOL isOn;
	BOOL isLogged;
	int timeLeft;
	char name[250];
}DEVICE_STATE;

typedef struct st_device_info{
	char szDeviceInfo[MAX_PATH];
	char szVID[8];
	char szPID[8];
	char szSN[64];
	char szPD[12];
	char szPI[MAX_PATH];
	char szAF[MAX_PATH];
	char szLabel[MAX_PATH];
	DWORD dwTotalSpace;
	DWORD dwFreeSpace;
	DWORD dwDeviceType;
}DEVICE_INFO;

typedef struct Struct_FILEATTRIBUTE {
    CHAR	FileName[32];			//文件名
    ULONG	FileSize;				//文件大小
    ULONG	ReadRights;				//读权限
    ULONG	WriteRights;			//写权限
} FILEATTRIBUTE, *PFILEATTRIBUTE;

typedef struct Struct_ECCCIPHERBLOB{
	BYTE	XCoordinate[ECC_MAX_XCOORDINATE_BITS_LEN/8];
	BYTE	YCoordinate[ECC_MAX_YCOORDINATE_BITS_LEN/8];
	BYTE    HASH[32]; 
	ULONG	CipherLen;
	BYTE    Cipher[128];
} ECCCIPHERBLOB, *PECCCIPHERBLOB;

#pragma pack(pop)