#include "STFTSAFunction.h"
#include "TSAServiceHTTPApp.h"

extern CTSAServiceHTTPApp g_app;

//soft alg
SGD_UINT32 TSATestSoft()
{
	int nRet = KSSIGN_Initialize();
	if (nRet != 0)
	{
		printf("KSSIGN_Initialize return 0x%08x\n", nRet);
		return 0;
	}
	/*
	unsigned char plain[10240], ts_req[10240], ts_res[10240];
	char ts_req_b64[10240], ts_res_b64[10240];
	memset(plain, 0, sizeof(plain));
	memset(ts_req, 0, sizeof(ts_req));
	memset(ts_res, 0, sizeof(ts_res));
	memset(ts_req_b64, 0, sizeof(ts_req_b64));
	memset(ts_res_b64, 0, sizeof(ts_res_b64));
	int plain_len=0, ts_req_len=0, ts_res_len=0;
	strcpy((char *)plain, "12345678");
	plain_len = strlen((char *)plain);
*/
	//ts request


	//verify
	char ts_res_b64[8192] = {0};
	unsigned char ts_res[8192] = {0};
	strcpy(ts_res_b64, "\
MIIFFQYKKoEcz1UGAQQCAqCCBQUwggUBAgEDMQ8wDQYJKoEcz1UBgxECBQAwggEIBgsqhkiG9w0B\
CRABBKCB+ASB9TCB8gIBAQYKKwYBBAGyMQIBATAvMAsGCSqBHM9VAYMRAgQgD///gelx+j8JEHq/\
d5MUY/wHEL+4li7+rj1WVLBzuwwCG0tUVFNBXzE2MjI0MzQ3NTFfMTIyMDEzMTk1ORgWMjAyMTA1\
MzEwNDE5MTEuODI3OTYzWgIEHzarP6B1pHMwcTELMAkGA1UEBgwCQ04xEjAQBgNVBAgMCeemj+W7\
uuecgTESMBAGA1UEBwwJ56aP5bee5biCMRUwEwYDVQQKDAzlh6/nibnkv6HlrokxEjAQBgNVBAsM\
CeaKgOacr+mDqDEPMA0GA1UEAwwGa2luc2VjoIICVzCCAlMwggH3oAMCAQICEGSyQfomfTfQa3Nt\
IFgJNnIwDAYIKoEcz1UBg3UFADBkMQswCQYDVQQGEwJDTjEPMA0GA1UECAwGRnVqaWFuMQ8wDQYD\
VQQHDAZGdXpob3UxDzANBgNVBAoMBktpbnNlYzEMMAoGA1UECwwDRGV2MRQwEgYDVQQDDAtLaW5z\
ZWNTTTJDQTAgFw0yMTAzMTgxNjAwMDBaGA8yMDYxMDMxOTE1NTk1OVowcTELMAkGA1UEBgwCQ04x\
EjAQBgNVBAgMCeemj+W7uuecgTESMBAGA1UEBwwJ56aP5bee5biCMRUwEwYDVQQKDAzlh6/nibnk\
v6HlrokxEjAQBgNVBAsMCeaKgOacr+mDqDEPMA0GA1UEAwwGa2luc2VjMFkwEwYHKoZIzj0CAQYI\
KoEcz1UBgi0DQgAE9STkRjDDLFm/13cP6vBnT9JP51kxQ3a2CoKBPOqRAbZvPSvZzHq6WzomQeEj\
vLUTrmRm/1lqVMqkGvAK+qk26qN6MHgwHwYDVR0jBBgwFoAUu0s8hIMwUd++Jo7SPAorEFGO8A0w\
HQYDVR0OBBYEFOZWd6S8+/FFXMudOs3Bni36wotyMAkGA1UdEwQCMAAwCwYDVR0PBAQDAgP4MB4G\
CCqBHNAUBAEBBBIzNTAxMDEyMDIxMDEwMTAwMDEwDAYIKoEcz1UBg3UFAANIADBFAiEA3T6qYssy\
IZy4xS7Dg68Cb/GZc/ZRUCChzDamQzsXJLkCICRz5ibi6Mbo2FQrMMk4w+l5hn4gGyp2CGc35t7K\
5dP6MYIBgjCCAX4CAQEweDBkMQswCQYDVQQGEwJDTjEPMA0GA1UECAwGRnVqaWFuMQ8wDQYDVQQH\
DAZGdXpob3UxDzANBgNVBAoMBktpbnNlYzEMMAoGA1UECwwDRGV2MRQwEgYDVQQDDAtLaW5zZWNT\
TTJDQQIQZLJB+iZ9N9Brc20gWAk2cjANBgkqgRzPVQGDEQIFAKCBmDAaBgkqhkiG9w0BCQMxDQYL\
KoZIhvcNAQkQAQQwHAYJKoZIhvcNAQkFMQ8XDTIxMDUzMTA0MTkxMVowKwYLKoZIhvcNAQkQAgwx\
HDAaMBgwFgQUAAAAAAAAAAAAAAAAAAAAAIvPwKYwLwYJKoZIhvcNAQkEMSIEIM2O+eojukHi1sDO\
u/ueGDPZ+cxWW4XiKfBXiXCeCnm5MA0GCSqBHM9VAYItAQUABEYwRAIgOHHyxecDeGioBvwC0/Re\
8VtrlKm8ME14WRFUqR6eu1YCIGb4N0WYKcgL9IBM1Izb9bN7XXsf+yF2m9K4fetgraBz");

	int ts_res_len = CMailCoder::base64_decode(ts_res_b64, strlen(ts_res_b64), (char *)ts_res);
	nRet = KSSIGN_VerifyTimeStamp(
		NULL,
		NULL,
		NULL,
		ts_res,
		ts_res_len,
		g_app.m_SM2Cert, 
		g_app.m_SM2CertLen);
	if (nRet != SAR_OK)
	{
		KSSIGN_Finalize();
		return nRet;
	}
	printf("KSSIGN_VerifyTimeStamp success\n");

	//STF_GetTSInfo
	//获取时间戳证书
	unsigned char cert[8192] = {0};
	int cert_length = 0;
	KSSIGN_GetTimeStampInfo(ts_res, ts_res_len, 2, cert, &cert_length);
	printf("cert_length=======%d\n", cert_length);
	//解析DN
	char dn[8192] = {0};
	long dnlen = 0;
	MYTOOL_GetCertDN(cert, cert_length, dn, &dnlen);
	printf("MYTOOL_GetCertDN...[DN]=======\n%s\n", dn);
	//
	char issuer_dn[8192] = {0};
	long issuer_dn_len = 0;
	MYTOOL_GetCertIssuerDN(cert, cert_length, issuer_dn, &issuer_dn_len);
	printf("MYTOOL_GetCertDN...[IssuerDN]=======\n%s\n", issuer_dn);

	//解析时间戳  kinsec:2021-05-31 12:19:11.827963 dean:20210525153710
	unsigned char time_stamp[8192] = {0};
	int time_stamp_length = 0;
	KSSIGN_GetTimeStampInfo(ts_res, ts_res_len, 8, time_stamp, &time_stamp_length);
	printf("time_stamp=======%s\n", time_stamp);
	

	KSSIGN_Finalize();

	return nRet;
}


