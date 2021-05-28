#ifndef __TSSVRSERVER_H__
#define __TSSVRSERVER_H__

#include "DK_Worker.h"
#include "DK_SocketStream.h"
#include "DK_SocketParam.h"
#include "TSEngine.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

enum HTTP_TYPE
{
	HTTP_GET,
	HTTP_POST,
	HTTP_UNSUPPORT
};
struct HttpReq
{
	HTTP_TYPE type;
	char target[1024];
	char body[8192];
	int bodylen;
};

class CTSServer : public DK_Worker
{
public :
	CTSServer(X509 *RsaCert, EVP_PKEY *RsaPkey, X509 *RsaCa, X509 *RsaRootCa, 
	time_t RsaBeforeValid, time_t RsaAfterValid, char *Sm2Cert, char *Sm2Pkey,
	X509 *Sm2Ca, X509 *Sm2RootCa, time_t Sm2BeforeValid, time_t Sm2AfterValid,
	pthread_mutex_t *plock, bool bDb);
	virtual ~CTSServer();
	DK_SocketStream m_stream;

protected :
	virtual int processRequest( const DK_Request & request );
	int sendRespone(int code, const char *data, int datalen, bool tspack);
	int recvRequest(string &pack);
	int parseNetPack(const string &pack, HttpReq &req);
	int processGet(const HttpReq &req, string &result, vector<string> &sqlactions);
	int processPost(const HttpReq &req, string &result, vector<string> &sqlactions);
	int processUnsupport(const HttpReq &req, string &result, vector<string> &sqlactions);
	void GetGmtTime(char *gmttime, int size);
	
	int sendResponse( const DK_SocketParam & resp, bool bOk );
	int recvRequest( DK_SocketParam & req );

private :
	CTSEngine m_engine;
	bool m_bDb;
	pthread_mutex_t* m_plock;
	
};

#endif

