#ifndef __KTUILOGIN_H__
#define __KTUILOGIN_H__

/**
  * @Name: KTUILogin.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 00:16
  * @Description: KTUILogin API function and pre define structure
*/

#include "KTType.h"
#include "PKIDef.h"

#ifdef KT_WIN32
#pragma pack(push,1)
#endif

#ifdef __cplusplus
    extern "C"{
#endif

KT_LIB_MODE KTINT32
KTPKI_UI_Initialize(
	IN	void *			pReserver	//default NULL
);
KT_LIB_MODE KTINT32
KTPKI_UI_Finalize(
	IN	void *			pReserver	//default NULL
);

#define KTPKI_UI_LOGIN_ATTACHMENT_INFO_MAGIC 0x55AA5A5A	//表示有附加信息填入登陆参数
#define KTPKI_UI_LOGIN_ATTACHMENT_INFO 0x00010001			//结构类型基码
#define KTPKI_UI_LOGIN_DEFAULT_USER_PASSWORD ( KTPKI_UI_LOGIN_ATTACHMENT_INFO + 1 ) //用户名和密码结构码
#define KTPKI_UI_LOGIN_FINGER ( KTPKI_UI_LOGIN_ATTACHMENT_INFO + 2 ) //用户名和指纹结构码

typedef struct KTPKI_UI_ATTACHMENT_INFO{
	KTUINT32 nMagic;	//表示有附加信息填入登陆参数
	KTUINT32 nId;		//结构类型码
}KTPKI_UI_ATTACHMENT_INFO, *KTPKI_UI_ATTACHMENT_INFO_PTR;

typedef struct KTPKI_UI_LOGIN_USERINFO{
	KTUINT32 nMagic;	//表示有附加信息填入登陆参数
	KTUINT32 nId;		//结构类型码
	KTCHAR	cUser[128];
	KTCHAR	cPassword[64];
}KTPKI_UI_LOGIN_USERINFO, *KTPKI_UI_LOGIN_USERINFO_PTR;

#define KTPKI_UI_LOGIN_SETDEFAULTPASSWORD_USERINFO(v, user, pass)	v.nMagic = KTPKI_UI_LOGIN_ATTACHMENT_INFO_MAGIC;	\
		v.nId = KTPKI_UI_LOGIN_DEFAULT_USER_PASSWORD;		\
		strcpy(v.cUser, user);							\
		strcpy(v.cPassword, pass);

typedef struct KTPKI_UI_LOGIN_PARAM{
	KTBOOL				bPopSelTokenDlg;		//如果当前设备个数只有1时，是否弹出选择设备对话框,，default:FALSE
	KTBYTE				VisibleSlot[MAX_SLOT_TYPE_NUM];//弹出有效SLOT类型 default :all
	KTUINT32			nContext;			//选择SLOT的上下文，相同的上下文才能共用SLOT.default=1
	KTUINT32			nCurSlot;			//设置当前SLOT id.
	KTBOOL				bNewUser;			//是否创建新用户  default=FALSE
	KTBOOL				bUnusedSlotOnly;	//仅使用未使用的Slot  default=FALSE
	KTPKI_UI_ATTACHMENT_INFO_PTR pAttachmentInfo;		//附加参数的指针
	KTBOOL				bShowLoginDlg;		//当需要输入登登录口令时，是否弹出登录对话框 default:TRUE
	KTBOOL				bUseLastLoginInfo;	//是否使用上次登录信息 default:TRUE
	KTBYTE				szReserved[48];
}KTPKI_UI_LOGIN_PARAM,*KTPKI_UI_LOGIN_PARAM_PTR;

//设置设备可用SLOT信息
//如:
// KTPKI_UI_LOGIN_PARAM param
// KTPKI_UI_LOGIN_SET_SLOT(param,KT_FILE_SLOT,FALSE) 不显示文件SLOT
#define KTPKI_UI_LOGIN_SET_SLOT(v,nType,result)	v.VisibleSlot[nType]=result

//设置所有SLOT可用
#define KTPKI_UI_LOGIN_SET_SLOT_ALL(v)	memset(v.VisibleSlot,1,MAX_SLOT_TYPE_NUM)

#define KTPKI_UI_LOGIN_DEFAULT_PARAM(v) v.bPopSelTokenDlg=FALSE;\
	memset(v.VisibleSlot,1,MAX_SLOT_TYPE_NUM);\
	v.nContext=1;\
	v.nCurSlot=-1;\
	v.bNewUser=FALSE;\
	v.bUnusedSlotOnly=FALSE;\
	v.pAttachmentInfo=0;\
	v.bShowLoginDlg=TRUE;\
	v.bUseLastLoginInfo=TRUE;\
	memset(v.szReserved,-1,sizeof(v.szReserved));

KT_LIB_MODE KTINT32
KTPKI_GetSlotState(
	IN  KTINT						nIndex,
	OUT KT_SLOT_STATE_PTR			pSlotState
);

KT_LIB_MODE KTINT32
KTPKI_UI_Login(
	IN  KTBOOL						bReset,			
	IN  KTPKI_UI_LOGIN_PARAM_PTR		pParam,	//can be NULL
	IN	KT_UI_PARAM_PTR				pUIParam, //can be NULL
	OUT KT_SESSION_HANDLE_PTR		phSession
);

KT_LIB_MODE KTINT32 
KTPKI_UI_GetTokenInfo(KTUINT32 nContext, KTUINT *pnTokenID, KTUINT *pnTokenType, 
					 KTChar pc_username[64], KTBYTE pb_userPIN[32]);
KT_LIB_MODE KTINT32 
KTPKI_UI_SetTokenInfo(KTUINT32 nContext, KTUINT nTokenID, KTUINT nTokenType, 
					 const KTChar pc_userName[], const KTBYTE pb_userPIN[32]);

KT_LIB_MODE void KTPKI_UI_ChngPIN(HWND hParentWnd, const KTChar* pc_caption);

#ifdef __cplusplus
    }
#endif

#ifdef KT_WIN32
    #pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 00:15  Hongwei Yu   created
///   1.0.0.1  2003/12/11 16:38  Hongwei Yu   added bShowLoginDlg and bUseLastLoginInfo param  

#endif