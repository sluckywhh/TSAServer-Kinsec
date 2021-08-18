

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu May 26 09:27:04 2016
 */
/* Compiler settings for DevProvider.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __DevProvider_h__
#define __DevProvider_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDevice0_FWD_DEFINED__
#define __IDevice0_FWD_DEFINED__
typedef interface IDevice0 IDevice0;
#endif 	/* __IDevice0_FWD_DEFINED__ */


#ifndef __IGeneralStorage_FWD_DEFINED__
#define __IGeneralStorage_FWD_DEFINED__
typedef interface IGeneralStorage IGeneralStorage;
#endif 	/* __IGeneralStorage_FWD_DEFINED__ */


#ifndef __IGeneralSettings_FWD_DEFINED__
#define __IGeneralSettings_FWD_DEFINED__
typedef interface IGeneralSettings IGeneralSettings;
#endif 	/* __IGeneralSettings_FWD_DEFINED__ */


#ifndef __IDevProvider0_FWD_DEFINED__
#define __IDevProvider0_FWD_DEFINED__
typedef interface IDevProvider0 IDevProvider0;
#endif 	/* __IDevProvider0_FWD_DEFINED__ */


#ifndef __DevProvider0_FWD_DEFINED__
#define __DevProvider0_FWD_DEFINED__

#ifdef __cplusplus
typedef class DevProvider0 DevProvider0;
#else
typedef struct DevProvider0 DevProvider0;
#endif /* __cplusplus */

#endif 	/* __DevProvider0_FWD_DEFINED__ */


#ifndef __Device0_FWD_DEFINED__
#define __Device0_FWD_DEFINED__

#ifdef __cplusplus
typedef class Device0 Device0;
#else
typedef struct Device0 Device0;
#endif /* __cplusplus */

#endif 	/* __Device0_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DevProvider_0000_0000 */
/* [local] */ 

typedef 
enum DeviceFilter
    {	FILTER_LOGIN	= 0x1,
	FILTER_ONLINE	= 0x2,
	FILTER_SOFTWARE	= 0x4
    } 	enumDeviceFilter;

typedef 
enum SettingOptions
    {	SEARCH_ALL_ITEM	= 0,
	SEARCH_CHILD_ITEM	= ( SEARCH_ALL_ITEM + 1 ) 
    } 	enumSettingOptions;



extern RPC_IF_HANDLE __MIDL_itf_DevProvider_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DevProvider_0000_0000_v0_0_s_ifspec;

#ifndef __IDevice0_INTERFACE_DEFINED__
#define __IDevice0_INTERFACE_DEFINED__

