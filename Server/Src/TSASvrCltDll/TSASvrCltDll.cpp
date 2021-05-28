// =================================================================================
//  File       : TSASvrCltDll.cpp
//  Purpose    : 时间戳服务器客户端接口。
//  Author     : 林灶钢
//  Created    : 2006-11-21
//  CopyRight (C) 林灶钢
// =================================================================================

// =================================================================================
//  Include Header Files
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/hmac.h>
#include <openssl/ts.h>

#if defined(_LINUX)
#include <sys/timeb.h>
#endif

#include "PKIDef.h"
#include "ASN1PKI.hpp"
#include "TSASvrClient.h"
#include "TSASvrCltDll.h"
#include "MailCoder.h"
#include "TSErr.h"
#include "pk7_sm2_sign.h"
#include "FJCAApiConst.h"
#include "MySMData.h"


//时间戳RSA修改源码不验证证书时间2018-10-08
/*
static int TS_verify_cert2(X509_STORE *store, STACK_OF(X509) *untrusted,
			  X509 *signer, STACK_OF(X509) **chain);
static int TS_check_signing_certs2(PKCS7_SIGNER_INFO *si, STACK_OF(X509) *chain);
static ESS_SIGNING_CERT *ESS_get_signing_cert2(PKCS7_SIGNER_INFO *si);
static int TS_find_cert2(STACK_OF(ESS_CERT_ID) *cert_ids, X509 *cert);
static int TS_issuer_serial_cmp2(ESS_ISSUER_SERIAL *is, X509_CINF *cinfo);
static int int_TS_RESP_verify_token2(TS_VERIFY_CTX *ctx, 
				 PKCS7 *token, TS_TST_INFO *tst_info);
static int TS_check_status_info2(TS_RESP *response);
static char *TS_get_status_text2(STACK_OF(ASN1_UTF8STRING) *text);
static int TS_check_policy2(ASN1_OBJECT *req_oid, TS_TST_INFO *tst_info);
static int TS_compute_imprint2(BIO *data, TS_TST_INFO *tst_info,
			      X509_ALGOR **md_alg, 
			      unsigned char **imprint, unsigned *imprint_len);
static int TS_check_imprints2(X509_ALGOR *algor_a, 
			     unsigned char *imprint_a, unsigned len_a,
			     TS_TST_INFO *tst_info);
static int TS_check_nonces2(const ASN1_INTEGER *a, TS_TST_INFO *tst_info);
static int TS_check_signer_name2(GENERAL_NAME *tsa_name, X509 *signer);
static int TS_find_name2(STACK_OF(GENERAL_NAME) *gen_names, GENERAL_NAME *name);
*/

/*
 * Local mapping between response codes and descriptions.
 * Don't forget to change TS_STATUS_BUF_SIZE when modifying 
 * the elements of this array.
 */
static const char *TS_status_text2[] =
	{ "granted",
	  "grantedWithMods",
	  "rejection",
	  "waiting",
	  "revocationWarning",
	  "revocationNotification" };

#define TS_STATUS_TEXT2_SIZE	(sizeof(TS_status_text2)/sizeof(*TS_status_text2))

/*
 * This must be greater or equal to the sum of the strings in TS_status_text
 * plus the number of its elements.
 */
#define TS_STATUS_BUF_SIZE	256

static struct
	{
	int code;
	const char *text;
	} TS_failure_info2[] =
		{ { TS_INFO_BAD_ALG, "badAlg" },
		  { TS_INFO_BAD_REQUEST, "badRequest" },
		  { TS_INFO_BAD_DATA_FORMAT, "badDataFormat" },
		  { TS_INFO_TIME_NOT_AVAILABLE, "timeNotAvailable" },
		  { TS_INFO_UNACCEPTED_POLICY, "unacceptedPolicy" },
		  { TS_INFO_UNACCEPTED_EXTENSION, "unacceptedExtension" },
		  { TS_INFO_ADD_INFO_NOT_AVAILABLE, "addInfoNotAvailable" },
		  { TS_INFO_SYSTEM_FAILURE, "systemFailure" } };

#define TS_FAILURE_INFO2_SIZE	(sizeof(TS_failure_info2) / \
				sizeof(*TS_failure_info2))

/* Functions for verifying a signed TS_TST_INFO structure. */

/*
 * This function carries out the following tasks:
 *	- Checks if there is one and only one signer.
 *	- Search for the signing certificate in 'certs' and in the response.
 *	- Check the extended key usage and key usage fields of the signer
 *	certificate (done by the path validation).
 *	- Build and validate the certificate path.
 *	- Check if the certificate path meets the requirements of the
 *	SigningCertificate ESS signed attribute.
 *	- Verify the signature value.
 *	- Returns the signer certificate in 'signer', if 'signer' is not NULL.
 */
/* Returns 1 if name is in gen_names, 0 otherwise. */
static int TS_find_name2(STACK_OF(GENERAL_NAME) *gen_names, GENERAL_NAME *name)
	{
	int i, found;
	for (i = 0, found = 0; !found && i < sk_GENERAL_NAME_num(gen_names);
	     ++i)
		{
		GENERAL_NAME *current = sk_GENERAL_NAME_value(gen_names, i);
		found = GENERAL_NAME_cmp(current, name) == 0;
		}
	return found ? i - 1 : -1;
	}

/* Check if the specified TSA name matches either the subject
   or one of the subject alternative names of the TSA certificate. */
static int TS_check_signer_name2(GENERAL_NAME *tsa_name, X509 *signer)
	{
	STACK_OF(GENERAL_NAME) *gen_names = NULL;
	int idx = -1;
	int found = 0;

	/* Check the subject name first. */
	if (tsa_name->type == GEN_DIRNAME 
	    && X509_name_cmp(tsa_name->d.dirn, signer->cert_info->subject) == 0)
		return 1;

	/* Check all the alternative names. */
	gen_names = (STACK_OF(GENERAL_NAME) *)X509_get_ext_d2i(signer, NID_subject_alt_name,
				     NULL, &idx);
	while (gen_names != NULL
	       && !(found = TS_find_name2(gen_names, tsa_name) >= 0))
		{
		/* Get the next subject alternative name,
		   although there should be no more than one. */
		GENERAL_NAMES_free(gen_names);
		gen_names = (STACK_OF(GENERAL_NAME) *)X509_get_ext_d2i(signer, NID_subject_alt_name,
					     NULL, &idx);
		}
	if (gen_names) GENERAL_NAMES_free(gen_names);
	
	return found;
	}

static int TS_check_nonces2(const ASN1_INTEGER *a, TS_TST_INFO *tst_info)
	{
	const ASN1_INTEGER *b = TS_TST_INFO_get_nonce(tst_info);

	/* Error if nonce is missing. */
	if (!b)
		{
		TSerr(TS_F_TS_CHECK_NONCES, TS_R_NONCE_NOT_RETURNED);
		return 0;
		}

	/* No error if a nonce is returned without being requested. */
	if (ASN1_INTEGER_cmp(a, b) != 0)
		{
		TSerr(TS_F_TS_CHECK_NONCES, TS_R_NONCE_MISMATCH);
		return 0;
		}

	return 1;
	}

static int TS_check_imprints2(X509_ALGOR *algor_a, 
			     unsigned char *imprint_a, unsigned len_a,
			     TS_TST_INFO *tst_info)
	{
	TS_MSG_IMPRINT *b = TS_TST_INFO_get_msg_imprint(tst_info);
	X509_ALGOR *algor_b = TS_MSG_IMPRINT_get_algo(b);
	int ret = 0;

	/* algor_a is optional. */
	if (algor_a)
		{
		/* Compare algorithm OIDs. */
		if (OBJ_cmp(algor_a->algorithm, algor_b->algorithm)) goto err;

		/* The parameter must be NULL in both. */
		if ((algor_a->parameter 
		     && ASN1_TYPE_get(algor_a->parameter) != V_ASN1_NULL)
		    || (algor_b->parameter
			&& ASN1_TYPE_get(algor_b->parameter) != V_ASN1_NULL))
			goto err;
		}

	/* Compare octet strings. */
	ret = len_a == (unsigned) ASN1_STRING_length(b->hashed_msg) &&
		memcmp(imprint_a, ASN1_STRING_data(b->hashed_msg), len_a) == 0;
 err:
	if (!ret)
		TSerr(TS_F_TS_CHECK_IMPRINTS, TS_R_MESSAGE_IMPRINT_MISMATCH);
	return ret;
	}

