#include "TSAServiceHTTPApp.h"

extern int addfd( int epollfd, int fd, bool one_shot );

CTSAServiceHTTPApp::CTSAServiceHTTPApp()
{
    m_strIP = "0.0.0.0";
    m_nHttpPort = 80;
	m_nCryptoAlg = 1;
	m_nConsoleLogLevel = LOG_LEVEL_DEBUG;
	m_nFileLogLevel = LOG_LEVEL_DEBUG;
	m_nDBLogLevel = LOG_LEVEL_DEBUG;

    memset(m_SM2Cert_B64, 0, sizeof(m_SM2Cert_B64));
	memset(m_SM2PKey_B64, 0, sizeof(m_SM2PKey_B64));
	memset(m_SM2Cert, 0, sizeof(m_SM2Cert));
	memset(m_SM2PKey, 0, sizeof(m_SM2PKey));
	memset(m_FMSM2Cert_B64, 0, sizeof(m_FMSM2Cert_B64));
	memset(m_FMSM2Cert, 0, sizeof(m_FMSM2Cert));
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
	//init log level
	string sConsoleLogLevel = util.getSettingValue_XML("ConsoleLogLevel");
    m_nConsoleLogLevel = atoi( sConsoleLogLevel.c_str() );
	string sFileLogLevel = util.getSettingValue_XML("FileLogLevel");
    m_nFileLogLevel = atoi( sFileLogLevel.c_str() );
	string sDBLogLevel = util.getSettingValue_XML("DBLogLevel");
    m_nDBLogLevel = atoi( sDBLogLevel.c_str() );
	log.init(m_nConsoleLogLevel, m_nFileLogLevel, m_nDBLogLevel);
	log.KTConsolePrint(LOG_LEVEL_INFO, "[INFO]读取配置成功：控制台日志等级[%d]，文件日志等级[%d]，数据库日志等级[%d]\n", m_nConsoleLogLevel, m_nFileLogLevel, m_nDBLogLevel);

    //port
    string port = util.getSettingValue_XML("HTTPServicePort");
    m_nHttpPort = atoi( port.c_str() );

	//CryptoAlg
	string sCryptoAlg = util.getSettingValue_XML("CryptoAlg");
    m_nCryptoAlg = atoi( sCryptoAlg.c_str() );

    //SM2Cert
    string SM2Cert = util.getSettingValue_XML("Sm2Cert");
	GetContent(SM2Cert.c_str(), m_SM2Cert_B64);
	//printf("m_SM2Cert_B64=====%s\n", m_SM2Cert_B64);
	m_SM2CertLen = CMailCoder::base64_decode( m_SM2Cert_B64, strlen(m_SM2Cert_B64), (char *)m_SM2Cert );
	//printf("m_SM2CertLen=====%d\n", m_SM2CertLen);

    //SM2PKey
    string SM2PKey = util.getSettingValue_XML("Sm2Pkey");
	GetContent(SM2PKey.c_str(), m_SM2PKey_B64);
	//printf("m_SM2PKey_B64=====%s\n", m_SM2PKey_B64);
	m_SM2PKeyLen = CMailCoder::base64_decode( m_SM2PKey_B64, strlen(m_SM2PKey_B64), (char *)m_SM2PKey );
	//printf("m_SM2PKeyLen=====%d\n", m_SM2PKeyLen);

	//FMSM2Cert index=1
    string FMSM2Cert = util.getSettingValue_XML("FMSm2Cert");
	GetContent(FMSM2Cert.c_str(), m_FMSM2Cert_B64);
	//printf("m_FMSM2Cert_B64=====%s\n", m_FMSM2Cert_B64);
	m_FMSM2CertLen = CMailCoder::base64_decode( m_FMSM2Cert_B64, strlen(m_FMSM2Cert_B64), (char *)m_FMSM2Cert );
	//printf("m_FMSM2CertLen=====%d\n", m_FMSM2CertLen);

	

    return 0;
}


void CTSAServiceHTTPApp::initSocket()
{
    m_nListenFD = socket( AF_INET, SOCK_STREAM, 0 );
    assert( m_nListenFD >= 0 );
    printf( "[Info]initSocket listen fd: %d\n", m_nListenFD );
	int reuse = 1;
    setsockopt( m_nListenFD, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    //struct linger tmp = { 1, 5 };
    //setsockopt( listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof( tmp ) );
	
    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, m_strIP.c_str(), &address.sin_addr );
    address.sin_port = htons( m_nHttpPort );
    ret = bind( m_nListenFD, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret >= 0 );
    ret = listen( m_nListenFD, 128 );
    assert( ret >= 0 );
}
void CTSAServiceHTTPApp::initEpoll()
{
    m_nEpollFD = epoll_create( MAX_FD );
    assert( m_nEpollFD != -1 );
    printf( "[Info]initEpoll epoll fd: %d\n", m_nEpollFD );
    addfd( m_nEpollFD, m_nListenFD, false ); //注册listenfd到epoll
    http_conn::m_epollfd = m_nEpollFD;
}
