#ifdef _WIN32
#include "stdafx.h"
#endif
#include "FJCAApiConst.h"
#include "pk7_sm2_sign.h"

#include "MySMData.h"
#include "MySMLibFunc.h"
#include "MyToolLibFunc.h"
#include "MailCoder.h"

#include <vector>
#include <string>

typedef std::string			String;
typedef std::vector<char>	CharArray;
typedef std::vector<BYTE>	ByteArray;

////////////////////////////////////////////////////////////////
// PKCS7_SM2数据结构

// 数据
ASN1_SEQUENCE(PKCS7_SM2_DATA) = {
	ASN1_SIMPLE(PKCS7_SM2_DATA, type, ASN1_OBJECT),
	ASN1_EXP_OPT(PKCS7_SM2_DATA, data, ASN1_OCTET_STRING, 0)
} ASN1_SEQUENCE_END(PKCS7_SM2_DATA)

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_SM2_DATA)

// 签名数据
ASN1_SEQUENCE(PKCS7_SM2_SIGNED_DATA) = {
	ASN1_SIMPLE(PKCS7_SM2_SIGNED_DATA, version, ASN1_INTEGER),
	ASN1_SET_OF(PKCS7_SM2_SIGNED_DATA, digestAlgorithms, X509_ALGOR),
	ASN1_SIMPLE(PKCS7_SM2_SIGNED_DATA, contentInfo, PKCS7_SM2_DATA),
	ASN1_IMP_SEQUENCE_OF_OPT(PKCS7_SM2_SIGNED_DATA, certificates, X509, 0),
	ASN1_IMP_SET_OF_OPT(PKCS7_SM2_SIGNED_DATA, crls, X509_CRL, 1),
	ASN1_SET_OF(PKCS7_SM2_SIGNED_DATA, signerInfos, PKCS7_SIGNER_INFO)
} ASN1_SEQUENCE_END(PKCS7_SM2_SIGNED_DATA)

ASN1_SEQUENCE(PKCS7_SM2_SIGNATURE) = {
	ASN1_SIMPLE(PKCS7_SM2_SIGNATURE, type, ASN1_OBJECT),
	ASN1_EXP(PKCS7_SM2_SIGNATURE, signedData, PKCS7_SM2_SIGNED_DATA, 0)
} ASN1_SEQUENCE_END(PKCS7_SM2_SIGNATURE)

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_SM2_SIGNED_DATA)
IMPLEMENT_ASN1_FUNCTIONS(PKCS7_SM2_SIGNATURE)

////////////////////////////////////////////////////////////////
X509_ALGOR *CreateAlgorFromType(
	ASN1_OBJECT *type,
	unsigned char *parameter,
	int parameterlen)
{
	int nRet = 0;

	X509_ALGOR *algor = X509_ALGOR_new();
	if (algor == NULL)
		return NULL;

	// set algorithm
	if (algor->algorithm != NULL)
		ASN1_OBJECT_free(algor->algorithm);
	algor->algorithm = type;

	// set parameter
	if (algor->parameter == NULL)
		algor->parameter = ASN1_TYPE_new();
 	if (algor->parameter == NULL)
		goto EXIT;

	if (parameterlen == 0)
		ASN1_TYPE_set(algor->parameter, V_ASN1_NULL, NULL);
	else
	{
		algor->parameter->type = V_ASN1_OCTET_STRING;
		algor->parameter->value.octet_string = ASN1_OCTET_STRING_new();
		if (algor->parameter->value.octet_string == NULL)
			goto EXIT;

		if (!ASN1_OCTET_STRING_set(algor->parameter->value.octet_string, parameter, parameterlen))
			goto EXIT;
	}

	nRet = 1;

EXIT:
	if (!nRet)
	{
		X509_ALGOR_free(algor);
		algor = NULL;
	}

	return algor;
}

X509_ALGOR *CreateAlgor(
	unsigned char *algorithm,
	int algorithmlen,
	unsigned char *parameter,
	int parameterlen)
{
	unsigned char *pbTmp = algorithm;
	ASN1_OBJECT *type = d2i_ASN1_OBJECT(NULL, (const unsigned char **)&pbTmp, algorithmlen);
	if (type == NULL)
		return NULL;

	return CreateAlgorFromType(type, parameter, parameterlen);
}

