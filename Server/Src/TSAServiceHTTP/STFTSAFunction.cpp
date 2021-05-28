#include "STFTSAFunction.h"
#include "TSAServiceHTTPApp.h"

extern CTSAServiceHTTPApp g_app;


SGD_UINT32 TSAFunctionAssign(int function_define, map<string, string> mrequestdata, map<string, string> &mresponsedata)
{	
	SGD_UINT32 uiretCode = 0;
	void **phTSHandle = NULL;
	void *hTSHandle = NULL;
	printf("TSAFunctionAssign start\n");
	STF_InitEnvironment(phTSHandle);
	if (function_define == TSA_INIT_ENVIRONMENT){
		//uiretCode = STF_InitEnvironment(phTSHandle);
	}else if (function_define == TSA_CLEAR_ENVIRONMENT){
		//uiretCode = STF_ClearEnvironment(hTSHandle);
	}else if (function_define == TSA_CREATE_TS_REQUEST){
		printf("TSA_CREATE_TS_REQUEST start\n");
		string in_data = mrequestdata["in_data"];
		int in_data_length = atoi(mrequestdata["in_data_length"].c_str());
		int hash_alg_id = atoi(mrequestdata["hash_alg_id"].c_str());

		//Base64 decode
		char byInData[8192] = {0};
		int byInDataLength = CMailCoder::base64_decode( const_cast<char *>(in_data.c_str()), 
			in_data.length(), byInData );
		printf("TSA_CREATE_TS_REQUEST base64_decode byInDataLength = %d\n", byInDataLength);
		printf("TSA_CREATE_TS_REQUEST base64_decode byInData = %s\n", byInData);
		//process
		SGD_UINT8 byTSRequest[8192] = {0};
		SGD_UINT32 uiTSRequestLength = 0;
		uiretCode = STF_CreateTSRequest(hTSHandle, (SGD_UINT8 *)byInData, byInDataLength, 0, NULL, NULL, 
			hash_alg_id, byTSRequest, &uiTSRequestLength);
		printf("TSA_CREATE_TS_REQUEST process uiretCode = %u\n", uiretCode);
		printf("TSA_CREATE_TS_REQUEST uiTSRequestLength = %u\n", uiTSRequestLength);
		//Base64 encode
		char byTSRequestB64[8192] = {0};
		int byTSRequestB64Len = CMailCoder::base64_encode((char *)byTSRequest, uiTSRequestLength, byTSRequestB64);
		printf("TSA_CREATE_TS_REQUEST base64_encode byTSRequestB64Len = %d\n", byTSRequestB64Len);
		printf("TSA_CREATE_TS_REQUEST base64_encode byTSRequestB64 = %s\n", byTSRequestB64);
		//response
		string ts_request = byTSRequestB64;
		mresponsedata["ts_request"] = ts_request;
		mresponsedata["ts_request_length"] = CTSACommonUtil::to_string(byTSRequestB64Len);
		
		printf("TSA_CREATE_TS_REQUEST end\n");
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
		//printf("TSA_CREATE_TS_RESPONSE base64_decode byRequest = %s\n", byRequest);
		//process
		SGD_UINT8 byTSResponse[8192] = {0};
		SGD_UINT32 uiTSResponseLen = 0;
		uiretCode = STF_CreateTSResponse(hTSHandle, (SGD_UINT8 *)byRequest, byRequestLen, signature_alg_id, 
			byTSResponse, &uiTSResponseLen);
		printf("TSA_CREATE_TS_RESPONSE process uiretCode = %u\n", uiretCode);
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
			mresponsedata["ts_response_length"] = byTSResponseB64Len;
			
			printf("TSA_CREATE_TS_RESPONSE end\n");
		}
	}else if (function_define == TSA_VERIFY_TS_VALIDITY){
		printf("TSA_VERIFY_TS_VALIDITY start\n");
		string ts_response = mrequestdata["ts_response"];
		string ts_cert = mrequestdata["ts_cert"];
		int ts_response_length = atoi(mrequestdata["ts_response_length"].c_str());
		int ts_cert_length = atoi(mrequestdata["ts_cert_length"].c_str());
		int hash_alg_id = atoi(mrequestdata["hash_alg_id"].c_str());
		int signature_alg_id = atoi(mrequestdata["signature_alg_id"].c_str());

		//Base64 decode
		char byResponse[8192] = {0}, byCert[8192] = {0};
		int byResponseLen = CMailCoder::base64_decode( const_cast<char *>(ts_response.c_str()), 
			ts_response.length(), byResponse );
		int byCertLen = CMailCoder::base64_decode( const_cast<char *>(ts_cert.c_str()), 
			ts_cert.length(), byCert );
		printf("TSA_VERIFY_TS_VALIDITY base64_decode byResponseLen = %d\n", byResponseLen);
		printf("TSA_VERIFY_TS_VALIDITY base64_decode byCertLen = %d\n", byCertLen);
		//process
		uiretCode = STF_VerifyTSValidity(hTSHandle, (SGD_UINT8 *)byResponse, byResponseLen, hash_alg_id, 
			signature_alg_id, (SGD_UINT8 *)byCert, byCertLen);
		printf("TSA_VERIFY_TS_VALIDITY process uiretCode = %u\n", uiretCode);
		//response
		mresponsedata["verify_result"] = CTSACommonUtil::to_string(uiretCode);
		
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
		printf("TSA_GET_TS_INFO process uiretCode = %u\n", uiretCode);
		printf("TSA_GET_TS_INFO byIssuerName = %s\n", byIssuerName);
		printf("TSA_GET_TS_INFO byTime = %s\n", byTime);
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
		printf("TSA_GET_TS_DETAIL process uiretCode = %u\n", uiretCode);
		printf("TSA_GET_TS_DETAIL byItemValue = %s\n", byItemValue);
		//Base64 encode
		char byItemValueB64[8192] = {0};
		int byItemValueLen = CMailCoder::base64_encode((char *)byItemValue, uiItemValueLength, byItemValueB64);
		printf("TSA_GET_TS_DETAIL base64_encode byItemValueB64 = %s\n", byItemValueB64);
		//response
		string item_value = byItemValueB64;
		mresponsedata["item_value"] = item_value;
		mresponsedata["item_value_length"] = byItemValueLen;
		
		printf("TSA_GET_TS_DETAIL end\n");
	}
	STF_ClearEnvironment(hTSHandle);
	printf("TSAFunctionAssign end\n");
	printf("TSAFunctionAssign uiretCode = %u\n", uiretCode);
	return uiretCode;
}

