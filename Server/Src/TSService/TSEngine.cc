#include "TSEngine.h"
#include "MailCoder.h"
#include "FJCAApiConst.h"
#include "TssDefine.h"
#include "MyToolLibFunc.h"
#include "pk7_sm2_sign.h"
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
#include <openssl/objects.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <vector>
using std::vector;
typedef unsigned char BYTE;
typedef vector<BYTE> ByteArray;
#define OID_SM3 "1.2.156.10197.6.1.4.2"
#define OID_SHA1 "sha1"
#define LEN_SM3_DIGEST 32

#define	NONCE_LENGTH		64
#define g2(p) (((p)[0] - '0') * 10 + (p)[1] - '0')
#define g4(p) (((p)[0] - '0') * 1000 + ((p)[1] - '0') * 100 + ((p)[2] - '0') * 10 + (p)[3] - '0')
void CertTime2String(asn1_string_st *stTime, char *pszTime);
TS_RESP *TS_RESP_create_sm2_response(TS_RESP_CTX *ctx, BIO *req_bio, CTSEngine *engine);
static ASN1_INTEGER *create_nonce(int bits);
bool HashData(unsigned char *in, int inlen, unsigned char *out, int *outlen);
static int get_serial(const char *serialfile, std::string & serialNumber);
static ASN1_INTEGER *next_serial(const char *serialfile);
static int save_ts_serial(const char *serialfile, ASN1_INTEGER *serial);
static ASN1_INTEGER* my_serial_cb(TS_RESP_CTX *ctx, void *data);
ASN1_INTEGER *  tsa_serial_cb(TS_RESP_CTX *ctx, void *data);
static void TS_RESP_CTX_init(TS_RESP_CTX *ctx);
static int TS_RESP_check_sm2_request(TS_RESP_CTX *ctx);
static ASN1_OBJECT *TS_RESP_get_policy(TS_RESP_CTX *ctx);
static TS_TST_INFO *TS_RESP_create_tst_info(TS_RESP_CTX *ctx,
					    ASN1_OBJECT *policy);
static int TS_RESP_sm2_sign(TS_RESP_CTX *ctx, CTSEngine *engine);						
static int TS_RESP_process_extensions(TS_RESP_CTX *ctx);						
static void TS_RESP_CTX_cleanup(TS_RESP_CTX *ctx);
static ASN1_GENERALIZEDTIME *
TS_RESP_set_genTime_with_precision(ASN1_GENERALIZEDTIME *asn1_time, 
				   long sec, long usec, unsigned precision);

CTSEngine::CTSEngine()
{
	m_RsaCert = NULL;
	m_RsaPkey = NULL;
	m_RsaCa = NULL;
	m_RsaRootCa = NULL;
	m_Sm2Cert = NULL;
	m_Sm2Pkey = NULL;
	m_Sm2Ca = NULL;
	m_Sm2RootCa = NULL;	
}

CTSEngine::~CTSEngine()
{
	if( m_RsaCert )
		X509_free( m_RsaCert );
	if( m_RsaPkey )
		EVP_PKEY_free( m_RsaPkey );
	if( m_RsaCa )
		X509_free( m_RsaCa );
	if( m_RsaRootCa )
		X509_free( m_RsaRootCa );
	if( m_Sm2Ca )
		X509_free( m_Sm2Ca );
	if( m_Sm2RootCa )
		X509_free( m_Sm2RootCa );
	
}


void CTSEngine::InitSerCer(X509 *RsaCert, EVP_PKEY *RsaPKey, X509 *RsaCa, X509 *RsaRootCa, 
	time_t RsaBeforeValid, time_t RsaAfterValid, char *Sm2Cert, char *Sm2PKey, 
	X509 *Sm2Ca, X509 *Sm2RootCa, time_t Sm2BeforeValid, time_t Sm2AfterValid)
{	
	m_RsaCert = RsaCert;
	m_RsaPkey = RsaPKey;
	m_RsaCa = RsaCa;
	m_RsaRootCa = RsaRootCa;
	m_RsaBeforeValid = RsaBeforeValid;
	m_RsaAfterValid = RsaAfterValid;
	m_Sm2Cert = Sm2Cert;
	m_Sm2Pkey = Sm2PKey;
	m_Sm2Ca = Sm2Ca;
	m_Sm2RootCa = Sm2RootCa;
	m_Sm2BeforeValid = Sm2BeforeValid;
	m_Sm2AfterValid = Sm2AfterValid;
}