static int TS_compute_imprint2(BIO *data, TS_TST_INFO *tst_info,
			      X509_ALGOR **md_alg, 
			      unsigned char **imprint, unsigned *imprint_len)
	{
	TS_MSG_IMPRINT *msg_imprint = TS_TST_INFO_get_msg_imprint(tst_info);
	X509_ALGOR *md_alg_resp = TS_MSG_IMPRINT_get_algo(msg_imprint);
	const EVP_MD *md;
	EVP_MD_CTX md_ctx;
	unsigned char buffer[4096];
	int length;

	*md_alg = NULL;
	*imprint = NULL;

	/* Return the MD algorithm of the response. */
	if (!(*md_alg = X509_ALGOR_dup(md_alg_resp))) goto err;

	/* Getting the MD object. */
	if (!(md = EVP_get_digestbyobj((*md_alg)->algorithm)))
		{
		TSerr(TS_F_TS_COMPUTE_IMPRINT, TS_R_UNSUPPORTED_MD_ALGORITHM);
		goto err;
		}

	/* Compute message digest. */
	length = EVP_MD_size(md);
	if (length < 0)
	    goto err;
	*imprint_len = length;
	if (!(*imprint = (unsigned char *)OPENSSL_malloc(*imprint_len))) 
		{
		TSerr(TS_F_TS_COMPUTE_IMPRINT, ERR_R_MALLOC_FAILURE);
		goto err;
		}

	EVP_DigestInit(&md_ctx, md);
	while ((length = BIO_read(data, buffer, sizeof(buffer))) > 0)
		{
		EVP_DigestUpdate(&md_ctx, buffer, length);
		}
	EVP_DigestFinal(&md_ctx, *imprint, NULL);

	return 1;
 err:
	X509_ALGOR_free(*md_alg);
	OPENSSL_free(*imprint);
	*imprint_len = 0;
	return 0;
	}

static int TS_check_policy2(ASN1_OBJECT *req_oid, TS_TST_INFO *tst_info)
	{
	ASN1_OBJECT *resp_oid = TS_TST_INFO_get_policy_id(tst_info);

	if (OBJ_cmp(req_oid, resp_oid) != 0)
		{
		TSerr(TS_F_TS_CHECK_POLICY, TS_R_POLICY_MISMATCH);
		return 0;
		}

	return 1;
	}

static char *TS_get_status_text2(STACK_OF(ASN1_UTF8STRING) *text)
	{
	int i;
	unsigned int length = 0;
	char *result = NULL;
	char *p;

	/* Determine length first. */
	for (i = 0; i < sk_ASN1_UTF8STRING_num(text); ++i)
		{
		ASN1_UTF8STRING *current = sk_ASN1_UTF8STRING_value(text, i);
		length += ASN1_STRING_length(current);
		length += 1;	/* separator character */
		}
	/* Allocate memory (closing '\0' included). */
	if (!(result = (char *)OPENSSL_malloc(length)))
		{
		TSerr(TS_F_TS_GET_STATUS_TEXT, ERR_R_MALLOC_FAILURE);
		return NULL;
		}
	/* Concatenate the descriptions. */
	for (i = 0, p = result; i < sk_ASN1_UTF8STRING_num(text); ++i)
		{
		ASN1_UTF8STRING *current = sk_ASN1_UTF8STRING_value(text, i);
		length = ASN1_STRING_length(current);
		if (i > 0) *p++ = '/';
		strncpy(p, (const char *)ASN1_STRING_data(current), length);
		p += length;
		}
	/* We do have space for this, too. */
	*p = '\0';
	
	return result;
	}

static int TS_check_status_info2(TS_RESP *response)
	{
	TS_STATUS_INFO *info = TS_RESP_get_status_info(response);
	long status = ASN1_INTEGER_get(info->status);
	const char *status_text = NULL;
	char *embedded_status_text = NULL;
	char failure_text[TS_STATUS_BUF_SIZE] = "";

	/* Check if everything went fine. */
	if (status == 0 || status == 1) return 1;

	/* There was an error, get the description in status_text. */
	if (0 <= status && status < (long)TS_STATUS_TEXT2_SIZE)
		status_text = TS_status_text2[status];
	else
		status_text = "unknown code";

	/* Set the embedded_status_text to the returned description. */
	if (sk_ASN1_UTF8STRING_num(info->text) > 0
	    && !(embedded_status_text = TS_get_status_text2(info->text)))
		return 0;
	
	/* Filling in failure_text with the failure information. */
	if (info->failure_info)
		{
		int i;
		int first = 1;
		for (i = 0; i < (int)TS_FAILURE_INFO2_SIZE; ++i)
			{
			if (ASN1_BIT_STRING_get_bit(info->failure_info,
						    TS_failure_info2[i].code))
				{
				if (!first)
					strcpy(failure_text, ",");
				else
					first = 0;
				strcat(failure_text, TS_failure_info2[i].text);
				}
			}
		}
	if (failure_text[0] == '\0')
		strcpy(failure_text, "unspecified");

	/* Making up the error string. */
	TSerr(TS_F_TS_CHECK_STATUS_INFO, TS_R_NO_TIME_STAMP_TOKEN);
	ERR_add_error_data(6,
			   "status code: ", status_text,
			   ", status text: ", embedded_status_text ? 
			   embedded_status_text : "unspecified",
			   ", failure codes: ", failure_text);
	OPENSSL_free(embedded_status_text);

	return 0;
	}

/*
 * The certificate chain is returned in chain. Caller is responsible for
 * freeing the vector.
 */
static int TS_verify_cert2(X509_STORE *store, STACK_OF(X509) *untrusted,
			  X509 *signer, STACK_OF(X509) **chain)
	{
	X509_STORE_CTX	cert_ctx;
	int i, j;
	int ret = 1;

	/* chain is an out argument. */
	*chain = NULL;
	X509_STORE_CTX_init(&cert_ctx, store, signer, untrusted);
	X509_STORE_CTX_set_purpose(&cert_ctx, X509_PURPOSE_TIMESTAMP_SIGN);
	i = X509_verify_cert(&cert_ctx);
/*	if (i <= 0)
		{
		int j = X509_STORE_CTX_get_error(&cert_ctx);
		TSerr(TS_F_TS_VERIFY_CERT, TS_R_CERTIFICATE_VERIFY_ERROR);
		ERR_add_error_data(2, "Verify error:",
				   X509_verify_cert_error_string(j));
		ret = 0;
		}
	else
		{
		// Get a copy of the certificate chain.
		*chain = X509_STORE_CTX_get1_chain(&cert_ctx);
		}
*/
	if (i <= 0)
		j = X509_STORE_CTX_get_error(&cert_ctx);

	if (i <= 0 && j != X509_V_ERR_CERT_HAS_EXPIRED)
		{
		TSerr(TS_F_TS_VERIFY_CERT, TS_R_CERTIFICATE_VERIFY_ERROR);
		ERR_add_error_data(2, "Verify error:",
				   X509_verify_cert_error_string(j));
		ret = 0;
		}
	else
		{
		/* Get a copy of the certificate chain. */
		*chain = X509_STORE_CTX_get1_chain(&cert_ctx);
		}

	X509_STORE_CTX_cleanup(&cert_ctx);

	return ret;
	}

static ESS_SIGNING_CERT *ESS_get_signing_cert2(PKCS7_SIGNER_INFO *si)
	{
	ASN1_TYPE *attr;
	const unsigned char *p;
	attr = PKCS7_get_signed_attribute(si, 
					  NID_id_smime_aa_signingCertificate);
	if (!attr) return NULL;
	p = attr->value.sequence->data;
	return d2i_ESS_SIGNING_CERT(NULL, &p, attr->value.sequence->length);
	}

static int TS_issuer_serial_cmp2(ESS_ISSUER_SERIAL *is, X509_CINF *cinfo)
	{
	GENERAL_NAME *issuer;

	if (!is || !cinfo || sk_GENERAL_NAME_num(is->issuer) != 1) return -1;

	/* Check the issuer first. It must be a directory name. */
	issuer = sk_GENERAL_NAME_value(is->issuer, 0);
	if (issuer->type != GEN_DIRNAME 
	    || X509_NAME_cmp(issuer->d.dirn, cinfo->issuer))
		return -1;

	/* Check the serial number, too. */
	if (ASN1_INTEGER_cmp(is->serial, cinfo->serialNumber))
		return -1;

	return 0;
	}

