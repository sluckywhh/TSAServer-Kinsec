#include "TSServer.h"
#include "TssDefine.h"
#include "FJCAApiConst.h"
#include "MailCoder.h"
#include <openssl/err.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_PATH       "TSADbService.msg"
#define MSG_NUM        10370

struct S_Cmd_Message
{
	long lMsgType;
	char szCmd[8192];

	S_Cmd_Message()
	{
		lMsgType = MSG_NUM;
	}
};


int ExecuteSQLCommand( const char *szCmd )
{
	S_Cmd_Message msg;
	strcpy( msg.szCmd, szCmd );

	key_t key;
	key = ftok( MSG_PATH, 'a' );
	int msgId = msgget( key, 0 );
	if( msgId < 0 )
	{
		printf( "[Error] Write Cmd Failed: Get Message Queue Error.\n" );
		return -1;
	}
	
	if( msgsnd( msgId, &msg, sizeof( msg ) - sizeof( long ), 0 ) == -1 )
	{
		printf( "[Error] Write Cmd Failed: Send Cmd Message Error.\n" );
		return -1;
	}
	return 0;
}


CTSServer::CTSServer( X509 *RsaCert, EVP_PKEY *RsaPkey, X509 *RsaCa, X509 *RsaRootCa, 
	time_t RsaBeforeValid, time_t RsaAfterValid, char *Sm2Cert, char *Sm2Pkey,
	X509 *Sm2Ca, X509 *Sm2RootCa, time_t Sm2BeforeValid, time_t Sm2AfterValid,
	pthread_mutex_t *plock, bool bDb)
{
	m_engine.InitSerCer(RsaCert, RsaPkey, RsaCa, RsaRootCa, RsaBeforeValid, RsaAfterValid,
	Sm2Cert, Sm2Pkey, Sm2Ca, Sm2RootCa, Sm2BeforeValid, Sm2AfterValid);
	m_plock = plock;
	m_bDb = bDb;
}

CTSServer::~CTSServer()
{

}

int CTSServer::processRequest( const DK_Request &request )
{		 
	request.getRequestData( &m_stream );
	string pack;
	if( recvRequest( pack ) != 0 )
	{
		m_stream.close();
		return -1;
	}
	HttpReq req;
	parseNetPack(pack, req);
	
	int ret = 0;
	bool btspack = true;
	string result;
	vector<string> sqlactions;
	switch(req.type)
	{
		case HTTP_GET:
		ret = processGet(req, result, sqlactions);
		btspack = false;
		break;
		case HTTP_POST:
		ret = processPost(req, result, sqlactions);
		btspack = true;
		break;
		default:
		ret = processUnsupport(req, result, sqlactions);
		btspack = false;
		break;
	}
	sendRespone(ret, result.c_str(), result.length(), btspack);
	if( m_bDb )
	{
		for( int i = 0; i < sqlactions.size(); i++ )
		ExecuteSQLCommand( sqlactions[i].c_str() );
	}
	return 0;

}

int CTSServer::processGet(const HttpReq &req, string &result, vector<string> &sqlactions)
{
	if(strcmp(req.target, "/currtime") == 0)
	{
		time_t t = time( NULL );
		struct tm Tm;
		localtime_r( &t, &Tm);
		char szCurrTime[32];
		sprintf( szCurrTime, "%04d-%02d-%02d %02d:%02d:%02d", Tm.tm_year + 1900,
		Tm.tm_mon + 1, Tm.tm_mday, Tm.tm_hour, Tm.tm_min, Tm.tm_sec );
		result = szCurrTime;
	}
	else
	{
		return 403;
	}	
	return 200;
}

