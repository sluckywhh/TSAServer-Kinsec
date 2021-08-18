#ifndef __KS_PROCESSLOCK_H__
#define __KS_PROCESSLOCK_H__

#include <string>

using namespace std;

// =================================================================================
//  Include Header Files

// =================================================================================
//  Micro Definition

// =================================================================================
//  Class Definition
class KSProcessLock
{
public:
	// = Default constructor.
	KSProcessLock(const char *pszFileName);

	// = Default destructor.
	virtual ~KSProcessLock();

	bool TryLock();
	bool Lock();
	bool UnLock();

private:
	bool m_bLocked;
	int m_fd;
	string m_strFilePath;
};

#endif  // __KS_PROCESSLOCK_H__
