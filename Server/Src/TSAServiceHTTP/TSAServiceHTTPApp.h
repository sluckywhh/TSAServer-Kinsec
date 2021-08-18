#ifndef __TSASERVICEHTTPAPP_H__
#define __TSASERVICEHTTPAPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../CommonUtil/TSACommonUtil.h"
#include "locker.h"
#include "time.h"

using namespace std;

class CTSAServiceHTTPApp
{
public:
    CTSAServiceHTTPApp(/* args */);
    ~CTSAServiceHTTPApp();
public:
    int start(); //启动服务
    void createTSASerialNumber(char *pszSN); //生成唯一时间戳序列号

private:
    locker m_queuelocker;

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

    //epoll


    //class declare
    CTSACommonUtil util;
    CKTLogKit log;
};





#endif