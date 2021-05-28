// =================================================================================
//  File    : TSAEngine.cpp
//  Purpose : 时间戳服务器签名引擎类
//  Author  : 林灶钢
//  Created : 2006-11-21
//  CopyRight (C) Fujian Kinsec
// =================================================================================

// =================================================================================
//  Include Header Files
#if defined(_LINUX)
#include <uuid/uuid.h>
#endif
#include "TSAEngine.h"
#include "MailCoder.h"
#include "TSErr.h"


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
#include <sys/types.h>
#include <sys/timeb.h>

// =================================================================================
//  Macro Definition

// 生成nonce（请求随机数）
#define	NONCE_LENGTH		64
static ASN1_INTEGER *create_nonce(int bits)
{
	unsigned char buf[20];
	ASN1_INTEGER *nonce = NULL;
	int len = (bits - 1) / 8 + 1;
	int i;

	/* Generating random byte sequence. */
	if (len > (int)sizeof(buf)) goto err;
	if (RAND_bytes(buf, len) <= 0) goto err;

	/* Find the first non-zero byte and creating ASN1_INTEGER object. */
	for (i = 0; i < len && !buf[i]; ++i);
	if (!(nonce = ASN1_INTEGER_new())) goto err;
	OPENSSL_free(nonce->data);
	/* Allocate at least one byte. */
	nonce->length = len - i;
	if (!(nonce->data = (unsigned char *)OPENSSL_malloc(nonce->length + 1))) goto err;
	memcpy(nonce->data, buf + i, nonce->length);

	return nonce;
err:
//	BIO_printf(bio_err, "could not create nonce\n");
	ASN1_INTEGER_free(nonce);
	return NULL;
}

// 对数据做sha1摘要
bool HashData(
    unsigned char *in,
    int inlen,
    unsigned char *out,
    int *outlen)
{
	SHA_CTX context;
	SHA1_Init(&context);
	SHA1_Update(&context, (const unsigned char *)in, inlen);
	SHA1_Final(out, &context);
	*outlen = 20;
	return true;
}

static int get_serial(const char *serialfile, std::string & serialNumber)
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

static ASN1_INTEGER *next_serial(const char *serialfile)
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

