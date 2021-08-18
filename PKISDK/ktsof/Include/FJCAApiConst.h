#ifndef _FJCAAPICONST_H
#define _FJCAAPICONST_H

// 接口返回值代码列表
#define SAR_OK							0			// 成功
#define SAR_UnknownErr					0X0B000001	// 异常错误
#define SAR_NotSupportYetErr			0X0B000002	// 不支持的服务
#define SAR_FileErr						0X0B000003	// 文件操作错误
#define SAR_ProviderTypeErr				0X0B000004	// 服务提供者参数类型错误
#define SAR_LoadProviderErr				0X0B000005	// 导入服务提供者接口错误
#define SAR_LoadDevMngApiErr			0X0B000006	// 导入设备管理接口错误
#define SAR_AlgoTypeErr					0X0B000007	// 算法类型错误
#define SAR_NameLenErr					0X0B000008	// 名称长度错误
#define SAR_KeyUsageErr					0X0B000009	// 密钥用途错误
#define SAR_ModulusLenErr				0X0B000010	// 模的长度错误
#define SAR_NotInitializeErr			0X0B000011	// 未初始化
#define SAR_ObjErr						0X0B000012	// 对象错误
#define SAR_MemoryErr					0X0B000100	// 内存错误
#define SAR_TimeoutErr					0X0B000101	// 服务超时
#define SAR_IndataLenErr				0X0B000200	// 输入数据长度错误
#define SAR_IndataErr					0X0B000201	// 输入数据错误
#define SAR_GenRandErr					0X0B000300	// 生成随机数错误
#define SAR_HashObjErr					0X0B000301	// HASH对象错
#define SAR_HashErr						0X0B000302	// HASH运算错误
#define SAR_GenRsaKeyErr				0X0B000303	// 产生RSA密钥错
#define SAR_RsaModulusLenErr			0X0B000304	// RSA密钥模长错误
#define SAR_CspImprtPubKeyErr			0X0B000305	// CSP服务导入公钥错误
#define SAR_RsaEncErr					0X0B000306	// RSA加密错误
#define SAR_RSGDecErr					0X0B000307	// RSA解密错误
#define SAR_HashNotEqualErr				0X0B000308	// HASH值不相等
#define SAR_KeyNotFountErr				0X0B000309	// 密钥未发现
#define SAR_CertNotFountErr				0X0B000310	// 证书未发现
#define SAR_NotExportErr				0X0B000311	// 对象未导出
#define SAR_DecryptPadErr				0X0B000400	// 解密时做补丁错误
#define SAR_MacLenErr					0X0B000401	// MAC长度错误
#define SAR_KeyInfoTypeErr				0X0B000402	// 密钥类型错误
#define SAR_NULLPointerErr				0X0B000403	// 某一个参数为空指针
#define SAR_APPNOTFOUNDErr				0X0B000404	// 没有找到该应用
#define SAR_CERTENCODEErr				0X0B000405	// 证书编码格式错误
#define SAR_CERTINVALIDErr				0X0B000406	// 证书无效，不是可信ca颁发的证书
#define SAR_CERTHASEXPIREDErr			0X0B000407	// 证书已过期
#define SAR_CERTREVOKEDErr				0X0B000408	// 证书已经被吊销
#define SAR_SIGNDATAErr					0X0B000409	// 签名失败
#define SAR_VERIFYSIGNDATAErr			0X0B000410	// 验证签名失败
#define SAR_READFILEErr					0X0B000411	// 读文件异常，可能文件不存在或没有读取权限等
#define SAR_WRITEFILEErr				0X0B000412	// 写文件异常，可能文件不存在或没有写权限等
#define SAR_SECRETSEGMENTErr			0X0B000413	// 门限算法密钥分割失败
#define SAR_SECERTRECOVERYErr			0X0B000414	// 门限恢复失败
#define SAR_ENCRYPTDATAErr				0X0B000415	// 对数据的对称加密失败
#define SAR_DECRYPTDATAErr				0X0B000416	// 对称算法的数据解密失败
#define SAR_PKCS7ENCODEErr				0X0B000417	// PKCS#7编码格式错误
#define SAR_XMLENCODEErr				0X0B000418	// 不是合法的xml编码数据
#define SAR_PARAMETERNOTSUPPORTErr		0X0B000419	// 不支持的参数
#define SAR_LoginErr					0X0B000420	// 登录失败
#define SAR_AddP7BErr					0X0B000421	// 添加信任列表失败
#define SAR_CertRevokedErr				0X0B000422	// 证书已注销
// 添加
#define SAR_BASE64ENCODEDECODEErr		0X0B000500	// Base64编解码失败
#define SAR_PINErr						0X0B000501	// 核对证书口令失败
#define SAR_ObjNotFountErr				0X0B000502	// 对象未发现
#define SAR_COMMUNICATIONErr			0X0B000503	// 通讯数据传输错误
#define SAR_DATABASEErr					0X0B000504	// 访问数据库时，发生系统错误
#define SAR_CERTNOTYETVALIDErr			0X0B000505	// 证书未生效
#define SAR_CERTNOTINRLErr				0X0B000506	// 企业不在红名单中，企业还未交网上服务费
#define SAR_CERTNOTINDZBErr				0X0B000507	// 企业不在对照表中，数据整理还未完成
#define SAR_SERVICEHASEXPIREDErr		0X0B000508	// 服务已过期
#define SAR_TSSERVERErr					0X0B000509	// 时间戳服务器内部错误
#define SAR_CREATETSREQUESTErr			0X0B000510	// 创建时间戳请求失败
#define SAR_SIGNTSErr					0X0B000511	// 签发时间戳失败
#define SAR_TSENCODEErr					0X0B000512	// 时间戳编码格式错误
#define SAR_VERIFYTSErr					0X0B000513	// 验证时间戳失败
#define SAR_ObjExistErr					0X0B000514	// 对象已存在
#define SAR_CRLENCODEErr				0X0B000515	// CRL编码格式错误
#define SAR_CRLINVALIDErr				0X0B000516	// 不是可信ca颁发的CRL
#define SAR_ECCCERTENCODEErr			0X0B000517	// ECC证书编码格式错误
#define SAR_ECCOPERErr					0X0B000518	// ECC运算错误
#define SAR_ECCSIGNErr					0X0B000519	// ECC签名错误
#define SAR_ECCVERIFYSIGNErr			0X0B000520	// ECC验证签名错误
#define SAR_ECCENCErr					0X0B000521	// ECC加密错误
#define SAR_ECCDECErr					0X0B000522	// ECC解密错误
#define SAR_ECCDATAErr					0X0B000523	// ECC数据编码错误
#define SAR_DATAENCODEErr				0X0B000524	// 数据编码转换失败
#define SAR_NotLogin					0X0B000800	// 没有登录
#define SAR_VERIFYCERTErr				0X0B000801	// 验证证书失败
#define SAR_GETCERTTRUSTLEVELErr		0X0B000802	// 获取证书所处信任域级别失败

