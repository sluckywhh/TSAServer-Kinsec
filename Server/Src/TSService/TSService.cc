#include "TSServiceApp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/asn1.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/hmac.h>
#include <openssl/ts.h>
#include <openssl/pkcs12.h>

#define PID_FILE   "Pid/TSService.pid"

CTSServiceApp g_app;

static pthread_mutex_t *lock_cs;
static long *lock_count;
void openssl_pthreads_locking_callback(int mode, int type, char *file, int line);
unsigned long get_pthreads_thread_id(void);
void openssl_setup(void);
void opesnssl_cleanup(void);


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
	openssl_setup();
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
		printf( "[Error] 启动时间戳颁发服务(HTTP)出错：创建PID文件失败！\n" );
		exit( 0 );
	}

	signal( SIGTERM, OnBeforeExit );

	if( g_app.start() != 0 )
	{
		Stop();
		exit( 0 );
	}

	printf( "[Info] 时间戳颁发服务(HTTP)启动成功！\n" );
	while( 1 )
	{
		DK_SocketStream stream;
		if( g_app.m_acceptor.accept( stream ) != 0 )
			continue;
		g_app.m_threadPool.putRequest( DK_Request( &stream, sizeof( stream ) ) );
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

int main( int argc, char * argv[] )
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

	printf( " Usage   : ./TSService start|stop|restart\n" );
	return 0;
}

void openssl_pthreads_locking_callback(int mode, int type, char *file, int line)
{

	if (mode & CRYPTO_LOCK)
	{
		pthread_mutex_lock(&(lock_cs[type]));
		lock_count[type]++;
	}
	else
	{
		pthread_mutex_unlock(&(lock_cs[type]));
	}
}

unsigned long get_pthreads_thread_id(void)
{
	unsigned long ret;
	ret=(unsigned long)pthread_self();
	return(ret);
}

void openssl_setup(void)
{
	OpenSSL_add_all_algorithms();
	lock_cs=(pthread_mutex_t*) OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
	lock_count=(long*) OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
	for (int i=0; i<CRYPTO_num_locks(); i++)
	{
		lock_count[i]=0;
		pthread_mutex_init(&(lock_cs[i]),NULL);
	}

	CRYPTO_set_id_callback((unsigned long (*)())get_pthreads_thread_id);
	CRYPTO_set_locking_callback((void (*)(int,int,const char *,int))openssl_pthreads_locking_callback);
}

void openssl_cleanup(void)
{
	CRYPTO_set_locking_callback(NULL);
	for (int i=0; i<CRYPTO_num_locks(); i++)
	{
		pthread_mutex_destroy(&(lock_cs[i]));
	}
	OPENSSL_free(lock_cs);
	OPENSSL_free(lock_count);
	CONF_modules_unload(1);
	EVP_cleanup();
	ENGINE_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_remove_state(0);
	ERR_free_strings();
}