/* Returns < 0 if certificate is not found, certificate index otherwise. */
static int TS_find_cert2(STACK_OF(ESS_CERT_ID) *cert_ids, X509 *cert)
	{
	int i;

	if (!cert_ids || !cert) return -1;

	/* Recompute SHA1 hash of certificate if necessary (side effect). */
	X509_check_purpose(cert, -1, 0);

	/* Look for cert in the cert_ids vector. */
	for (i = 0; i < sk_ESS_CERT_ID_num(cert_ids); ++i)
		{
		ESS_CERT_ID *cid = sk_ESS_CERT_ID_value(cert_ids, i);

		/* Check the SHA-1 hash first. */
		if (cid->hash->length == sizeof(cert->sha1_hash)
		    && !memcmp(cid->hash->data, cert->sha1_hash,
			       sizeof(cert->sha1_hash)))
			{
			/* Check the issuer/serial as well if specified. */
			ESS_ISSUER_SERIAL *is = cid->issuer_serial;
			if (!is || !TS_issuer_serial_cmp2(is, cert->cert_info))
				return i;
			}
		}
	
	return -1;
	}

static int TS_check_signing_certs2(PKCS7_SIGNER_INFO *si, STACK_OF(X509) *chain)
	{
	ESS_SIGNING_CERT *ss = ESS_get_signing_cert2(si);
	STACK_OF(ESS_CERT_ID) *cert_ids = NULL;
	X509 *cert;
	int i = 0;
	int ret = 0;

	if (!ss) goto err;
	cert_ids = ss->cert_ids;
	/* The signer certificate must be the first in cert_ids. */
	cert = sk_X509_value(chain, 0);
	if (TS_find_cert2(cert_ids, cert) != 0) goto err;
	
	/* Check the other certificates of the chain if there are more
	   than one certificate ids in cert_ids. */
	if (sk_ESS_CERT_ID_num(cert_ids) > 1)
		{
		/* All the certificates of the chain must be in cert_ids. */
		for (i = 1; i < sk_X509_num(chain); ++i)
			{
			cert = sk_X509_value(chain, i);
			if (TS_find_cert2(cert_ids, cert) < 0) goto err;
			}
		}
	ret = 1;
 err:
	if (!ret)
		TSerr(TS_F_TS_CHECK_SIGNING_CERTS, 
		      TS_R_ESS_SIGNING_CERTIFICATE_ERROR);
	ESS_SIGNING_CERT_free(ss);
	return ret;
	}

int TS_RESP_verify_signature2(PKCS7 *token, STACK_OF(X509) *certs,
			     X509_STORE *store, X509 **signer_out)
	{
	STACK_OF(PKCS7_SIGNER_INFO) *sinfos = NULL;
	PKCS7_SIGNER_INFO *si;
	STACK_OF(X509) *signers = NULL;
	X509	*signer;
	STACK_OF(X509) *chain = NULL;
	char	buf[4096];
	int	i, j = 0, ret = 0;
	BIO	*p7bio = NULL;

	/* Some sanity checks first. */
	if (!token)
		{
		TSerr(TS_F_TS_RESP_VERIFY_SIGNATURE, TS_R_INVALID_NULL_POINTER);
		goto err;
		}

	/* Check for the correct content type */
	if(!PKCS7_type_is_signed(token))
		{
		TSerr(TS_F_TS_RESP_VERIFY_SIGNATURE, TS_R_WRONG_CONTENT_TYPE);
		goto err;
		}

	/* Check if there is one and only one signer. */
	sinfos = PKCS7_get_signer_info(token);
	if (!sinfos || sk_PKCS7_SIGNER_INFO_num(sinfos) != 1)
		{
		TSerr(TS_F_TS_RESP_VERIFY_SIGNATURE,
		      TS_R_THERE_MUST_BE_ONE_SIGNER);
		goto err;
		}
	si = sk_PKCS7_SIGNER_INFO_value(sinfos, 0);

	/* Check for no content: no data to verify signature. */
	if (PKCS7_get_detached(token))
		{
		TSerr(TS_F_TS_RESP_VERIFY_SIGNATURE, TS_R_NO_CONTENT);
		goto err;
		}
	
	/* Get hold of the signer certificate, search only internal
	   certificates if it was requested. */
	signers = PKCS7_get0_signers(token, certs, 0);
	if (!signers || sk_X509_num(signers) != 1) goto err;
	signer = sk_X509_value(signers, 0);

	/* Now verify the certificate. */
	if (!TS_verify_cert2(store, certs, signer, &chain)) goto err;

	/* Check if the signer certificate is consistent with the
	   ESS extension. */
	if (!TS_check_signing_certs2(si, chain)) goto err;

	/* Creating the message digest. */
	p7bio = PKCS7_dataInit(token, NULL);

	/* We now have to 'read' from p7bio to calculate digests etc. */
	while ((i = BIO_read(p7bio,buf,sizeof(buf))) > 0);

	/* Verifying the signature. */
	j = PKCS7_signatureVerify(p7bio, token, si, signer);
	if (j <= 0)
		{
		TSerr(TS_F_TS_RESP_VERIFY_SIGNATURE, TS_R_SIGNATURE_FAILURE);
		goto err;
		}

	/* Return the signer certificate if needed. */
	if (signer_out)
		{
		*signer_out = signer;
		CRYPTO_add(&signer->references, 1, CRYPTO_LOCK_X509);
		}

	ret = 1;

 err:
	BIO_free_all(p7bio);
	sk_X509_pop_free(chain, X509_free);
	sk_X509_free(signers);

	return ret;
	}

/*
 * Verifies whether the 'token' contains a valid time stamp token 
 * with regards to the settings of the context. Only those checks are
 * carried out that are specified in the context:
 *	- Verifies the signature of the TS_TST_INFO.
 *	- Checks the version number of the response.
 *	- Check if the requested and returned policies math.
 *	- Check if the message imprints are the same.
 *	- Check if the nonces are the same.
 *	- Check if the TSA name matches the signer.
 *	- Check if the TSA name is the expected TSA.
 */
static int int_TS_RESP_verify_token2(TS_VERIFY_CTX *ctx, 
				 PKCS7 *token, TS_TST_INFO *tst_info)
	{
	X509 *signer = NULL;
	GENERAL_NAME *tsa_name = TS_TST_INFO_get_tsa(tst_info);
	X509_ALGOR *md_alg = NULL;
	unsigned char *imprint = NULL;
	unsigned imprint_len = 0;
	int ret = 0;

	/* Verify the signature. */
	if ((ctx->flags & TS_VFY_SIGNATURE)
	    && !TS_RESP_verify_signature2(token, ctx->certs, ctx->store,
					 &signer))
		goto err;
	
	/* Check version number of response. */
	if ((ctx->flags & TS_VFY_VERSION)
	    && TS_TST_INFO_get_version(tst_info) != 1)
		{
		TSerr(TS_F_INT_TS_RESP_VERIFY_TOKEN, TS_R_UNSUPPORTED_VERSION);
		goto err;
		}

	/* Check policies. */
	if ((ctx->flags & TS_VFY_POLICY)
	    && !TS_check_policy2(ctx->policy, tst_info))
		goto err;
	
	/* Check message imprints. */
	if ((ctx->flags & TS_VFY_IMPRINT)
	    && !TS_check_imprints2(ctx->md_alg, ctx->imprint, ctx->imprint_len,
				  tst_info)) 
		goto err;

	/* Compute and check message imprints. */
	if ((ctx->flags & TS_VFY_DATA)
	    && (!TS_compute_imprint2(ctx->data, tst_info,
				    &md_alg, &imprint, &imprint_len)
	    || !TS_check_imprints2(md_alg, imprint, imprint_len, tst_info)))
		goto err;

	/* Check nonces. */
	if ((ctx->flags & TS_VFY_NONCE)
	    && !TS_check_nonces2(ctx->nonce, tst_info))
		goto err;

	/* Check whether TSA name and signer certificate match. */
	if ((ctx->flags & TS_VFY_SIGNER)
	    && tsa_name && !TS_check_signer_name2(tsa_name, signer))
		{
		TSerr(TS_F_INT_TS_RESP_VERIFY_TOKEN, TS_R_TSA_NAME_MISMATCH);
		goto err;
		}

	/* Check whether the TSA is the expected one. */
	if ((ctx->flags & TS_VFY_TSA_NAME)
	    && !TS_check_signer_name2(ctx->tsa_name, signer))
		{
		TSerr(TS_F_INT_TS_RESP_VERIFY_TOKEN, TS_R_TSA_UNTRUSTED);
		goto err;
		}

	ret = 1;
 err:
	X509_free(signer);
	X509_ALGOR_free(md_alg);
	OPENSSL_free(imprint);
	return ret;
	}