static ASN1_INTEGER* my_serial_cb(TS_RESP_CTX *ctx, void *data)
{
	unsigned char bySerial[64];
	memset(bySerial, 0, sizeof(bySerial));
	memcpy(bySerial, "\x02\x10\x12\x34\x56\x78\x90\x12\x34\x56\x78\x90\x12\x34\x56\x78\x90\x21 ", 18);
	int nSerialLen = 18;

	unsigned char *pbTmp = bySerial;
	ASN1_INTEGER *serial = d2i_ASN1_INTEGER(NULL, (const unsigned char **)&pbTmp, nSerialLen);
	if (!serial)
		return NULL;

	return serial;
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
//  Class Definition
// = Default constructor.
CTSAEngine::CTSAEngine()
{
	this->m_pPriKey = NULL;
	this->m_SerCert = NULL;
	this->m_SerPriKey = NULL;
	this->m_SerCertCa = NULL;
	this->m_SerCertRoot = NULL;

}

// = Default destructor.
CTSAEngine::~CTSAEngine()
{
	if( this->m_pPriKey )
		delete this->m_pPriKey;
	if( m_SerCert )
		X509_free( m_SerCert );
	if( m_SerCertRoot )
		X509_free( m_SerCertRoot );
	if( m_SerCertCa )
		X509_free( m_SerCertCa );
	if( m_SerPriKey )
		EVP_PKEY_free( m_SerPriKey );
}

// = Load certificate.
int CTSAEngine::loadCertificate( const KTCertificate & cert, const KTByteArray & rPriKey )
{
	this->m_cert.cloneContent( cert );
	this->m_pPriKey = new CRSAPrivateKey( rPriKey );
	return 0;
}

// = Get Tsa certificate.   获取证书
int CTSAEngine::getCertData( std::string & certData )
{
	KTByteArray rData( this->m_cert.totalCount() );
	this->m_cert.output( &rData[0] );
	this->base64Encode( &rData[0], rData.size(), certData );
	return 0;
}

// = Create time stamp.        制作时间戳
int CTSAEngine::createTimeStamp( const char * szReqData, std::string & serialNumber,
                                 std::string & tsData, std::string & respData )
{
	if( szReqData && strlen( szReqData ) > 0 )
	{
		KTByteArray rReqData;
		KTTimeStampReq tsReq;
		this->base64Decode( szReqData, strlen( szReqData ), rReqData );
		tsReq.create( &rReqData[0], rReqData.size() );

		KTTimeStampResp tsResp;
		tsResp.m_timeStampToken.setTypeValue( ObjID_signedData );
		KTSignedData * pData = (KTSignedData *)tsResp.m_timeStampToken.valueObj();
		pData->m_contentInfo.setTypeValue( ObjID_id_ct_TSTInfo );

		KTTSTInfo * pTSTInfo = (KTTSTInfo *)pData->m_contentInfo.valueObj();
		KTSignerInfos * pSignerInfos = &pData->m_signerInfos;
		KTCertificatesChoice * pSignCertsChoice = &pData->m_certificates;
		KTDigestAlgorithmIdentifiers * pAlgorithmIdentifiers = &pData->m_digestAlgorithms;

		if( pAlgorithmIdentifiers )
			pAlgorithmIdentifiers->attachDigestAlgo( ObjID_id_sha1 );
		if( pSignCertsChoice )
		{
			pSignCertsChoice->m_certSet.increaseNode( 0 )->m_certificate.cloneContent(
			    this->m_cert );
			pSignCertsChoice->setExist();
		}

		if( pTSTInfo )
		{
			this->setTSTInfo( tsReq, pTSTInfo );
			pTSTInfo->m_serialNumber.getValue( serialNumber );
		}

		KTByteArray rDigest;
		tsReq.m_messageImprint.m_hashedMessage.getValue( rDigest );
		this->setSignerInfo( rDigest, pSignerInfos );

		tsResp.m_timeStampToken.modifyLen();
		KTByteArray rBuff( tsResp.m_timeStampToken.totalCount() );
		tsResp.m_timeStampToken.output( &rBuff[0] );
		this->base64Encode( &rBuff[0], rBuff.size(), tsData );

		tsResp.m_status.m_status = PKIStatusGranted;
		tsResp.m_timeStampToken.setExist();
		tsResp.modifyLen();
		rBuff.resize( tsResp.totalCount() );
		tsResp.output( &rBuff[0] );
		this->base64Encode( &rBuff[0], rBuff.size(), respData );//将制作完成的时间戳数据加密
		return 0;
	}
	return -1;
}

// = Base64 encode.
void CTSAEngine::base64Encode( const unsigned char * pData, unsigned int unDataLen,
                               std::string & data )
{
	unsigned int unBuffLen = ( unDataLen + 2 ) / 3 * 4 + 10;
	data.resize( unBuffLen + 1 );
	K_B64_Code( EM_CODE, pData, unDataLen, (unsigned char *)&data[0], &unBuffLen );
	data.resize( unBuffLen + 1 );
	data[unBuffLen] = '\0';
}

// = Base64 decode.
void CTSAEngine::base64Decode( const char * szData, unsigned int unDataLen,
                               KTByteArray & rData )
{
	unsigned int unBuffLen = unDataLen;
	rData.resize( unBuffLen );
	K_B64_Code( EM_DECODE, (const unsigned char *)szData, unDataLen, &rData[0], &unBuffLen );
	rData.resize( unBuffLen );
}

// = Set tst info.
void CTSAEngine::setTSTInfo( const KTTimeStampReq & tsReq, KTTSTInfo * pTSTInfo )
{
	pTSTInfo->m_version = tsReq.m_version.value();
	if( tsReq.m_reqPolicy.exist() == TRUE )
		pTSTInfo->m_policy.cloneContent( tsReq.m_reqPolicy );
	else
		pTSTInfo->m_policy.setValue( "1.2.3.4" );
	pTSTInfo->m_messageImprint.cloneContent( tsReq.m_messageImprint );

	time_t curTime = time( NULL );
#if defined(_WIN32)
	pTSTInfo->m_serialNumber = curTime;
#elif defined(_LINUX)
	uuid_t uid;
	uuid_generate( uid );
	CBigInteger big;
	for( int i = 0; i < 16; i++ )
	{
		BYTE b = uid[i];
		if( b & 0x01 )
			big.setBit( 8 * i );
		if( b & 0x02 )
			big.setBit( 8 * i + 1 );
		if( b & 0x04 )
			big.setBit( 8 * i + 2 );
		if( b & 0x08 )
			big.setBit( 8 * i + 3 );
		if( b & 0x10 )
			big.setBit( 8 * i + 4 );
		if( b & 0x20 )
			big.setBit( 8 * i + 5 );
		if( b & 0x40 )
			big.setBit( 8 * i + 6 );
		if( b & 0x80 )
			big.setBit( 8 * i + 7 );
	}
	pTSTInfo->m_serialNumber = big;
#endif
	pTSTInfo->m_genTime.setValue( *localtime( &curTime ) );

	pTSTInfo->m_ordering = FALSE;
	if( tsReq.m_nonce.exist() == TRUE )
	{
		pTSTInfo->m_nonce.cloneContent( tsReq.m_nonce );
		pTSTInfo->m_nonce.setExist();
	}
	pTSTInfo->modifyLen();
}

// = Set signer info.设置签名者信息
void CTSAEngine::setSignerInfo( const KTByteArray & rData, KTSignerInfos * pSignerInfos )
{
	unsigned int unSignedDataLen = 128;
	KTByteArray rSignedData( unSignedDataLen );
	K_RSA_Cipher( EM_SIGNATURE, this->m_pPriKey, &rData[0], rData.size(),
	              &rSignedData[0], &unSignedDataLen );
	rSignedData.resize( unSignedDataLen );

	KTSignerInfo * pSigner = pSignerInfos->m_siSet.increaseNode();
	pSigner->m_issuerAndSerialNumber.m_issuer.cloneContent( this->m_cert.m_tbsCertificate.m_issuer );
	pSigner->m_issuerAndSerialNumber.m_serialNumber.cloneContent( this->m_cert.m_tbsCertificate.m_serialNumber );
	pSigner->m_digestAlgorithm.setTypeValue( ObjID_id_sha1 );
	pSigner->m_digestEncryptionAlgorithm.setTypeValue( ObjID_rsaEncryption );
	pSigner->m_encryptedDigest.setValue( &rSignedData[0], rSignedData.size() );

	time_t timeSigned = time( NULL );
	KTAttribute * pAttribute = pSigner->m_authenticatedAttributes.m_aaSet.increaseNode();
	pAttribute->setTypeValue( ObjID_pkcs_9_at_signingTime );
	KTTime * pTime = ( (KTTimeSet *)pAttribute->valueObj() )->increaseNode();
	pTime->setValue( *gmtime( &timeSigned ) );
	pSigner->m_authenticatedAttributes.setExist();
}

void CTSAEngine::InitSerCer(X509* SerCert, EVP_PKEY* SerPriKey, X509* SerCertCa, X509* SerCertRoot, time_t CertBeginValid, time_t CertEndValid)
{
	m_SerCert = SerCert;
	m_SerPriKey = SerPriKey;
	m_SerCertCa = SerCertCa;
	m_SerCertRoot = SerCertRoot;
	m_CertBeginValid = CertBeginValid;
	m_CertEndValid = CertEndValid;
}

int CTSAEngine::CreateTSReq(	unsigned char *data, int datalen,	unsigned char *tsreq,	int *tsreqlen	)
{


	bool bRet = false;
	int nRet = 0, RetVal = 0;
	TS_MSG_IMPRINT *msg_imprint = NULL;
	X509_ALGOR *x509_algor = NULL;
	ASN1_INTEGER *nonce_asn1 = NULL;

	TS_REQ *ts_req = NULL;

	// digest ********************************************************
	unsigned char byDigest[256];
	memset(byDigest, 0, sizeof(byDigest));
	int nDigestLen = 256;
	bRet = HashData(data, datalen, byDigest, &nDigestLen);
	if (!bRet)
	{
		RetVal = TS_HashErr;
		goto end;
	}

	// ts req ********************************************************
	ts_req = TS_REQ_new();
	if (!ts_req)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	// version
	long version;
	version = 1;
	nRet = TS_REQ_set_version(ts_req, version);
	if (!nRet)
	{
		RetVal = TS_SetVerErr;
		goto end;
	}

	// messageImprint
	x509_algor = X509_ALGOR_new();
	if (!ts_req)
	{
		RetVal = TS_MemErr;
		goto end;
	}
	x509_algor->algorithm = OBJ_nid2obj(NID_sha1);
	if (!(x509_algor->algorithm))
	{
		RetVal = TS_ANSIErr;
		goto end;
	}
	x509_algor->parameter = ASN1_TYPE_new();
	if (!(x509_algor->parameter))
	{
		RetVal = TS_MemErr;
		goto end;
	}
	x509_algor->parameter->type = V_ASN1_NULL;

	msg_imprint = TS_MSG_IMPRINT_new();
	if (!msg_imprint)
	{
		RetVal = TS_MemErr;
		goto end;
	}
	nRet = TS_MSG_IMPRINT_set_algo(msg_imprint, x509_algor);
	if (!nRet)
	{
		RetVal = TS_MSGAlgoErr;
		goto end;
	}
	nRet = TS_MSG_IMPRINT_set_msg(msg_imprint, byDigest, nDigestLen);
	if (!nRet)
	{
		RetVal = TS_MSGErr;
		goto end;
	}
	nRet = TS_REQ_set_msg_imprint(ts_req, msg_imprint);
	if (!nRet)
	{
		RetVal = TS_MSGImpErr;
		goto end;
	}

	// nonce
	nonce_asn1 = create_nonce(NONCE_LENGTH);
	if (!nonce_asn1)
	{
		RetVal = TS_GenRandErr;
		goto end;
	}

	nRet = TS_REQ_set_nonce(ts_req, nonce_asn1);
	if (!nRet)
	{
		RetVal = TS_SetRandErr;
		goto end;
	}

	// certReq
	int cert_req;
	cert_req = 1;
	nRet = TS_REQ_set_cert_req(ts_req, cert_req);
	if (!nRet)
	{
		RetVal = TS_NewReqErr;
		goto end;
	}

	// output
	unsigned char byOut[10240];
	memset(byOut, 0, sizeof(byOut));
	int nOutLen;
	unsigned char *pbTmp;
	pbTmp = byOut;
	nOutLen = i2d_TS_REQ(ts_req, &pbTmp);
	if (nOutLen <= 0)
	{
		RetVal = TS_ReqErr;
		goto end;
	}
	memcpy(tsreq, byOut, nOutLen);
	*tsreqlen = nOutLen;

end:
	if (msg_imprint)
		TS_MSG_IMPRINT_free(msg_imprint);
	if (x509_algor)
		X509_ALGOR_free(x509_algor);
	if (nonce_asn1)
		ASN1_INTEGER_free(nonce_asn1);

	if (ts_req)
		TS_REQ_free(ts_req);

	return RetVal;
}


int CTSAEngine::CreateTSResp( unsigned char* tsreq, int tsreqlen,	X509* rootcacert, X509* cacert,
                              X509* cert, EVP_PKEY *pkey,	unsigned char *tsresp, int *tsresplen,	unsigned char *tstoken,
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
	if (rootcacert != NULL || cacert != NULL)
	{
		x509_cacerts = sk_X509_new_null();
		if (!x509_cacerts)
		{
			RetVal = TS_MemErr;
			goto end;
		}

		if (rootcacert != NULL)
		{
			nRet = sk_X509_push(x509_cacerts, rootcacert);
			if (!nRet)
			{
				RetVal = TS_RootCACertErr;
				goto end;
			}
		}

		if (cacert != NULL)
		{
			nRet = sk_X509_push(x509_cacerts, cacert);
			if (!nRet)
			{
				RetVal = TS_CACertErr;
				goto end;
			}
		}
	}

	// ts response ********************************************************
	// req
	req_bio = BIO_new(BIO_s_mem());
	if (!req_bio)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	BIO_set_close(req_bio, BIO_CLOSE); // BIO_free() free BUF_MEM
	if (BIO_write(req_bio, tsreq, tsreqlen) != tsreqlen)
	{
		RetVal = TS_ReqErr;
		goto end;
	}

	// Setting up response generation context.
	ts_resp_ctx = TS_RESP_CTX_new();
	if (!ts_resp_ctx)
	{
		RetVal = TS_MemErr;
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
			RetVal = TS_CACertErr;
			goto end;
		}
	}

	// Setting TSA signer certificate.
	nRet = TS_RESP_CTX_set_signer_cert(ts_resp_ctx, cert);
	if (!nRet)
	{
		RetVal = TS_CertErr;
		goto end;
	}

	// Setting TSA signer private key.
	nRet = TS_RESP_CTX_set_signer_key(ts_resp_ctx, pkey);
	if (!nRet)
	{
		RetVal = TS_KeyErr;
		goto end;
	}

	// Setting default policy OID.
	policy_obj1 = OBJ_txt2obj("1.2.3.4.1", 0);
	if (!policy_obj1)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	nRet = TS_RESP_CTX_set_def_policy(ts_resp_ctx, policy_obj1);
	if (!nRet)
	{
		RetVal = TS_PolicyErr;
		goto end;
	}

	// Setting acceptable policy OIDs.
	policy_obj2 = OBJ_txt2obj("1.2.3.4.5.6", 0);
	if (!policy_obj2)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	nRet = TS_RESP_CTX_add_policy(ts_resp_ctx, policy_obj2);
	if (!nRet)
	{
		RetVal = TS_PolicyErr;
		goto end;
	}

	policy_obj3 = OBJ_txt2obj("1.2.3.4.5.7", 0);
	if (!policy_obj3)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	nRet = TS_RESP_CTX_add_policy(ts_resp_ctx, policy_obj3);
	if (!nRet)
	{
		RetVal = TS_PolicyErr;
		goto end;
	}

	// Setting the acceptable one-way hash algorithms.
	nRet = TS_RESP_CTX_add_md(ts_resp_ctx, EVP_md5());
	if (!nRet)
	{
		RetVal = TS_RespHashErr;
		goto end;
	}

	nRet = TS_RESP_CTX_add_md(ts_resp_ctx, EVP_sha1());
	if (!nRet)
	{
		RetVal = TS_RespHashErr;
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
		RetVal = TS_AccurErr;
		goto end;
	}

	// Setting the precision of the time.
	unsigned clock_precision_digits;
	clock_precision_digits = 0;
	nRet = TS_RESP_CTX_set_clock_precision_digits(ts_resp_ctx, clock_precision_digits);
	if (!nRet)
	{
		RetVal = TS_PreciErr;
		goto end;
	}
	// Setting the ordering flaf if requested.
	TS_RESP_CTX_add_flags(ts_resp_ctx, TS_ORDERING);

	// Setting the TSA name required flag if requested.
	TS_RESP_CTX_add_flags(ts_resp_ctx, TS_TSA_NAME);

	// Creating the response.
	ts_resp = TS_RESP_create_response(ts_resp_ctx, req_bio);
	if (!ts_resp)
		if (!nRet)
		{
			RetVal = TS_NewRespErr;
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
		RetVal = TS_RespErr;
		goto end;
	}

	memcpy(tsresp, byOut, nOutLen);
	*tsresplen = nOutLen;

	// output ts token
	PKCS7 *ts_token;
	ts_token = TS_RESP_get_token(ts_resp);
	if (!ts_token)
	{
		RetVal = TS_RespErr;
		goto end;
	}

	memset(byOut, 0, sizeof(byOut));
	pbTmp = byOut;
	nOutLen = i2d_PKCS7(ts_token, &pbTmp);
	if (nOutLen <= 0)
	{
		RetVal = TS_RespErr;
		goto end;
	}

	get_serial("tsa_serial_file",serialNumber);
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

int CTSAEngine::VerifyTSToken( unsigned char *data, int datalen,  X509* rootcacert, X509* cacert,
                               unsigned char *tstoken, int tstokenlen)
{
	bool bRet = false;
	int nRet = 0, RetVal = 0;

	PKCS7 *ts_token = NULL;
	TS_VERIFY_CTX *ts_verify_ctx = NULL;

	// verify ts token ********************************************************
	// ts token
	unsigned char *pbTmp;
	pbTmp = tstoken;
	ts_token = d2i_PKCS7(NULL, (const unsigned char **)&pbTmp, tstokenlen);
	if (!ts_token)
	{
		RetVal = TS_RespErr;
		goto end;
	}

	// verify ctx
	ts_verify_ctx = TS_VERIFY_CTX_new();
	if (!ts_verify_ctx)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	ts_verify_ctx->flags = TS_VFY_VERSION | TS_VFY_DATA | TS_VFY_SIGNATURE | TS_VFY_SIGNER;

	// data
	ts_verify_ctx->data = BIO_new(BIO_s_mem());
	if (!ts_verify_ctx->data)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	//	BIO_set_close(ts_verify_ctx->data, BIO_CLOSE); // BIO_free() free BUF_MEM
	if (BIO_write(ts_verify_ctx->data, data, datalen) != datalen)
	{
		RetVal = TS_RespErr;
		goto end;
	}

	// x509 store
	ts_verify_ctx->store = X509_STORE_new();
	if (!ts_verify_ctx->store)
	{
		RetVal = TS_MemErr;
		goto end;
	}

	if( rootcacert )
	{
		nRet = X509_STORE_add_cert(ts_verify_ctx->store, rootcacert);
		if (!nRet)
		{
			RetVal = TS_RootCACertErr;
			goto end;
		}
	}

	if ( cacert )
	{
		nRet = X509_STORE_add_cert(ts_verify_ctx->store, cacert);
		if (!nRet)
		{
			RetVal = TS_CACertErr;
			goto end;
		}
	}

	// verify
	nRet = TS_RESP_verify_token(ts_verify_ctx, ts_token);
	if (!nRet)
	{
		RetVal = TS_VerifyErr;
		goto end;
	}

end:
	if (ts_token)
		PKCS7_free(ts_token);
	if (ts_verify_ctx)
		TS_VERIFY_CTX_free(ts_verify_ctx);

	return RetVal;

}

int CTSAEngine::ParseTSToken(unsigned char *tstoken, int tstokenlen, char *pszTime,int flag)
{
	bool bRet = false;
	int nRet = 0, RetVal = 0;

	PKCS7 *ts_token = NULL;
	TS_TST_INFO *ts_tst_info = NULL;

	// parse ts token ********************************************************
	// ts token
	unsigned char *pbTmp;
	pbTmp = tstoken;
	ts_token = d2i_PKCS7(NULL, (const unsigned char **)&pbTmp, tstokenlen);
	if (!ts_token)
	{
		RetVal = TS_RespErr;
		goto end;
	}

	if( 0 == flag)
	{
		// tst info
		int nTSTInfoLen;
		nTSTInfoLen = ts_token->d.sign->contents->d.other->value.octet_string->length;
		pbTmp =	ts_token->d.sign->contents->d.other->value.octet_string->data;
		ts_tst_info = d2i_TS_TST_INFO(NULL, (const unsigned char **)&pbTmp, nTSTInfoLen);
		if (!ts_tst_info)
		{
			RetVal = TS_TstErr;
			goto end;
		}

		// get time
		ASN1_GENERALIZEDTIME *generalized_time;
		generalized_time = (ASN1_GENERALIZEDTIME *)TS_TST_INFO_get_time(ts_tst_info);
		if (!generalized_time)
		{
			RetVal = TS_TstErr;
			goto end;
		}
		CertTime2String(generalized_time, pszTime);
	}
	else if(1 == flag)
	{
		STACK_OF(PKCS7_SIGNER_INFO) *sk = NULL;
		PKCS7_SIGNER_INFO * si = NULL;
		PKCS7_ISSUER_AND_SERIAL *ias = NULL;
		X509 *x509 = NULL;
		STACK_OF(X509) *cert = NULL;
		char CertB64[4096] = {0};
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
			int B64Len = CMailCoder::base64_encode((char *)pbTmp, len, CertB64);
			if (B64Len <= 0)
			{
				RetVal = TS_RespErr;
				goto end;
			}
			strcpy(pszTime, CertB64);
			free(pbTmp);
			pbTmp = NULL;
		}
	}
	else if(2 == flag)
	{
		STACK_OF(PKCS7_SIGNER_INFO) *sk = NULL;
		PKCS7_SIGNER_INFO * si = NULL;
		PKCS7_ISSUER_AND_SERIAL *ias = NULL;
		X509 *x509 = NULL;
		STACK_OF(X509) *cert = NULL;

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
			strcpy(pszTime, bySignData);

		}
	}

end:
	if (ts_token)
		PKCS7_free(ts_token);
	if (ts_tst_info)
		TS_TST_INFO_free(ts_tst_info);

	return RetVal;

}

// =================================================================================
//  1.00    2006-11-21    Created By LinZaoGang
//  File End
