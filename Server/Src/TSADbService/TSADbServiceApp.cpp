// =================================================================================
//  File    : TSADbServiceApp.h
//  Purpose : 时间戳数据库服务.
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
#include "TSADbServiceApp.h"
#include "XmlDocument.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
// = Default constructor.
CTSADbServiceApp::CTSADbServiceApp()
{
}

// = Default destructor.
CTSADbServiceApp::~CTSADbServiceApp()
{
}

// = Start.
int CTSADbServiceApp::start()
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

	xmlElem = setting.GetFirstChild( xmlRoot, "DatabaseName" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Name Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strDbName );

	xmlElem = setting.GetFirstChild( xmlRoot, "DatabaseUser" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database User Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strDbUser );

	xmlElem = setting.GetFirstChild( xmlRoot, "DatabasePwd" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Password Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strDbPwd );

	xmlElem = setting.GetFirstChild( xmlRoot, "DatabaseServer" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Server Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strDbServer );

	xmlElem = setting.GetFirstChild( xmlRoot, "DatabasePort" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Port Failed.\n" );
		return -1;
	}
	this->m_nDbPort = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() );

	xmlElem = setting.GetFirstChild( xmlRoot, "LogPath" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Log Path Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strLogPath );

	// Load database connection.
	if( this->m_dbConn.connect( this->m_strDbName.c_str(), this->m_strDbUser.c_str(), 
		this->m_strDbPwd.c_str(), this->m_strDbServer.c_str(), this->m_nDbPort ) != 0 )
	{
		printf( "[Error] Load Database Connection Failed.\n" );
		return -1;
	}

	return 0;
}

// = Write service log.
void CTSADbServiceApp::writeServiceLog( const char * szInfo )
{
	char szFilePath[260];
	time_t t = time( NULL );
	struct tm * pTm = localtime( &t );

#if defined(_WIN32)
	if( this->m_strLogPath[this->m_strLogPath.length() - 1] == '\\' )
		this->m_strLogPath.erase( this->m_strLogPath.length() - 1, 1 );
	_mkdir( this->m_strLogPath.c_str() );
	sprintf( szFilePath, "%s\\DbService_%d-%02d-%02d.log", this->m_strLogPath.c_str(), 
		pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday );
#elif defined(_LINUX)
	if( this->m_strLogPath[this->m_strLogPath.length() - 1] == '/' )
		this->m_strLogPath.erase( this->m_strLogPath.length() - 1, 1 );
	DIR * dir = opendir( this->m_strLogPath.c_str() );
	dir ? closedir( dir ) : mkdir( this->m_strLogPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
	sprintf( szFilePath, "%s/DbService_%d-%02d-%02d.log", this->m_strLogPath.c_str(), 
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
