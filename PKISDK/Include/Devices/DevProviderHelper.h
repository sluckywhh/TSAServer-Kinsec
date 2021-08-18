#pragma once

#include <Runtime/Controller.h>
#include <Devices/DevProvider.h>

typedef HRESULT (__stdcall *DevProvider_GetObject)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

template<typename T, const GUID* clsid>
bool __stdcall DevProvider_CreateObject(T** device){
	HMODULE hMod = Runtime_Component_LoadLibrary(CComBSTR(_T("DevProvider")));
	if(hMod == NULL){
		return false;
	}
	DevProvider_GetObject fnGetObject = (DevProvider_GetObject)GetProcAddress(hMod, "NComGetObject");
	if(fnGetObject == NULL){
		return false;
	}
	if (SUCCEEDED(fnGetObject(*clsid, __uuidof(T), (void**)device))){
		return true;
	}
	return false;
}