//hard alg
SGD_UINT32 TSATestHard()
{
	void *hDeviceHandle, *hSessionHandle;
	long retcode = 0;
	retcode = SDF_OpenDevice(&hDeviceHandle);
	printf("SDF_OpenDevice ret = %d\n", retcode);
    retcode = SDF_OpenSession(hDeviceHandle, &hSessionHandle);
	printf("SDF_OpenSession ret = %d\n", retcode);

	unsigned char plain[10240], ts_req[10240], ts_res[10240];
	char ts_req_b64[10240], ts_res_b64[10240];
	memset(plain, 0, sizeof(plain));
	memset(ts_req, 0, sizeof(ts_req));
	memset(ts_res, 0, sizeof(ts_res));
	memset(ts_req_b64, 0, sizeof(ts_req_b64));
	memset(ts_res_b64, 0, sizeof(ts_res_b64));
	int plain_len=0, ts_req_len=0, ts_res_len=0;
	strcpy((char *)plain, "12345678");
	plain_len = strlen((char *)plain);

	//ts request
	retcode = KSSIGN_CreateTimeStampReq(
		hSessionHandle,
		FMFuncDigest,
		1,
		plain,
		plain_len,
		ts_req,
		&ts_req_len);
	if (retcode != SAR_OK)
	{
		SDF_CloseSession(hSessionHandle);
    	SDF_CloseDevice(hDeviceHandle);
		return retcode;
	}
	printf("KSSIGN_CreateTimeStampReq success\n");
	printf("KSSIGN_CreateTimeStampReq...ts_req_len: %d\n", ts_req_len);
	int ts_req_b64_len = CMailCoder::base64_encode((char *)ts_req, ts_req_len, ts_req_b64);
	printf("KSSIGN_CreateTimeStampReq...ts_req_b64: %s\n", ts_req_b64);
	printf("KSSIGN_CreateTimeStampReq...ts_req_b64_len: %d\n", ts_req_b64_len);

	//ts response
	retcode = KSSIGN_CreateTimeStamp(
		hSessionHandle,
		FMFuncPrivEnc,
		FMFuncDigest,
		1,
		false,
		ts_req,
		ts_req_len,
		g_app.m_FMSM2Cert, 
		g_app.m_FMSM2CertLen,
		NULL, 0,
		(char *)"1122334455667788",
		ts_res,
		&ts_res_len);
	if (retcode != SAR_OK)
	{
		SDF_CloseSession(hSessionHandle);
    	SDF_CloseDevice(hDeviceHandle);
		return retcode;
	}
	printf("KSSIGN_CreateTimeStamp success\n");
	printf("KSSIGN_CreateTimeStamp...ts_res_len: %d\n", ts_res_len);
	int ts_res_b64_len = CMailCoder::base64_encode((char *)ts_res, ts_res_len, ts_res_b64);
	printf("KSSIGN_CreateTimeStamp...ts_res_b64: %s\n", ts_res_b64);
	printf("KSSIGN_CreateTimeStamp...ts_res_b64_len: %d\n", ts_res_b64_len);

	//verify hard
	retcode = KSSIGN_VerifyTimeStamp(
		hSessionHandle,
		FMFuncCertVrfSign,
		FMFuncDigest,
		ts_res,
		ts_res_len,
		NULL, 0, 
		//g_app.m_FMSM2Cert, 
		//g_app.m_FMSM2CertLen,
		plain,
		plain_len,
		false);
	if (retcode != SAR_OK)
	{
		SDF_CloseSession(hSessionHandle);
    	SDF_CloseDevice(hDeviceHandle);
		return retcode;
	}
	printf("KSSIGN_VerifyTimeStamp hard success\n");

	//verify soft
	retcode = KSSIGN_Initialize();
	retcode = KSSIGN_VerifyTimeStamp(
		NULL,
		NULL,
		NULL,
		ts_res,
		ts_res_len,
		g_app.m_FMSM2Cert, 
		g_app.m_FMSM2CertLen,
		plain,
		plain_len,
		false);
	if (retcode != SAR_OK)
	{
		KSSIGN_Finalize();
		SDF_CloseSession(hSessionHandle);
    	SDF_CloseDevice(hDeviceHandle);
		return retcode;
	}
	printf("KSSIGN_VerifyTimeStamp soft success\n");

	retcode = SDF_CloseSession(hSessionHandle);
	printf("SDF_CloseSession ret = %d\n", retcode);
    retcode = SDF_CloseDevice(hDeviceHandle);
	printf("SDF_CloseDevice ret = %d\n", retcode);

	return retcode;
}