// 常量定义
#define SGD_TRUE						0x00000001
#define SGD_FALSE						0x00000000

// 全局参数定义
// 对称算法
#define SGD_STR_SM1_ECB					"SM1_ECB"	// SM1算法ECB加密模式
#define SGD_STR_SM1_CBC					"SM1_CBC"	// SM1算法CBC加密模式
#define SGD_STR_SM1_CFB					"SM1_CFB"	// SM1算法CFB加密模式
#define SGD_STR_SM1_OFB					"SM1_OFB"	// SM1算法OFB加密模式
#define SGD_STR_SM1_MAC					"SM1_MAC"	// SM1算法MAC加密模式
#define SGD_STR_SSF33_ECB				"SSF33_ECB"	// SSF33算法ECB加密模式
#define SGD_STR_SSF33_CBC				"SSF33_CBC"	// SSF33算法CBC加密模式
#define SGD_STR_SSF33_CFB				"SSF33_CFB"	// SSF33算法CFB加密模式
#define SGD_STR_SSF33_OFB				"SSF33_OFB"	// SSF33算法OFB加密模式
#define SGD_STR_SSF33_MAC				"SSF33_MAC"	// SSF33算法MAC加密模式
#define SGD_STR_SM4_ECB					"SM4_ECB"	// SM4算法ECB加密模式
#define SGD_STR_SM4_CBC					"SM4_CBC"	// SM4算法CBC加密模式
#define SGD_STR_SM4_CFB					"SM4_CFB"	// SM4算法CFB加密模式
#define SGD_STR_SM4_OFB					"SM4_OFB"	// SM4算法OFB加密模式
#define SGD_STR_SM4_MAC					"SM4_MAC"	// SM4算法MAC加密模式
#define SGD_STR_ZUC_EEA3				"ZUC_EEA3"	// ZUC祖冲之机密性算法128-EEA3算法
#define SGD_STR_ZUC_EIA3				"ZUC_EIA3"	// ZUC祖冲之完整性算法128-EIA3算法
// 非对称算法
#define SGD_STR_RSA						"RSA"		// RSA算法
#define SGD_STR_SM2						"SM2"		// SM2椭圆曲线密码算法
#define SGD_STR_SM2_1					"SM2_1"		// SM2椭圆曲线签名算法
#define SGD_STR_SM2_2					"SM2_2"		// SM2椭圆曲线密钥交换协议
#define SGD_STR_SM2_3					"SM2_3"		// SM2椭圆曲线加密算法
// 杂凑算法
#define	SGD_STR_SM3						"SM3"		// SM3杂凑算法
#define SGD_STR_SHA1					"SHA1"		// SHA_1杂凑算法
#define SGD_STR_SHA256					"SHA256"	// SHA_256杂凑算法
// 签名算法
#define SGD_STR_SM3_RSA					"SM3_RSA"	// 基于SM3算法和RSA算法的签名
#define SGD_STR_SHA1_RSA				"SHA1_RSA"	// 基于SHA_1算法和RSA算法的签名
#define SGD_STR_SHA256_RSA				"SHA256_RSA"// 基于SHA_256算法和RSA算法的签名
#define SGD_STR_SM3_SM2					"SM3_SM2"	// 基于SM3算法和SM2算法的签名

