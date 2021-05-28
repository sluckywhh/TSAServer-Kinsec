#ifndef _STF_TSA_FUNCTION_H_
#define _STF_TSA_FUNCTION_H_

#include <stdio.h>
#include <string>
#include <map>
#include "../CommonUtil/TSACommonUtil.h"
#include "STFDefine.h"
#include "KSSignLibFunc.h"
#include "MyToolLibFunc.h"
//#include "ktsgd.h"

/****************************************************************
 * 函数名定义
 ****************************************************************/
#define TSA_INIT_ENVIRONMENT 1001
#define TSA_CLEAR_ENVIRONMENT 1002
#define TSA_CREATE_TS_REQUEST 1003
#define TSA_CREATE_TS_RESPONSE 1004
#define TSA_VERIFY_TS_VALIDITY 1005
#define TSA_GET_TS_INFO 1006
#define TSA_GET_TS_DETAIL 1007
/****************************************************************/

/****************************************************************
 * 函数分发
 ****************************************************************/
SGD_UINT32 TSAFunctionAssign(
	int function_define, 
	map<string, string> mrequestdata, 
	map<string, string> &mresponsedata);
/****************************************************************/

/*
建立时间戳环境句柄
phTSHandle[OUT]:时间截环境句柄指针。
返回值： 0:成功; 其他:失败。
*/
SGD_UINT32 STF_InitEnvironment(void **phTSHandle = NULL);

/*
清除时间截环境句柄。
参数:
hTSHandle[IN]:时间截环境句柄。
返回值:
0:成功; 其他:失败。
*/
SGD_UINT32 STF_ClearEnvironment(void* hTSHandle = NULL);

/*
用指定算法对时间截请求信息indata进行密码杂凑运算,生成时间戳请求包。
参数:
hTSHandle[IN];时间戳环境句柄
pucInData[IN];需要加盖时间截的用户信息
uiInDataLength [IN]:用户信息的长度
uiReqType[IN]:请求的时间截服务类型
pucTSExt[IN]:时间戳请求包的其他扩展，DER编码格式
uiTSExtLength[IN]:时间戳请求包扩展的长度
uiHashAlgID[IN]:密码杂凑算法标识
pucTSRequest [OUT]:时间戳请求
puiTSRequestLength [IN/OUT]:时间戳请求的长度
返回值:
0:成功;其他;失败。
备注:
uiReqType: 0代表时间戳响应应该包含时间戳服务器的证书;1代表时间截响应不含时间戳服务器的证书。
puiTSRequestLength[IN/OUT]:入口值为指定的用于存放时间戳请求的字符数的最大长度,出口值为时间戳请求的实际长度。
*/
/*
HTTP Post 请求body：
in_data(Base64), in_data_length, hash_alg_id(SGD_SM3,SGD_SHA1,SGD_SHA256)
HTTP Post 响应body：
ts_request(Base64), ts_request_length
*/
SGD_UINT32 STF_CreateTSRequest( void* hTSHandle, 
	SGD_UINT8 *pucInData, 
	SGD_UINT32 uiInDataLength, 
	SGD_UINT32 uiReqType, 
	SGD_UINT8 *pucTSExt,
	SGD_UINT32 *uiTSExtLength,
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT8 *pucTSRequest, 
	SGD_UINT32 *puiTSRequestLength);

/*
根据时间戳请求包生成时间戳响应包。
参数:
hTSHandle[IN]:时间截环境句柄
pucTSRequest [IN]:时间戳请求
uiTSRequestLength [IN]:时间戳请求的长度
uiSignatureAlgID[IN];签名算法标识
pucTSResponse [OUT]:时间戳响应
puiTSResponseLength [IN/OUT]:时间戳响应的长度
返回值:
0 :成功;其他:失败。
备注:
puiTSResponseLength[IN/OUT]:入口值为指定的用于存放时间戳的字符数组的最大长度,出口值为时间戳的实际长度。
*/
/*
HTTP Post 请求body：
ts_request(Base64), ts_request_length, signature_alg_id
HTTP Post 响应body：
ts_response(Base64), ts_response_length
*/
SGD_UINT32 STF_CreateTSResponse(void* hTSHandle,
	SGD_UINT8 *pucTSRequest,
	SGD_UINT32 uiTSRequestLength,
	SGD_UINT32 uiSignatureAlgID,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 *puiTSResponseLength);