int CTSEngine::CreateTSResp( unsigned char* tsreq, int tsreqlen, X509* rootcacert, X509* cacert,
                              X509* cert, EVP_PKEY *pkey, unsigned char *tsresp, int *tsresplen,	unsigned char *tstoken,
                              int *tstokenlen, std::string & serialNumber)
{

	bool bRet = false;
	int nRet = 0, RetVal = 0;
	STACK_OF(X509) *x509_cacerts = NULL;
	BIO *req_bio = NULL;
	ASN1_OBJECT *policy_obj1 = NULL;
	ASN1_OBJECT *policy_obj2 = NULL;
	ASN1_OBJECT *policy_obj3 = NULL;

	TS_RESP *ts_resp = NULL;
	TS_RESP_CTX *ts_resp_ctx = NULL;
	unsigned char *pbTmp;
	OpenSSL_add_all_algorithms();

	// cert chain
	x509_cacerts = sk_X509_new_null();
	if (!x509_cacerts)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	if (rootcacert != NULL)
	{
		nRet = sk_X509_push(x509_cacerts, rootcacert);
		if (!nRet)
		{
			RetVal = SAR_CERTENCODEErr;
			goto end;
		}
	}

	if (cacert != NULL)
	{
		nRet = sk_X509_push(x509_cacerts, cacert);
		if (!nRet)
		{
			RetVal = SAR_CERTENCODEErr;
			goto end;
		}
	}

	// ts response ********************************************************
	// req
	req_bio = BIO_new(BIO_s_mem());
	if (!req_bio)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	BIO_set_close(req_bio, BIO_CLOSE); // BIO_free() free BUF_MEM
	if (BIO_write(req_bio, tsreq, tsreqlen) != tsreqlen)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	// Setting up response generation context.
	ts_resp_ctx = TS_RESP_CTX_new();
	if (!ts_resp_ctx)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	// Setting serial number provider callback.
	TS_RESP_CTX_set_serial_cb(ts_resp_ctx, tsa_serial_cb, (void*)"tsa_serial_file");

	// Setting TSA signer certificate chain.
	if (x509_cacerts != NULL)
	{
		nRet = TS_RESP_CTX_set_certs(ts_resp_ctx, x509_cacerts);
		if (!nRet)
		{
			RetVal = SAR_CERTENCODEErr;
			goto end;
		}
	}

	// Setting TSA signer certificate.
	nRet = TS_RESP_CTX_set_signer_cert(ts_resp_ctx, cert);
	if (!nRet)
	{
		RetVal = SAR_CERTENCODEErr;
		goto end;
	}

	// Setting TSA signer private key.
	nRet = TS_RESP_CTX_set_signer_key(ts_resp_ctx, pkey);
	if (!nRet)
	{
		RetVal = SAR_KeyInfoTypeErr;
		goto end;
	}

	// Setting default policy OID.
	policy_obj1 = OBJ_txt2obj("1.2.3.4.1", 0);
	if (!policy_obj1)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	nRet = TS_RESP_CTX_set_def_policy(ts_resp_ctx, policy_obj1);
	if (!nRet)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	// Setting acceptable policy OIDs.
	policy_obj2 = OBJ_txt2obj("1.2.3.4.5.6", 0);
	if (!policy_obj2)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	nRet = TS_RESP_CTX_add_policy(ts_resp_ctx, policy_obj2);
	if (!nRet)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	policy_obj3 = OBJ_txt2obj("1.2.3.4.5.7", 0);
	if (!policy_obj3)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	nRet = TS_RESP_CTX_add_policy(ts_resp_ctx, policy_obj3);
	if (!nRet)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	// Setting the acceptable one-way hash algorithms.
	nRet = TS_RESP_CTX_add_md(ts_resp_ctx, EVP_md5());
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	nRet = TS_RESP_CTX_add_md(ts_resp_ctx, EVP_sha1());
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// Setting guaranteed time stamp accuracy.
	int secs, millis, micros;
	secs = 1;
	millis = 500;
	micros = 100;
	nRet = TS_RESP_CTX_set_accuracy(ts_resp_ctx, secs, millis, micros);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// Setting the precision of the time.
	unsigned clock_precision_digits;
	clock_precision_digits = 0;
	nRet = TS_RESP_CTX_set_clock_precision_digits(ts_resp_ctx, clock_precision_digits);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}
	// Setting the ordering flaf if requested.
	TS_RESP_CTX_add_flags(ts_resp_ctx, TS_ORDERING);

	// Setting the TSA name required flag if requested.
	TS_RESP_CTX_add_flags(ts_resp_ctx, TS_TSA_NAME);

	// Creating the response.
	ts_resp = TS_RESP_create_response(ts_resp_ctx, req_bio);
	if (!ts_resp)
	{	
		if (!nRet)
		{
			RetVal = SAR_SIGNTSErr;
			goto end;
		}
	}
	// output ts response
	unsigned char byOut[10240];
	memset(byOut, 0, sizeof(byOut));
	int nOutLen;
	pbTmp = byOut;
	nOutLen = i2d_TS_RESP(ts_resp, &pbTmp);
	if (nOutLen <= 0)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}

	memcpy(tsresp, byOut, nOutLen);
	*tsresplen = nOutLen;

	// output ts token
	PKCS7 *ts_token;
	ts_token = TS_RESP_get_token(ts_resp);
	if (!ts_token)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}

	memset(byOut, 0, sizeof(byOut));
	pbTmp = byOut;
	nOutLen = i2d_PKCS7(ts_token, &pbTmp);
	if (nOutLen <= 0)
	{
		RetVal = STF_TS_MALFORMAT;
		goto end;
	}

	get_serial("ts_serial",serialNumber);
	memcpy(tstoken, byOut, nOutLen);
	*tstokenlen = nOutLen;

