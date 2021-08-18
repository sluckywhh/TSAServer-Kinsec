#ifndef __RSA_PUBLIC_KEY_H__
#define __RSA_PUBLIC_KEY_H__

/**
  * @Name: RSAPublicKey.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:11
  * @Description:  the rsa public key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"
#include "Math.hpp"
#include "RSAPrivateKey.h"

#ifndef KT_NO_RSA_ALGO

class CBigInteger;

class CRSAPublicKey : public CKey
{
public:
	CRSAPublicKey(const KTByteArray& rDERData);
	CRSAPublicKey(const KTBYTE* pbDERData, KTUINT unLen){ construct(pbDERData, unLen); };	
	CRSAPublicKey(const CBigInteger& n,const CBigInteger& e);
	CRSAPublicKey(const CRSAPrivateKey& prvKey){ m_N=prvKey.getN(); m_E=prvKey.getE(); m_nType=CKey::RSA_PUBLIC_KEY; };

	virtual ~CRSAPublicKey();
	const CBigInteger& getN() const {return m_N;}
	const CBigInteger& getE() const {return m_E;}
	void setN(const CBigInteger& n) { m_N = n;}
	void setE(const CBigInteger& e) { m_E = e;}
	
	void encode(KTByteArray& rData)const;
	void hashData(KTByteArray& rHashData)const;
private:
	CBigInteger m_N;
	CBigInteger m_E;
	
	void construct(const KTBYTE* pbDERData, KTUINT unLen);
};

#endif //#ifndef KT_NO_RSA_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:13  Hongwei Yu   created 
///   1.0.0.1  2003/06/03 01:25  Hongwei Yu   support construct with prvkey

#endif
