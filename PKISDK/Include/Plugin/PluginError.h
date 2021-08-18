#ifndef _PLUGIN_ERROR_H_
#define _PLUGIN_ERROR_H_

namespace Plugin {

/* sdaf */
typedef enum{
	PLUGIN_FALSE		= 0, /* 返回值不是错误 */
	
	PLUGIN_TRUE			= 1,
	PLUGIN_OK			= 0,
	PLUGIN_ERROR		= -1, /* 未指定错误 */
	
	PLUGIN_NOT_FOUND	= -2,
	PLUGIN_NOT_ALLOW	= -3,
	PLUGIN_NOT_ENOUGH_MEMORY	= -4,
	PLUGIN_OUT_OF_MEMORY		= -5,
	PLUGIN_OUT_OF_INDEX			= -6,
	PLUGIN_NOT_IMPLEMENT		= -7,
	PLUGIN_INVALID_PARAM		= -8,
}enumPluginError;

/* 检测返回的值是否正确 */
#define IsPluginError(x)		(x<0)

} // namespace Plugin


#endif // ! _PLUGIN_ERROR_H_
