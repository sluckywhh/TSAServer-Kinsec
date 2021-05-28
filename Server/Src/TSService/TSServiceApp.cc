#include "TSServiceApp.h"
#include "TSServer.h"
#include "MailCoder.h"
#include "XmlDocument.h"


const char *setting_file = "SysSetting/TSASetting.xml";
const int DefPort = 80;
const int DefParCnt = 256;

extern CTSServiceApp g_app;

time_t ASN1_TIME_get ( ASN1_TIME  * a, 	int  *err );

DK_Worker * CreateNewWorker()
{
	return new CTSServer(g_app.m_RsaCert, g_app.m_RsaPKey, g_app.m_RsaCa, g_app.m_RsaRootCa,
	                          g_app.m_RsaBeforeValid, g_app.m_RsaAfterValid, g_app.m_Sm2Cert,
							  g_app.m_Sm2PKey, g_app.m_Sm2Ca, g_app.m_Sm2RootCa,
							  g_app.m_Sm2BeforeValid, g_app.m_Sm2AfterValid, &g_app.m_lock, g_app.m_bDb);
}

CTSServiceApp::CTSServiceApp()
{
	m_nServicePort = DefPort;
	m_nParallelCount = DefParCnt;
	m_bDb = false;
	m_RsaCert = NULL;
	m_RsaPKey = NULL;
	m_RsaCa = NULL;
	m_RsaRootCa = NULL;
	memset(m_Sm2Cert, 0, sizeof(m_Sm2Cert));
	memset(m_Sm2PKey, 0, sizeof(m_Sm2PKey));
	m_Sm2Ca = NULL;
	m_Sm2RootCa = NULL;
	pthread_mutex_init(&m_lock, NULL);
}

CTSServiceApp::~CTSServiceApp()
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

int LoadCerB64( const char *file, X509 **ppX509_CA)
{
	char *tmp = NULL;

	FILE *fpCA = fopen( file, "rb");
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
	if(pos != std::string::npos)
	{	
		std::string::size_type posA = strCa.find_first_not_of( '-', pos + strFlag.length() );
		std::string::size_type posB = strCa.find_first_of( '-', posA );
		strCa = strCa.substr( posA, posB - posA );
	}
	char byCa[8192] = { 0 };
	int nbyCaLen = CMailCoder::base64_decode( const_cast<char*>( strCa.c_str() ), strCa.length(), byCa );

	if ( nbyCaLen <= 0 )
	{
		return -1;
	}
	char *tmpChar = byCa;
	X509 *x509_cacert = d2i_X509( NULL, (const unsigned char **)&tmpChar, nbyCaLen);

	if ( !x509_cacert)
	{
		return -1;
	}

	*ppX509_CA = x509_cacert;
	return 0;
}

