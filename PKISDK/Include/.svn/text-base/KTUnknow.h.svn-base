#if !defined(AFX_KTUnknow_H_INCLUDED_)
#define AFX_KTUnknow_H_INCLUDED_

/**
  *@Name: KTUnknow.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/03 16:16
  *@Description: unknow class define and realization 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./KTType.h"
#include "./KTMacro.h"
typedef KTUINT32 KTIID;
typedef KTUINT32 KTCID;
typedef KTINT32	 KTRESULT;
	
#define KT_STDMETHODCALLTYPE

#define KT_STDMETHOD(method) 	    virtual KTRESULT KT_STDMETHODCALLTYPE method
#define KT_STDMETHOD_NOVIR(method) 	KTRESULT KT_STDMETHODCALLTYPE method
#define KT_STDMETHODIMP		        KTRESULT KT_STDMETHODCALLTYPE
#define KT_PURE			            =0
#define KT_NO_VTABLE	            __declspec(novtable)

class IKTUnknow  
{
public:
	/**
	* Query interface.
	* @param nIDD:the id of interface.[IN]
	* @ppv:Address of output variable that receives the 
    *     interface pointer requested in iid.[OUT]
	*/
	KT_STDMETHOD(queryInterface)(KTIID nIID,void ** ppv)=0;

	/**
	* add interface Ref.
	*/
	virtual KTULONG KT_STDMETHODCALLTYPE addRef()=0;
	/**
	* Release interface.
	*/
	virtual KTULONG KT_STDMETHODCALLTYPE release()=0;
};

#define KT_DECLARE_INTERFACE()			\
	private:							\
		KTUINT32 m_nRef;				\
	public:								\
		KT_STDMETHOD(queryInterface)(KTIID nIID,void ** ppv);\
		virtual KTULONG KT_STDMETHODCALLTYPE addRef();		\
		virtual KTULONG KT_STDMETHODCALLTYPE release();		

#define KT_INIT_INTERFACE()			m_nRef=0

#define KT_BEGIN_INTERFACE(theClass)		\
	KTULONG KT_STDMETHODCALLTYPE theClass::addRef()				\
	{										\
		m_nRef++;							\
		return (KTULONG)m_nRef;				\
	}										\
	KTULONG KT_STDMETHODCALLTYPE theClass::release()				\
	{										\
		m_nRef--;							\
		if(0 == m_nRef)						\
		{									\
			delete this;				\
			return 0;					\
		}									\
		return (KTULONG)m_nRef;				\
	}										\
	KT_STDMETHODIMP theClass::queryInterface(KTIID nIID,void ** pv)	\
	{														\
		KTRESULT rv=KT_ERR_CRNT_NOSUPPORT;							\
		*pv=NULL;											\
		switch(nIID)										\
		{													

#define KT_ON_INTERFACE(IID,Interface)						\
		case IID:											\
			*pv=(Interface*)this;							\
			((Interface*)(*pv))->addRef();					\
			rv=KT_OK;										\
			break;							

#define KT_END_INTERFACE()		\
		default:				\
			*pv=NULL;			\
			break;				\
		}							\
		return rv;					\
	}



#define KT_BEGIN_INTERFACE_INLINE()		\
private:								\
	KTUINT32 m_nRef;					\
public:									\
	KTULONG KT_STDMETHODCALLTYPE addRef()	\
	{										\
		m_nRef++;							\
		return (KTULONG)m_nRef;				\
	}										\
	KTULONG KT_STDMETHODCALLTYPE release()	\
	{										\
		m_nRef--;							\
		if(0 == m_nRef){					\
			delete this;					\
			return 0;						\
		}									\
		return (KTULONG)m_nRef;				\
	}										\
	KT_STDMETHODIMP queryInterface(KTIID nIID,void ** pv)	\
	{														\
		KTRESULT rv=KT_ERR_CRNT_NOSUPPORT;						\
		*pv=NULL;											\
		switch(nIID)										\
		{													

#define KT_ON_INTERFACE_INLINE(IID,Interface)						\
	case IID:												\
		*pv=(Interface*)this;								\
		((Interface*)(*pv))->addRef();						\
		rv=KT_OK;											\
	break;							

#define KT_END_INTERFACE_INLINE()		\
	default:				\
		*pv=NULL;			\
		break;				\
	}							\
		return rv;					\
	}

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/03 16:18  Hongwei Yu   created 

#endif
