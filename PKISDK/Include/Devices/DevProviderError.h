#pragma once


namespace DevProvider{

/**
 * 转换设备错误码到HRESULT.  
 * @param code 服务端返回的错误代码
 * @return 生成的HRESULT值
 * @see MakeCommnucationError
 */
inline HRESULT MakeDeviceError(int code)
{
	return MAKE_HRESULT(1, FACILITY_NULL, code);
}

/**
 * 转换通讯错误码到HRESULT.  
 * @return 生成的HRESULT值
 * @see MakeDeviceError
 */
inline HRESULT MakeCommnucationError()
{
	return MAKE_HRESULT(1, FACILITY_RPC, 2);
}

/**
 * 判断HRESULT值是否是通迅错误. 
 * @param result 相获取的HRESULT值 
 * @return 返回true表示是通迅错误
 * @see MakeCommnucationError
 */
inline bool IsCommnucationError(HRESULT result)
{
	return FAILED(result) && (result&FACILITY_RPC) && (result&2);
}

/**
 * 从HRESULT中获取设备错误代码. 
 * @param result 相获取的HRESULT值 
 * @return 设备错误代码
 * @see MakeDeviceError
 */
inline int GetDeviceErrorCode(HRESULT result)
{
	return FAILED(result) ? (result&0xFF) : 0; 
}
	
}; // namespace DevProvider