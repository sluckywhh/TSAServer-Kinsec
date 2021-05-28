// =================================================================================
//  File    : TSASoapClient.h
//  Purpose : 时间戳服务器管理客户端接口
//  Author  : 林灶钢
//  Created : 2006-11-25
//  CopyRight (C) 林灶钢
// =================================================================================

#ifndef __TSASOAPCLIENT_H__
#define __TSASOAPCLIENT_H__

#pragma warning( disable : 4786 )

// =================================================================================
//  Include Header Files
#include "SoapClient.h"
#include <string>
#include <vector>

// =================================================================================
//  Macro Definition
#define SSF_SERVICE  0x00000001
#define SSF_CERT     0x00000002
#define SSF_TIME     0x00000004
#define SSF_ALL      0xffffffff

// =================================================================================
//  Structure Definition
struct S_Admin
{
	std::string serialNumber;
	std::string commonName;
};
typedef std::vector<S_Admin> S_AdminList;

struct S_Setting
{
	std::string servicePort;
	std::string serviceBacklog;
	std::string timeSourceAddr;
	std::string timeSyncType;
	std::string timeInterval;
	std::string upSecs;
	std::string useEmail;
	std::string warningRecvAddr;
	std::string warningSendAddr;
	std::string warningSendUser;
	std::string warningSendPwd;
};

struct S_TimeStamp
{
	std::string entryTime;
	std::string serialNumber;
	std::string fullData;
};
typedef std::vector<S_TimeStamp> S_TimeStampList;

struct S_VisitLog
{
	std::string logTime;
	std::string clientIp;
	std::string operater;
};
typedef std::vector<S_VisitLog> S_VisitLogList;

struct S_RequestLog
{
	std::string logTime;
	std::string requestData;
	std::string responseData;
	std::string status;
};
typedef std::vector<S_RequestLog> S_RequestLogList;

struct S_SettingLog
{
	std::string logTime;
	std::string settingUser;
	std::string settingInfo;
};
typedef std::vector<S_SettingLog> S_SettingLogList;

// =================================================================================
//  Class Definition
class CTSASoapClient : public CSoapClient
{
public :
	// = Login tsa server.
	int LoginTSAServer();

	// = Get admin list.
	int GetAdminList( S_AdminList & adminList );

	// = Add admin.
	int AddAdmin( const S_Admin & admin );

	// = Remove admin.
	int RemoveAdmin( const char * szSerailNumber );

	// = Get setting.
	int GetSetting( unsigned long ulMask, S_Setting & setting );

	// = Set setting.
	int SetSetting( const S_Setting & setting, unsigned long ulMask );

	// = Restart service.
	int RestartService();

	// = Restart system.
	int RestartSystem();

	// = Backup.
	int DBBackup();

	// = Get backup path.
	int GetBackupPath( std::string & backupPath );

	// = Get backup list.
	int GetBackupList( std::vector<std::string> & backupList );

	// = Query time stamp.
	int QueryTimeStamp( const char * szStartTime, const char * szEndTime, const char * szSN, 
		const char * szData, S_TimeStampList & timeStampList );

	// = Query visit log.
	int QueryVisitLog( const char * szStartTime, const char * szEndTime, S_VisitLogList & logList );

	// = Query request log.
	int QueryRequestLog( const char * szStartTime, const char * szEndTime, S_RequestLogList & logList );

	// = Query setting log.
	int QuerySettingLog( const char * szStartTime, const char * szEndTime, S_SettingLogList & logList );
};

// =================================================================================
//  1.00    2006-11-25    Created By LinZaoGang
//  File End 
#endif  // __TSASOAPCLIENT_H__
