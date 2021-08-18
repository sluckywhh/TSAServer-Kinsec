#ifndef _KS_SMDATA_H
#define _KS_SMDATA_H

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>

#include <openssl/asn1t.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/sha.h>
#include <openssl/ts.h>

#include "MySMObjID.h"

////////////////////////////////////////////////////////////////
// 国密SM2数据结构

// SM2公钥
typedef ASN1_BIT_STRING SM2_PUBLICKEY;

DECLARE_ASN1_FUNCTIONS(SM2_PUBLICKEY)

// SM2私钥
typedef ASN1_INTEGER SM2_PRIVATEKEY;

DECLARE_ASN1_FUNCTIONS(SM2_PRIVATEKEY)

// SM2签名
typedef struct SM2_signature_st
{
	ASN1_INTEGER *r;
	ASN1_INTEGER *s;
} SM2_SIGNATURE;

DECLARE_ASN1_FUNCTIONS(SM2_SIGNATURE)

// SM2密文
typedef struct SM2_cipher_st
{
	ASN1_INTEGER *x;
	ASN1_INTEGER *y;
	ASN1_OCTET_STRING *hash;
	ASN1_OCTET_STRING *cipherText;
} SM2_CIPHER;

DECLARE_ASN1_FUNCTIONS(SM2_CIPHER)

////////////////////////////////////////////////////////////////
// PKCS7数据结构

// 数据
typedef struct ks_pkcs7_data_st
{
	ASN1_OBJECT *type;
	ASN1_OCTET_STRING *data;
} KS_PKCS7_DATA;

DECLARE_ASN1_FUNCTIONS(KS_PKCS7_DATA)

// 签名数据
typedef struct ks_pkcs7_signed_data_st
{
	ASN1_INTEGER				*version;			/* version 1 */
	STACK_OF(X509_ALGOR)		*digestAlgorithms;	/* md used */
	KS_PKCS7_DATA				*contentInfo;
	STACK_OF(X509)				*certificates;		/* [ 0 ] */
	STACK_OF(X509_CRL)			*crls;				/* [ 1 ] */
	STACK_OF(PKCS7_SIGNER_INFO)	*signerInfos;
} KS_PKCS7_SIGNED_DATA;

typedef struct ks_pkcs7_signature_st
{
	ASN1_OBJECT *type;
	KS_PKCS7_SIGNED_DATA *signedData;
} KS_PKCS7_SIGNATURE;

DECLARE_ASN1_FUNCTIONS(KS_PKCS7_SIGNED_DATA)
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_SIGNATURE)

// 签名数据（带校验码）
typedef struct ks_pkcs7_signature_with_verification_code_st
{
	KS_PKCS7_SIGNATURE *signature;
	ASN1_OCTET_STRING	*verificationCode;
} KS_PKCS7_SIGNATURE_WITH_VERIFICATION_CODE;
 
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_SIGNATURE_WITH_VERIFICATION_CODE)

// 时间戳响应数据
typedef struct ks_ts_status_info_st
{
	ASN1_INTEGER *status;
	STACK_OF(ASN1_UTF8STRING) *text;
	ASN1_BIT_STRING *failure_info;
} KS_TS_STATUS_INFO;

DECLARE_ASN1_FUNCTIONS(KS_TS_STATUS_INFO)

typedef struct ks_time_stamp_resp_st
{
	KS_TS_STATUS_INFO *statusInfo;
	KS_PKCS7_SIGNATURE *timeStampToken;
} KS_TIME_STAMP_RESP;
 
DECLARE_ASN1_FUNCTIONS(KS_TIME_STAMP_RESP)

// 数字信封数据
typedef struct ks_pkcs7_encrypted_contentinfo_st
{
	ASN1_OBJECT			*contentType;
	X509_ALGOR			*contentEncryptionAlgorithm;
	ASN1_OCTET_STRING	*encryptedContent;	/* [ 0 ] */
	ASN1_OCTET_STRING	*sharedInfo;		/* [ 1 ] */
	ASN1_OCTET_STRING	*sharedInfo2;		/* [ 2 ] */
} KS_PKCS7_ENCRYPTED_CONTENTINFO;

typedef struct ks_pkcs7_enveloped_data_st
{
	ASN1_INTEGER				*version;	/* version 1 */
	STACK_OF(PKCS7_RECIP_INFO)	*recipientInfos;
	KS_PKCS7_ENCRYPTED_CONTENTINFO	*encryptedContentInfo;
} KS_PKCS7_ENVELOPED_DATA;

typedef struct ks_pkcs7_envelope_st
{
	ASN1_OBJECT *type;
	KS_PKCS7_ENVELOPED_DATA *envelopedData;
} KS_PKCS7_ENVELOPE;
 
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_ENCRYPTED_CONTENTINFO)
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_ENVELOPED_DATA)
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_ENVELOPE)

// 数字信封数据（带校验码）
typedef struct ks_pkcs7_envelope_with_verification_code_st
{
	KS_PKCS7_ENVELOPE *envelope;
	ASN1_OCTET_STRING	*verificationCode;
} KS_PKCS7_ENVELOPE_WITH_VERIFICATION_CODE;
 
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_ENVELOPE_WITH_VERIFICATION_CODE)

// 签名及数字信封数据
typedef struct ks_pkcs7_signed_and_enveloped_data_st
{
	ASN1_INTEGER				*version;			/* version 1 */
	STACK_OF(PKCS7_RECIP_INFO)	*recipientInfos;
	STACK_OF(X509_ALGOR)		*digestAlgorithms;	/* md used */
	KS_PKCS7_ENCRYPTED_CONTENTINFO	*encryptedContentInfo;
	STACK_OF(X509)				*certificates;		/* [ 0 ] */
	STACK_OF(X509_CRL)			*crls;				/* [ 1 ] */
	STACK_OF(PKCS7_SIGNER_INFO)	*signerInfos;
} KS_PKCS7_SIGNED_AND_ENVELOPED_DATA;

typedef struct ks_pkcs7_signature_and_envelope_st
{
	ASN1_OBJECT *type;
	KS_PKCS7_SIGNED_AND_ENVELOPED_DATA *signedAndEnvelopedData;
} KS_PKCS7_SIGNATURE_AND_ENVELOPE;

DECLARE_ASN1_FUNCTIONS(KS_PKCS7_SIGNED_AND_ENVELOPED_DATA)
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_SIGNATURE_AND_ENVELOPE)

// 签名及数字信封数据（带校验码）
typedef struct ks_pkcs7_signature_and_envelope_with_verification_code_st
{
	KS_PKCS7_SIGNATURE_AND_ENVELOPE *signAndEnvelope;
	ASN1_OCTET_STRING	*verificationCode;
} KS_PKCS7_SIGNATURE_AND_ENVELOPE_WITH_VERIFICATION_CODE;
 
DECLARE_ASN1_FUNCTIONS(KS_PKCS7_SIGNATURE_AND_ENVELOPE_WITH_VERIFICATION_CODE)

#endif