X509_ALGOR *CreateAlgor(
	char *algorithm,
	unsigned char *parameter,
	int parameterlen)
{
	// no_name为1，使用数字形式。
	ASN1_OBJECT *type = OBJ_txt2obj(algorithm, 1);
	if (type == NULL)
		return NULL;

	return CreateAlgorFromType(type, parameter, parameterlen);
}

X509_ALGOR *CreateAlgor(
	int algorithm,
	unsigned char *parameter,
	int parameterlen)
{
	ASN1_OBJECT *type = OBJ_nid2obj(algorithm);
	if (type == NULL)
		return NULL;

	return CreateAlgorFromType(type, parameter, parameterlen);
}

int PKCS7_SM2_SIGNATURE_add_certificate(PKCS7_SM2_SIGNATURE *p7, X509 *x509)
{
	STACK_OF(X509) **sk;
	sk = &(p7->signedData->certificates);
	if (*sk == NULL)
		*sk = sk_X509_new_null();
	if (*sk == NULL)
		return 0;

	CRYPTO_add(&x509->references, 1, CRYPTO_LOCK_X509);
	if (!sk_X509_push(*sk, x509))
	{
		X509_free(x509);
		return 0;
	}

	return 1;
}

int PKCS7_SM2_SIGNATURE_add_mdalgs(PKCS7_SM2_SIGNATURE *p7, X509_ALGOR *alg)
{
	STACK_OF(X509_ALGOR) **sk;
	sk = &(p7->signedData->digestAlgorithms);
	if (*sk == NULL)
		*sk = sk_X509_ALGOR_new_null();
	if (*sk == NULL)
		return 0;

	X509_ALGOR *alg2 = X509_ALGOR_dup(alg);
	if (alg2 == NULL)
		return 0;

	if (!sk_X509_ALGOR_push(*sk, alg2))
	{
		X509_ALGOR_free(alg2);
		return 0;
	}

	return 1;
}

int PKCS7_SIGNER_INFO_set_sm2(PKCS7_SIGNER_INFO *p7i, X509 *x509,
	X509_ALGOR *digest_alg, X509_ALGOR *digest_enc_alg, unsigned char *sign, int signlen)
{
	// set version
	if (!ASN1_INTEGER_set(p7i->version, 1))
		return 0;

	// set issuer
	if (!X509_NAME_set(&p7i->issuer_and_serial->issuer,
			X509_get_issuer_name(x509)))
		return 0;

	// set sn
	if (p7i->issuer_and_serial->serial != NULL)
		M_ASN1_INTEGER_free(p7i->issuer_and_serial->serial);
	if (!(p7i->issuer_and_serial->serial =
			M_ASN1_INTEGER_dup(X509_get_serialNumber(x509))))
		return 0;

	// set algorithms
	if (p7i->digest_alg != NULL)
		X509_ALGOR_free(p7i->digest_alg);
	if (!(p7i->digest_alg = X509_ALGOR_dup(digest_alg)))
		return 0;

	if (p7i->digest_enc_alg != NULL)
		X509_ALGOR_free(p7i->digest_enc_alg);
	if (!(p7i->digest_enc_alg = X509_ALGOR_dup(digest_enc_alg)))
		return 0;

	// set sign data
	if (!ASN1_OCTET_STRING_set(p7i->enc_digest, sign, signlen))
		return 0;

	return 1;
}

int PKCS7_SM2_SIGNATURE_add_signer(PKCS7_SM2_SIGNATURE *p7, PKCS7_SIGNER_INFO *psi)
{
	STACK_OF(PKCS7_SIGNER_INFO) **sk;
	sk = &(p7->signedData->signerInfos);
	if (*sk == NULL)
		*sk = sk_PKCS7_SIGNER_INFO_new_null();
	if (*sk == NULL)
		return 0;

	if (!sk_PKCS7_SIGNER_INFO_push(*sk, psi))
		return 0;

	return 1;
}