end:

	if (req_bio)
		BIO_free(req_bio);
	if (policy_obj1)
		ASN1_OBJECT_free(policy_obj1);
	if (policy_obj2)
		ASN1_OBJECT_free(policy_obj2);
	if (policy_obj3)
		ASN1_OBJECT_free(policy_obj3);

	if (ts_resp)
		TS_RESP_free(ts_resp);
	if (ts_resp_ctx)
		TS_RESP_CTX_free(ts_resp_ctx);

	return RetVal;

}



int CTSEngine::CreateSM2TSResp( unsigned char *tsreq, int tsreqlen, X509 *rootcacert, X509 *cacert,
                              char *cert, char *pkey, unsigned char *tsresp, int *tsresplen, unsigned char *tstoken,
                              int *tstokenlen, std::string &serialNumber)
{
	bool bRet = false;
	int nRet = 0, RetVal = 0;
	STACK_OF(X509) *x509_cacerts = NULL;
	BIO *req_bio = NULL;
	ASN1_OBJECT *policy_obj1 = NULL;
	ASN1_OBJECT *policy_obj2 = NULL;
	ASN1_OBJECT *policy_obj3 = NULL;

	TS_RESP *ts_resp = NULL;
	TS_RESP_CTX *ts_resp_ctx = NULL;
	unsigned char *pbTmp;
	
	char byCert[8192] = { 0 };
	int nbyCertLen = CMailCoder::base64_decode( cert, strlen(cert), byCert );
	if ( nbyCertLen <= 0 )
	{
		return SAR_CERTENCODEErr;
	}
	pbTmp = (unsigned char*)byCert;
	X509* x509cert = d2i_X509( NULL, (const unsigned char **)&pbTmp, nbyCertLen);
	if(x509cert == NULL)
	{
		return SAR_CERTENCODEErr;
	}

	// cert chain
	x509_cacerts = sk_X509_new_null();
	if (!x509_cacerts)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	if (rootcacert != NULL)
	{
		nRet = sk_X509_push(x509_cacerts, rootcacert);
		if (!nRet)
		{
			RetVal = SAR_CERTENCODEErr;
			goto end;
		}
	}

	if (cacert != NULL)
	{
		nRet = sk_X509_push(x509_cacerts, cacert);
		if (!nRet)
		{
			RetVal = SAR_CERTENCODEErr;
			goto end;
		}
	}
	
	// ts response ********************************************************
	// req
	req_bio = BIO_new(BIO_s_mem());
	if (!req_bio)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	BIO_set_close(req_bio, BIO_CLOSE); // BIO_free() free BUF_MEM
	if (BIO_write(req_bio, tsreq, tsreqlen) != tsreqlen)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}
	

	// Setting up response generation context.
	ts_resp_ctx = TS_RESP_CTX_new();
	if (!ts_resp_ctx)
	{
		RetVal = SAR_MemoryErr;
		goto end;
	}

	// Setting serial number provider callback.
	TS_RESP_CTX_set_serial_cb(ts_resp_ctx, tsa_serial_cb, (void*)"tsa_serial_file");

	// Setting TSA signer certificate chain.
	if (x509_cacerts != NULL)
	{
		nRet = TS_RESP_CTX_set_certs(ts_resp_ctx, x509_cacerts);
		if (!nRet)
		{
			RetVal = SAR_CERTENCODEErr;
			goto end;
		}
	}

 	// Setting TSA signer certificate.
	nRet = TS_RESP_CTX_set_signer_cert(ts_resp_ctx, x509cert);
	if (!nRet)
	{
		RetVal = SAR_CERTENCODEErr;
		goto end;
	}
	
	/*
	// Setting TSA signer private key.
	nRet = TS_RESP_CTX_set_signer_key(ts_resp_ctx, pkey);
	if (!nRet)
	{
		RetVal = SAR_KeyInfoTypeErr;
		goto end;
	} */

	// Setting default policy OID.
	policy_obj1 = OBJ_txt2obj("1.2.3.4.1", 0);
	if (!policy_obj1)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	nRet = TS_RESP_CTX_set_def_policy(ts_resp_ctx, policy_obj1);
	if (!nRet)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	// Setting acceptable policy OIDs.
	policy_obj2 = OBJ_txt2obj("1.2.3.4.5.6", 0);
	if (!policy_obj2)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	nRet = TS_RESP_CTX_add_policy(ts_resp_ctx, policy_obj2);
	if (!nRet)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	policy_obj3 = OBJ_txt2obj("1.2.3.4.5.7", 0);
	if (!policy_obj3)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}

	nRet = TS_RESP_CTX_add_policy(ts_resp_ctx, policy_obj3);
	if (!nRet)
	{
		RetVal = STF_TS_UNACCEPTED_POLICY;
		goto end;
	}
	
	
 	// Setting the acceptable one-way hash algorithms.
	nRet = TS_RESP_CTX_add_md(ts_resp_ctx, EVP_md5());
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	nRet = TS_RESP_CTX_add_md(ts_resp_ctx, EVP_sha1());
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	} 

	// Setting guaranteed time stamp accuracy.
	int secs, millis, micros;
	secs = 1;
	millis = 500;
	micros = 100;
	nRet = TS_RESP_CTX_set_accuracy(ts_resp_ctx, secs, millis, micros);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// Setting the precision of the time.
	unsigned clock_precision_digits;
	clock_precision_digits = 0;
	nRet = TS_RESP_CTX_set_clock_precision_digits(ts_resp_ctx, clock_precision_digits);
	if (!nRet)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}
	// Setting the ordering flaf if requested.
	TS_RESP_CTX_add_flags(ts_resp_ctx, TS_ORDERING);

	// Setting the TSA name required flag if requested.
	TS_RESP_CTX_add_flags(ts_resp_ctx, TS_TSA_NAME);
	
	// Creating the response.
	ts_resp = TS_RESP_create_sm2_response(ts_resp_ctx, req_bio, this);
	if (!ts_resp)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	// output ts response
	unsigned char byOut[10240];
	memset(byOut, 0, sizeof(byOut));
	int nOutLen;
	pbTmp = byOut;
	nOutLen = i2d_TS_RESP(ts_resp, &pbTmp);
	if (nOutLen <= 0)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	memcpy(tsresp, byOut, nOutLen);
	*tsresplen = nOutLen;

	// output ts token
	PKCS7 *ts_token;
	ts_token = TS_RESP_get_token(ts_resp);
	if (!ts_token)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}

	memset(byOut, 0, sizeof(byOut));
	pbTmp = byOut;
	nOutLen = i2d_PKCS7(ts_token, &pbTmp);
	if (nOutLen <= 0)
	{
		RetVal = STF_TS_SYSTEM_FAILURE;
		goto end;
	}
	memcpy(tstoken, byOut, nOutLen);
	*tstokenlen = nOutLen;
	
	get_serial("tsa_serial_file",serialNumber);
