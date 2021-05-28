// =================================================================================
//  File    : TSAServiceApp.cpp
//  Purpose : 时间戳服务器服务类
//  Author  : 林灶钢
//  Created : 2006-12-29
//  CopyRight (C) 林灶钢
// =================================================================================

#pragma warning( disable:4786 )

// =================================================================================
//  Include Header Files
#include "XmlDocument.h"
#include "TSASvrServer.h"
#include "TSAServiceApp.h"
#include "MailCoder.h"

#include <openssl/asn1.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/hmac.h>
#include <openssl/ts.h>
#include <openssl/pkcs12.h>

time_t ASN1_TIME_get ( ASN1_TIME  * a, 	int  *err );

inline DK_Worker * newWorker()
{
	return new CTSASvrServer( g_app.m_certificate, g_app.m_rPriKey, g_app.m_bUsingDb,
	                          g_app.m_SerCert, g_app.m_SerPriKey, g_app.m_SerCertCa, g_app.m_SerCertRoot,
	                          g_app.m_CertBeginValid, g_app.m_CertEndValid, &g_app.m_lock);
}

CTSAServiceApp::CTSAServiceApp()
{
	this->m_nServicePort = 80;
	this->m_nParallelCount = 256;
	this->m_bUsingDb = true;
	this->m_SerCert = NULL;
	this->m_SerPriKey = NULL;
	this->m_SerCertCa = NULL;
	this->m_SerCertRoot = NULL;
	pthread_mutex_init(&m_lock, NULL);

}

CTSAServiceApp::~CTSAServiceApp()
{
	pthread_mutex_destroy(&m_lock);
}

int LoadKeyEx( const char *pszPFX, const char *pszPwd, X509 **ppX509, EVP_PKEY **ppKey )
{
	FILE *fp = fopen( pszPFX, "rb" );
	if ( 0 == fp )
		return -1;
	PKCS12 *p12 = d2i_PKCS12_fp( fp, 0 );
	fclose(fp);
	if (p12 == 0)
		return -1;

	EVP_PKEY *pkey = 0;
	X509 *cert = 0;
	int nRet = PKCS12_parse(p12, pszPwd, &pkey, &cert, 0);
	PKCS12_free(p12);
	if (nRet != 1)
		return -1;

	if (cert == 0 || pkey == 0)
	{
		nRet = -2;
		if (pkey != 0)
		{
			EVP_PKEY_free(pkey);
			nRet = -1;
		}
		if (cert != 0)
		{
			X509_free(cert);
			nRet = -1;
		}
		return nRet;
	}

	*ppKey = pkey;
	*ppX509 = cert;
	pkey = 0;
	cert = 0;

	return 0;
}

int LoadCerB64( const char* file, X509** ppX509_CA)
{
	char* tmp = NULL;

	FILE* fpCA = fopen( file, "rb");
	if( fpCA == NULL)
	{
		return -1;
	}
	char szCa[8192] = { 0 };
	tmp = szCa;
	while( !feof(fpCA) )
	{
		fread( tmp, 1 ,1 ,fpCA );
		tmp++;
	}
	fclose(fpCA);

	std::string strFlag = "BEGIN CERTIFICATE";
	std::string strCa = szCa;

	std::string::size_type pos = strCa.find( strFlag );
	std::string::size_type posA = strCa.find_first_not_of( '-', pos + strFlag.length() );
	std::string::size_type posB = strCa.find_first_of( '-', posA );
	strCa = strCa.substr( posA, posB - posA );

	char byCa[8192] = { 0 };
	int nbyCaLen = CMailCoder::base64_decode( const_cast<char*>( strCa.c_str() ), strCa.length(), byCa );

	if ( nbyCaLen <= 0 )
	{
		return -1;
	}
	char* tmpChar = byCa;
	X509* x509_cacert = d2i_X509( NULL, (const unsigned char **)&tmpChar, nbyCaLen);

	if ( !x509_cacert)
	{
		return -1;
	}

	*ppX509_CA = x509_cacert;
	return 0;
}

