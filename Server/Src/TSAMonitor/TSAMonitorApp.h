// =================================================================================
//  File    : TSAMonitorApp.h
//  Purpose : 时间戳服务器监控服务服务.
//  Author  : 林灶钢
//  Created : 2006-12-30
//  CopyRight (C) 林灶钢
// =================================================================================

#ifndef __TSAMONITORAPP_H__
#define __TSAMONITORAPP_H__

// =================================================================================
//  Include Header Files
#include <string>

// =================================================================================
//  Macro Definition
#if defined(_WIN32)
#  define SETTING_FILE "SysSetting\\TSASetting.xml"
#elif defined(_LINUX)
#  define SETTING_FILE "SysSetting/TSASetting.xml"
#endif

// =================================================================================
//  Class Definition
class CTSAMonitorApp
{
public :
	// = Default constructor.
	CTSAMonitorApp();

	// = Default destructor.
	virtual ~CTSAMonitorApp();

	// = Start.
	int start();

	// = Write monitor log.
	void writeMonitorLog( const char * szInfo );

	// = Service port.
	short m_nServicePort;

	// = Monitor time interval.
	int m_nInterval;

	// = Log path.
	std::string m_strLogPath;
};

// =================================================================================
//  1.00    2006-12-30    Created By LinZaoGang
//  File End  
#endif  // __TSAMONITORAPP_H__
