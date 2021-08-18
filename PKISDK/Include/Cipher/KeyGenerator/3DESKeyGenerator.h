#ifndef __3DES_KEY_GENERATOR_H__
#define __3DES_KEY_GENERATOR_H__

/**
  * @Name: 3DESKeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:14
  * @Description:  the 3des key generator define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_3DES_ALGO

class CAlgorithmParameter;
class CKey;

class C3DESKeyGenerator : public IKeyGeneratorImpl<C3DESKeyGenerator>
{
public:
	C3DESKeyGenerator();

	~C3DESKeyGenerator();

	KTINT32 init(CAlgorithmParameter* pParam);

	KTINT32 generate(CKey ** pKey);
private:
	enum{
		KEY_SIZE	=24,
	};
	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_3DES_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:15  Hongwei Yu   created 

#endif
