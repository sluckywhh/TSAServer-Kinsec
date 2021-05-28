#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "TSASvrCltDll.h"
#include "TSATimeSyncApp.h"

#define PID_FILE   "Pid/TSATimeSync.pid"

CTSATimeSyncApp g_app;

void OnBeforeExit( int signal )
{
	remove( PID_FILE );
	printf( "[Info] 时间同步服务停止成功！\n" );
	exit( 0 );
}

int GetNtpCmdExitCode(const char *szServ)
{	
	char szCmd[512] = { 0 };
	//printf( "GetNtpCmdExitCode === %s\n", szServ);
	sprintf( szCmd, "ntpdate %s", szServ);
	int ret = system( szCmd );
	if(ret == -1)
	{
		return -1;
	}
	if(WIFEXITED(ret))
	{
		if(WEXITSTATUS(ret) == 0)
		{
			return 0;
		}
		else
		{
			return WEXITSTATUS(ret);
		}
	}
	else
		return WEXITSTATUS(ret);
}

void HaltSystem()
{
	system("./TSAMonitor stop");
	system("./TSAService stop");
	system("./TSService stop");
	system("./TSATimeSync stop");
}

void CheckNtpServer()
{
	if(GetNtpCmdExitCode(g_app.m_strTimeSourceAddr1.c_str()) != 0 &&
	GetNtpCmdExitCode(g_app.m_strTimeSourceAddr2.c_str()) != 0 &&
	GetNtpCmdExitCode(g_app.m_strTimeSourceAddr3.c_str()) != 0)
	{
		char szCmd[260] = { 0 };
		memset(szCmd,0,sizeof(szCmd));
		sprintf( szCmd, "./sendmail %s %s %s %s 时间戳服务器报警！ 时间戳服务器时间源故障，无法获取时间源有效时间！", 
		g_app.m_strEmailRecvAddr.c_str(), 
		g_app.m_strEmailSendAddr.c_str(), 
		g_app.m_strEmailSendUser.c_str(), 
		g_app.m_strEmailSendPwd.c_str() );
		system( szCmd );
		if(g_app.m_bHaltWhenSyncErr)
		{
			HaltSystem();
		}
	}
	
}

int compare_time(int time1, int time2, int mode)
{
	if( 1 == mode )
	{
		return abs( time1 - time2 ) <= 60 ? 1 : 0; //误差1分钟
	}
	else if( 3 == mode )
	{
		return abs( time1 - time2 ) <= 180 ? 3 : 0; //误差3分钟
	}
	else
	{
		return -1;
	}
}

int Get_GPStime(std::string ip)
{
	char szCmd[260] = { 0 };
	memset(szCmd,0,sizeof(szCmd));
	//printf( "Get_GPStime === %s\n", ip.c_str());
	sprintf( szCmd, "ntpdate %s", ip.c_str() );
	system( szCmd );
	time_t _time = time( NULL );
	return _time;
}

void sendmail()
{
	char szCmd[260] = { 0 };
	memset(szCmd,0,sizeof(szCmd));
	sprintf( szCmd, "./sendmail %s %s %s %s 时间戳服务器报警! 系统时间误差较大，系统报警并自动同步时间！", 
	g_app.m_strEmailRecvAddr.c_str(), 
	g_app.m_strEmailSendAddr.c_str(), 
	g_app.m_strEmailSendUser.c_str(), 
	g_app.m_strEmailSendPwd.c_str() );
	system( szCmd ); 
}

void Stop();
void SyncTime();
void Service()
{
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
		printf( "[Error] 启动时间同步服务出错：创建PID文件失败！\n" );
		exit( 0 );
	}

	signal( SIGTERM, OnBeforeExit );

	// Get setting.
	if( g_app.start() != 0 )
	{
		Stop();
		exit( 0 );
	}
	
	printf( "[Info] 时间同步服务启动成功！\n" );

	while( 1 )
	{
		CheckNtpServer();
		SyncTime();
		sleep( g_app.m_nInterval );
	}
}


