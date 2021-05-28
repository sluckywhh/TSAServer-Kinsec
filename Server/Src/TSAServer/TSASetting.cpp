// =================================================================================
//  File    : TSASetting.cpp
//  Purpose : 应用程序配置文件操作类
//  Author  : 林灶钢
//  Created : 2006-11-20
//  CopyRight (C) Fujian Kinsec
// =================================================================================

#pragma warning( disable : 4786 )


#include "TSASetting.h"
#include <sys/stat.h>
#include <sys/types.h>


CTSASetting::CTSASetting()
{
}

// = Get setting.
int CTSASetting::GetSetting()
{
	DOMElement * xmlElem = NULL;
	DOMElement * xmlRoot = this->GetRootElement();
	if( xmlRoot == NULL )
	{
		printf( "[Error] Load Root Element Failed.\n" );
		return -1;
	}

	// ServicePort
	xmlElem = this->GetFirstChild( xmlRoot, "ServicePort" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Service Port Failed.\n" );
		return -1;
	}
	this->m_nServicePort = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() );

	// ParallelCount
	xmlElem = this->GetFirstChild( xmlRoot, "ParallelCount" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Parallel Count Failed.\n" );
		return -1;
	}
	this->m_nParallelCount = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() );

	// CertificateFile
	xmlElem = this->GetFirstChild( xmlRoot, "CertificateFile" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Certificate File Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", m_strCertFile );

	// CertificatePin
	xmlElem = this->GetFirstChild( xmlRoot, "CertificatePin" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Certificate Pin Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", m_strCertPin );

	// SyncTimeInterval
	xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeInterval" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Interval Failed.\n" );
		return -1;
	}
	this->m_nSyncTimeInterval = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() );

	// SyncUpSecs
	xmlElem = this->GetFirstChild( xmlRoot, "SyncUpSecs" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Up Secs Failed.\n" );
		return -1;
	}
	this->m_nUpSecs = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() );

	// WarnUsingEmail
	xmlElem = this->GetFirstChild( xmlRoot, "WarnUsingEmail" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Using Email Failed.\n" );
		return -1;
	}
	this->m_bUsingEmail = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	// WarnEmailRecvAddr
	xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailRecvAddr" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Recv Address Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strEmailRecvAddr );

	// WarnEmailSendAddr
	xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailSendAddr" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Send Address Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strEmailSendAddr );

	// WarnEmailSendUser
	xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailSendUser" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Send User Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strEmailSendUser );

	// WarnEmailSendPwd
	xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailSendPwd" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Warn Email Send Password Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strEmailSendPwd );

	// SyncType
	xmlElem = this->GetFirstChild( xmlRoot, "SyncType" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Type Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strSyncType );

	// SyncTimeSourceAddr
	xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeSourceAddr1" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Source Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr1 );

	xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeSourceAddr2" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Source Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr2 );

	xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeSourceAddr3" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sync Time Source Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr3 );

	// UsingDatabase
	xmlElem = this->GetFirstChild( xmlRoot, "UsingDatabase" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Using Database Failed.\n" );
		return -1;
	}
	this->m_bUsingDb = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	// DatabaseBackup
	xmlElem = this->GetFirstChild( xmlRoot, "DatabaseBackup" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Backup Path Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strDbBackupPath );
	mkdir(m_strDbBackupPath.c_str(), 0777);

	// DatabaseName
	xmlElem = this->GetFirstChild( xmlRoot, "DatabaseName" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Name Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strDbName );

	// DatabaseUser
	xmlElem = this->GetFirstChild( xmlRoot, "DatabaseUser" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database User Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strDbUser );

	// DatabasePwd
	xmlElem = this->GetFirstChild( xmlRoot, "DatabasePwd" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Password Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strDbPwd );

	// DatabaseServer
	xmlElem = this->GetFirstChild( xmlRoot, "DatabaseServer" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Server Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strDbServer );

	// DatabasePort
	xmlElem = this->GetFirstChild( xmlRoot, "DatabasePort" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Database Port Failed.\n" );
		return -1;
	}
	this->m_nDbPort = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() );

	// TSAPolicy
	xmlElem = this->GetFirstChild( xmlRoot, "TSAPolicy" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get TSA Policy Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strTsaPolicy );
		
	// TSAUsingOrdering
	xmlElem = this->GetFirstChild( xmlRoot, "TSAUsingOrdering" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get TSA Using Ordering Failed.\n" );
		return -1;
	}
	this->m_bTsaUsingOrdering = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	// TSAUsingAccuracy
	xmlElem = this->GetFirstChild( xmlRoot, "TSAUsingAccuracy" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get TSA Using Accuracy Failed.\n" );
		return -1;
	}
	this->m_bTsaUsingAccuracy = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	// TSAAccuracySecs
	xmlElem = this->GetFirstChild( xmlRoot, "TSAAccuracySecs" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get TSA Accuracy Seconds Failed.\n" );
		return -1;
	}
	this->m_nTsaAccuracySecs = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	// TSAAccuracyMillisecs
	xmlElem = this->GetFirstChild( xmlRoot, "TSAAccuracyMillisecs" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get TSA Accuracy Milli Seconds Failed.\n" );
		return -1;
	}
	this->m_nTsaAccuracyMillisecs = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	// TSA_AccuracyMicrosecs
	xmlElem = this->GetFirstChild( xmlRoot, "TSAAccuracyMicrosecs" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get TSA Accuracy Micro Seconds Failed.\n" );
		return -1;
	}
	this->m_nTsaAccuracyMicrosecs = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	// Admin List.
	DOMNodeList * xmlList = this->GetChildren( xmlRoot, "Admin" );
	if( xmlList )
	{
		S_Admin admin;
		this->m_adminList.clear();
		for( int j = 0; j < xmlList->getLength(); j++ )
		{
			xmlElem = (DOMElement *)xmlList->item( j );
			this->GetElementAttribute( xmlElem, "SerialNumber", admin.serialNumber );
			this->GetElementAttribute( xmlElem, "CommonName", admin.commonName );
			this->m_adminList.push_back( admin );
		}
	}

	// MonitorTimeInterval
	xmlElem = this->GetFirstChild( xmlRoot, "MonitorTimeInterval" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Monitor Time Interval Failed.\n" );
		return -1;
	}
	this->m_nMonitorTimeInterval = atoi( this->GetElementAttribute( xmlElem, "Description" ).c_str() );

	// LogPath
	xmlElem = this->GetFirstChild( xmlRoot, "LogPath" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Log Path Failed.\n" );
		return -1;
	}
	this->GetElementAttribute( xmlElem, "Description", this->m_strLogPath );

	// TrustCert List.
	xmlList = this->GetChildren( xmlRoot, "TrustCert" );
	if( xmlList )
	{
		this->m_trustCertList.clear();
		for( int j = 0; j < xmlList->getLength(); j++ )
		{
			DOMElement * xmlElemTmp = (DOMElement *)xmlList->item( j );
			this->m_trustCertList.push_back( this->GetElementAttribute( xmlElemTmp, "Description" ).c_str() );
		}
	}

	return 0;
}

