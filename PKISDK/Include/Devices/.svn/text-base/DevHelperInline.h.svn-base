#pragma once

#include "DevDefs.h"

inline FILEATTRIBUTE* QueryFileAttribute(VARIANT* unknown){
	if (unknown != NULL && unknown->vt == 0x8014 && unknown->punkVal != NULL){
		return (FILEATTRIBUTE*)unknown->punkVal;
	}
	return NULL;
}

inline VARIANT& MakeFileAttributeVariant(FILEATTRIBUTE* block, VARIANT& var){
	::VariantInit(&var);
	var.vt = 0x8014;
	var.punkVal = (IUnknown*)(void*)block;
	return var;
}

inline DEVICE_STATE* QueryDeviceState(VARIANT* unknown){
	if (unknown != NULL && unknown->vt == 0x8015 && unknown->punkVal != NULL){
		return (DEVICE_STATE*)unknown->punkVal;
	}
	return NULL;
}

inline VARIANT& MakeDeviceStateVariant(DEVICE_STATE* block, VARIANT& var){
	::VariantInit(&var);
	var.vt = 0x8015;
	var.punkVal = (IUnknown*)(void*)block;
	return var;
}

inline DEVICE_INFO* QueryDeviceInfo(VARIANT* unknown){
	if (unknown != NULL && unknown->vt == 0x8016 && unknown->punkVal != NULL){
		return (DEVICE_INFO*)unknown->punkVal;
	}
	return NULL;
}

inline VARIANT& MakeDeviceInfoVariant(DEVICE_INFO* block, VARIANT& var){
	::VariantInit(&var);
	var.vt = 0x8016;
	var.punkVal = (IUnknown*)(void*)block;
	return var;
}