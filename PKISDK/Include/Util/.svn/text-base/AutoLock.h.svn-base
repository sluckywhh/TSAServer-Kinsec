#ifndef Included_CAutoLock_h
#define Included_CAutoLock_h

/**
  * @Name: CAutoLock.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 22:48
  * @Description: Automatic Lock and Unlock
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

class CKTMutex;

class KT_LIB_MODE CAutoLock
{
public:
	CAutoLock(CKTMutex *pMutex) {
		assert(pMutex);
		m_pMutex = pMutex;
		m_pMutex->lock();
	}
	~CAutoLock() {
		m_pMutex->unLock();
	}

private:
	CKTMutex	*m_pMutex;
};


#ifdef KT_WIN32
	#pragma pack(pop)
#endif

// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 22:50  Hongwei Yu   created 

#endif	//	Included_CAutoLock_h
