// =================================================================================
//  File    : TSASvrServer.cpp
//  Author  : 林灶钢
//  Created : 2006-12-29
//  CopyRight (C) 林灶钢.
// =================================================================================

// =================================================================================
//  Include Header Files
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_LINUX)
#  include <sys/types.h>
#  include <sys/ipc.h>
#  include <sys/msg.h>
#  include <unistd.h>
#  include <openssl/err.h>

#endif
#include "TSASvrServer.h"
#include "MailCoder.h"
#include "TSErr.h"

// =================================================================================
//  Macro Definition
#define MSG_PATH       "TSADbService.msg"
#define MSG_NUM        10370

// =================================================================================
//  Structure Definition
struct S_Cmd_Message
{
	long lMsgType;
	char szCmd[8192];

	S_Cmd_Message()
	{
		lMsgType = MSG_NUM;
	}
};

// =================================================================================
//  Global Function Definition
int ExecuteSQLCommand( const char * szCmd )
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
	// Send the message to index process
	if( msgsnd( msgId, &msg, sizeof( msg ) - sizeof( long ), 0 ) == -1 )
	{
		printf( "[Error] Write Cmd Failed: Send Cmd Message Error.\n" );
		return -1;
	}
	return 0;
}

// =================================================================================
//  Class Definition
// = Default constructor.
CTSASvrServer::CTSASvrServer( const KTCertificate & cert, const KTByteArray & rPriKey, bool bUsingDb,
                              X509* SerCert, EVP_PKEY* SerPriKey, X509* SerCertCa, X509* SerCertRoot,
                              time_t CertBeginValid, time_t CertEndValid, pthread_mutex_t* plock)
{
	this->m_engine.loadCertificate( cert, rPriKey );
	this->m_bUsingDb = bUsingDb;
	this->m_plock = plock;
	this->m_engine.InitSerCer(SerCert, SerPriKey, SerCertCa, SerCertRoot, CertBeginValid, CertEndValid);
}

// = Default destructor.
CTSASvrServer::~CTSASvrServer()
{

}