/* interface IDevice0 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDevice0;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E1A67CD8-71BC-46FE-B00C-EE3BF5C0CBAB")
    IDevice0 : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetCertNO( 
            /* [out] */ LONG *pCertNO,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadCert( 
            /* [in] */ LONG CertNO,
            /* [out] */ VARIANT *pCert,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteCert( 
            /* [in] */ LONG CertNO,
            /* [in] */ VARIANT *pCert,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GenRSAKey( 
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *pubKey,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPubKey( 
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *pubKey,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PrivateKeyEncrypt( 
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *plain,
            /* [out] */ VARIANT *cipher,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Signature( 
            /* [in] */ VARIANT *plain,
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *digest,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE VerifyDigest( 
            /* [in] */ VARIANT *digest,
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *plain,
            /* [in] */ VARIANT *cert,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RSAEncrypt( 
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *plain,
            /* [out] */ VARIANT *cipher,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PrivateKeyDecrypt( 
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *cipher,
            /* [out] */ VARIANT *data,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateFile( 
            /* [in] */ BSTR filename,
            /* [in] */ LONG filesize,
            /* [in] */ LONG readrights,
            /* [in] */ LONG writerights,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DeleteFile( 
            /* [in] */ BSTR filename,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EnumFiles( 
            /* [out] */ BSTR *filelist,
            /* [out] */ LONG *count,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetFileInfo( 
            /* [in] */ BSTR filename,
            /* [out] */ VARIANT *attribute,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadFile( 
            /* [in] */ BSTR filename,
            /* [in] */ LONG offset,
            /* [in] */ LONG size,
            /* [out] */ VARIANT *data,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteFile( 
            /* [in] */ BSTR filename,
            /* [in] */ LONG offset,
            /* [in] */ VARIANT *writedata,
            /* [in] */ LONG size,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Encrypt( 
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *SymmKey,
            /* [in] */ VARIANT *plain,
            /* [out] */ VARIANT *cipher,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Decrypt( 
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *SymmKey,
            /* [in] */ VARIANT *cipher,
            /* [out] */ VARIANT *plain,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryState( 
            /* [out] */ VARIANT *state,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [in] */ BSTR user,
            LONG *rcount,
            /* [in] */ LONG keep,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangePin( 
            BSTR oldPin,
            BSTR newPin,
            LONG *rcount,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDeviceInfo( 
            /* [out] */ VARIANT *info,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitKey( 
            /* [in] */ BSTR UserPin,
            /* [in] */ LONG MaxRetryCount1,
            /* [in] */ BSTR AdminPin,
            /* [in] */ LONG MaxRetryCount2,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VerifyUserPin( 
            /* [in] */ BSTR UserPin,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VerifyAdminPin( 
            /* [in] */ BSTR AdminPin,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeAdminPin( 
            /* [in] */ BSTR oldPin,
            /* [in] */ BSTR newPin,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnlockUserPin( 
            /* [in] */ BSTR AdminPin,
            /* [in] */ BSTR UserPin,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetContainerName( 
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *ContainerName,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetupIECert( 
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Digest( 
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toDigest,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GroupDigest( 
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toDigest,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Mac( 
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toMac,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GroupMac( 
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toMac,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Logout( 
            /* [out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCGetCertNo( 
            LONG *pCertNo,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCReadCert( 
            LONG CertNO,
            VARIANT *pCert,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCWriteCert( 
            LONG CertNO,
            VARIANT *pCert,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCGetContainerName( 
            LONG pair,
            VARIANT *ContainerName,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCSetupIECert( 
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCGenKey( 
            LONG pair,
            VARIANT *ECCKey,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCGetPubKey( 
            LONG pair,
            VARIANT *pubKey,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCSign( 
            LONG pair,
            VARIANT *plain,
            VARIANT *signeddata,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtECCVerify( 
            VARIANT *signeddata,
            VARIANT *eccPubKey,
            VARIANT *plain,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCSignDigest( 
            LONG pair,
            LONG AlgID,
            VARIANT *UserID,
            VARIANT *plain,
            VARIANT *result,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtECCVerifyDigest( 
            LONG AlgID,
            VARIANT *plain,
            VARIANT *cipher,
            VARIANT *eccPubKey,
            VARIANT *UserID,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtECCEncrypt( 
            VARIANT *eccPubKey,
            VARIANT *plain,
            VARIANT *cipher,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCDecrypt( 
            LONG pair,
            VARIANT *cipher,
            VARIANT *decipher,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCGetTempPubKey( 
            LONG pair,
            VARIANT *TempPubKey,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ECCKeyExchange( 
            LONG pair,
            LONG AlgID,
            LONG Role,
            LONG *phKey,
            VARIANT *ID,
            VARIANT *OID,
            VARIANT *TempECCPubKey,
            VARIANT *ECCPubKey,
            VARIANT *OTempPubKey,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRandom( 
            LONG dataLen,
            VARIANT *randomData,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DigestInit( 
            int AlgID,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DigestUpdate( 
            VARIANT *toDigest,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DigestFinal( 
            VARIANT *resultFinal,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncryptInit( 
            VARIANT *IV,
            LONG PaddingType,
            LONG FeedBitLen,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncryptUpdate( 
            VARIANT *plain,
            VARIANT *cipher,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncryptFinal( 
            VARIANT *plain,
            VARIANT *cipher,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecryptInit( 
            VARIANT *IV,
            LONG PaddingType,
            LONG FeedBitLen,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecryptUpdate( 
            VARIANT *cipher,
            VARIANT *plain,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecryptFinal( 
            VARIANT *cipher,
            VARIANT *plain,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportSessionKey( 
            LONG AlgID,
            VARIANT *WrapedData,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSymmKey( 
            LONG AlgID,
            VARIANT *SymmKey,
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseHandle( 
            LONG *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDLLType( 
            LONG *pDLLType,
            LONG *retval) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDevice0Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDevice0 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDevice0 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDevice0 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDevice0 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDevice0 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDevice0 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDevice0 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCertNO )( 
            IDevice0 * This,
            /* [out] */ LONG *pCertNO,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadCert )( 
            IDevice0 * This,
            /* [in] */ LONG CertNO,
            /* [out] */ VARIANT *pCert,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteCert )( 
            IDevice0 * This,
            /* [in] */ LONG CertNO,
            /* [in] */ VARIANT *pCert,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GenRSAKey )( 
            IDevice0 * This,
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *pubKey,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPubKey )( 
            IDevice0 * This,
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *pubKey,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PrivateKeyEncrypt )( 
            IDevice0 * This,
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *plain,
            /* [out] */ VARIANT *cipher,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Signature )( 
            IDevice0 * This,
            /* [in] */ VARIANT *plain,
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *digest,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *VerifyDigest )( 
            IDevice0 * This,
            /* [in] */ VARIANT *digest,
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *plain,
            /* [in] */ VARIANT *cert,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *RSAEncrypt )( 
            IDevice0 * This,
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *plain,
            /* [out] */ VARIANT *cipher,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PrivateKeyDecrypt )( 
            IDevice0 * This,
            /* [in] */ LONG pair,
            /* [in] */ VARIANT *cipher,
            /* [out] */ VARIANT *data,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CreateFile )( 
            IDevice0 * This,
            /* [in] */ BSTR filename,
            /* [in] */ LONG filesize,
            /* [in] */ LONG readrights,
            /* [in] */ LONG writerights,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DeleteFile )( 
            IDevice0 * This,
            /* [in] */ BSTR filename,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EnumFiles )( 
            IDevice0 * This,
            /* [out] */ BSTR *filelist,
            /* [out] */ LONG *count,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFileInfo )( 
            IDevice0 * This,
            /* [in] */ BSTR filename,
            /* [out] */ VARIANT *attribute,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReadFile )( 
            IDevice0 * This,
            /* [in] */ BSTR filename,
            /* [in] */ LONG offset,
            /* [in] */ LONG size,
            /* [out] */ VARIANT *data,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *WriteFile )( 
            IDevice0 * This,
            /* [in] */ BSTR filename,
            /* [in] */ LONG offset,
            /* [in] */ VARIANT *writedata,
            /* [in] */ LONG size,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Encrypt )( 
            IDevice0 * This,
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *SymmKey,
            /* [in] */ VARIANT *plain,
            /* [out] */ VARIANT *cipher,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Decrypt )( 
            IDevice0 * This,
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *SymmKey,
            /* [in] */ VARIANT *cipher,
            /* [out] */ VARIANT *plain,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryState )( 
            IDevice0 * This,
            /* [out] */ VARIANT *state,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Login )( 
            IDevice0 * This,
            /* [in] */ BSTR user,
            LONG *rcount,
            /* [in] */ LONG keep,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangePin )( 
            IDevice0 * This,
            BSTR oldPin,
            BSTR newPin,
            LONG *rcount,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDeviceInfo )( 
            IDevice0 * This,
            /* [out] */ VARIANT *info,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitKey )( 
            IDevice0 * This,
            /* [in] */ BSTR UserPin,
            /* [in] */ LONG MaxRetryCount1,
            /* [in] */ BSTR AdminPin,
            /* [in] */ LONG MaxRetryCount2,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VerifyUserPin )( 
            IDevice0 * This,
            /* [in] */ BSTR UserPin,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VerifyAdminPin )( 
            IDevice0 * This,
            /* [in] */ BSTR AdminPin,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeAdminPin )( 
            IDevice0 * This,
            /* [in] */ BSTR oldPin,
            /* [in] */ BSTR newPin,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnlockUserPin )( 
            IDevice0 * This,
            /* [in] */ BSTR AdminPin,
            /* [in] */ BSTR UserPin,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetContainerName )( 
            IDevice0 * This,
            /* [in] */ LONG pair,
            /* [out] */ VARIANT *ContainerName,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetupIECert )( 
            IDevice0 * This,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Digest )( 
            IDevice0 * This,
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toDigest,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GroupDigest )( 
            IDevice0 * This,
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toDigest,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Mac )( 
            IDevice0 * This,
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toMac,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GroupMac )( 
            IDevice0 * This,
            /* [in] */ LONG AlgID,
            /* [in] */ VARIANT *toMac,
            /* [out] */ VARIANT *result,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Logout )( 
            IDevice0 * This,
            /* [out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCGetCertNo )( 
            IDevice0 * This,
            LONG *pCertNo,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCReadCert )( 
            IDevice0 * This,
            LONG CertNO,
            VARIANT *pCert,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCWriteCert )( 
            IDevice0 * This,
            LONG CertNO,
            VARIANT *pCert,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCGetContainerName )( 
            IDevice0 * This,
            LONG pair,
            VARIANT *ContainerName,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCSetupIECert )( 
            IDevice0 * This,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCGenKey )( 
            IDevice0 * This,
            LONG pair,
            VARIANT *ECCKey,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCGetPubKey )( 
            IDevice0 * This,
            LONG pair,
            VARIANT *pubKey,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCSign )( 
            IDevice0 * This,
            LONG pair,
            VARIANT *plain,
            VARIANT *signeddata,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExtECCVerify )( 
            IDevice0 * This,
            VARIANT *signeddata,
            VARIANT *eccPubKey,
            VARIANT *plain,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCSignDigest )( 
            IDevice0 * This,
            LONG pair,
            LONG AlgID,
            VARIANT *UserID,
            VARIANT *plain,
            VARIANT *result,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExtECCVerifyDigest )( 
            IDevice0 * This,
            LONG AlgID,
            VARIANT *plain,
            VARIANT *cipher,
            VARIANT *eccPubKey,
            VARIANT *UserID,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExtECCEncrypt )( 
            IDevice0 * This,
            VARIANT *eccPubKey,
            VARIANT *plain,
            VARIANT *cipher,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCDecrypt )( 
            IDevice0 * This,
            LONG pair,
            VARIANT *cipher,
            VARIANT *decipher,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCGetTempPubKey )( 
            IDevice0 * This,
            LONG pair,
            VARIANT *TempPubKey,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ECCKeyExchange )( 
            IDevice0 * This,
            LONG pair,
            LONG AlgID,
            LONG Role,
            LONG *phKey,
            VARIANT *ID,
            VARIANT *OID,
            VARIANT *TempECCPubKey,
            VARIANT *ECCPubKey,
            VARIANT *OTempPubKey,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRandom )( 
            IDevice0 * This,
            LONG dataLen,
            VARIANT *randomData,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DigestInit )( 
            IDevice0 * This,
            int AlgID,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DigestUpdate )( 
            IDevice0 * This,
            VARIANT *toDigest,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DigestFinal )( 
            IDevice0 * This,
            VARIANT *resultFinal,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncryptInit )( 
            IDevice0 * This,
            VARIANT *IV,
            LONG PaddingType,
            LONG FeedBitLen,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncryptUpdate )( 
            IDevice0 * This,
            VARIANT *plain,
            VARIANT *cipher,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncryptFinal )( 
            IDevice0 * This,
            VARIANT *plain,
            VARIANT *cipher,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecryptInit )( 
            IDevice0 * This,
            VARIANT *IV,
            LONG PaddingType,
            LONG FeedBitLen,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecryptUpdate )( 
            IDevice0 * This,
            VARIANT *cipher,
            VARIANT *plain,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecryptFinal )( 
            IDevice0 * This,
            VARIANT *cipher,
            VARIANT *plain,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportSessionKey )( 
            IDevice0 * This,
            LONG AlgID,
            VARIANT *WrapedData,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSymmKey )( 
            IDevice0 * This,
            LONG AlgID,
            VARIANT *SymmKey,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseHandle )( 
            IDevice0 * This,
            LONG *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDLLType )( 
            IDevice0 * This,
            LONG *pDLLType,
            LONG *retval);
        
        END_INTERFACE
    } IDevice0Vtbl;

    interface IDevice0
    {
        CONST_VTBL struct IDevice0Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDevice0_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDevice0_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDevice0_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDevice0_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDevice0_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDevice0_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDevice0_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDevice0_GetCertNO(This,pCertNO,retval)	\
    ( (This)->lpVtbl -> GetCertNO(This,pCertNO,retval) ) 

#define IDevice0_ReadCert(This,CertNO,pCert,retval)	\
    ( (This)->lpVtbl -> ReadCert(This,CertNO,pCert,retval) ) 

#define IDevice0_WriteCert(This,CertNO,pCert,retval)	\
    ( (This)->lpVtbl -> WriteCert(This,CertNO,pCert,retval) ) 

#define IDevice0_GenRSAKey(This,pair,pubKey,retval)	\
    ( (This)->lpVtbl -> GenRSAKey(This,pair,pubKey,retval) ) 

#define IDevice0_GetPubKey(This,pair,pubKey,retval)	\
    ( (This)->lpVtbl -> GetPubKey(This,pair,pubKey,retval) ) 

#define IDevice0_PrivateKeyEncrypt(This,pair,plain,cipher,retval)	\
    ( (This)->lpVtbl -> PrivateKeyEncrypt(This,pair,plain,cipher,retval) ) 

#define IDevice0_Signature(This,plain,pair,digest,retval)	\
    ( (This)->lpVtbl -> Signature(This,plain,pair,digest,retval) ) 

#define IDevice0_VerifyDigest(This,digest,pair,plain,cert,retval)	\
    ( (This)->lpVtbl -> VerifyDigest(This,digest,pair,plain,cert,retval) ) 

#define IDevice0_RSAEncrypt(This,pair,plain,cipher,retval)	\
    ( (This)->lpVtbl -> RSAEncrypt(This,pair,plain,cipher,retval) ) 

#define IDevice0_PrivateKeyDecrypt(This,pair,cipher,data,retval)	\
    ( (This)->lpVtbl -> PrivateKeyDecrypt(This,pair,cipher,data,retval) ) 

#define IDevice0_CreateFile(This,filename,filesize,readrights,writerights,retval)	\
    ( (This)->lpVtbl -> CreateFile(This,filename,filesize,readrights,writerights,retval) ) 

#define IDevice0_DeleteFile(This,filename,retval)	\
    ( (This)->lpVtbl -> DeleteFile(This,filename,retval) ) 

#define IDevice0_EnumFiles(This,filelist,count,retval)	\
    ( (This)->lpVtbl -> EnumFiles(This,filelist,count,retval) ) 

#define IDevice0_GetFileInfo(This,filename,attribute,retval)	\
    ( (This)->lpVtbl -> GetFileInfo(This,filename,attribute,retval) ) 

#define IDevice0_ReadFile(This,filename,offset,size,data,retval)	\
    ( (This)->lpVtbl -> ReadFile(This,filename,offset,size,data,retval) ) 

#define IDevice0_WriteFile(This,filename,offset,writedata,size,retval)	\
    ( (This)->lpVtbl -> WriteFile(This,filename,offset,writedata,size,retval) ) 

#define IDevice0_Encrypt(This,AlgID,SymmKey,plain,cipher,retval)	\
    ( (This)->lpVtbl -> Encrypt(This,AlgID,SymmKey,plain,cipher,retval) ) 

#define IDevice0_Decrypt(This,AlgID,SymmKey,cipher,plain,retval)	\
    ( (This)->lpVtbl -> Decrypt(This,AlgID,SymmKey,cipher,plain,retval) ) 

#define IDevice0_QueryState(This,state,retval)	\
    ( (This)->lpVtbl -> QueryState(This,state,retval) ) 

#define IDevice0_Login(This,user,rcount,keep,retval)	\
    ( (This)->lpVtbl -> Login(This,user,rcount,keep,retval) ) 

#define IDevice0_ChangePin(This,oldPin,newPin,rcount,retval)	\
    ( (This)->lpVtbl -> ChangePin(This,oldPin,newPin,rcount,retval) ) 

#define IDevice0_GetDeviceInfo(This,info,retval)	\
    ( (This)->lpVtbl -> GetDeviceInfo(This,info,retval) ) 

#define IDevice0_InitKey(This,UserPin,MaxRetryCount1,AdminPin,MaxRetryCount2,retval)	\
    ( (This)->lpVtbl -> InitKey(This,UserPin,MaxRetryCount1,AdminPin,MaxRetryCount2,retval) ) 

#define IDevice0_VerifyUserPin(This,UserPin,retval)	\
    ( (This)->lpVtbl -> VerifyUserPin(This,UserPin,retval) ) 

#define IDevice0_VerifyAdminPin(This,AdminPin,retval)	\
    ( (This)->lpVtbl -> VerifyAdminPin(This,AdminPin,retval) ) 

#define IDevice0_ChangeAdminPin(This,oldPin,newPin,retval)	\
    ( (This)->lpVtbl -> ChangeAdminPin(This,oldPin,newPin,retval) ) 

#define IDevice0_UnlockUserPin(This,AdminPin,UserPin,retval)	\
    ( (This)->lpVtbl -> UnlockUserPin(This,AdminPin,UserPin,retval) ) 

#define IDevice0_GetContainerName(This,pair,ContainerName,retval)	\
    ( (This)->lpVtbl -> GetContainerName(This,pair,ContainerName,retval) ) 

#define IDevice0_SetupIECert(This,retval)	\
    ( (This)->lpVtbl -> SetupIECert(This,retval) ) 

#define IDevice0_Digest(This,AlgID,toDigest,result,retval)	\
    ( (This)->lpVtbl -> Digest(This,AlgID,toDigest,result,retval) ) 

#define IDevice0_GroupDigest(This,AlgID,toDigest,result,retval)	\
    ( (This)->lpVtbl -> GroupDigest(This,AlgID,toDigest,result,retval) ) 

#define IDevice0_Mac(This,AlgID,toMac,result,retval)	\
    ( (This)->lpVtbl -> Mac(This,AlgID,toMac,result,retval) ) 

#define IDevice0_GroupMac(This,AlgID,toMac,result,retval)	\
    ( (This)->lpVtbl -> GroupMac(This,AlgID,toMac,result,retval) ) 

#define IDevice0_Logout(This,retval)	\
    ( (This)->lpVtbl -> Logout(This,retval) ) 

#define IDevice0_ECCGetCertNo(This,pCertNo,retval)	\
    ( (This)->lpVtbl -> ECCGetCertNo(This,pCertNo,retval) ) 

#define IDevice0_ECCReadCert(This,CertNO,pCert,retval)	\
    ( (This)->lpVtbl -> ECCReadCert(This,CertNO,pCert,retval) ) 

#define IDevice0_ECCWriteCert(This,CertNO,pCert,retval)	\
    ( (This)->lpVtbl -> ECCWriteCert(This,CertNO,pCert,retval) ) 

#define IDevice0_ECCGetContainerName(This,pair,ContainerName,retval)	\
    ( (This)->lpVtbl -> ECCGetContainerName(This,pair,ContainerName,retval) ) 

#define IDevice0_ECCSetupIECert(This,retval)	\
    ( (This)->lpVtbl -> ECCSetupIECert(This,retval) ) 

#define IDevice0_ECCGenKey(This,pair,ECCKey,retval)	\
    ( (This)->lpVtbl -> ECCGenKey(This,pair,ECCKey,retval) ) 

#define IDevice0_ECCGetPubKey(This,pair,pubKey,retval)	\
    ( (This)->lpVtbl -> ECCGetPubKey(This,pair,pubKey,retval) ) 

#define IDevice0_ECCSign(This,pair,plain,signeddata,retval)	\
    ( (This)->lpVtbl -> ECCSign(This,pair,plain,signeddata,retval) ) 

#define IDevice0_ExtECCVerify(This,signeddata,eccPubKey,plain,retval)	\
    ( (This)->lpVtbl -> ExtECCVerify(This,signeddata,eccPubKey,plain,retval) ) 

#define IDevice0_ECCSignDigest(This,pair,AlgID,UserID,plain,result,retval)	\
    ( (This)->lpVtbl -> ECCSignDigest(This,pair,AlgID,UserID,plain,result,retval) ) 

#define IDevice0_ExtECCVerifyDigest(This,AlgID,plain,cipher,eccPubKey,UserID,retval)	\
    ( (This)->lpVtbl -> ExtECCVerifyDigest(This,AlgID,plain,cipher,eccPubKey,UserID,retval) ) 

#define IDevice0_ExtECCEncrypt(This,eccPubKey,plain,cipher,retval)	\
    ( (This)->lpVtbl -> ExtECCEncrypt(This,eccPubKey,plain,cipher,retval) ) 

#define IDevice0_ECCDecrypt(This,pair,cipher,decipher,retval)	\
    ( (This)->lpVtbl -> ECCDecrypt(This,pair,cipher,decipher,retval) ) 

#define IDevice0_ECCGetTempPubKey(This,pair,TempPubKey,retval)	\
    ( (This)->lpVtbl -> ECCGetTempPubKey(This,pair,TempPubKey,retval) ) 

#define IDevice0_ECCKeyExchange(This,pair,AlgID,Role,phKey,ID,OID,TempECCPubKey,ECCPubKey,OTempPubKey,retval)	\
    ( (This)->lpVtbl -> ECCKeyExchange(This,pair,AlgID,Role,phKey,ID,OID,TempECCPubKey,ECCPubKey,OTempPubKey,retval) ) 

#define IDevice0_GetRandom(This,dataLen,randomData,retval)	\
    ( (This)->lpVtbl -> GetRandom(This,dataLen,randomData,retval) ) 

#define IDevice0_DigestInit(This,AlgID,retval)	\
    ( (This)->lpVtbl -> DigestInit(This,AlgID,retval) ) 

#define IDevice0_DigestUpdate(This,toDigest,retval)	\
    ( (This)->lpVtbl -> DigestUpdate(This,toDigest,retval) ) 

#define IDevice0_DigestFinal(This,resultFinal,retval)	\
    ( (This)->lpVtbl -> DigestFinal(This,resultFinal,retval) ) 

#define IDevice0_EncryptInit(This,IV,PaddingType,FeedBitLen,retval)	\
    ( (This)->lpVtbl -> EncryptInit(This,IV,PaddingType,FeedBitLen,retval) ) 

#define IDevice0_EncryptUpdate(This,plain,cipher,retval)	\
    ( (This)->lpVtbl -> EncryptUpdate(This,plain,cipher,retval) ) 

#define IDevice0_EncryptFinal(This,plain,cipher,retval)	\
    ( (This)->lpVtbl -> EncryptFinal(This,plain,cipher,retval) ) 

#define IDevice0_DecryptInit(This,IV,PaddingType,FeedBitLen,retval)	\
    ( (This)->lpVtbl -> DecryptInit(This,IV,PaddingType,FeedBitLen,retval) ) 

#define IDevice0_DecryptUpdate(This,cipher,plain,retval)	\
    ( (This)->lpVtbl -> DecryptUpdate(This,cipher,plain,retval) ) 

#define IDevice0_DecryptFinal(This,cipher,plain,retval)	\
    ( (This)->lpVtbl -> DecryptFinal(This,cipher,plain,retval) ) 

#define IDevice0_ImportSessionKey(This,AlgID,WrapedData,retval)	\
    ( (This)->lpVtbl -> ImportSessionKey(This,AlgID,WrapedData,retval) ) 

#define IDevice0_SetSymmKey(This,AlgID,SymmKey,retval)	\
    ( (This)->lpVtbl -> SetSymmKey(This,AlgID,SymmKey,retval) ) 

#define IDevice0_CloseHandle(This,retval)	\
    ( (This)->lpVtbl -> CloseHandle(This,retval) ) 

#define IDevice0_GetDLLType(This,pDLLType,retval)	\
    ( (This)->lpVtbl -> GetDLLType(This,pDLLType,retval) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDevice0_INTERFACE_DEFINED__ */


#ifndef __IGeneralStorage_INTERFACE_DEFINED__
#define __IGeneralStorage_INTERFACE_DEFINED__

/* interface IGeneralStorage */
/* [helpstring][uuid][object] */ 


EXTERN_C const IID IID_IGeneralStorage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3d739ab2-33ab-4461-9098-d1bcd4e16e7d")
    IGeneralStorage : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE FindMoreByType( 
            /* [in] */ int type,
            /* [out][in] */ LONG *tokens,
            /* [out][in] */ LONG *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindByData( 
            /* [in] */ VARIANT *data,
            /* [retval][out] */ LONG *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindByHash( 
            /* [in] */ VARIANT *hash,
            /* [retval][out] */ LONG *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveByToken( 
            /* [in] */ LONG token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateByToken( 
            /* [in] */ LONG token,
            /* [in] */ VARIANT *data) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QuerySizeByToken( 
            /* [in] */ LONG token,
            /* [retval][out] */ LONG *size) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendData( 
            /* [in] */ VARIANT *data,
            /* [in] */ LONG type,
            /* [in] */ VARIANT_BOOL override,
            /* [retval][out] */ LONG *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadData( 
            /* [in] */ LONG token,
            /* [out] */ VARIANT *data) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendDataByTrustLevel( 
            VARIANT *data,
            LONG type,
            VARIANT_BOOL override,
            LONG *token,
            int trustlevel,
            CHAR *altname) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindMoreByTrustLevel( 
            int trustlevel,
            LONG *tokens,
            LONG *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryTrustLevelByToken( 
            LONG token,
            LONG *trustlevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryAltNameByToken( 
            LONG token,
            CHAR *altname) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryCAaltNameByToken( 
            LONG token,
            CHAR *caAltName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendDataByTrustLevelCAaltName( 
            VARIANT *data,
            LONG type,
            VARIANT_BOOL override,
            LONG *token,
            int trustlevel,
            CHAR *altname,
            CHAR *CAaltName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindMoreByCAAltName( 
            CHAR *caAltName,
            LONG *tokens,
            LONG *count) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindMoreByTrustLevelType( 
            int trustlevel,
            int type,
            LONG *tokens,
            LONG *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindMoreByCAAltNameType( 
            CHAR *caAltName,
            int type,
            LONG *tokens,
            LONG *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryTypeByToken( 
            /* [in] */ LONG token,
            /* [retval][out] */ LONG *type) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeneralStorageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGeneralStorage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGeneralStorage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGeneralStorage * This);
        
        HRESULT ( STDMETHODCALLTYPE *FindMoreByType )( 
            IGeneralStorage * This,
            /* [in] */ int type,
            /* [out][in] */ LONG *tokens,
            /* [out][in] */ LONG *count);
        
        HRESULT ( STDMETHODCALLTYPE *FindByData )( 
            IGeneralStorage * This,
            /* [in] */ VARIANT *data,
            /* [retval][out] */ LONG *token);
        
        HRESULT ( STDMETHODCALLTYPE *FindByHash )( 
            IGeneralStorage * This,
            /* [in] */ VARIANT *hash,
            /* [retval][out] */ LONG *token);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveByToken )( 
            IGeneralStorage * This,
            /* [in] */ LONG token);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateByToken )( 
            IGeneralStorage * This,
            /* [in] */ LONG token,
            /* [in] */ VARIANT *data);
        
        HRESULT ( STDMETHODCALLTYPE *QuerySizeByToken )( 
            IGeneralStorage * This,
            /* [in] */ LONG token,
            /* [retval][out] */ LONG *size);
        
        HRESULT ( STDMETHODCALLTYPE *AppendData )( 
            IGeneralStorage * This,
            /* [in] */ VARIANT *data,
            /* [in] */ LONG type,
            /* [in] */ VARIANT_BOOL override,
            /* [retval][out] */ LONG *token);
        
        HRESULT ( STDMETHODCALLTYPE *ReadData )( 
            IGeneralStorage * This,
            /* [in] */ LONG token,
            /* [out] */ VARIANT *data);
        
        HRESULT ( STDMETHODCALLTYPE *AppendDataByTrustLevel )( 
            IGeneralStorage * This,
            VARIANT *data,
            LONG type,
            VARIANT_BOOL override,
            LONG *token,
            int trustlevel,
            CHAR *altname);
        
        HRESULT ( STDMETHODCALLTYPE *FindMoreByTrustLevel )( 
            IGeneralStorage * This,
            int trustlevel,
            LONG *tokens,
            LONG *count);
        
        HRESULT ( STDMETHODCALLTYPE *QueryTrustLevelByToken )( 
            IGeneralStorage * This,
            LONG token,
            LONG *trustlevel);
        
        HRESULT ( STDMETHODCALLTYPE *QueryAltNameByToken )( 
            IGeneralStorage * This,
            LONG token,
            CHAR *altname);
        
        HRESULT ( STDMETHODCALLTYPE *QueryCAaltNameByToken )( 
            IGeneralStorage * This,
            LONG token,
            CHAR *caAltName);
        
        HRESULT ( STDMETHODCALLTYPE *AppendDataByTrustLevelCAaltName )( 
            IGeneralStorage * This,
            VARIANT *data,
            LONG type,
            VARIANT_BOOL override,
            LONG *token,
            int trustlevel,
            CHAR *altname,
            CHAR *CAaltName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindMoreByCAAltName )( 
            IGeneralStorage * This,
            CHAR *caAltName,
            LONG *tokens,
            LONG *count);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindMoreByTrustLevelType )( 
            IGeneralStorage * This,
            int trustlevel,
            int type,
            LONG *tokens,
            LONG *count);
        
        HRESULT ( STDMETHODCALLTYPE *FindMoreByCAAltNameType )( 
            IGeneralStorage * This,
            CHAR *caAltName,
            int type,
            LONG *tokens,
            LONG *count);
        
        HRESULT ( STDMETHODCALLTYPE *QueryTypeByToken )( 
            IGeneralStorage * This,
            /* [in] */ LONG token,
            /* [retval][out] */ LONG *type);
        
        END_INTERFACE
    } IGeneralStorageVtbl;

    interface IGeneralStorage
    {
        CONST_VTBL struct IGeneralStorageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeneralStorage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGeneralStorage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGeneralStorage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGeneralStorage_FindMoreByType(This,type,tokens,count)	\
    ( (This)->lpVtbl -> FindMoreByType(This,type,tokens,count) ) 

#define IGeneralStorage_FindByData(This,data,token)	\
    ( (This)->lpVtbl -> FindByData(This,data,token) ) 

#define IGeneralStorage_FindByHash(This,hash,token)	\
    ( (This)->lpVtbl -> FindByHash(This,hash,token) ) 

#define IGeneralStorage_RemoveByToken(This,token)	\
    ( (This)->lpVtbl -> RemoveByToken(This,token) ) 

#define IGeneralStorage_UpdateByToken(This,token,data)	\
    ( (This)->lpVtbl -> UpdateByToken(This,token,data) ) 

#define IGeneralStorage_QuerySizeByToken(This,token,size)	\
    ( (This)->lpVtbl -> QuerySizeByToken(This,token,size) ) 

#define IGeneralStorage_AppendData(This,data,type,override,token)	\
    ( (This)->lpVtbl -> AppendData(This,data,type,override,token) ) 

#define IGeneralStorage_ReadData(This,token,data)	\
    ( (This)->lpVtbl -> ReadData(This,token,data) ) 

#define IGeneralStorage_AppendDataByTrustLevel(This,data,type,override,token,trustlevel,altname)	\
    ( (This)->lpVtbl -> AppendDataByTrustLevel(This,data,type,override,token,trustlevel,altname) ) 

#define IGeneralStorage_FindMoreByTrustLevel(This,trustlevel,tokens,count)	\
    ( (This)->lpVtbl -> FindMoreByTrustLevel(This,trustlevel,tokens,count) ) 

#define IGeneralStorage_QueryTrustLevelByToken(This,token,trustlevel)	\
    ( (This)->lpVtbl -> QueryTrustLevelByToken(This,token,trustlevel) ) 

#define IGeneralStorage_QueryAltNameByToken(This,token,altname)	\
    ( (This)->lpVtbl -> QueryAltNameByToken(This,token,altname) ) 

#define IGeneralStorage_QueryCAaltNameByToken(This,token,caAltName)	\
    ( (This)->lpVtbl -> QueryCAaltNameByToken(This,token,caAltName) ) 

#define IGeneralStorage_AppendDataByTrustLevelCAaltName(This,data,type,override,token,trustlevel,altname,CAaltName)	\
    ( (This)->lpVtbl -> AppendDataByTrustLevelCAaltName(This,data,type,override,token,trustlevel,altname,CAaltName) ) 

#define IGeneralStorage_FindMoreByCAAltName(This,caAltName,tokens,count)	\
    ( (This)->lpVtbl -> FindMoreByCAAltName(This,caAltName,tokens,count) ) 

#define IGeneralStorage_FindMoreByTrustLevelType(This,trustlevel,type,tokens,count)	\
    ( (This)->lpVtbl -> FindMoreByTrustLevelType(This,trustlevel,type,tokens,count) ) 

#define IGeneralStorage_FindMoreByCAAltNameType(This,caAltName,type,tokens,count)	\
    ( (This)->lpVtbl -> FindMoreByCAAltNameType(This,caAltName,type,tokens,count) ) 

#define IGeneralStorage_QueryTypeByToken(This,token,type)	\
    ( (This)->lpVtbl -> QueryTypeByToken(This,token,type) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGeneralStorage_INTERFACE_DEFINED__ */


#ifndef __IGeneralSettings_INTERFACE_DEFINED__
#define __IGeneralSettings_INTERFACE_DEFINED__

/* interface IGeneralSettings */
/* [helpstring][uuid][object] */ 


EXTERN_C const IID IID_IGeneralSettings;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("53b16ee3-80c1-4235-81cf-448237432b27")
    IGeneralSettings : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ReadSettingString( 
            /* [in] */ BSTR path,
            /* [in] */ BSTR defaultValue,
            /* [retval][out] */ BSTR *data) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadSettingLong( 
            /* [in] */ BSTR path,
            /* [in] */ LONG defaultValue,
            /* [retval][out] */ LONG *data) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadSettingBytes( 
            /* [in] */ BSTR path,
            /* [retval][out] */ VARIANT *data) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSettingItems( 
            /* [in] */ BSTR path,
            /* [in] */ LONG options,
            /* [out][in] */ LONG *count,
            /* [out] */ BSTR *entries) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSettingString( 
            /* [in] */ BSTR path,
            /* [in] */ BSTR value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSettingLong( 
            /* [in] */ BSTR path,
            /* [in] */ LONG value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WriteSettingBytes( 
            /* [in] */ BSTR path,
            /* [in] */ VARIANT *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeneralSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGeneralSettings * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGeneralSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGeneralSettings * This);
        
        HRESULT ( STDMETHODCALLTYPE *ReadSettingString )( 
            IGeneralSettings * This,
            /* [in] */ BSTR path,
            /* [in] */ BSTR defaultValue,
            /* [retval][out] */ BSTR *data);
        
        HRESULT ( STDMETHODCALLTYPE *ReadSettingLong )( 
            IGeneralSettings * This,
            /* [in] */ BSTR path,
            /* [in] */ LONG defaultValue,
            /* [retval][out] */ LONG *data);
        
        HRESULT ( STDMETHODCALLTYPE *ReadSettingBytes )( 
            IGeneralSettings * This,
            /* [in] */ BSTR path,
            /* [retval][out] */ VARIANT *data);
        
        HRESULT ( STDMETHODCALLTYPE *GetSettingItems )( 
            IGeneralSettings * This,
            /* [in] */ BSTR path,
            /* [in] */ LONG options,
            /* [out][in] */ LONG *count,
            /* [out] */ BSTR *entries);
        
        HRESULT ( STDMETHODCALLTYPE *WriteSettingString )( 
            IGeneralSettings * This,
            /* [in] */ BSTR path,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE *WriteSettingLong )( 
            IGeneralSettings * This,
            /* [in] */ BSTR path,
            /* [in] */ LONG value);
        
        HRESULT ( STDMETHODCALLTYPE *WriteSettingBytes )( 
            IGeneralSettings * This,
            /* [in] */ BSTR path,
            /* [in] */ VARIANT *value);
        
        END_INTERFACE
    } IGeneralSettingsVtbl;

    interface IGeneralSettings
    {
        CONST_VTBL struct IGeneralSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeneralSettings_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGeneralSettings_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGeneralSettings_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGeneralSettings_ReadSettingString(This,path,defaultValue,data)	\
    ( (This)->lpVtbl -> ReadSettingString(This,path,defaultValue,data) ) 

#define IGeneralSettings_ReadSettingLong(This,path,defaultValue,data)	\
    ( (This)->lpVtbl -> ReadSettingLong(This,path,defaultValue,data) ) 

#define IGeneralSettings_ReadSettingBytes(This,path,data)	\
    ( (This)->lpVtbl -> ReadSettingBytes(This,path,data) ) 

#define IGeneralSettings_GetSettingItems(This,path,options,count,entries)	\
    ( (This)->lpVtbl -> GetSettingItems(This,path,options,count,entries) ) 

#define IGeneralSettings_WriteSettingString(This,path,value)	\
    ( (This)->lpVtbl -> WriteSettingString(This,path,value) ) 

#define IGeneralSettings_WriteSettingLong(This,path,value)	\
    ( (This)->lpVtbl -> WriteSettingLong(This,path,value) ) 

#define IGeneralSettings_WriteSettingBytes(This,path,value)	\
    ( (This)->lpVtbl -> WriteSettingBytes(This,path,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGeneralSettings_INTERFACE_DEFINED__ */


#ifndef __IDevProvider0_INTERFACE_DEFINED__
#define __IDevProvider0_INTERFACE_DEFINED__

/* interface IDevProvider0 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDevProvider0;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A82F5E12-965C-47D5-98AD-E9C5E95B746C")
    IDevProvider0 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDeviceCount( 
            /* [in] */ LONG filters,
            /* [retval][out] */ LONG *count) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ BSTR ip,
            /* [in] */ LONG port,
            /* [retval][out] */ LONG *retval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDevice( 
            BSTR id,
            IDevice0 **device) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryGUIDS( 
            /* [out] */ BSTR *guids,
            /* [out] */ LONG *count,
            /* [retval][out] */ LONG *retval) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDevProvider0Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDevProvider0 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDevProvider0 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDevProvider0 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDeviceCount )( 
            IDevProvider0 * This,
            /* [in] */ LONG filters,
            /* [retval][out] */ LONG *count);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IDevProvider0 * This,
            /* [in] */ BSTR ip,
            /* [in] */ LONG port,
            /* [retval][out] */ LONG *retval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDevice )( 
            IDevProvider0 * This,
            BSTR id,
            IDevice0 **device);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *QueryGUIDS )( 
            IDevProvider0 * This,
            /* [out] */ BSTR *guids,
            /* [out] */ LONG *count,
            /* [retval][out] */ LONG *retval);
        
        END_INTERFACE
    } IDevProvider0Vtbl;

    interface IDevProvider0
    {
        CONST_VTBL struct IDevProvider0Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDevProvider0_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDevProvider0_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDevProvider0_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDevProvider0_GetDeviceCount(This,filters,count)	\
    ( (This)->lpVtbl -> GetDeviceCount(This,filters,count) ) 

#define IDevProvider0_Initialize(This,ip,port,retval)	\
    ( (This)->lpVtbl -> Initialize(This,ip,port,retval) ) 

#define IDevProvider0_GetDevice(This,id,device)	\
    ( (This)->lpVtbl -> GetDevice(This,id,device) ) 

#define IDevProvider0_QueryGUIDS(This,guids,count,retval)	\
    ( (This)->lpVtbl -> QueryGUIDS(This,guids,count,retval) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDevProvider0_INTERFACE_DEFINED__ */



#ifndef __DEVPROVIDERLib_LIBRARY_DEFINED__
#define __DEVPROVIDERLib_LIBRARY_DEFINED__

/* library DEVPROVIDERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DEVPROVIDERLib;

EXTERN_C const CLSID CLSID_DevProvider0;

#ifdef __cplusplus

class DECLSPEC_UUID("6735AF3C-721C-469E-831A-FE84CEBBE742")
DevProvider0;
#endif

EXTERN_C const CLSID CLSID_Device0;

#ifdef __cplusplus

class DECLSPEC_UUID("620D5FAE-D29F-4CA3-B633-67DDD106DBCD")
Device0;
#endif
#endif /* __DEVPROVIDERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