/*
 * Tries to extract a TS_TST_INFO structure from the PKCS7 token and
 * calls the internal int_TS_RESP_verify_token2 function for verifying it.
 */
int TS_RESP_verify_token2(TS_VERIFY_CTX *ctx, PKCS7 *token)
	{
	TS_TST_INFO *tst_info = PKCS7_to_TS_TST_INFO(token);
	int ret = 0;
	if (tst_info)
		{
		ret = int_TS_RESP_verify_token2(ctx, token, tst_info);
		TS_TST_INFO_free(tst_info);
		}
	return ret;
	}

/*
 * Verifies whether 'response' contains a valid response with regards 
 * to the settings of the context:
 *	- Gives an error message if the TS_TST_INFO is not present.
 *	- Calls _TS_RESP_verify_token to verify the token content.
 */
int TS_RESP_verify_response2(TS_VERIFY_CTX *ctx, TS_RESP *response)
	{
	PKCS7 *token = TS_RESP_get_token(response);
	TS_TST_INFO *tst_info = TS_RESP_get_tst_info(response);
	int ret = 0;

	/* Check if we have a successful TS_TST_INFO object in place. */
	if (!TS_check_status_info2(response)) goto err;

	/* Check the contents of the time stamp token. */
	if (!int_TS_RESP_verify_token2(ctx, token, tst_info))
		goto err;

	ret = 1;
 err:
	return ret;
	}

static time_t ASN1_GetTimeT(ASN1_TIME* time)
{
	if(!time || 0 >= time->length || !time->data)
		return 0;

    struct tm t;
    const char* str = (const char*) time->data;
    size_t i = 0;

    memset(&t, 0, sizeof(t));

	if (time->type == V_ASN1_UTCTIME) {
		///two digit year
        t.tm_year = (str[i++] - '0') * 10;
        t.tm_year += (str[i++] - '0');
        if (t.tm_year < 70)
            t.tm_year += 100;
    } else if (time->type == V_ASN1_GENERALIZEDTIME) {
		//four digit year
        t.tm_year = (str[i++] - '0') * 1000;
        t.tm_year+= (str[i++] - '0') * 100;
        t.tm_year+= (str[i++] - '0') * 10;
        t.tm_year+= (str[i++] - '0');
        t.tm_year -= 1900;
    }
    t.tm_mon = (str[i++] - '0') * 10;
    t.tm_mon += (str[i++] - '0') - 1; // -1 since January is 0 not 1.
    t.tm_mday = (str[i++] - '0') * 10;
    t.tm_mday+= (str[i++] - '0');
    t.tm_hour = (str[i++] - '0') * 10;
    t.tm_hour+= (str[i++] - '0');
    t.tm_min = (str[i++] - '0') * 10;
    t.tm_min += (str[i++] - '0');
    t.tm_sec = (str[i++] - '0') * 10;
    t.tm_sec += (str[i++] - '0');

    /* Note: we did not adjust the time based on time zone information */
    return mktime(&t);
}

// 验证时间戳时间在证书有效期内
long MYTOOL_VerifyTimeStampTime(
	unsigned char *tsinfo, 
	int tsinfolen, 
	unsigned char *cert, 
	int certlen)
{
	TS_TST_INFO *ts_tst_info = NULL;
	BJCA_TS_TST_INFO *bjca_tst_info = NULL;

	unsigned char *pszTmp = cert;
	X509 *x509 = d2i_X509(NULL, (const unsigned char **)&pszTmp, certlen);
	if (x509 == NULL)
		return SAR_CERTENCODEErr;

	unsigned char *pbTmp = tsinfo;
	ASN1_GENERALIZEDTIME *generalized_time = NULL;
	bjca_tst_info = d2i_BJCA_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, tsinfolen);
	if (!bjca_tst_info)
	{
		ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, tsinfolen);
		if(!ts_tst_info){
			if(x509){
				X509_free(x509);
				x509 = NULL;
			}
			if(bjca_tst_info){
				BJCA_TS_TST_INFO_free(bjca_tst_info);
				bjca_tst_info = NULL;
			}
			return SAR_IndataErr;
		}
		generalized_time = (ASN1_GENERALIZEDTIME *)TS_TST_INFO_get_time(ts_tst_info);
	}
	else
		generalized_time = bjca_tst_info->time;

	time_t tmTimeStampTime = ASN1_GetTimeT( generalized_time );
	time_t tmNotBefore = ASN1_GetTimeT( X509_get_notBefore(x509) );
	time_t tmNotAfter = ASN1_GetTimeT( X509_get_notAfter(x509) );

	if(x509){
		X509_free(x509);
		x509 = NULL;
	}
	if(bjca_tst_info){
		BJCA_TS_TST_INFO_free(bjca_tst_info);
		bjca_tst_info = NULL;
	}
	if(ts_tst_info){
		TS_TST_INFO_free(ts_tst_info);
		ts_tst_info = NULL;
	}

	if( tmNotBefore > tmTimeStampTime )
		return SAR_CERTNOTYETVALIDErr; 
	if( tmNotAfter < tmTimeStampTime )
		return SAR_CERTHASEXPIREDErr;

	return SAR_OK;
}

//


// =================================================================================
//  Macro Definition
#define NONCE_LENGTH              64   //

// =================================================================================
//  Class Definition
class KTTimeStampResp : public KTSEQUENCE2
{
public :
	KTTimeStampResp()
	{
		m_timeStampToken.setOptional();
		PLACE2OBJS( m_status, m_timeStampToken )
	}

	KTPKIStatusInfo         m_status;
	KTTimeStampToken        m_timeStampToken;        // OPTIONAL
};

// =================================================================================
//  Global Function Definition
static int g_nCall = 0;
#if defined(_WIN32)  
static HANDLE *lock_cs;
#elif defined(_LINUX)
static pthread_mutex_t *lock_cs;
static long *lock_count;
#endif

void locking_callback(int mode, int type, char *file, int line)
{
#if defined(_WIN32)
	if (mode & CRYPTO_LOCK)
	{
		WaitForSingleObject(lock_cs[type],INFINITE);
	}
	else
	{
		ReleaseMutex(lock_cs[type]);
	}
#elif defined(_LINUX)
	if (mode & CRYPTO_LOCK)
	{
		pthread_mutex_lock(&(lock_cs[type]));
		lock_count[type]++;
	}
	else
	{
		pthread_mutex_unlock(&(lock_cs[type]));
	}
#endif
}

unsigned long thread_id()
{
#if defined(_WIN32)
	return GetCurrentThreadId();
#elif defined(_LINUX)
	return (unsigned long)pthread_self();
#endif
}

void thread_setup(void)
{
#if defined(_WIN32)

	int i;
	lock_cs = (HANDLE*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(HANDLE));
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		lock_cs[i]=CreateMutex(NULL,FALSE,NULL);
	}
	CRYPTO_set_locking_callback((void (*)(int,int,const char *,int))locking_callback);

#elif defined(_LINUX)

	int i;
	lock_cs=(pthread_mutex_t*) OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
	lock_count=(long*) OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		lock_count[i]=0;
		pthread_mutex_init(&(lock_cs[i]),NULL);
	}
	
	CRYPTO_set_id_callback((unsigned long (*)())thread_id);
	CRYPTO_set_locking_callback((void (*)(int,int,const char *,int))locking_callback);
#endif
	
}

void thread_cleanup(void)
{
#if defined(_WIN32)

	int i;
	CRYPTO_set_locking_callback(NULL);
	for (i=0; i<CRYPTO_num_locks(); i++)
		CloseHandle(lock_cs[i]);
	OPENSSL_free(lock_cs);	
#elif defined(_LINUX)

	int i;
	CRYPTO_set_locking_callback(NULL);
	for (i=0; i<CRYPTO_num_locks(); i++)
	{
		pthread_mutex_destroy(&(lock_cs[i]));
	}
	OPENSSL_free(lock_cs);
	OPENSSL_free(lock_count);
#endif

}

