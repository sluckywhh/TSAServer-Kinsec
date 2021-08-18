#ifndef __RC2_KEY_H__
#define __RC2_KEY_H__

/**
  * @Name: RC2Key.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:06
  * @Description:  the RC2 Key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"

#ifndef KT_NO_RC2_ALGO
class CRC2Key : public CKey
{
public:
	CRC2Key(const KTByteArray& rData)
	{
		m_nType=RC2_KEY;
		m_rValue=rData;
	}
	void encode(KTByteArray& rData)const{doEncode(m_rValue,m_nType,rData);}
	const KTByteArray& getValue()const{return m_rValue;}
	void hashData(KTByteArray& rHashData)const;
private:
	KTByteArray m_rValue;

};
#endif //#ifndef KT_NO_RC2_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:07  Hongwei Yu   created 

#endif
