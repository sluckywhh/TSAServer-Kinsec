#ifndef __PLUGINMGR__
#define __PLUGINMGR__

/**
  * @Name: PluginMgr.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 00:02
  * @Description:插件管理，包括插件的装载、释放及接口的获取
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

//定义插件的位置信息
struct PLUGINLIB_INFO 
{
public:
	PLUGINLIB_INFO(){szPath[0]=0;szFile[0]=0;}
	KTCHAR szPath[KT_MAX_PATH];			//插件路径
	KTCHAR szFile[KT_MAX_PATH];			//插件文件，可用通配符 如TKN*.KL
	KTCHAR szReserved[64];
};

class CPluginMangr 
{
public:
	/**
	* 装载插件
	*/
	virtual KTINT32 load(PLUGINLIB_INFO* pPluginLibInfo, KTIID nIID);
	/**
	* 释放插件
	*/
	void free();

	/**
	* 返回满足条件的插件个数
	*/
	int getCount();

	/**
	* 返回某一插件的属性
	*/
	void getInfo(int nIndex, PLUGIN_INFO* pInfo);

	/**
	* 获取插件指针
	*/
	KTINT32 getInterface(int nIndex,KTIID nIID,IKTUnknow** pUnknow);

	virtual ~CPluginMangr();

	CPluginMangr();

protected:
	struct PLUGINATTR 
	{
		PLUGINATTR(){nHandle=0;pPlugin=NULL;}
		KTHANDLE nHandle;
		IPlugin* pPlugin;
	};
	typedef std::vector<PLUGINATTR> PLUGINATTR_ARRAY;
	
    //根据要求选择插件
	virtual KTBOOL beforeLoad(IKTUnknow* pUnknow);
    //读取插件库信息
	virtual KTBOOL readPluginLibInfo(PLUGINLIB_INFO* pPluginLibInfo,KTString& strPath,KTString& strFile);
	PLUGINATTR_ARRAY m_rPluginAttr;
};

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 00:00  Hongwei Yu   created 

#endif 
