#ifndef _PLUGIN_UTIL_H_
#define _PLUGIN_UTIL_H_

namespace Plugin{

//////////////////////////////////////////////////////////////////////////
///
template<typename T>
class PluginPtr
{
public:
	PluginPtr():_p(0){}
	PluginPtr(const T* p):_p(0){
		operator=(p);
	}
	PluginPtr( const PluginPtr<T>& p):_p(0){
		operator=(p._p);
	}
	~PluginPtr(){
		if (_p){
			_p->Release();
			_p = NULL;
		}
	}

public:
	void CopyTo(T** pp){
		*pp = NULL;
		if (_p){
			_p->AddRef();
			*pp = _p;
		}
	}
	T** operator &(){
		return &_p;
	}
	bool operator !() const{
		return (_p != NULL);
	}
	operator T*() const{
		return _p;
	}
	T* operator=(const T* p){
		if (_p){
			_p->Release();
			_p = NULL;
		}
		if ( p ){
			_p = const_cast<T*>(p);
			_p->AddRef();
		}
		return _p;
	}
	T* operator=(const PluginPtr<T>& p){
		return operator=(p._p);
	}
	T* operator->(){
		return _p;
	}
private:
	T* _p;
};

} // namespace Plugin


#endif // £¡_PLUGIN_UTIL_H_
