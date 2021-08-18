#ifndef CRLINTERFACE_H_H_H_H_H
#define CRLINTERFACE_H_H_H_H_H

#ifdef __cplusplus
extern "C" {
#endif

#define CRL_OK 0	//成功
#define CRL_ERR_UNKNOWN -1
#define CRL_ERR_parameternull -2
#define CRL_ERR_Initialization -3
#define CRL_ERR_downfail -4 //crl下载失败
	/************************************************************************/
	/* 功能：crl下载
		参数：
		url：黑名单地址
		savePath:下载黑名单保存路径
		nTimeout:超时时间限制

		返回:0成功
	*/
	/************************************************************************/
	int CrlHttpDownload(const char* url, const char* savePath, int nTimeout = 30 );

	/************************************************************************/
	/* 功能：验证证书是否被作废
		参数：
		cert：二进制证书
		len:长度

		返回:0不在黑名单中，1表示在黑名单中
	*/
	/************************************************************************/
	int CrlCertInCRL(const unsigned char* cert,int len);

	
	/************************************************************************/
	/* 功能：获取吊销时间
		参数：
		[in]cert：二进制证书
		[in]len:长度
		[out]revocationtime:吊销时间utc时间

		返回:0不在黑名单中，1表示在黑名单中
	*/
	/************************************************************************/
	int GetCertRevocationTime(const unsigned char* cert,int len, unsigned int* revocationtime);

#ifdef __cplusplus
}
#endif

#endif