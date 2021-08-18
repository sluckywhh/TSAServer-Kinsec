#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>
#include <sys/types.h>
#include <dirent.h>

#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"
#include "TSAServiceHTTPApp.h"

#define PID_FILE   "Pid/TSAServiceHTTP.pid"

#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000

CTSAServiceHTTPApp g_app;

extern int addfd( int epollfd, int fd, bool one_shot );
extern int removefd( int epollfd, int fd );

void addsig( int sig, void( handler )(int), bool restart = true )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = handler;
    if( restart )
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );
}
void show_error( int connfd, const char* info )
{
    printf( "%s", info );
    send( connfd, info, strlen( info ), 0 );
    close( connfd );
}

/***********************************************************************************/
void OnBeforeExit( int signal )
{
	remove( PID_FILE );
	printf( "[Info] 时间戳颁发服务(HTTP)停止成功！\n" );
	exit( 0 );
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

void Service()
{
	pid_t pid;
	pid = fork();
	if( pid != 0 ) exit( 0 ); //退出父进程

	DIR *dir = opendir( "Pid" );
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
		printf( "[Error] 启动时间戳颁发服务(HTTP)出错：创建PID文件失败！\n" );
		exit( 0 );
	}
    
	signal( SIGTERM, OnBeforeExit ); // 设置进程退出前处理函数

    //读取应用配置数据
	if( g_app.start() != 0 )
	{
		Stop();
	}

    const char* ip = g_app.m_strIP.c_str();
    int port = g_app.m_nHttpPort;
    addsig( SIGPIPE, SIG_IGN );

    threadpool< http_conn >* pool = NULL;
    try
    {
        pool = new threadpool< http_conn >;
    }
    catch( ... )
    {
        Stop();
    }

    http_conn* users = new http_conn[ MAX_FD ];
    assert( users );

    int listenfd = socket( AF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );
    printf( "[Info] listen fd: %d\n", listenfd );
    //struct linger tmp = { 1, 5 };
    //setsockopt( listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof( tmp ) );

    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret >= 0 );
    ret = listen( listenfd, 128 );
    assert( ret >= 0 );

    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( MAX_FD ); //创建epoll
    assert( epollfd != -1 );
    printf( "[Info] epoll fd: %d\n", epollfd );
    addfd( epollfd, listenfd, false ); //注册listenfd到epoll
    http_conn::m_epollfd = epollfd;
    printf( "[Info] 时间戳颁发服务(HTTP)启动成功！\n" );
    printf( "[Info] IP:%s, Port:%d\n", ip, port );
    while( true )
    {
        printf( "[Info] m_user_count :%d\n", http_conn::m_user_count );
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        printf( "[Info] epoll_wait num:%d\n", number );
        if ( ( number < 0 ) && ( errno != EINTR ) )
        {
            printf( "[Error]epoll failure\n" );
            Stop();
            exit( 0 );
            break;
        }

        for ( int i = 0; i < number; i++ )
        {
            int sockfd = events[i].data.fd;
            printf( "[Info] epoll event fd:%d\n", sockfd );
            if( sockfd == listenfd )
            {
                struct sockaddr_in client_address;
                socklen_t client_addr_length = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addr_length );
                if ( connfd < 0 )
                {
                    printf( "[Error] accept errno is: %d\n", errno );
                    continue;
                }
                if( http_conn::m_user_count >= MAX_FD )
                {
                    show_error( connfd, "Internal server busy" );
                    continue;
                }
                
                users[connfd].init( connfd, client_address );
            }
            else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) )
            {
                users[sockfd].close_conn();
            }
            else if( events[i].events & EPOLLIN ) //可读
            {
                if( users[sockfd].read() )
                {
                    pool->append( users + sockfd );
                }
                else
                {
                    users[sockfd].close_conn();
                }
            }
            else if( events[i].events & EPOLLOUT ) //可写
            {
                if( !users[sockfd].write() )
                {
                    users[sockfd].close_conn();
                }
            }
            else
            {}
        }
    }

    close( epollfd );
    close( listenfd );
    delete [] users;
    delete pool;
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
			printf( "[Info] 时间戳颁发服务(HTTP)已在运行，无法再次启动！\n" );
			return ;
		}
	}
	Service();
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
/***********************************************************************************/


int main( int argc, char* argv[] )
{
    signal( SIGCLD, SIG_IGN );
	signal( SIGILL, SIG_IGN );
	signal( SIGIOT, SIG_IGN );
	signal( SIGFPE, SIG_IGN );
	signal( SIGBUS, SIG_IGN );
	signal( SIGSYS, SIG_IGN );
	signal( SIGTRAP, SIG_IGN );
	signal( SIGSEGV, SIG_IGN );
	
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

	printf( " Usage   : ./TSAServiceHTTP start|stop|restart\n" );
	return 0;
}
