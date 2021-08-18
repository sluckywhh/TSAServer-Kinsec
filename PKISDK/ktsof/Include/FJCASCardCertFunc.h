/*************************************************************
 *				福建省数字安全证书管理有限公司				 * 
 *					 数字证书接口函数说明					 * 
 *				   适用操作系统:Windows环境 				 *
 *************************************************************/
#ifndef _FJCASCARDCERTFUNC_H_
#define _FJCASCARDCERTFUNC_H_

#ifndef FJCA_COMMON_DEFINE
#define FJCA_COMMON_DEFINE

/*
 *签名函数中的摘要算法
 */
//#define calg_md2	1
//#define calg_md4	2
#define calg_md5	3
#define calg_sha1	4
#define calg_sm3	5
#define calg_sha256 6

/*
 *设备类型
 */
#define DEVTYPE_MH	1				//明华
#define DEVTYPE_WQ	2				//握奇
#define DEVTYPE_ZC	3				//中钞
#define DEVTYPE_HB	4				//恒宝
#define DEVTYPE_FT	5				//飞天
#define DEVTYPE_HT	6				//海泰

typedef HANDLE		DEVHANDLE;

/*
*临界值定义
*/
#define MAX_IV_LEN			32		//初始化向量的最大长度
#define MAX_FILE_NAME_LEN	32		//文件名最大长度
#define MAX_CONTAINER_NAME_LEN	128 //容器名最大长度

/*
 *算法标识符
 */
#define SGD_SM1_ECB 	0x00000101	//SM1算法ECB加密模式
#define SGD_SM1_CBC 	0x00000102	//SM1算法CBC加密模式
#define SGD_SM1_CFB 	0x00000104	//SM1算法CFB加密模式
#define SGD_SM1_OFB 	0x00000108	//SM1算法OFB加密模式
#define SGD_SM1_MAC 	0x00000110	//SM1算法MAC运算
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
/*	0x00000400-0x800000xx	为其它分组密码算法预留	*/

#define SGD_RSA 		0x00010000	//RSA算法
#define SGD_SM2_1		0x00020100	//椭圆曲线签名算法
#define SGD_SM2_2		0x00020200	//椭圆曲线密钥交换协议
#define SGD_SM2_3		0x00020400	//椭圆曲线加密算法
/*	0x00000400～0x800000xx	为其它非对称密码算法预留	*/

#define SGD_SM3 		0x00000001	//SM3杂凑算法
#define SGD_SHA1		0x00000002	//SHA1杂凑算法
#define SGD_SHA256		0x00000004	//SHA256杂凑算法
/*	0x00000010～0x000000FF	为其它密码杂凑算法预留	*/

/*
 *权限类型
 */
#define SECURE_NEVER_ACCOUNT	0x00000000		//不允许
#define SECURE_ADM_ACCOUNT		0x00000001		//管理员权限
#define SECURE_USER_ACCOUNT 	0x00000010		//用户权限
#define SECURE_EVERYONE_ACCOUNT 0x000000FF		//任何人

#pragma pack(1)

/*
 *分组密码参数
 */
typedef struct Struct_BLOCKCIPHERPARAM{
	BYTE	IV[MAX_IV_LEN]; 		//初始向量，MAX_IV_LEN为初始向量的最大长度
	ULONG	IVLen;					//初始向量实际长度，按字节计算
	ULONG	PaddingType;			//填充方式，0表示不填充，1表示按照PKCS#5方式进行填充
	ULONG	FeedBitLen; 			//反馈值的位长度，按字节计算，只针对OFB、CFB模式
} BLOCKCIPHERPARAM, *PBLOCKCIPHERPARAM;

/*
 *文件属性
 */
typedef struct Struct_FILEATTRIBUTE{
	CHAR	FileName[32];			//文件名
	ULONG	FileSize;				//文件大小
	ULONG	ReadRights; 			//读权限
	ULONG	WriteRights;			//写权限
} FILEATTRIBUTE, *PFILEATTRIBUTE;

#pragma pack()

#endif

