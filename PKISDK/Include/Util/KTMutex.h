#ifndef __KTMUTEX_H__
#define __KTMUTEX_H__

/**
  * @Name: KTMutex.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 22:52
  * @Description: generic Mutex code.
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "KTThread.h"

class KT_LIB_MODE CKTMutex
{
public:
	KTMutex_t		*m_mutex;
	KTMutexAttr_t	*m_attr;

public:

	CKTMutex(KTCHAR* pMutexName=NULL);
	~CKTMutex();

	int lock();
	int unLock();
	int tryLock();
};
#ifdef KT_WIN32
	#pragma pack(pop)
#endif

// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 22:51  Hongwei Yu   created 

#endif 