end:

	if (req_bio)
		BIO_free(req_bio);
	if (policy_obj1)
		ASN1_OBJECT_free(policy_obj1);
	if (policy_obj2)
		ASN1_OBJECT_free(policy_obj2);
	if (policy_obj3)
		ASN1_OBJECT_free(policy_obj3);

	if (ts_resp)
		TS_RESP_free(ts_resp);
	if (ts_resp_ctx)
		TS_RESP_CTX_free(ts_resp_ctx);

	return RetVal;
}


int get_serial(const char *serialfile, std::string &serialNumber)
{
	int nRet = -1;
	BIO *in = NULL;
	ASN1_INTEGER *serial = NULL;
	BIGNUM *bn = NULL;
	char *Dec = NULL;
	char *Hex = NULL;

	if (!(serial = ASN1_INTEGER_new())) goto err;

	if (!(in = BIO_new_file(serialfile, "r")))
	{
		ERR_clear_error();
		if (!ASN1_INTEGER_set(serial, 1)) goto err;
	}
	else
	{
		char buf[1024];
		if (!a2i_ASN1_INTEGER(in, serial, buf, sizeof(buf))) goto err;
		if (!(bn = ASN1_INTEGER_to_BN(serial, NULL))) goto err;
		Hex = BN_bn2hex(bn);
		Dec = BN_bn2dec(bn);
		serialNumber = Dec;
		ASN1_INTEGER_free(serial);
		serial = NULL;

	}

	nRet = 0;
err:
	if (nRet)
	{
		ASN1_INTEGER_free(serial);
		serial = NULL;
	}
	BIO_free(in);
	BN_free(bn);
	OPENSSL_free(Dec);
	OPENSSL_free(Hex);
	return nRet;
}


