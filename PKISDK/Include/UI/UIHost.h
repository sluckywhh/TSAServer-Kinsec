/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Aug 09 16:40:11 2011
 */
/* Compiler settings for E:\Workfolder\KTProject\NSAA\trunk\UI\UIHost\UIHost.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __UIHost_h__
#define __UIHost_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IUILogin_FWD_DEFINED__
#define __IUILogin_FWD_DEFINED__
typedef interface IUILogin IUILogin;
#endif 	/* __IUILogin_FWD_DEFINED__ */


#ifndef __UILogin_FWD_DEFINED__
#define __UILogin_FWD_DEFINED__

#ifdef __cplusplus
typedef class UILogin UILogin;
#else
typedef struct UILogin UILogin;
#endif /* __cplusplus */

#endif 	/* __UILogin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IUILogin_INTERFACE_DEFINED__
#define __IUILogin_INTERFACE_DEFINED__

/* interface IUILogin */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IUILogin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6E15771B-0BA0-4B00-8F9C-BFACFF67B0C6")
    IUILogin : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetParameter( 
            /* [in] */ BSTR name,
            /* [in] */ VARIANT __RPC_FAR *value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryParameter( 
            /* [in] */ BSTR name,
            /* [retval][out] */ VARIANT __RPC_FAR *pRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [out] */ BSTR __RPC_FAR *guid,
            /* [retval][out] */ LONG __RPC_FAR *pRet) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUILoginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IUILogin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IUILogin __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IUILogin __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IUILogin __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IUILogin __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IUILogin __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IUILogin __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetParameter )( 
            IUILogin __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [in] */ VARIANT __RPC_FAR *value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryParameter )( 
            IUILogin __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ VARIANT __RPC_FAR *pRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Login )( 
            IUILogin __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *guid,
            /* [retval][out] */ LONG __RPC_FAR *pRet);
        
        END_INTERFACE
    } IUILoginVtbl;

    interface IUILogin
    {
        CONST_VTBL struct IUILoginVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUILogin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IUILogin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IUILogin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IUILogin_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IUILogin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IUILogin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IUILogin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IUILogin_SetParameter(This,name,value)	\
    (This)->lpVtbl -> SetParameter(This,name,value)

#define IUILogin_QueryParameter(This,name,pRet)	\
    (This)->lpVtbl -> QueryParameter(This,name,pRet)

#define IUILogin_Login(This,guid,pRet)	\
    (This)->lpVtbl -> Login(This,guid,pRet)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUILogin_SetParameter_Proxy( 
    IUILogin __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [in] */ VARIANT __RPC_FAR *value);


void __RPC_STUB IUILogin_SetParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUILogin_QueryParameter_Proxy( 
    IUILogin __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ VARIANT __RPC_FAR *pRet);


void __RPC_STUB IUILogin_QueryParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IUILogin_Login_Proxy( 
    IUILogin __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *guid,
    /* [retval][out] */ LONG __RPC_FAR *pRet);


void __RPC_STUB IUILogin_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IUILogin_INTERFACE_DEFINED__ */



#ifndef __UIHOSTLib_LIBRARY_DEFINED__
#define __UIHOSTLib_LIBRARY_DEFINED__

/* library UIHOSTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_UIHOSTLib;

EXTERN_C const CLSID CLSID_UILogin;

#ifdef __cplusplus

class DECLSPEC_UUID("A031A1AA-1C6A-4FED-93CE-2EB5BB93B554")
UILogin;
#endif
#endif /* __UIHOSTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