// 对称算法标识
#define SGD_SM1_ECB						0x00000101	// SM1算法ECB加密模式
#define SGD_SM1_CBC						0x00000102	// SM1算法CBC加密模式
#define SGD_SM1_CFB						0x00000104	// SM1算法CFB加密模式
#define SGD_SM1_OFB						0x00000108	// SM1算法OFB加密模式
#define SGD_SM1_MAC						0x00000110	// SM1算法MAC加密模式
#define SGD_SSF33_ECB					0x00000201	// SSF33算法ECB加密模式
#define SGD_SSF33_CBC					0x00000202	// SSF33算法CBC加密模式
#define SGD_SSF33_CFB					0x00000204	// SSF33算法CFB加密模式
#define SGD_SSF33_OFB					0x00000208	// SSF33算法OFB加密模式
#define SGD_SSF33_MAC					0x00000210	// SSF33算法MAC加密模式
#define SGD_SM4_ECB						0x00000401	// SM4算法ECB加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_CBC						0x00000402	// SM4算法CBC加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_CFB						0x00000404	// SM4算法CFB加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_OFB						0x00000408	// SM4算法OFB加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM4_MAC						0x00000410	// SM4算法MAC加密模式				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_ZUC_EEA3					0x00000801	// ZUC祖冲之机密性算法128-EEA3算法	// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_ZUC_EIA3					0x00000802	// ZUC祖冲之完整性算法128-EIA3算法	// 《密码应用标识规范》（GM/T 0006-2012）新加
// 非对称算法标识
#define SGD_RSA							0x00010000	// RSA算法
#define SGD_SM2							0x00020100	// SM2椭圆曲线密码算法				// 《密码应用标识规范》（GM/T 0006-2012）新加
#define SGD_SM2_1						0x00020100	// SM2椭圆曲线签名算法				// 与《密码应用标识规范》（GM/T 0006-2012）不一致（国标为0x00020200）
#define SGD_SM2_2						0x00020200	// SM2椭圆曲线密钥交换协议			// 与《密码应用标识规范》（GM/T 0006-2012）不一致（国标为0x00020400）
#define SGD_SM2_3						0x00020400	// SM2椭圆曲线加密算法				// 与《密码应用标识规范》（GM/T 0006-2012）不一致（国标为0x00020800）
// 杂凑算法标识
#define	SGD_SM3							0x00000001	// SM3杂凑算法
#define SGD_SHA1						0x00000002	// SHA_1杂凑算法
#define SGD_SHA256						0x00000004	// SHA_256杂凑算法
// 签名算法标识
#define SGD_SM3_RSA						0x00001001	// 基于SM3算法和RSA算法的签名
#define SGD_SHA1_RSA					0x00001002	// 基于SHA_1算法和RSA算法的签名
#define SGD_SHA256_RSA					0x00001004	// 基于SHA_256算法和RSA算法的签名
#define SGD_SM3_SM2						0x00020101	// 基于SM3算法和SM2算法的签名		// 与《密码应用标识规范》（GM/T 0006-2012）不一致（国标为0x00020201）

