// =================================================================================
//  File    : TSAManager.h
//  Purpose : 时间戳服务器服务类
//  Author  : 林灶钢
//  Created : 2006-11-26
//  CopyRight (C) Fujian Kinsec
// =================================================================================

#ifndef __TSAMANAGER_H__
#define __TSAMANAGER_H__

// =================================================================================
//  Include Header Files
#include "TSASetting.h"
//#include "DK_PgSQLRecordset.h"

// =================================================================================
//  Macro Definition

// =================================================================================
//  Class Definition
class CTSAManager
{
public : 
	// = Start.
	int start();

	// = Server setting.
	CTSASetting m_setting;

	// = Database connection.
	//DK_PgSQLConnection m_dbConn;
};
extern CTSAManager g_mgr;

// =================================================================================
//  1.00    2006-11-26    Created By LinZaoGang
//  File End 
#endif  // __TSAMANAGER_H__  
