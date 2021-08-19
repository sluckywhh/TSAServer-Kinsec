#include "TSAServiceHTTPApp.h"

#define PID_FILE   "Pid/TSAServiceHTTP.pid"

CTSAServiceHTTPApp g_app;
threadpool< http_conn >* pool = NULL;
http_conn* users = new http_conn[ MAX_FD ];

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
int handleAccept(struct sockaddr_in &client_address)
{
    socklen_t client_addr_length = sizeof( client_address );
    int connfd = accept( g_app.m_nListenFD, ( struct sockaddr* )&client_address, &client_addr_length );
    if ( connfd < 0 )
    {
        printf( "[Error] handleAccept errno is: %d\n", errno );
        return 0;
    }
    if( http_conn::m_user_count >= MAX_FD )
    {
        show_error( connfd, "Internal server busy" );
        return 0;
    }
    return connfd;
} 
/***********************************************************************************/
void OnBeforeExit( int signal )
{
    close( g_app.m_nEpollFD );
    close( g_app.m_nListenFD );
    delete [] users;
    delete pool;
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
        printf( "[Info] Stop pid: %d\n", pid );
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
        printf( "[Info] Service pid: %d\n", pid );
		fclose( fp );
	}
	else
	{
		printf( "[Error] 启动时间戳颁发服务(HTTP)出错：创建PID文件失败！\n" );
		exit( 0 );
	}
    
	signal( SIGTERM, OnBeforeExit ); // 设置进程退出前处理函数
    addsig( SIGPIPE, SIG_IGN );
    //读取应用配置数据
	if( g_app.start() != 0 ) Stop();
    //创建线程池
    try
    {
        pool = new threadpool< http_conn >;
    }
    catch( ... )
    {
        Stop();
    }
    assert( users );

    g_app.initSocket();
    g_app.initEpoll();

    printf( "[Info] 时间戳颁发服务(HTTP)启动成功！\n" );
    printf( "[Info] IP:%s, Port:%d\n", g_app.m_strIP.c_str(), g_app.m_nHttpPort );
    while( true )
    {
        printf( "[Info] user count :%d\n", http_conn::m_user_count );
        int event_number = epoll_wait( g_app.m_nEpollFD, g_app.m_Events, MAX_EVENT_NUMBER, -1 );
        printf( "[Info] epoll_wait num:%d\n", event_number );
        if ( ( event_number < 0 ) && ( errno != EINTR ) )
        {
            printf( "[Error]epoll_wait failure\n" );
            Stop();
            return;
        }

        for ( int i = 0; i < event_number; i++ )
        {
            int fd = g_app.m_Events[i].data.fd;
            int events = g_app.m_Events[i].events;
            printf( "[Info] epoll_wait event fd:%d\n", fd );
            if( fd == g_app.m_nListenFD )
            {
                struct sockaddr_in client_address;
                int connfd = handleAccept(client_address);
                if(connfd > 0) users[connfd].init( connfd, client_address );
            }
            else if( events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) )
            {
                users[fd].close_conn();
            }
            else if( events & EPOLLIN ) //可读
            {
                if( users[fd].read() ) pool->append( users + fd );
                else users[fd].close_conn();
            }
            else if( events & EPOLLOUT ) //可写
            {
                if( !users[fd].write() ) users[fd].close_conn();
            }
            else
            {
                printf( "[WARNING]epoll_wait unknown event[%d]! \n", events );
            }
        }
        //sleep(3);
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
            printf( "[Info] exist Service pid: %d\n", pid );
			printf( "[Info] 时间戳颁发服务(HTTP)已在运行，无法再次启动！\n" );
			return;
		}
	}
	Service();
}
void Restart()
{
	Stop();
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
