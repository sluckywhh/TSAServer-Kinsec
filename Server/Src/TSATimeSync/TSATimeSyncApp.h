// =================================================================================
//  File    : TSATimeSyncApp.h
//  Purpose : 时间戳服务器时间同步服务.
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================

#ifndef __TSATIMESYNCAPP_H__
#define __TSATIMESYNCAPP_H__

#include <stdio.h>
#include <string>
#define SETTING_FILE "SysSetting/TSASetting.xml"


class CTSATimeSyncApp
{
public :
	// = Default constructor.
	CTSATimeSyncApp();

	// = Default destructor.
	virtual ~CTSATimeSyncApp();

	// = Start.
	int start();

	// = Sync time interval.
	int m_nInterval;

	// = Time up.
	int m_nUpSecs;

	// = Using email.
	bool m_bUsingEmail;
	
	bool m_bHaltWhenSyncErr;

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
};

// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End  
#endif  // __TSATIMESYNCAPP_H__
