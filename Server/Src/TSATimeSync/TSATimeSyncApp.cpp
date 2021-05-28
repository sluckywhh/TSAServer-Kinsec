// =================================================================================
//  File    : TSATimeSync.cpp
//  Purpose : 时间戳服务器时间同步服务.
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================


#include "TSATimeSyncApp.h"
#include "XmlDocument.h"


CTSATimeSyncApp::CTSATimeSyncApp()
{
}


CTSATimeSyncApp::~CTSATimeSyncApp()
{
}

// = Start.
int CTSATimeSyncApp::start()
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
	
	xmlElem = setting.GetFirstChild( xmlRoot, "HaltWhenSyncErr" );  //600
	if( xmlElem == NULL )
	{
		printf( "[Error] Get HaltWhenSyncErr Failed.\n" );
		return -1;
	}
	this->m_bHaltWhenSyncErr = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str());

	xmlElem = setting.GetFirstChild( xmlRoot, "SyncTimeInterval" );  //600
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Interval Failed.\n" );
		return -1;
	}
	this->m_nInterval = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() );

	xmlElem = setting.GetFirstChild( xmlRoot, "SyncUpSecs" );  //10
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Up Secs Failed.\n" );
		return -1;
	}
	this->m_nUpSecs = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() );

	xmlElem = setting.GetFirstChild( xmlRoot, "WarnUsingEmail" );  //0
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Using Email Failed.\n" );
		return -1;
	}
	this->m_bUsingEmail = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	xmlElem = setting.GetFirstChild( xmlRoot, "WarnEmailRecvAddr" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Recv Address Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strEmailRecvAddr );

	xmlElem = setting.GetFirstChild( xmlRoot, "WarnEmailSendAddr" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Send Address Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strEmailSendAddr );

	xmlElem = setting.GetFirstChild( xmlRoot, "WarnEmailSendUser" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Send User Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strEmailSendUser );

	xmlElem = setting.GetFirstChild( xmlRoot, "WarnEmailSendPwd" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Send Password Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strEmailSendPwd );

	xmlElem = setting.GetFirstChild( xmlRoot, "SyncType" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Type Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strSyncType );

	xmlElem = setting.GetFirstChild( xmlRoot, "SyncTimeSourceAddr1" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Source Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr1 );

	xmlElem = setting.GetFirstChild( xmlRoot, "SyncTimeSourceAddr2" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Source Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr2 );

	xmlElem = setting.GetFirstChild( xmlRoot, "SyncTimeSourceAddr3" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Source Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr3 );

	return 0;
}


// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End  