TSA_EXPORT void TSA_Initialize ()
{
	if( 0 == g_nCall )
	{
		OpenSSL_add_all_algorithms();
		thread_setup();
	}
	g_nCall++;
}

TSA_EXPORT void TSA_UnInitialize()
{
	g_nCall--;
	if( 0 == g_nCall )
	{
		thread_cleanup();
		CONF_modules_unload(1);
		EVP_cleanup();
		ENGINE_cleanup();
		CRYPTO_cleanup_all_ex_data();
		ERR_remove_state(0);
		ERR_free_strings();

	}
}


void Base64Encode( const unsigned char * pData, unsigned int unDataLen, std::string & data )
{
	unsigned int unBuffLen = ( unDataLen + 2 ) / 3 * 4 + 10;
	data.resize( unBuffLen + 1 );
	K_B64_Code( EM_CODE, pData, unDataLen, (unsigned char *)&data[0], &unBuffLen );
	data.resize( unBuffLen + 1 );
	data[unBuffLen] = '\0';
}

void Base64Decode( const char * szData, unsigned int unDataLen, KTByteArray & rData )
{
	unsigned int unBuffLen = unDataLen;
	rData.resize( unBuffLen );
	K_B64_Code( EM_DECODE, (const unsigned char *)szData, unDataLen, &rData[0], &unBuffLen );
	rData.resize( unBuffLen );
}

void SHA1Digest( const char * pData, unsigned int unDataLen, KTByteArray & rDigest )
{
	unsigned int unDigestLen = 20;
	rDigest.resize( unDigestLen );
	K_SHA1_Digest( (const unsigned char *)pData, unDataLen, &rDigest[0], &unDigestLen );
	rDigest.resize( unDigestLen );
}

#define g2(p) (((p)[0] - '0') * 10 + (p)[1] - '0')
#define g4(p) (((p)[0] - '0') * 1000 + ((p)[1] - '0') * 100 + ((p)[2] - '0') * 10 + (p)[3] - '0')
void CertTime2String(asn1_string_st *stTime, char *pszTime)
{
	int nBias = -480;
	// 兼容linux
#ifdef _WIN32
	TIME_ZONE_INFORMATION TimeZoneInformation;
	if (GetTimeZoneInformation(&TimeZoneInformation) != TIME_ZONE_ID_INVALID)
		nBias = TimeZoneInformation.Bias;
#else
	struct timeb tstruct;
	ftime(&tstruct);
	nBias = tstruct.timezone;
#endif
	
	ASN1_UTCTIME *utcTime = ASN1_STRING_dup(stTime);
	int nLen = utcTime->length;
	int nYearLen = 2;
	if (nLen == 15)
		nYearLen = 4;
	int year, month, day, hour, minute, second;
	if (nYearLen == 2)
	{
		year = g2(utcTime->data);
		year < 50 ? year += 2000 : year += 1900;
	}
	else
	{
		year = g4(utcTime->data);
	}
	month = g2(utcTime->data + nYearLen);
	day = g2(utcTime->data + nYearLen + 2);
	hour = g2(utcTime->data + nYearLen + 4);
	minute = g2(utcTime->data + nYearLen + 6);
	second = g2(utcTime->data + nYearLen + 8);
	
	struct tm tmTmp;
	memset(&tmTmp, 0, sizeof(tm));
	tmTmp.tm_year = year - 1900;
	tmTmp.tm_mon = month - 1;
	tmTmp.tm_mday = day;
	tmTmp.tm_hour = hour;
	tmTmp.tm_min = minute;
	tmTmp.tm_sec = second;
	time_t timeTmp = mktime(&tmTmp);
	timeTmp -= nBias * 60;
	tmTmp = *localtime(&timeTmp);
	sprintf(pszTime, "%d-%02d-%02d %02d:%02d:%02d",
		tmTmp.tm_year + 1900, tmTmp.tm_mon + 1, tmTmp.tm_mday,
		tmTmp.tm_hour, tmTmp.tm_min, tmTmp.tm_sec);
	
	M_ASN1_UTCTIME_free(utcTime);
}





// =================================================================================
//  Interface Function Definition
// = TSA_GetErrorString
// Description: 根据错误号取错误信息
// Params: 
//   nErrNum         - [输入参数] 错误号
// Return: 返回错误信息串std::string 
TSA_EXPORT std::string TSA_GetErrorString( int nErrNum )
{
	switch( nErrNum )
	{
	case TSERR_INVALID_URL :
		return "无效服务器地址！";
	case TSERR_INVALID_SRC :
		return "无效的原文！";
	case TSERR_INVALID_OUTPUT :
		return "无效的输出参数！";
	case TSERR_TS_FORMAT_FAILED :
		return "传入的时间戳数据格式错误！";
	case TSERR_CER_FORMAT_FAILED:
		return "传入的证书格式错误";
	case TSERR_INVALID_CONNECT :
		return "连接服务器超时，请确认服务器地址是否正确！";
	case TSERR_SEND_TIMEOUT :
		return "发送数据超时，请重试！";
	case TSERR_RECV_TIMEOUT :
		return "接收数据超时，请重试！";
	case TSERR_INVALID_REQ:
		return "无效的请求数据";
	case TSERR_INVALID_METHOD:
		return "不支持的方法";
	case TSERR_SERVER :
		return "服务器内部错误！";
	case TSERR_ISSUER_FAILED :
		return "时间戳签发失败！";
	case TSERR_VERIFY_FAILED :
		return "时间戳验证失败！";
	case TS_MemErr:
		return "内存错误";
	case TS_HashErr:
		return "哈希运算错误";
	case TS_SetVerErr:
		return "设置请求版本错误";
	case TS_ANSIErr:
		return "ANSI对象错误";
	case TS_MSGAlgoErr:
		return "设置MSG算法错误 ";
	case TS_MSGErr:
		return "设置MSG内容错误";
	case TS_MSGImpErr:
		return "设置MSG标记错误";
	case TS_GenRandErr:
		return "生成随机数错误";
	case TS_SetRandErr:
		return "设置随机数错误";
	case TS_NewReqErr:
		return "生成时间戳请求数据错误";
	case TS_ReqErr:
		return "时间戳请求数据非法";
	case TS_RootCACertErr:
		return "根CA证书错误";
	case TS_CACertErr:
		return "中级CA证书错误";
	case TS_CertErr:
		return "签名证书错误";
	case TS_KeyErr:
		return "私钥错误";
	case TS_PolicyErr:
		return "设置签名策略错误";
	case TS_RespHashErr:
		return "设置RESP哈希算法错误";
	case TS_AccurErr:
		return "设置时间准确度错误";
	case TS_PreciErr:
		return "设置时间精确度错误";
	case TS_NewRespErr:
		return "生成时间戳响应数据错误";
	case TS_RespErr:
		return "时间戳响应数据非法";
	case TS_VerifyErr:
		return "验证时间戳失败";
	case TS_TstErr:
		return "获取TST信息失败";
	case TS_B64EnErr:
		return "B64编码失败";
	case TS_B64DeErr:
		return "B64解码失败";
	default :
		return "未知错误！";
	}
}

// = TSA_GetCurrentTime
// Description: 取当前标准时间
// Params: 
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   pTimeStampTime  - [输出参数] 时间戳时间，如：2006-11-21 15:05:16
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_GetCurrentTime( const char * szUrl, 
								  std::string * pCurrentTime )
{
	if( pCurrentTime == NULL )
		return TSERR_INVALID_OUTPUT;
	
	CTSASvrClient client;
	client.Initialize( szUrl );
	return client.GetTSATime( *pCurrentTime );
}


// = TSA_GetTSACert
// Description: 取时间戳服务器证书
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   pCertData       - [输出参数] 时间戳服务器证书完整数据——BASE64编码
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_GetTSACert( const char * szUrl, 
							  std::string * pCertData )
							  
{
	if( szUrl == NULL || strlen( szUrl ) == 0 )
		return TSERR_INVALID_URL;
	
	CTSASvrClient client;
	client.Initialize( szUrl );
	return client.GetTSACert( *pCertData );
}