#ifdef __cplusplus
extern "C"{
#endif

/************************************************************************/
/*	1. 设备管理 														*/
/*	FJCA_SCardGetDevStateX												*/
/*	FJCA_SCardGetDevState												*/
/*	FJCA_InitSCard														*/
/*	FJCA_GetKeyType														*/
/*	FJCA_GetDevType														*/
/*	FJCA_ExitSCard														*/
/*	FJCA_SCardGetDeviceInfo 											*/
/*	FJCA_SCardGetRandom 												*/
/************************************************************************/

/*
 *	返回指定设备的状态值。
 *	nDevType			设备类型
 *	pbState 			设备状态值
 *	备注：
		对每种类型的设备，插入的Key序号（索引号）从0开始依次增加。
		新插入的Key使用空闲序号。若没有空闲序号，则使用新的序号。
		若有Key被拔出，其序号空闲。
		若Key未拔出，其序号始终保持不变。
		举例如下：
		插入三个Key，序号依次为0、1、2，该函数返回时：
			pbState[0] = 0x31;
			pbState[1] = 0x31;
			pbState[2] = 0x31;
			pbState[3] = 0x0;
			pbState[4] = 0x0;
			pbState[5] = 0x0;
			pbState[6] = 0x0;
			pbState[7] = 0x0;
		新插入一个Key，则其序号为3，该函数返回时：
			pbState[0] = 0x31;
			pbState[1] = 0x31;
			pbState[2] = 0x31;
			pbState[3] = 0x31;
			pbState[4] = 0x0;
			pbState[5] = 0x0;
			pbState[6] = 0x0;
			pbState[7] = 0x0;
		拔出第二、三个Key（序号为1、2），该函数返回时：
			pbState[0] = 0x31;
			pbState[1] = 0x0;
			pbState[2] = 0x0;
			pbState[3] = 0x31;
			pbState[4] = 0x0;
			pbState[5] = 0x0;
			pbState[6] = 0x0;
			pbState[7] = 0x0;
		新插入一个Key，则其序号为1，该函数返回时：
			pbState[0] = 0x31;
			pbState[1] = 0x31;
			pbState[2] = 0x0;
			pbState[3] = 0x31;
			pbState[4] = 0x0;
			pbState[5] = 0x0;
			pbState[6] = 0x0;
			pbState[7] = 0x0;
  */
BOOL FJCA_SCardGetDevStateX(
	IN int				nDevType,
	OUT BYTE			*pbState
	);

/*
 *	返回所有设备的状态值。
 *	byState 			设备状态值
 *	pbState 			设备状态值
 *	备注：
		byState为二维数组，数组的每一行表示一种设备的状态，如：
			byState[0]为明华的状态
			byState[1]为握奇的状态
			以此类推。
		pbState为一维数组：
			pbState[0]－pbState[7]为明华的状态
			pbState[8]－pbState[15]为握奇的状态
			以此类推。
 */
BOOL FJCA_SCardGetDevState(
	OUT BYTE			byState[][8]
	);
BOOL FJCA_SCardGetDevState2(
	OUT BYTE			*pbState
	);

/*
 *	连接设备，返回设备的句柄。
 *	nDevType			设备类型
 *	szDevName			对于USB读写器和Key可指定要打开的智能卡读写设备
						szDevName为USB1、USB2 ...
						USBX，X为Key序号加1，先插入的设备为USB1，后插入的为USB2
 *	phDev				返回设备操作句柄
 *	备注：				若nDevType使用缺省参数，则按设备类型顺序（明华、握奇...）依次寻找设备，并连接找到的第一个设备。
 */
BOOL FJCA_InitSCard(
	IN int				nDevType = 0,
	IN char 			*szDevName = "USB1",
	OUT HANDLE			*phDev = NULL
	);

/*
 *	获取设备类型。
 *	hDev				连接设备时返回的设备句柄
 */
int FJCA_GetKeyType(
	IN HANDLE			hDev = NULL
	);

int FJCA_GetDevType(
	IN HANDLE			hDev = NULL
	);

/*
 *	断开一个已经连接的设备，并释放句柄。
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_ExitSCard(
	IN HANDLE			hDev = NULL
	);

/*
 *	取设备信息。
 *	szDeviceInfo		设备的详细信息。若无，可返回空串
 *	szVID				Vender ID，即厂商编号，长度4字节。如："D6A3"
 *	szPID				Product ID，即产品编号，长度4字节。如："2599"
 *	szSN				Serial Number，即产品唯一序列号。如："03081612130000001895"
 *	szPD				Produce Date，即生产日期，长度8字节。如："20061213"
 *	szPI				附加信息。若无，可返回空串
 *	szAF				预留。若无，可返回空串
 *	szLabel 			标签
 *	pdwTotalSpace		设备存储空间
 *	pdwFreeSpace		设备剩余空间
 *	hDev				连接设备时返回的设备句柄
 *	备注：				返回的各个字符串参数必须只包含可打印字符。
						厂家应保证，对每个Key，pszVID、pszPID、pszSN这3个串相连的结果是唯一的。
 */
BOOL FJCA_SCardGetDeviceInfo(
	OUT char			*szDeviceInfo,
	OUT char			*szVID,
	OUT char			*szPID,
	OUT char			*szSN,
	OUT char			*szPD,
	OUT char			*szPI,
	OUT char			*szAF,
	OUT char			*szLabel,
	OUT DWORD			*pdwTotalSpace,
	OUT DWORD			*pdwFreeSpace,
	IN HANDLE			hDev = NULL
	);

/*
 *	产生随机数。
 *	dwRandomDataLen 	随机数的长度
 *	pbRandomData		随机数
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardGetRandom(
	IN DWORD			dwRandomDataLen,
	OUT BYTE			*pbRandomData,
	IN HANDLE			hDev = NULL
	);

/************************************************************************/
/*	2. 访问控制 														*/
/*	FJCA_SCardInitKey													*/
/*	FJCA_SCardVerifyUserPin 											*/
/*	FJCA_SCardVerifyAdminPin											*/
/*	FJCA_SCardChangeUserPin 											*/
/*	FJCA_SCardChangeAdminPin											*/
/*	FJCA_SCardUnlockPin 												*/
/*	FJCA_GetPinTimes	 												*/
/************************************************************************/

/*
 *	初始化Key。
 *	pbUserPin				用户口令
 *	byUserPinLen			用户口令的长度
 *	dwUserPinMaxRetryCount	用户口令最大重试次数
 *	pbAdminPin				管理员口令
 *	byAdminPinLen			管理员口令的长度
 *	dwAdminPinMaxRetryCount 管理员口令最大重试次数
 *	hDev					连接设备时返回的设备句柄
 */
BOOL FJCA_SCardInitKey(
	IN BYTE 				*pbUserPin,
	IN BYTE 				byUserPinLen,
	IN DWORD				dwUserPinMaxRetryCount,
	IN BYTE 				*pbAdminPin,
	IN BYTE 				byAdminPinLen,
	IN DWORD				dwAdminPinMaxRetryCount,
	IN HANDLE				hDev = NULL
	);

/*
 *	核对用户口令。
 *	pbUserPin			用户口令
 *	byUserPinLen		用户口令的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				若校验口令失败，请在函数返回前调用SetLastError设置错误码：
							0x6983		Key已被锁定
							0x63CX		X为Key口令还可尝试次数
 */
BOOL FJCA_SCardVerifyUserPin(
	IN BYTE 			*pbUserPin,
	IN BYTE 			byUserPinLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	核对管理员口令。
 *	pbAdminPin			管理员口令
 *	byAdminPinLen		管理员口令的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				若校验口令失败，请在函数返回前调用SetLastError设置错误码：
							0x6983		Key已被锁定
							0x63CX		X为Key口令还可尝试次数
 */
BOOL FJCA_SCardVerifyAdminPin(
	IN BYTE 			*pbAdminPin,
	IN BYTE 			byAdminPinLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	核对并修改用户口令。
 *	pbOldUserPin		旧用户口令
 *	byOldUserPinLen 	旧用户口令的长度
 *	pbNewUserPin		新用户口令
 *	byNewUserPinLen 	新用户口令的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				若校验口令失败，请在函数返回前调用SetLastError设置错误码：
							0x6983		Key已被锁定
							0x63CX		X为Key口令还可尝试次数
 */
BOOL FJCA_SCardChangeUserPin(
	IN BYTE 			*pbOldUserPin,
	IN BYTE 			byOldUserPinLen,
	IN BYTE 			*pbNewUserPin,
	IN BYTE 			byNewUserPinLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	核对并修改管理员口令。
 *	pbOldAdminPin		旧管理员口令
 *	byOldAdminPinLen	旧管理员口令的长度
 *	pbNewAdminPin		新管理员口令
 *	byNewAdminPinLen	新管理员口令的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				若校验口令失败，请在函数返回前调用SetLastError设置错误码：
							0x6983		Key已被锁定
							0x63CX		X为Key口令还可尝试次数
 */
BOOL FJCA_SCardChangeAdminPin(
	IN BYTE 			*pbOldAdminPin,
	IN BYTE 			byOldAdminPinLen,
	IN BYTE 			*pbNewAdminPin,
	IN BYTE 			byNewAdminPinLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	当用户口令锁死后，通过调用该函数来解锁用户口令。
	解锁后，用户口令被设置成新值，用户口令的重试次数也恢复到原值。
 *	pbAdminPin			管理员口令
 *	byAdminPinLen		管理员口令的长度
 *	pbNewUserPin		新用户口令
 *	byNewUserPinLen 	新用户口令的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				若校验口令失败，请在函数返回前调用SetLastError设置错误码：
							0x6983		Key已被锁定
							0x63CX		X为Key口令还可尝试次数
 */
BOOL FJCA_SCardUnlockPin(
	IN BYTE 			*pbAdminPin,
	IN BYTE 			byAdminPinLen,
	IN BYTE 			*pbNewUserPin,
	IN BYTE 			byNewUserPinLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	以上口令相关函数调用失败时，获取口令还可尝试次数。
 *	pbPinTimes			口令还可尝试次数
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_GetPinTimes(
	OUT BYTE			*pbPinTimes,
	IN HANDLE			hDev = NULL
	);

/************************************************************************/
/*	3. 文件管理 														*/
/*	FJCA_SCardCreateFile												*/
/*	FJCA_SCardDeleteFile												*/
/*	FJCA_SCardEnumFiles 												*/
/*	FJCA_SCardGetFileInfo												*/
/*	FJCA_SCardReadFile													*/
/*	FJCA_SCardWriteFile 												*/
/************************************************************************/

/*
 *	创建一个文件。
 *	szFileName			文件名称，长度不得大于32个字节
 *	ulFileSize			文件大小
 *	ulReadRights		文件读权限
 *	ulWriteRights		文件写权限
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardCreateFile(
	IN CHAR 			*szFileName,
	IN ULONG			ulFileSize,
	IN ULONG			ulReadRights,
	IN ULONG			ulWriteRights,
	IN HANDLE			hDev = NULL
	);

/*
 *	删除指定文件。文件删除后，文件中写入的所有信息将丢失。文件在设备中的占用的空间将被释放。
 *	szFileName			要删除文件的名称
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDeleteFile(
	IN CHAR 			*szFileName,
	IN HANDLE			hDev = NULL
	);

/*
 *	枚举所有文件。
 *	szFileList			返回文件名称列表
						每个文件名称以单个'\0'结束，以双'\0'表示列表的结束
 *	pulSize 			输入为数据缓冲区的长度，输出为实际文件名称的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardEnumFiles(
	OUT CHAR			*szFileList,
	IN OUT ULONG		*pulSize,
	IN HANDLE			hDev = NULL
	);

/*
 *	获取文件的属性信息，例如文件的大小、权限等。
 *	szFileName			文件名称
 *	pFileInfo			文件信息，指向文件属性结构的指针
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardGetFileInfo(
	IN CHAR 			*szFileName,
	OUT FILEATTRIBUTE	*pFileInfo,
	IN HANDLE			hDev = NULL
	);

/*
 *	读取文件内容。
 *	szFileName			文件名
 *	ulOffset			读取文件的偏移位置
 *	ulSize				要读取的长度
 *	pbData				读取数据
 *	pulDataLen			输入为数据缓冲区的长度，输出为实际读取数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardReadFile(
	IN CHAR 			*szFileName,
	IN ULONG			ulOffset,
	IN ULONG			ulSize,
	OUT BYTE			*pbData,
	IN OUT ULONG		*pulDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	写数据到文件中。
 *	szFileName			文件名
 *	ulOffset			写入文件的偏移量
 *	pbData				写入数据
 *	ulSize				写入数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardWriteFile(
	IN CHAR 			*szFileName,
	IN ULONG			ulOffset,
	IN BYTE 			*pbData,
	IN ULONG			ulSize,
	IN HANDLE			hDev = NULL
	);

/************************************************************************/
/*	4. 证书操作 														*/
/*	FJCA_SCardGetCertNo 												*/
/*	FJCA_SCardSetCertNo 												*/
/*	FJCA_SCardReadCert													*/
/*	FJCA_SCardWriteCert 												*/
/*	FJCA_SCardGetContainerName											*/
/*	FJCA_SCardSetupIECert												*/
/************************************************************************/

/*
 *	返回卡内存放的证书状态:
		0				卡内无证书
		1				卡内存在第一张证书 
		2				卡内存在第二张证书	 
		3				卡内存在二张证书
 *	pbCertNo			证书状态
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardGetCertNo(
	OUT BYTE			*pbCertNo,
	IN HANDLE			hDev = NULL
	);

/*
 *	选择当前操作的证书，隐含为第一张证书。
 *	byCertNo			证书号（1-2）
						当Key内存放双证书时，第一张证书为签名证书，第二张证书为加密证书。
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardSetCertNo(
	IN BYTE 			byCertNo,
	IN HANDLE			hDev = NULL
	);

/*
 *	从智能卡或Key中读取当前证书（DER编码）。
 *	pbCert				证书数据
 *	pdwCertLen			输入为证书数据缓冲区的长度，输出为实际证书数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardReadCert(
	OUT BYTE			*pbCert,
	IN OUT DWORD		*pdwCertLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	把DER编码的证书写入智能卡或Key中。
 *	pbCert				证书数据
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardWriteCert(
	IN BYTE 			*pbCert,
	IN HANDLE			hDev = NULL
	);

/*
 *	取当前证书所在容器的名称。
 *	szContainerName 	证书容器名称
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardGetContainerName(
	OUT char			*szContainerName,
	IN HANDLE			hDev = NULL
	);

/*
 *	当有Key插入时，弹出口令框提示用户输入口令。若口令校验成功，Key内证书自动注册到IE。
 *	当有Key拔出时，Key内证书从IE中删除。
 *	nDevType			设备类型
 *	pnKeyStatus 		Key的状态
 *	pnKeyIndex			Key的序号，从0开始
 *	pbOperation 		是否有证书操作
 *	pszPin				用户输入的口令
 *	备注：
 *		调用者在WM_DEVICECHANGE消息处理函数中调用FJCA_SCardSetupIECert。
 *		如果插拔的不是本厂家Key，*pnKeyStatus = 0
 *		如果有本厂家Key插入，*pnKeyStatus = 1，*pnKeyIndex存放Key的序号。
 *			如果用户证书被导入IE，*pbOperation = TRUE, pszPin存放用户输入的口令。
 *		如果有本厂家Key拔出，*pnKeyStatus = 2，*pnKeyIndex存放Key的序号。
 *			如果用户证书从IE中删除，*pbOperation = TRUE。
 */
BOOL FJCA_SCardSetupIECert(
	IN int				nDevType,
	OUT int 			*pnKeyStatus,
	OUT int 			*pnKeyIndex,
	OUT BOOL			*pbOperation,
	OUT char			*pszPin
	);

/************************************************************************/
/*	5. RSA运算															*/
/*	FJCA_SCardGenRSAKey 												*/
/*	FJCA_SCardGetPubKey 												*/
/*	FJCA_SCardImportRSAKey												*/
/*	FJCA_SCardSign														*/
/*	FJCA_SCardSignDigest												*/
/*	FJCA_SCardVerifyDigest												*/
/*	FJCA_SCardRSAEncrypt												*/
/*	FJCA_SCardRSADecrypt												*/
/************************************************************************/

/*
 *	生成RSA密钥对，并输出公钥（DER编码）。
 *	pbPubKey			公钥数据
 *	pdwPubKeyLen		输入为公钥数据缓冲区的长度，输出为实际公钥数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardGenRSAKey(
	OUT BYTE			*pbPubKey,
	IN OUT DWORD		*pdwPubKeyLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	读RSA公钥数据（DER编码）。
 *	pbPubKey			公钥数据
 *	pdwPubKeyLen		输入为公钥数据缓冲区的长度，输出为实际公钥数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardGetPubKey(
	OUT BYTE			*pbPubKey,
	IN OUT DWORD		*pdwPubKeyLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	导入RSA解密私钥。
 *	pbPriKey			使用对称密钥保护的RSA解密私钥
 *	dwPriKeyLen 		使用对称密钥保护的RSA解密私钥的长度
 *	pbSymmKey			使用签名公钥保护的对称密钥
 *	dwSymmKeyLen		使用签名公钥保护的对称密钥的长度
 *	dwSymmAlgID 		对称算法
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardImportRSAKey(
	IN BYTE 			*pbPriKey,
	IN DWORD			dwPriKeyLen,
	IN BYTE 			*pbSymmKey,
	IN DWORD			dwSymmKeyLen,
	IN DWORD			dwSymmAlgID,
	IN HANDLE			hDev = NULL
	);

/*
 *	用RSA私钥对数据进行加密。
 *	pbData				待加密数据
 *	dwDataLen			待加密数据的长度
 *	pbSignedData		加密数据
 *	pdwSignedDataLen	输入为加密数据缓冲区的长度，输出为实际加密数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardSign(
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	OUT BYTE			*pbSignedData,
	IN OUT DWORD		*pdwSignedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	用RSA私钥对数据进行签名。
 *	pbData				待签名数据
 *	dwDataLen			待签名数据的长度
 *	byHashAlgID 		摘要算法
 *	pbSignedData		签名数据
 *	pdwSignedDataLen	输入为签名数据缓冲区的长度，输出为实际签名数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardSignDigest(
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	IN BYTE 			byHashAlgID,
	OUT BYTE			*pbSignedData,
	IN OUT DWORD		*pdwSignedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	验证RSA数据签名。
 *	pbSignedData		签名数据
 *	dwSignedDataLen 	签名数据的长度
 *	byHashAlgID 		摘要算法
 *	pbData				待签名数据
 *	dwDataLen			待签名数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardVerifyDigest(
	IN BYTE 			*pbSignedData,
	IN DWORD			dwSignedDataLen,
	IN BYTE 			byHashAlgID,
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	RSA数据加密。
 *	pbData				待加密数据
 *	dwDataLen			待加密数据的长度
 *	pbEncryptedData 	加密数据
 *	pdwEncryptedDataLen 输入为加密数据缓冲区的长度，输出为实际加密数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardRSAEncrypt(
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	OUT BYTE			*pbEncryptedData,
	IN OUT DWORD		*pdwEncryptedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	用RSA私钥对数据进行解密。
 *	pbEncryptedData 	已加密数据
 *	dwEncryptedDataLen	已加密数据的长度
 *	pbDecryptedData 	解密数据
 *	pdwDecryptedDataLen 输入为解密数据缓冲区的长度，输出为实际解密数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardRSADecrypt(
	IN BYTE 			*pbEncryptedData,
	IN DWORD			dwEncryptedDataLen,
	OUT BYTE			*pbDecryptedData,
	IN OUT DWORD		*pdwDecryptedDataLen,
	IN HANDLE			hDev = NULL
	);

/************************************************************************/
/*	6. 分组密码算法运算 												*/
/*	FJCA_SCardSetSymmKey												*/
/*	FJCA_SCardEncryptInit												*/
/*	FJCA_SCardEncrypt													*/
/*	FJCA_SCardEncryptUpdate 											*/
/*	FJCA_SCardEncryptFinal												*/
/*	FJCA_SCardDecryptInit												*/
/*	FJCA_SCardDecrypt													*/
/*	FJCA_SCardDecryptUpdate 											*/
/*	FJCA_SCardDecryptFinal												*/
/*	FJCA_SCardCloseHandle												*/
/************************************************************************/

/*
 *	设置明文对称密钥，返回密钥句柄。
 *	pbKey				会话密钥值
 *	ulAlgID 			会话密钥的算法标识
 *	phKey				返回会话密钥句柄
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardSetSymmKey(
	IN BYTE 			*pbKey,
	IN ULONG			ulAlgID,
	OUT HANDLE			*phKey,
	IN HANDLE			hDev = NULL
	);

/*
 *	数据加密初始化。设置数据加密的算法相关参数。
 *	hKey				加密密钥句柄
 *	EncryptParam		分组密码算法相关参数
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardEncryptInit(
	IN HANDLE			hKey,
	IN BLOCKCIPHERPARAM EncryptParam,
	IN HANDLE			hDev = NULL
	);

/*
 *	单一分组数据的加密操作。
	用指定加密密钥对指定数据进行加密，被加密的数据只包含一个分组，加密后的密文保存到指定的缓冲区中。
	SKF_Encrypt只对单个分组数据进行加密。
	在调用SKF_Encrypt之前，必须调用SKF_EncryptInit初始化加密操作。
	SKF_Encypt等价于先调用SKF_EncryptUpdate再调用SKF_EncryptFinal。
 *	hKey				加密密钥句柄
 *	pbData				待加密数据
 *	ulDataLen			待加密数据长度
 *	pbEncryptedData 	加密数据
 *	pulEncryptedDataLen 输入为加密数据缓冲区的长度，输出为实际加密数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardEncrypt(
	IN HANDLE			hKey,
	IN BYTE 			*pbData,
	IN ULONG			ulDataLen,
	OUT BYTE			*pbEncryptedData,
	IN OUT ULONG		*pulEncryptedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	多个分组数据的加密操作。
	用指定加密密钥对指定数据进行加密，被加密的数据包含多个分组，加密后的密文保存到指定的缓冲区中。
	SKF_EncryptUpdate对多个分组数据进行加密。
	在调用SKF_EncryptUpdate之前，必须调用SKF_EncryptInit初始化加密操作；
	在调用SKF_EncryptUpdate之后，必须调用SKF_EncryptFinal结束加密操作。
 *	hKey				加密密钥句柄
 *	pbData				待加密数据
 *	ulDataLen			待加密数据长度
 *	pbEncryptedData 	加密数据
 *	pulEncryptedDataLen 输入为加密数据缓冲区的长度，输出为实际加密数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardEncryptUpdate(
	IN HANDLE			hKey,
	IN BYTE 			*pbData,
	IN ULONG			ulDataLen,
	OUT BYTE			*pbEncryptedData,
	IN OUT ULONG		*pulEncryptedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	结束多个分组数据的加密，返回剩余加密结果。
	先调用SKF_EncryptInit初始化加密操作，
	再调用SKF_EncryptUpdate对多个分组数据进行加密，
	最后调用SKF_EncryptFinal结束多个分组数据的加密。
 *	hKey				加密密钥句柄
 *	pbData				待加密数据
 *	ulDataLen			待加密数据长度
 *	pbEncryptedData 	加密数据
 *	pulEncryptedDataLen 输入为加密数据缓冲区的长度，输出为实际加密数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardEncryptFinal(
	IN HANDLE			hKey,
	IN BYTE 			*pbData,
	IN ULONG			ulDataLen,
	OUT BYTE			*pbEncryptedData,
	IN OUT ULONG		*pulEncryptedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	数据解密初始化，设置解密密钥相关参数。
	调用SKF_DecryptInit之后，可以调用SKF_Decrypt对单个分组数据进行解密，
	也可以多次调用SKF_DecryptUpdate之后再调用SKF_DecryptFinal完成对多个分组数据的解密。
 *	hKey				解密密钥句柄
 *	DecryptParam		分组密码算法相关参数
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDecryptInit(
	IN HANDLE			hKey,
	IN BLOCKCIPHERPARAM DecryptParam,
	IN HANDLE			hDev = NULL
	);

/*
 *	单个分组数据的解密操作。
	用指定解密密钥对指定数据进行解密，被解密的数据只包含一个分组，解密后的明文保存到指定的缓冲区中。
	SKF_Decrypt只对单个分组数据进行解密。
	在调用SKF_Decrypt之前，必须调用SKF_DecryptInit初始化解密操作。
	SKF_Decypt等价于先调用SKF_DecryptUpdate再调用SKF_DecryptFinal。
 *	hKey				解密密钥句柄
 *	pbEncryptedData 	待解密数据
 *	ulEncryptedDataLen	待解密数据长度
 *	pbData				解密数据
 *	pulDataLen			输入为解密数据缓冲区的长度，输出为实际解密数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDecrypt(
	IN HANDLE			hKey,
	IN BYTE 			*pbEncryptedData,
	IN ULONG			ulEncryptedDataLen,
	OUT BYTE			*pbData,
	IN OUT ULONG		*pulDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	多个分组数据的解密操作。
	用指定解密密钥对指定数据进行解密，被解密的数据包含多个分组，解密后的明文保存到指定的缓冲区中。
	SKF_DecryptUpdate对多个分组数据进行解密。
	在调用SKF_DecryptUpdate之前，必须调用SKF_DecryptInit初始化解密操作；
	在调用SKF_DecryptUpdate之后，必须调用SKF_DecryptFinal结束解密操作。
 *	hKey				解密密钥句柄
 *	pbEncryptedData 	待解密数据
 *	ulEncryptedDataLen	待解密数据长度
 *	pbData				解密数据
 *	pulDataLen			输入为解密数据缓冲区的长度，输出为实际解密数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDecryptUpdate(
	IN HANDLE			hKey,
	IN BYTE 			*pbEncryptedData,
	IN ULONG			ulEncryptedDataLen,
	OUT BYTE			*pbData,
	IN OUT ULONG		*pulDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	结束多个分组数据的解密。
 *	hKey				解密密钥句柄
 *	pbEncryptedData 	待解密数据
 *	ulEncryptedDataLen	待解密数据长度
 *	pbData				解密数据
 *	pulDataLen			输入为解密数据缓冲区的长度，输出为实际解密数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDecryptFinal(
	IN HANDLE			hKey,
	IN BYTE 			*pbEncryptedData,
	IN ULONG			ulEncryptedDataLen,
	OUT BYTE			*pbData,
	IN OUT ULONG		*pulDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	关闭会话密钥、杂凑、消息认证码句柄。
 *	hHandle 			要关闭的对象句柄
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardCloseHandle(
	IN HANDLE			hHandle,
	IN HANDLE			hDev = NULL
	);

/************************************************************************/
/*	7. 杂凑、消息认证码运算 											*/
/*	FJCA_SCardDigestInit												*/
/*	FJCA_SCardDigest													*/
/*	FJCA_SCardDigestUpdate												*/
/*	FJCA_SCardDigestFinal												*/
/*	FJCA_SCardMacInit													*/
/*	FJCA_SCardMac														*/
/*	FJCA_SCardMacUpdate 												*/
/*	FJCA_SCardMacFinal													*/
/************************************************************************/

/*
 *	初始化消息杂凑计算操作，指定计算消息杂凑的算法。
 *	ulAlgID 			杂凑算法标识
 *	phHash				杂凑对象句柄
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDigestInit(
	IN ULONG			ulAlgID,
	OUT HANDLE			*phHash,
	IN HANDLE			hDev = NULL
	);

/*
 *	对单一分组的消息进行杂凑计算。
 *	hHash				杂凑对象句柄
 *	pbData				消息数据
 *	ulDataLen			消息数据的长度
 *	pbHashData			杂凑数据
 *	pulHashLen			输入为杂凑数据缓冲区的长度，输出为实际杂凑数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDigest(
	IN HANDLE			hHash,
	IN BYTE 			*pbData,
	IN ULONG			ulDataLen,
	OUT BYTE			*pbHashData,
	IN OUT ULONG		*pulHashLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	对多个分组的消息进行杂凑计算。
 *	hHash				杂凑对象句柄
 *	pbData				消息数据
 *	ulDataLen			消息数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDigestUpdate(
	IN HANDLE			hHash,
	IN BYTE 			*pbData,
	IN ULONG			ulDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	结束多个分组消息的杂凑计算操作，将杂凑保存到指定的缓冲区。
 *	hHash				杂凑对象句柄
 *	pHashData			杂凑数据
 *	pulHashLen			输入为杂凑数据缓冲区的长度，输出为实际杂凑数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardDigestFinal(
	IN HANDLE			hHash,
	OUT BYTE			*pHashData,
	IN OUT ULONG		*pulHashLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	初始化消息认证码计算操作，设置计算消息认证码的密钥参数，并返回消息认证码句柄。
 *	hKey				计算消息认证码的密钥句柄
 *	MacParam			消息认证计算相关参数，包括初始向量、初始向量长度、填充方法等
 *	phMac				消息认证码对象句柄
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardMacInit(
	IN HANDLE			hKey,
	IN BLOCKCIPHERPARAM *MacParam,
	OUT HANDLE			*phMac,
	IN HANDLE			hDev = NULL
	);

/*
 *	计算单一分组数据的消息认证码。
 *	hMac				消息认证码句柄
 *	pbData				待计算数据
 *	ulDataLen			待计算数据的长度
 *	pbMacData			Mac数据
 *	pulMacLen			输入为Mac数据缓冲区的长度，输出为实际Mac数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardMac(
	IN HANDLE			hMac,
	IN BYTE 			*pbData,
	IN ULONG			ulDataLen,
	OUT BYTE			*pbMacData,
	IN OUT ULONG		*pulMacLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	计算多个分组数据的消息认证码。
 *	hMac				消息认证码句柄
 *	pbData				待计算数据
 *	plDataLen			待计算数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardMacUpdate(
	IN HANDLE			hMac,
	IN BYTE 			*pbData,
	IN ULONG			ulDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	结束多个分组数据的消息认证码计算操作。
 *	hMac				消息认证码句柄
 *	pbMacData			Mac数据
 *	pulMacLen			输入为Mac数据缓冲区的长度，输出为实际Mac数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardMacFinal(
	IN HANDLE			hMac,
	OUT BYTE			*pbMacData,
	IN OUT ULONG		*pulMacDataLen,
	IN HANDLE			hDev = NULL
	);

/************************************************************************/
/*	8. ECC证书操作														*/
/*	FJCA_SCardECCGetCertNo												*/
/*	FJCA_SCardECCSetCertNo												*/
/*	FJCA_SCardECCReadCert												*/
/*	FJCA_SCardECCWriteCert												*/
/*	FJCA_SCardECCGetContainerName										*/
/*	FJCA_SCardECCSetupIECert											*/
/************************************************************************/

/*
 *	返回卡内存放的证书状态:
		0				卡内无证书
		1				卡内存在第一张证书 
		2				卡内存在第二张证书	 
		3				卡内存在二张证书
 *	pbCertNo			证书状态
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardECCGetCertNo(
	OUT BYTE			*pbCertNo,
	IN HANDLE			hDev = NULL
	);

/*
 *	选择当前操作的证书，隐含为第一张证书。
 *	byCertNo			证书号（1-2）
						当Key内存放双证书时，第一张证书为签名证书，第二张证书为加密证书。
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardECCSetCertNo(
	IN BYTE 			byCertNo,
	IN HANDLE			hDev = NULL
	);

/*
 *	从智能卡或Key中读取当前证书（DER编码）。
 *	pbCert				证书数据
 *	pdwCertLen			输入为证书数据缓冲区的长度，输出为实际证书数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCReadCert(
	OUT BYTE			*pbCert,
	IN OUT DWORD		*pdwCertLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	把DER编码的证书写入智能卡或Key中。
 *	pbCert				证书数据
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCWriteCert(
	IN BYTE 			*pbCert,
	IN HANDLE			hDev = NULL
	);

/*
 *	取当前证书所在容器的名称。
 *	szContainerName 	证书容器名称
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCGetContainerName(
	OUT char			*szContainerName,
	IN HANDLE			hDev = NULL
	);

/*
 *	当有Key插入时，弹出口令框提示用户输入口令。若口令校验成功，Key内证书自动注册到IE。
 *	当有Key拔出时，Key内证书从IE中删除。
 *	nDevType			设备类型
 *	pnKeyStatus 		Key的状态
 *	pnKeyIndex			Key的序号，从0开始
 *	pbOperation 		是否有证书操作
 *	pszPin				用户输入的口令
 *	备注：
 *		调用者在WM_DEVICECHANGE消息处理函数中调用FJCA_SCardECCSetupIECert。
 *		如果插拔的不是本厂家Key，*pnKeyStatus = 0
 *		如果有本厂家Key插入，*pnKeyStatus = 1，*pnKeyIndex存放Key的序号。
 *			如果用户证书被导入IE，*pbOperation = TRUE, pszPin存放用户输入的口令。
 *		如果有本厂家Key拔出，*pnKeyStatus = 2，*pnKeyIndex存放Key的序号。
 *			如果用户证书从IE中删除，*pbOperation = TRUE。
 */
BOOL FJCA_SCardECCSetupIECert(
	IN int				nDevType,
	OUT int 			*pnKeyStatus,
	OUT int 			*pnKeyIndex,
	OUT BOOL			*pbOperation,
	OUT char			*pszPin
	);

/************************************************************************/
/*	9. ECC运算															*/
/*	FJCA_SCardECCGenKey 												*/
/*	FJCA_SCardECCGetPubKey												*/
/*	FJCA_SCardECCImportKey												*/
/*	FJCA_SCardECCSign													*/
/*	FJCA_SCardExtECCVerify												*/
/*	FJCA_SCardECCSignDigest 											*/
/*	FJCA_SCardExtECCVerifyDigest										*/
/*	FJCA_SCardExtECCEncrypt 											*/
/*	FJCA_SCardECCDecrypt												*/
/*	FJCA_SCardECCGetTempPubKey											*/
/*	FJCA_SCardECCKeyExchange											*/
/************************************************************************/
//	备注：
//	x||y：x与y的拼接

/*
 *	产生ECC密钥对，并输出公钥。
 *	pbECCPubKey 		公钥数据，格式：x||y
 *	pdwECCPubKeyLen 	输入为公钥数据缓冲区的长度，输出为实际公钥数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCGenKey(
	OUT BYTE			*pbECCPubKey,
	IN OUT DWORD		*pdwECCPubKeyLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	读ECC公钥数据。
 *	pbECCPubKey 		公钥数据，格式：x||y
 *	pdwECCPubKeyLen 	输入为公钥数据缓冲区的长度，输出为实际公钥数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCGetPubKey(
	OUT BYTE			*pbECCPubKey,
	IN OUT DWORD		*pdwECCPubKeyLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	导入ECC密钥。
 *	pbEncryptKey		使用对称密钥保护的ECC加密公私钥对
 *	dwEncryptKeyLen 	使用对称密钥保护的ECC加密公私钥对的长度
 *	pbSymmKey			使用签名公钥保护的对称密钥
 *	dwSymmKeyLen		使用签名公钥保护的对称密钥的长度
 *	dwSymmAlgID 		对称算法
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardECCImportKey(
	IN BYTE 			*pbEncryptKey,
	IN DWORD			dwEncryptKeyLen,
	IN BYTE 			*pbSymmKey,
	IN DWORD			dwSymmKeyLen,
	IN DWORD			dwSymmAlgID,
	IN HANDLE			hDev = NULL
	);

/*
 *	用ECC密钥对数据签名。
 *	pbData				待签名数据，必须为32字节
 *	dwDataLen			待签名数据的长度
 *	pbSignedData		签名数据，格式：r||s
 *	pdwSignedDataLen	输入为签名数据缓冲区的长度，输出为实际签名数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCSign(
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	OUT BYTE			*pbSignedData,
	IN OUT DWORD		*pdwSignedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	使用外部传入的ECC公钥做签名验证。
 *	pbSignedData		签名数据，格式：r||s
 *	dwSignedDataLen 	签名数据的长度
 *	pbECCPubKey 		公钥数据，格式：x||y
 *	dwECCPubKeyLen		公钥数据的长度
 *	pbData				待签名数据，必须为32字节
 *	dwDataLen			待签名数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardExtECCVerify(
	IN BYTE 			*pbSignedData,
	IN DWORD			dwSignedDataLen,
	IN BYTE 			*pbECCPubKey,
	IN DWORD			dwECCPubKeyLen,
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	用ECC密钥对数据签名。
 *	pbData				待签名数据
 *	dwDataLen			待签名数据的长度
 *	byHashAlgID 		摘要算法
						当byHashAlgID为SM3且dwUserIDLen不为0的情况下pbUserID有效
						此时执行SM2算法签名预处理1、预处理2操作
 *	pbUserID			用户ID
 *	dwUserIDLen 		用户ID的长度
 *	pbSignedData		签名数据，格式：r||s
 *	pdwSignedDataLen	输入为签名数据缓冲区的长度，输出为实际签名数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCSignDigest(
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	IN BYTE 			byHashAlgID,
	IN BYTE 			*pbUserID,
	IN DWORD			dwUserIDLen,
	OUT BYTE			*pbSignedData,
	IN OUT DWORD		*pdwSignedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	使用外部传入的ECC公钥做签名验证。
 *	pbSignedData		签名数据，格式：r||s
 *	dwSignedDataLen 	签名数据的长度
 *	byHashAlgID 		摘要算法
						当byHashAlgID为SM3且dwUserIDLen不为0的情况下pbUserID有效
						此时执行SM2算法签名预处理1、预处理2操作
 *	pbECCPubKey 		公钥数据，格式：x||y
 *	dwECCPubKeyLen		公钥数据的长度
 *	pbUserID			用户ID
 *	dwUserIDLen 		用户ID的长度
 *	pbData				待签名数据
 *	dwDataLen			待签名数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardExtECCVerifyDigest(
	IN BYTE 			*pbSignedData,
	IN DWORD			dwSignedDataLen,
	IN BYTE 			byHashAlgID,
	IN BYTE 			*pbECCPubKey,
	IN DWORD			dwECCPubKeyLen,
	IN BYTE 			*pbUserID,
	IN DWORD			dwUserIDLen,
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	使用外部传入的ECC公钥做加密运算。
 *	pbData				待加密数据
 *	dwDataLen			待加密数据的长度
 *	pbECCPubKey 		公钥数据，格式：x||y
 *	dwECCPubKeyLen		公钥数据的长度
 *	pbEncryptedData 	加密数据，格式：C1长度(DWORD)||C1||C2长度(DWORD)||C2||C3长度(DWORD)||C3
 *	pdwEncryptedDataLen 输入为加密数据缓冲区的长度，输出为实际加密数据的长度
 *	hDev				连接设备时返回的设备句柄
 */
BOOL FJCA_SCardExtECCEncrypt(
	IN BYTE 			*pbData,
	IN DWORD			dwDataLen,
	IN BYTE 			*pbECCPubKey,
	IN DWORD			dwECCPubKeyLen,
	OUT BYTE			*pbEncryptedData,
	IN OUT DWORD		*pdwEncryptedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	ECC数据解密。
 *	pbEncryptedData 	已加密数据，格式：C1长度(DWORD)||C1||C2长度(DWORD)||C2||C3长度(DWORD)||C3
 *	dwEncryptedDataLen	已加密数据的长度
 *	pbDecryptedData 	解密数据
 *	pdwDecryptedDataLen 输入为解密数据缓冲区的长度，输出为实际解密数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCDecrypt(
	IN BYTE 			*pbEncryptedData,
	IN DWORD			dwEncryptedDataLen,
	OUT BYTE			*pbDecryptedData,
	IN OUT DWORD		*pdwDecryptedDataLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	使用ECC密钥协商算法，为计算会话密钥而产生协商参数，返回临时ECC密钥对的公钥。
 *	pbTempECCPubKey 	临时公钥数据，格式：x||y
 *	pdwTempECCPubKeyLen 输入为公钥数据缓冲区的长度，输出为实际公钥数据的长度
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCGetTempPubKey(
	OUT BYTE			*pbTempECCPubKey,
	IN OUT DWORD		*pdwTempECCPubKeyLen,
	IN HANDLE			hDev = NULL
	);

/*
 *	使用ECC密钥协商算法，产生协商参数并计算会话密钥，输出临时ECC密钥对公钥，并返回产生的密钥句柄
 *	bRole				角色　1发起方，0接收方
 *	pbID　				已方ID
 *	dwIDLen　			已方ID长度
 *	pbTempECCPubKey 	已方临时公钥数据，格式：x||y
 *	pdwTempECCPubKeyLen 输入为公钥数据缓冲区的长度，输出为实际公钥数据的长度
 *	pbOID　 			对方ID
 *	dwOIDLen			对方ID长度
 *	pbOECCPubKey		对方公钥数据，格式：x||y
 *	dwOECCPubKeyLen 	公钥数据的长度
 *	pbOTempECCPubKey	对方临时公钥数据，格式：x||y
 *	dwOTempECCPubKeyLen 公钥数据的长度
 *	ulAlgId 			会话密钥算法标识
 *	phKey				会话密钥句柄
 *	hDev				连接设备时返回的设备句柄
 *	备注：				请先调用FJCA_SCardECCSetCertNo选择当前操作的证书。
 */
BOOL FJCA_SCardECCKeyExchange( 
	IN BYTE 			bRole,
	IN BYTE 			*pbID,
	IN DWORD			dwIDLen,
	OUT BYTE			*pbTempECCPubKey,
	IN OUT DWORD		*pdwTempECCPubKeyLen,
	IN BYTE 			*pbOID,
	IN DWORD			dwOIDLen,
	IN BYTE 			*pbOECCPubKey,
	IN DWORD			dwOECCPubKeyLen,
	IN BYTE 			*pbOTempECCPubKey,
	IN DWORD			dwOTempECCPubKeyLen,
	IN ULONG			ulAlgId,
	OUT HANDLE			*phKey,
	IN HANDLE			hDev = NULL
	);

#ifdef __cplusplus
}
#endif

#endif


