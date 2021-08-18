#ifndef __RC4_KEY_GENERATOR_H__
#define __RC4_KEY_GENERATOR_H__

/**
  * @Name: RC4KeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:35
  * @Description:  the RC4 key generator define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_RC4_ALGO
class CKey;
class CAlgorithmParameter;

class CRC4KeyGenerator : public IKeyGeneratorImpl<CRC4KeyGenerator>
{
public:
	~CRC4KeyGenerator();

	CRC4KeyGenerator();

	KTINT32 init(CAlgorithmParameter* pParam);

	KTINT32 generate(CKey ** pKey);

private:
	enum{
		KEY_SIZE	=16,
	};

	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_RC4_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:35  Hongwei Yu   created 

#endif 