SGD_UINT32 TSAFunctionAssign(int function_define, string &errMsg, 
	map<string, string> mrequestdata, map<string, string> &mresponsedata)
{	
	SGD_UINT32 uiretCode = 0;
	char PKIerrmsg[256] = {0};
	void **phTSHandle = NULL;
	void *hTSHandle = NULL;
	void *hDeviceHandle, *hSessionHandle;
	//g_app.log.KTConsolePrint(LOG_LEVEL_DEBUG, "TSAFunctionAssign start\n");
	//Init
	if (g_app.m_nCryptoAlg == 1) {
		uiretCode = SDF_OpenDevice(&hDeviceHandle);
		//printf("SDF_OpenDevice uiretCode = %08x\n", uiretCode);
		uiretCode = SDF_OpenSession(hDeviceHandle, &hSessionHandle);
		//printf("SDF_OpenSession uiretCode = %08x\n", uiretCode);
		hTSHandle = hSessionHandle;
	}else{
		uiretCode = KSSIGN_Initialize();
	}
	if( uiretCode != 0 ){
		errMsg = "Init error!";
		return uiretCode;
	}

	//Function
	if (function_define == TSA_TEST){
		// to do
	}
	else if (function_define == TSA_INIT_ENVIRONMENT){
		//uiretCode = STF_InitEnvironment(phTSHandle);
	}else if (function_define == TSA_CLEAR_ENVIRONMENT){
		//uiretCode = STF_ClearEnvironment(hTSHandle);
	}else if (function_define == TSA_CREATE_TS_REQUEST){
		//printf("TSA_CREATE_TS_REQUEST start\n");
		string in_data = mrequestdata["in_data"];
		int in_data_length = atoi(mrequestdata["in_data_length"].c_str());
		int hash_alg_id = atoi(mrequestdata["hash_alg_id"].c_str());

		//Base64 decode
		char byInData[8192] = {0};
		int byInDataLength = CMailCoder::base64_decode( const_cast<char *>(in_data.c_str()), 
			in_data.length(), byInData );
		//printf("TSA_CREATE_TS_REQUEST base64_decode byInDataLength = %d\n", byInDataLength);
		//printf("TSA_CREATE_TS_REQUEST base64_decode byInData = %s\n", byInData);
		//process
		SGD_UINT8 byTSRequest[8192] = {0};
		SGD_UINT32 uiTSRequestLength = 0;
		uiretCode = STF_CreateTSRequest(hTSHandle, (SGD_UINT8 *)byInData, byInDataLength, 0, NULL, NULL, 
			hash_alg_id, byTSRequest, &uiTSRequestLength);
		//printf("TSA_CREATE_TS_REQUEST process uiretCode = %08x\n", uiretCode);
		//printf("TSA_CREATE_TS_REQUEST uiTSRequestLength = %u\n", uiTSRequestLength);
		if (SAR_OK == uiretCode){
			//Base64 encode
			char byTSRequestB64[8192] = {0};
			int byTSRequestB64Len = CMailCoder::base64_encode((char *)byTSRequest, uiTSRequestLength, byTSRequestB64);
			//printf("TSA_CREATE_TS_REQUEST base64_encode byTSRequestB64Len = %d\n", byTSRequestB64Len);
			//printf("TSA_CREATE_TS_REQUEST base64_encode byTSRequestB64 = %s\n", byTSRequestB64);
			//response
			string ts_request = byTSRequestB64;
			mresponsedata["ts_request"] = ts_request;
			mresponsedata["ts_request_length"] = CTSACommonUtil::to_string(byTSRequestB64Len);
		}
		//printf("TSA_CREATE_TS_REQUEST end\n");
	}else if (function_define == TSA_CREATE_TS_RESPONSE){
		printf("TSA_CREATE_TS_RESPONSE start\n");
		string ts_request = mrequestdata["ts_request"];
		int ts_request_length = atoi(mrequestdata["ts_request_length"].c_str());
		int signature_alg_id = atoi(mrequestdata["signature_alg_id"].c_str());

		//Base64 decode
		char byRequest[8192] = {0};
		int byRequestLen = CMailCoder::base64_decode( const_cast<char *>(ts_request.c_str()), 
			ts_request.length(), byRequest );
		printf("TSA_CREATE_TS_RESPONSE base64_decode byRequestLen = %d\n", byRequestLen);
		//process
		SGD_UINT8 byTSResponse[8192] = {0};
		SGD_UINT32 uiTSResponseLen = 0;
		uiretCode = STF_CreateTSResponse(hTSHandle, (SGD_UINT8 *)byRequest, byRequestLen, signature_alg_id, 
			byTSResponse, &uiTSResponseLen);
		printf("TSA_CREATE_TS_RESPONSE process uiretCode = %08x\n", uiretCode);
		printf("TSA_CREATE_TS_RESPONSE uiTSResponseLen = %u\n", uiTSResponseLen);
		if (SAR_OK == uiretCode){
			//Base64 encode
			char byTSResponseB64[8192] = {0};
			int byTSResponseB64Len = CMailCoder::base64_encode((char *)byTSResponse, uiTSResponseLen, byTSResponseB64);
			printf("TSA_CREATE_TS_RESPONSE base64_encode byTSResponseB64Len = %d\n", byTSResponseB64Len);
			printf("TSA_CREATE_TS_RESPONSE base64_encode byTSResponseB64 = %s\n", byTSResponseB64);
			//response
			string ts_response = byTSResponseB64;
			mresponsedata["ts_response"] = ts_response;
			mresponsedata["ts_response_length"] = CTSACommonUtil::to_string(byTSResponseB64Len);
		}
		printf("TSA_CREATE_TS_RESPONSE end\n");
	}else if (function_define == TSA_VERIFY_TS_VALIDITY){
		printf("TSA_VERIFY_TS_VALIDITY start\n");
		string ts_response = mrequestdata["ts_response"];
		string ts_cert = mrequestdata["ts_cert"];
		string in_data = mrequestdata["in_data"];
		int ts_response_length = atoi(mrequestdata["ts_response_length"].c_str());
		int ts_cert_length = atoi(mrequestdata["ts_cert_length"].c_str());
		int hash_alg_id = atoi(mrequestdata["hash_alg_id"].c_str());
		int signature_alg_id = atoi(mrequestdata["signature_alg_id"].c_str());
		int in_data_length = atoi(mrequestdata["in_data_length"].c_str());

		//Base64 decode
		char byResponse[8192] = {0}, byCert[8192] = {0}, byInData[8192] = {0};
		int byResponseLen = CMailCoder::base64_decode( const_cast<char *>(ts_response.c_str()), 
			ts_response.length(), byResponse );
		int byCertLen = CMailCoder::base64_decode( const_cast<char *>(ts_cert.c_str()), 
			ts_cert.length(), byCert );
		int byInDataLen = CMailCoder::base64_decode( const_cast<char *>(in_data.c_str()), 
			in_data.length(), byInData );
		printf("TSA_VERIFY_TS_VALIDITY base64_decode byResponseLen = %d\n", byResponseLen);
		printf("TSA_VERIFY_TS_VALIDITY base64_decode byCertLen = %d\n", byCertLen);
		printf("TSA_VERIFY_TS_VALIDITY base64_decode byInDataLen = %d\n", byInDataLen);
		//process
		uiretCode = STF_VerifyTSValidity(hTSHandle, (SGD_UINT8 *)byResponse, byResponseLen, hash_alg_id, 
			signature_alg_id, (SGD_UINT8 *)byCert, byCertLen, (SGD_UINT8 *)byInData, byInDataLen);
		printf("TSA_VERIFY_TS_VALIDITY process uiretCode = %08x\n", uiretCode);
		if (SAR_OK == uiretCode){
			mresponsedata["verify_result"] = "0";
		}
		
		printf("TSA_VERIFY_TS_VALIDITY end\n");
	}else if (function_define == TSA_GET_TS_INFO){
		printf("TSA_GET_TS_INFO start\n");
		string ts_response = mrequestdata["ts_response"];
		int ts_response_length = atoi(mrequestdata["ts_response_length"].c_str());

		//Base64 decode
		char byResponse[8192] = {0};
		int byResponseLen = CMailCoder::base64_decode( const_cast<char *>(ts_response.c_str()), 
			ts_response.length(), byResponse );
		printf("TSA_GET_TS_INFO base64_decode byResponseLen = %d\n", byResponseLen);
		printf("TSA_GET_TS_INFO base64_decode byResponse = %s\n", byResponse);
		//process
		SGD_UINT8 byIssuerName[8192] = {0};
		SGD_UINT8 byTime[8192] = {0};
		SGD_UINT32 uiIssuerNameLength = 0, uiTimeLength = 0;
		uiretCode = STF_GetTSInfo(hTSHandle, (SGD_UINT8 *)byResponse, byResponseLen, byIssuerName, 
			&uiIssuerNameLength, byTime, &uiTimeLength);
		printf("TSA_GET_TS_INFO process uiretCode = %08x\n", uiretCode);
		printf("TSA_GET_TS_INFO byIssuerName = %s\n", byIssuerName);
		printf("TSA_GET_TS_INFO byTime = %s\n", byTime);
		if (SAR_OK == uiretCode){
			//Base64 encode
			char byIssuerNameB64[8192] = {0};
			char byTimeB64[8192] = {0};
			int byIssuerNameB64Len = CMailCoder::base64_encode((char *)byIssuerName, uiIssuerNameLength, byIssuerNameB64);
			int byTimeB64Len = CMailCoder::base64_encode((char *)byTime, uiTimeLength, byTimeB64);
			printf("TSA_GET_TS_INFO base64_encode byIssuerNameB64 = %d\n", byIssuerNameB64);
			printf("TSA_GET_TS_INFO base64_encode byTimeB64 = %s\n", byTimeB64);
			//response
			string issuer_name = byIssuerNameB64;
			string strtime = byTimeB64;
			mresponsedata["issuer_name"] = byIssuerNameB64;
			mresponsedata["issuer_name_length"] = byIssuerNameB64Len;
			mresponsedata["time"] = byTimeB64;
			mresponsedata["time_length"] = byTimeB64Len;
		}
		
		printf("TSA_GET_TS_INFO end\n");
	}else if (function_define == TSA_GET_TS_DETAIL){
		printf("TSA_GET_TS_DETAIL start\n");
		string ts_response = mrequestdata["ts_response"];
		int ts_response_length = atoi(mrequestdata["ts_response_length"].c_str());
		int item_number = atoi(mrequestdata["item_number"].c_str());

		//Base64 decode
		char byResponse[8192] = {0};
		int byResponseLen = CMailCoder::base64_decode( const_cast<char *>(ts_response.c_str()), 
			ts_response.length(), byResponse );
		printf("TSA_GET_TS_DETAIL base64_decode byResponseLen = %d\n", byResponseLen);
		printf("TSA_GET_TS_DETAIL base64_decode byResponse = %s\n", byResponse);
		//process
		SGD_UINT8 byItemValue[8192] = {0};
		SGD_UINT32 uiItemValueLength = 0;
		uiretCode = STF_GetTSDetail(hTSHandle, (SGD_UINT8 *)byResponse, byResponseLen, item_number, 
			byItemValue, &uiItemValueLength);
		printf("TSA_GET_TS_DETAIL process uiretCode = %08x\n", uiretCode);
		printf("TSA_GET_TS_DETAIL byItemValue = %s\n", byItemValue);
		if (SAR_OK == uiretCode){
			//Base64 encode
			char byItemValueB64[8192] = {0};
			int byItemValueLen = CMailCoder::base64_encode((char *)byItemValue, uiItemValueLength, byItemValueB64);
			printf("TSA_GET_TS_DETAIL base64_encode byItemValueB64 = %s\n", byItemValueB64);
			//response
			string item_value = byItemValueB64;
			mresponsedata["item_value"] = item_value;
			mresponsedata["item_value_length"] = byItemValueLen;
		}
		
		printf("TSA_GET_TS_DETAIL end\n");
	}

	//Clear
	if (g_app.m_nCryptoAlg == 1) {
		SDF_CloseSession(hSessionHandle);
    	SDF_CloseDevice(hDeviceHandle);
	}else{
		KSSIGN_Finalize();
	}

	MYTOOL_GetReturnString(uiretCode, PKIerrmsg);
	errMsg = PKIerrmsg;
	//g_app.log.KTConsolePrint(LOG_LEVEL_DEBUG, "TSAFunctionAssign end\n");
	//printf("TSAFunctionAssign uiretCode = %u\n", uiretCode);
	return uiretCode;
}


