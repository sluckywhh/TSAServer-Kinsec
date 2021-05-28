#include "TssClient.h"
#include "FJCAApiConst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined( WIN32 )
#include <winsock2.h>
#elif defined( _LINUX )
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>
extern int h_errno;
#endif

CTSClient::CTSClient()
{
	static bool bInit = false;
	if(!bInit)
	{
		bInit = true;
#if defined(_WIN32)
		WSADATA wsa_data;
		WSAStartup( 0x202, &wsa_data );
#endif
	}
	socket_ = -1;
	tssheader.insert(std::pair<string, string>("Content-Type", "application/timestamp-query"));
	basicheader.insert(std::pair<string, string>("Accept", "text/plain"));
}

CTSClient::~CTSClient()
{
	if(socket_ != -1 )
	{
#if defined(_WIN32)
		closesocket(socket_ );
#elif defined(_LINUX)
		close(socket_ );
#endif
	}
}

int CTSClient::TS_Time( std::string & curTime )
{
	string pack;
	CreateTssPack(NULL, 0, basicheader, pack, false);
	string respone;
	int ret = SendNetPack(pack.c_str(), pack.length(), respone);
	if (ret != 0)
	{
		return ret;
	}
	string status_code;
	string reason_phrase;
	string body;
	ParseNetPack(respone, status_code, reason_phrase, body);
	int code = atoi(status_code.c_str());
	curTime = body;
	return code;
}


int CTSClient::TS_Resp( string &requstData, string &respData )
{
	string pack;
	CreateTssPack(requstData.c_str(), requstData.length(), tssheader, pack);
	string respone;
	int ret = SendNetPack(pack.c_str(), pack.length(), respone);
	if (ret != 0)
	{
		return ret;
	}
	string status_code;
	string reason_phrase;
	string body;
	ParseNetPack(respone, status_code, reason_phrase, body);
	int code = atoi(status_code.c_str());
	respData = body;
	return code;
}

int CTSClient::Init( const char * url, long lConnTimeoutSecs )
{
	strcpy(url_, url);
	string addr(url_);
	string::size_type pos = addr.find('/');
	string target = pos == string::npos ? "/" : addr.substr(pos);
	strcpy(target_, target.c_str());
	string host = pos == string::npos ? addr: addr.substr(0, pos) ;
	strcpy(host_, host.c_str());
	port_ = 80;
	string domainname = host;
	if((pos = host.find(':')) != string::npos) 
	{
		domainname = host.substr(0, pos);
		port_ = atoi(host.substr(pos+1).c_str());
	}

#if defined WIN32
	socket_ = (unsigned int)socket( PF_INET, SOCK_STREAM, 0 );
	struct hostent *remotehost = gethostbyname(domainname.c_str());
	if (remotehost == NULL)
	{
		return WSAGetLastError();
	}
	struct in_addr ip_addr;
	ip_addr.s_addr = *(unsigned long*)remotehost->h_addr;
	strcpy(ip_, inet_ntoa(ip_addr));

#elif defined(_LINUX)
	socket_ = (int)socket( PF_INET, SOCK_STREAM, 0 );
	struct hostent *remotehost = gethostbyname(domainname.c_str());
	if (remotehost == NULL)
	{
		return h_errno;
	}
	inet_ntop(remotehost->h_addrtype, remotehost->h_addr, ip_, sizeof(ip_));
#endif

	conn_timeout_ = lConnTimeoutSecs;
	return 0;
}

int CTSClient::CreateTssPack(const char *data, int datalen, const map<string, string> &headers, string &pack, bool is_post)
{
	pack.clear();
	pack.append(is_post ? "POST " : "GET ").append(target_).append(" HTTP/1.1\r\n");
	pack.append("Host: ").append(host_).append("\r\n");
	pack.append("User-Agent: timestamp-agent\r\n");
	pack.append("Connection: keep-alive\r\n");
	if (!headers.empty())
	{
		map<string, string>::const_iterator it = headers.begin();
		for (; it != headers.end(); ++it)
		{
			pack.append(it->first).append(": ").append(it->second).append("\r\n");
		}
	}
	if (is_post)
	{
		char len[16];
		sprintf(len, "%d", datalen);
		pack.append("Content-Length: ").append(len).append("\r\n");;
	}
	pack.append("\r\n");
	if (is_post)
	{
		pack.append(data, datalen);
	}
	return 0;
}


int CTSClient::SendNetPack(const char *data, int datalen, string &respone)
{
	struct sockaddr_in saddr = { 0 };
	saddr.sin_addr.s_addr = inet_addr( ip_ );
	saddr.sin_port = htons( port_ );
	saddr.sin_family = PF_INET;
#if defined(WIN32)
	BOOL bDontLinger = FALSE;
	if( ::setsockopt(socket_, SOL_SOCKET, SO_DONTLINGER, (char *)&bDontLinger, sizeof( bDontLinger ) ) < 0 )
		return -1;
#elif defined(_LINUX)
#endif
	struct linger ling;
	ling.l_onoff = 1;
	ling.l_linger = 0;   
#if defined(WIN32)
	if( ::setsockopt(socket_, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof( ling ) ) < 0 )
#elif defined(_LINUX)
	if( ::setsockopt( socket_, SOL_SOCKET, SO_LINGER, &ling, sizeof( ling ) ) < 0 )
#endif
		return -1;
	bool bRet = false;
	unsigned long ul = 1;
#if defined(WIN32)
	ioctlsocket(socket_, FIONBIO, &ul );
#elif defined(_LINUX)
	ioctl( socket_, FIONBIO, &ul );
#endif
	if( connect(socket_, (struct sockaddr *)&saddr, sizeof( saddr ) ) == -1 )
	{
		fd_set set;
		FD_ZERO( &set );
		FD_SET( socket_, &set );
		timeval tm;
		tm.tv_sec = conn_timeout_;
		tm.tv_usec = 0;
		if( select( socket_ + 1, NULL, &set, NULL, &tm ) > 0 )
		{
#if defined(WIN32)
			bRet = true;
#elif defined(_LINUX)
			int nErr = -1, nLen = sizeof(nErr);
			getsockopt( socket_, SOL_SOCKET, SO_ERROR, &nErr, (socklen_t *)&nLen );
			bRet = nErr == 0 ? true : false;
#endif
		}
		else
			bRet = false;
	}
	else
		bRet = true;
	if( bRet == false )
		return STF_TS_CONNECT_TIMEOUT;
	ul = 0;
#if defined(WIN32)
	ioctlsocket( socket_, FIONBIO, &ul ); // 设置为非阻塞模式
#elif defined(_LINUX)
	ioctl( socket_, FIONBIO, &ul ); // 设置为非阻塞模式
#endif
	int nErr = Send_n( data, datalen);
	if( nErr <= 0 )
		return STF_TS_SEND_TIMEOUT;

	return RecvNetPack(respone);
}


