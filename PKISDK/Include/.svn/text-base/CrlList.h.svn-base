// =================================================================================
//  File    : CrlList.h
//  Author  : HFN
//  Created : 2007-07-21
//  CopyRight (C) HFN
// =================================================================================

#ifndef __CRLLIST_H__
#define __CRLLIST_H__

// =================================================================================
//  Include Header Files
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <string>
#include <vector>
using namespace std;

// =================================================================================
//  Macro Definition
#define FORMAT_ASN1     1
#define FORMAT_PEM      3

#ifndef ERR_CACERT
#define ERR_CACERT   					4	//根证书错误
#endif
#ifndef ERR_CRLSIGN
#define ERR_CRLSIGN						9	//不是福建CA签发的CRL
#endif


// =================================================================================
//  Main Function Definition
class CCrlList
{
public :
	CCrlList();
	virtual ~CCrlList();
	void Clear();
	void Update(const char * szCrlPath);

	bool CertInCRL(X509 * pCert);
	bool CertInCRLA(const unsigned char* cert,int len);


	time_t m_revocationTime;//吊销时间
	
protected :
	//added yyx 2016-03-09 for 递归读取目录下的crl文件
	bool do_read_crl(string strCrlPath);

	X509_CRL * ReadCRL(const char * szFileName, int nFormat);
	void AddCRL(X509_CRL * crl);
	bool Find(std::vector<ASN1_INTEGER *> * pSerialList, ASN1_INTEGER * value);
	void Insert(std::vector<ASN1_INTEGER *> * pSerialList, ASN1_INTEGER * pSerialNumber);
	
	bool FindA(std::vector<ASN1_INTEGER *> * pSerialList, ASN1_INTEGER * value,std::vector<ASN1_TIME *> * prevocationDateList);
	void InsertA(std::vector<ASN1_INTEGER *> * pSerialList, ASN1_INTEGER * pSerialNumber,
		std::vector<ASN1_TIME *> * prevocationDateList,ASN1_TIME *PrevocationDate);

	struct S_CRL
	{
		X509_NAME                   * pIssuerName;
		std::vector<ASN1_INTEGER *>   serialList;
		std::vector<ASN1_TIME*>	revocationDateList;//add hfn
	};

	std::vector<S_CRL>    m_crlList;

private:
	//黑名单预先加载更新所需的读写锁 2017-12-28
	//比mutex有更高的适用性，可以多个线程同时占用读模式的读写锁，但是只能一个线程占用写模式的读写锁
	//1. 当读写锁是写加锁状态时，在这个锁被解锁之前，所有试图对这个锁加锁的线程都会被阻塞；
	//2. 当读写锁在读加锁状态时，所有试图以读模式对它进行加锁的线程都可以得到访问权，但是以写模式对它进行枷锁的线程将阻塞；
	//3. 当读写锁在读模式锁状态时，如果有另外线程试图以写模式加锁，读写锁通常会阻塞随后的读模式锁请求，这样可以避免读模式锁长期占用，而等待的写模式锁请求长期阻塞；
	pthread_rwlock_t m_rwlock;
};

// =================================================================================
//  1.00    2007-07-21    Created By LinZaoGang
//  2.00    2016-03-09    Modified By Yeyouxiao
//  3.00	2017-12-28    Modified By Yeyouxiao
//	4.00	2019-07-30	  Modified By 83735471
//  File End 
#endif  // __CRLLIST_H__
