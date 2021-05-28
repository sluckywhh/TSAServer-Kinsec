#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char * argv[] )
{
	if( argc > 1 )
	{
		if( strcmp( argv[1], "start" ) == 0 )
		{
			system( "./TSATimeSync start" );
			sleep(5);
			system( "./TSAServer start" );
			system( "./TSService start" );
			//system( "./TSADbService start" );
			//system( "./TSAService start" );
			//system( "./TSAMonitor start" );
			return 0;
		}
		else if( strcmp( argv[1], "stop" ) == 0 )
		{
			system( "./TSService stop" );
			system( "./TSATimeSync stop" );
			system( "./TSAServer stop" );
			//system( "./TSAMonitor stop" );
			//system( "./TSAService stop" );
			//system( "./TSADbService stop" );
			
			return 0;
		}
		else if( strcmp( argv[1], "restart" ) == 0 )
		{
			system( "./TSService stop" );
			system( "./TSATimeSync stop" );
			system( "./TSAServer stop" );
			//system( "./TSAMonitor stop" );
			//system( "./TSAService stop" );
			//system( "./TSADbService stop" );

			system( "./TSATimeSync start" );
			sleep(5);
			system( "./TSAServer start" );
			system( "./TSService start" );
			//system( "./TSADbService start" );
			//system( "./TSAService start" );
			//system( "./TSAMonitor start" );
			return 0;
		}
	}

	printf( "--------------------------------------------------------\n" );
	printf( " System  : 凯特信安时间戳服务器\n" );
	printf( " Version : V1.2\n" );
	printf( " Company : 福建凯特信息安全技术有限公司\n" );
	printf( " Copyright (C) 福建凯特信息安全技术有限公司.\n\n" );
	printf( " Usage   : TSAAdmin [Option]\n" );
	printf( " Option  : start     - 启动所有服务！\n" );
	printf( "           stop      - 停止所有服务！\n" );
	printf( "           restart   - 重新启动所有服务！\n" );
	printf( "           help      - 系统帮助！\n" );
	printf( " Thank you for using this system.\n" );
	printf( "--------------------------------------------------------\n" );
    return 0;
}


