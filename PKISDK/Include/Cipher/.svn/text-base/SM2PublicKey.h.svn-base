#ifndef __SM2_PUBLIC_KEY_H__
#define __SM2_PUBLIC_KEY_H__

/**
  * @Name: SM2PublicKey.h
  * @Copyright: KTXA SoftWare
  * @Author: 
  * @Date:  2013
  * @Description:  the sm2 public key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"
#include "Math.hpp"
#include "SM2PrivateKey.h"
#include "ASN1PKI/pkcs1.h"

#ifndef KT_NO_SM2_ALGO

class KTINTEGER;

class CSM2PublicKey : public CKey
{
public:
	CSM2PublicKey(const KTByteArray& rDERData);
	CSM2PublicKey(const KTBYTE *pbDERData, KTUINT unLen) {construct(pbDERData, unLen);};
 	CSM2PublicKey(const CSM2PrivateKey& prvKey);

	virtual ~CSM2PublicKey();

	int getX(unsigned char *out, int *outlen) const;
	int getY(unsigned char *out, int *outlen) const;

	int getX(KTByteArray& out) const;
	int getY(KTByteArray& out) const;

	void encode(KTByteArray& rData) const;
	void hashData(KTByteArray& rHashData) const;

	KTSM2PublicKey m_q;

private:
	void construct(const KTBYTE *pbDERData, KTUINT unLen);
};

#endif //#ifndef KT_NO_SM2_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

#endif
