// =================================================================================
//  File    : TSADbService.cpp
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================

// =================================================================================
//  Include Header Files
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(_LINUX)
#  include <unistd.h>
#  include <signal.h>
#  include <dirent.h>
#  include <sys/ipc.h>
#  include <sys/msg.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#endif
#include "TSADbServiceApp.h"

// =================================================================================
//  Macro Definition
#define PID_FILE       "Pid/TSADbService.pid"
#define MSG_PATH       "TSADbService.msg"
#define MSG_NUM        10370
CTSADbServiceApp g_app;
#if defined(_WIN32)
#elif defined(_LINUX)
int g_msgId = 0;
#endif

// =================================================================================
//  Structure Definition
struct S_Cmd_Message
{
	long lMsgType;
	char szCmd[8192];
};

// =================================================================================
//  Global Function Definition
#if defined(_LINUX)
// = Before exit.
void OnBeforeExit( int signal )
{

	remove( PID_FILE );
	if( g_msgId > 0 && msgctl( g_msgId, IPC_RMID, NULL ) == -1 )
	{
		switch( errno )
		{
		case EIDRM :
			printf( "[Error] 删除消息队列出错：当前消息队列已经被删除！\n" ); break;
		case EINVAL :
			printf( "[Error] 删除消息队列出错：无效的消息队列！\n" ); break;
		case EPERM :
			printf( "[Error] 删除消息队列出错：无权限执行该操作！\n" ); break;
		default :
			printf( "[Error] 删除消息队列出错：系统内部错误！\n" ); break;
		}
		g_msgId = 0;
	}

	printf( "[Info] 时间戳数据库服务停止成功！\n" );
	exit( 0 );
}
#endif

void Stop();
void Service()
{
	// Create child process.
#if defined(_LINUX)
	pid_t pid;
	pid = fork();
	if( pid != 0 )
		exit( 0 );

	DIR * dir = opendir( "Pid" );
	dir ? closedir( dir ) : mkdir( "Pid", S_IRWXU | S_IRWXG | S_IRWXO );
	FILE * fp = fopen( PID_FILE, "wb" );
	if( fp )
	{
		pid = getpid();
		fwrite( &pid, 1, sizeof( pid ), fp );
		fclose( fp );
	}
	else
	{
		printf( "[Error] 启动时间戳数据库服务出错：创建PID文件失败！\n" );
		exit( 0 );
	}

	// setup signal callback.
	signal( SIGTERM, OnBeforeExit );

	// Create message queue.
	key_t key = ftok( MSG_PATH, 'a' );
	g_msgId = msgget( key, IPC_CREAT | 0666 );
	if( g_msgId == -1 )
	{
		switch( errno )
		{
		case EEXIST :
			printf( "[Error] 创建消息队列出错：该消息队列已经存在！\n" ); break;
		case ENOMEM :
			printf( "[Error] 创建消息队列出错：核心内存不足！\n" ); break;
		case ENOSPC :
			printf( "[Error] 创建消息队列出错：超过可建立的消息队列最大数目！\n" ); break;
		default :
			printf( "[Error] 创建消息队列出错：系统内部错误！\n" ); break;
		}
		Stop();
		exit( 0 );
	}

	if( g_app.start() != 0 )
	{
		Stop();
		exit( 0 );
	}
	printf( "[Info] 时间戳数据库服务启动成功！\n" );

	// Wait for the cmd service message.
	S_Cmd_Message msg;
	while( 1 )
	{
		if( msgrcv( g_msgId, &msg, sizeof( msg ) - sizeof( long ), MSG_NUM, MSG_NOERROR ) == -1 )
		{
			switch( errno )
			{
			case EFAULT :
				g_app.writeServiceLog( "接收消息出错：内存缓冲区地址无效！" );
				break;
			default :
				g_app.writeServiceLog( "接收消息出错：系统内部错误！" );
				break;
			}
			continue;
		}
		
		// Handle message.
		g_app.m_dbConn.execute( msg.szCmd );
	}
#endif
}

void Start()
{
	printf( "[Info] 时间戳数据库服务开始启动\n" );
#if defined(_LINUX)
    FILE * fp = fopen( PID_FILE, "rb" );
    if( fp )
    {
		pid_t pid;
		fread( &pid, 1, sizeof( pid ), fp );
        fclose( fp );
        if( kill( pid, 0 ) )
            remove( PID_FILE );
        else
		{
			printf( "[Info] 时间戳数据库服务已在运行，无法再次启动！\n" );
			return ;
		}
    }
#endif

	Service();
}

void Stop()
{

#if defined(_LINUX)
    FILE * fp = fopen( PID_FILE, "rb" );
    if( fp )
	{
		pid_t pid;
		fread( &pid, 1, sizeof( pid ), fp );
		kill( pid, SIGTERM );
	}
#endif
}

void Restart()
{
#if defined(_LINUX)
    FILE * fp = fopen( PID_FILE, "rb" );
    if( fp )
	{
		pid_t pid;
		fread( &pid, 1, sizeof( pid ), fp );
		kill( pid, SIGTERM );
	}
#endif

	Service();
}

// =================================================================================
//  Main Function Definition
int main( int argc, char * argv[] )
{
	// 屏蔽信号
#if defined(_LINUX)
	signal( SIGCLD, SIG_IGN );
	signal( SIGILL, SIG_IGN );
	signal( SIGIOT, SIG_IGN );
	signal( SIGFPE, SIG_IGN );
	signal( SIGBUS, SIG_IGN );
	signal( SIGSYS, SIG_IGN );
	signal( SIGTRAP, SIG_IGN );
	signal( SIGSEGV, SIG_IGN );
#endif
	
	if( argc > 1 )
	{
		if( strcmp( argv[1], "start" ) == 0 )
		{
			Start();
			return 0;
		}
		else if( strcmp( argv[1], "stop" ) == 0 )
		{
			Stop();
			return 0;
		}
		else if( strcmp( argv[1], "restart" ) == 0 )
		{
			Restart();
			return 0;
		}
	}
	
	printf( " Usage   : ./TSADbService start|stop|restart\n" );
    return 0;
}


// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End  