int CTSClient::RecvNetPack(string &respone)
{
	char szBuff[4096] = { 0 };
	long lRecvSize = Recv( szBuff, sizeof(szBuff));
	if( lRecvSize <= 0 )
		return STF_TS_RECV_TIMEOUT;
	szBuff[lRecvSize] = '\0';
	char * flag = strstr( szBuff, "Content-Length: " );
	if( flag == NULL )
		return STF_TS_RECV_TIMEOUT;
	flag += strlen( "Content-Length: " );
	char * pEnd = strstr( flag, "\r\n\r\n" );
	if( pEnd == NULL )
		return STF_TS_RECV_TIMEOUT;
	*pEnd = '\0';
	long lContentLen = atol( flag );
	*pEnd = '\r';
	int lHead = pEnd + strlen("\r\n\r\n") - szBuff;
	char *content = new char[lHead + lContentLen + 1];
	content[lHead + lContentLen ] = '\0';
	memcpy( content, szBuff, lRecvSize);
	long lBodyLen = lRecvSize - lHead;
	if( lBodyLen < lContentLen )
	{
		if( Recv_n(content + lRecvSize, lContentLen - lBodyLen ) <= 0 )
		{
			delete []content;
			return STF_TS_RECV_TIMEOUT;
		}
	}
	respone.clear();
	respone.append(content, lHead + lContentLen);
	delete[] content;
	return 0;
}

int CTSClient::ParseNetPack(const string &respone, string &status_code, string &reason_phrase, string &body)
{
	string::size_type pos = respone.find(" ");
	string temp = respone.substr(pos + 1);
	pos = temp.find(" ");
	status_code = temp.substr(0, pos);
	temp = temp.substr(pos + 1);
	pos = temp.find("\r\n");
	reason_phrase = temp.substr(0, pos);
	pos = respone.find("\r\n\r\n");
	body = respone.substr(pos + strlen("\r\n\r\n"));
	return 0;
}


long CTSClient::Send( const void * pBuff, long lSize )
{
	int nRet = WaitForSendData();
	if( nRet < 0 )
		return -1;
	else if( nRet == 0 )
		return 0;
	else
		return send( socket_, (char *)pBuff, lSize, 0 );
}


long CTSClient::Send_n( const void * pBuff, long lSize )
{
	long bt = 0;
	while( bt < lSize )
	{
		int nRet = WaitForSendData();
		if( nRet < 0 )
			return -1;
		else if( nRet == 0 )
			break; // timeout.
		long bytes_send = send(socket_, (const char *)pBuff + bt, lSize - bt, 0 );
		if( bytes_send > 0 )
			bt += bytes_send;
		else
			return -1;
	}

	return bt;
}


long CTSClient::Recv( void * pBuff, long lSize )
{
	int nRet = WaitForRecvData();
	if( nRet < 0 )
		return -1;
	else if( nRet == 0 )
		return 0;
	return recv( socket_, (char *)pBuff, lSize, 0 );
}


long CTSClient::Recv_n( void * pBuff, long lSize )
{
	long bt = 0;
	while( bt < lSize )
	{
		int nRet = WaitForRecvData();
		if( nRet < 0 )
			return -1;
		else if( nRet == 0 )
			break;
		long bytes_recv = recv( socket_, (char *)pBuff + bt, lSize - bt, 0 );
		if( bytes_recv > 0 )
			bt += bytes_recv;
		else
			return -1;
	}
	return bt;
}


int CTSClient::WaitForSendData()
{
	fd_set handle_set;
	FD_ZERO( &handle_set );
	FD_SET( socket_, &handle_set );

	struct timeval tv;
	tv.tv_usec = 0;
	tv.tv_sec = 60;
	int nRet = select( socket_ + 1, 0, &handle_set, 0, &tv );
	if( nRet > 0 && FD_ISSET( socket_, &handle_set ) )
		FD_CLR( socket_, &handle_set );
	return nRet;
}

int CTSClient::WaitForRecvData()
{
	fd_set handle_set;
	FD_ZERO( &handle_set );
	FD_SET( socket_, &handle_set );

	struct timeval tv;
	tv.tv_usec = 0;
	tv.tv_sec = 60;
	int nRet = select( socket_ + 1, &handle_set, 0, 0, &tv );
	if( nRet > 0 && FD_ISSET( socket_, &handle_set ) )
		FD_CLR( socket_, &handle_set );
	return nRet;
}