/**************************************************************************************************/
int toSGDAlgID(int KTAlgID)
{
	int AlgID = KTAlgID;
	switch (KTAlgID)
	{
	case SGD_SM2_1:
		AlgID = 0x00020200;
		break;
	case SGD_SM2_2:
		AlgID = 0x00020400;
		break;
	case SGD_SM2_3:
		AlgID = 0x00020800;
		break;
	case SGD_SM3_SM2:
		AlgID = 0x00020201;
		break;
	default:
		break;
	}
	return AlgID;
}

int FMSM2YCL(void *hSessionHandle, unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
	printf("FMSM2YCL start \n");
	long retcode = 0;
	//导出签名公钥
	ECCrefPublicKey pucEccKey;
	ECCrefPublicKey *pEccPubKey = NULL;
	memset(&pucEccKey, 0, sizeof(pucEccKey));
	retcode = SDF_ExportSignPublicKey_ECC(hSessionHandle, nKeyIndex, &pucEccKey);
	//printf("FMSM2YCL SDF_ExportSignPublicKey_ECC ret = %d\n", retcode);
	if(retcode != 0) return retcode;
	pEccPubKey = &pucEccKey;
	unsigned char *pUserid = (unsigned char*)SGD_STR_ECCUSERID;
	/*
	printf("ECCref_MAX_LEN====%d\n", ECCref_MAX_LEN);
	string key_x = CTSACommonUtil::to_hexstring(pEccPubKey->x, ECCref_MAX_LEN);
	printf("SDF_ExportSignPublicKey_ECC key_x = %s\n", key_x.c_str());
	string key_y = CTSACommonUtil::to_hexstring(pEccPubKey->y, ECCref_MAX_LEN);
	printf("SDF_ExportSignPublicKey_ECC key_y = %s\n", key_y.c_str());
	*/
	//Hash
	unsigned int uiHashValueLen = 0;
	retcode = SDF_HashInit(hSessionHandle, toSGDAlgID(SGD_SM3), pEccPubKey, pUserid, strlen(SGD_STR_ECCUSERID));
	//printf("FMSM2YCL SDF_HashInit ret = %d\n", retcode);
	if(retcode != 0) return retcode;
	retcode = SDF_HashUpdate(hSessionHandle, in, inlen);
	//printf("FMSM2YCL SDF_HashUpdate ret = %d\n", retcode);
	if(retcode != 0) return retcode;
	retcode = SDF_HashFinal(hSessionHandle, out, &uiHashValueLen);
	//printf("FMSM2YCL SDF_HashFinal ret = %d\n", retcode);
	if(retcode != 0) return retcode;
	*outlen = uiHashValueLen;

	string hex_out = CTSACommonUtil::to_hexstring(out, *outlen);
	//printf("FMSM2YCL hex_out = %s\n", hex_out.c_str());

	printf("FMSM2YCL end \n");
	return retcode;
}

