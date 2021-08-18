#pragma once

#include <windef.h>
#include <WTypes.h>

#ifndef RUNTIME_EXPORT
# define RUNTIME_EXPORT extern "C" 
#endif

/////////////////////////////////////////////////////////////////
RUNTIME_EXPORT int __stdcall Runtime_Initialize();

/////////////////////////////////////////////////////////////////
// 组件环境
#define DLL_PKISDK		L"PKISDK"		// pkisdk10.dll


RUNTIME_EXPORT HMODULE __stdcall Runtime_Component_LoadLibrary(BSTR name, BSTR loader = NULL);


//////////////////////////////////////////////////////////////////
// 设备服务

RUNTIME_EXPORT bool __stdcall Runtime_DeviceService_IsRunning();
RUNTIME_EXPORT bool __stdcall Runtime_DeviceService_Register(BSTR address, long port);
RUNTIME_EXPORT long __stdcall Runtime_DeviceService_Port();


//////////////////////////////////////////////////////////////////////////
// 登录界面的运行时配置

/*
 * 获取刷新频率. 毫秒
 *
 */
RUNTIME_EXPORT long __stdcall Runtime_LoginUI_GetRefreshRate();

/*
 * 设置刷新频率. 毫秒
 *
 */
RUNTIME_EXPORT void __stdcall Runtime_LoginUI_SetRefreshRate(long rate);


