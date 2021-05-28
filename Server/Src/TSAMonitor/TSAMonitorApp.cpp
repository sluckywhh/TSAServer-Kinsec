// =================================================================================
//  File    : TSAMonitorApp.cpp
//  Purpose : 时间戳服务器监控服务服务.
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================

// =================================================================================
//  Include Header Files
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(_WIN32)
#  include <direct.h>
#elif defined(_LINUX)
#  include <unistd.h>
#  include <dirent.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#endif
#include "TSAMonitorApp.h"
#include "XmlDocument.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
// = Default constructor.
CTSAMonitorApp::CTSAMonitorApp()
{
}

// = Default destructor.
CTSAMonitorApp::~CTSAMonitorApp()
{
}

// = Start.
int CTSAMonitorApp::start()
{
	// Get setting.
	CXmlDocument setting;
	DOMElement * xmlElem = NULL;
	DOMElement * xmlRoot = NULL;
	if( setting.Load( SETTING_FILE ) != 0 || ( xmlRoot = setting.GetRootElement() ) == NULL )
	{
		printf( "[Error] Load TSASetting.xml Failed.\n" );
		return -1;
	}

	xmlElem = setting.GetFirstChild( xmlRoot, "ServicePort" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Service Port Failed.\n" );
		return -1;
	}
	this->m_nServicePort = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() );

	xmlElem = setting.GetFirstChild( xmlRoot, "MonitorTimeInterval" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Monitor Time Interval Failed.\n" );
		return -1;
	}
	this->m_nInterval = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() );

	xmlElem = setting.GetFirstChild( xmlRoot, "LogPath" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Log Path Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strLogPath );

	return 0;
}

// = Write monitor log.
void CTSAMonitorApp::writeMonitorLog( const char * szInfo )
{
	char szFilePath[260];
	time_t t = time( NULL );
	struct tm * pTm = localtime( &t );

#if defined(_WIN32)
	if( this->m_strLogPath[this->m_strLogPath.length() - 1] == '\\' )
		this->m_strLogPath.erase( this->m_strLogPath.length() - 1, 1 );
	_mkdir( this->m_strLogPath.c_str() );
	sprintf( szFilePath, "%s\\Monitor_%d-%02d-%02d.log", this->m_strLogPath.c_str(), 
		pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday );
#elif defined(_LINUX)
	if( this->m_strLogPath[this->m_strLogPath.length() - 1] == '/' )
		this->m_strLogPath.erase( this->m_strLogPath.length() - 1, 1 );
	DIR * dir = opendir( this->m_strLogPath.c_str() );
	dir ? closedir( dir ) : mkdir( this->m_strLogPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
	sprintf( szFilePath, "%s/Monitor_%d-%02d-%02d.log", this->m_strLogPath.c_str(), 
		pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday );
#endif

	FILE * fp = fopen( szFilePath, "a+" );
	if( fp )
	{
		fprintf( fp, "[%02d:%02d:%02d] %s\n\n", pTm->tm_hour, pTm->tm_min, pTm->tm_sec, 
			szInfo );
		fclose( fp );
	}
}


// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End  
