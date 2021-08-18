#ifndef __KTTIME_H__
#define __KTTIME_H__

/**
  * @Name: KTTIME.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/25 23:46
  * @Description: some TIME class  
*/

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include <time.h>
//#include "BaseType.h"
//#include "CString.h"

bool ks_is_leap_year(int year);
struct tm * ks_nolocks_localtime(struct tm *tmp, time_t t, time_t tz, int dst);
struct tm * ks_nolocks_gmtime(struct tm *tmp, time_t t, time_t tz, int dst);
int ks_localtime_s(struct tm * _Tm, const time_t * _Time);
int ks_gmtime_s(struct tm * _Tm, const time_t * _Time);

typedef struct _KTSYSTEMTIME {
    KTUINT32 nYear;
    KTUINT32 nMonth;
    KTUINT32 nDayOfWeek;
    KTUINT32 nDay;
    KTUINT32 nHour;
    KTUINT32 nMinute;
    KTUINT32 nSecond;
    KTUINT32 nMilliseconds;
} KTSYSTEMTIME;

void KTGetSystemTime(KTSYSTEMTIME* lpSystemTime); 
class KT_LIB_MODE KTTIMESpan
{
public:

// Constructors
	KTTIMESpan();
	KTTIMESpan(time_t time);
	KTTIMESpan(KTLONG lDays, int nHours, int nMins, int nSecs);

	KTTIMESpan(const KTTIMESpan& timeSpanSrc);
	const KTTIMESpan& operator=(const KTTIMESpan& timeSpanSrc);

// Attributes
	// extract parts
	KTLONG GetDays() const;   // total # of days
	KTLONG GetTotalHours() const;
	int GetHours() const;
	KTLONG GetTotalMinutes() const;
	int GetMinutes() const;
	KTLONG GetTotalSeconds() const;
	int GetSeconds() const;

// Operations
	// time math
	KTTIMESpan operator-(KTTIMESpan timeSpan) const;
	KTTIMESpan operator+(KTTIMESpan timeSpan) const;
	const KTTIMESpan& operator+=(KTTIMESpan timeSpan);
	const KTTIMESpan& operator-=(KTTIMESpan timeSpan);
	KTBOOL operator==(KTTIMESpan timeSpan) const;
	KTBOOL operator!=(KTTIMESpan timeSpan) const;
	KTBOOL operator<(KTTIMESpan timeSpan) const;
	KTBOOL operator>(KTTIMESpan timeSpan) const;
	KTBOOL operator<=(KTTIMESpan timeSpan) const;
	KTBOOL operator>=(KTTIMESpan timeSpan) const;

private:
	time_t m_timeSpan;
	friend class KTTIME;
};

class KT_LIB_MODE KTTIME
{
public:

// Constructors
	static KTTIME GetCurrentTime();

	KTTIME();
	KTTIME(time_t time);
	KTTIME(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
		int nDST = -1);
	KTTIME(const KTTIME& timeSrc);
	KTTIME(const KTSYSTEMTIME& sysTime, int nDST = -1);

	const KTTIME& operator=(const KTTIME& timeSrc);
	const KTTIME& operator=(time_t t);

// Attributes
	time_t GetTime() const;
	int GetYear() const;
	int GetMonth() const;       // month of year (1 = Jan)
	int GetDay() const;         // day of month
	int GetHour() const;
	int GetMinute() const;
	int GetSecond() const;
	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat

// Operations
	// time math
	KTTIMESpan operator-(KTTIME time) const;
	KTTIME operator-(KTTIMESpan timeSpan) const;
	KTTIME operator+(KTTIMESpan timeSpan) const;
	const KTTIME& operator+=(KTTIMESpan timeSpan);
	const KTTIME& operator-=(KTTIMESpan timeSpan);
	KTBOOL operator==(KTTIME time) const;
	KTBOOL operator!=(KTTIME time) const;
	KTBOOL operator<(KTTIME time) const;
	KTBOOL operator>(KTTIME time) const;
	KTBOOL operator<=(KTTIME time) const;
	KTBOOL operator>=(KTTIME time) const;

	// formatting using "C" strftime
	KTString Format(KTCHAR* pFormat) const;
	KTString Format(KTUINT nFormatID) const;
//	KTBOOL ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags = 0 );

private:
	time_t m_time;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/25 23:10  Hongwei Yu   created 

#endif 
