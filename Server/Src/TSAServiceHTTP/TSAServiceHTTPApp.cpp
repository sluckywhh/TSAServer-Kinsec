#include "TSAServiceHTTPApp.h"

CTSAServiceHTTPApp::CTSAServiceHTTPApp()
{
    m_strIP = "127.0.0.1";
    m_nHttpPort = 80;

    memset(m_SM2Cert_B64, 0, sizeof(m_SM2Cert_B64));
	memset(m_SM2PKey_B64, 0, sizeof(m_SM2PKey_B64));
	//m_SM2Ca = NULL;
	//m_SM2RootCa = NULL;
}

CTSAServiceHTTPApp::~CTSAServiceHTTPApp()
{
}

/********************************************************************************************/
int GetContent(const char *file, char *out)
{
	char *tmp = NULL;
	FILE *fp = fopen( file, "rb");
	if( fp == NULL)
	{
		return -1;
	}
	tmp = out;
	while( !feof(fp) )
	{
		fread( tmp, 1 ,1 ,fp );
		tmp++;
	}
	fclose(fp);
	return 0;
}


/********************************************************************************************/
//生成唯一时间戳序列号(HexString)
//规则：KTTSA+时间戳+随机序号
void CTSAServiceHTTPApp::createTSASerialNumber(char *pszSN)
{
	char pctmp[64] = {0};
    time_t t;
    time(&t);
    srand(t);
	//printf("ts===========%lld\n", t);
    m_queuelocker.lock();
    int rn = rand();
	//printf("rn===========%d\n", rn);
    sprintf(pctmp, "KTTSA_%lld_%d", t, rn);
    m_queuelocker.unlock();
	//printf("pctmp===========%s\n", pctmp);
	string stmp = CTSACommonUtil::to_hexstring(pctmp);
	//printf("stmp===========%s\n", stmp.c_str());
	strcpy(pszSN, stmp.c_str());

	printf("createTSASerialNumber: %s\n", pszSN);
}

int CTSAServiceHTTPApp::start()
{
    //port
    string port = util.getSettingValue_XML("HTTPServicePort");
    m_nHttpPort = atoi( port.c_str() );

    //SM2Cert
    string SM2Cert = util.getSettingValue_XML("Sm2Cert");
	GetContent(SM2Cert.c_str(), m_SM2Cert_B64);
	printf("m_SM2Cert_B64=====%s\n", m_SM2Cert_B64);
	char byCert[8192] = {0};
	m_SM2CertLen = CMailCoder::base64_decode( m_SM2Cert_B64, strlen(m_SM2Cert_B64), byCert );
	m_SM2Cert = (unsigned char*)byCert;
	printf("m_SM2CertLen=====%d\n", m_SM2CertLen);

    //SM2PKey
    string SM2PKey = util.getSettingValue_XML("Sm2Pkey");
	GetContent(SM2PKey.c_str(), m_SM2PKey_B64);
	printf("m_SM2PKey_B64=====%s\n", m_SM2PKey_B64);
	char byPkey[2048] = {0};
	m_SM2PKeyLen = CMailCoder::base64_decode( m_SM2PKey_B64, strlen(m_SM2PKey_B64), byPkey );
	m_SM2PKey = (unsigned char*)byPkey;
	printf("m_SM2PKeyLen=====%d\n", m_SM2PKeyLen);


    return 0;
}