#pragma once

#include <IMemoryBlock.h>

inline IMemoryBlock* QueryMemoryBlock(VARIANT* unknown){
	if (unknown != NULL && unknown->vt == 0x8013 && unknown->punkVal != NULL){
		return (IMemoryBlock*)unknown->punkVal;
	}
	return NULL;
}

inline VARIANT& MakeMemoryBlockVariant(IMemoryBlock* block, VARIANT& var){
	::VariantInit(&var);
	var.vt = 0x8013;
	var.punkVal = (IUnknown*)(void*)block;
	return var;
}
