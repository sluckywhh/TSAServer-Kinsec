#ifndef _KT_INTERFACE_H
#define _KT_INTERFACE_H

/**
  *@Name: KTInterface.h
  *@Copyright: KTXA SoftWare
  *@Author: Hongwei Yu
  *@Date:  2003/04/03 16:23
  *@Description: class id and interface id define 
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include <string.h>
#include "KTSTL.h"
#include "KTType.h"
#include "KTMacro.h"
#include "KTUnknow.h"

#ifdef KT_DLL // assume this is defined when we build the dll
#define _DYNLINK __declspec( dllexport)
#else
#define _DYNLINK __declspec( dllimport)
#endif
const KTUINT32		KT_LIB_MODEID_BASE				=	100000;
const KTUINT32		KT_INTERFACEID_BASE			=	900000;

const KTUINT32		KT_LIB_MODEID_KSDK_BASE		=	KT_LIB_MODEID_BASE-1001;
const KTUINT32		KT_INTERFACEID_KSDK_BASE	=	KT_INTERFACEID_BASE-1001;

const KTIID		KTIID_UNKNOW					=	KT_INTERFACEID_BASE-1;
///////////////////////////////////////////////////////////////////////
//define class id.
//this is for icc class id.
const KTCID		KTCID_ICC_GEMPLUS_PK			=	KT_LIB_MODEID_KSDK_BASE-1;	// gemplus卡
const KTCID		KTCID_ICC_WATCH_PK				=	KT_LIB_MODEID_KSDK_BASE-2; // 握奇卡 
const KTCID		KTCID_ICC_MW_PK					=	KT_LIB_MODEID_KSDK_BASE-7; // 明华卡 

//this is for key class id.
const KTCID		KTCID_KEY_SMARTKEY  			=	KT_LIB_MODEID_KSDK_BASE-3;	// SmartKey
const KTCID     KTCID_KEY_INFOKEY               =   KT_LIB_MODEID_KSDK_BASE-4;	// InfoKey
const KTCID     KTCID_KEY_WATCHKEY              =   KT_LIB_MODEID_KSDK_BASE-5;	// 握奇Key
const KTCID     KTCID_KEY_MWKEY                 =   KT_LIB_MODEID_KSDK_BASE-6;	// 明华Key
const KTCID     KTCID_KEY_WATCHTWIN             =   KT_LIB_MODEID_KSDK_BASE-8;	// 握奇KeyTwin
const KTCID     KTCID_KEY_MWTWIN                =   KT_LIB_MODEID_KSDK_BASE-9;	// 明华KeyTwin
const KTCID     KTCID_KEY_WESTONEKEY            =   KT_LIB_MODEID_KSDK_BASE-10;	// 卫士通Key
const KTCID     KTCID_KEY_WESTONETWIN           =   KT_LIB_MODEID_KSDK_BASE-11;	// 卫士通KeyTwin
const KTCID     KTCID_KEY_DEANKEY				=   KT_LIB_MODEID_KSDK_BASE-12;	// 得安Key(SZD13)
const KTCID     KTCID_KEY_DEANTWIN				=   KT_LIB_MODEID_KSDK_BASE-13;	// 得安KeyTwin
const KTCID     KTCID_KEY_ZCKEY					=   KT_LIB_MODEID_KSDK_BASE-14;	// 中钞Key
const KTCID     KTCID_KEY_ZCTWIN		        =   KT_LIB_MODEID_KSDK_BASE-15;	// 中钞KeyTwin
const KTCID     KTCID_KEY_MW_PCSCKEY			=   KT_LIB_MODEID_KSDK_BASE-16;	// 明华eKey(PCSC)
const KTCID     KTCID_KEY_MW_PCSCTWIN			=   KT_LIB_MODEID_KSDK_BASE-17;	// 明华eKeyTwin(PCSC)
const KTCID     KTCID_KEY_HBKEY					=   KT_LIB_MODEID_KSDK_BASE-18;	// 恒宝Key
const KTCID     KTCID_KEY_HBTWIN		        =   KT_LIB_MODEID_KSDK_BASE-19;	// 恒宝KeyTwin
const KTCID     KTCID_KEY_PKCS11WQKEY			=   KT_LIB_MODEID_KSDK_BASE-20;	// 握奇3.0Key


//this is for keymangr class id.
const KTCID		KTCID_KEYMANGR_SMARTKEY			=	KT_LIB_MODEID_KSDK_BASE-43;
const KTCID		KTCID_KEYMANGR_INFOKEY			=	KT_LIB_MODEID_KSDK_BASE-44;
const KTCID		KTCID_KEYMANGR_WATCHKEY			=	KT_LIB_MODEID_KSDK_BASE-45;
const KTCID		KTCID_KEYMANGR_MWKEY			=	KT_LIB_MODEID_KSDK_BASE-46;
const KTCID		KTCID_KEYMANGR_WATCHTWIN		=	KT_LIB_MODEID_KSDK_BASE-47;
const KTCID		KTCID_KEYMANGR_MWTWIN			=	KT_LIB_MODEID_KSDK_BASE-48;
const KTCID		KTCID_KEYMANGR_WESTONEKEY		=	KT_LIB_MODEID_KSDK_BASE-49;
const KTCID		KTCID_KEYMANGR_WESTONETWIN		=	KT_LIB_MODEID_KSDK_BASE-50;
const KTCID		KTCID_KEYMANGR_DEANKEY			=	KT_LIB_MODEID_KSDK_BASE-51;
const KTCID		KTCID_KEYMANGR_DEANTWIN			=	KT_LIB_MODEID_KSDK_BASE-52;
const KTCID		KTCID_KEYMANGR_ZCKEY			=	KT_LIB_MODEID_KSDK_BASE-53;
const KTCID		KTCID_KEYMANGR_ZCTWIN			=	KT_LIB_MODEID_KSDK_BASE-54;
const KTCID		KTCID_KEYMANGR_MW_PCSCKEY		=	KT_LIB_MODEID_KSDK_BASE-55;
const KTCID		KTCID_KEYMANGR_MW_PCSCTWIN		=	KT_LIB_MODEID_KSDK_BASE-56;
const KTCID		KTCID_KEYMANGR_HBKEY			=	KT_LIB_MODEID_KSDK_BASE-57;
const KTCID		KTCID_KEYMANGR_HBTWIN			=	KT_LIB_MODEID_KSDK_BASE-58;
const KTCID		KTCID_KEYMANGR_PKCS11WQKEY		=	KT_LIB_MODEID_KSDK_BASE-59;


//this is for ikey class id.
const KTCID		KTCID_IKEY_1000					=	KT_LIB_MODEID_KSDK_BASE-20;

const KTCID		KTCID_EMC_SJL05					=	KT_LIB_MODEID_KSDK_BASE-30;
const KTCID		KTCID_EMC_SJY36					=	KT_LIB_MODEID_KSDK_BASE-31;
const KTCID		KTCID_EMC_SJY15					=	KT_LIB_MODEID_KSDK_BASE-32;

///////////////////////////////////////////////////////////////////////
//define interface id.

//this is for icc interface id.
const KTIID		KTIID_ICC_HELPER				=	KT_INTERFACEID_KSDK_BASE-1;
const KTIID		KTIID_ICC_HELPER_PK				=	KT_INTERFACEID_KSDK_BASE-2;

const KTIID		KTIID_IKEY_HELPER				=	KT_INTERFACEID_KSDK_BASE-20;

const  KTIID	KTIID_RSA_EMC					=	KT_INTERFACEID_KSDK_BASE-30;

const KTIID		KTIID_KEY_MANGR					=	KT_INTERFACEID_KSDK_BASE-40;

const KTIID		KTIID_PLUGIN					=	KT_INTERFACEID_KSDK_BASE-100;


const  KTIID	KTIID_CIPHER_ENGINE				=	KT_INTERFACEID_KSDK_BASE-200;
const  KTIID	KTIID_MD_ENGINE					=	KT_INTERFACEID_KSDK_BASE-201;
const  KTIID	KTIID_KEY_GENERATOR				=	KT_INTERFACEID_KSDK_BASE-202;
const  KTIID	KTIID_KEY_PAIR_GENERATOR		=	KT_INTERFACEID_KSDK_BASE-203;

//define 20001-11-22
const  KTIID KTIID_KPUIFACTORY					=KT_INTERFACEID_KSDK_BASE-300;


///////////////////////////////////////////////////////////////////////
/**
* define Get Interface function.
* @param cid:the class id.[IN]
* @param iid:the interface id.[IN]
* @ppv:Address of output variable that receives the 
*      interface pointer requested in iid.[OUT]	
*	   !!!it must be release.
*/