ASN1_INTEGER *next_serial(const char *serialfile)
{
	int nRet = 0;
	BIO *in = NULL;
	ASN1_INTEGER *serial = NULL;
	BIGNUM *bn = NULL;

	if (!(serial = ASN1_INTEGER_new())) goto err;

	if (!(in = BIO_new_file(serialfile, "r")))
	{
		ERR_clear_error();
		if (!ASN1_INTEGER_set(serial, 1)) goto err;
	}
	else
	{
		char buf[1024];
		if (!a2i_ASN1_INTEGER(in, serial, buf, sizeof(buf))) goto err;
		if (!(bn = ASN1_INTEGER_to_BN(serial, NULL))) goto err;
		ASN1_INTEGER_free(serial);
		serial = NULL;
		if (!BN_add_word(bn, 1)) goto err;
		if (!(serial = BN_to_ASN1_INTEGER(bn, NULL))) goto err;
	}

	nRet = 1;
err:
	if (!nRet)
	{
		ASN1_INTEGER_free(serial);
		serial = NULL;
	}
	BIO_free(in);
	BN_free(bn);
	return serial;
}

static int save_ts_serial(const char *serialfile, ASN1_INTEGER *serial)
{
	int nRet = 0;
	BIO *out = NULL;
	if (!(out = BIO_new_file(serialfile, "w"))) goto err;
	if (i2a_ASN1_INTEGER(out, serial) <= 0) goto err;
	if (BIO_puts(out, "\n") <= 0) goto err;
	nRet = 1;
err:
	if (!nRet)
		nRet = -1;
	BIO_free(out);
	return nRet;
}

ASN1_INTEGER *  tsa_serial_cb(TS_RESP_CTX *ctx, void *data)
{
	const char *serial_file = (const char *) data;
	ASN1_INTEGER *serial = next_serial(serial_file);

	if (!serial)
	{
		TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
		                            "Error during serial number "
		                            "generation.");
		TS_RESP_CTX_add_failure_info(ctx,
		                             TS_INFO_ADD_INFO_NOT_AVAILABLE);
	}
	else
		save_ts_serial(serial_file, serial);

	return serial;
}


void CertTime2String(asn1_string_st *stTime, char *pszTime)
{
	int nBias = -480;
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


/* Main entry method of the response generation. */
TS_RESP *TS_RESP_create_sm2_response(TS_RESP_CTX *ctx, BIO *req_bio, CTSEngine *engine)
	{
	ASN1_OBJECT *policy;
	TS_RESP *response;
	int result = 0;

	TS_RESP_CTX_init(ctx);

	/* Creating the response object. */
	if (!(ctx->response = TS_RESP_new())) 
		{
		TSerr(TS_F_TS_RESP_CREATE_RESPONSE, ERR_R_MALLOC_FAILURE);
		goto end;
		}

	/* Parsing DER request. */
	if (!(ctx->request = d2i_TS_REQ_bio(req_bio, NULL)))
		{
		TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
					    "Bad request format or "
					    "system error.");
		TS_RESP_CTX_add_failure_info(ctx, TS_INFO_BAD_DATA_FORMAT);
		goto end;
		}
		
	/* Setting default status info. */
	if (!TS_RESP_CTX_set_status_info(ctx, TS_STATUS_GRANTED, NULL))
		goto end;
	
	/* Checking the request format. */
	if (!TS_RESP_check_sm2_request(ctx)) goto end;

	/* Checking acceptable policies. */
	if (!(policy = TS_RESP_get_policy(ctx))) goto end;

	/* Creating the TS_TST_INFO object. */
	if (!(ctx->tst_info = TS_RESP_create_tst_info(ctx, policy)))
		goto end;

	/* Processing extensions. */
	if (!TS_RESP_process_extensions(ctx)) goto end;

	
	/* Generating the signature. */
	if (!TS_RESP_sm2_sign(ctx, engine)) goto end;

	/* Everything was successful. */
	result = 1;
 end:
	if (!result)
		{
		TSerr(TS_F_TS_RESP_CREATE_RESPONSE, TS_R_RESPONSE_SETUP_ERROR);
		if (ctx->response != NULL)
			{
			if (TS_RESP_CTX_set_status_info_cond(ctx,
				TS_STATUS_REJECTION, "Error during response "
				"generation.") == 0)
				{
				TS_RESP_free(ctx->response);
				ctx->response = NULL;
				}
			}
		}
	response = ctx->response;
	ctx->response = NULL;	/* Ownership will be returned to caller. */
	TS_RESP_CTX_cleanup(ctx);
	return response;
}
	
