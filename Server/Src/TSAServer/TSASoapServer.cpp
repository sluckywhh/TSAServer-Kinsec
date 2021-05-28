  // =================================================================================
//  File    : TSASoapServer.cpp
//  Purpose : Soap Server Class Definition.
//  Author  : 林灶钢
//  Created : 2006-11-18
//  CopyRight (C) 林灶钢.
// =================================================================================

#include "TSAManager.h"
#include "TSASoapServer.h"
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dirent.h>
#include <string>
using std::string;


// =================================================================================
//  Macro Definition
#define MSG_PATH       "TSADbService.msg"
#define MSG_NUM        10370

// =================================================================================
//  Structure Definition
struct S_Cmd_Message
{
	long lMsgType;
	char szCmd[8192];

	S_Cmd_Message()
	{
		lMsgType = MSG_NUM;
	}
};

// =================================================================================
//  Global Function Definition
int ExecuteSQLCommand( const char * szCmd )
{
	S_Cmd_Message msg;
	strcpy( msg.szCmd, szCmd );

	key_t key;
	key = ftok( MSG_PATH, 'a' );
	int msgId = msgget( key, 0 );
	if( msgId < 0 )
	{
		printf( "[Error] Write Cmd Failed: Get Message Queue Error.\n" );
		return -1;
	}
	// Send the message to index process
	if( msgsnd( msgId, &msg, sizeof( msg ) - sizeof( long ), 0 ) == -1 )
	{
		printf( "[Error] Write Cmd Failed: Send Cmd Message Error.\n" );
		return -1;
	}
	return 0;
}

