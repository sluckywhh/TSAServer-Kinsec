
#ifndef _FM_SGD_H_
#define _FM_SGD_H_ 1

#ifdef __cplusplus
	extern "C"{
#endif

#define FM_SDF_API_VERSION "1.0.3.0"

//对称算法标识
#define SGD_SM1_ECB		0x00000101
#define SGD_SM1_CBC		0x00000102
#define SGD_SM1_CFB		0x00000104
#define SGD_SM1_OFB		0x00000108
#define SGD_SM1_MAC		0x00000110
#define SGD_SM1_CTR		0x00000120

#define SGD_SSF33_ECB	0x00000201
#define SGD_SSF33_CBC	0x00000202
#define SGD_SSF33_CFB	0x00000204
#define SGD_SSF33_OFB	0x00000208
#define SGD_SSF33_MAC	0x00000210
#define SGD_SSF33_CTR	0x00000220

#define SGD_SM4_ECB		0x00000401
#define SGD_SM4_CBC		0x00000402
#define SGD_SM4_CFB		0x00000404
#define SGD_SM4_OFB		0x00000408
#define SGD_SM4_MAC		0x00000410

#define SGD_DES_ECB		0x00001001
#define SGD_DES_CBC		0x00001002
#define SGD_DES_CFB		0x00001004
#define SGD_DES_OFB		0x00001008
#define SGD_DES_MAC		0x00001010
#define SGD_DES_CTR		0x00001020

#define SGD_3DES_ECB	0x00002001
#define SGD_3DES_CBC	0x00002002
#define SGD_3DES_CFB	0x00002004
#define SGD_3DES_OFB	0x00002008
#define SGD_3DES_MAC	0x00002010
#define SGD_3DES_CTR	0x00002020

#define SGD_AES_ECB		0x00004001
#define SGD_AES_CBC		0x00004002
#define SGD_AES_CFB		0x00004004
#define SGD_AES_OFB		0x00004008
#define SGD_AES_MAC		0x00004010

//非对称算法标识
#define SGD_RSA			0x00010000
#define SGD_RSA_SIGN	0x00010100
#define SGD_RSA_ENC		0x00010200
#define SGD_SM2			0x00020100
#define SGD_SM2_1		0x00020200
#define SGD_SM2_2		0x00020400
#define SGD_SM2_3		0x00020800

#define SGD_SM3			0x00000001
#define SGD_SHA1		0x00000002
#define SGD_SHA256		0x00000004
#define SGD_SHA384		0x00000008
#define SGD_SHA512		0x00000010
#define SGD_SHA224		0x00000020
#define SGD_MD5			0x00000040

//错误码定义
#define SDR_OK						0x0						//操作成功
#define SDR_BASE					0x01000000				//错误码基础值
#define SDR_UNKNOWERR				SDR_BASE + 0x00000001	//未知错误
#define SDR_NOTSUPPORT				SDR_BASE + 0x00000002	//不支持的接口调用
#define SDR_COMMFAIL				SDR_BASE + 0x00000003	//与设备通信失败
#define SDR_HARDFAIL				SDR_BASE + 0x00000004	//运算模块无响应
#define SDR_OPENDEVICE				SDR_BASE + 0x00000005	//打开设备失败
#define SDR_OPENSESSION				SDR_BASE + 0x00000006	//创建会话失败
#define SDR_PARDENY					SDR_BASE + 0x00000007	//无私钥使用权限
#define SDR_KEYNOTEXIST				SDR_BASE + 0x00000008	//不存在的密钥调用
#define SDR_ALGNOTSUPPORT			SDR_BASE + 0x00000009	//不支持的算法调用
#define SDR_ALGMODNOTSUPPORT		SDR_BASE + 0x0000000A	//不支持的算法模式调用
#define SDR_PKOPERR					SDR_BASE + 0x0000000B	//公钥运算失败
#define SDR_SKOPERR					SDR_BASE + 0x0000000C	//私钥运算失败
#define SDR_SIGNERR					SDR_BASE + 0x0000000D	//签名运算失败
#define SDR_VERIFYERR				SDR_BASE + 0x0000000E	//验证签名失败
#define SDR_SYMOPERR				SDR_BASE + 0x0000000F	//对称算法运算失败
#define SDR_STEPERR					SDR_BASE + 0x00000010	//多步运算步骤错误
#define SDR_FILESIZEERR				SDR_BASE + 0x00000011	//文件长度超出限制
#define SDR_FILENOEXIST				SDR_BASE + 0x00000012	//指定的文件不存在
#define SDR_FILEOFSERR				SDR_BASE + 0x00000013	//文件起始位置错误
#define SDR_KEYTYPEERR				SDR_BASE + 0x00000014	//密钥类型错误
#define SDR_KEYERR					SDR_BASE + 0x00000015	//密钥错误


/*自定义误码*/
#define SDR_NOMANAGEMENTAUTH	SDR_BASE + 0x00000016	//管理权限不满足
#define SDR_NOOPERATIONAUTH		SDR_BASE + 0x00000017	//操作权限不满足

#define SDR_MALLOCERR			SDR_BASE + 0x00000018	//内存分配错误

#define SDR_HANDLENULL			SDR_BASE + 0x00000019	//句柄为空
#define SDR_PARAMETERSERR		SDR_BASE + 0x00000020	//参数错误
#define SDR_DEVICEERR			SDR_BASE + 0x00000021	//调用设备函数错误
#define SDR_CREATEFILEERR		SDR_BASE + 0x00000022	//创建文件失败
#define SDR_PRIVATEERR			SDR_BASE + 0x00000023	//私钥权限码错误
#define SDR_LENGTH_ERROR        SDR_BASE + 0x00000024   //传入的长度错误
#define SDR_INDEX_ERROR         SDR_BASE + 0x00000025   //密钥索引错误
#define SDR_KEYLENGTHERROR      SDR_BASE + 0x00000026   //密钥长度错误

/************************设备信息结构**************************************/
typedef struct DeviceInfo_st
{
	unsigned char IssuerName[40];		//设备生产厂商名称
	unsigned char DeviceName[16];		//设备型号
	unsigned char DeviceSerial[16];		//设备编号，包含：日期（8字符）、批次号（3字符）、流水号（5字符）
	unsigned int  DeviceVersion;		//密码设备内部软件的版本号
	unsigned int  StandardVersion;		//密码设备支持的接口规范版本号
	unsigned int  AsymAlgAbility[2];	//前4字节表示支持的算法,表示方法为非对称算法标识按位或的结果
	unsigned int  SymAlgAbility;		//所有支持的对称算法，表示方法为对称算法标识按位或运算结果
	unsigned int  HashAlgAbility;		//所有支持的杂凑算法，表示方法为杂凑算法标识按位或运算结果
	unsigned int  BufferSize;			//支持的最大文件存储空间（单位字节）
}DEVICEINFO;


/*********************************RSA密钥结构*******************************/
#define LiteRSAref_MAX_BITS    2048
#define LiteRSAref_MAX_LEN     ((LiteRSAref_MAX_BITS + 7) / 8)
#define LiteRSAref_MAX_PBITS   ((LiteRSAref_MAX_BITS + 1) / 2)
#define LiteRSAref_MAX_PLEN    ((LiteRSAref_MAX_PBITS + 7)/ 8)

typedef struct RSArefPublicKeyLite_st
{
	unsigned int  bits;
	unsigned char m[LiteRSAref_MAX_LEN];
	unsigned char e[LiteRSAref_MAX_LEN];
}RSArefPublicKeyLite;

typedef struct RSArefPrivateKeyLite_st
{
	unsigned int  bits;
	unsigned char m[LiteRSAref_MAX_LEN];
	unsigned char e[LiteRSAref_MAX_LEN];
	unsigned char d[LiteRSAref_MAX_LEN];
	unsigned char prime[2][LiteRSAref_MAX_PLEN];
	unsigned char pexp[2][LiteRSAref_MAX_PLEN];
	unsigned char coef[LiteRSAref_MAX_PLEN];
}RSArefPrivateKeyLite;

#define ExRSAref_MAX_BITS    4096
#define ExRSAref_MAX_LEN     ((ExRSAref_MAX_BITS + 7) / 8)
#define ExRSAref_MAX_PBITS   ((ExRSAref_MAX_BITS + 1) / 2)
#define ExRSAref_MAX_PLEN    ((ExRSAref_MAX_PBITS + 7)/ 8)

typedef struct RSArefPublicKeyEx_st
{
	unsigned int  bits;
	unsigned char m[ExRSAref_MAX_LEN];
	unsigned char e[ExRSAref_MAX_LEN];
} RSArefPublicKeyEx;

typedef struct RSArefPrivateKeyEx_st
{
	unsigned int  bits;
	unsigned char m[ExRSAref_MAX_LEN];
	unsigned char e[ExRSAref_MAX_LEN];
	unsigned char d[ExRSAref_MAX_LEN];
	unsigned char prime[2][ExRSAref_MAX_PLEN];
	unsigned char pexp[2][ExRSAref_MAX_PLEN];
	unsigned char coef[ExRSAref_MAX_PLEN];
} RSArefPrivateKeyEx;

#if defined(SGD_RSA_MAX_BITS) && (SGD_RSA_MAX_BITS > LiteRSAref_MAX_BITS)
#define RSAref_MAX_BITS    ExRSAref_MAX_BITS
#define RSAref_MAX_LEN     ExRSAref_MAX_LEN
#define RSAref_MAX_PBITS   ExRSAref_MAX_PBITS
#define RSAref_MAX_PLEN    ExRSAref_MAX_PLEN

typedef struct RSArefPublicKeyEx_st  RSArefPublicKey;
typedef struct RSArefPrivateKeyEx_st  RSArefPrivateKey;
#else
#define RSAref_MAX_BITS    LiteRSAref_MAX_BITS
#define RSAref_MAX_LEN     LiteRSAref_MAX_LEN
#define RSAref_MAX_PBITS   LiteRSAref_MAX_PBITS
#define RSAref_MAX_PLEN    LiteRSAref_MAX_PLEN

typedef struct RSArefPublicKeyLite_st  RSArefPublicKey;
typedef struct RSArefPrivateKeyLite_st  RSArefPrivateKey;
#endif

#define ECCMAXBITS256
/******************************ECC密钥结构********************************/
#ifdef ECCMAXBITS256
#define ECCref_MAX_BITS			256 
#else
#define ECCref_MAX_BITS			512 
#endif
#define ECCref_MAX_LEN			((ECCref_MAX_BITS+7) / 8)

typedef struct ECCrefPublicKey_st
{
	unsigned int  bits;					//模长
	unsigned char x[ECCref_MAX_LEN]; 	//公钥x坐标
	unsigned char y[ECCref_MAX_LEN]; 	//公钥y坐标
} ECCrefPublicKey;

typedef struct ECCrefPrivateKey_st
{
    unsigned int  bits;				//模长
    unsigned char D[ECCref_MAX_LEN];//私钥
} ECCrefPrivateKey;


/*****************************ECC 密文结构********************************/
typedef struct ECCCipher_st
{
	unsigned char x[ECCref_MAX_LEN]; //与y组成椭圆曲线上的点（x，y）
	unsigned char y[ECCref_MAX_LEN]; //与x组成椭圆曲线上的点（x，y）
	unsigned char M[32]; //预留，用于支持带MAC输出的ECC算法
	unsigned int  L;				 //密文数据长度
	unsigned char C[136];			 //加密数据
}ECCCipher;


/****************************ECC 签名结构*********************************/
typedef struct ECCSignature_st
{
	unsigned char r[ECCref_MAX_LEN];	//签名的r部分
	unsigned char s[ECCref_MAX_LEN];	//签名的s部分
} ECCSignature;

/******************ECC加密密钥对保护结构**********************************/
typedef struct SDF_ENVELOPEDKEYBLOB
{
	unsigned long ulAsymmAlgID;
	unsigned long ulSymmAlgID;
	ECCCipher	  ECCCipherBlob;
	ECCrefPublicKey PubKey;
	unsigned char  cbEncryptedPriKey[64];
} ENVELOPEDKEYBLOB, *PENVELOPEDKEYBLOB;

/*************************************************************************/

/*
#define USER_KEY_START_INDEX  2
#define USER_KEY_END_INDEX    61
#define ENCRYPT_USER_KEY      63

#define UserKeyINDEX(i)   if(((2*i)<USER_KEY_START_INDEX)||((2*i+1)>USER_KEY_END_INDEX))\
								return SDR_INDEX_ERROR;
							
#define EncryptUserKey(i)  if(i<1||i>ENCRYPT_USER_KEY)\
							return SDR_INDEX_ERROR;	
*/
#define UserKeyINDEX_RSA(i)   if(RSAKeyIndexCheck(i) == -1)\
							return SDR_INDEX_ERROR;

#define UserKeyINDEX_ECC(i)   if(ECCKeyIndexCheck(i) == -1)\
							return SDR_INDEX_ERROR;

#define EncryptUserKey(i)  if(SYMKeyIndexCheck(i) == -1)\
							return SDR_INDEX_ERROR;	


/********************************设备管理类函数**************************/
/*
原型：	int SDF_OpenDevice(void **phDeviceHandle);
描述：	打开密码设备
参数：	phDeviceHandle[out]	返回设备句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	phDeviceHandle由函数初始化并填写内容
*/
int SDF_OpenDevice(void **phDeviceHandle);

/*
6.1.1 打开设备（可指定配置文件路径）
原型：
int SDF_OpenDeviceWithPath(
char * pcCfgPath,
unsigned int phDeviceHandle);
描述： 打开密码设备
参数：
pcCfgPath[in]
配置文件部署目录（不包含配置文件名称，配置文件
名称为swsds.ini）
如：/etc/swhsm/
phDeviceHandle[out] 返回设备句柄
返回值： 0 成功
非0 失败，返回错误代码
备注： phDeviceHandle 由函数初始化并填写内容
*/
int SDF_OpenDeviceWithPath(char * pcCfgPath, void **phDeviceHandle);

/*
原型：
int SDF_OpenDeviceWithParameter(
unsigned int phDeviceHandle,
char **argv);
描述： 打开密码设备
参数： phDeviceHandle[out] 返回设备句柄
Argv[in]
返回值： 0 成功
非0 失败，返回错误代码
备注： phDeviceHandle 由函数初始化并填写内容
*/
int SDF_OpenDeviceWithParameter(void **phDeviceHandle, char **argv);

/*
原型：	int SDF_CloseDevice(void *hDeviceHandle);
描述：	关闭密码设备，并释放相关资源
参数：	hDeviceHandle[in]	已打开的设备句柄
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_CloseDevice(void *hDeviceHandle);

/*
原型：	int SDF_OpenSession(void *hDeviceHandle, void **phSessionHandle);
描述：	创建与密码设备的会话
参数：	hDeviceHandle[in]	已打开的设备句柄
	phSessionHandle[out]	返回与密码设备建立的新会话句柄
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_OpenSession(void *hDeviceHandle, void **phSessionHandle);

/*
原型：	int SDF_CloseSession(void *hSessionHandle);
描述：	关闭与密码设备已建立的会话，并释放相关资源
参数：	hSessionHandle [in]	与密码设备已建立的会话句柄
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_CloseSession(void *hSessionHandle);

/*
原型：	int SDF_GetDeviceInfo (
void *hSessionHandle,
DEVICEINFO *pstDeviceInfo);
描述：	获取密码设备能力描述
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pstDeviceInfo [out]	设备能力描述信息，内容及格式见设备信息定义

返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_GetDeviceInfo (void *hSessionHandle,DEVICEINFO *pstDeviceInfo);

/*
原型：	int SDF_GenerateRandom (
void *hSessionHandle, 
unsigned int  uiLength,
unsigned char *pucRandom);
描述：	获取指定长度的随机数
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiLength[in]	欲获取的随机数长度
	pucRandom[out]	缓冲区指针，用于存放获取的随机数
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_GenerateRandom 
(
	void *hSessionHandle, 
	unsigned int  uiLength, 
	unsigned char *pucRandom
);

/*
描述：	获取密码设备内部存储的指定索引私钥的使用权
参数：	hSessionHandle[in]
	uiKeyIndex[in]
	pucPassword[in]
	uiPwdLength[in]
返回值：	0
	非0
备注：	本规范涉及密码设备存储的密钥对索引值的的起始索引值为1，最大为n，密码设备的实际存储容量决定n值
*/
int SDF_GetPrivateKeyAccessRight 
(
	void *hSessionHandle, 
	unsigned int  uiKeyIndex, 
	unsigned char *pucPassword, 
	unsigned int  uiPwdLength
);

/*
原型：	int SDF_ReleasePrivateKeyAccessRight (
void *hSessionHandle, 
unsigned int  uiKeyIndex);
描述：	释放密码设备存储的指定索引私钥的使用授权
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备存储私钥索引值
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ReleasePrivateKeyAccessRight (void *hSessionHandle, unsigned int  uiKeyIndex);


/**********************************密钥管理类函数***********************************/
/*
原型：	int SDF_ExportSignPublicKey_RSA(
void *hSessionHandle, 
unsigned int  uiKeyIndex,
RSArefPublicKey *pucPublicKey);
描述：	导出密码设备内部存储的指定索引位置的签名公钥
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备存储的RSA密钥对索引值
	pucPublicKey[out]	RSA公钥结构
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExportSignPublicKey_RSA
(
	void *hSessionHandle, 
	unsigned int  uiKeyIndex, 
	RSArefPublicKey *pucPublicKey
);

/*
原型：	int SDF_ExportEncPublicKey_RSA(
void *hSessionHandle, 
unsigned int  uiKeyIndex,
RSArefPublicKey *pucPublicKey);
描述：	导出密码设备内部存储的指定索引位置的加密公钥
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备存储的RSA密钥对索引值
	pucPublicKey[out]	RSA公钥结构
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExportEncPublicKey_RSA
(
	void *hSessionHandle, 
	unsigned int  uiKeyIndex, 
	RSArefPublicKey *pucPublicKey
);

/*
原型：	int SDF_GenerateKeyPair_RSA(
void *hSessionHandle, 
unsigned int  uiKeyBits,
RSArefPublicKey *pucPublicKey,
RSArefPrivateKey *pucPrivateKey);
描述：	请求密码设备产生指定模长的RSA密钥对
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyBits [in]	指定密钥模长
	pucPublicKey[out]	RSA公钥结构
	pucPrivateKey[out]	RSA私钥结构
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_GenerateKeyPair_RSA
(
	void *hSessionHandle, 
	unsigned int  uiKeyBits, 
	RSArefPublicKey *pucPublicKey, 
	RSArefPrivateKey *pucPrivateKey
);

/*
原型：	int SDF_GenerateKeyWithIPK_RSA (
void *hSessionHandle, 
unsigned int uiIPKIndex,
unsigned int uiKeyBits,
unsigned char *pucKey,
unsigned int *puiKeyLength,
void **phKeyHandle);
描述：	生成会话密钥并用指定索引的内部加密公钥加密输出，同时返回密钥句柄
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiIPKIndex[in]	密码设备内部存储公钥的索引值
	uiKeyBits[in]	指定产生的会话密钥长度
	pucKey[out]	缓冲区指针，用于存放返回的密钥密文
	puiKeyLength[out]	返回的密钥密文长度
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	公钥加密数据时填充方式按照PKCS#1 v1.5的要求进行；返回的密钥不包含IV部分。
*/
int SDF_GenerateKeyWithIPK_RSA 
(
	void *hSessionHandle, 
	unsigned int uiIPKIndex, 
	unsigned int uiKeyBits, 
	unsigned char *pucKey, 
	unsigned int *puiKeyLength, 
	void **phKeyHandle
);

/*
原型：	int SDF_GenerateKeyWithEPK_RSA (
void *hSessionHandle, 
unsigned int uiKeyBits,
RSArefPublicKey *pucPublicKey,
unsigned char *pucKey,
unsigned int *puiKeyLength,
void **phKeyHandle);
描述：	生成会话密钥并用外部公钥加密输出，同时返回密钥句柄
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyBits[in]	指定产生的会话密钥长度
	pucPublicKey[in]	输入的外部RSA公钥结构
	pucKey[out]	缓冲区指针，用于存放返回的密钥密文
	puiKeyLength[out]	返回的密钥密文长度
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	公钥加密数据时填充方式按照PKCS#1 v1.5的要求进行；返回的密钥不包含IV部分。
*/
int SDF_GenerateKeyWithEPK_RSA 
(
	void *hSessionHandle, 
	unsigned int uiKeyBits, 
	RSArefPublicKey *pucPublicKey, 
	unsigned char *pucKey, 
	unsigned int *puiKeyLength, 
	void **phKeyHandle
);

/*
原型：	int SDF_ImportKeyWithISK_RSA (
void *hSessionHandle, 
unsigned int uiISKIndex,
unsigned char *pucKey,
unsigned int *puiKeyLength,
void **phKeyHandle);
描述：	导入会话密钥并用内部私钥解密，同时返回密钥句柄
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiISKIndex[in]	密码设备内部存储加密私钥的索引值，对应于加密时的公钥
	pucKey[in]	缓冲区指针，用于存放输入的密钥密文
	puiKeyLength[in]	输入的密钥密文长度
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	填充方式与公钥加密时相同。
*/
int SDF_ImportKeyWithISK_RSA 
(
	void *hSessionHandle, 
	unsigned int uiISKIndex, 
	unsigned char *pucKey, 
	unsigned int uiKeyLength, 
	void **phKeyHandle
);

/*
	int SDF_ExchangeDigitEnvelopeBaseOnRSA(
void *hSessionHandle, 
unsigned int  uiKeyIndex,
RSArefPublicKey *pucPublicKey,
unsigned char *pucDEInput,
unsigned int  uiDELength,
unsigned char *pucDEOutput,
unsigned int  *puiDELength);
描述：	将由内部加密公钥加密的会话密钥转换为由外部指定的公钥加密，可用于数字信封转换。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备存储的内部RSA密钥对索引值
	pucPublicKey [in]	外部RSA公钥结构
	pucDEInput [in]	缓冲区指针，用于存放输入的会话密钥密文
	uiDELength[in]	输入的会话密钥密文长度
	pucDEOutput[out]	缓冲区指针，用于存放输出的会话密钥密文
	puiDELength[out]	输出的会话密钥密文长度
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExchangeDigitEnvelopeBaseOnRSA
(
	void *hSessionHandle, 
	unsigned int  uiKeyIndex, 
	RSArefPublicKey *pucPublicKey, 
	unsigned char *pucDEInput, 
	unsigned int  uiDELength, 
	unsigned char *pucDEOutput, 
	unsigned int  *puiDELength
);

/*
	int SDF_ExportSignPublicKey_ECC(
void *hSessionHandle, 
unsigned int  uiKeyIndex,
ECCrefPublicKey *pucPublicKey);
描述：	导出密码设备内部存储的指定索引位置的签名公钥
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备存储的ECC密钥对索引值
	pucPublicKey[out]	ECC公钥结构
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExportSignPublicKey_ECC
(
	void *hSessionHandle, 
	unsigned int  uiKeyIndex,
	ECCrefPublicKey *pucPublicKey
);

/*
原型：	int SDF_ExportEncPublicKey_ECC(
void *hSessionHandle, 
unsigned int  uiKeyIndex,
ECCrefPublicKey *pucPublicKey);
描述：	导出密码设备内部存储的指定索引位置的加密公钥
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备存储的ECC密钥对索引值
	pucPublicKey[out]	ECC公钥结构
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExportEncPublicKey_ECC
(
	void *hSessionHandle, 
	unsigned int  uiKeyIndex,
	ECCrefPublicKey *pucPublicKey
);

/*
原型：	int SDF_GenerateKeyPair_ECC(
void *hSessionHandle, 
unsigned int  uiAlgID,
unsigned int  uiKeyBits,
ECCrefPublicKey *pucPublicKey,
ECCrefPrivateKey *pucPrivateKey);
描述：	请求密码设备产生指定类型和模长的ECC密钥对
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiAlgID[in]	指定算法标识
	uiKeyBits [in]	指定密钥长度
	pucPublicKey[out]	ECC公钥结构
	pucPrivateKey[out]	ECC私钥结构
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_GenerateKeyPair_ECC
(
	void *hSessionHandle, 
	unsigned int  uiAlgID,
	unsigned int  uiKeyBits,
	ECCrefPublicKey *pucPublicKey,
	ECCrefPrivateKey *pucPrivateKey
);

/*
原型：	int SDF_GenerateKeyWithIPK_ECC (
void *hSessionHandle, 
unsigned int uiIPKIndex,
unsigned int uiKeyBits,
ECCCipher *pucKey,
void **phKeyHandle);
描述：	生成会话密钥并用指定索引的内部ECC加密公钥加密输出，同时返回密钥句柄
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiIPKIndex[in]	密码设备内部存储公钥的索引值
	uiKeyBits[in]	指定产生的会话密钥长度
	pucKey[out]	缓冲区指针，用于存放返回的密钥密文
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	返回的密钥不包含IV部分。
*/
int SDF_GenerateKeyWithIPK_ECC 
(
	void *hSessionHandle, 
	unsigned int uiIPKIndex,
	unsigned int uiKeyBits,
	ECCCipher *pucKey,
	void **phKeyHandle
);

/*
原型：	int SDF_GenerateKeyWithEPK_ECC (
void *hSessionHandle, 
unsigned int uiKeyBits,
unsigned int  uiAlgID,
ECCrefPublicKey *pucPublicKey,
ECCCipher *pucKey,
void **phKeyHandle);
描述：	生成会话密钥并用外部ECC公钥加密输出，同时返回密钥句柄
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyBits[in]	指定产生的会话密钥长度
	uiAlgID[in]	外部ECC公钥的算法标识
	pucPublicKey[in]	输入的外部ECC公钥结构
	pucKey[out]	缓冲区指针，用于存放返回的密钥密文
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	返回的密钥不包含IV部分。
*/
int SDF_GenerateKeyWithEPK_ECC 
(
	void *hSessionHandle, 
	unsigned int uiKeyBits,
	unsigned int  uiAlgID,
	ECCrefPublicKey *pucPublicKey,
	ECCCipher *pucKey,
	void **phKeyHandle
);


/*
原型：	int SDF_ImportKeyWithISK_ECC (
void *hSessionHandle,
unsigned int uiISKIndex,
ECCCipher *pucKey,
void **phKeyHandle);
描述：	导入会话密钥并用内部ECC加密私钥解密，同时返回密钥句柄
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiISKIndex[in]	密码设备内部存储加密私钥的索引值，对应于加密时的公钥
	pucKey[in]	缓冲区指针，用于存放输入的密钥密文
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ImportKeyWithISK_ECC 
(
	void *hSessionHandle,
	unsigned int uiISKIndex,
	ECCCipher *pucKey,
	void **phKeyHandle
);

/*
原型：	int SDF_GenerateAgreementDataWithECC (
void *hSessionHandle, 
unsigned int uiISKIndex,
unsigned int uiKeyBits,
unsigned char *pucSponsorID,
unsigned int uiSponsorIDLength,
ECCrefPublicKey  *pucSponsorPublicKey,
ECCrefPublicKey  *pucSponsorTmpPublicKey,
void **phAgreementHandle);
描述：	使用ECC密钥协商算法，为计算会话密钥而产生协商参数，同时返回指定索引位置的ECC公钥、临时ECC密钥对的公钥及协商句柄。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiISKIndex[in]	密码设备内部存储加密私钥的索引值，该私钥用于参与密钥协商
	uiKeyBits[in]	要求协商的密钥长度
	pucSponsorID[in]	参与密钥协商的发起方ID值
	uiSponsorIDLength[in]	发起方ID长度
	pucSelfPublicKey[out]	返回的发起方ECC公钥结构
	pucSelfTmpPublicKey[out]	返回的发起方临时ECC公钥结构
	phAgreementHandle[out]	返回的协商句柄，用于计算协商密钥
返回值：	0	成功
	非0	失败，返回错误代码
备注：	为协商会话密钥，协商的发起方应首先调用本函数。
如果在具体的应用中，协商双方没有统一分配的ID，可以将ID设定为常量。
*/
int SDF_GenerateAgreementDataWithECC 
(
	void *hSessionHandle, 
	unsigned int uiISKIndex,
	unsigned int uiKeyBits,
	unsigned char *pucSponsorID,
	unsigned int uiSponsorIDLength,
	ECCrefPublicKey  *pucSponsorPublicKey,
	ECCrefPublicKey  *pucSponsorTmpPublicKey,
	void **phAgreementHandle
);

/*
原型：	int SDF_GenerateKeyWithECC (
void *hSessionHandle, 
unsigned char *pucResponseID,
unsigned int uiResponseIDLength，
ECCrefPublicKey *pucResponsePublicKey,
ECCrefPublicKey *pucResponseTmpPublicKey,
void *hAgreementHandle,
void **phKeyHandle);
描述：	使用ECC密钥协商算法，使用自身协商句柄和响应方的协商参数计算会话密钥，同时返回会话密钥句柄。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucResponseID[in]	外部输入的响应方ID值
	uiResponseIDLength[in]	外部输入的响应方ID长度
	pucResponsePublicKey[in]	外部输入的响应方ECC公钥结构
	pucResponseTmpPublicKey[in]	外部输入的响应方临时ECC公钥结构
	hAgreementHandle[in]	协商句柄，用于计算协商密钥
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	协商的发起方获得响应方的协商参数后调用本函数，计算会话密钥。
如果在具体的应用中，协商双方没有统一分配的ID，可以将ID设定为常量。
*/
int SDF_GenerateKeyWithECC 
(
	void *hSessionHandle, 
	unsigned char *pucResponseID,
	unsigned int uiResponseIDLength,
	ECCrefPublicKey *pucResponsePublicKey,
	ECCrefPublicKey *pucResponseTmpPublicKey,
	void *hAgreementHandle,
	void **phKeyHandle
);

/*
原型：	int SDF_GenerateAgreementDataAndKeyWithECC (
void *hSessionHandle, 
unsigned int uiISKIndex,
unsigned int uiKeyBits,
unsigned char *pucResponseID,
unsigned int uiResponseIDLength,
unsigned char *pucSponsorID,
unsigned int uiSponsorIDLength,
ECCrefPublicKey *pucSponsorPublicKey,
ECCrefPublicKey *pucSponsorTmpPublicKey,
ECCrefPublicKey  *pucResponsePublicKey,
ECCrefPublicKey  *pucResponseTmpPublicKey,
void **phKeyHandle);
描述：	使用ECC密钥协商算法，产生协商参数并计算会话密钥，同时返回产生的协商参数和和密钥句柄。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiISKIndex[in]	密码设备内部存储加密私钥的索引值，该私钥用于参与密钥协商
	uiKeyBits[in]	协商后要求输出的密钥长度
	pucResponseID[in]	响应方ID值
	uiResponseIDLength[in]	响应方ID长度
	pucSponsorID[in]	发起方ID值
	uiSponsorIDLength[in]	发起方ID长度
	pucSponsorPublicKey[in]	外部输入的发起方ECC公钥结构
	pucSponsorTmpPublicKey[in]	外部输入的发起方临时ECC公钥结构
	pucResponsePublicKey[out]	返回的响应方ECC公钥结构
	pucResponseTmpPublicKey[out]	返回的响应方临时ECC公钥结构
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	本函数由响应方调用。
如果在具体的应用中，协商双方没有统一分配的ID，可以将ID设定为常量
*/
int SDF_GenerateAgreementDataAndKeyWithECC 
(
	void *hSessionHandle, 
	unsigned int uiISKIndex,
	unsigned int uiKeyBits,
	unsigned char *pucResponseID,
	unsigned int uiResponseIDLength,
	unsigned char *pucSponsorID,
	unsigned int uiSponsorIDLength,
	ECCrefPublicKey *pucSponsorPublicKey,
	ECCrefPublicKey *pucSponsorTmpPublicKey,
	ECCrefPublicKey  *pucResponsePublicKey,
	ECCrefPublicKey  *pucResponseTmpPublicKey,
	void **phKeyHandle
);

/*
	int SDF_ExchangeDigitEnvelopeBaseOnECC(
void *hSessionHandle, 
unsigned int  uiKeyIndex,
unsigned int  uiAlgID,
ECCrefPublicKey *pucPublicKey,
ECCCipher *pucEncDataIn,
ECCCipher *pucEncDataOut);
描述：	将由内部加密公钥加密的会话密钥转换为由外部指定的公钥加密，可用于数字信封转换。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备存储的ECC密钥对索引值
	uiAlgID[in]	外部ECC公钥的算法标识
	pucPublicKey [in]	外部ECC公钥结构
	pucEncDataIn[in]	缓冲区指针，用于存放输入的会话密钥密文
	pucEncDataOut[out]	缓冲区指针，用于存放输出的会话密钥密文
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExchangeDigitEnvelopeBaseOnECC
(
	void *hSessionHandle, 
	unsigned int  uiKeyIndex,
	unsigned int  uiAlgID,
	ECCrefPublicKey *pucPublicKey,
	ECCCipher *pucEncDataIn,
	ECCCipher *pucEncDataOut
);

/*
原型：	int SDF_GenerateKeyWithKEK (
void *hSessionHandle, 
unsigned int uiKeyBits,
unsigned int  uiAlgID,
unsigned int uiKEKIndex, 
unsigned char *pucKey, 
unsigned int *puiKeyLength, 
void **phKeyHandle);
描述：	生成会话密钥并用密钥加密密钥加密输出，同时返回密钥句柄。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyBits[in]	指定产生的会话密钥长度
	uiAlgID[in]	算法标识，指定对称加密算法
	uiKEKIndex[in]	密码设备内部存储密钥加密密钥的索引值
	pucKey[out]	缓冲区指针，用于存放返回的密钥密文
	puiKeyLength[out]	返回的密钥密文长度
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	加密模式使用ECB模式。
*/
int SDF_GenerateKeyWithKEK 
(
	void *hSessionHandle, 
	unsigned int uiKeyBits,
	unsigned int  uiAlgID,
	unsigned int uiKEKIndex, 
	unsigned char *pucKey, 
	unsigned int *puiKeyLength, 
	void **phKeyHandle
);

/*
原型：	int SDF_ImportKeyWithKEK (
void *hSessionHandle, 
unsigned int  uiAlgID,
unsigned int uiKEKIndex, 
unsigned char *pucKey, 
unsigned int *puiKeyLength, 
void **phKeyHandle);
描述：	导入会话密钥并用密钥加密密钥解密，同时返回会话密钥句柄。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiAlgID[in]	算法标识，指定对称加密算法
	uiKEKIndex[in]	密码设备内部存储密钥加密密钥的索引值
	pucKey[in]	缓冲区指针，用于存放输入的密钥密文
	puiKeyLength[in]	输入的密钥密文长度
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	加密模式使用ECB模式。
*/
int SDF_ImportKeyWithKEK 
(
	void *hSessionHandle, 
	unsigned int  uiAlgID,
	unsigned int uiKEKIndex, 
	unsigned char *pucKey, 
	unsigned int puiKeyLength, 
	void **phKeyHandle
);

/*
原型：
int SDF_InternalEncrypt_ECC(
void *hSessionHandle,
unsigned int uiISKIndex,
unsigned int uiKeyUsage,
unsigned char *pucData,
unsigned int uiDataLength,
ECCSignature *pucSignature);
描述： 使用内部ECC 公钥对数据进行加密运算
参数： hSessionHandle[in] 与设备建立的会话句柄
uiISKIndex [in] 密码设备内部存储的ECC 公钥的索引值
uiKeyUsage [in] 指定使用签名公钥还是加密公钥
SGD_SM2_1：签名公钥
SGD_SM2_3：加密公钥
pucData[in] 缓冲区指针，用于存放外部输入的数据
uiDataLength[in] 输入的数据长度
pucSignature[in] 缓冲区指针，用于存放输入的签名值数据
返回值： 0 成功
非0 失败，返回错误代码
备注：
*/
int SDF_InternalEncrypt_ECC
(
	void		  *hSessionHandle,
	unsigned int  uiISKIndex,
	unsigned int  uiKeyUsage,
	unsigned char *pucData,
	unsigned int  uiDataLength,
	ECCCipher	  *pucEncData
);

/*
原型：
int SDF_InternalDecrypt_ECC(
void *hSessionHandle,
unsigned int uiISKIndex,
unsigned int uiKeyUsage,
unsigned char *pucData,
unsigned int uiDataLength,
ECCSignature *pucSignature);
描述： 使用内部ECC 私钥对数据进行解密运算
参数： hSessionHandle[in] 与设备建立的会话句柄
uiISKIndex [in] 密码设备内部存储的ECC 私钥的索引值
uiKeyUsage [in] 指定使用签名私钥还是加密私钥
SGD_SM2_1：签名私钥
SGD_SM2_3：加密私钥
pucData[in] 缓冲区指针，用于存放外部输入的数据
uiDataLength[in] 输入的数据长度
pucSignature [out] 缓冲区指针，用于存放输出的签名值数据
返回值： 0 成功
非0 失败，返回错误代码
备注：
*/
int SDF_InternalDecrypt_ECC
(
	void		  *hSessionHandle,
	unsigned int  uiISKIndex,
	unsigned int  uiKeyUsage,
		ECCCipher	  *pucEncData,
	unsigned char *pucData,
	unsigned int  *puiDataLength
);

/*
原型：	int SDF_ImportKey (
void *hSessionHandle, 
unsigned char *pucKey, 
unsigned int uiKeyLength,
void **phKeyHandle);
描述：	导入明文会话密钥，同时返回密钥句柄
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucKey[in]	缓冲区指针，用于存放输入的密钥明文
	puiKeyLength[in]	输入的密钥明文长度
	phKeyHandle[out]	返回的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ImportKey 
(
	void *hSessionHandle, 
	unsigned char *pucKey, 
	unsigned int uiKeyLength,
	void **phKeyHandle
);

/*
原型：	int SDF_DestoryKey (
void *hSessionHandle, 
void *hKeyHandle);
描述：	销毁会话密钥，并释放为密钥句柄分配的内存等资源。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	hKeyHandle[in]	输入的密钥句柄
返回值：	0	成功
	非0	失败，返回错误代码
备注：	在对称算法运算完成后，应调用本函数销毁会话密钥。
*/
int SDF_DestroyKey (void *hSessionHandle, void *hKeyHandle);


/******************************非对称密码运算函数************************************/

/*
原型：	int SDF_ExternalPublicKeyOperation_RSA(
void *hSessionHandle, 
RSArefPublicKey *pucPublicKey,
unsigned char *pucDataInput,
unsigned int  uiInputLength,
unsigned char *pucDataOutput,
unsigned int  *puiOutputLength);
描述：	指定使用外部公钥对数据进行运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucPublicKey [in]	外部RSA公钥结构
	pucDataInput [in]	缓冲区指针，用于存放输入的数据
	uiInputLength[in]	输入的数据长度
	pucDataOutput[out]	缓冲区指针，用于存放输出的数据
	puiOutputLength[out]	输出的数据长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	数据格式由应用层封装
*/
int SDF_ExternalPublicKeyOperation_RSA
(
	void *hSessionHandle, 
	RSArefPublicKey *pucPublicKey,
	unsigned char *pucDataInput,
	unsigned int  uiInputLength,
	unsigned char *pucDataOutput,
	unsigned int  *puiOutputLength
);

/*
原型：	int SDF_ExternalPrivateKeyOperation_RSA(
void *hSessionHandle, 
RSArefPrivateKey *pucPrivateKey,
unsigned char *pucDataInput,
unsigned int  uiInputLength,
unsigned char *pucDataOutput,
unsigned int  *puiOutputLength);
描述：	指定使用外部私钥对数据进行运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucPrivateKey [in]	外部RSA私钥结构
	pucDataInput [in]	缓冲区指针，用于存放输入的数据
	uiInputLength [in]	输入的数据长度
	pucDataOutput [out]	缓冲区指针，用于存放输出的数据
	puiOutputLength [out]	输出的数据长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	数据格式由应用层封装
*/
int SDF_ExternalPrivateKeyOperation_RSA
(
	void *hSessionHandle, 
	RSArefPrivateKey *pucPrivateKey,
	unsigned char *pucDataInput,
	unsigned int  uiInputLength,
	unsigned char *pucDataOutput,
	unsigned int  *puiOutputLength
);

/*
原型：	int SDF_InternalPublicKeyOperation_RSA(
void *hSessionHandle,
unsigned int  uiKeyIndex,
unsigned char *pucDataInput,
unsigned int  uiInputLength,
unsigned char *pucDataOutput,
unsigned int  *puiOutputLength);
描述：	使用内部指定索引的公钥对数据进行运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备内部存储公钥的索引值
	pucDataInput[in]	缓冲区指针，用于存放外部输入的数据
	uiInputLength[in]	输入的数据长度
	pucDataOutput[out]	缓冲区指针，用于存放输出的数据
	puiOutputLength[out]	输出的数据长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	索引范围仅限于内部签名密钥对，数据格式由应用层封装
*/
int SDF_InternalPublicKeyOperation_RSA
(
	void *hSessionHandle,
	unsigned int  uiKeyIndex,
	unsigned char *pucDataInput,
	unsigned int  uiInputLength,
	unsigned char *pucDataOutput,
	unsigned int  *puiOutputLength
);

/*
原型：	int SDF_InternalPrivateKeyOperation_RSA(
void *hSessionHandle,
unsigned int  uiKeyIndex,
unsigned char *pucDataInput,
unsigned int  uiInputLength,
unsigned char *pucDataOutput,
unsigned int  *puiOutputLength);
描述：	使用内部指定索引的私钥对数据进行运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备内部存储私钥的索引值
	pucDataInput[in]	缓冲区指针，用于存放外部输入的数据
	uiInputLength[in]	输入的数据长度
	pucDataOutput[out]	缓冲区指针，用于存放输出的数据
	puiOutputLength[out]	输出的数据长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	索引范围仅限于内部签名密钥对，数据格式由应用层封装
*/
int SDF_InternalPrivateKeyOperation_RSA
(
	void *hSessionHandle,
	unsigned int  uiKeyIndex,
	unsigned char *pucDataInput,
	unsigned int  uiInputLength,
	unsigned char *pucDataOutput,
	unsigned int  *puiOutputLength
);

/*
原型：	int SDF_InternalPublicKeyOperation_RSA(
void *hSessionHandle,
unsigned int  uiKeyIndex,
unsigned char *pucDataInput,
unsigned int  uiInputLength,
unsigned char *pucDataOutput,
unsigned int  *puiOutputLength);
描述：	使用内部指定索引的公钥对数据进行运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备内部存储公钥的索引值
	pucDataInput[in]	缓冲区指针，用于存放外部输入的数据
	uiInputLength[in]	输入的数据长度
	pucDataOutput[out]	缓冲区指针，用于存放输出的数据
	puiOutputLength[out]	输出的数据长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	索引范围仅限于内部签名密钥对，数据格式由应用层封装
*/
int SDF_InternalPublicKeyOperation_RSA_Ex
(
	void *hSessionHandle,
	unsigned int  uiKeyIndex,
	unsigned int  uiKeyUsage,
	unsigned char *pucDataInput,
	unsigned int  uiInputLength,
	unsigned char *pucDataOutput,
	unsigned int  *puiOutputLength
);

/*
原型：	int SDF_InternalPrivateKeyOperation_RSA(
void *hSessionHandle,
unsigned int  uiKeyIndex,
unsigned char *pucDataInput,
unsigned int  uiInputLength,
unsigned char *pucDataOutput,
unsigned int  *puiOutputLength);
描述：	使用内部指定索引的私钥对数据进行运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiKeyIndex[in]	密码设备内部存储私钥的索引值
	pucDataInput[in]	缓冲区指针，用于存放外部输入的数据
	uiInputLength[in]	输入的数据长度
	pucDataOutput[out]	缓冲区指针，用于存放输出的数据
	puiOutputLength[out]	输出的数据长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	索引范围仅限于内部签名密钥对，数据格式由应用层封装
*/
int SDF_InternalPrivateKeyOperation_RSA_Ex
(
	void *hSessionHandle,
	unsigned int  uiKeyIndex,
	unsigned int uiKeyUsage,
	unsigned char *pucDataInput,
	unsigned int  uiInputLength,
	unsigned char *pucDataOutput,
	unsigned int  *puiOutputLength
);

/*
原型：	int SDF_ExternalSign_ECC(
void *hSessionHandle,
unsigned int uiAlgID,
ECCrefPrivateKey *pucPrivateKey,
unsigned char *pucData,
unsigned int  uiDataLength,
ECCSignature *pucSignature);
描述：	使用外部ECC私钥对数据进行签名运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiAlgID[in]	算法标识，指定使用的ECC算法
	pucPrivateKey[in]	外部ECC私钥结构
	pucData[in]	缓冲区指针，用于存放外部输入的数据
	uiDataLength[in]	输入的数据长度
	pucSignature[out]	缓冲区指针，用于存放输出的签名值数据
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExternalSign_ECC
(
	void *hSessionHandle,
	unsigned int uiAlgID,
	ECCrefPrivateKey *pucPrivateKey,
	unsigned char *pucData,
	unsigned int  uiDataLength,
	ECCSignature *pucSignature
);

/*
原型：	int SDF_ExternalVerify_ECC(
void *hSessionHandle,
unsigned int uiAlgID,
ECCrefPublicKey *pucPublicKey,
unsigned char *pucDataInput,
unsigned int  uiInputLength,
ECCSignature *pucSignature);
描述：	使用外部ECC公钥对ECC签名值进行验证运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiAlgID[in]	算法标识，指定使用的ECC算法
	pucPublicKey[in]	外部ECC公钥结构
	pucData[in]	缓冲区指针，用于存放外部输入的数据
	uiDataLength[in]	输入的数据长度
	pucSignature[in]	缓冲区指针，用于存放输入的签名值数据
返回值：	0	成功
	非0	失败，返回错误代码
备注：	对原文的杂凑运算，在函数外部完成。
*/
int SDF_ExternalVerify_ECC
(
	void *hSessionHandle,
	unsigned int uiAlgID,
	ECCrefPublicKey *pucPublicKey,
	unsigned char *pucDataInput,
	unsigned int  uiInputLength,
	ECCSignature *pucSignature
);


/*
原型：	int SDF_InternalSign_ECC(
void *hSessionHandle,
unsigned int  uiISKIndex,
unsigned char *pucData,
unsigned int  uiDataLength,
ECCSignature *pucSignature);
描述：	使用内部ECC私钥对数据进行签名运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiISKIndex [in]	密码设备内部存储的ECC签名私钥的索引值
	pucData[in]	缓冲区指针，用于存放外部输入的数据
	uiDataLength[in]	输入的数据长度
	pucSignature [out]	缓冲区指针，用于存放输出的签名值数据
返回值：	0	成功
	非0	失败，返回错误代码
备注：	对原文的杂凑运算，在函数外部完成。
*/
int SDF_InternalSign_ECC
(
	void *hSessionHandle,
	unsigned int  uiISKIndex,
	unsigned char *pucData,
	unsigned int  uiDataLength,
	ECCSignature *pucSignature
);

/*
原型：	int SDF_InternalVerify_ECC(
void *hSessionHandle,
unsigned int  uiISKIndex,
unsigned char *pucData,
unsigned int  uiDataLength,
ECCSignature *pucSignature);
描述：	使用内部ECC公钥对ECC签名值进行验证运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiISKIndex [in]	密码设备内部存储的ECC签名公钥的索引值
	pucData[in]	缓冲区指针，用于存放外部输入的数据
	uiDataLength[in]	输入的数据长度
	pucSignature[in]	缓冲区指针，用于存放输入的签名值数据
返回值：	0	成功
	非0	失败，返回错误代码
备注：	对原文的杂凑运算，在函数外部完成。
*/
int SDF_InternalVerify_ECC
(
	void *hSessionHandle,
	unsigned int  uiISKIndex,
	unsigned char *pucData,
	unsigned int  uiDataLength,
	ECCSignature *pucSignature
);

/*
原型：	int SDF_ExternalEncrytp_ECC(
void *hSessionHandle,
unsigned int uiAlgID,
ECCrefPublicKey *pucPublicKey,
unsigned char *pucData,
unsigned int  uiDataLength,
ECCCipher *pucEncData);
描述：	使用外部ECC公钥对数据进行加密运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiAlgID[in]	算法标识，指定使用的ECC算法
	pucPublicKey[in]	外部ECC公钥结构
	pucData[in]	缓冲区指针，用于存放外部输入的数据
	uiDataLength[in]	输入的数据长度
	pucEncData[out]	缓冲区指针，用于存放输出的数据密文
返回值：	0	成功
	非0	失败，返回错误代码
备注：	输入的数据长度uiDataLength不大于ECCref_MAX_LEN。
*/
int SDF_ExternalEncrypt_ECC
(
	void *hSessionHandle,
	unsigned int uiAlgID,
	ECCrefPublicKey *pucPublicKey,
	unsigned char *pucData,
	unsigned int  uiDataLength,
	ECCCipher *pucEncData
);

/*
原型：	int SDF_ExternalDecrypt_ECC(
void *hSessionHandle,
unsigned int uiAlgID,
ECCrefPrivateKey *pucPrivateKey,
ECCCipher *pucEncData,
unsigned char *pucData,
unsigned int  *puiDataLength);
描述：	使用外部ECC私钥进行解密运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiAlgID[in]	算法标识，指定使用的ECC算法
	pucPrivateKey[in]	外部ECC私钥结构
	pucEncData[in]	缓冲区指针，用于存放输入的数据密文
	pucData[out]	缓冲区指针，用于存放输出的数据明文
	puiDataLength[out]	输出的数据明文长度
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ExternalDecrypt_ECC
(
	void *hSessionHandle,
	unsigned int uiAlgID,
	ECCrefPrivateKey *pucPrivateKey,
	ECCCipher *pucEncData,
	unsigned char *pucData,
	unsigned int  *puiDataLength
);



/***************************对称密码运算函数**************************************/
/*
原型：	int SDF_Encrypt(
void *hSessionHandle,
void *hKeyHandle,
unsigned int uiAlgID,
unsigned char *pucIV,
unsigned char *pucData,
unsigned int uiDataLength,
unsigned char *pucEncData,
unsigned int  *puiEncDataLength);
描述：	使用指定的密钥句柄和IV对数据进行对称加密运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	hKeyHandle[in]	指定的密钥句柄
	uiAlgID[in]	算法标识，指定对称加密算法
	pucIV[in|out]	缓冲区指针，用于存放输入和返回的IV数据
	pucData[in]	缓冲区指针，用于存放输入的数据明文
	uiDataLength[in]	输入的数据明文长度
	pucEncData[out]	缓冲区指针，用于存放输出的数据密文
	puiEncDataLength[out]	输出的数据密文长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	此函数不对数据进行填充处理，输入的数据必须是指定算法分组长度的整数倍。
*/
int SDF_Encrypt
(
	void *hSessionHandle,
	void *hKeyHandle,
	unsigned int uiAlgID,
	unsigned char *pucIV,
	unsigned char *pucData,
	unsigned int uiDataLength,
	unsigned char *pucEncData,
	unsigned int  *puiEncDataLength
);

/*
原型：	int SDF_Decrypt (
void *hSessionHandle,
void *hKeyHandle,
unsigned int uiAlgID,
unsigned char *pucIV,
unsigned char *pucEncData,
unsigned int  uiEncDataLength,
unsigned char *pucData,
unsigned int *puiDataLength);
描述：	使用指定的密钥句柄和IV对数据进行对称解密运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	hKeyHandle[in]	指定的密钥句柄
	uiAlgID[in]	算法标识，指定对称加密算法
	pucIV[in|out]	缓冲区指针，用于存放输入和返回的IV数据
	pucEncData[in]	缓冲区指针，用于存放输入的数据密文
	uiEncDataLength[in]	输入的数据密文长度
	pucData[out]	缓冲区指针，用于存放输出的数据明文
	puiDataLength[out]	输出的数据明文长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	此函数不对数据进行填充处理，输入的数据必须是指定算法分组长度的整数倍。
*/
int SDF_Decrypt 
(
	void *hSessionHandle,
	void *hKeyHandle,
	unsigned int uiAlgID,
	unsigned char *pucIV,
	unsigned char *pucEncData,
	unsigned int  uiEncDataLength,
	unsigned char *pucData,
	unsigned int *puiDataLength
);

/*
原型：	int SDF_CalculateMAC(
void *hSessionHandle,
void *hKeyHandle,
unsigned int uiAlgID,
unsigned char *pucIV,
unsigned char *pucData,
unsigned int uiDataLength,
unsigned char *pucMAC,
unsigned int  *puiMACLength);
描述：	使用指定的密钥句柄和IV对数据进行MAC运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	hKeyHandle[in]	指定的密钥句柄
	uiAlgID[in]	算法标识，指定MAC加密算法
	pucIV[in|out]	缓冲区指针，用于存放输入和返回的IV数据
	pucData[in]	缓冲区指针，用于存放输出的数据明文
	uiDataLength[in]	输出的数据明文长度
	pucMAC[out]	缓冲区指针，用于存放输出的MAC值
	puiMACLength[out]	输出的MAC值长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	此函数不对数据进行分包处理，多包数据MAC运算由IV控制最后的MAC值。
*/
int SDF_CalculateMAC
(
	void *hSessionHandle,
	void *hKeyHandle,
	unsigned int uiAlgID,
	unsigned char *pucIV,
	unsigned char *pucData,
	unsigned int uiDataLength,
	unsigned char *pucMAC,
	unsigned int  *puiMACLength
);

/***************************杂凑运算函数************************************/

/*
原型：	int SDF_HashInit(
void *hSessionHandle,
unsigned int uiAlgID
ECCrefPublicKey *pucPublicKey,
unsigned char *pucID,
unsigned int uiIDLength);
描述：	三步式数据杂凑运算第一步。
参数：	hSessionHandle[in]	与设备建立的会话句柄
	uiAlgID[in]	指定杂凑算法标识
	pucPublicKey[in]	签名者的ECC公钥，产生用于ECC签名的杂凑值时有效
	pucID[in]	签名者的ID值，产生用于ECC签名的杂凑值时有效
	uiIDLength[in]	签名者的ID长度
返回值：	0	成功
	非0	失败，返回错误代码
备注：	如果在具体的应用中，协商双方没有统一分配的ID，可以将ID设定为常量。
*/
int SDF_HashInit
(
	void *hSessionHandle,
	unsigned int uiAlgID,
	ECCrefPublicKey *pucPublicKey,
	unsigned char *pucID,
	unsigned int uiIDLength
);

/*
原型：	int SDF_HashUpdate(
void *hSessionHandle,
unsigned char *pucData,
unsigned int  uiDataLength);
描述：	三步式数据杂凑运算第二步，对输入的明文进行杂凑运算
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucData[in]	缓冲区指针，用于存放输入的数据明文
	uiDataLength[in]	输入的数据明文长度
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_HashUpdate
(
	void *hSessionHandle,
	unsigned char *pucData,
	unsigned int  uiDataLength
);

/*
原型：	int SDF_HashFinal(
void *hSessionHandle,
unsigned char *pucHash,
unsigned int  *puiHashLength);
描述：	三步式数据杂凑运算第三步，杂凑运算结束返回杂凑数据并清除中间数据
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucHash[out]	缓冲区指针，用于存放输出的杂凑数据
	puiHashLength[out]	返回的杂凑数据长度
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_HashFinal
(
	void *hSessionHandle,
	unsigned char *pucHash,
	unsigned int  *puiHashLength
);

/*******************************用户文件操作函数********************************/

/*
原型：	int SDF_CreateFile(
void *hSessionHandle,
unsigned char *pucFileName，
unsigned int uiNameLen，
unsigned int uiFileSize);
描述：	在密码设备内部创建用于存储用户数据的文件
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucFileName[in]	缓冲区指针，用于存放输入的文件名，最大长度128字节
	uiNameLen[in]	文件名长度
	uiFileSize[in]	文件所占存储空间的长度
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_CreateFile
(
	void *hSessionHandle,
	unsigned char *pucFileName,
	unsigned int uiNameLen,
	unsigned int uiFileSize
);

/*
原型：	int SDF_ReadFile(
void *hSessionHandle,
unsigned char *pucFileName，
unsigned int uiNameLen，
unsigned int uiOffset,
unsigned int *puiFileLength,
unsigned char *pucBuffer);
描述：	读取在密码设备内部存储用户数据的文件的内容
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucFileName[in]	缓冲区指针，用于存放输入的文件名，最大长度128字节
	uiNameLen[in]	文件名长度
	uiOffset[in]	指定读取文件时的偏移值
	puiFileLength[in|out]	入参时指定读取文件内容的长度；出参时返回实际读取文件内容的长度
	pucBuffer[out]	缓冲区指针，用于存放读取的文件数据
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_ReadFile
(
	void *hSessionHandle,
	unsigned char *pucFileName,
	unsigned int uiNameLen,
	unsigned int uiOffset,
	unsigned int *puiFileLength,
	unsigned char *pucBuffer
);

/*
原型：	int SDF_WriteFile(
void *hSessionHandle,
unsigned char *pucFileName，
unsigned int uiNameLen，
unsigned int uiOffset,
unsigned int uiFileLength,
unsigned char *pucBuffer);
描述：	向密码设备内部存储用户数据的文件中写入内容
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucFileName[in]	缓冲区指针，用于存放输入的文件名，最大长度128字节
	uiNameLen[in]	文件名长度
	uiOffset[in]	指定写入文件时的偏移值
	uiFileLength[in]	指定写入文件内容的长度
	pucBuffer[in]	缓冲区指针，用于存放输入的写文件数据
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_WriteFile
(
	void *hSessionHandle,
	unsigned char *pucFileName,
	unsigned int uiNameLen,
	unsigned int uiOffset,
	unsigned int uiFileLength,
	unsigned char *pucBuffer
);

/*
原型：	int SDF_DeleteFile(
void *hSessionHandle,
unsigned char *pucFileName，
unsigned int uiNameLen);
描述：	删除指定文件名的密码设备内部存储用户数据的文件
参数：	hSessionHandle[in]	与设备建立的会话句柄
	pucFileName[in]	缓冲区指针，用于存放输入的文件名，最大长度128字节
	uiNameLen[in]	文件名长度
返回值：	0	成功
	非0	失败，返回错误代码
*/
int SDF_DeleteFile
(
	void *hSessionHandle,
	unsigned char *pucFileName,
	unsigned int uiNameLen
);

/*福建凯特定制
原型：
int SDF_ImportECCKeyPair 
(
	void *hSessionHandle, 
	unsigned int uiISKIndex, 
	ENVELOPEDKEYBLOB *pEnvelopedKeyBlob 
)
描述：	导入ECC加密密钥对
参数：	hSessionHandle[in]		与设备建立的会话句柄
		uiISKIndex[in]			密码设备内部存储ECC加密密钥对的索引值
		pEnvelopedKeyBlob[in]	ECC密钥对的对保护结构
返回值：	0	成功
		非0	失败，返回错误代码
备注：从pEnvelopedKeyBlob 中解密出ECC密钥对，导入到uiISKIndex的位置。
*/
int SDF_ImportECCKeyPair 
(
	void *hSessionHandle, 
	unsigned int uiISKIndex, 
	ENVELOPEDKEYBLOB *pEnvelopedKeyBlob 
);
/*
int SDF_GenEnvBlob
(
    void *hSessionHandle,
	unsigned long  SymmAlgID,
    ECCrefPublicKey *pucPublicKey,
    ENVELOPEDKEYBLOB *pEnvelopedKeyBlob

 )
 描述：	生成ECC密钥对的对保护结构
 参数：	hSessionHandle[in]		与设备建立的会话句柄
 SymmAlgID[in]    对称算法标识
 pucPublicKey[in]	ECC0号签名公钥结构
 pEnvelopedKeyBlob[out]	ECC密钥对的对保护结构
 返回值：	0	成功
		非0	失败，返回错误代码
*/
int SDF_GenEnvBlob
(
 void *hSessionHandle,
 unsigned long  SymmAlgID,
 ECCrefPublicKey *pucPublicKey,
 ENVELOPEDKEYBLOB *pEnvelopedKeyBlob
 );
/*
int SDF_ImportECCEncKeyPair 
(
	void *hSessionHandle,  
	ENVELOPEDKEYBLOB *pEnvelopedKeyBlob 
)
描述：	导入ECC加密密钥对
参数：	hSessionHandle[in]		与设备建立的会话句柄
		pEnvelopedKeyBlob[in]	ECC密钥对的对保护结构
返回值：	0	成功
		非0	失败，返回错误代码

*/
int SDF_ImportECCEncKeyPair 
(
	void *hSessionHandle, 
	ENVELOPEDKEYBLOB *pEnvelopedKeyBlob 
);

#ifdef __cplusplus
}
#endif

#endif 

