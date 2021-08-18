#ifndef __IV_PARAMETER_H__
#define __IV_PARAMETER_H__

/**
  * @Name: IVParameter.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:39
  * @Description:  the IV parameter define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "AlgorithmParameter.h"

class CIVParameter 
: public CAlgorithmParameter
{
public:
	CIVParameter() {}
	CIVParameter(const KTByteArray& v){
		m_rIV=v;
	}
	const KTByteArray& getIV() const {return m_rIV;}
	void setIV(const KTByteArray& v) {m_rIV=v;}
private:
	KTByteArray m_rIV;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:41  Hongwei Yu   created 

#endif
