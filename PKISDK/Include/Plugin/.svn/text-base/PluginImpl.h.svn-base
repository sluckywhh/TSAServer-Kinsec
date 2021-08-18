#ifndef _PLUGIN_IMPL_H
#define _PLUGIN_IMPL_H

#include <Plugin/PluginError.h>
#include <Plugin/PluginBase.h>

namespace Plugin{

//////////////////////////////////////////////////////////////////////////
///
/*
 *
 *
 */
#ifndef _W64
# define _W64
#endif


#ifndef LONG_PTR
# ifdef _WIN64
    typedef __int64         LONGPTR;
# else
    typedef _W64 int		LONGPTR;
# endif
#else
  typedef LONG_PTR			LONGPTR;
#endif


//////////////////////////////////////////////////////////////////////////
///
/*
 *
 */

typedef struct {
	const PLUGINIID *iid;
	LONGPTR offset;
	bool ned;
}IM_ENTRY;

#define OFFSET_PACKING				0x10000000
#define interfaceoffset(i,b)		(((LONGPTR)(static_cast<i*>(reinterpret_cast<b*>(OFFSET_PACKING))))-OFFSET_PACKING)

#ifdef START_INTERFACE_MAP
#undef START_INTERFACE_MAP
#endif

#ifdef INTERFACE_MAP_ENTRY
#undef INTERFACE_MAP_ENTRY
#endif

#ifdef END_INTERFACE_MAP
#undef END_INTERFACE_MAP
#endif

/* 使用宏开始一个接口映射表 */
#define START_INTERFACE_MAP			static const IM_ENTRY* _GetInterfaceMapEntries(){ \
										static const IM_ENTRY _InterfaceEntries[] = { 
/* 使用宏接口映射一个接口
   
   i - 要映射的接口
   
   b - 定现接口的类       */
#define INTERFACE_MAP_ENTRY(i,b)			{ &__uuidof(i), interfaceoffset(i,b), true },
/* <title START_INTERFACE_MAP>
   <toctitle START_INTERFACE_MAP>
   <keywords START_INTERFACE_MAP, Macro, 宏>
   
   用于结束以START_INTERFACE_MAP开始的接口定义宏 */
#define END_INTERFACE_MAP					{ &__uuidof(NULL_PLUGINUID), 0, false } \
										}; \
										return _InterfaceEntries; \
									}

//////////////////////////////////////////////////////////////////////////
///
/*
 * 多线程锁模板类
 *
 */
class MultithreadLock{
public:
	MultithreadLock():nRef(0){
		InitializeCriticalSection(&cs);
	}
	~MultithreadLock(){
		DeleteCriticalSection(&cs);
	}
public:
	long InternalIncreaseRef(){
		return ::InterlockedIncrement(&nRef);
	}
	long InternalDecreaseRef(){
		return ::InterlockedDecrement(&nRef);
	}
	void Lock(){
		EnterCriticalSection(&cs);
	}
	void Unlock(){
		LeaveCriticalSection(&cs);
	}
private:
	CRITICAL_SECTION cs;
	long nRef;
};

class SinglethreadLock{
public:
	SinglethreadLock():nRef(0){}
public:
	long InternalIncreaseRef(){
		return ++nRef;
	}
	long InternalDecreaseRef(){
		return --nRef;
	}
	void Lock(){
		return;
	}
	void Unlock(){
		return;
	}
private:
	long nRef;
};
//////////////////////////////////////////////////////////////////////////
///
/*
 * 简单实现IPlugableObject接口, 多线程版本
 *
 */
template<typename TBase, typename TLock = SinglethreadLock>
class SimplePluginObject : public TBase{
private:
	SimplePluginObject(){};
	~SimplePluginObject(){};
public:
	long PluginCall AddRef();
	long PluginCall Release();
	long PluginCall QueryInterface(const PLUGINIID iid, void** ppInterface);

public:
	/* \ \  */
	static long CreateInstance(SimplePluginObject<TBase,TLock> **ppObject, void* pParam);
private:
	TLock m_lock;
};

template<typename TBase, typename TLock>
inline long PluginCall SimplePluginObject<TBase,TLock>::AddRef(){
	return m_lock.InternalIncreaseRef();
}

template<typename TBase, typename TLock>
inline long PluginCall SimplePluginObject<TBase,TLock>::Release(){
	long nRef = 0;
	nRef = m_lock.InternalDecreaseRef();
	if ( nRef <= 0 ){
		this->FinalRelease();
		delete this;
	}
	return nRef;
}

template<typename TBase, typename TLock>
inline long PluginCall SimplePluginObject<TBase,TLock>::QueryInterface(const PLUGINIID iid, void** ppInterface){
	*ppInterface = NULL;
	m_lock.Lock();
	TBase *pBase = static_cast<TBase*>(this);
	/*
	 * 遍历所有的接口条目. 
	 * 如果请求的是IPlugableObject接口,那么直接返回第一个接口的IPlugableObject
	 * 
	 */
	for( const IM_ENTRY* pEntry = TBase::_GetInterfaceMapEntries(); pEntry != NULL && pEntry->ned; pEntry ++ ){
		if ( IsEqualGUID( iid, __uuidof(IPlugableObject) ) ){
			AddRef();
			*ppInterface = (void*)((LONGPTR)pBase+pEntry->offset);
			m_lock.Unlock();
			return PLUGIN_OK;
		}
		if ( IsEqualGUID(iid, *pEntry->iid) ){
			AddRef();
			*ppInterface = (void*)((LONGPTR)pBase+pEntry->offset);
			m_lock.Unlock();
			return PLUGIN_OK;
		}
	}
	m_lock.Unlock();
	return PLUGIN_NOT_IMPLEMENT;
}

template<typename TBase, typename TLock>
long SimplePluginObject<TBase,TLock>::CreateInstance(SimplePluginObject<TBase,TLock> **ppObject, void* pParam)
{
	SimplePluginObject<TBase,TLock> *pTemp = NULL;
	*ppObject = NULL;
	pTemp = new SimplePluginObject<TBase,TLock>();
	if ( pTemp == NULL ){
		return PLUGIN_NOT_ENOUGH_MEMORY;
	}
	long nRet = pTemp->FinalConstruct(pParam);
	if ( nRet != PLUGIN_OK ){
		delete pTemp;
		return nRet;
	}
	*ppObject = pTemp;
	return PLUGIN_OK;
}

} // namespace Plugin

#endif // ! _PLUGIN_IMPL_H
