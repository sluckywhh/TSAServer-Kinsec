#ifndef __RSA_PRIVATE_KEY_H__
#define __RSA_PRIVATE_KEY_H__

/**
  * @Name: RSAPrivateKey.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:10
  * @Description:  the rsa private key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"
#include "Math.hpp"
#ifndef KT_NO_RSA_ALGO

class CBigInteger;
class CRSAPublicKey;
class CRSAPrivateKey : public CKey
{
public:
	CRSAPrivateKey(const KTByteArray& rASNData);
	CRSAPrivateKey(const KTBYTE* pbASNData, KTUINT unLen){ construct(pbASNData, unLen); };		
	CRSAPrivateKey(const CBigInteger& n,const CBigInteger& e,
		const CBigInteger& d,const CBigInteger& p,const CBigInteger&q,
		const CBigInteger& p1,const CBigInteger& q1,const CBigInteger& dmq);


	virtual ~CRSAPrivateKey();
	const CBigInteger& getN() const {return m_N;}
	const CBigInteger& getE() const {return m_E;}
	const CBigInteger& getD() const {return m_D;}
	const CBigInteger& getP() const {return m_P;}
	const CBigInteger& getQ() const {return m_Q;}
	const CBigInteger& getP1() const {return m_P1;}
	const CBigInteger& getQ1() const {return m_Q1;}
	const CBigInteger& getDMQ() const {return m_DMQ;}
	
	void encode(KTByteArray& rData)const;
	void hashData(KTByteArray& rHashData)const;
	void exportPubKey(CRSAPublicKey& pubKey)const;
private:
	CBigInteger m_N;
	CBigInteger m_E;
	CBigInteger m_D;
	CBigInteger m_P;
	CBigInteger m_Q;
	CBigInteger m_P1;
	CBigInteger m_Q1;
	CBigInteger m_DMQ;
	
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
///   1.0.0.0  2003/04/26 10:11  Hongwei Yu   created 
///   1.0.0.1  2003/06/03 00:54  Hongwei Yu   support export pubkey
///											  it's should to use KTRSAPrivateKey and KTInteger and support +- etc 
///											  operators in KTInteger to avoid lower efficiency 

#endif