// = TSA_TimeStamp
// Description: 制作时间戳
// Params:
//   szUrl           - [输入参数] 时间戳服务器地址及端口，如：192.168.1.60:103
//   szSrc           - [输入参数] 待制作时间戳的原文数据
//   pTimeStampData  - [输出参数] 颁发返回的时间戳完整数据——BASE64编码
//   unSrcLen        - [输入参数] 原文数据的长度，若以'\0'结尾传-1，默认为-1
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_TimeStamp( const char * szUrl, 
							 const char * szSrc, 
							 std::string * pTimeStampData, 
							 unsigned int unSrcLen /* = -1 */ )
{
	if( szUrl == NULL || strlen( szUrl ) == 0 )
		return TSERR_INVALID_URL;
	if( pTimeStampData == NULL )
		return TSERR_INVALID_OUTPUT;
	if( szSrc == NULL || ( unSrcLen == -1 && strlen( szSrc ) == 0 ) )
		return TSERR_INVALID_SRC;
	if( unSrcLen == -1 )
		unSrcLen = strlen( szSrc );

	// Make request.
	KTTimeStampReq tsReq;
	tsReq.m_version = 1;
	
	unsigned int unDigestLen = 20;
	KTByteArray rDigest( unDigestLen );
	K_SHA1_Digest( (const unsigned char *)szSrc, unSrcLen, &rDigest[0], &unDigestLen );
	rDigest.resize( unDigestLen );
	tsReq.m_messageImprint.m_hashedMessage.setValue( &rDigest[0], rDigest.size() );
	tsReq.m_messageImprint.m_hashAlgorithm.setTypeValue( ObjID_id_sha1 );
	
	tsReq.m_certReq = FALSE; //  default false
	tsReq.modifyLen();
	KTByteArray rReqData( tsReq.totalCount() );
	tsReq.output( &rReqData[0] );

	// Send request and get response.
	std::string reqData;
	std::string respData;
	KTByteArray rRespData;
	Base64Encode( &rReqData[0], rReqData.size(), reqData );
	
	CTSASvrClient client;
	client.Initialize( szUrl );
	int nErr = client.IssuerTimeStamp( reqData.c_str(), respData );
	if( nErr != 0 )
		return nErr;

	Base64Decode( respData.c_str(), respData.length(), rRespData );

	// Parse response.
	KTTimeStampResp tsResp;
	if( rRespData.size() == 0 )
		return 0;
	if( rRespData.size() <= 0 || tsResp.create( &rRespData[0], rRespData.size() ) != 0 )
		return TSERR_ISSUER_FAILED;
	if( tsResp.m_status.m_status.value() != 0 && tsResp.m_status.m_status.value() != 1 )
	{
		return TSERR_SERVER;
	}
	tsResp.modifyLen();
	KTByteArray rTimeStampData( tsResp.m_timeStampToken.totalCount() );
	tsResp.m_timeStampToken.output( &rTimeStampData[0] );
	Base64Encode( &rTimeStampData[0], rTimeStampData.size(), *pTimeStampData );
    return 0;
}

// = TSA_Verify
// Description: 验证时间戳
// Params: 
//   szSrc           - [输入参数] 时间戳原文
//   timeStampData   - [输入参数] 时间戳完整数据——BASE64编码
//   pCertData       - [输出参数] 时间戳服务器证书完整数据——BASE64编码
//   pTimeStampTime  - [输出参数] 时间戳时间，如：2006-11-21 15:05:16
//   unSrcLen        - [输入参数] 原文数据的长度，若以'\0'结尾传-1，默认为-1
// Return: 成功返回0，失败返回错误号
TSA_EXPORT int TSA_Verify( const char * szSrc, 
						  std::string timeStampData, 
						  std::string * pCertData, 
						  std::string * pTimeStampTime, 
						  unsigned int unSrcLen /* = -1 */ )
{
	if( timeStampData.length() == 0 )
		return TSERR_TS_FORMAT_FAILED;
	if( szSrc == NULL || ( unSrcLen == -1 && strlen( szSrc ) == 0 ) )
		return TSERR_INVALID_SRC;
	if( unSrcLen == -1 )
		unSrcLen = strlen( szSrc );

	// Parse timestamp data.
	KTByteArray rData;
	KTContentInfo contentInfo;
	Base64Decode( &timeStampData[0], timeStampData.length(), rData );
	if( contentInfo.create( &rData[0], rData.size() ) != 0 )
		return TSERR_TS_FORMAT_FAILED;

	KTSignedData * pSignedData = (KTSignedData *)contentInfo.valueObj();
	if( !( pSignedData->m_contentInfo.typeObj() == ObjID_id_ct_TSTInfo ) )
		return TSERR_TS_FORMAT_FAILED;

	KTDigestAlgorithmIdentifiers * pAlgoIdentifiers = &pSignedData->m_digestAlgorithms;
	KTCertificatesChoice * pCertsChoice = &pSignedData->m_certificates;
	KTSignerInfos * pSignerInfos = &pSignedData->m_signerInfos;
	KTTSTInfo * pTSTInfo = (KTTSTInfo *)pSignedData->m_contentInfo.valueObj();

	// Verify src digest sign.
	// pTSTInfo->m_messageImprint.m_hashAlgorithm.typeObj() == ObjID_id_sha1
	unsigned int unDigestLen = 20;
	KTByteArray rDigest( unDigestLen );
	K_SHA1_Digest( (const unsigned char *)szSrc, unSrcLen, &rDigest[0], &unDigestLen );
	rDigest.resize( unDigestLen );

	const KTSignerInfo * pSigner = pSignerInfos->m_siSet.indexObj( 0 );
	const KTCertificate * pCert = &pCertsChoice->m_certSet.indexObj( 0 )->m_certificate;

	unsigned int unBuffLen = 36;
	KTByteArray rBuff( unBuffLen );
	KTByteArray rPubKey;
	pCert->m_tbsCertificate.m_subjectPublicKeyInfo.getRSAPublicKey( rPubKey );
	CRSAPublicKey pubKey( rPubKey );
	K_RSA_Cipher( EM_VERIFY, &pubKey, pSigner->m_encryptedDigest.contentData(), 
		pSigner->m_encryptedDigest.contentCount(), &rBuff[0], &unBuffLen );
	if( memcmp( &rBuff[0], &rDigest[0], rDigest.size() ) != 0 )
		return TSERR_VERIFY_FAILED;

	// Parse timestamp time.
	struct tm tmTs;
	pTSTInfo->m_genTime.getValue( tmTs );
	char szTime[28];
	sprintf( szTime, "%04d/%02d/%02d %02d:%02d:%02d", tmTs.tm_year + 1900, tmTs.tm_mon + 1, 
		tmTs.tm_mday, tmTs.tm_hour, tmTs.tm_min, tmTs.tm_sec );
	if( pTimeStampTime )
		*pTimeStampTime = szTime;

	// Parse timestamp cert.
	if( pCertData )
	{
		KTByteArray rCertData;
		rCertData.resize( pCert->totalCount() );
		pCert->output( &rCertData[0] );
		Base64Encode( &rCertData[0], rCertData.size(), *pCertData );
	}
	return 0;
}

TSA_EXPORT int TS_CreateReq( const char* szUrl,const char* SrcData, std::string& ReqData)
{
	if( szUrl == NULL || strlen( szUrl ) == 0 )
		return TSERR_INVALID_URL;
	if ( SrcData == NULL || strlen(SrcData) == 0 )
		return TSERR_INVALID_SRC;
	CTSASvrClient client;
	client.Initialize( szUrl );
	
	return client.TS_Req( SrcData, ReqData);
}

TSA_EXPORT int TS_CreateResp( const char* szUrl,const char* ReqData, std::string& RespData)
{
	if( szUrl == NULL || strlen( szUrl ) == 0 )
		return TSERR_INVALID_URL;
	if ( ReqData == NULL || strlen(ReqData) == 0 )
		return TSERR_INVALID_REQ;
	CTSASvrClient client;
	client.Initialize( szUrl );
	return client.TS_Resp( ReqData, RespData);
}

TSA_EXPORT int TS_Verify( const char* szUrl, const char* SrcData, const char* RespData)
{
	if( szUrl == NULL || strlen( szUrl ) == 0 )
		return TSERR_INVALID_URL;
	if ( SrcData == NULL || strlen(SrcData) == 0 )
		return TSERR_INVALID_SRC;
	if ( RespData == NULL || strlen(RespData) == 0 )
		return TSERR_TS_FORMAT_FAILED;

	CTSASvrClient client;
	client.Initialize( szUrl );
	return client.TS_Verify( SrcData, RespData);
}