int FMFuncDigest( void *hSessionHandle,	int algid, unsigned char *in, int inlen, unsigned char *cert, int certlen,
	unsigned char *out,	int *outlen)
{
	//printf("FMDigest start \n");
	long retcode = 0;
	//判断是否需要预处理运算
	if( cert && algid == toSGDAlgID(SGD_SM3))
	{
		//printf("FMDigest...FMSM2YCL \n");
		retcode = FMSM2YCL(hSessionHandle, in, inlen, out, outlen);
		if(retcode != 0) return retcode;
	}
	else
	{	
		retcode = SDF_HashInit(hSessionHandle, algid, NULL, NULL, 0);
		if(retcode != 0) return retcode;
		retcode = SDF_HashUpdate(hSessionHandle, in, inlen);
		if(retcode != 0) return retcode;
		retcode = SDF_HashFinal(hSessionHandle, out, (unsigned int *)outlen);
		if(retcode != 0) return retcode;
	}
	
	//printf("FMDigest end \n");

	return retcode;
}

int FMFuncPrivEnc( void *hSessionHandle, unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
	printf("FMFuncPrivEnc start \n");
	long retcode = 0;
	unsigned char password[] = "12345678";
	ECCSignature ECCsign;
	memset(&ECCsign, 0, sizeof(ECCsign));
	//
	retcode = SDF_GetPrivateKeyAccessRight(hSessionHandle, nRSAKeyMax+nKeyIndex, password, 8);
	if(retcode != 0) return retcode;
	retcode = SDF_InternalSign_ECC(hSessionHandle, nKeyIndex, in, inlen, &ECCsign);
	if(retcode != 0) return retcode;
	retcode = SDF_ReleasePrivateKeyAccessRight(hSessionHandle, nRSAKeyMax+nKeyIndex);
	if(retcode != 0) return retcode;
	
	//转换为国密标准(ASN1格式)
	memset(out, 0, sizeof(out));
	long lSignLen = sizeof(out);
	int offset = 0;
#ifdef ECCMAXBITS256
	offset = 0;
#else
	offset = 32;
#endif
	retcode = MYTOOL_SM2SignDataEncode(ECCsign.r + offset, 32, ECCsign.s + offset, 32, out, &lSignLen);
	if(retcode != 0) return retcode;
	*outlen = (int)lSignLen;
	
	printf("FMFuncPrivEnc end \n");
	return retcode;
}