// 接口描述标识
#define SGD_PROVIDER_CSP				0x00000001	// CSP接口
#define SGD_PROVIDER_PKCS11				0x00000001	// PKCS#11接口
#define SGD_PROVIDER_SDS				0x00000001	// 国标密码设备应用接口
#define SGD_KEYUSAGE_SIGN				0x00000001	// 签名/验证的密钥用途
#define SGD_KEYUSAGE_KEYEXCHANGE		0x00000001	// 加/解密的密钥用途
#define SGD_MODE_ECB					0x00000001	// ECB模式
#define SGD_MODE_CBC					0x00000001	// CBC模式
#define SGD_MODE_CFB					0x00000001	// CFB模式
#define SGD_MODE_OFB					0x00000001	// OFB模式
#define SGD_KEYINFO_DEV_GENERATE		0x00000001	// 设备产生
#define SGD_KEYINFO_KEY					0x00000001	// 外部输入KEY
#define SGD_KEYINFO_IV					0x00000001	// 外部输入IV
#define SGD_KEYINFO_PASSWORD_DERIVE_KEY	0x00000001	// 通过口令生成KEY

// 证书解析标识
// 与《密码应用标识规范》（GM/T 0006-2012）不一致（国标为0x00020201）
#define SGD_GET_CERT_VERSION			0x00000001	// 证书版本
#define SGD_GET_CERT_SERIAL				0x00000002	// 证书序列号
#define SGD_GET_CERT_SIGNALG			0x00000003	// 证书签名算法标识
#define SGD_GET_CERT_ISSUER_C			0x00000004	// 证书颁发者国家(C)
#define SGD_GET_CERT_ISSUER_O			0x00000005	// 证书颁发者组织名(O)
#define SGD_GET_CERT_ISSUER_OU			0x00000006	// 证书颁发者部门名(OU)
#define SGD_GET_CERT_ISSUER_S			0x00000007	// 证书颁发者所在的省、自治区、直辖市(S)
#define SGD_GET_CERT_ISSUER_CN			0x00000008	// 证书颁发者通用名称(CN)
#define SGD_GET_CERT_ISSUER_L			0x00000009	// 证书颁发者所在的城市、地区(L)
#define SGD_GET_CERT_ISSUER_E			0x00000010	// 证书颁发者Email
#define SGD_GET_CERT_NOTBEFORE			0x00000011	// 证书有效期：起始日期
#define SGD_GET_CERT_AFTER				0x00000012	// 证书有效期：终止日期
#define SGD_GET_CERT_SUBJECT_C			0x00000013	// 证书拥有者国家(C)
#define SGD_GET_CERT_SUBJECT_O			0x00000014	// 证书拥有者组织名(O)
#define SGD_GET_CERT_SUBJECT_OU			0x00000015	// 证书拥有者部门名(OU)
#define SGD_GET_CERT_SUBJECT_S			0x00000016	// 证书拥有者所在的省、自治区、直辖市(S)
#define SGD_GET_CERT_SUBJECT_CN			0x00000017	// 证书拥有者通用名称(CN)
#define SGD_GET_CERT_SUBJECT_L			0x00000018	// 证书拥有者所在的城市、地区(L)
#define SGD_GET_CERT_SUBJECT_E			0x00000019	// 证书拥有者Email
#define SGD_GET_CERT_ISSUER_DN			0x00000020	// 证书颁发者DN
#define SGD_GET_CERT_SUBJECT_DN			0x00000021	// 证书拥有者DN
#define SGD_GET_CERT_DER_PUBKEY			0x00000022	// 证书公钥信息
#define SGD_GET_CERT_DER_EXTENSIONS		0x00000023	// 证书扩展项信息
#define SGD_EXT_AUTHORITYKEYIDENTIFIER	0x00000024	// 颁发者密钥标识符
#define SGD_EXT_SUBJECTKEYIDENTIFIER	0x00000025	// 证书持有者密钥标识符
#define SGD_EXT_KEYUSAGE				0x00000026	// 密钥用途
#define SGD_EXT_PRIVATEKEYUSAGEPERIOD	0x00000027	// 私钥有效期
#define SGD_EXT_CERTIFICATEPOLICIES		0x00000028	// 证书策略
#define SGD_EXT_POLICYMAPPINGS			0x00000029	// 策略映射
#define SGD_EXT_BASICCONSTRAINTS		0x00000030	// 基本限制
#define SGD_EXT_POLICYCONSTRAINTS		0x00000031	// 策略限制
#define SGD_EXT_EXTKEYUSAGE				0x00000032	// 扩展密钥用途
#define SGD_EXT_CRLDISTRIBUTIONPOINTS	0x00000033	// CRL发布点
#define SGD_EXT_NETSCAPE_CERT_TYPE		0x00000034	// netscape属性
#define SGD_EXT_CERT_UNIQUEID			0x00000035	// 证书实体唯一标识
#define SGD_EXT_IDENTIFYCARDNUMBER		0x00000036	// 个人身份证号码
#define SGD_EXT_INURANCENUMBER			0x00000037	// 个人社会保险号
#define SGD_EXT_ICREGISTRATIONNUMBER	0x00000038	// 企业工商注册号
#define SGD_EXT_ORGANIZATIONCODE		0x00000039	// 企业组织机构代码
#define SGD_EXT_TAXATIONNUMBER			0x00000040	// 企业税务登记证号
#define SGD_MAX_NAME_SIZE				0x00000080	// 名称最大长度
#define SGD_MAX_COUNT					0x00000100	// 列表最大长度
#define SGD_MAX_CONTAINER				0x00000800	// 容器容量

