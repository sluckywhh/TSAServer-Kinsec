#ifndef __3DES_KEY_H__
#define __3DES_KEY_H__

/**
  * @Name: 3DESKey.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:00
  * @Description:  the 3DES Key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"

#ifndef KT_NO_3DES_ALGO


class C3DESKey : public CKey
{
public:	
	
	
	C3DESKey(const KTByteArray& rData)
	{
		m_nType=DES3_KEY;
		m_rValue=rData;
	}
	void encode(KTByteArray& rData)const{doEncode(m_rValue,m_nType,rData);}
	const KTByteArray& getValue()const{return m_rValue;}
	const KTCHAR* getAlgorithmName()const{return s_pAlgoName;}
	void hashData(KTByteArray& rHashData)const;
	
private:
	KTByteArray m_rValue;
	static const char s_pAlgoName[];	
};

#endif //#ifndef KT_NO_3DES_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:01  Hongwei Yu   created 

#endif
