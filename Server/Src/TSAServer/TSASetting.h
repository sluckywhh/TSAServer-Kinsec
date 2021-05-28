// =================================================================================
//  File    : TSASetting.h
//  Purpose : 应用程序配置文件操作类
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================

#ifndef __TSASETTING_H__
#define __TSASETTING_H__

// =================================================================================
//  Include Header Files
#include <stdio.h>
#include "XmlDocument.h"

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  define SETTING_FILE "SysSetting\\TSASetting.xml"
#elif defined(_LINUX)
#  define SETTING_FILE "SysSetting/TSASetting.xml"
#endif
#define SSF_SERVICE  0x00000001
#define SSF_CERT     0x00000002
#define SSF_TIME     0x00000004
#define SSF_EMAIL    0x00000008
#define SSF_TSA      0x00000010
#define SSF_ADMIN    0x00000020
#define SSF_SYSTEM   0x00000040
#define SSF_DATABASE 0x00000080
#define SSF_ALL      0xffffffff

// =================================================================================
//  Structure Definition
struct S_Admin
{
	std::string serialNumber;
	std::string commonName;
};
typedef std::vector<S_Admin> S_AdminList;

// =================================================================================
//  Class Definition
class CTSASetting : public CXmlDocument
{
public :
	// = Default constructor.
	CTSASetting();

	// = Get setting.
	int GetSetting();

	// = Set setting.
	int SetSetting( unsigned long ulMask );

public :
	// = Service port.
	short m_nServicePort;

	// = Parallel count.
	int m_nParallelCount;

	// = TSA certificate file.
	std::string m_strCertFile;

	// = TSA certificate pin.
	std::string m_strCertPin;

	// = Sync time interval.
	int m_nSyncTimeInterval;

	// = Time up.
	int m_nUpSecs;

	// = Using email.
	bool m_bUsingEmail;

	// = Email recv address.
	std::string m_strEmailRecvAddr;

	// = Email send address.
	std::string m_strEmailSendAddr;

	// = Email send user.
	std::string m_strEmailSendUser;

	// = Email send password.
	std::string m_strEmailSendPwd;

	// = Time sync type.
	std::string m_strSyncType;

	// = GPS Time source address.
	std::string m_strTimeSourceAddr1;
	std::string m_strTimeSourceAddr2;
	std::string m_strTimeSourceAddr3;

	// = Using database.
	bool m_bUsingDb;

	// = Database backup path.
	std::string m_strDbBackupPath;

	// = Database name.
	std::string m_strDbName;

	// = Database user.
	std::string m_strDbUser;

	// = Database password.
	std::string m_strDbPwd;

	// = Database server.
	std::string m_strDbServer;

	// = Database port.
	short m_nDbPort;

	// = TSA policy.
	std::string m_strTsaPolicy;

	// = TSA using ordering.
	bool m_bTsaUsingOrdering;

	// = TSA using accuracy.
	bool m_bTsaUsingAccuracy;

	// = TSA accuracy secsonds.
	int m_nTsaAccuracySecs;

	// = TSA accuracy millisecs.
	int m_nTsaAccuracyMillisecs;

	// = TSA accuracy microsecs.
	int m_nTsaAccuracyMicrosecs;

	// = Monitor time interval.
	int m_nMonitorTimeInterval;

	// = System log path.
	std::string m_strLogPath;

	// = Admin list.
	S_AdminList m_adminList;

	// = Trust cert file list.
	std::vector<std::string> m_trustCertList;
};

// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End 
#endif  // __TSASETTING_H__ 
