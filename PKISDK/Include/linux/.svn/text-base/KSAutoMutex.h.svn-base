#ifndef __KS_AUTOMUTEX_H__
#define __KS_AUTOMUTEX_H__     

// =================================================================================
//  Include Header Files

// =================================================================================
//  Micro Definition

// =================================================================================
//  Class Definition
class KSAutoMutex
{
public :
	// = Default constructor.
	KSAutoMutex(void *hMutex);

	// = Default destructor.
	virtual ~KSAutoMutex();

private :
	// = Mutex handle.
	void *m_hMutex;
};

void *KSCreateMutex();
void KSDeleteMutex(void *hMutex);

#endif  // __KS_AUTOMUTEX_H__
