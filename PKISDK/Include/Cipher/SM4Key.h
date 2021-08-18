#ifndef __SM4_KEY_H__
#define __SM4_KEY_H__

/**
  * @Name: SM4Key.h
  * @Copyright: KTXA SoftWare
  * @Description:  the SM4 Key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"

#ifndef KT_NO_SM4_ALGO
class CSM4Key : public CKey
{
public:
	CSM4Key(const KTByteArray& rData)
	{
		m_nType = SM4_KEY;
		m_rValue = rData;
	}
	void encode(KTByteArray& rData)const{doEncode(m_rValue, m_nType, rData);}
	const KTByteArray& getValue()const{return m_rValue;}
	void hashData(KTByteArray& rHashData)const;
private:
	KTByteArray m_rValue;

};
#endif //#ifndef KT_NO_SM4_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

#endif