/*
验证时间戳响应是否有效。
参数:
hTSHandle[IN]:时间截环境句柄
pucTSResponse [IN]:获取的时间戳响应
uiTSResponseLength[IN]:时间戳响应的长度
uiHashAlgID[IN]:密码杂凑算法标识
uiSignatureAlgID[IN]:签名算法标识
pucTSCert[IN]:TSA的证书,DER编码格式
uiTSCertLength [IN]:TSA证书的长度
返回值:
0:成功;其他:失败。
备注:
该函数验证时间戳响应是否有效。对于不包含时间戳服务器证书的响应,需要指定时间戳服务器的证书才能进行验证;
对于包含时间戳服务器证书的响应,可以把入口的证书参数置为空,使用响应中自带的证书进行验证,否则将使用指定的证书进行验证,
即指定的证书优先于自带的证书。
*/
/*
HTTP Post 请求body：
ts_response(Base64), ts_response_length, hash_alg_id(SGD_SM3,SGD_SHA1,SGD_SHA256), signature_alg_id,
ts_cert(Base64), ts_cert_length
HTTP Post 响应body：
verify_result(0成功)
*/
SGD_UINT32 STF_VerifyTSValidity(void* hTSHandle,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT32 uiSignatureAlgID, 
	SGD_UINT8 *pucTSCert, 
	SGD_UINT32 uiTSCertLength);

/*
获取时间戳的主要信息。
参数:
hTSHandle[IN]:时间戳环境句柄
pucTSResponse[IN]:获取的时间戳响应
uiTSResponseLength [IN]:时间戳响应的长度
pucIssuerName [OUT]:TSA的通用名
puiIssuerNameLength[IN/OUT]:TSA通用名的长度
pucTime [OUT]:时间戳标注的时间值
puiTimeLength [IN/OUT]:时间戳标注的时间值长度
返回值:
0:成功;其他:失败。
备注:
该函数解析时间戳的主要信息,包括TSA的通用名和时间戳的签发时间。
puilssuerNameLength[IN/OUT]:入口值为指定的用于存放签发者名称的字符数组的最大长度,出口值为签发者名称的实际长度。
puiTimeLength[IN/OUT]:入口值为指定的用于存放时间截标注时间值的字符数组的最大长度,出口值为时间戳标注的时间值的实际长度。
*/
/*
HTTP Post 请求body：
ts_response(Base64), ts_response_length
HTTP Post 响应body：
issuer_name, issuer_name_length, time, time_length
*/
SGD_UINT32 STF_GetTSInfo(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT8 *pucIssuerName, 
	SGD_UINT32 *puiIssuerNameLength, 
	SGD_UINT8 *pucTime, 
	SGD_UINT32 *puiTimeLength);

/*
解析时间截的详细信息。
参数:
hTSHandle[IN]:时间截环境句柄
pucTSResponse[IN]:获取的时间戳响应
uiTSResponseLength [IN]:时间戳响应的长度
uiItemnumber [IN]:指定获取时间截详细信息的项目编号, 详见STFDefine.h
pucItemValue [OUT]:解析得到的时间戳相关信息
puiItemValueLength [IN/OUT]:时间截相关信息的对应长度
返回值:
0:成功;其他:失败。
备注：
当时间戳响应不包含时间戳服务器证书,调用该函数解析时间戳服务器的证书和证书链时,返回的是对应的证书序列号。
此时,无法解析签发者的国家、组织、城市和电子信箱信息;但可以正常解析签发时间、签发者通用名和时间戳请求的原始信息。
pulItemValueLength [IN/OUT]:入口时指定用于存放时间戳相关信息的字符数组的最大长度,出口时为时间戳相关信息的实际长度。
*/
/*
HTTP Post 请求body：
ts_response(Base64), ts_response_length, item_number
HTTP Post 响应body：
item_value, item_value_length
*/
SGD_UINT32 STF_GetTSDetail(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiItemnumber, 
	SGD_UINT8 *pucItemValue, 
	SGD_UINT32 *puiItemValueLength);


#endif