TSA_EXPORT int TS_Verify( const char* SrcData, const char* RespData, const char* cacert, const char* rootcacert)
{
	if ( SrcData == NULL || strlen(SrcData) == 0 )
		return TSERR_INVALID_SRC;
	if ( RespData == NULL || strlen(RespData) == 0 )
		return TSERR_TS_FORMAT_FAILED;
	if ( rootcacert == NULL || strlen(rootcacert) ==0)
		return TSERR_CER_FORMAT_FAILED;

	unsigned char byTSToken[10240];
	unsigned char byCacert[10240];
	unsigned char byRootcacert[10240];
	unsigned char* pbTmp = NULL;
	int nTSTokenLen , nCacert, nRootcacert, RetVal = 0, nTmpLen = 0;
	X509* pX509ca = NULL;
	X509* pX509rootca = NULL;
	unsigned char out[4096];
	long lOutLen = 0;
	TS_TST_INFO *ts_tst_info = NULL;
	BJCA_TS_TST_INFO *bjca_tst_info = NULL;
	int nHashLen = 0;

	STACK_OF(PKCS7_SIGNER_INFO) *sk = NULL;
	PKCS7_SIGNER_INFO * si = NULL;
	PKCS7_ISSUER_AND_SERIAL *ias = NULL;
	X509 *x509 = NULL;
	STACK_OF(X509) *certs = NULL;
	unsigned char signcert[10240] = {0};
	long lSignCertLen = 0;
	unsigned char tsinfo[4096];
	long lTSInfoLen = 0;

	unsigned int len = 0;
	memset(byTSToken, 0, sizeof(byTSToken));
	memset(byCacert, 0, sizeof(byCacert));
	memset(byRootcacert, 0, sizeof(byRootcacert));

	nTSTokenLen = CMailCoder::base64_decode(  const_cast<char*>(RespData), strlen(RespData), (char *)byTSToken );
	if (nTSTokenLen <= 0)
		return TSERR_TS_FORMAT_FAILED;

	if ( cacert && strlen(cacert) != 0 )
	{
		nCacert = CMailCoder::base64_decode( const_cast<char*>(cacert), strlen(cacert), (char *)byCacert );
		if (nCacert <= 0)
			return TSERR_CER_FORMAT_FAILED;
	}

	if ( rootcacert )
	{
		nRootcacert = CMailCoder::base64_decode( const_cast<char*>(rootcacert), strlen(rootcacert), (char *)byRootcacert );
		if (nRootcacert <= 0)
			return TSERR_CER_FORMAT_FAILED;
	}

	if ( cacert && strlen(cacert) != 0 )
	{
		pbTmp = byCacert;
		pX509ca = d2i_X509( NULL, (const unsigned char**)&pbTmp, nCacert);
	}

	pbTmp = byRootcacert;
	pX509rootca = d2i_X509( NULL, (const unsigned char**)&pbTmp, nRootcacert);

	int nRet = 0;
	PKCS7 *ts_token = NULL;
	TS_VERIFY_CTX *ts_verify_ctx = NULL;
	
	pbTmp = byTSToken;
	ts_token = d2i_PKCS7(NULL, (const unsigned char**)&pbTmp, nTSTokenLen);
	if (!ts_token)
	{
		RetVal = TS_RespErr;
		goto end;
	}

	//added yyx 2018-09-07
	if (OBJ_obj2nid(ts_token->type) != NID_pkcs7_signed)
	{
		//2018-10-08 验证签名、验证根证书
		nRet = MYSIGN_VerifySignedDataByP7_SM2_Ex(const_cast<char*>(RespData), NULL, 0, byCacert, nCacert);
		switch(nRet)
		{
		case SAR_OK:
			RetVal = 0;
			break;
		case SAR_CERTENCODEErr:
		case SAR_BASE64ENCODEDECODEErr:
			RetVal = TSERR_CER_FORMAT_FAILED;
			break;
		case SAR_CERTINVALIDErr:
			RetVal = TS_CACertErr;
			break;
		case SAR_PKCS7ENCODEErr:
			RetVal = TS_RespErr;
			break;
		case SAR_ECCVERIFYSIGNErr:
		default:
			RetVal = TS_VerifyErr;
			break;
		}
		//比对原文摘要
		nRet = MYSIGN_GetP7SignDataInfo_SM2(const_cast<char*>(RespData), 1, tsinfo, &lTSInfoLen);
		if(0 != nRet){
			RetVal = TS_VerifyErr;
			goto end;
		}
		pbTmp = tsinfo;
		bjca_tst_info = d2i_BJCA_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, lTSInfoLen);
		if (!bjca_tst_info)
		{
			ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, lTSInfoLen);
			if(!ts_tst_info){
				RetVal = TS_TstErr;
				goto end;
			}
			nHashLen = ts_tst_info->msg_imprint->hashed_msg->length;
			pbTmp = ts_tst_info->msg_imprint->hashed_msg->data;
		}
		else{
			nHashLen = bjca_tst_info->msg_imprint->hashed_msg->length;
			pbTmp = bjca_tst_info->msg_imprint->hashed_msg->data;
		}
		memset(out, 0, sizeof(out));
		nRet = MYSM_SM3((unsigned char*)SrcData, strlen(SrcData), out, &lOutLen);
		if(0 != nRet){
			RetVal = TS_RespHashErr;
			goto end;
		}
		if(lOutLen != nHashLen 
			|| 0 != memcmp(out, pbTmp, lOutLen)){
			RetVal = TS_VerifyErr;
			goto end;
		}

		//验证时间戳时间在证书的合法期限内
		nRet = MYSIGN_GetP7SignDataInfo_SM2(const_cast<char*>(RespData), 2, signcert, &lSignCertLen);
		if(0 != nRet){
			RetVal = TS_VerifyErr;
			goto end;
		}
		lSignCertLen = CMailCoder::base64_decode( const_cast<char*>((char *)signcert), lSignCertLen, (char *)signcert );
		if (lSignCertLen <= 0)
			return TSERR_CER_FORMAT_FAILED;

		nRet = MYTOOL_VerifyTimeStampTime(tsinfo, (int)lTSInfoLen, signcert, (int)lSignCertLen);
		if(0 != nRet){
			RetVal = TS_VerifyErr;
		}
		goto end;
	}
	//

	ts_verify_ctx = TS_VERIFY_CTX_new();
	if (!ts_verify_ctx)
	{
		RetVal = TS_MemErr;
		goto end;
	}
	ts_verify_ctx->flags = TS_VFY_VERSION | TS_VFY_DATA | TS_VFY_SIGNATURE | TS_VFY_SIGNER;
	ts_verify_ctx->data = BIO_new(BIO_s_mem());
	if (!ts_verify_ctx->data)
	{
		RetVal = TS_MemErr;
		goto end;
	}
	if (BIO_write(ts_verify_ctx->data, SrcData, strlen(SrcData) ) != strlen(SrcData) )
	{
		RetVal = TS_RespErr;
		goto end;
	}
	ts_verify_ctx->store = X509_STORE_new();
	if (!ts_verify_ctx->store)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	if ( pX509rootca )
	{
		nRet = X509_STORE_add_cert(ts_verify_ctx->store, pX509rootca);
		if (!nRet)
		{
			RetVal = TS_RootCACertErr;
			goto end;
		}
	}
	if ( pX509ca )
	{
		nRet = X509_STORE_add_cert(ts_verify_ctx->store, pX509ca);
		if (!nRet)
		{
			RetVal = TS_CACertErr;
			goto end;
		}
	}

	//TS_RESP_verify_token2不验证证书有效期
	nRet = TS_RESP_verify_token2(ts_verify_ctx, ts_token);
	if (!nRet)
	{
		RetVal = TS_VerifyErr;
		goto end;
	}

	//验证时间戳时间在证书有效期内
	sk=PKCS7_get_signer_info(ts_token);
	if( sk == NULL )
	{
		RetVal = TS_RespErr;
		goto end;
	}
	else
	{
		if( sk_PKCS7_SIGNER_INFO_num(sk) < 1 )
		{
			 RetVal = TS_RespErr;
			 goto end;
		}
		si = sk_PKCS7_SIGNER_INFO_value(sk, 0);
		certs = ts_token->d.signed_and_enveloped->cert;
		ias = si->issuer_and_serial;
		x509 = X509_find_by_issuer_and_serial(certs, ias->issuer, ias->serial);
		if( !x509 )
		{
			RetVal = TS_RespErr;
			goto end;
		}
		pbTmp = out;
		len = i2d_X509(x509, &pbTmp);
		if(0 >= len){
			RetVal = TS_RespErr;
			goto end;
		}
		nTmpLen = ts_token->d.sign->contents->d.other->value.octet_string->length;
		pbTmp =	ts_token->d.sign->contents->d.other->value.octet_string->data;
		if(0 != MYTOOL_VerifyTimeStampTime(pbTmp, nTmpLen, out, len))
		{
			RetVal = TS_VerifyErr;
			goto end;
		}
	}
	