int Sign_SM3SM2(
	unsigned char *pbIn,
	int nInLen,
	unsigned char *pbCert,
	int nCertLen,
	unsigned char *pbKey,
	int nKeyLen,
	FUNC_PRIV_ENC funcPrivEnc,
	unsigned char *pbSign,
	int *pnSignLen)
{
	// get public key from ecc cert
	unsigned char byPubKey_x[256], byPubKey_y[256];
	memset(byPubKey_x, 0, sizeof(byPubKey_x));
	memset(byPubKey_y, 0, sizeof(byPubKey_y));
	int nPubKeyLen_x, nPubKeyLen_y;

	// (int *)指针强转为(long *)在部分系统（如64位linux系统）有问题
	long lPubKeyLen_x, lPubKeyLen_y;
	lPubKeyLen_x = 0;
	lPubKeyLen_y = 0;
	long lRet = MYTOOL_ECCGetPubKeyFromCert(
		pbCert,
		nCertLen,
		byPubKey_x,
		&lPubKeyLen_x,
		byPubKey_y,
		&lPubKeyLen_y);
	if (lRet != SAR_OK)
		return 0;
	nPubKeyLen_x = (int)lPubKeyLen_x;
	nPubKeyLen_y = (int)lPubKeyLen_y;

	if ((nPubKeyLen_x != nPubKeyLen_y) ||
		(nPubKeyLen_x != SGD_SIZE_ECC && nPubKeyLen_x != SGD_SIZE_ECC_64))
		return 0;
	int nSM2Len;
	nSM2Len = nPubKeyLen_x;

	// user id
	char szUserID[32];
	memset(szUserID, 0, sizeof(szUserID));
	strcpy(szUserID, SGD_STR_ECCUSERID);
	int nUserIDLen;
	nUserIDLen = strlen(szUserID);

	// 预处理1
	BYTE byZ[128];
	memset(byZ, 0, sizeof(byZ));
	int nZLen;
	nZLen = sizeof(byZ);

	// (int *)指针强转为(long *)在部分系统（如64位linux系统）有问题
	long lZLen = nZLen;
	lRet = MYTOOL_ECCGetZ(
		(BYTE *)szUserID,
		nUserIDLen,
		byPubKey_x,
		byPubKey_y,
		byZ,
		&lZLen,
		nSM2Len);
	if (lRet != SAR_OK)
		return 0;
	nZLen = (int)lZLen;

	// 预处理2
	BYTE byH[128];
	memset(byH, 0, sizeof(byH));
	int nHLen;
	nHLen = sizeof(byH);

	// (int *)指针强转为(long *)在部分系统（如64位linux系统）有问题
	long lHLen = nHLen;
	lRet = MYTOOL_ECCGetH(
		byZ,
		nZLen,
		pbIn,
		nInLen,
		byH,
		&lHLen);
	if (lRet != SAR_OK)
		return 0;
	nHLen = (int)lHLen;

	// 签名
	BYTE byTmp[SGD_SIZE_RSADATA];
	memset(byTmp, 0, sizeof(byTmp));
	int nTmpLen;
	nTmpLen = sizeof(byTmp);
	if (nKeyLen > 0)
	{
		if (nKeyLen != nSM2Len)
			return 0;

		lRet = MYSM_ECCSign(
			byH,
			nHLen,
			byTmp,
			byTmp + nSM2Len,
			pbKey,
			nSM2Len);
		if (lRet != MYSMERR_SUCCESS)
			return 0;

		nTmpLen = nSM2Len * 2;
	}
	else
	{
		if (funcPrivEnc == NULL)
			return 0;

		nTmpLen = funcPrivEnc(
			nHLen,
			byH,
			byTmp,
			NULL,
			0);
		if (nTmpLen <= 0)
			return 0;
	}

	// 签名转为国密标准格式
	if (nTmpLen == nSM2Len * 2)
	{
		// (int *)指针强转为(long *)在部分系统（如64位linux系统）有问题
		long lSignLen = (*pnSignLen);
		lRet = MYTOOL_SM2SignDataEncode(
			byTmp,
			nSM2Len,
			byTmp + nSM2Len,
			nSM2Len,
			pbSign,
			&lSignLen);
		if (lRet != SAR_OK)
			return 0;
		*pnSignLen = (int)lSignLen;
	}
	else
	{
		memcpy(pbSign, byTmp, nTmpLen);
		*pnSignLen = nTmpLen;
	}

	return 1;
}