int FMFuncCertVrfSign( void *hSessionHandle, bool bIsDigest, unsigned char *data, int datalen, 
	unsigned char *sign, int signlen, unsigned char *cert, int certlen)
{
	printf("FMFuncCertVrfSign start \n");
	long retcode = 0;
	unsigned char *data_hash;
	int data_hash_len = 0;
	//判断是否需要预处理
	if( bIsDigest ){
		data_hash = data;
		data_hash_len = datalen;
	}else {
		printf("FMFuncCertVrfSign...FMSM2YCL \n");
		unsigned char out[64] = {0};
		int outlen = 0;
		retcode = FMSM2YCL(hSessionHandle, data, datalen, out, &outlen);
		if(retcode != 0) return retcode;
		data_hash = out;
		data_hash_len = outlen;
	}
	
	//Decode
	unsigned char out_r[64] = {0}, out_s[64] = {0};
	long outlen_r = 0, outlen_s = 0;
	retcode = MYTOOL_SM2SignDataDecode(sign, signlen, out_r, &outlen_r, out_s, &outlen_s);
	//printf("MYTOOL_SM2SignDataDecode retcode: %ld \n", retcode);
	if(retcode != 0) return retcode;
	ECCSignature ECCsign;
	memset(&ECCsign, 0, sizeof(ECCsign));
	int offset = 0;
#ifdef ECCMAXBITS256
	offset = 0;
#else
	offset = 32;
#endif
	memcpy(ECCsign.r+offset, out_r, outlen_r);
	memcpy(ECCsign.s+offset, out_s, outlen_s);
	//InternalVerify_ECC
	/*
	retcode = SDF_InternalVerify_ECC(hSessionHandle, nKeyIndex, data_hash, data_hash_len, &ECCsign);
	printf("SDF_InternalVerify_ECC retcode: %ld \n", retcode);
	if(retcode != 0) return retcode;*/
	//获取证书公钥
	unsigned char pubkey_x[64] = {0}, pubkey_y[64] = {0};
	long pubkeylen_x = 0, pubkeylen_y = 0;
	retcode = MYTOOL_ECCGetPubKeyFromCert(cert, certlen, pubkey_x, &pubkeylen_x, pubkey_y, &pubkeylen_y);
	//printf("MYTOOL_ECCGetPubKeyFromCert retcode: %ld \n", retcode);
	if(retcode != 0) return retcode;

	//ExternalVerify_ECC
	ECCrefPublicKey pucEccKey;
	memset(&pucEccKey, 0, sizeof(pucEccKey));
	pucEccKey.bits = pubkeylen_x * 8;
	memcpy(pucEccKey.x+offset, pubkey_x, pubkeylen_x);
	memcpy(pucEccKey.y+offset, pubkey_y, pubkeylen_y);
	retcode = SDF_ExternalVerify_ECC(hSessionHandle, toSGDAlgID(SGD_SM2_1), &pucEccKey, 
		data_hash, data_hash_len, &ECCsign);
	//printf("SDF_ExternalVerify_ECC retcode: %ld \n", retcode);

	printf("FMFuncCertVrfSign end \n");
	return retcode;
}