// = Process request.
int CTSASvrServer::processRequest( const DK_Request & request )
{
	bool bOk = false;
	time_t t = time( NULL );
	struct tm * pTm = localtime( &t );
	char szVisitTime[32] = { 0 };
	sprintf( szVisitTime, "%d-%02d-%02d %02d:%02d:%02d", pTm->tm_year + 1900,
	         pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec );
	std::vector<std::string> rSqlCommand;
	request.getRequestData( &this->m_stream );

	DK_SocketParam req;
	DK_SocketParam resp;
	if( this->recvRequest( req ) != 0 )
	{
		this->m_stream.close();
		return -1;
	}

	const char * pMethod = req.getValue( "RemoteFnName" );

	if( strcmp( pMethod, "GetTSATime" ) == 0 )
	{
		time_t t = time( NULL );
		struct tm * pTm = localtime( &t );
		char szCurTime[32];
		sprintf( szCurTime, "%4d/%02d/%02d %02d:%02d:%02d", pTm->tm_year + 1900,
		         pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec );
		resp.addValue( "CurrentTime", szCurTime );
		bOk = true;
	}
	else if( strcmp( pMethod, "GetTSACert" ) == 0 )
	{
		std::string certData;
		this->m_engine.getCertData( certData );//证书已加密
		resp.addValue( "CertData", certData.c_str() );
		bOk = true;
	}
	else if( strcmp( pMethod, "IssuerTimeStamp" ) == 0 )
	{
		std::string sql;
		std::string respData;
		std::string statusInfo;
		std::string serialNumber;
		std::string timeStampData;
		const char * pReqData = req.getValue( "SrcData" );  //取加密原文数据，等待制作时间戳
		if( this->m_engine.createTimeStamp( pReqData, serialNumber, timeStampData,
		                                    respData ) == 0 )  //失败返回-1，respData为已制作完成并加密的时间戳数据
		{
			bOk = true;
			resp.addValue( "ResponseData", respData.c_str() );
			statusInfo = "时间戳签发成功！";
		}
		else
		{
			bOk = false;
			resp.addValue( ERROR_INFO_NAME, "颁发时间戳出错！" );
			statusInfo = "签发时间戳出错，请求数据无效！";
		}
		if(m_bUsingDb)
		{
			sql = "insert into t_time_stamp( f_entry_time, f_serial_number, f_data ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( serialNumber.c_str() );
			sql.append( "', '" );
			sql.append( timeStampData.c_str() );
			sql.append( "' );" );
			rSqlCommand.push_back( sql );

			sql = "insert into t_request_log( f_log_time, f_request_data, f_response_data, f_status ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( pReqData );
			sql.append( "', '" );
			sql.append( respData.c_str() );
			sql.append( "', '" );
			sql.append( statusInfo.c_str() );
			sql.append( "' );" );
			rSqlCommand.push_back( sql );

			sql = "insert into t_visit_log( f_log_time, f_client_ip, f_operater ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( this->m_stream.getHost() );
			sql.append( "', '请求颁发时间戳！' );" );
			rSqlCommand.push_back( sql );
		}
	}
	else if( strcmp( pMethod, "TSReq" ) == 0 )
	{
		bOk = true;
		const char * pSrcData = req.getValue( "SrcData" );

		char byTSReq[10240] = {0};
		int nTSReqLen = 0;
		int Ret = this->m_engine.CreateTSReq( (unsigned char*)const_cast<char*> (pSrcData), strlen(pSrcData),
		                                      (unsigned char* )byTSReq, &nTSReqLen );
		if (0 != Ret)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", Ret);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		char szTSReqB64[10240];
		memset(szTSReqB64, 0, sizeof(szTSReqB64));
		int nTSReqB64Len;
		nTSReqB64Len = CMailCoder::base64_encode( byTSReq, nTSReqLen, szTSReqB64);
		if (nTSReqB64Len <= 0)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", TS_B64EnErr);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		resp.addValue( "TSReqData", szTSReqB64 );

	}
	else if( strcmp( pMethod, "TSResp" ) == 0 )
	{
		bOk = true;
		time_t curTime;
		time(&curTime);

		if(curTime < m_engine.m_CertBeginValid)
		{
			bOk = false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", TS_BEFOREVALID);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		if(curTime > m_engine.m_CertEndValid)
		{
			bOk = false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", TS_AFTERVALID);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		const char * pReqData = req.getValue( "ReqData" );
		unsigned char byTSReq[10240]= { 0 };
		int nTSReqLen = CMailCoder::base64_decode( const_cast<char* >(pReqData), strlen(pReqData), (char *)byTSReq);
		if (nTSReqLen <= 0)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", TS_B64DeErr);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		// ts response
		std::string serialNumber;
		unsigned char byTSResp[10240];
		unsigned char byTSToken[10240];
		memset(byTSResp, 0, sizeof(byTSResp));
		memset(byTSToken, 0, sizeof(byTSToken));
		int nTSRespLen = 0, nTSTokenLen = 0;

		pthread_mutex_lock( m_plock );

		int Ret = this->m_engine.CreateTSResp( byTSReq, nTSReqLen, NULL, NULL,
		                                       m_engine.GetSerCer(), m_engine.GetSerPriKey(), byTSResp, &nTSRespLen, byTSToken, &nTSTokenLen, serialNumber);

		pthread_mutex_unlock( m_plock );

		if (0 != Ret)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", Ret);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}
		char szTSTokenB64[10240] = { 0 };
		int nTSTokenB64Len = CMailCoder::base64_encode((char *)byTSToken, nTSTokenLen, szTSTokenB64);
		if (nTSTokenB64Len <= 0)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", TS_B64EnErr);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}
		resp.addValue( "TSRespData", szTSTokenB64 );

		if(m_bUsingDb)
		{
			std::string sql;
			std::string respData;
			std::string statusInfo("时间戳签发成功");
			sql = "insert into t_time_stamp( f_entry_time, f_serial_number, f_data ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( serialNumber.c_str() );
			sql.append( "', '" );
			sql.append( szTSTokenB64 );
			sql.append( "' );" );
			rSqlCommand.push_back( sql );

			sql = "insert into t_request_log( f_log_time, f_request_data, f_response_data, f_status ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( pReqData );
			sql.append( "', '" );
			sql.append( szTSTokenB64 );
			sql.append( "', '" );
			sql.append( statusInfo.c_str() );
			sql.append( "' );" );
			rSqlCommand.push_back( sql );

			sql = "insert into t_visit_log( f_log_time, f_client_ip, f_operater ) values( '";
			sql.append( szVisitTime );
			sql.append( "', '" );
			sql.append( this->m_stream.getHost() );
			sql.append( "', '请求颁发时间戳！' );" );
			rSqlCommand.push_back( sql );
		}

	}
	else if( strcmp( pMethod, "TSVerify" ) == 0 )
	{
		bOk = true;
		const char * pSrcData = req.getValue( "SrcData" );
		const char * pRespData = req.getValue( "RespData" );

		// ts token
		unsigned char byTSToken[10240];
		memset(byTSToken, 0, sizeof(byTSToken));
		int nTSTokenLen;
		nTSTokenLen = CMailCoder::base64_decode( const_cast<char*>(pRespData), strlen( pRespData ), (char *)byTSToken );
		if (nTSTokenLen <= 0)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", TS_B64DeErr);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		// verify ts token
		int Ret = this->m_engine.VerifyTSToken( (unsigned char*) const_cast<char*>(pSrcData), strlen( pSrcData),
		                                        m_engine.GetSerCertRoot(), m_engine.GetSerCertCa(), byTSToken, nTSTokenLen );

		if (0 != Ret)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", Ret);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		resp.addValue( "TSVerifyData", "0" );


	}
	else if( strcmp( pMethod, "TSParse" ) == 0 )
	{

		bOk = true;
		const char * pRespData = req.getValue( "RespData" );
		const char * szFlag = req.getValue( "Flag" );
		unsigned char byTSToken[10240];
		int nTSTokenLen;
		memset(byTSToken, 0, sizeof(byTSToken));
		int flag = atoi( szFlag );
		nTSTokenLen = CMailCoder::base64_decode(const_cast<char*>(pRespData), strlen(pRespData), (char *)byTSToken);
		if (nTSTokenLen <= 0)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", TS_B64DeErr);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}

		// parse ts token
		char szRes[4096];
		memset(szRes, 0, sizeof(szRes));
		int Ret = this->m_engine.ParseTSToken(byTSToken, nTSTokenLen, szRes, flag);
		if (0 != Ret)
		{
			bOk= false;
			char szErr[50] = {0};
			sprintf(szErr, "%d", Ret);
			resp.addValue( ERROR_INFO_CODE, szErr );
			goto end;
		}
		resp.addValue( "TSParseData", szRes );

	}
	else
	{
		resp.addValue( ERROR_INFO_NAME, "不支持的方法！" );
		bOk = false;
	}

end:

	this->sendResponse( resp, bOk );
	this->m_stream.close();

	if( this->m_bUsingDb )
	{
		for( int i = 0; i < rSqlCommand.size(); i++ )
			ExecuteSQLCommand( rSqlCommand[i].c_str() );
	}
	return 0;
}

// = Send response data.
int CTSASvrServer::sendResponse( const DK_SocketParam & resp, bool bOk )
{
	char szBuff[256];
	long lContentLen = strlen( resp.m_pContent ? resp.m_pContent : "" );
	sprintf( szBuff, "HTTP/1.1 %s\r\nContent-Length: %ld\r\n\r\n", bOk ? "200 OK" : "500 Error",
	         lContentLen );
	//printf("szBuff=%s\n",szBuff);
	if( this->m_stream.send_n( szBuff, strlen( szBuff ) ) <= 0 )
		return -1;
	if( lContentLen > 0 && this->m_stream.send_n( resp.m_pContent, lContentLen ) <= 0 )
		return -1;
	return 0;
}

// = Recv request data.
int CTSASvrServer::recvRequest( DK_SocketParam & req )
{
	char szBuff[2048] = { 0 };
	long lRecvSize = this->m_stream.recv( szBuff, 2047 );
//	printf("buff:%s\n",szBuff);
//	printf("lRecvSize:%d\n",lRecvSize);

	if( lRecvSize <= 0 )
		return -1;
	szBuff[lRecvSize] = '\0';
	char * pBegin = strstr( szBuff, "Content-Length: " );
	if( pBegin == NULL )
	{
		return -1;
	}
	pBegin += strlen( "Content-Length: " );
	char * pEnd = strstr( pBegin, "\r\n\r\n" );
	if( pEnd == NULL )
	{
		return -1;
	}
	*pEnd = '\0';
	long lContentLen = atol( pBegin );
	*pEnd = '\r';

	if( lContentLen == 0 )
		return 0;
	if( req.m_pContent )
		delete [] req.m_pContent;
	req.m_pContent = new char[lContentLen + 1];
	req.m_pContent[lContentLen] = '\0';

	pBegin = pEnd + strlen( "\r\n\r\n" );
	long lHadRecvLen = lRecvSize - ( pBegin - szBuff );
	if( lHadRecvLen > 0 )
		memcpy( req.m_pContent, pBegin, lHadRecvLen );
	if( lHadRecvLen < lContentLen &&
	        this->m_stream.recv_n( req.m_pContent + lHadRecvLen, lContentLen - lHadRecvLen ) <= 0 )
	{
		return -1;
	}
	return 0;
}


// =================================================================================
//  1.00    2006-11-28    Created By LinZaoGang
//  File End