SGD_UINT32 STF_InitEnvironment(void **phTSHandle)
{
    return KSSIGN_Initialize();
}

SGD_UINT32 STF_ClearEnvironment(void* hTSHandle)
{
    return KSSIGN_Finalize();
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
    return KSSIGN_CreateTimeStampReq(hTSHandle, NULL, uiHashAlgID, pucInData, uiInDataLength, 
        pucTSRequest, (int *)puiTSRequestLength);
}

SGD_UINT32 STF_CreateTSResponse(void* hTSHandle,
	SGD_UINT8 *pucTSRequest,
	SGD_UINT32 uiTSRequestLength,
	SGD_UINT32 uiSignatureAlgID,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 *puiTSResponseLength)
{
	printf("STF_CreateTSResponse  start\n");
	char pszSN[128] = {0};
	g_app.createTSASerialNumber(pszSN);
	
    return KSSIGN_CreateTimeStamp(hTSHandle, NULL, NULL, SGD_SM3, pucTSRequest, uiTSRequestLength, 
        g_app.m_SM2Cert, g_app.m_SM2CertLen, g_app.m_SM2PKey, g_app.m_SM2PKeyLen, pszSN, pucTSResponse, 
		(int *)puiTSResponseLength);
}

SGD_UINT32 STF_VerifyTSValidity(void* hTSHandle,
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiHashAlgID, 
	SGD_UINT32 uiSignatureAlgID, 
	SGD_UINT8 *pucTSCert, 
	SGD_UINT32 uiTSCertLength)
{
	return KSSIGN_VerifyTimeStamp(hTSHandle, NULL, NULL, pucTSResponse, uiTSResponseLength, 
		pucTSCert, uiTSCertLength);
}

SGD_UINT32 STF_GetTSInfo(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT8 *pucIssuerName, 
	SGD_UINT32 *puiIssuerNameLength, 
	SGD_UINT8 *pucTime, 
	SGD_UINT32 *puiTimeLength)
{
	//获取时间戳证书
	SGD_UINT8 *cert;
	int cert_length;
	KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 2, cert, &cert_length);
	printf("cert_length=======%d\n", cert_length);
	//解析DN
	MYTOOL_GetCertDN(cert, cert_length, (char *)pucIssuerName, (long *)puiIssuerNameLength);
	printf("puiIssuerNameLength=======%ld\n", puiIssuerNameLength);

	//解析时间戳  dean:20210525153710
	unsigned char *time_stamp;
	int time_stamp_length;
	KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, 8, time_stamp, &time_stamp_length);
	printf("time_stamp_length=======%d\n", time_stamp_length);
}

SGD_UINT32 STF_GetTSDetail(void* hTSHandle, 
	SGD_UINT8 *pucTSResponse, 
	SGD_UINT32 uiTSResponseLength, 
	SGD_UINT32 uiItemnumber, 
	SGD_UINT8 *pucItemValue, 
	SGD_UINT32 *puiItemValueLength)
{
	for(int i=1; i<=8; i++)
	{
		unsigned char *out;
		int out_length;
		KSSIGN_GetTimeStampInfo(pucTSResponse, uiTSResponseLength, i, out, &out_length);
		printf("[%d]out_length=======%d\n", i, out_length);
	}
	
}