end:
	if (pX509ca)
	{
		X509_free(pX509ca);
	}
	if (pX509rootca)
	{
		X509_free(pX509rootca);
	}
	if (ts_token)
		PKCS7_free(ts_token);
	if (ts_verify_ctx)
		TS_VERIFY_CTX_free(ts_verify_ctx);
	if(ts_tst_info)
		TS_TST_INFO_free(ts_tst_info);
	if(bjca_tst_info)
		BJCA_TS_TST_INFO_free(bjca_tst_info);

	return RetVal;
}

TSA_EXPORT int TS_Parse( const char* szUrl, const char* RespData,  std::string& ParseData , int flag)
{
	if( szUrl == NULL || strlen( szUrl ) == 0 )
		return TSERR_INVALID_URL;
	if ( RespData == NULL || strlen(RespData) == 0 )
		return TSERR_TS_FORMAT_FAILED;
	
	CTSASvrClient client;
	client.Initialize( szUrl );
	return client.TS_Parse( RespData, ParseData, flag);
	return 0;
}

TSA_EXPORT int TS_Parse(const char* RespData,  std::string& ParseData , int flag)
{
	if ( RespData == NULL )
		return TSERR_TS_FORMAT_FAILED;
	unsigned char byTSToken[10240];
	char pszTime[100];
	memset(byTSToken, 0, sizeof(byTSToken));
	int nTSTokenLen, RetVal = 0;
	nTSTokenLen = CMailCoder::base64_decode( const_cast<char*>(RespData), strlen(RespData), (char *)byTSToken);
	if (nTSTokenLen <= 0)
	{
		return TSERR_TS_FORMAT_FAILED;
	}

	PKCS7 *ts_token = NULL;
	TS_TST_INFO *ts_tst_info = NULL;
	BJCA_TS_TST_INFO *bjca_tst_info = NULL;
	STACK_OF(PKCS7_SIGNER_INFO) *sk = NULL;
	PKCS7_SIGNER_INFO * si = NULL;
	PKCS7_ISSUER_AND_SERIAL *ias = NULL;
	X509 *x509 = NULL;
	STACK_OF(X509) *cert = NULL;
	int len = 0;
	unsigned char *pbTmp;
	int nTSTInfoLen;
	
	pbTmp = byTSToken;
	ts_token = d2i_PKCS7( NULL, (const unsigned char **)&pbTmp, nTSTokenLen );
	if (!ts_token)
	{
		RetVal = TS_RespErr;
		goto end;
	}

	//added yyx 2018-09-07
	if (OBJ_obj2nid(ts_token->type) != NID_pkcs7_signed)
	{
		//SM2
		unsigned char out[4096] = {0};
		long lOutLen = 0;
		RetVal = MYSIGN_GetP7SignDataInfo_SM2(const_cast<char*>(RespData), flag+1, out, &lOutLen);
		switch(RetVal)
		{
		case SAR_OK:
			RetVal = 0;
			break;
		case SAR_CERTENCODEErr:
		case SAR_BASE64ENCODEDECODEErr:
			RetVal = TSERR_CER_FORMAT_FAILED;
			break;
		case SAR_PKCS7ENCODEErr:
		default:
			RetVal = TS_RespErr;
			break;
		}
		if(0 != RetVal)
			goto end;
		if(0 == flag)
		{
			pbTmp = out;
			ASN1_GENERALIZEDTIME *generalized_time = NULL;
			bjca_tst_info = d2i_BJCA_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, lOutLen);
			if (!bjca_tst_info)
			{
				ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, lOutLen);
				if(!ts_tst_info){
					RetVal = TS_TstErr;
					goto end;
				}
				generalized_time = (ASN1_GENERALIZEDTIME *)TS_TST_INFO_get_time(ts_tst_info);
			}
			else
				generalized_time = bjca_tst_info->time;
			if (!generalized_time)
			{
				RetVal = TS_TstErr;
				goto end;
			}
			CertTime2String(generalized_time, pszTime);
			ParseData = pszTime;
		}
		else
		{
			out[lOutLen] = '\0';
			ParseData = (char *)out;
		}
		goto end;
	}

	//RSA
	if( 0 == flag)
	{
		nTSTInfoLen = ts_token->d.sign->contents->d.other->value.octet_string->length;
		pbTmp =	ts_token->d.sign->contents->d.other->value.octet_string->data;
		ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, nTSTInfoLen);
		if (!ts_tst_info)
		{
			RetVal = TS_TstErr;
			goto end;
		}
		ASN1_GENERALIZEDTIME *generalized_time;
		generalized_time = (ASN1_GENERALIZEDTIME *)TS_TST_INFO_get_time(ts_tst_info);
		if (!generalized_time)
		{
			RetVal = TS_TstErr;
			goto end;
		}
		CertTime2String(generalized_time, pszTime);
		ParseData = pszTime;
	}
	else if( 1 == flag)
	{
		char CertB64[4096];
		memset( CertB64, 0 , sizeof( CertB64));
		sk=PKCS7_get_signer_info(ts_token);
		if( sk == NULL )
		{
			RetVal = TS_RespErr;
			goto end;
		}
		else
		{
			if( sk_PKCS7_SIGNER_INFO_num(sk) < 1 )
			{
			  RetVal = TS_RespErr;
			  goto end;
			}
			si = sk_PKCS7_SIGNER_INFO_value(sk,0);
			cert = ts_token->d.signed_and_enveloped->cert;
			ias = si->issuer_and_serial;
			x509 = X509_find_by_issuer_and_serial(cert,ias->issuer,ias->serial);
			if( !x509 )
			{
				RetVal = TS_RespErr;
				goto end;
			}
			pbTmp = NULL;
			len = i2d_X509( x509, &pbTmp );
			int B64Len;
			B64Len = CMailCoder::base64_encode((char *)pbTmp, len, CertB64);
			if (B64Len <= 0)
			{
			  RetVal = TS_RespErr;
			  goto end;
			}
			ParseData = CertB64;
			free( pbTmp );
			pbTmp = NULL;
		}
	}
	else if (2 == flag)
	{
		STACK_OF(PKCS7_SIGNER_INFO) *sk = NULL;
		PKCS7_SIGNER_INFO * si = NULL;
		PKCS7_ISSUER_AND_SERIAL *ias = NULL;
		X509 *x509 = NULL;
		STACK_OF(X509) *cert = NULL;
		//char CertB64[4096] = {0};
		int len = 0;
		sk=PKCS7_get_signer_info(ts_token);
		if( sk == NULL )
		{
			RetVal = TS_RespErr;
			goto end;
		}
		else
		{
			if( sk_PKCS7_SIGNER_INFO_num(sk) < 1 )
			{
				RetVal = TS_RespErr;
				goto end;
			}
			si = sk_PKCS7_SIGNER_INFO_value(sk,0);
			char bySignData[4096] = {0};
			int nSignLen = si->enc_digest->length;
			unsigned char* pSignData = si->enc_digest->data;
			int B64len = CMailCoder::base64_encode((char *)pSignData, nSignLen, bySignData);
			bySignData[B64len] = '\0';
			//strcpy(pszTime, bySignData);
			//ParseData = CertB64;
			ParseData = bySignData;
		}
	}
	
end:
	if(ts_tst_info)
		TS_TST_INFO_free(ts_tst_info);
	if(bjca_tst_info)
		BJCA_TS_TST_INFO_free(bjca_tst_info);
	if (ts_token)
		PKCS7_free(ts_token);

	return RetVal;
}

// =================================================================================
//  1.00    2006-11-21    Created By LinZaoGang
//  File End 