void SyncTime()
{
	int time_choice = 0;
	time_t localtime = time( NULL );

	int t1, t2;
	int n;
	int time1 = Get_GPStime( g_app.m_strTimeSourceAddr1.c_str() );           //第一次取通过GPS校对后的系统时间,并记录为time1
	int time2 = Get_GPStime( g_app.m_strTimeSourceAddr2.c_str() );           //第二次取通过GPS校对后的系统时间,并记录为time2
	time_choice = compare_time( time1, time2, 1 );
	
	if ( 1 == time_choice )
	{
		n = compare_time( time1, localtime, 3 );
		switch( n )
		{
		default: break;
		case 0: 
			sendmail();
			if(g_app.m_bHaltWhenSyncErr)
			{
				HaltSystem();
			}
		case 3: Get_GPStime( g_app.m_strTimeSourceAddr1.c_str() );	
			break;
		}
		printf( "系统同步时间成功\n" );
	}
	else if ( 0 == time_choice )
	{
		int time3 = Get_GPStime( g_app.m_strTimeSourceAddr3.c_str() );           //第三次取通过GPS校对后的系统时间,并记录为time3
		t1 = abs( time3 - time1 );
		time_choice = compare_time(time1, time3, 1);
		if ( 1 == time_choice )
		{
			n = compare_time( time1 , localtime, 3);
			switch( n )
			{
			default: break;
			case 0:
					sendmail();
					if(g_app.m_bHaltWhenSyncErr)
					{
						HaltSystem();
					}
			case 3: Get_GPStime( g_app.m_strTimeSourceAddr1.c_str() );	
				break;
			}
			printf( "系统同步时间成功\n" );
		}
		else if ( 0 == time_choice )
		{
			t2 = abs( time3-time2 );
			time_choice = compare_time(time2, time3, 1);
			if ( 1 == time_choice )
			{
				n = compare_time( time2, localtime, 3);
				switch( n )
				{
				default: break;
				case 0:
						sendmail();
						if(g_app.m_bHaltWhenSyncErr)
						{
							HaltSystem();
						}
				case 3: Get_GPStime( g_app.m_strTimeSourceAddr2.c_str() );	
					break;
				}
				printf( "系统同步时间成功\n" );
			}
			else if ( 0 == time_choice )
			{
				if ( t1 <= t2 )
				{
					n = compare_time( time1 , localtime, 3);
					switch( n )
					{
					default: break;
					case 0: 
							sendmail();
							if(g_app.m_bHaltWhenSyncErr)
							{
								HaltSystem();
							}
					case 3: Get_GPStime( g_app.m_strTimeSourceAddr1.c_str() );
						break;
					}
					printf( "系统同步时间成功\n" );
				}
				else
				{
					n = compare_time( time2, localtime, 3);
					switch( n )
					{
					default: break;
					case 0:
							sendmail();
							if(g_app.m_bHaltWhenSyncErr)
							{
								HaltSystem();
							}
					case 3: Get_GPStime( g_app.m_strTimeSourceAddr2.c_str() );	
						break;
					}
					printf( "系统同步时间成功\n" );
				}		
			}
		}
	}
	
}

void Start()
{
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
			printf( "[Info] 时间同步服务已在运行，无法再次启动！\n" );
			return;
		}
    }
	Service();
}

void Stop()
{
    FILE * fp = fopen( PID_FILE, "rb" );
    if( fp )
	{
		pid_t pid;
		fread( &pid, 1, sizeof( pid ), fp );
		fclose( fp );
		kill( pid, SIGTERM );
	}
}

void Restart()
{
    FILE * fp = fopen( PID_FILE, "rb" );
    if( fp )
	{
		pid_t pid;
		fread( &pid, 1, sizeof( pid ), fp );
		fclose( fp );
		kill( pid, SIGTERM );
	}
	Service();
}

int main( int argc, char * argv[] )
{

	// signal( SIGCLD, SIG_IGN );
	// signal( SIGILL, SIG_IGN );
	// signal( SIGIOT, SIG_IGN );
	// signal( SIGFPE, SIG_IGN );
	// signal( SIGBUS, SIG_IGN );
	// signal( SIGSYS, SIG_IGN );
	// signal( SIGTRAP, SIG_IGN );
	// signal( SIGSEGV, SIG_IGN );

	
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

	printf( " Usage   : ./TSATimeSync start|stop|restart\n" );
    return 0;
}