// = Start.
int CTSAServiceApp::start()
{
	std::string strCertFile;
	std::string strCertPin;
	std::string strCertCa;
	std::string strCertRoot;
	// Get setting.
	CXmlDocument setting;
	DOMElement * xmlElem = NULL;
	DOMElement * xmlRoot = NULL;
	if( setting.Load( SETTING_FILE ) != 0 || ( xmlRoot = setting.GetRootElement() ) == NULL )
	{
		printf( "[Error] Load TSASetting.xml Failed.\n" );
		return -1;
	}

	xmlElem = setting.GetFirstChild( xmlRoot, "ServicePort" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Service Port Failed.\n" );
		return -1;
	}
	this->m_nServicePort = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() );

	xmlElem = setting.GetFirstChild( xmlRoot, "ParallelCount" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Parallel Count Failed.\n" );
		return -1;
	}
	this->m_nParallelCount = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() );

	xmlElem = setting.GetFirstChild( xmlRoot, "UsingDatabase" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Using Database Failed.\n" );
		return -1;
	}
	this->m_bUsingDb = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	xmlElem = setting.GetFirstChild( xmlRoot, "CertificateFile" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Certificate File Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", strCertFile );

	xmlElem = setting.GetFirstChild( xmlRoot, "CertificatePin" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Certificate Pin Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", strCertPin );

	xmlElem = setting.GetFirstChild( xmlRoot, "CertificateCA" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Certificate CA Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", strCertCa );

	xmlElem = setting.GetFirstChild( xmlRoot, "CertificateRoot" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Certificate Root Failed.\n" );
		return -1;
	}
	setting.GetElementAttribute( xmlElem, "Description", strCertRoot );


	// Start acceptor.
	if( this->m_acceptor.open( this->m_nServicePort, 128 ) != 0 )
	{
		printf( "[Error] Bind Service Port Failed.\n" );
		return -1;
	}

	// Load certificate.
	FILE * fp = fopen( strCertFile.c_str(), "rb" );
	if( fp == NULL )
	{
		printf( "[Error] Open Certificate File Failed.\n" );
		return -1;
	}
	fseek( fp, 0, SEEK_END );    //标记到文件结尾
	KTByteArray rCertData( ftell( fp )  );  //取文件大小
	rewind( fp );    //回到文件开头
	fread( &rCertData[0], 1, rCertData.size(), fp );   //把文件读到数组缓冲区
	fclose( fp );

	KTPFX pfx;    //解析软证书
	KTCRLSequence  crls;
	KTCertificates certs;
	KTRSAPrivateKeySequence privateKeys;
	if( pfx.create( &rCertData[0], rCertData.size() ) != 0 )
	{
		printf( "[Error] Certificate Format Invalid.\n" );
		return -1;
	}
	if( pfx.checkDigest( (const unsigned char *)strCertPin.c_str(),
	                     strCertPin.length() + 1 ) != 0 ||
	        pfx.getValue( (const unsigned char *)strCertPin.c_str(),
	                      strCertPin.length() + 1, privateKeys, certs, crls ) != 0 )
	{
		printf( "[Error] Certificate Pin Invalid.\n" );
		return -1;
	}

	this->m_certificate.cloneContent( *certs.indexObj( 0 ) );
	this->m_rPriKey.resize( privateKeys.indexObj( 0 )->totalCount() );
	privateKeys.indexObj( 0 )->output( &this->m_rPriKey[0] );

	int ret = -1;
	X509 *pX509  = NULL;
	EVP_PKEY *pKey = NULL ;

	if(  0 != LoadKeyEx( strCertFile.c_str(), strCertPin.c_str(), &pX509, &pKey ) )
	{
		printf( "[Error] Load Certificate Invalid.\n" );
		return -1;
	}

	m_CertBeginValid = ASN1_TIME_get(pX509->cert_info->validity->notBefore, NULL);
	m_CertEndValid = ASN1_TIME_get(pX509->cert_info->validity->notAfter, NULL);
	time_t curTime;
	time(&curTime);

	if(curTime < m_CertBeginValid)
	{
		printf( "[Error] Certificate Invalid Before Valid.\n" );
		return -1;
	}

	if(curTime > m_CertEndValid)
	{
		printf( "[Error] Certificate Invalid After Valid.\n" );
		return -1;
	}

	m_SerCert = pX509;
	m_SerPriKey = pKey;
	X509 *pCa  = NULL;
	X509 *pRoot  = NULL;
	if( 0 != strCertCa.length() )
	{
		if( 0 != LoadCerB64( strCertCa.c_str(), &pCa ) )
		{
			printf( "[Error] Load CA Invalid.\n" );
			return -1;
		}
	}
	if( 0 != LoadCerB64( strCertRoot.c_str(), &pRoot ) )
	{
		printf( "[Error] Load RootCA Invalid.\n" );
		return -1;
	}

	m_SerCertCa = pCa;
	m_SerCertRoot = pRoot;

	if (access("tsa_serial_file", R_OK | W_OK) !=0 )
	{
		FILE *fp = fopen("tsa_serial_file", "w");
		if(fp)
		{
			char Serial[18] = {0};
			time_t TimeSec;
			struct tm* pTime;
			time( &TimeSec);
			pTime = localtime( &TimeSec);
			sprintf(Serial,"%d%02d%02d%02d%02d%02d%02d%02d", 1900+pTime->tm_year, 1+pTime->tm_mon,
			        pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, pTime->tm_min, pTime->tm_sec);
			fwrite(Serial, 1, sizeof(Serial), fp);

		}
		fclose(fp);
	}

	// Start thread pool.
	if( this->m_threadPool.activate( newWorker, this->m_nParallelCount ) != 0 )
	{
		printf( "[Error] Create Thread Pool Failed.\n" );
		return -1;
	}
	return 0;
}

int  mypint( const char ** s, int n, int min, int max,int* e )
{
	int  retval = 0;
	while  (n)
	{
		if  (**s < '0' || **s > '9')
		{
			*e = 1;
			return  0;
		}
		retval *= 10;
		retval += **s - '0';
		--n;
		++(*s);
	}
	if (retval < min || retval > max) *e = 1;
	return  retval;
}

time_t ASN1_TIME_get ( ASN1_TIME  * a, 	int  *err )
{
	char  days[2][12] =
	{
		{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
		{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
	};

	int  dummy;
	const  char  *s;
	int  generalized;
	struct  tm t;
	int  i, year, isleap, offset;
	time_t  retval;
	if(err == NULL) err = &dummy;
	if(a->type == V_ASN1_GENERALIZEDTIME)
	{
		generalized = 1;
	}
	else if (a->type == V_ASN1_UTCTIME)
	{
		generalized = 0;
	}
	else
	{
		*err = 1;
		return  0;
	}
	s = (char*)a->data;  // Data should be always null terminated
	if  (s == NULL || s[a->length] != '\0')
	{
		*err = 1;
		return  0;
	}
	*err = 0;
	if  (generalized)
	{
		t.tm_year = mypint(&s, 4, 0, 9999, err) - 1900;
	}
	else
	{
		t.tm_year = mypint(&s, 2, 0, 99, err);
		if  (t.tm_year < 50) t.tm_year += 100;
	}
	t.tm_mon = mypint(&s, 2, 1, 12, err) - 1;
	t.tm_mday = mypint(&s, 2, 1, 31, err);
	// NOTE: It's not yet clear, if this implementation is 100% correct
	// for GeneralizedTime... but at least misinterpretation is
	// impossible --- we just throw an exception
	t.tm_hour = mypint(&s, 2, 0, 23, err);
	t.tm_min = mypint(&s, 2, 0, 59, err);
	if  (*s >= '0' && *s <= '9')
	{
		t.tm_sec = mypint(&s, 2, 0, 59, err);
	}
	else
	{
		t.tm_sec = 0;
	}
	if  (*err)  return  0;  // Format violation
	if  (generalized)
	{
		// skip fractional seconds if any
		while  (*s == '.' || *s == ',' || (*s >= '0' && *s <= '9')) ++s;
		// special treatment for local time
		if  (*s == 0)
		{
			t.tm_isdst = -1;
			retval = mktime(&t);  // Local time is easy :)
			if  (retval == (time_t)-1)
			{
				*err = 2;
				retval = 0;
			}
			return  retval;
		}
	}
	if  (*s == 'Z')
	{
		offset = 0;
		++s;
	}
	else  if  (*s == '-' || *s == '+')
	{
		i = (*s++ == '-');
		offset = mypint(&s, 2, 0, 12, err);
		offset *= 60;
		offset += mypint(&s, 2, 0, 59, err);
		if  (*err)  return  0;  // Format violation
		if  (i) offset = -offset;
	}
	else
	{
		*err = 1;
		return  0;
	}
	if  (*s)
	{
		*err = 1;
		return  0;
	}
	// And here comes the hard part --- there's no standard function to
	// convert struct tm containing UTC time into time_t without
	// messing global timezone settings (breaks multithreading and may
	// cause other problems) and thus we have to do this "by hand"
	//

	// NOTE: Overflow check does not detect too big overflows, but is
	// sufficient thanks to the fact that year numbers are limited to four
	// digit non-negative values.

	retval = t.tm_sec;
	retval += (t.tm_min - offset) * 60;
	retval += t.tm_hour * 3600;
	retval += (t.tm_mday - 1) * 86400;
	year = t.tm_year + 1900;
	if  ( sizeof (time_t) == 4)
	{
		// This is just to avoid too big overflows being undetected, finer
		// overflow detection is done below.
		if  (year < 1900 || year > 2040) *err = 2;
	}

	// FIXME: Does POSIX really say, that all years divisible by 4 are

	// leap years (for consistency)??? Fortunately, this problem does

	// not exist for 32-bit time_t and we should'nt be worried about

	// this until the year of 2100 :)

	isleap = ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
	for  (i = t.tm_mon - 1; i >= 0; --i) retval += days[isleap][i] * 86400;
	retval += (year - 1970) * 31536000;
	if  (year < 1970)
	{
		retval -= ((1970 - year + 2) / 4) * 86400;
		if  ( sizeof (time_t) > 4)
		{
			for  (i = 1900; i >= year; i -= 100)
			{
				if  (i % 400 == 0)  continue ;
				retval += 86400;
			}
		}
		if  (retval >= 0) *err = 2;
	}
	else
	{
		retval += ((year - 1970 + 1) / 4) * 86400;
		if  ( sizeof (time_t) > 4)
		{
			for  (i = 2100; i < year; i += 100)
			{
				// The following condition is the reason to
				// start with 2100 instead of 2000
				if  (i % 400 == 0)  continue ;
				retval -= 86400;
			}
		}
		if  (retval < 0) *err = 2;
	}
	if  (*err) retval = 0;
	return  retval;
}

// =================================================================================
//  1.00    2006-12-29    Created By LinZaoGang
//  File End