// = Start.
int CTSServiceApp::start()
{
	CXmlDocument setting;
	DOMElement * xmlElem = NULL;
	DOMElement * xmlRoot = NULL;
	if( setting.Load( setting_file ) != 0 || ( xmlRoot = setting.GetRootElement() ) == NULL )
	{
		printf( "[Error] Load TSASetting.xml Failed.\n" );
		return -1;
	}

	xmlElem = setting.GetFirstChild( xmlRoot, "HTTPServicePort" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get HTTPService Port Failed.\n" );
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
	this->m_bDb = atoi( setting.GetElementAttribute( xmlElem, "Description" ).c_str() ) == 1;

	
	xmlElem = setting.GetFirstChild( xmlRoot, "CertificateFile" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get RsaPfx File Failed.\n" );
		return -1;
	}
	std::string RsaPfx;
	setting.GetElementAttribute( xmlElem, "Description", RsaPfx );

	xmlElem = setting.GetFirstChild( xmlRoot, "CertificatePin" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get PfxPin Failed.\n" );
		return -1;
	}
	std::string PfxPin;
	setting.GetElementAttribute( xmlElem, "Description", PfxPin );
	
	X509 *pX509  = NULL;
	EVP_PKEY *pKey = NULL ;
	if(  0 != LoadKeyEx( RsaPfx.c_str(), PfxPin.c_str(), &pX509, &pKey ) )
	{
		printf( "[Error] Load RsaCert Invalid.\n" );
		return -1;
	}
	m_RsaCert = pX509;
	m_RsaPKey = pKey;
	m_RsaBeforeValid = ASN1_TIME_get(pX509->cert_info->validity->notBefore, NULL);
	m_RsaAfterValid = ASN1_TIME_get(pX509->cert_info->validity->notAfter, NULL);

	xmlElem = setting.GetFirstChild( xmlRoot, "CertificateCA" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get RsaCa Failed.\n" );
		return -1;
	}
	std::string RsaCa;
	setting.GetElementAttribute( xmlElem, "Description", RsaCa );
	X509 *pCa  = NULL;
	if( 0 != RsaCa.length() )
	{
		if( 0 != LoadCerB64( RsaCa.c_str(), &pCa ) )
		{
			printf( "[Error] Load RsaCa Invalid.\n" );
			return -1;
		}
	}
	m_RsaCa = pCa;

	xmlElem = setting.GetFirstChild( xmlRoot, "CertificateRoot" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get RsaRoot Failed.\n" );
		return -1;
	}
	std::string RsaRootCa;
	setting.GetElementAttribute( xmlElem, "Description", RsaRootCa );
	X509 *pRoot  = NULL;
	if( 0 != LoadCerB64( RsaRootCa.c_str(), &pRoot ) )
	{
		printf( "[Error] Load RsaRootCa Invalid.\n" );
		return -1;
	}
	m_RsaRootCa = pRoot;
	
	
	xmlElem = setting.GetFirstChild( xmlRoot, "Sm2Cert" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sm2Cert Failed.\n" );
		return -1;
	}
	std::string Sm2Cert;
	setting.GetElementAttribute( xmlElem, "Description", Sm2Cert );
	GetContent(Sm2Cert.c_str(), m_Sm2Cert);
	
	if( 0 != LoadCerB64( Sm2Cert.c_str(), &pX509 ) )
	{
		printf( "[Error] Load Sm2Cert Invalid.\n" );
		return -1;
	}
	m_Sm2BeforeValid = ASN1_TIME_get(pX509->cert_info->validity->notBefore, NULL);
	m_Sm2AfterValid = ASN1_TIME_get(pX509->cert_info->validity->notAfter, NULL);
	
	
	xmlElem = setting.GetFirstChild( xmlRoot, "Sm2Pkey" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sm2Pkey Failed.\n" );
		return -1;
	}
	std::string Sm2Pkey;
	setting.GetElementAttribute( xmlElem, "Description", Sm2Pkey );
	GetContent(Sm2Pkey.c_str(), m_Sm2PKey);
	
	xmlElem = setting.GetFirstChild( xmlRoot, "Sm2Ca" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sm2Ca Failed.\n" );
		return -1;
	}
	std::string Sm2Ca;
	setting.GetElementAttribute( xmlElem, "Description", Sm2Ca );
	if( 0 != Sm2Ca.length() )
	{
		if( 0 != LoadCerB64( Sm2Ca.c_str(), &pCa ) )
		{
			printf( "[Error] Load Sm2CA Invalid.\n" );
			return -1;
		}
	}
	m_Sm2Ca = pCa;
	
	xmlElem = setting.GetFirstChild( xmlRoot, "Sm2RootCa" );
	if( xmlElem == NULL )
	{
		printf( "[Error] Get Sm2RootCa Failed.\n" );
		return -1;
	}
	std::string Sm2RootCa;
	setting.GetElementAttribute( xmlElem, "Description", Sm2RootCa );
	if( 0 != LoadCerB64( Sm2RootCa.c_str(), &pRoot ) )
	{
		printf( "[Error] Load Sm2RootCA Invalid.\n" );
		return -1;
	}
	m_Sm2RootCa = pRoot;

	if(access("ts_serial", R_OK | W_OK) !=0 )	
	{
		FILE *fp = fopen("ts_serial", "w");
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
	
	time_t curTime;
	time(&curTime);
	if(curTime < m_RsaBeforeValid)
	{
		printf( "[Error] RSA Certificate Invalid Before Valid.\n" );
		return -1;
	}
	if(curTime > m_RsaAfterValid)
	{
		printf( "[Error] RSA Certificate Invalid After Valid.\n" );
		return -1;
	}
	if(curTime < m_Sm2BeforeValid)
	{
		printf( "[Error] SM2 Certificate Invalid Before Valid.\n" );
		return -1;
	}
	if(curTime > m_Sm2AfterValid)
	{
		printf( "[Error] SM2 Certificate Invalid After Valid.\n" );
		return -1;
	}	

	// Start acceptor.
	if( this->m_acceptor.open( this->m_nServicePort, 128 ) != 0 )
	{
		printf( "[Error] Bind Service Port Failed.\n" );
		return -1;
	}

	// Start thread pool.
	if( this->m_threadPool.activate( CreateNewWorker, this->m_nParallelCount ) != 0 )
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

