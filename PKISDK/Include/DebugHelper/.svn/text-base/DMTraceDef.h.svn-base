/******************************************************************

		@Author:			chenlc
		@Create:			2005-03-25
		@LastModify:		2005-03-25
		@Copyright:			kinsec (C)

******************************************************************/
#ifndef ADVTRACEDEF_H
#define ADVTRACEDEF_H


// 定义调试消息的基本编号,用于调试输出终端
#define WMTRACE_BASEID	WM_USER + 1000

#define WNTRACE_DEF_WNDNAME		"MAZE::CWNDTRACE::ASCII"

// 调试输出终端最大支持消息文本长度
#ifndef ADVTRACE_MAXMSG
#define ADVTRACE_MAXMSG		256
#endif

// 调试器的设计版本 0x,0(是否Unicode),001(主版本号),0000(次版本号)	
#ifndef _UNICODE				
#define CWNDTRACE_VERSION		0x00010000
#else
#define CWNDTRACE_VERSION		0x10010000
#endif // ! _UNICODE

#define ISADVTRACEUNICODE(v)		((v & 0x10000000)!=0)


// 调试消息内部编号
#define TRACE_INITIALIZE		1
#define TRACE_ENABLE			2
#define TRACE_FINALIZE			3
#define TRACE_MESSAGE			4

// 调试消息编号
#define WMTRACE_MESSAGE			WMTRACE_BASEID + TRACE_MESSAGE
#define WMTRACE_INITIALIZE		WMTRACE_BASEID + TRACE_INITIALIZE
#define WMTRACE_ENABLE			WMTRACE_BASEID + TRACE_ENABLE
#define WMTRACE_FINALIZE		WMTRACE_BASEID + TRACE_FINALIZE


/************************************************************************/
/*
	模式: ::SendMessage( 窗口, 消息编号, 两字节参数, 四字节参数 );
	WMTRACE_MESSAGE:
		消息编号:	WM_COPYDATA
		两字节参数:	WMTRACE_MESSAGE	
		四字节参数:	指向TRACEDATA结构的指针
	WMTRACE_INITIALIZE:
		消息编号:	WMTRACE_INITIALIZE
		两字节参数:	发送者进程编号	
		四字节参数:	发送者的调试器版本
	WMTRACE_ENABLE:
		未使用
	WMTRACE_FINALIZE:
		消息编号:	WMTRACE_FINALIZE
		两字节参数:	发送者进程编号	
		四字节参数:	未使用
		
                                                                     	*/
/************************************************************************/


// 返回代码定义
#define TRACERET_OK			0
#define TRACERET_NOERROR		TRACERET_OK
#define TRACERET_UNKNOWN		0xFFFFFFFF

// 调试消息数据结构
namespace maze {

typedef struct st_tracedata {
	DWORD dwSize;
	DWORD dwProcess;
	DWORD dwThread;
	DWORD dwParam;
	TCHAR szMessage[1];
}TRACEDATA,*LPTRACEDATA;

} // namespace maze

#endif // ! ADVTRACEDEF_H