// PKCS7签名
long MYSIGN_SignDataByP7_SM2(
	unsigned char *pbIn,				// 输入数据
	long lInLen,						// 输入数据的长度
	char *pszCert,						// 签名证书
	char *pszKey,						// 签名私钥（若使用硬件设备，应为NULL）
	FUNC_PRIV_ENC funcPrivEnc,			// 私钥加密函数（若使用软私钥，应为NULL）
	char *pszOut,						// 输出数据
	bool bAttached)						// 是否包含原文
{
	long lRet = SAR_OK;
	ByteArray rBuff, rBuff2;
	long lBuffLen, lBuff2Len;

	unsigned char *pbTmp;
	unsigned char data[10240];
	long datalen;

	ASN1_OBJECT *type = NULL;
	X509_ALGOR *digest_alg = NULL, *digest_enc_alg = NULL;
	X509 *signcert = NULL;

	PKCS7_SIGNER_INFO *si = NULL;
	PKCS7_SM2_SIGNATURE *p7 = NULL;

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// cert
	unsigned char byCert[SGD_SIZE_CERT];
	memset(byCert, 0, sizeof(byCert));
	long lCertLen = CMailCoder::base64_decode(pszCert, strlen(pszCert), (char *)byCert);
	if (lCertLen <= 0)
		return SAR_BASE64ENCODEDECODEErr;

	pbTmp = byCert;
	signcert = d2i_X509(NULL, (const unsigned char **)&pbTmp, lCertLen);
	if (signcert == NULL)
		return SAR_CERTENCODEErr;

	// key
	unsigned char byPrvKey[SGD_SIZE_CERT];
	memset(byPrvKey, 0, sizeof(byPrvKey));
	long lPrvKeyLen;
	lPrvKeyLen = 0;
	if (pszKey != NULL && strlen(pszKey) > 0)
	{
		// Base64 decode
		memset(data, 0, sizeof(data));
		datalen = CMailCoder::base64_decode((char *)pszKey, strlen(pszKey), (char *)data);
		if (datalen <= 0)
		{
			lRet = SAR_BASE64ENCODEDECODEErr;
			goto EXIT;
		}

		lRet = MYTOOL_SM2PrivateKeyDecode(data, datalen, byPrvKey, &lPrvKeyLen);
		if (lRet != SAR_OK)
			goto EXIT;
	}

	// digest algorithm
//	digest_alg = CreateAlgor((unsigned char *)g_bySM3_2, sizeof(g_bySM3_2), NULL, 0);
	digest_alg = CreateAlgor(TXT_gm_sm3_2, NULL, 0);
	if (digest_alg == NULL)
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	// sign algorithm
//	digest_enc_alg = CreateAlgor((unsigned char *)g_bySM2_1, sizeof(g_bySM2_1), NULL, 0);
	digest_enc_alg = CreateAlgor(TXT_gm_sm2_1, NULL, 0);
	if (digest_enc_alg == NULL)
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	p7 = PKCS7_SM2_SIGNATURE_new();
	if (p7 == NULL)
	{
		lRet = SAR_MemoryErr;
		goto EXIT;
	}

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// type
// 	memset(data, 0, sizeof(data));
// 	datalen = sizeof(g_bySM2SignedData);
// 	memcpy(data, g_bySM2SignedData, datalen);
// 
// 	pbTmp = data;
// 	type = d2i_ASN1_OBJECT(NULL, (const unsigned char **)&pbTmp, datalen);
// 	if (type == NULL)
// 	{
// 		lRet = SAR_ObjErr;
// 		goto EXIT;
// 	}

	type = OBJ_txt2obj(TXT_gm_signed_data, 1);
	if (type == NULL)
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	if (p7->type != NULL)
		ASN1_OBJECT_free(p7->type);
 	p7->type = type;

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// signed data

	////////////////////////////////////////////////////
	// version
	if (!ASN1_INTEGER_set(p7->signedData->version, 1))
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	////////////////////////////////////////////////////
	// digest algorithm
	if (!PKCS7_SM2_SIGNATURE_add_mdalgs(p7, digest_alg))
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	////////////////////////////////////////////////////
	// content info
	if (p7->signedData->contentInfo == NULL)
		p7->signedData->contentInfo = PKCS7_SM2_DATA_new();
	if (p7->signedData->contentInfo == NULL)
	{
		lRet = SAR_MemoryErr;
		goto EXIT;
	}

	// content type
// 	memset(data, 0, sizeof(data));
// 	datalen = sizeof(g_bySM2Data);
// 	memcpy(data, g_bySM2Data, datalen);
// 
// 	pbTmp = data;
// 	type = d2i_ASN1_OBJECT(NULL, (const unsigned char **)&pbTmp, datalen);
// 	if (type == NULL)
// 	{
// 		lRet = SAR_ObjErr;
// 		goto EXIT;
// 	}

	type = OBJ_txt2obj(TXT_gm_data, 1);
	if (type == NULL)
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	if (p7->signedData->contentInfo->type != NULL)
		ASN1_OBJECT_free(p7->signedData->contentInfo->type);
 	p7->signedData->contentInfo->type = type;

	if (bAttached)
	{
		// content data
		p7->signedData->contentInfo->data = ASN1_OCTET_STRING_new();
		if (p7->signedData->contentInfo->data == NULL)
		{
			lRet = SAR_ObjErr;
			goto EXIT;
		}

		if (!ASN1_OCTET_STRING_set(p7->signedData->contentInfo->data, pbIn, lInLen))
		{
			lRet = SAR_ObjErr;
			goto EXIT;
		}
	}

	////////////////////////////////////////////////////
	// sign cert
	if (!PKCS7_SM2_SIGNATURE_add_certificate(p7, signcert))
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	////////////////////////////////////////////////////
	// signer info

	// sign data
	unsigned char bySign[SGD_SIZE_RSADATA];
	memset(bySign, 0, sizeof(bySign));
	long lSignLen;
	lSignLen = sizeof(bySign);
	if (!Sign_SM3SM2(
		pbIn,
		lInLen,
		byCert,
		lCertLen,
		byPrvKey,
		lPrvKeyLen,
		funcPrivEnc,
		bySign,
		(int *)&lSignLen))
	{
		lRet = SAR_ECCSIGNErr;
		goto EXIT;
	}

	// signer info
	si = PKCS7_SIGNER_INFO_new();
	if (si == NULL)
	{
		lRet = SAR_MemoryErr;
		goto EXIT;
	}

	if (!PKCS7_SIGNER_INFO_set_sm2(si, signcert, digest_alg, digest_enc_alg, bySign, lSignLen))
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	if (!PKCS7_SM2_SIGNATURE_add_signer(p7, si))
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	si = NULL;

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	// output data
	lBuffLen = i2d_PKCS7_SM2_SIGNATURE(p7, NULL);
	if (lBuffLen <= 0)
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	rBuff.resize(lBuffLen);
	memset(&rBuff[0], 0, lBuffLen);
	pbTmp = &rBuff[0];
	lBuffLen = i2d_PKCS7_SM2_SIGNATURE(p7, &pbTmp);
	if (lBuffLen <= 0)
	{
		lRet = SAR_ObjErr;
		goto EXIT;
	}

	// Base64 encode
	lBuff2Len = lBuffLen * 2 + 10;
	rBuff2.resize(lBuff2Len);
	memset(&rBuff2[0], 0, lBuff2Len);
	lBuff2Len = CMailCoder::base64_encode((char *)&rBuff[0], lBuffLen, (char *)&rBuff2[0]);
	if (lBuff2Len <= 0)
	{
		lRet = SAR_BASE64ENCODEDECODEErr;
		goto EXIT;
	}

	strcpy(pszOut, (char *)&rBuff2[0]);

EXIT:
	if (digest_alg != NULL)
		X509_ALGOR_free(digest_alg);
	if (digest_enc_alg != NULL)
		X509_ALGOR_free(digest_enc_alg);
	if (signcert != NULL)
		X509_free(signcert);

	if (si != NULL)
		PKCS7_SIGNER_INFO_free(si);
	if (p7 != NULL)
		PKCS7_SM2_SIGNATURE_free(p7);

	return lRet;
}