static void TS_RESP_CTX_init(TS_RESP_CTX *ctx)
{
	ctx->request = NULL;
	ctx->response = NULL;
	ctx->tst_info = NULL;
}	

static int TS_RESP_check_sm2_request(TS_RESP_CTX *ctx)
{
	TS_REQ *request = ctx->request;
	TS_MSG_IMPRINT *msg_imprint;
	X509_ALGOR *md_alg;
	int md_alg_id;
	const ASN1_OCTET_STRING *digest;
	EVP_MD *md = NULL;
	int i;

	/* Checking request version. */
	if (TS_REQ_get_version(request) != 1)
		{
			
		TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
					    "Bad request version.");
		TS_RESP_CTX_add_failure_info(ctx, TS_INFO_BAD_REQUEST);
		return 0;
		}

	/* Checking message digest algorithm. */
	msg_imprint = TS_REQ_get_msg_imprint(request);
	md_alg = TS_MSG_IMPRINT_get_algo(msg_imprint);
	md_alg_id = OBJ_obj2nid(md_alg->algorithm);
	
	char obj_txt[256];
	int digest_len = OBJ_obj2txt(obj_txt, sizeof(obj_txt), md_alg->algorithm, 0);
	
	if(strcasecmp(obj_txt, OID_SM3) == 0)
	{
		digest = TS_MSG_IMPRINT_get_msg(msg_imprint);
		if(digest->length != LEN_SM3_DIGEST)
		{
			TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
					    "Bad message digest.");
			TS_RESP_CTX_add_failure_info(ctx, TS_INFO_BAD_DATA_FORMAT);
			return 0;
		}
		return 1;		
	}
	
	for (i = 0; !md && i < sk_EVP_MD_num(ctx->mds); ++i)
		{
		EVP_MD *current_md = sk_EVP_MD_value(ctx->mds, i);
		if (md_alg_id == EVP_MD_type(current_md))
			md = current_md;
		}
	if (!md)
		{
		TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
					    "Message digest algorithm is "
					    "not supported.");
		TS_RESP_CTX_add_failure_info(ctx, TS_INFO_BAD_ALG);
		return 0;
		}

	/* No message digest takes parameter. */
	if (md_alg->parameter 
	    && ASN1_TYPE_get(md_alg->parameter) != V_ASN1_NULL)
		{	
		TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
					    "Superfluous message digest "
					    "parameter.");
		TS_RESP_CTX_add_failure_info(ctx, TS_INFO_BAD_ALG);
		return 0;
		}
	/* Checking message digest size. */
	digest = TS_MSG_IMPRINT_get_msg(msg_imprint);
	if (digest->length != EVP_MD_size(md))
		{	
		TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
					    "Bad message digest.");
		TS_RESP_CTX_add_failure_info(ctx, TS_INFO_BAD_DATA_FORMAT);
		return 0;
		}

	return 1;
}

static ASN1_OBJECT *TS_RESP_get_policy(TS_RESP_CTX *ctx)
{
	ASN1_OBJECT *requested = TS_REQ_get_policy_id(ctx->request);
	ASN1_OBJECT *policy = NULL;
	int i;

	if (ctx->default_policy == NULL)
		{
		TSerr(TS_F_TS_RESP_GET_POLICY, TS_R_INVALID_NULL_POINTER);
		return NULL;
		}
	/* Return the default policy if none is requested or the default is
	   requested. */
	if (!requested || !OBJ_cmp(requested, ctx->default_policy))
		policy = ctx->default_policy;

	/* Check if the policy is acceptable. */
	for (i = 0; !policy && i < sk_ASN1_OBJECT_num(ctx->policies); ++i)
		{
		ASN1_OBJECT *current = sk_ASN1_OBJECT_value(ctx->policies, i);
		if (!OBJ_cmp(requested, current))
			policy = current;
		}
	if (!policy)
		{
		TSerr(TS_F_TS_RESP_GET_POLICY, TS_R_UNACCEPTABLE_POLICY);
		TS_RESP_CTX_set_status_info(ctx, TS_STATUS_REJECTION,
					    "Requested policy is not "
					    "supported.");
		TS_RESP_CTX_add_failure_info(ctx, TS_INFO_UNACCEPTED_POLICY);
		}
	return policy;
}