/**************************************************************************************************/

SGD_UINT32 STF_InitEnvironment(void **phTSHandle)
{
	SGD_UINT32 retcode = 0;
	if (g_app.m_nCryptoAlg == 1) {
		void *hDeviceHandle, *hSessionHandle;
		retcode = SDF_OpenDevice(&hDeviceHandle);
		if(retcode != 0) return retcode;
		retcode = SDF_OpenSession(hDeviceHandle, &hSessionHandle);
		if(retcode != 0) return retcode;
	}else return KSSIGN_Initialize();
}

SGD_UINT32 STF_ClearEnvironment(void* hTSHandle)
{
	SGD_UINT32 retcode = 0;
	if (g_app.m_nCryptoAlg == 1) {
		void *hDeviceHandle, *hSessionHandle;
		retcode = SDF_CloseSession(hSessionHandle);
		if(retcode != 0) return retcode;
		retcode = SDF_CloseDevice(hDeviceHandle);
		if(retcode != 0) return retcode;
	}else return KSSIGN_Finalize();
}

SGD_UINT32 STF_CreateTSRequest( void* hTSHandle, 
	SGD_UINT8 *pucInData, 
	SGD_UINT32 uiInDataLength, 
	SGD_UINT32 uiReqType, 
	SGD_UINT8 *pucTSExt,
	SGD_UINT32 *uiTSExtLength,
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT8 *pucTSRequest, 
	SGD_UINT32 *puiTSRequestLength)
{
	SGD_UINT32 retcode = 0;
	int outlen = 0;
	if (g_app.m_nCryptoAlg == 1) {
		retcode = KSSIGN_CreateTimeStampReq( hTSHandle, FMFuncDigest, uiHashAlgID, pucInData, uiInDataLength,
			pucTSRequest, &outlen);
	}else{
		retcode = KSSIGN_CreateTimeStampReq( hTSHandle, NULL, uiHashAlgID, pucInData, uiInDataLength, 
        	pucTSRequest, &outlen);
	}
	*puiTSRequestLength = outlen;
	return retcode;
}

SGD_UINT32 STF_CreateTSResponse(void* hTSHandle,
	SGD_UINT8 *pucTSRequest,
	SGD_UINT32 uiTSRequestLength,
	SGD_UINT32 uiSignatureAlgID,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 *puiTSResponseLength)
{
	SGD_UINT32 retcode = 0;
	int outlen = 0;
	bool bWithStatusInfo = false; // 是否包含状态信息
	char pszSN[128] = {0};
	g_app.createTSASerialNumber(pszSN);
	//char pszSN[] = "24d07d32427739d926cc2c9a236e1ba8";
	if (g_app.m_nCryptoAlg == 1) {
		retcode = KSSIGN_CreateTimeStamp(hTSHandle, FMFuncPrivEnc, FMFuncDigest, SGD_SM3, bWithStatusInfo, 
			pucTSRequest, uiTSRequestLength, g_app.m_FMSM2Cert, g_app.m_FMSM2CertLen, NULL, 0, pszSN, 
			pucTSResponse, &outlen);
	}else{
		retcode = KSSIGN_CreateTimeStamp(hTSHandle, NULL, NULL, SGD_SM3, bWithStatusInfo, 
			pucTSRequest, uiTSRequestLength, g_app.m_SM2Cert, g_app.m_SM2CertLen, 
			g_app.m_SM2PKey, g_app.m_SM2PKeyLen, pszSN, pucTSResponse, &outlen);
	}
	*puiTSResponseLength = outlen;
    return retcode;
}

