#ifndef __KTTHREAD_H__
#define __KTTHREAD_H__

/**
  * @Name: KTThread.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 23:43
  * @Description: generic thread code
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include <KTType.h>

#ifdef  KT_UNIX
	#ifndef KT_UNIX_SOLARIS	
		#define KT_UNIX_LINUX   
	#endif
#endif

/*
 * On all platforms but Win32, cond variables can do broadcasts.
 * cond_init2 needs to be special for Win32 to support broadcasts.
 * Use cond_init if you never need broadcast.  Use cond_init2 if you
 * ONLY do broadcasts.
 */

#ifdef KT_UNIX_SOLARIS
#pragma error("asdasdf")

#include <thread.h>
#include <synch.h>


#define _KT_MUTEX
#undef HAVE_PTHREAD_CREATE
//#define HAVE_PTHREAD_CREATE 0	/* we also have this, but don't want it */

typedef void	*(*VFP)();

/* solaris threads are preemptive */
#define KTTHREAD_PREEMPTIVE	1

/* default attr states */
#define KTMutexAttr_default	NULL

/* mutex and condition variable scope - process or system */
#define KTTHREAD_SHARE_PRIVATE   USYNC_THREAD
#define KTTHREAD_SHARE_PROCESS   USYNC_PROCESS

typedef int		KTMutexAttr_t;
typedef mutex_t		KTMutex_t;

#endif /* end KT_UNIX_SOLARIS */

#ifdef HAVE_PTHREAD_CREATE

#include <pthread.h>

#define _KT_MUTEX

#define KTMutexAttr_default	NULL

typedef void	*(*VFP)();

#ifdef KT_UNIX_LINUX	/* I think Linux pthreads are preemptive */
# define KTTHREAD_PREEMPTIVE	1
#else
# define KTTHREAD_PREEMPTIVE	0
#endif

/* default attr states */
#define KTMutexAttr_default	NULL

/* mutex and condition variable scope - process or system */
#define KTTHREAD_SHARE_PRIVATE   0
#define KTTHREAD_SHARE_PROCESS   0

typedef pthread_mutex_t		KTMutex_t;
typedef pthread_mutexattr_t	KTMutexAttr_t;

#endif /* end PTHREAD_CREATE */

#ifdef HAVE_PTHREAD_ATTR_CREATE

#define _KT_THREAD
#define _KT_MUTEX

#include <pthread.h>

/* dce threads are preemptive */
#define KTTHREAD_PREEMPTIVE	1

#endif /* end HAVE_PTHREAD_ATTR_CREATE */


#ifdef KT_WIN32

/***********************************
 *								 *
 * thread definitions for winnt	*
 *								 *
 ***********************************/
#include <windows.h>

#define _KT_MUTEX

typedef unsigned (__stdcall *VFP)(void *);
/* typedef DWORD WINAPI (*VFP)(LPVOID); */

/* winnt threads are preemptive */
#define KTTHREAD_PREEMPTIVE	1

/* default attr states */
#define KTMutexAttr_default	NULL
#define KTCondAttr_default	NULL

/* thread state - joinable or not */
#define KTTHREAD_CREATE_JOINABLE	0
#define KTTHREAD_CREATE_DETACHED	1

/* thread scope - who is in scheduling pool */
#define KTTHREAD_SCOPE_PROCESS	0
#define KTTHREAD_SCOPE_SYSTEM	1

/* mutex and condition variable scope - process or system */
#define KTTHREAD_SHARE_PRIVATE	0
#define KTTHREAD_SHARE_PROCESS	1

struct KTMutexAttr_t{
	LPSECURITY_ATTRIBUTES lpsa;
	LPTSTR				  lpszMutexName;
};

//typedef int	KTMutexAttr_t;
typedef HANDLE  KTMutex_t;


#endif /* winnt threads */

#ifndef _KT_MUTEX
    #define KTMutexAttr_default	NULL
    typedef int	KTMutexAttr_t;
    typedef int	KTMutex_t;
#endif /* _KT_MUTEX */

#ifdef KT_NO_MUTEX
    typedef int	KTMutexAttr_t;
    typedef int  KTMutex_t;
#endif /* KT_NO_MUTEX */

int KTMutexCreate( KTMutex_t *mp, KTMutexAttr_t *attr );
int KTMutexDestroy( KTMutex_t *mp );
int KTMutexLock( KTMutex_t *mp );
int KTMutexUnlock( KTMutex_t *mp );
int KTMutexTryLock( KTMutex_t *mp );

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 23:10  Hongwei Yu   created 

#endif
