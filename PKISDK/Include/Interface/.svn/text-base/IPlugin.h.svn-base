#ifndef __I_PLUGIN_H__
#define __I_PLUGIN_H__

/**
  * @Name: IPlugin.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 02:57
  * @Description: define class IPlugin
*/

#include "KTUnknow.h"

/**
* 插件信息描述
*/
struct PLUGIN_INFO{
	KTCHAR		szDesc[KT_MAX_PATH];
	KTUINT		nVersion;
	KTCHAR		szReserved[64];
};

class IPlugin : public IKTUnknow{
public:
	/**
	* 返回插件信息
	*/
	virtual void getPluginInfo(PLUGIN_INFO* pInfo)=0;
};

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 02:56  Hongwei Yu   created 

#endif
