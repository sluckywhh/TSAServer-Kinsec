#ifndef __DES_KEY_GENERATOR_H__
#define __DES_KEY_GENERATOR_H__

/**
  * @Name: DESKeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 03:49
  * @Description:  the des key generator define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_DES_ALGO

class CKey;
class CAlgorithmParameter;

class CDESKeyGenerator : public IKeyGeneratorImpl<CDESKeyGenerator>
{
public:
	CDESKeyGenerator();

	~CDESKeyGenerator();

	KTINT32 init(CAlgorithmParameter* pParam);

	KTINT32 generate(CKey ** pKey);
private:
	enum{
		KEY_SIZE	=8,
	};
	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_DES_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:17  Hongwei Yu   created 

#endif 