#define KT_CREATE_INSTANCE(cid,iid,ppv)		\
	KTstd_##cid::KTCreateInstance(cid,iid,ppv)
	
//KTINT32 KTCreateInstance(KTCID cid,KTIID iid,void **ppv);
#ifndef KT_EXPORTS
#define KT_EXPORTS
#endif
//in static lib.
#define DECLARE_KT_CREATE_INSTANCE_LIB(names)			\
namespace KTstd_##names{								\
	KT_EXPORTS KTINT32 KTCreateInstance(KTCID cid,KTIID iid,void **ppv);\
}

#define BEGIN_KT_CREATE_INSTANCE(names)	\
namespace KTstd_##names{						

#define IMPLEMENT_KT_CREATE_INSTANCE(theClass)				\
	KTINT32 KTCreateInstance(KTCID cid,KTIID iid,void **ppv)	\
	{														\
		KTINT32 rv=KT_ERR_CRNT_NOSUPPORT;							\
		*ppv=NULL;											\
		theClass *pClass=NULL;								\
		pClass=new theClass;								\
		rv=pClass->queryInterface(iid,ppv);					\
		if(KT_OK !=rv)										\
		{													\
			safe_delete(pClass);							\
		}													\
		return rv;											\
	}								

#define END_KT_CREATE_INSTANCE()	\
}

