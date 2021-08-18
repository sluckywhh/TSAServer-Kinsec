#ifndef __KEY_H__
#define __KEY_H__

/**
  * @Name: Key.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/26 10:03
  * @Description:  the base class for key
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#ifdef KT_WIN32
	#pragma pack(push,1)
#endif

#define KEY_HASH_DATA_LEN		16
#define MAKE_KEY_HASH(nInput,nInputLen,pOutput,pnOutput)	K_MD5_Digest(nInput,nInputLen,pOutput,pnOutput)
class CKey 
{
public:
	enum KEY_TYPE{
		RSA_PUBLIC_KEY	=0x80101,
		RSA_PRIVATE_KEY	=0x80201,

		DES_KEY		=0x80302,
		DES3_KEY	=0x80303,
		RC2_KEY		=0x80304,
		RC4_KEY		=0x80305,
		SM4_KEY		=0x80306,
		
		PLUGABLE_KEY	=0x803f0,

		SM2_PUBLIC_KEY	=0x80801,
		SM2_PRIVATE_KEY	=0x80802,
	};
	virtual const KTCHAR* getAlgorithmName(){return NULL;};
	
	virtual void encode(KTByteArray& rData)const=0;
	
	static CKey* getInstance(KEY_TYPE nType,const KTBYTE* pDERData,KTUINT nDERDataLen);

	virtual void hashData(KTByteArray& rHashData)const=0;
	
	int getKeyType()const{
		return m_nType;
	}
	void release();
protected:
	KEY_TYPE	m_nType;
	void doEncode(const KTByteArray& rValue,KEY_TYPE nType,KTByteArray& rData)const;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/26 10:04  Hongwei Yu   created 

#endif 
