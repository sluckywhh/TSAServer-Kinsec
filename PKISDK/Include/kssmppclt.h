#ifndef __KS_SMPP__
#define __KS_SMPP__

#include <string>

#ifdef _WIN32
	#ifndef KSSMPPCLT_EXPORTS
		#if defined(_DEBUG)
			#pragma comment(lib, "kssmppcltd.lib")
			#pragma message("Automatically linking with kssmppcltd.lib")
		#else
			#pragma comment(lib, "kssmppclt.lib")
			#pragma message("Automatically linking with kssmppclt.lib")
		#endif	
	#endif
#endif

std::string KSSMPP_GetErrorString(int nErrNum);

int KSSMPP_GetTSACert(char *pszServerIP, int nPort, char *pszTSSApplicationName, unsigned char *pbCertData, int *pnCertDataLen);
int KSSMPP_GetTSACert(const char *pszUrl, const char *pszTSSApplicationName, std::string& strCertData);

int KSSMPP_CreateTSReq(char *pszServerIP, int nPort, char *pszTSSApplicationName, unsigned char *pbSrcData, int nSrcDataLen, unsigned char *pbReqData, int *pnReqDataLen);
int KSSMPP_CreateTSReq(const char *pszUrl, const char *pszTSSApplicationName, const char *pszSrcData, std::string& strReqData);

int KSSMPP_CreateTSResp(char *pszServerIP, int nPort, char *pszTSSApplicationName, unsigned char *pbReqData, int nReqDataLen, unsigned char *pbRespData, int *pnRespDataLen);
int KSSMPP_CreateTSResp(const char *pszUrl, const char *pszTSSApplicationName, const char *pszReqData, std::string& strRespData);

int KSSMPP_VerifyTS(char *pszServerIP, int nPort, char *pszTSSApplicationName, unsigned char *pbSrcData, int nSrcDataLen, unsigned char *pbRespData, int nRespDataLen);
int KSSMPP_VerifyTS(const char *pszUrl, const char *pszTSSApplicationName, const char *pszSrcData, const char *pszRespData);

int KSSMPP_ParseTS(char *pszServerIP, int nPort, char *pszTSSApplicationName, unsigned char *pbRespData, int nRespDataLen, std::string& strParseData, int nFlag = 0);
int KSSMPP_ParseTS(const char *pszUrl, const char *pszTSSApplicationName, const char *pszRespData, std::string& strParseData, int nFlag = 0);

#endif
