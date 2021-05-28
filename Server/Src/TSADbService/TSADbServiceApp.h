// =================================================================================
//  File    : TSADbServiceApp.h
//  Purpose : 时间戳数据库服务.
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================

#ifndef __TSADBSERVICEAPP_H__
#define __TSADBSERVICEAPP_H__

// =================================================================================
//  Include Header Files
#include <string>
#include "DK_PgSQLConnection.h"

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  define SETTING_FILE "SysSetting\\TSASetting.xml"
#elif defined(_LINUX)
#  define SETTING_FILE "SysSetting/TSASetting.xml"
#endif

// =================================================================================
//  Class Definition
class CTSADbServiceApp
{
public :
	// = Default constructor.
	CTSADbServiceApp();

	// = Default destructor.
	virtual ~CTSADbServiceApp();

	// = Start.
	int start();

	// = Write service log.
	void writeServiceLog( const char * szInfo );

	// = Database connection.
	DK_PgSQLConnection m_dbConn;

private :
	// = Log path.
	std::string m_strLogPath;

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
};

// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End  
#endif  // __TSADBSERVICEAPP_H__
