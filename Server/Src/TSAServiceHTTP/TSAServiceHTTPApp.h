#ifndef __TSASERVICEHTTPAPP_H__
#define __TSASERVICEHTTPAPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>
#include <sys/types.h>
#include <dirent.h>

#include "../CommonUtil/TSACommonUtil.h"
#include "locker.h"
#include "time.h"
#include "threadpool.h"
#include "http_conn.h"

using namespace std;

#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000

class CTSAServiceHTTPApp
{
public:
    CTSAServiceHTTPApp(/* args */);
    ~CTSAServiceHTTPApp();
public:
    int start(); //启动服务
    void createTSASerialNumber(char *pszSN); //生成唯一时间戳序列号
    void initSocket();
    void initEpoll();
    //void initThreadpool();

private:
    locker m_queuelocker;
    //threadpool< http_conn >* pool;

public:
    string m_strIP;
    int m_nHttpPort;
    int m_nCryptoAlg; //密码算法：1硬件算法; 2软算法
    
    //日志等级
    int m_nConsoleLogLevel; //控制台日志等级
    int m_nFileLogLevel; //文件日志等级
    int m_nDBLogLevel; //数据库日志等级

    //软证书
    char m_SM2Cert_B64[8192];   //时间戳证书(Base64)
	char m_SM2PKey_B64[1024];   //时间戳证书私钥(Base64)
    unsigned char m_SM2Cert[8192];   //时间戳证书
	unsigned char m_SM2PKey[8192];   //时间戳证书私钥
    int m_SM2CertLen;           //时间戳证书长度
    int m_SM2PKeyLen;           //时间戳证书私钥长度
    //密码卡
    int m_FMSM2CertLen;           //密码卡时间戳证书长度
    char m_FMSM2Cert_B64[8192];   //密码卡时间戳证书(Base64)
    unsigned char m_FMSM2Cert[8192];   //密码卡时间戳证书
    //有效期
	time_t m_SM2BeforeValid;
	time_t m_SM2AfterValid;

    //socket
    int m_nListenFD;

    //epoll
    epoll_event m_Events[ MAX_EVENT_NUMBER ];
    int m_nEpollFD;

    //class declare
    CTSACommonUtil util;
    CKTLogKit log;
};





#endif