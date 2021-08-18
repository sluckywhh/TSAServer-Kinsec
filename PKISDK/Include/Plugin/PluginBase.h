#ifndef _PLUGIN_BASE_H_
#define _PLUGIN_BASE_H_

#ifndef IN 
# define IN
#endif

#ifndef OUT
# define OUT
#endif

#ifndef INOUT
# define INOUT
#endif

#ifndef _T
# ifdef _UNICODE
#  define _T(x)			L##x
# else
#  define _T(x)			x
# endif
#endif

#define PluginCall		_stdcall

namespace Plugin {

//////////////////////////////////////////////////////////////////////////
///
#ifndef GUID_DEFINED
typedef struct {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;
#endif

typedef GUID			PLUGINUID;
typedef PLUGINUID		PLUGINIID;
typedef PLUGINUID		PLUGINOID;
typedef PLUGINUID		PLUGINCID;

#ifndef IsEqualGUID
# define IsEqualGUID(g1,g2)			(memcmp((void*)&g1,(void*)&g2,sizeof(GUID))==0)
#endif

#define IsEqualPluginUID	IsEqualGUID


/* 空的唯一标识符 */
class __declspec(uuid("00000000-0000-0000-0000-000000000000")) NULL_PLUGINUID; 

#if _MSC_VER > 0
#define DEFINE_CLASS_GUID(x,n) class __declspec(uuid(x)) n;
#elif defined(_LINUX)
#define DEFINE_CLASS_GUID(x,n) 
#else
/* 为类(class)或数据类型指定唯一标识符
   
   x - 输入一个字符串格式的唯一标识符, 如 "00000000-0000-0000-0000-000000000000"
   
   n - 输入类或类型,如 SampleClass
                                                                                 */
#define DEFINE_CLASS_GUID(x,n)
#endif


//#define PLUGINUID_NULL				GUID_NULL
//////////////////////////////////////////////////////////////////////////
///
typedef long (PluginCall *LPPluginGetObject)(/* asdfasdfasdfasdfasdfasdf */
IN const PLUGINOID oid, IN const PLUGINIID iid, INOUT void *pParam, OUT void** ppInterface);
typedef long (PluginCall *LPPluginCanUnloadNow)(IN bool bForceClean);

#define PLUGIN_GET_OBJECT		_T("PluginGetObject")
#define PLUGIN_CAN_UNLOAD_NOW	_T("PluginCanUnloadNow")

#define PLUGIN_GET_OBJECT_A			"PluginGetObject"
#define PLUGIN_CAN_UNLOAD_NOW_A		"PluginCanUnloadNow"

#define PLUGIN_GET_OBJECT_W			L"PluginGetObject"
#define PLUGIN_CAN_UNLOAD_NOW_W		L"PluginCanUnloadNow"

// Prototype

/* oid : 对象唯一标识
 * iid : 对象接口唯一标识 
 * pParam : 创建参数
 * ppInterface : 输出接口
 * return : 成功 IsPluginError(return) 为真
 */
/*
long PluginCall PluginGetObject(IN const PLUGINOID oid, 
	IN const PLUGINIID iid, INOUT void *pParam, OUT void** ppInterface);
*/

/*
 * bForceClean : 是否请求强制卸载
 * return : 返回PLUGIN_TRUE表明可以卸载，返回PLUGIN_FALSE表明不能够被卸载
 */
/*
long PluginCall PluginCanUnloadNow(IN bool bForceClean);
*/

//////////////////////////////////////////////////////////////////////////
///
class _declspec(uuid("1633611A-B06E-4416-A006-2FB69A71DBE5")) IPlugableObject;

class IPlugableObject{
public:
	/* 对象的引用计数加一 */
	virtual long PluginCall AddRef() = 0;
	/* 对象的引用计数减一 */
	virtual long PluginCall Release() = 0;
	
	/* Summary
	   调用此方法查询对象支持的指定接口.
	   Description
	   Add a description here...
	   Parameters
	   iid :          指定被查询接口的唯一标识符, PLUGINIID
	   ppInterface :  返回被查询到的接口指针
	   Returns
	   Add a return value description here...
	   Return Value List
	   PLUGIN_ERROR :  %PLUGIN_ERROR%
	   %PAR0% :        \Return value 2
	   Remarks
	   Add Remarks here...                                  */
	virtual long PluginCall QueryInterface(const PLUGINIID iid, void** ppInterface) = 0;
};


} // namespace Plugin

#endif // ! _PLUGIN_BASE_H_
