#ifndef __RSA_KEY_GENERATOR_H__
#define __RSA_KEY_GENERATOR_H__

/**
  * @Name: RSAKeyGenerator.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:36
  * @Description:  the RSA key generator define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#ifndef KT_NO_RSA_ALGO

class CKey;
class CAlgorithmParameter;


class CRSAKeyGenerator : public IKeyPairGeneratorImpl<CRSAKeyGenerator>
{
public:
	CRSAKeyGenerator();

	~CRSAKeyGenerator();

	KTINT32 init(CAlgorithmParameter* pParam);
	KTINT32 generate(CKey** ppPubKey, CKey** ppPrvKey);

protected:
	virtual KTINT32 doGenerate(int nBits,CKey** ppPubKey, CKey** ppPrvKey);
private:
	int		m_nBits;
	KT_DECLARE_INTERFACE()
};

#endif //#ifndef KT_NO_RSA_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:37  Hongwei Yu   created 

#endif 
