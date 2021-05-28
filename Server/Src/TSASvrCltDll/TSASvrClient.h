// =================================================================================
//  File       : TSASvrClient.h
//  Programmer : 林灶钢
//  Created    : 2006-11-28
//  CopyRight (C) 林灶钢
// =================================================================================

#ifndef __TSASVRCLIENT_H__
#define __TSASVRCLIENT_H__

// =================================================================================
//  Include Header Files
#include <string>
#include "TSASvrStream.h"
#include "TSASvrContent.h"

// =================================================================================
//  Macro Definition
#define ERROR_INFO_CODE           "ErrorCode"
#define TSERR_INVALID_URL         500  // 无效服务器地址！
#define TSERR_INVALID_SRC         501  // 无效的原文！
#define TSERR_INVALID_OUTPUT      502  // 无效的输出参数！
#define TSERR_TS_FORMAT_FAILED    503  // 传入的时间戳数据格式错误
#define TSERR_CER_FORMAT_FAILED   504  // 传入的证书格式错误
#define TSERR_INVALID_CONNECT     510  // 连接服务器超时，请确认服务器地址是否正确！
#define TSERR_SEND_TIMEOUT        511  // 发送数据超时，请重试！
#define TSERR_RECV_TIMEOUT        512  // 接收数据超时，请重试！
#define TSERR_INVALID_REQ         513  // 无效的请求数据
#define TSERR_INVALID_METHOD      514  //不支持的方法

#define TSERR_SERVER              520  // 服务器内部错误！

#define TSERR_ISSUER_FAILED       530  // 时间戳签发失败！
#define TSERR_VERIFY_FAILED       531  // 时间戳验证失败！


// =================================================================================
//  Class Definition
class CTSASvrClient
{
public :
	// = Initialize soap client interface.
	int Initialize( const char * szUrl, long lConnTimeoutSecs = 20 );

	// = Get current time.
	int GetTSATime( std::string & curTime );

	// = Get tsa cert.
	int GetTSACert( std::string & certData );

	// = Issuer time stamp.
	int IssuerTimeStamp( const char * szSrc, std::string & timeStampData );

	int TS_Req( const char * SrcData, std::string & ReqData );
	
	int TS_Resp( const char * ReqData, std::string & RespData );
	
	int TS_Verify( const char* SrcData, const char* RespData);
	
	int TS_Parse( const char* RespData, std::string& ParseData , int flag);

protected :
	// = Call remote method.
	int CallRemoteMethod( const char * szMethodName, CTSASvrContent & request, 
		CTSASvrContent & response );

	// = Send request data.
	int SendRequestData( const CTSASvrStream & stream, CTSASvrContent & request );

	// = Recv response data.
	int RecvResponseData( const CTSASvrStream & stream, CTSASvrContent & response );

	// = Remote ip address.
	char remote_ip_[64];

	// = Remote port.
	short remote_port_;

	// = Connect timeout seconds.
	long conn_timeout_;
};

// =================================================================================
//  1.00    2006-11-28    Created By LinZaoGang
//  File End 
#endif  // __TSASVRCLIENT_H__