// = Set setting.
int CTSASetting::SetSetting( unsigned long ulMask )
{
	DOMElement * xmlElem = NULL;
	DOMElement * xmlRoot = this->GetRootElement();
	if( xmlRoot == NULL )
	{
		printf( "[Error] Load Root Element Failed.\n" );
		return -1;
	}

	char szDes[32] = { 0 };
	if( ulMask & SSF_SERVICE )
	{
		// ServicePort
		xmlElem = this->GetFirstChild( xmlRoot, "ServicePort" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "ServicePort" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nServicePort );
		this->SetElementAttribute( xmlElem, "Description", szDes );

		// ParallelCount
		xmlElem = this->GetFirstChild( xmlRoot, "ParallelCount" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "ParallelCount" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nParallelCount );
		this->SetElementAttribute( xmlElem, "Description", szDes );
	}
	if( ulMask & SSF_CERT )
	{
		// CertificateFile
		xmlElem = this->GetFirstChild( xmlRoot, "CertificateFile" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "CertificateFile" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strCertFile.c_str() );

		// CertificatePin
		xmlElem = this->GetFirstChild( xmlRoot, "CertificatePin" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "CertificatePin" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strCertPin.c_str() );
	}
	if( ulMask & SSF_EMAIL )
	{
		// SyncTimeInterval
		xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeInterval" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "SyncTimeInterval" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nSyncTimeInterval );
		this->SetElementAttribute( xmlElem, "Description", szDes );

		// SyncType
		xmlElem = this->GetFirstChild( xmlRoot, "SyncType" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "SyncType" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strSyncType.c_str() );

		// SyncTimeSource
		xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeSourceAddr1" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "SyncTimeSourceAddr1" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr1.c_str() );

		xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeSourceAddr2" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "SyncTimeSourceAddr2" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr2.c_str() );

		xmlElem = this->GetFirstChild( xmlRoot, "SyncTimeSourceAddr3" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "SyncTimeSourceAddr3" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strTimeSourceAddr3.c_str() );

	}
	if( ulMask & SSF_TIME )
	{
		// WarnUsingEmail
		xmlElem = this->GetFirstChild( xmlRoot, "WarnUsingEmail" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "WarnUsingEmail" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_bUsingEmail ? "1" : "0" );

		// WarnEmailRecvAddr
		xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailRecvAddr" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "WarnEmailRecvAddr" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strEmailRecvAddr.c_str() );

		// WarnEmailSendAddr
		xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailSendAddr" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "WarnEmailSendAddr" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strEmailSendAddr.c_str() );

		// WarnEmailSendUser
		xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailSendUser" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "WarnEmailSendUser" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strEmailSendUser.c_str() );

		// WarnEmailSendPwd
		xmlElem = this->GetFirstChild( xmlRoot, "WarnEmailSendPwd" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "WarnEmailSendPwd" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strEmailSendPwd.c_str() );
		
	}
	if( ulMask & SSF_DATABASE )
	{
		// UsingDatabase
		xmlElem = this->GetFirstChild( xmlRoot, "UsingDatabase" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "UsingDatabase" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_bUsingDb ? "1" : "0" );

		// DatabaseBackup
		xmlElem = this->GetFirstChild( xmlRoot, "DatabaseBackup" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "DatabaseBackup" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strDbBackupPath.c_str() );

		// DatabaseName
		xmlElem = this->GetFirstChild( xmlRoot, "DatabaseName" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "DatabaseName" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strDbName.c_str() );

		// DatabaseUser
		xmlElem = this->GetFirstChild( xmlRoot, "DatabaseUser" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "DatabaseUser" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strDbUser.c_str() );

		// DatabasePwd
		xmlElem = this->GetFirstChild( xmlRoot, "DatabasePwd" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "DatabasePwd" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strDbPwd.c_str() );

		// DatabaseServer
		xmlElem = this->GetFirstChild( xmlRoot, "DatabaseServer" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "DatabaseServer" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strDbServer.c_str() );

		// DatabasePort
		xmlElem = this->GetFirstChild( xmlRoot, "DatabasePort" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "DatabasePort" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nDbPort );
		this->SetElementAttribute( xmlElem, "Description", szDes );
	}
	if( ulMask & SSF_TSA )
	{
		// TSAPolicy
		xmlElem = this->GetFirstChild( xmlRoot, "TSAPolicy" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "TSAPolicy" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strTsaPolicy.c_str() );

		// TSAUsingOrdering
		xmlElem = this->GetFirstChild( xmlRoot, "TSAUsingOrdering" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "TSAUsingOrdering" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_bTsaUsingOrdering ? "1" : "0" );

		// TSAUsingAccurary
		xmlElem = this->GetFirstChild( xmlRoot, "TSAUsingAccurary" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "TSAUsingAccurary" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_bTsaUsingAccuracy ? "1" : "0" );

		// TSAAccuracySecs
		xmlElem = this->GetFirstChild( xmlRoot, "TSAAccuracySecs" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "TSAAccuracySecs" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nTsaAccuracySecs );
		this->SetElementAttribute( xmlElem, "Description", szDes );

		// TSAAccuracyMillisecs
		xmlElem = this->GetFirstChild( xmlRoot, "TSAAccuracyMillisecs" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "TSAAccuracyMillisecs" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nTsaAccuracyMillisecs );
		this->SetElementAttribute( xmlElem, "Description", szDes );

		// TSAAccuracyMicrosecs
		xmlElem = this->GetFirstChild( xmlRoot, "TSAAccuracyMicrosecs" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "TSAAccuracyMicrosecs" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nTsaAccuracyMicrosecs );
		this->SetElementAttribute( xmlElem, "Description", szDes );
	}
	if( ulMask & SSF_ADMIN )
	{
		DOMNodeList * xmlList = this->GetChildren( xmlRoot, "Admin" );
		if( xmlList )
		{
			for( int i = xmlList->getLength() - 1; i >= 0; i-- )
				this->RemoveElement( (DOMElement *)xmlList->item( i ) );
		}
		for( int i = 0; i < this->m_adminList.size(); i++ )
		{
			xmlElem = this->AppendChild( xmlRoot, "Admin" );
			if( xmlElem )
			{
				this->SetElementAttribute( xmlElem, "SerialNumber", this->m_adminList[i].serialNumber.c_str() );
				this->SetElementAttribute( xmlElem, "CommonName", this->m_adminList[i].commonName.c_str() );
			}
		}
	}
	if( ulMask & SSF_SYSTEM )
	{
		// MonitorTimeInterval
		xmlElem = this->GetFirstChild( xmlRoot, "MonitorTimeInterval" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "MonitorTimeInterval" );
		if( xmlElem == NULL )
			return -1;
		sprintf( szDes, "%d", this->m_nMonitorTimeInterval );
		this->SetElementAttribute( xmlElem, "Description", szDes );

		// LogPath
		xmlElem = this->GetFirstChild( xmlRoot, "LogPath" );
		if( xmlElem == NULL )
			this->AppendChild( xmlRoot, "LogPath" );
		if( xmlElem == NULL )
			return -1;
		this->SetElementAttribute( xmlElem, "Description", this->m_strLogPath.c_str() );
	}

	this->Save();
	return 0;
}


// =================================================================================
//  1.00    2006-11-20    Created By LinZaoGang
//  File End 
