#pragma once

#include <Runtime/Controller.h>
#include <Devices/DevProvider.h>

typedef HRESULT (__stdcall *FN_NComGetObject)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

template<typename T, const GUID* clsid>
bool __stdcall DLLNComGetObject(T** obj, BSTR dllname, BOOL loadany = TRUE){
	USES_CONVERSION;
	HMODULE hMod = Runtime_Component_LoadLibrary(dllname);
	if(hMod == NULL){
		if (!loadany){
			return false;
		}
		hMod = ::LoadLibraryW(dllname);
		if (hMod == NULL){
			return false;
		}
	}
	FN_NComGetObject fnGetObject = (FN_NComGetObject)GetProcAddress(hMod, "NComGetObject");
	if(fnGetObject == NULL){
		return false;
	}
	if (SUCCEEDED(fnGetObject(*clsid, __uuidof(T), (void**)obj))){
		return true;
	}
	return false;
}