int CTSServer::processPost(const HttpReq &req, string &result, vector<string> &sqlactions)
{
	if(strcmp(req.target, "/timestamp") == 0)
	{
		
		int ret = 0;
		time_t curTime;
		time(&curTime);
		string serialNumber;
		char szTSTokenB64[10240] = { 0 };
		do
		{	
			if(curTime < m_engine.m_Sm2BeforeValid)
			{
				ret = SAR_CERTNOTYETVALIDErr;
				break;
			}
			if(curTime > m_engine.m_Sm2AfterValid)
			{
				ret = SAR_CERTHASEXPIREDErr;
				break;
			}
			
			int nTSRespLen = 0;
			int nTSTokenLen = 0;
			unsigned char byTSResp[10240];
			unsigned char byTSToken[10240];
			memset(byTSResp, 0, sizeof(byTSResp));
			memset(byTSToken, 0, sizeof(byTSToken));
			string tsreq;
			tsreq.append(req.body, req.bodylen);
			
			unsigned char *byTSReq = (unsigned char *)tsreq.c_str();
			int nTSReqLen = tsreq.length();
			
			pthread_mutex_lock(m_plock);
			ret = m_engine.CreateSM2TSResp(byTSReq, nTSReqLen, m_engine.m_Sm2Ca, 
			m_engine.m_Sm2RootCa, m_engine.m_Sm2Cert, m_engine.m_Sm2Pkey,
			byTSResp, &nTSRespLen, byTSToken, &nTSTokenLen, serialNumber);
			pthread_mutex_unlock(m_plock);
			
			if(ret != 0)
			{
				break;
			}				
			
			result.clear();
			result.append((char*)byTSToken, nTSTokenLen);
			
			int nTSTokenB64Len = CMailCoder::base64_encode((char *)byTSToken, nTSTokenLen, szTSTokenB64);
			if (nTSTokenB64Len <= 0)
			{
				ret = SAR_BASE64ENCODEDECODEErr;
				break;
			}
		}while(false);
		char szReqB64[4096] = {0};
		int nReqB64Len = CMailCoder::base64_encode((char *)req.body, req.bodylen, szReqB64);
		
		if(m_bDb)
		{
			struct tm Tm;
			localtime_r( &curTime, &Tm);
			char szVisitTime[32];
			sprintf( szVisitTime, "%04d-%02d-%02d %02d:%02d:%02d", Tm.tm_year + 1900,
			Tm.tm_mon + 1, Tm.tm_mday, Tm.tm_hour, Tm.tm_min, Tm.tm_sec );
		
			std::string sql;
			sql = "insert into t_time_stamp( f_entry_time, f_serial_number, f_data ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( serialNumber.c_str() );
			sql.append( "', '" );
			if(ret == 0)
			{
				sql.append( szTSTokenB64 );				
			}
			else
			{
				sql.append("无法颁发时间戳，错误码为：");
				char szErr[64];
				sprintf(szErr, "%x", ret);
				sql.append(szErr);
			}
			sql.append( "' );" );
			ExecuteSQLCommand(sql.c_str());
			
			
			sql = "insert into t_request_log( f_log_time, f_request_data, f_response_data, f_status ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( szReqB64);
			sql.append( "', '" );
			sql.append( szTSTokenB64 );
			sql.append( "', '" );
			if(ret == 0)
			{
				sql.append( "时间戳签发成功！" );				
			}
			else
			{
				sql.append("签发时间戳出错，错误码为：");
				char szErr[64];
				sprintf(szErr, "%x", ret);
				sql.append(szErr);
			}
			sql.append( "' );" );
			ExecuteSQLCommand(sql.c_str());

			sql = "insert into t_visit_log( f_log_time, f_client_ip, f_operater ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( m_stream.getHost() );
			sql.append( "', '请求颁发时间戳！' );" );
			ExecuteSQLCommand(sql.c_str());
		}
		if(ret == 0)
			return 200;
		else
			return ret;
	}
	else
	{
		return 403;
	}
}

int CTSServer::processUnsupport(const HttpReq &req, string &result, vector<string> &sqlactions)
{
	
	return 0;
}	

