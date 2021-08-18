#ifndef __PLUGABLE_KEY_H__
#define __PLUGABLE_KEY_H__

/**
  * @Name: PlugableKey.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:04
  * @Description:  the  plugable key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"

#ifndef KT_NO_PLUGABLE_ALGO

class CPlugableKey : public CKey
{
public:
	CPlugableKey(const KTByteArray& rData)
	{
		m_nType=PLUGABLE_KEY;
		m_rValue=rData;
	}
	void encode(KTByteArray& rData)const{doEncode(m_rValue,m_nType,rData);}
	const KTByteArray& getValue()const{return m_rValue;}
	const KTCHAR* getAlgorithmName()const{return NULL;}	
	void hashData(KTByteArray& rHashData)const;

private:
	KTByteArray m_rValue;

};

#endif //#ifndef KT_NO_PLUGABLE_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:06  Hongwei Yu   created 

#endif
