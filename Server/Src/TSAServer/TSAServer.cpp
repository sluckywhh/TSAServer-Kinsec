// =================================================================================
//  File    : TSAServer.cpp
//  Purpose : 时间戳服务器管理服务.
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================

#pragma warning( disable : 4786 )

// =================================================================================
//  Include Header Files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#if defined(_WIN32)
#  include <windows.h>
#  include <direct.h>
#elif defined(_LINUX)
#  include <dirent.h>
#  include <unistd.h>
#  include <pthread.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#endif
#include "TSAManager.h"
#include "TSASoapServer.h"
#include "SoapAcceptor.h"

// =================================================================================
//  Macro Definition
#define PID_FILE   "Pid/TSAServer.pid"
CSoapAcceptor g_socket;

// =================================================================================
//  Thread Function Definition
#if defined(_WIN32)
unsigned long CALLBACK ThreadFunc( void * argp )
{
#elif defined(_LINUX)
void * ThreadFunc( void * argp )
{
    pthread_detach( pthread_self() );
#endif

	CTSASoapServer * pObj = (CTSASoapServer *)argp;
	pObj->Handle();
	delete pObj;

#if defined(_WIN32)
	ExitThread( 0 );
#elif defined(_LINUX)
	pthread_exit( 0 );
#endif

	return 0;
}

// =================================================================================
//  Global Function Definition
#if defined(_LINUX)
// = Before exit.
void OnBeforeExit( int signal )
{
	remove( PID_FILE );
	printf( "[Info] 时间戳服务器管理服务停止成功！\n" );
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
		printf( "[Error] 启动时间戳服务器管理服务出错：创建PID文件失败！\n" );
		exit( 0 );
	}

	// setup signal callback.
	signal( SIGTERM, OnBeforeExit );
#endif

	// Start service.
	if( g_mgr.start() != 0 )
	{
		Stop();
		exit( 0 );
	}
	if( g_socket.Open( 10300, 8 ) != 0 )
	{
		printf( "[Error] 启动时间戳服务器管理服务出错：创建服务端监听Socket失败！\n" );
		exit( 0 );
	}

	printf( "[Info] 时间戳服务器管理启动成功！\n" );
	while( 1 )
	{
		CTSASoapServer * pObj = new CTSASoapServer;
		for( int i = 0; i < g_mgr.m_setting.m_trustCertList.size(); i++ )
		{
			FILE * fp = fopen( g_mgr.m_setting.m_trustCertList[i].c_str(), "rb" );
			if( fp )
			{
				std::vector<unsigned char> rBuff;
				fseek( fp, 0, SEEK_END );
				rBuff.resize( ftell( fp ) );
				rewind( fp );
				fread( &rBuff[0], 1, rBuff.size(), fp );
				fclose( fp );
				pObj->AddRootCert( &rBuff[0], rBuff.size() );
			}
		}
		if( g_socket.Accept( pObj ) == -1 )
		{
			delete pObj;
			continue;
		}

		// Create thread.
#if defined(_WIN32)
		unsigned long t_id;
		CreateThread( NULL, 0, ThreadFunc, (void *)pObj, 0, &t_id );
#elif defined(_LINUX)
		pthread_t t_id;
		pthread_create( &t_id, 0, (void *(*)(void *))ThreadFunc, (void *)pObj );
#endif
	}
}

void Start()
{
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
			printf( "[Info] 时间戳服务器管理服务已在运行，无法再次启动！\n" );
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
		fclose( fp );
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
		fclose( fp );
		kill( pid, SIGTERM );
	}
#endif

	Service();
}

// =================================================================================
//  Main Function Definition
int main( int argc, char * argv[] )
{
#if defined( _WIN32 ) && defined( _DEBUG )
	Start();
#endif

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

	printf( " Usage   : ./TSAServer start|stop|restart\n" );
    return 0;
}


// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End  