/* Creates the TS_TST_INFO object based on the settings of the context. */
static TS_TST_INFO *TS_RESP_create_tst_info(TS_RESP_CTX *ctx,
					    ASN1_OBJECT *policy)
{
	int result = 0;
	TS_TST_INFO *tst_info = NULL;
	ASN1_INTEGER *serial = NULL;
	ASN1_GENERALIZEDTIME *asn1_time = NULL;
	long sec, usec;
	TS_ACCURACY *accuracy = NULL;
	const ASN1_INTEGER *nonce;
	GENERAL_NAME *tsa_name = NULL;

	if (!(tst_info = TS_TST_INFO_new())) goto end;
	if (!TS_TST_INFO_set_version(tst_info, 1)) goto end;
	if (!TS_TST_INFO_set_policy_id(tst_info, policy)) goto end;
	if (!TS_TST_INFO_set_msg_imprint(tst_info, ctx->request->msg_imprint))
		goto end;
	if (!(serial = (*ctx->serial_cb)(ctx, ctx->serial_cb_data))
	    || !TS_TST_INFO_set_serial(tst_info, serial))
		goto end;
	if (!(*ctx->time_cb)(ctx, ctx->time_cb_data, &sec, &usec)
            || !(asn1_time = TS_RESP_set_genTime_with_precision(NULL, 
					sec, usec, 
					ctx->clock_precision_digits))
	    || !TS_TST_INFO_set_time(tst_info, asn1_time))
		goto end;

	/* Setting accuracy if needed. */
	if ((ctx->seconds || ctx->millis || ctx->micros) 
	    && !(accuracy = TS_ACCURACY_new()))
		goto end;

	if (ctx->seconds && !TS_ACCURACY_set_seconds(accuracy, ctx->seconds))
		goto end;
	if (ctx->millis && !TS_ACCURACY_set_millis(accuracy, ctx->millis))
		goto end;
	if (ctx->micros && !TS_ACCURACY_set_micros(accuracy, ctx->micros))
		goto end;
	if (accuracy && !TS_TST_INFO_set_accuracy(tst_info, accuracy)) 
		goto end;

	/* Setting ordering. */
	if ((ctx->flags & TS_ORDERING) 
	    && !TS_TST_INFO_set_ordering(tst_info, 1))
		goto end;
	
	/* Setting nonce if needed. */
	if ((nonce = TS_REQ_get_nonce(ctx->request)) != NULL
	    && !TS_TST_INFO_set_nonce(tst_info, nonce))
		goto end;

	/* Setting TSA name to subject of signer certificate. */
	if (ctx->flags & TS_TSA_NAME)
		{
		if (!(tsa_name = GENERAL_NAME_new())) goto end;
		tsa_name->type = GEN_DIRNAME;
		tsa_name->d.dirn = 
			X509_NAME_dup(ctx->signer_cert->cert_info->subject);
		if (!tsa_name->d.dirn) goto end;
		if (!TS_TST_INFO_set_tsa(tst_info, tsa_name)) goto end;
		}

	result = 1;
 end:
	if (!result)
		{
		TS_TST_INFO_free(tst_info);
		tst_info = NULL;
		TSerr(TS_F_TS_RESP_CREATE_TST_INFO, TS_R_TST_INFO_SETUP_ERROR);
		TS_RESP_CTX_set_status_info_cond(ctx, TS_STATUS_REJECTION,
						 "Error during TSTInfo "
						 "generation.");
		}
	GENERAL_NAME_free(tsa_name);
	TS_ACCURACY_free(accuracy);
	ASN1_GENERALIZEDTIME_free(asn1_time);
	ASN1_INTEGER_free(serial);
	
	return tst_info;
}

/* Processing the extensions of the request. */
static int TS_RESP_process_extensions(TS_RESP_CTX *ctx)
{
	STACK_OF(X509_EXTENSION) *exts = TS_REQ_get_exts(ctx->request);
	int i;
	int ok = 1;

	for (i = 0; ok && i < sk_X509_EXTENSION_num(exts); ++i)
		{
		X509_EXTENSION *ext = sk_X509_EXTENSION_value(exts, i);
		/* XXXXX The last argument was previously
		   (void *)ctx->extension_cb, but ISO C doesn't permit
		   converting a function pointer to void *.  For lack of
		   better information, I'm placing a NULL there instead.
		   The callback can pick its own address out from the ctx
		   anyway...
		*/
		ok = (*ctx->extension_cb)(ctx, ext, NULL);
		}

	return ok;
}

