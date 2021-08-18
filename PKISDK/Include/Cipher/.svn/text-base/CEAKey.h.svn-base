/*
 * File:	CCEAKey.h
 *
 * Purpose: -
 *
 * Date:    02.07.2001
 *
 * Author: guw
 *
 * Revision History:-
 *
 * Copyright (C) 1998-2001, KOAL, Inc.
 *
 */

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CCEAKEY_3B38B6BB03DD_INCLUDED
#define _INC_CCEAKEY_3B38B6BB03DD_INCLUDED

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "CKey.h"

#ifndef KT_NO_CEA_ALGO


class CCEAKey : public CKey
{
public:
	CCEAKey(const KTByteArray& rData)
	{
		m_nType=CEA_KEY;
		m_rValue=rData;
	}
	void encode(KTByteArray& rData)const{doEncode(m_rValue,m_nType,rData);}
	const KTByteArray& getValue(){return m_rValue;}
	const KTCHAR* getAlgorithmName(){return s_pAlgoName;}	
	void hashData(KTByteArray& rHashData);
private:
	KTByteArray m_rValue;
	static const char s_pAlgoName[];
};

#endif //#ifndef KT_NO_CEA_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

#endif /* _INC_CCEAKEY_3B38B6BB03DD_INCLUDED */
