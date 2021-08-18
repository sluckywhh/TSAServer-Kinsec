#ifndef __RSA_PARAMETER_H__
#define __RSA_PARAMETER_H__

/**
  * @Name: RSAParameter.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:43
  * @Description:  the RSA parameter define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "AlgorithmParameter.h"

#ifndef KT_NO_RSA_ALGO
class CRSAParameter : public CAlgorithmParameter
{
public:
	enum KEY_BITS{
		KEY_512_BITS	=512,
		KEY_768_BITS	=768,
		KEY_1024_BITS	=1024,
		KEY_2048_BITS	=2048,
		KEY_4096_BITS	=4096
	};
	CRSAParameter(KEY_BITS bits=KEY_1024_BITS){
		m_nBits=bits;
	}
	KEY_BITS getBits() const{
		return m_nBits;
	}
private:
	KEY_BITS m_nBits;
};
#endif //KT_NO_RSA_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:44  Hongwei Yu   created 

#endif
