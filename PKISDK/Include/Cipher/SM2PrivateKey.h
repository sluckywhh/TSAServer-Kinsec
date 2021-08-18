#ifndef __SM2_PRIVATE_KEY_H__
#define __SM2_PRIVATE_KEY_H__

/**
  * @Name: SM2PrivateKey.h
  * @Copyright: KTXA SoftWare
  * @Author: 
  * @Date:  2013
  * @Description:  the sm2 private key define
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#include "Key.h"
#include "Math.hpp"
#include "ASN1PKI/pkcs1.h"
#ifndef KT_NO_SM2_ALGO

class KTINTEGER;
class CSM2PublicKey;
class CSM2PrivateKey : public CKey
{
public:
	CSM2PrivateKey(const KTByteArray& rASNData);
	CSM2PrivateKey(const KTBYTE *pbASNData, KTUINT unLen) {construct(pbASNData, unLen);};
	CSM2PrivateKey(const KTINTEGER& d);

	virtual ~CSM2PrivateKey();

	const KTINTEGER& getD() const {return m_d;}
	void setD(const KTINTEGER& d) {m_d.cloneContent(d);}
	
	int getD(unsigned char *out, int *outlen) const;
	int setD(unsigned char *in, int inlen);

	int getD(KTByteArray& out) const;
	int setD(KTByteArray& in);

	void encode(KTByteArray& rData) const;
	void hashData(KTByteArray& rHashData) const;
	void exportPubKey(CSM2PublicKey& pubKey) const;

	KTSM2PrivateKey m_d;

private:
	void construct(const KTBYTE *pbDERData, KTUINT unLen);
};

#endif //#ifndef KT_NO_SM2_ALGO

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

#endif
