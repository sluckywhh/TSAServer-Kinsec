#ifndef _CKTSTRING_H
#define _CKTSTRING_H

/*
  class:	CKTString
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <iostream>
#include <string.h>

using namespace std;

#if defined(_WIN32)
    #include <windows.h>
    #define vsnprintf _vsnprintf
#elif defined(__GNUC__)
    #include "ptspinlock.h"
#endif

//#define INT_MAX 2147483647 

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef LPCSTR
typedef const char* LPCSTR;
#endif

#ifndef LPSTR
typedef char* LPSTR;
#endif

//#ifndef
//typedef
//#endif

/////////////////////////////////////////////////////////////////////////////
// CKTString - String class

struct CKTStringData
{
    volatile long nRefs;     // reference count
    int nDataLength;
    int nAllocLength;
    // char data[nAllocLength]

    char* data()
        { return (char*)(this + 1); }
};


class CKTString
{
public:
// Constructors
    CKTString();
    CKTString(const CKTString& stringSrc);
    CKTString(char ch, int nRepeat = 1);
    CKTString(LPCSTR lpsz);
    CKTString(LPCSTR lpch, int nLength);
    CKTString(const unsigned char* psz);

// Attributes & Operations
    // as an array of characters
    int GetLength() const;
    bool IsEmpty() const;
    void Empty();                       // free up the data

    char GetAt(int nIndex) const;      // 0 based
    char operator[](int nIndex) const; // same as GetAt
    void SetAt(int nIndex, char ch);
    operator LPCSTR() const;           // as a C string

    // overloaded assignment
    const CKTString& operator=(const CKTString& stringSrc);
    const CKTString& operator=(char ch);
    const CKTString& operator=(LPCSTR lpsz);
    const CKTString& operator=(const unsigned char* psz);

    // string concatenation
    const CKTString& operator+=(const CKTString& string);
    const CKTString& operator+=(char ch);
    const CKTString& operator+=(LPCSTR lpsz);

    friend CKTString operator+(const CKTString& string1, const CKTString& string2);
    friend CKTString operator+(const CKTString& string, char ch);
    friend CKTString operator+(char ch, const CKTString& string);
    friend CKTString operator+(const CKTString& string, LPCSTR lpsz);
    friend CKTString operator+(LPCSTR lpsz, const CKTString& string);
    friend ostream& operator<<(ostream& os, const CKTString& string);
    friend istream& operator>>(istream& is, CKTString& string);

    // string comparison
    int Compare(LPCSTR lpsz) const;         // straight character
    int CompareNoCase(LPCSTR lpsz) const;   // ignore case

    // simple sub-string extraction
    CKTString Mid(int nFirst, int nCount) const;
    CKTString Mid(int nFirst) const;
    CKTString Left(int nCount) const;
    CKTString Right(int nCount) const;

    CKTString SpanIncluding(LPCSTR lpszCharSet) const;
    CKTString SpanExcluding(LPCSTR lpszCharSet) const;

    // upper/lower/reverse conversion
    void MakeUpper();
    void MakeLower();
    void MakeReverse();

    // trimming whitespace (either side)
    void TrimRight();
    void TrimLeft();

    // remove continuous occurrences of chTarget starting from right
    void TrimRight(char chTarget);
    // remove continuous occcurrences of characters in passed string,
    // starting from right
    void TrimRight(LPCSTR lpszTargets);
    // remove continuous occurrences of chTarget starting from left
    void TrimLeft(char chTarget);
    // remove continuous occcurrences of characters in
    // passed string, starting from left
    void TrimLeft(LPCSTR lpszTargets);

    // advanced manipulation
    // replace occurrences of chOld with chNew
    int Replace(char chOld, char chNew);
    // replace occurrences of substring lpszOld with lpszNew;
    // empty lpszNew removes instances of lpszOld
    int Replace(LPCSTR lpszOld, LPCSTR lpszNew);
    // remove occurrences of chRemove
    int Remove(char chRemove);
    // insert character at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, char ch);
    // insert substring at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, LPCSTR pstr);
    // delete nCount characters starting at zero-based index
    int Delete(int nIndex, int nCount = 1);

    // searching (return starting index, or -1 if not found)
    // look for a single character match
    int Find(char ch) const;                     // like "C" strchr
    int ReverseFind(char ch) const;
    int Find(char ch, int nStart) const;         // starting at index
    int FindOneOf(LPCSTR lpszCharSet) const;

    // look for a specific sub-string
    int Find(LPCSTR lpszSub) const;        // like "C" strstr
    int Find(LPCSTR lpszSub, int nStart) const;  // starting at index

    // Concatentation for non strings
    const CKTString& Append(int n);

    // simple formatting
    int Format(LPCSTR lpszFormat, ...);

    // Access to string implementation buffer as "C" character array
    LPSTR GetBuffer(int nMinBufLength);
    void ReleaseBuffer(int nNewLength = -1);
    LPSTR GetBufferSetLength(int nNewLength);
    void FreeExtra();

    // Use LockBuffer/UnlockBuffer to turn refcounting off
    LPSTR LockBuffer();
    void UnlockBuffer();

// Implementation
public:
    ~CKTString();
    int GetAllocLength() const;

protected:
    LPSTR m_pchData;   // pointer to ref counted string data
#if defined(__GNUC__)
    static ptSpinLock lockrefs;
#endif

    // implementation helpers
    CKTStringData* GetData() const;
    void Init();
    void AllocCopy(CKTString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
    bool AllocBuffer(int nLen);
    void AssignCopy(int nSrcLen, LPCSTR lpszSrcData);
    bool ConcatCopy(int nSrc1Len, LPCSTR lpszSrc1Data, int nSrc2Len, LPCSTR lpszSrc2Data);
    void ConcatInPlace(int nSrcLen, LPCSTR lpszSrcData);
    void CopyBeforeWrite();
    bool AllocBeforeWrite(int nLen);
    void Release();
    static void Release(CKTStringData* pData);
    static int SafeStrlen(LPCSTR lpsz);
    static void FreeData(CKTStringData* pData);
    static const CKTString& _GetEmptyString();

    static inline int safe_inc(int* value)
    {
#if defined(_WIN32)
        return InterlockedIncrement((long*)value);
#elif defined(__GNUC__)
        ptSpinLockAdp lock(&lockrefs);
        return ++(*value);
#else
    #error not support
#endif
    }

    static inline int safe_dec(int* value)
    {
#if defined(_WIN32)
        return InterlockedDecrement((long*)value);
#elif defined(__GNUC__)
        ptSpinLockAdp lock(&lockrefs);
        return --(*value);
#else
    #error not support
#endif
    }
};


// Compare helpers
bool operator==(const CKTString& s1, const CKTString& s2);
bool operator==(const CKTString& s1, LPCSTR s2);
bool operator==(LPCSTR s1, const CKTString& s2);
bool operator!=(const CKTString& s1, const CKTString& s2);
bool operator!=(const CKTString& s1, LPCSTR s2);
bool operator!=(LPCSTR s1, const CKTString& s2);
bool operator<(const CKTString& s1, const CKTString& s2);
bool operator<(const CKTString& s1, LPCSTR s2);
bool operator<(LPCSTR s1, const CKTString& s2);
bool operator>(const CKTString& s1, const CKTString& s2);
bool operator>(const CKTString& s1, LPCSTR s2);
bool operator>(LPCSTR s1, const CKTString& s2);
bool operator<=(const CKTString& s1, const CKTString& s2);
bool operator<=(const CKTString& s1, LPCSTR s2);
bool operator<=(LPCSTR s1, const CKTString& s2);
bool operator>=(const CKTString& s1, const CKTString& s2);
bool operator>=(const CKTString& s1, LPCSTR s2);
bool operator>=(LPCSTR s1, const CKTString& s2);


/////////////////////////////////////////////////////////////////////////////
// CKTString Implementation

inline CKTStringData* CKTString::GetData() const
    { assert(m_pchData != NULL); return ((CKTStringData*)m_pchData) - 1; }
inline void CKTString::Init()
    { m_pchData = _GetEmptyString().m_pchData; }
inline CKTString::CKTString(const unsigned char* lpsz)
    { Init(); *this = (LPCSTR)lpsz; }
inline const CKTString& CKTString::operator=(const unsigned char* lpsz)
    { *this = (LPCSTR)lpsz; return *this; }
inline int CKTString::GetLength() const
    { return GetData()->nDataLength; }
inline int CKTString::GetAllocLength() const
    { return GetData()->nAllocLength; }
inline bool CKTString::IsEmpty() const
    { return GetData()->nDataLength == 0; }
inline CKTString::operator LPCSTR() const
    { return m_pchData; }
inline int CKTString::SafeStrlen(LPCSTR lpsz)
    { return (lpsz == NULL) ? 0 : strlen(lpsz); }
inline CKTString::CKTString() { Init(); }

// CKTString support (windows specific)
inline int CKTString::Compare(LPCSTR lpsz) const
    { return strcmp(m_pchData, lpsz); }
inline int CKTString::CompareNoCase(LPCSTR lpsz) const
    { 
#if defined(_WIN32)
        return _stricmp(m_pchData, lpsz);
#elif defined(__GNUC__)
        return strcasecmp(m_pchData, lpsz);
#endif
    }

inline char CKTString::GetAt(int nIndex) const
{
    assert(nIndex >= 0);
    assert(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}
inline char CKTString::operator[](int nIndex) const
{
    // same as GetAt
    assert(nIndex >= 0);
    assert(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}
inline bool operator==(const CKTString& s1, const CKTString& s2)
    { return s1.Compare(s2) == 0; }
inline bool operator==(const CKTString& s1, LPCSTR s2)
    { return s1.Compare(s2) == 0; }
inline bool operator==(LPCSTR s1, const CKTString& s2)
    { return s2.Compare(s1) == 0; }
inline bool operator!=(const CKTString& s1, const CKTString& s2)
    { return s1.Compare(s2) != 0; }
inline bool operator!=(const CKTString& s1, LPCSTR s2)
    { return s1.Compare(s2) != 0; }
inline bool operator!=(LPCSTR s1, const CKTString& s2)
    { return s2.Compare(s1) != 0; }
inline bool operator<(const CKTString& s1, const CKTString& s2)
    { return s1.Compare(s2) < 0; }
inline bool operator<(const CKTString& s1, LPCSTR s2)
    { return s1.Compare(s2) < 0; }
inline bool operator<(LPCSTR s1, const CKTString& s2)
    { return s2.Compare(s1) > 0; }
inline bool operator>(const CKTString& s1, const CKTString& s2)
    { return s1.Compare(s2) > 0; }
inline bool operator>(const CKTString& s1, LPCSTR s2)
    { return s1.Compare(s2) > 0; }
inline bool operator>(LPCSTR s1, const CKTString& s2)
    { return s2.Compare(s1) < 0; }
inline bool operator<=(const CKTString& s1, const CKTString& s2)
    { return s1.Compare(s2) <= 0; }
inline bool operator<=(const CKTString& s1, LPCSTR s2)
    { return s1.Compare(s2) <= 0; }
inline bool operator<=(LPCSTR s1, const CKTString& s2)
    { return s2.Compare(s1) >= 0; }
inline bool operator>=(const CKTString& s1, const CKTString& s2)
    { return s1.Compare(s2) >= 0; }
inline bool operator>=(const CKTString& s1, LPCSTR s2)
    { return s1.Compare(s2) >= 0; }
inline bool operator>=(LPCSTR s1, const CKTString& s2)
    { return s2.Compare(s1) <= 0; }

#endif

