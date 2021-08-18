#ifndef __PKISDK_51_H__
#define __PKISDK_51_H__

#ifdef _WIN32

#include <Devices/DevProvider.h>

KTINT32
KTPKI_GetDeviceFromSession(
		IN KT_SESSION_HANDLE hSession, OUT IDevProvider0** pProvider, OUT IDevice0** pDevice, BOOL& bPubSession);

#else

#include "Device0.h"
#include "DevProvider0.h"

KTINT32
KTPKI_GetDeviceFromSession(
		IN KT_SESSION_HANDLE hSession, OUT CDevProvider0** pProvider, OUT CDevice0** pDevice, KTBOOL32& bPubSession);

#endif

#endif // __PKISDK_51_H__