int IsPKCS7SM2Sign(PKCS7_SM2_SIGNATURE *p7)
{
// 	unsigned char byType[64];
// 	memset(byType, 0, sizeof(byType));
// 	int nTypeLen = sizeof(g_bySM2SignedData);
// 	memcpy(byType, g_bySM2SignedData, nTypeLen);
// 
// 	unsigned char byType2[64];
// 	memset(byType2, 0, sizeof(byType2));
// 	unsigned char *pbTmp = byType2;
// 	int nTypeLen2 = i2d_ASN1_OBJECT(p7->type, &pbTmp);
// 	if (nTypeLen != nTypeLen2 ||
// 		memcmp(byType, byType2, nTypeLen) != 0)
// 		return 0;

	// no_name为1，使用数字形式。
	unsigned char byType[64];
	memset(byType, 0, sizeof(byType));
	if (!OBJ_obj2txt((char *)byType, sizeof(byType), p7->type, 1))
		return 0;

	if (strcmp((char *)byType, TXT_gm_signed_data) != 0)
 		return 0;

	return 1;
}

// PKCS7验证签名
long MYSIGN_VerifySignedDataByP7_SM2(
	char *pszSign,
	unsigned char *pbData,
	long lDataLen)
{
	long lRet = SAR_OK;

	int i;
	unsigned char *pbTmp;

	STACK_OF(X509) *signcerts = NULL;
	STACK_OF(PKCS7_SIGNER_INFO) *signers = NULL;

	PKCS7_SM2_SIGNATURE *p7 = NULL;

	// Base64 decode
	long lBuffLen = strlen(pszSign);
	ByteArray rBuff(lBuffLen);
	memset(&rBuff[0], 0, lBuffLen);
	lBuffLen = CMailCoder::base64_decode((char *)pszSign, strlen(pszSign), (char *)&rBuff[0]);
	if (lBuffLen <= 0)
		return SAR_BASE64ENCODEDECODEErr;

	// p7 signed data
	pbTmp = &rBuff[0];
	p7 = d2i_PKCS7_SM2_SIGNATURE(NULL, (const unsigned char **)&pbTmp, lBuffLen);
	if (p7 == NULL)
		return SAR_PKCS7ENCODEErr;

	if (!IsPKCS7SM2Sign(p7))
	{
		lRet = SAR_PKCS7ENCODEErr;
		goto EXIT;
	}

	// get data
	unsigned char *pbOriginalData;
	long lOriginalDataLen;
	if (pbData != NULL && lDataLen > 0)
	{
		pbOriginalData = pbData;
		lOriginalDataLen = lDataLen;
	}
	else
	{
		if (p7->signedData->contentInfo->data == NULL)
		{
			lRet = SAR_ECCVERIFYSIGNErr;
			goto EXIT;
		}
		pbOriginalData = p7->signedData->contentInfo->data->data;
		lOriginalDataLen = p7->signedData->contentInfo->data->length;
	}
	if (lOriginalDataLen <= 0)
	{
		lRet = SAR_ECCVERIFYSIGNErr;
		goto EXIT;
	}

	// get sign cert
	unsigned char byCert[SGD_SIZE_CERT];
	memset(byCert, 0, sizeof(byCert));
	long lCertLen;
	lCertLen = 0;

	signcerts = p7->signedData->certificates;
	if (signcerts == NULL)
	{
		lRet = SAR_PKCS7ENCODEErr;
		goto EXIT;
	}

	for (i = 0; i < sk_X509_num(signcerts); i++)
	{
		X509 *signcert = sk_X509_value(signcerts, i);

		pbTmp = byCert;
		lCertLen = i2d_X509(signcert, &pbTmp);
		if (lCertLen > 0)
			break;
	}
	if (lCertLen <= 0)
	{
		lRet = SAR_PKCS7ENCODEErr;
		goto EXIT;
	}

	// get signed data
	unsigned char bySign[SGD_SIZE_RSADATA];
	memset(bySign, 0, sizeof(bySign));
	long lSignLen;
	lSignLen = 0;

	signers = p7->signedData->signerInfos;
	if (signers == NULL)
	{
		lRet = SAR_PKCS7ENCODEErr;
		goto EXIT;
	}

	for (i = 0; i < sk_PKCS7_SIGNER_INFO_num(signers); i++)
	{
		PKCS7_SIGNER_INFO *signer = sk_PKCS7_SIGNER_INFO_value(signers, i);
		lSignLen = signer->enc_digest->length;
		if (lSignLen > 0)
		{
			memcpy(bySign, signer->enc_digest->data, lSignLen);
			break;
		}
	}
	if (lSignLen <= 0)
	{
		lRet = SAR_PKCS7ENCODEErr;
		goto EXIT;
	}

	// verify sign
	lRet = MYTOOL_ECCVerifySignedData(
		pbOriginalData,
		lOriginalDataLen,
		bySign,
		lSignLen,
		byCert,
		lCertLen);
	if (lRet != SAR_OK)
		goto EXIT;

EXIT:	
	PKCS7_SM2_SIGNATURE_free(p7);

	return lRet;
}

