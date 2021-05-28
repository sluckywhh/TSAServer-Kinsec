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
    CTSACommonUtil util;
    locker m_queuelocker;

public:
    string m_strIP;
    int m_nHttpPort;

    //软证书
    char m_SM2Cert_B64[8192];   //时间戳证书(Base64)
	char m_SM2PKey_B64[1024];   //时间戳证书私钥(Base64)
    unsigned char *m_SM2Cert;   //时间戳证书
	unsigned char *m_SM2PKey;   //时间戳证书私钥
    int m_SM2CertLen;           //时间戳证书长度
    int m_SM2PKeyLen;           //时间戳证书私钥长度
	//X509* m_SM2Ca;
	//X509* m_SM2RootCa;
	time_t m_SM2BeforeValid;
	time_t m_SM2AfterValid;

};





#endif