int CTSASoapServer::FindFnDoHandle( const char * szMethod )
{
	const char * pClientIP = this->cert_info_.GetValue( SECSOAP_CLIENT_IP );
	const char * pCertSN = this->cert_info_.GetValue( SECSOAP_CERT_SERIAL_NUMBER );
	const char * pCertCN = this->cert_info_.GetValue( SECSOAP_CERT_CN );
	time_t t = time( NULL );
	struct tm * pTm = localtime( &t );
	char szVisitTime[32] = { 0 };
	sprintf( szVisitTime, "%d-%02d-%02d %02d:%02d:%02d", pTm->tm_year + 1900,
	         pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec );
	
	printf("cn= %s num=%s rpc=%s\n", pCertCN, pCertSN, szMethod);
	
	string sql;
	// Check administrator.
	if( g_mgr.m_setting.m_adminList.size() == 0 )
	{
		S_Admin admin;
		admin.serialNumber = pCertSN;
		admin.commonName = pCertCN;
		g_mgr.m_setting.m_adminList.push_back( admin );
		g_mgr.m_setting.SetSetting( SSF_ADMIN );
		sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
		sql += szVisitTime;
		sql += "', '";
		sql += pCertCN;
		sql += "', '";
		sql += "添加系统管理员：";
		sql += pCertCN;
		sql += "')";
	}
	else
	{
		int i = 0;
		for( ; i < g_mgr.m_setting.m_adminList.size(); i++ )
			if( strcmp( g_mgr.m_setting.m_adminList[i].serialNumber.c_str(), pCertSN ) == 0 )
				break;
		if( i == g_mgr.m_setting.m_adminList.size() )
		{
			this->response_.AddValue( ERROR_INFO_NAME, "您不是当前服务器的用户，无权操作！" );
			
			sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
			sql += szVisitTime;
			sql += "', '";
			sql += pCertCN;
			sql += "', '";
			sql += "试图访问系统因权限不足被拒绝";
			sql += "')";
			return -1;
		}

	}

	// Remote function call.
	if( strcmp( szMethod, "LoginTSAServer" ) == 0 )
	{
		sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
		sql += szVisitTime;
		sql += "', '";
		sql += pCertCN;
		sql += "', '";
		sql += "登录时间戳服务管理系统";
		sql += "')";
	}
	else if( strcmp( szMethod, "RestartService" ) == 0 )
	{
		system( "./TSAService restart" );
		sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
		sql += szVisitTime;
		sql += "', '";
		sql += pCertCN;
		sql += "', '";
		sql += "重启时间戳颁发服务";
		sql += "')";
	}
	else if(strcmp( szMethod, "RestartTimeSyncService" ) == 0)
	{
		system( "./TSATimeSync restart" );
		sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
		sql += szVisitTime;
		sql += "', '";
		sql += pCertCN;
		sql += "', '";
		sql += "重启时间戳时间同步服务";
		sql += "')";
	}
	else if( strcmp( szMethod, "RestartSystem" ) == 0 )
	{
		sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
		sql += szVisitTime;
		sql += "', '";
		sql += pCertCN;
		sql += "', '";
		sql += "重启时间戳服务器";
		sql += "')";
		ExecuteSQLCommand(sql.c_str());
		system( "reboot" );
		
	}
	else if( strcmp( szMethod, "DBBackup") == 0 )
	{
		time_t t = time( NULL );
		struct tm * pTm = localtime( &t);
		char szCmd[300] = { 0 };
		
		sprintf( szCmd, "pg_dump %s > %s/TSADB%d-%02d-%02d.dump", 
			g_mgr.m_setting.m_strDbName.c_str(), g_mgr.m_setting.m_strDbBackupPath.c_str(), 
			pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday );
		int ret = system( szCmd );
		
		sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
		sql += szVisitTime;
		sql += "', '";
		sql += pCertCN;
		sql += "', '";
		sql += "备份时间戳服务系统数据库";
		sql += "')";
	}
	else if( strcmp( szMethod, "GetBackupPath" ) == 0 )
	{
		this->response_.AddValue( "BackupPath", g_mgr.m_setting.m_strDbBackupPath.c_str() );
		
	}
	else if( strcmp( szMethod, "GetBackupList" ) == 0 )
	{
		std::vector<std::string> backupList;
		struct stat buf;
		struct dirent * dirp;
		DIR * dp = opendir( g_mgr.m_setting.m_strDbBackupPath.c_str() );
		if( dp )
		{
			while( ( dirp = readdir( dp ) ) )
			{
				char * pBegin = strrchr( dirp->d_name, '.' );
				if( strcmp( pBegin, ".dump" ) == 0 )
					backupList.push_back( dirp->d_name );
			}
		}
		closedir( dp );
		char szDes[32] = { 0 };
		sprintf( szDes, "%d", backupList.size() );
		this->response_.AddValue( "BackupList.Count", szDes );
		for( int i = 0; i < backupList.size(); i++ )
		{
			sprintf( szDes, "BackupList[%d].Name", i );
			this->response_.AddValue( szDes, backupList[i].c_str() );
		}
		
	}
	else if( strcmp( szMethod, "GetAdminList" ) == 0 )
	{
		S_AdminList adminList;
		char szDes[32] = { 0 };
		sprintf( szDes, "%d", g_mgr.m_setting.m_adminList.size() );
		this->response_.AddValue( "AdminList.Count", szDes );
		for( int i = 0; i < g_mgr.m_setting.m_adminList.size(); i++ )
		{
			sprintf( szDes, "AdminList[%d].SerialNumber", i );
			this->response_.AddValue( szDes, g_mgr.m_setting.m_adminList[i].serialNumber.c_str() );
			sprintf( szDes, "AdminList[%d].CommonName", i );
			this->response_.AddValue( szDes, g_mgr.m_setting.m_adminList[i].commonName.c_str() );
		}
		
	}
	else if( strcmp( szMethod, "AddAdmin" ) == 0 )
	{
		const char * pSerialNumber = this->request_.GetValue( "SerialNumber" );
		const char * pCommonName = this->request_.GetValue( "CommonName" );
		int i = 0;
		for( ; i < g_mgr.m_setting.m_adminList.size(); i++ )
		{
			if( strcmp( g_mgr.m_setting.m_adminList[i].serialNumber.c_str(), pSerialNumber ) == 0 )
			{
				this->response_.AddValue( ERROR_INFO_NAME, "该证书已经是管理员，不必重复添加！" );
				return -1;
			}
		}
		S_Admin admin;
		admin.serialNumber = pSerialNumber;
		admin.commonName = pCommonName;
		g_mgr.m_setting.m_adminList.push_back( admin );
		g_mgr.m_setting.SetSetting( SSF_ADMIN );
		
		sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
		sql += szVisitTime;
		sql += "', '";
		sql += pCertCN;
		sql += "', '";
		sql += "添加系统管理员：";
		sql += pCommonName;
		sql += "')";	
	}
	else if( strcmp( szMethod, "RemoveAdmin" ) == 0 )
	{
		const char * pSerialNumber = this->request_.GetValue( "SerialNumber" );
		const char * pCommonName = this->request_.GetValue( "CommonName" );
		std::vector<S_Admin>::iterator i = g_mgr.m_setting.m_adminList.begin();
		for( ; i < g_mgr.m_setting.m_adminList.end(); i++ )
		{
			if( strcmp( (*i).serialNumber.c_str(), pSerialNumber ) == 0 )
			{
				g_mgr.m_setting.m_adminList.erase( i );
				g_mgr.m_setting.SetSetting( SSF_ADMIN );
				
				sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
				sql += szVisitTime;
				sql += "', '";
				sql += pCertCN;
				sql += "', '";
				sql += "删除系统管理员：";
				sql += pCommonName;
				sql += "')";	
				break;
			}
		}
	}
	else if( strcmp( szMethod, "GetSetting" ) == 0 )
	{
		char szDes[32] ={ 0 };
		unsigned long ulMask = atol( this->request_.GetValue( "Mask" ) );
		if( ulMask & SSF_SERVICE )
		{
			sprintf( szDes, "%d", g_mgr.m_setting.m_nServicePort );
			this->response_.AddValue( "ServicePort", szDes );
			sprintf( szDes, "%d", g_mgr.m_setting.m_nParallelCount );
			this->response_.AddValue( "ServiceBacklog", szDes );
		}
		if( ulMask & SSF_CERT )
		{
		}
		if( ulMask & SSF_TIME )
		{
			sprintf( szDes, "%d", g_mgr.m_setting.m_nSyncTimeInterval );
			this->response_.AddValue( "TimeSyncInterval", szDes );
			sprintf( szDes, "%d", g_mgr.m_setting.m_nUpSecs );
			this->response_.AddValue( "TimeUpSecs", szDes );
			this->response_.AddValue( "TimeSourceAddr1", g_mgr.m_setting.m_strTimeSourceAddr1.c_str() );
			this->response_.AddValue( "TimeSourceAddr2", g_mgr.m_setting.m_strTimeSourceAddr2.c_str() );
			this->response_.AddValue( "TimeSourceAddr3", g_mgr.m_setting.m_strTimeSourceAddr3.c_str() );
			this->response_.AddValue( "TimeSyncType", g_mgr.m_setting.m_strSyncType.c_str() );
			this->response_.AddValue( "WarnUseEmail", g_mgr.m_setting.m_bUsingEmail ? "1" : "0" );
			this->response_.AddValue( "WarnRecvAddr", g_mgr.m_setting.m_strEmailRecvAddr.c_str() );
			this->response_.AddValue( "WarnSendAddr", g_mgr.m_setting.m_strEmailSendAddr.c_str() );
			this->response_.AddValue( "WarnSendUser", g_mgr.m_setting.m_strEmailSendUser.c_str() );
			this->response_.AddValue( "WarnSendPwd", g_mgr.m_setting.m_strEmailSendPwd.c_str() );
		}
		if( ulMask & SSF_DATABASE )
		{
		}
		if( ulMask & SSF_TSA )
		{
		}
		
	}
	else if( strcmp( szMethod, "SetSetting" ) == 0 )
	{
		unsigned long ulMask = atol( this->request_.GetValue( "Mask" ) );
		if( ulMask & SSF_SERVICE )
		{
			g_mgr.m_setting.m_nServicePort = atoi( this->request_.GetValue( "ServicePort" ) );
			g_mgr.m_setting.m_nParallelCount = atoi( this->request_.GetValue( "ServiceBacklog" ) );
			
				sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
				sql += szVisitTime;
				sql += "', '";
				sql += pCertCN;
				sql += "', '";
				sql += "修改系统服务配置：服务端口：";
				sql += request_.GetValue( "ServicePort" );
				sql += ",服务请求队列长度：";
				sql += request_.GetValue( "ServiceBacklog" );
				sql += "')";	
				
		}
		if( ulMask & SSF_CERT )
		{
		}
		if( ulMask & SSF_TIME )
		{
			g_mgr.m_setting.m_nSyncTimeInterval = atoi( this->request_.GetValue("TimeSyncInterval"));
			g_mgr.m_setting.m_nUpSecs = atoi(this->request_.GetValue("TimeUpSecs"));
			g_mgr.m_setting.m_strTimeSourceAddr1 = this->request_.GetValue( "TimeSourceAddr1" );
			g_mgr.m_setting.m_strTimeSourceAddr2 = this->request_.GetValue( "TimeSourceAddr2" );
			g_mgr.m_setting.m_strTimeSourceAddr3 = this->request_.GetValue( "TimeSourceAddr3" );
			g_mgr.m_setting.m_strSyncType = this->request_.GetValue( "TimeSyncType" );
			
			sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
			sql += szVisitTime;
			sql += "', '";
			sql += pCertCN;
			sql += "', '";
			sql += "修改时间同步配置：同步类型：";
			sql += g_mgr.m_setting.m_strSyncType;	
			sql += ",时间源地址：";
			sql += g_mgr.m_setting.m_strTimeSourceAddr1;
			sql += ",";
			sql += g_mgr.m_setting.m_strTimeSourceAddr2;
			sql += ",";
			sql += g_mgr.m_setting.m_strTimeSourceAddr3;
			sql += ",时间同步间隔：";
			sql += request_.GetValue("TimeSyncInterval");
			sql += "')";	

		}
		if( ulMask & SSF_EMAIL)
		{
			g_mgr.m_setting.m_bUsingEmail = atoi( this->request_.GetValue( "WarnUseEmail" ) ) == 1;
			g_mgr.m_setting.m_strEmailRecvAddr = this->request_.GetValue( "WarnRecvAddr" );
			g_mgr.m_setting.m_strEmailSendAddr = this->request_.GetValue( "WarnSendAddr" );
			g_mgr.m_setting.m_strEmailSendUser = this->request_.GetValue( "WarnSendUser" );
			g_mgr.m_setting.m_strEmailSendPwd = this->request_.GetValue( "WarnSendPwd" );
			
				sql = "insert into t_setting_log(f_log_time, f_setting_user, f_setting_info) values ('";
				sql += szVisitTime;
				sql += "', '";
				sql += pCertCN;
				sql += "', '";
				sql += "修改邮件报警配置：";
				if(g_mgr.m_setting.m_bUsingEmail)
				{
					sql += "启用邮件报警";
				}
				else
				{
					sql += "关闭邮件报警";
				}
				sql += ",接收者邮件地址：";
				sql += g_mgr.m_setting.m_strEmailRecvAddr;
				sql += ",发送者邮件地址：";
				sql += g_mgr.m_setting.m_strEmailSendAddr;
				sql += ",发送者名称：";
				sql += g_mgr.m_setting.m_strEmailSendUser;
				sql += ",发送者邮箱密码：";
				sql += g_mgr.m_setting.m_strEmailSendPwd;
				sql += "')";	
		}
		if( ulMask & SSF_TSA )
		{
		}
		g_mgr.m_setting.SetSetting( ulMask );
		
	}
	else if( strcmp( szMethod, "QueryTimeStamp" ) == 0 )
	{
		std::string filter = "f_entry_time >= '";
		filter.append( this->request_.GetValue( "StartTime" ) );
		filter.append( "' and f_entry_time <= '" );
		filter.append( this->request_.GetValue( "EndTime" ) );
		filter.append( "'" );
		if( strlen( this->request_.GetValue( "SerialNumber" ) ) > 0 )
		{
			filter.append( " and f_serial_number = '" );
			filter.append( this->request_.GetValue( "SerialNumber" ) );
			filter.append( "'" );
		}
		if( strlen( this->request_.GetValue( "FullData" ) ) > 0 )
		{
			filter.append( " and f_data = '" );
			filter.append( this->request_.GetValue( "FullData" ) );
			filter.append( "'" );
		}

		DK_PgSQLRecordset rs( &g_mgr.m_dbConn );
		if( rs.open( "t_time_stamp", filter.c_str() ) != 0 )
		{
			this->response_.AddValue( ERROR_INFO_NAME, "打开数据库表出错！" );
			return -1;
		}
		char szDes[64];
		sprintf( szDes, "%ld", rs.getRecordCount() );
		this->response_.AddValue( "TimeStampList.Count", szDes );
		int i = 0;
		while( !rs.isEof() )
		{
			sprintf( szDes, "TimeStampList[%d].SerialNumber", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_serial_number" ) );
			sprintf( szDes, "TimeStampList[%d].EntryTime", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_entry_time" ) );
			sprintf( szDes, "TimeStampList[%d].FullData", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_data" ) );
			rs.moveNext();
			i++;
		}
		
	}
	else if( strcmp( szMethod, "QueryVisitLog" ) == 0 )
	{
		std::string filter = "f_log_time >= '";
		filter.append( this->request_.GetValue( "StartTime" ) );
		filter.append( "' and f_log_time <= '" );
		filter.append( this->request_.GetValue( "EndTime" ) );
		filter.append( "'" );
		filter.append(" order by f_log_time ");
		filter.append("limit 17 offset ");
		filter.append(this->request_.GetValue( "OffSet" ));

		DK_PgSQLRecordset rs( &g_mgr.m_dbConn );
		if( rs.open( "t_visit_log ", filter.c_str() ) != 0 )
		{
			this->response_.AddValue( ERROR_INFO_NAME, "打开数据库表出错！" );
			return -1;
		}
		char szDes[64];
		sprintf( szDes, "%ld", rs.getRecordCount() );
		this->response_.AddValue( "LogList.Count", szDes );
		int i = 0;
		while( !rs.isEof() )
		{
			sprintf( szDes, "LogList[%d].LogTime", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_log_time" ) );
			sprintf( szDes, "LogList[%d].ClientIP", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_client_ip" ) );
			sprintf( szDes, "LogList[%d].Operater", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_operater" ) );
			rs.moveNext();
			i++;
		}
		
	}
	else if( strcmp( szMethod, "QueryRequestLog" ) == 0 )
	{
		std::string filter = "f_log_time >= '";
		filter.append( this->request_.GetValue( "StartTime" ) );
		filter.append( "' and f_log_time <= '" );
		filter.append( this->request_.GetValue( "EndTime" ) );
		filter.append( "'" );
		filter.append(" order by f_log_time ");
		filter.append("limit 17 offset ");
		filter.append(this->request_.GetValue( "OffSet" ));
		
		DK_PgSQLRecordset rs( &g_mgr.m_dbConn );
		if( rs.open( "t_request_log ", filter.c_str() ) != 0 )
		{
			this->response_.AddValue( ERROR_INFO_NAME, "打开数据库表出错！" );
			return -1;
		}
		char szDes[64];
		sprintf( szDes, "%ld", rs.getRecordCount() );
		this->response_.AddValue( "LogList.Count", szDes );
		int i = 0;
		while( !rs.isEof() )
		{
			sprintf( szDes, "LogList[%d].LogTime", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_log_time" ) );
			sprintf( szDes, "LogList[%d].RequestData", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_request_data" ) );
			sprintf( szDes, "LogList[%d].ResponseData", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_response_data" ) );
			sprintf( szDes, "LogList[%d].Status", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_status" ) );
			rs.moveNext();
			i++;
		}
		
	}
	else if( strcmp( szMethod, "QuerySettingLog" ) == 0 )
	{
		std::string filter = "f_log_time >= '";
		filter.append( this->request_.GetValue( "StartTime" ) );
		filter.append( "' and f_log_time <= '" );
		filter.append( this->request_.GetValue( "EndTime" ) );
		filter.append( "'" );

		DK_PgSQLRecordset rs( &g_mgr.m_dbConn );
		if( rs.open( "t_setting_log ", filter.c_str() ) != 0 )
		{
			this->response_.AddValue( ERROR_INFO_NAME, "打开数据库表出错！" );
			return -1;
		}
		char szDes[64];
		sprintf( szDes, "%ld", rs.getRecordCount() );
		this->response_.AddValue( "LogList.Count", szDes );
		int i = 0;
		while( !rs.isEof() )
		{
			sprintf( szDes, "LogList[%d].LogTime", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_log_time" ) );
			sprintf( szDes, "LogList[%d].SettingUser", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_setting_user" ) );
			sprintf( szDes, "LogList[%d].SettingInfo", i );
			this->response_.AddValue( szDes, rs.fieldValues( "f_setting_info" ) );
			rs.moveNext();
			i++;
		}
		
	} 
	else if( strcmp( szMethod, "RequestLogCount" ) == 0 )
	{

		std::string filter = "f_log_time >= '";
		filter.append( this->request_.GetValue( "StartTime" ) );
		filter.append( "' and f_log_time <= '" );
		filter.append( this->request_.GetValue( "EndTime" ) );
		filter.append( "'" );
		

		DK_PgSQLRecordset rs( &g_mgr.m_dbConn );
		if( rs.open( "t_request_log ", filter.c_str() ) != 0 )
		{
			this->response_.AddValue( ERROR_INFO_NAME, "打开数据库表出错！" );
			return -1;
		}
		char szCount[10];
		sprintf( szCount, "%ld", rs.getRecordCount() );
		this->response_.AddValue( "LogList.Count", szCount);
	
	}	
	else if( strcmp( szMethod, "VisitLogCount" ) == 0 )
	{

		std::string filter = "f_log_time >= '";
		filter.append( this->request_.GetValue( "StartTime" ) );
		filter.append( "' and f_log_time <= '" );
		filter.append( this->request_.GetValue( "EndTime" ) );
		filter.append( "'" );

		DK_PgSQLRecordset rs( &g_mgr.m_dbConn );
		if( rs.open( "t_visit_log ", filter.c_str() ) != 0 )
		{
			this->response_.AddValue( ERROR_INFO_NAME, "打开数据库表出错！" );
			return -1;
		}
		char szCount[10];
		sprintf( szCount, "%ld", rs.getRecordCount() );
		this->response_.AddValue( "LogList.Count", szCount);
	
	}
	else
		return CSoapServer::FindFnDoHandle( szMethod );
	
	if(sql.length() != 0)
	{
		ExecuteSQLCommand(sql.c_str());
	}
	return 0;
}


// =================================================================================
//  1.00    2006-11-18    Created By LinZaoGang
//  File End 
