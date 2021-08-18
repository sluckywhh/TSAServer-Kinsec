// =================================================================================
//  File       : TSASvrCltDll.h
//  Purpose    : 时间戳服务器客户端接口。
//  Programmer : 林灶钢
//  Created    : 2006-11-21
//  CopyRight (C) 福建凯特信息安全技术有限公司
// =================================================================================

// =================================================================================
//  Remark: 
//    1、在C/C++项目中使用时包含该头文件 TSASvrCltDll.h，链接 TSASvrCltDll.lib。
//    2、VC应用中项目属性编译选项必须为 /MDd 或 /MD，设置方法如下：
//       "Project" -> "Project Settings" -> "C/C++" -> "Code Generation"
//       调试版 Use run-time library: Debug Multithreaded DLL
//       发布版 Use run-time library: Multithreaded DLL
// =================================================================================

#ifndef __TSASVRCLTDLL_H__
#define __TSASVRCLTDLL_H__

// =================================================================================
//  Include Header Files
#include <string>
using namespace std;

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  ifdef TSASVRCLTDLL_EXPORTS
#    define TSA_EXPORT __declspec( dllexport )
#  else
#    define TSA_EXPORT __declspec( dllimport )
#    if !defined(_MT)
#      error Please use the /MDd or /MT switch (Debug Multithreaded DLL)
#    endif
#	 ifdef _DEBUG
#		pragma comment( lib, "TSASvrCltDllD.lib" )
#		pragma message( "Automatically linking with TSASvrCltDllD.lib" )
#	 else
#		pragma comment( lib, "TSASvrCltDll.lib" )
#		pragma message( "Automatically linking with TSASvrCltDll.lib" )
#	 endif  // end if
#  endif
#pragma warning( disable:4786 )
#pragma warning( disable:4060 )
//#elif defined(_LINUX)
#else
#  define TSA_EXPORT
#endif

// =================================================================================
//  Interface Function Declear
// = TSA_Initialize
// Description: TSA接口初始化
TSA_EXPORT void TSA_Initialize ();


// =================================================================================
//  Interface Function Declear
// = TSA_Initialize
// Description: TSA接口清理
TSA_EXPORT void TSA_UnInitialize();

// =================================================================================
//  Interface Function Declear
// = TSA_GetErrorString
// Description: 根据错误号取错误信息
// Params: 
//   nErrNum         - [输入参数] 错误号
// Return: 返回错误信息串std::string 
TSA_EXPORT std::string TSA_GetErrorString( int nErrNum );

// = TSA_GetCurrentTime
// Description: 取当前标准时间
// Params: 
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   pTimeStampTime  - [输出参数] 时间戳时间，如：2006/11/21 15:05:16
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_GetCurrentTime( const char * szUrl, 
								  std::string * pCurrentTime );

// = TSA_GetTSACert
// Description: 取时间戳服务器证书
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   pCertData       - [输出参数] 时间戳服务器证书完整数据——BASE64编码
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_GetTSACert( const char * szUrl, std::string * pCertData );

// = TSA_TimeStamp
// Description: 制作时间戳
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   szSrc           - [输入参数] 待制作时间戳的原文数据
//   pTimeStampData  - [输出参数] 颁发返回的时间戳完整数据——BASE64编码
//   unSrcLen        - [输入参数] 原文数据的长度，若以'\0'结尾传-1，默认为-1
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_TimeStamp( const char * szUrl, 
							 const char * szSrc, 
							 std::string * pTimeStampData, 
							 unsigned int unSrcLen = -1 );
//
// = TSA_Verify
// Description: 验证时间戳
// Params: 
//   szSrc           - [输入参数] 时间戳原文
//   timeStampData   - [输入参数] 时间戳完整数据——BASE64编码
//   pCertData       - [输出参数] 时间戳服务器证书完整数据——BASE64编码
//   pTimeStampTime  - [输出参数] 时间戳时间，如：2006/11/21 15:05:16
//   unSrcLen        - [输入参数] 原文数据的长度，若以'\0'结尾传-1，默认为-1
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_Verify( const char * szSrc, 
						  std::string timeStampData, 
						  std::string * pCertData, 
						  std::string * pTimeStampTime, 
						  unsigned int unSrcLen = -1 );

// = TS_CreateReq
// Description: 制作时间戳请求
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   SrcData         - [输入参数] 待制作时间戳的原文数据
//   pTimeStampData  - [输出参数] 返回的时间戳请求数据——BASE64编码
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TS_CreateReq( const char* szUrl,const char* SrcData, std::string& ReqData);


// = TS_CreateResp
// Description: 制作时间戳
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   ReqData         - [输入参数] 时间戳请求数据
//   RespData        - [输出参数] 返回的时间戳数据——BASE64编码
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TS_CreateResp( const char* szUrl,const char* ReqData, std::string& RespData);



// = TS_Verify
// Description: 验证时间戳
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   SrcData         - [输入参数] 时间戳请求数据
//   RespData        - [输入参数] 时间戳数据——BASE64编码
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TS_Verify( const char* szUrl, const char* SrcData, const char* RespData);


// = TS_Verify
// Description: 验证时间戳
// Params:
//   SrcData         - [输入参数] 时间戳原文
//   RespData        - [输入参数] 时间戳数据——BASE64编码
//   cacert          - [输入参数] 中级CA证书——BASE64编码
//   rootcacert      - [输入参数] 根CA证书——BASE64编码
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TS_Verify( const char* SrcData, const char* RespData, const char* cacert, const char* rootcacert);


// = TS_Verify
// Description: 验证时间戳
// Params:
//   SrcData         - [输入参数] 时间戳原文(二进制数据)
//   SrcDataLen      - [输入参数] 时间戳原文数据长度
//   RespData        - [输入参数] 时间戳数据——BASE64编码
//   cacert          - [输入参数] 中级CA证书——BASE64编码
//   rootcacert      - [输入参数] 根CA证书——BASE64编码
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TS_Verify( const unsigned char* SrcData, int SrcDataLen, const char* RespData, const char* cacert, const char* rootcacert);


// = TS_Verify
// Description: 解析时间戳
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   RespData        - [输入参数] 时间戳数据
//   PraseData       - [输出参数] 时间戳时间
//   flag            - [输入参数] 标志 0-时间  1-证书
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TS_Parse( const char* szUrl, const char* RespData,  std::string& ParseData ,int flag = 0);

// = TS_Verify
// Description: 解析时间戳
// Params:
//   RespData        - [输入参数] 时间戳数据
//   PraseData       - [输出参数] 时间戳时间
//   flag            - [输入参数] 标志 0-时间  1-证书  2-签名数据  3-时间戳原文摘要  4-时间戳原文摘要算法
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TS_Parse(const char* RespData,  std::string& ParseData , int flag = 0);



// =================================================================================
//  1.00    2006-11-21    Created By LinZaoGang
//  File End 
#endif // __TSASVRCLTDLL_H__