// 获取PKCS7签名信息
long MYSIGN_GetP7SignDataInfo_SM2(
	char *pszIn,
	long lType,
	unsigned char *pbOut,
	long *plOutLen)
{
	long lRet = SAR_OK;

	// Base64 decode
	long lBuffLen = strlen(pszIn);
	ByteArray rBuff(lBuffLen);
	memset(&rBuff[0], 0, lBuffLen);
	lBuffLen = CMailCoder::base64_decode((char *)pszIn, strlen(pszIn), (char *)&rBuff[0]);
	if (lBuffLen <= 0)
		return SAR_BASE64ENCODEDECODEErr;

	// p7 signed data
	unsigned char *pbTmp = &rBuff[0];
	PKCS7_SM2_SIGNATURE *p7 = d2i_PKCS7_SM2_SIGNATURE(NULL, (const unsigned char **)&pbTmp, lBuffLen);
	if (p7 == NULL)
		return SAR_PKCS7ENCODEErr;

	if (!IsPKCS7SM2Sign(p7))
	{
		lRet = SAR_PKCS7ENCODEErr;
		goto EXIT;
	}

	int i;
	bool bFound;
	bFound = false;
	if (lType == 1)
	{
		// get data
		if (p7->signedData->contentInfo->data == NULL)
		{
			*plOutLen = 0;
		}
		else
		{
			if (p7->signedData->contentInfo->data->length <= 0)
			{
				lRet = SAR_PKCS7ENCODEErr;
				goto EXIT;
			}

			*plOutLen = p7->signedData->contentInfo->data->length;
			memcpy(pbOut, p7->signedData->contentInfo->data->data, *plOutLen);
		}
	}
	else if (lType == 2)
	{
		// get sign cert
		STACK_OF(X509) *signcerts = p7->signedData->certificates;
		if (signcerts == NULL)
		{
			lRet = SAR_PKCS7ENCODEErr;
			goto EXIT;
		}

		for (i = 0; i < sk_X509_num(signcerts); i++)
		{
			X509 *signcert = sk_X509_value(signcerts, i);

			unsigned char byCert[SGD_SIZE_CERT];
			memset(byCert, 0, sizeof(byCert));
			pbTmp = byCert;
			long lCertLen = i2d_X509(signcert, &pbTmp);
			if (lCertLen > 0)
			{
				char szCertB64[SGD_SIZE_CERT * 2];
				memset(szCertB64, 0, sizeof(szCertB64));
				long lCertB64Len = CMailCoder::base64_encode((char *)byCert, lCertLen, szCertB64);
				if (lCertB64Len > 0)
				{
					strcpy((char *)pbOut, szCertB64);
					*plOutLen = lCertB64Len;
					bFound = true;
					break;
				}
			}
		}

		if (!bFound)
		{
			lRet = SAR_PKCS7ENCODEErr;
			goto EXIT;
		}
	}
	else
	{
		// get signed data
		STACK_OF(PKCS7_SIGNER_INFO) *signers = p7->signedData->signerInfos;
		if (signers == NULL)
		{
			lRet = SAR_PKCS7ENCODEErr;
			goto EXIT;
		}

		for (i = 0; i < sk_PKCS7_SIGNER_INFO_num(signers); i++)
		{
			PKCS7_SIGNER_INFO *signer = sk_PKCS7_SIGNER_INFO_value(signers, i);
			long lSignLen = signer->enc_digest->length;
			if (lSignLen > 0)
			{
				unsigned char bySign[SGD_SIZE_RSADATA];
				memset(bySign, 0, sizeof(bySign));
				memcpy(bySign, signer->enc_digest->data, lSignLen);

				char szSignB64[SGD_SIZE_RSADATA * 2];
				memset(szSignB64, 0, sizeof(szSignB64));
				long lSignB64Len = CMailCoder::base64_encode((char *)bySign, lSignLen, szSignB64);
				if (lSignB64Len > 0)
				{
					strcpy((char *)pbOut, szSignB64);
					*plOutLen = lSignB64Len;
					bFound = true;
					break;
				}
			}
		}

		if (!bFound)
		{
			lRet = SAR_PKCS7ENCODEErr;
			goto EXIT;
		}
	}

EXIT:	
	PKCS7_SM2_SIGNATURE_free(p7);

	return lRet;
}





