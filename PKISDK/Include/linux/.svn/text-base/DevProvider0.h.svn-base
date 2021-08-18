// DevProvider0.h : Declaration of the CDevProvider0

#ifndef __DEVPROVIDER0_H_
#define __DEVPROVIDER0_H_

#include "Device0.h"

/////////////////////////////////////////////////////////////////////////////
// CDevProvider0
class CDevProvider0 
{
public:
	CDevProvider0();
	~CDevProvider0();

public:
	int Initialize(KTString ip, int port);
	int GetDeviceCount(int filters, int *count);
	int QueryGUIDS(KTStringArray& guids);
	int GetDevice(KTString id, CDevice0 **device);
	
	int AppendData(KTByteArray& data, int type, bool override, int *token);
	int RemoveByToken(int token);
	int UpdateByToken(int token, KTByteArray& data);
	int FindMoreByType(int type, int *tokens, int *count);
	int FindByData(KTByteArray& data, int *token);
	int FindByHash(KTByteArray& hash, int *token);
	int QuerySizeByToken(int token, int *size);
	int QueryTypeByToken(int token, int *type);
	int ReadData(int token, KTByteArray& data);

	int ReadSettingString(KTString path, KTString defaultValue, KTString& data);
	int ReadSettingInt(KTString path, int defaultValue, int *data);
	int ReadSettingBytes(KTString path, KTByteArray& data);
	int WriteSettingString(KTString path, KTString value);
	int WriteSettingInt(KTString path, int value);
	int WriteSettingBytes(KTString path, KTByteArray& value);
	int GetSettingItems(KTString path, int options, KTStringArray& entries);
};

#endif //__DEVPROVIDER0_H_