/**
* get a kind of interface in dll. 
* @see KTINTerface.cpp
* @param dllPath:the path of dll file.[IN]
*				 if NULL then it's default path.win32:system directory.
* @param dllName:the name of dll name. supported * ?[IN]
*				 e.g. KLSlt*.dll,KLTkn*.dll							
* @parma iid:the id of interface to be gotten.[IN]
* @param ppv:the array of interface pointer.
* @return the handle of dll.
*/
struct KT_DYN_INTERFACE
{
	KTHANDLE handle;
	void *pInterface;
	KT_DYN_INTERFACE(){handle=0;pInterface=NULL;};
};

#define	MODULE_NAME_LEN				32
#define	MODULE_DESCRIPTION_LEN		64

struct ModuleInfo
{
	KTUINT32	nType;
	KTUINT32	nVersion;
	KTCHAR		szName[MODULE_NAME_LEN];
	KTCHAR		szDescription[MODULE_DESCRIPTION_LEN];
	KTUINT32	nProps[8];
	KTBYTE		bReserved[120];
};

struct KT_DYN_INTERFACE2
{
	KTHANDLE handle;
	void *pInterface;
	ModuleInfo modInfo;
	KT_DYN_INTERFACE2(){handle=0;pInterface=NULL;memset(&modInfo, 0, sizeof(ModuleInfo));};
};

typedef std::vector<KT_DYN_INTERFACE> KTDynInterfaceArray;
typedef std::vector<KT_DYN_INTERFACE2> KTDynInterface2Array;

/////in KTUtil, end with'\\'
KTBOOL KTGetDefaultPlugFilePath(KTChar pc_plugPath[KT_MAX_PATH], UINT unBufLen);
KTINT32 KTLoadDynInterface(const KTCHAR* dllPath,KTCHAR* dllName,KTIID iid,KTDynInterfaceArray& rDynInterface);
KTINT32 KTFreeDynInterface(KTDynInterfaceArray& rDynInterface);

KTINT32 KTLoadDynInterface2(
	const KTCHAR*				dllPath,
	KTCHAR*				dllName,
	KTIID					iid,
	const ModuleInfo		*pCallerInfo,
	KTDynInterface2Array	&rDynInterface2
);

KTINT32 KTFreeDynInterface2(
	KTDynInterface2Array	&rDynInterface2
);

typedef std::vector<void *> KTPtrArray;
#define		KT_MAX_INTERFACE_NUM		100
#ifndef KT_DLL
typedef KTINT32 (*PFN_QueryInterface)(KTIID iid,void **ppv);
typedef KTINT32 (*PFN_QueryInterface2)(KTIID iid, const ModuleInfo *pCallerInfo, ModuleInfo *pCalleeInfo, void **ppv);
//typedef KTINT32 (*PFN_QueryInterface)(KTIID iid,KTPtrArray& rInterfacePtr);
#else
//_DYNLINK KTINT32 KTQueryInterface(KTIID iid,KTPtrArray& rInterfacePtr);
_DYNLINK KTINT32 KTQueryInterface(KTIID iid,void **ppv);
_DYNLINK KTINT32 KTQueryInterface2(KTIID iid, const ModuleInfo *pCallerInfo, ModuleInfo *pCalleeInfo, void **ppv);
#endif

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/03 16:26  Hongwei Yu   created 
///   1.0.0.0  2003/05/12 21:53  Hongwei Yu   added KTGetDefaultPlugFilePath

#endif //#ifndef _INTERFACE_H