// 证书扩展项信息
#define SGD_EXT_OBJ_AUTHORITYKEYIDENTIFIER	"2.5.29.35"	// 颁发者密钥标识符
#define SGD_EXT_OBJ_SUBJECTKEYIDENTIFIER	"2.5.29.14"	// 证书持有者密钥标识符
#define SGD_EXT_OBJ_KEYUSAGE				"2.5.29.15"	// 密钥用途
#define SGD_EXT_OBJ_PRIVATEKEYUSAGEPERIOD	"2.5.29.16"	// 私钥有效期
#define SGD_EXT_OBJ_CERTIFICATEPOLICIES		"2.5.29.32"	// 证书策略
#define SGD_EXT_OBJ_POLICYMAPPINGS			"2.5.29.33"	// 策略映射
#define SGD_EXT_OBJ_BASICCONSTRAINTS		"2.5.29.19"	// 基本限制
#define SGD_EXT_OBJ_POLICYCONSTRAINTS		"2.5.29.36"	// 策略限制
#define SGD_EXT_OBJ_EXTKEYUSAGE				"2.5.29.37"	// 扩展密钥用途
#define SGD_EXT_OBJ_CRLDISTRIBUTIONPOINTS	"2.5.29.31"	// CRL发布点
#define SGD_EXT_OBJ_NETSCAPE_CERT_TYPE		"2.16.840.1.113730.1.1"	// netscape属性
#define SGD_EXT_OBJ_AUTHORITYINFOACCESS		"1.3.6.1.5.5.7.1.1"		// 颁发机构信息访问

