#ifndef __TSCLIENT_H__
#define __TSCLIENT_H__

#include "TssDefine.h"
#include <string>
#include <map>
using std::string;
using std::map;

class CTSClient
{
public :
	CTSClient();
	~CTSClient();
	int Init( const char *url, long lConnTimeoutSecs = 20 );
	int TS_Time( string &curTime );
	int TS_Resp( string &requstData, string &respData );

private:
	int CreateTssPack(const char *data, int datalen, const map<string, string> &headers, string &pack , bool is_post = true);
	int SendNetPack(const char *data, int datalen, string &respone);
	int RecvNetPack(string &respone);
	int ParseNetPack(const string &respone, string &status_code, string &reason_phrase, string &body);
	long Send( const void * pBuff, long lSize );
	long Send_n( const void * pBuff, long lSize );
	long Recv( void * pBuff, long lSize );
	long Recv_n( void * pBuff, long lSize );
	int WaitForSendData();
	int WaitForRecvData();

private :
#if defined( WIN32 )
	unsigned int socket_;
#elif defined( _LINUX )
	int socket_;
#endif
	char url_[4096];
	char host_[1024];
	char ip_[64];
	short port_;
	char target_[4096];
	long conn_timeout_;
	map<string, string> tssheader;
	map<string, string> basicheader;
};

#endif