/* Functions for signing the TS_TST_INFO structure of the context. */
static int TS_RESP_sm2_sign(TS_RESP_CTX *ctx, CTSEngine *engine)
{
	unsigned char *pTmp = NULL;
	unsigned char tst_data[10240] = {0};
	pTmp = tst_data;
	int tst_len = i2d_TS_TST_INFO(ctx->tst_info, &pTmp);
	if(tst_len <= 0)
	{
		return SAR_ObjErr;
	}		

	unsigned char byCert[10240];
	memset(byCert, 0, sizeof(byCert));
	long lCertLen = CMailCoder::base64_decode(engine->m_Sm2Cert, strlen(engine->m_Sm2Cert), (char *)byCert);
	if (lCertLen <= 0)
	{	
		return SAR_BASE64ENCODEDECODEErr;
	}
	
	if (!MYTOOL_IsECCCert(byCert, lCertLen))
	{
		return SAR_VERIFYCERTErr;
	}
	char pszOut[10240] = {0};
	long ret = MYSIGN_SignDataByP7_SM2(
		tst_data,
		tst_len,
		engine->m_Sm2Cert,
		engine->m_Sm2Pkey,
		NULL,
		pszOut,
		true);
	if(ret != 0)
		return ret;
	
	char byData[10240]= { 0 };
	int nLen = CMailCoder::base64_decode( pszOut, strlen(pszOut), (char *)byData);
	if (nLen <= 0) 
	{
		return SAR_BASE64ENCODEDECODEErr;
	}
	pTmp = (unsigned char*)byData;
	PKCS7 *p7 = d2i_PKCS7(NULL, (const unsigned char**)&pTmp, nLen);
	if(p7 == NULL)
	{
		return STF_TS_MALFORMAT;
	}
	PKCS7_free(ctx->response->token);
	ctx->response->token = p7;
	TS_TST_INFO_free(ctx->response->tst_info);
	ctx->response->tst_info = ctx->tst_info;
	p7 = NULL;
	ctx->tst_info = NULL;
	
	return 0;
}

static void TS_RESP_CTX_cleanup(TS_RESP_CTX *ctx)
{
	TS_REQ_free(ctx->request);
	ctx->request = NULL;
	TS_RESP_free(ctx->response);
	ctx->response = NULL;
	TS_TST_INFO_free(ctx->tst_info);
	ctx->tst_info = NULL;
}

static ASN1_GENERALIZEDTIME *
TS_RESP_set_genTime_with_precision(ASN1_GENERALIZEDTIME *asn1_time, 
				   long sec, long usec, unsigned precision)
{
	time_t time_sec = (time_t) sec;
	struct tm *tm = NULL;	
	char genTime_str[17 + TS_MAX_CLOCK_PRECISION_DIGITS];
	char *p = genTime_str;
	char *p_end = genTime_str + sizeof(genTime_str);

	if (precision > TS_MAX_CLOCK_PRECISION_DIGITS)
		goto err;

	
	if (!(tm = gmtime(&time_sec)))
		goto err;

	/* 
	 * Put "genTime_str" in GeneralizedTime format.  We work around the 
	 * restrictions imposed by rfc3280 (i.e. "GeneralizedTime values MUST 
	 * NOT include fractional seconds") and OpenSSL related functions to 
	 * meet the rfc3161 requirement: "GeneralizedTime syntax can include 
	 * fraction-of-second details". 
	 */                   
	p += BIO_snprintf(p, p_end - p,
			  "%04d%02d%02d%02d%02d%02d",
			  tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, 
			  tm->tm_hour, tm->tm_min, tm->tm_sec);
	if (precision > 0)
	{
		/* Add fraction of seconds (leave space for dot and null). */
		BIO_snprintf(p, 2 + precision, ".%ld", usec);
		/* We cannot use the snprintf return value, 
		   because it might have been truncated. */
		p += strlen(p);

		/* To make things a bit harder, X.690 | ISO/IEC 8825-1 provides
		   the following restrictions for a DER-encoding, which OpenSSL
		   (specifically ASN1_GENERALIZEDTIME_check() function) doesn't 
		   support:
		   "The encoding MUST terminate with a "Z" (which means "Zulu" 
		   time). The decimal point element, if present, MUST be the 
		   point option ".". The fractional-seconds elements, 
		   if present, MUST omit all trailing 0's; 
		   if the elements correspond to 0, they MUST be wholly
		   omitted, and the decimal point element also MUST be
		   omitted." */
		/* Remove trailing zeros. The dot guarantees the exit
		   condition of this loop even if all the digits are zero. */
		while (*--p == '0')
			/* empty */;
		/* p points to either the dot or the last non-zero digit. */
		if (*p != '.') ++p;
		}
	/* Add the trailing Z and the terminating null. */
	*p++ = 'Z';
	*p++ = '\0';

	/* Now call OpenSSL to check and set our genTime value */
	if (!asn1_time && !(asn1_time = M_ASN1_GENERALIZEDTIME_new()))
		goto err;
	if (!ASN1_GENERALIZEDTIME_set_string(asn1_time, genTime_str))
		{
		ASN1_GENERALIZEDTIME_free(asn1_time);
		goto err;
		}

	return asn1_time;
 err:
	TSerr(TS_F_TS_RESP_SET_GENTIME_WITH_PRECISION, TS_R_COULD_NOT_SET_TIME);
	return NULL;
}