#define SGD_EXT_OBJ_FJCA_CERTCODE			"1.2.156.112578.1"		// 福建CA证书编码
#define SGD_EXT_OBJ_CERT_UNIQUEID			"1.2.86.21.1.1"			// 卫生系统证书实体唯一标识

// 电子政务数字证书格式规范（2010.8）和数字证书格式国标（2006）
#define SGD_EXT_OBJ_IDENTIFYCARDNUMBER		"1.2.156.10260.4.1.1"	// 个人身份标识码
#define SGD_EXT_OBJ_INURANCENUMBER			"1.2.156.10260.4.1.2"	// 个人社会保险号
#define SGD_EXT_OBJ_ICREGISTRATIONNUMBER	"1.2.156.10260.4.1.3"	// 企业工商注册号
#define SGD_EXT_OBJ_ORGANIZATIONCODE		"1.2.156.10260.4.1.4"	// 企业组织机构代码
#define SGD_EXT_OBJ_TAXATIONNUMBER			"1.2.156.10260.4.1.5"	// 企业税号
// 电子政务数字证书格式规范（2010.4）
#define SGD_EXT_OBJ_IDENTIFYCARDNUMBER_0	"1.2.156.11.7.1"		// 个人身份证号码
#define SGD_EXT_OBJ_INURANCENUMBER_0		"1.2.156.11.7.2"		// 个人社会保险号
#define SGD_EXT_OBJ_ICREGISTRATIONNUMBER_0	"1.2.156.11.7.4"		// 企业工商注册号
#define SGD_EXT_OBJ_ORGANIZATIONCODE_0		"1.2.156.11.7.3"		// 企业组织机构代码
#define SGD_EXT_OBJ_TAXATIONNUMBER_0		"1.2.156.11.7.5"		// 企业税务登记证号

// 证书对应信息
#define CERT_SUBJECT					1			// 用户名
#define CERT_UNIQUEID					2			// 证书实体唯一标识
#define CERT_DEPT						3			// 部门
#define CERT_ISSUE						4			// 颁发者DN
#define CERT_DEVICETYPE					8			// 证书介质类型
#define CERT_CATYPE						9			// CA类型
#define CERT_KEYTYPE					10			// 用户证书密钥类型(双证或单证)
#define CERT_DEVICENAME					13			// 用户证书介质名称
#define CERT_DEVICEPROVIDER				14			// 用户证书介质提供者即csp名称
#define CERT_DEVICEAFFIX				15			// 用户证书介质附加库
#define CERT_SIGNPATH					16			// 用户签名证书路径
#define CERT_EXCHPATH					17			// 用户加密证书路径
#define CERT_SIGNPFXPATH				18			// 用户签名P12证书路径
#define CERT_EXCHPFXPATH				19			// 用户加密P12证书路径
#define CERT_UNIQUEIDOID				22			// 用户证书UniqueID的OID

// 自定义常量
#define SGD_CURRENT_VERSION				0x00010001	// 版本号的格式为：0xAAAABBBB，其中AAAA为主版本号，BBBB为次版本号