SGD_UINT32 STF_VerifyTSValidity(void* hTSHandle,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT32 uiSignatureAlgID, 
	SGD_UINT8 *pucTSCert, 
	SGD_UINT32 uiTSCertLength,
	SGD_UINT8 *pucInData, 
	SGD_UINT32 uiInDataLength)
{
	SGD_UINT32 retcode = 0;
	if (g_app.m_nCryptoAlg == 1) {
		retcode = KSSIGN_VerifyTimeStamp(hTSHandle, FMFuncCertVrfSign, FMFuncDigest, 
			pucTSResponse, uiTSResponseLength, pucTSCert, uiTSCertLength, pucInData, uiInDataLength, false);
	}else{
		retcode = KSSIGN_VerifyTimeStamp(hTSHandle, NULL, NULL, pucTSResponse, uiTSResponseLength, 
			pucTSCert, uiTSCertLength, pucInData, uiInDataLength, false);
	}
    return retcode;
}

SGD_UINT32 STF_GetTSInfo(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT8 *pucIssuerName, 
	SGD_UINT32 *puiIssuerNameLength, 
	SGD_UINT8 *pucTime, 
	SGD_UINT32 *puiTimeLength)
{
	SGD_UINT32 retcode = 0;
	//获取时间戳证书
	unsigned char cert[8192] = {0};
	int cert_length = 0;
	retcode = KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 2, cert, &cert_length);
	printf("STF_GetTSInfo...cert_length=======%d\n", cert_length);
	//解析DN
	/*C=CN
	S=福建省
	L=福州市
	O=凯特信安
	OU=技术部
	CN=kinsec
	*/
	unsigned char dn[128] = {0};
	long dnlen = 0;
	retcode = MYTOOL_GetCertDN(cert, cert_length, (char *)dn, &dnlen);
	printf("STF_GetTSInfo...[DN]=======\n%s\n", dn);
	pucIssuerName = dn;
	*puiIssuerNameLength = dnlen;

	//解析时间戳  kinsec:2021-05-31 12:19:11.827963 dean:20210525153710
	unsigned char time_stamp[64] = {0};
	int time_stamp_length = 0;
	retcode = KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 8, time_stamp, &time_stamp_length);
	printf("STF_GetTSInfo...time_stamp=======%s\n", time_stamp);
	pucTime = time_stamp;
	*puiTimeLength = time_stamp_length;

	return retcode;
}

SGD_UINT32 STF_GetTSDetail(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiItemnumber, 
	SGD_UINT8 *pucItemValue, 
	SGD_UINT32 *puiItemValueLength)
{
	SGD_UINT32 retcode = 0;

	if (uiItemnumber == STF_TIME_OF_STAMP){
		unsigned char time_stamp[64] = {0};
		int length = 0;
		retcode = KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 8, time_stamp, &length);
		printf("STF_GetTSDetail...STF_TIME_OF_STAMP=======%s\n", time_stamp);
		pucItemValue = time_stamp;
		*puiItemValueLength = length;
	}else if (uiItemnumber == STF_CN_OF_TSSIGNER){
		//获取时间戳证书
		unsigned char cert[8192] = {0};
		int cert_length = 0;
		retcode = KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 2, cert, &cert_length);
		printf("STF_GetTSDetail...cert_length=======%d\n", cert_length);
		//解析DN
		unsigned char dn[128] = {0};
		long dnlen = 0;
		retcode = MYTOOL_GetCertDN(cert, cert_length, (char *)dn, &dnlen);
		printf("STF_GetTSDetail...[DN]=======\n%s\n", dn);
		pucItemValue = dn;
		*puiItemValueLength = dnlen;
	}else if (uiItemnumber == STF_ORIGINAL_DATA){
		unsigned char original_data[8192] = {0};
		int length = 0;
		retcode = KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 1, original_data, &length);
		printf("STF_GetTSDetail...original_data=======%s\n", original_data);
		pucItemValue = original_data;
		*puiItemValueLength = length;
	}else if (uiItemnumber == STF_CERT_OF_TSSERVER){
		//获取时间戳证书
		unsigned char cert[8192] = {0};
		int cert_length = 0;
		retcode = KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 2, cert, &cert_length);
		printf("STF_GetTSDetail...cert_length=======%d\n", cert_length);
		pucItemValue = cert;
		*puiItemValueLength = cert_length;
	}else if (uiItemnumber == STF_CERTCHAIN_OF_TSSERVER){
		return SAR_NotSupportYetErr;
	}else if (uiItemnumber == STF_SOURCE_OF_TIME){
		return SAR_NotSupportYetErr;
	}else if (uiItemnumber == STF_TIME_PRECISION){
		return SAR_NotSupportYetErr;
	}else if (uiItemnumber == STF_RESPONSE_TYPE){
		return SAR_NotSupportYetErr;
	}else if (uiItemnumber == STF_SUBJECT_COUNTRY_OF_TSSIGNER){
		return SAR_NotSupportYetErr;
	}else if (uiItemnumber == STF_SUBJECT_ORGNIZATION_OF_TSSIGNER){
		return SAR_NotSupportYetErr;
	}else if (uiItemnumber == STF_SUBJECT_CITY_OF_TSSIGNER){
		return SAR_NotSupportYetErr;
	}else if (uiItemnumber == STF_SUBJECT_EMIAL_OF_TSSIGNER){
		return SAR_NotSupportYetErr;
	}
	else return SAR_NotSupportYetErr;
	
	return retcode;
}