int CTSServer::recvRequest(string &pack)
{
	char szBuff[1024] = { 0 };
	long lRecvSize = 0;
	int nRecv = 0;
	char *pHeaderEnd = NULL;
	while(1)
	{
		int nRecv= m_stream.recv( szBuff + lRecvSize, sizeof(szBuff) - lRecvSize);
		if(nRecv <= 0 )
			return -1;
		lRecvSize += nRecv;
		pHeaderEnd = strstr( szBuff, "\r\n\r\n" );
		if(pHeaderEnd != NULL)
			break;
	}
	szBuff[lRecvSize] = '\0';
	
	bool bGet = strncmp(szBuff, "GET", 3) == 0;
	if(bGet)
	{
		char *temp = new char[pHeaderEnd - szBuff + 3];
		temp[pHeaderEnd - szBuff + 2] = '\0';
		memcpy(temp, szBuff, pHeaderEnd - szBuff + 2);
		pack = temp;
		delete []temp;
		return 0;
	}
	
	char *pContent = strstr( szBuff, "Content-Length: " );
	if( pContent == NULL )
	{
		return -1;
	}
	pContent += strlen( "Content-Length: " );
	char *pEnd = strstr( pContent, "\r\n\r\n" );
	if( pEnd == NULL )
	{
		return -1;
	}
	*pEnd = '\0';
	long lContentLen = atol( pContent );
	*pEnd = '\r';
	if( lContentLen == 0 )
		return 0;

	char *temp = new char[pHeaderEnd - szBuff + 4 + lContentLen + 1];
	temp[pHeaderEnd - szBuff + 4 + lContentLen] = '\0';
	pContent = pHeaderEnd + strlen( "\r\n\r\n" );
	long lHadRecvLen = lRecvSize - ( pContent - szBuff );
	if( lHadRecvLen > 0 )
		memcpy(temp, szBuff, lRecvSize);
	if( lHadRecvLen < lContentLen &&
	        m_stream.recv_n(temp + lRecvSize, lContentLen - lHadRecvLen ) <= 0 )
	{
		delete []temp;
		return -1;
	}
	pack.append(temp, pHeaderEnd - szBuff + 4 + lContentLen);
	
	delete []temp;
	return 0;	
	
}

int CTSServer::parseNetPack(const string &pack, HttpReq &req)
{
	string::size_type pos = pack.find(" ");
	string temp = pack.substr(0, pos);
	if(strcmp(temp.c_str(), "POST") ==0)
		req.type = HTTP_POST;
	else if(strcmp(temp.c_str(), "GET") ==0)
		req.type = HTTP_GET;
	else
		req.type = HTTP_UNSUPPORT;
	temp = pack.substr(pos + 1, pack.find(" ", pos + 1) - (pos + 1));
	strcpy(req.target, temp.c_str());
	if(req.type == HTTP_POST)
	{
		pos = pack.find("\r\n\r\n");
		temp = pack.substr(pos + 4);
		memcpy(req.body, temp.c_str(), temp.length());
		req.bodylen = temp.length();
	}
	return 0;
}

void CTSServer::GetGmtTime(char *gmttime, int size)
{
	time_t t = time( NULL );
	struct tm Tm;
	gmtime_r( &t, &Tm );
	strftime(gmttime, size, "%a, %d %b %Y %T %Z", &Tm);
}	
	
int CTSServer::sendRespone(int code, const char *data, int datalen, bool tspack)
{
	char szCode[16];
	sprintf(szCode, "%d", code);
	const char *reson_phrase = "Internal Server Error";
	switch(code)
	{
		case 200:
		reson_phrase = "OK";
		break;
		case 403:
		reson_phrase = "Forbidden";
		default:
		break;
	}
	string response;
	response.append("HTTP/1.1 ").append(szCode).append(" ").append(reson_phrase).append("\r\n");
	response.append("Server: X Timestamp Server\r\n");
	char szGmttime[32];
	GetGmtTime(szGmttime, sizeof(szGmttime));
	response.append("Date: ").append(szGmttime).append("\r\n");
	if(tspack)
	{
		response.append("Content-Type: application/timestamp-reply\r\n");		
	}
	else
	{
		response.append("Content-Type: text/plain\r\n");
	}	
	char szlen[32];
	sprintf(szlen, "%d", datalen);
	response.append("Content-Length: ").append(szlen).append("\r\n");
	response.append("Connection: close\r\n");
	response.append("\r\n");
	response.append(data, datalen);
	if( m_stream.send_n(response.c_str(), response.length()) <= 0 )
		return -1;
	return 0;
}
	