#define SGD_SIZE_DATABLOCK				4096		// 每次操作的数据块大小，4k
#define SGD_SIZE_CERT					4096		// 证书长度，4k
#define SGD_SIZE_RSADATA				1024		// RSA数据长度，1024
#define SGD_SIZE_ASYMDATA				1024		// 非对称数据长度，1024
#define SGD_SIZE_PINMIN					2			// 口令最小长度
#define SGD_SIZE_PINMAX					16			// 口令最大长度
#define SGD_SIZE_SM3HASH				32			// SM3杂凑长度
#define SGD_SIZE_ECC					32			// ECC长度
#define SGD_SIZE_ECC_64					64			// ECC最大长度

#define SGD_STR_SEPARATOR				"\r\n"				// DN中的分隔符
#define SGD_STR_SEPARATOR2				","					// DN中的分隔符2
#define SGD_STR_RETURN					"\r\n"				// 回车换行
#define SGD_STR_ECCUSERID				"1234567812345678"	// user id

#define SGD_KEYUSAGE_SIGNATURE			1			// 密钥用法：签名
#define SGD_KEYUSAGE_EXCHANGE			2			// 密钥用法：加密

#define SGD_CSPNAME_MH					"Antaeus & Mingwah Aohan eKey RSA Cryptographic Provider v1.0"
#define SGD_CSPNAME_WQ					"Watchdata CSP v3.4 For FJCA"
#define SGD_CSPNAME_ZC					"NJA EKEY CSP V1.0"
#define SGD_CSPNAME_HB					"HENGBAO USB KEY CSP1.0.0 For FJCA"
#define SGD_CSPNAME_FT					"EnterSafe ePass3003 CSP For FJCA v1.0"
#define SGD_CSPNAME_HT					"HaiTai Cryptographic Service Provider For FJCA"
#define SGD_CSPNAME_MH_L				L"Antaeus & Mingwah Aohan eKey RSA Cryptographic Provider v1.0"
#define SGD_CSPNAME_WQ_L				L"Watchdata CSP v3.4 For FJCA"
#define SGD_CSPNAME_ZC_L				L"NJA EKEY CSP V1.0"
#define SGD_CSPNAME_HB_L				L"HENGBAO USB KEY CSP1.0.0 For FJCA"
#define SGD_CSPNAME_FT_L				L"EnterSafe ePass3003 CSP For FJCA v1.0"
#define SGD_CSPNAME_HT_L				L"HaiTai Cryptographic Service Provider For FJCA"

#define SGD_KEYTYPE_MH					1
#define SGD_KEYTYPE_WQ					2
#define SGD_KEYTYPE_ZC					3
#define SGD_KEYTYPE_HB					4
#define SGD_KEYTYPE_FT					5
#define SGD_KEYTYPE_HT					6
#define SGD_KEYTYPE_MH_STR				"明华"
#define SGD_KEYTYPE_WQ_STR				"握奇"
#define SGD_KEYTYPE_ZC_STR				"中钞"
#define SGD_KEYTYPE_HB_STR				"恒宝"
#define SGD_KEYTYPE_FT_STR				"飞天"
#define SGD_KEYTYPE_HT_STR				"海泰"

// openssl key usage
#define X509v3_KU_DIGITAL_SIGNATURE	0x0080
#define X509v3_KU_NON_REPUDIATION	0x0040
#define X509v3_KU_KEY_ENCIPHERMENT	0x0020
#define X509v3_KU_DATA_ENCIPHERMENT	0x0010
#define X509v3_KU_KEY_AGREEMENT		0x0008
#define X509v3_KU_KEY_CERT_SIGN		0x0004
#define X509v3_KU_CRL_SIGN			0x0002
#define X509v3_KU_ENCIPHER_ONLY		0x0001
#define X509v3_KU_DECIPHER_ONLY		0x8000
#define X509v3_KU_UNDEF				0xffff

#endif
