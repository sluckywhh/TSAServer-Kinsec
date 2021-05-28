#include "SmtpConnect.h"
#define STRING std::string 

CSMTPConnection::CSMTPConnection(STRING& strMailServ, STRING& strUserName, STRING& strUserPasswd)
//CSMTPConnection::CSMTPConnection()
{
	strcpy(m_st_socket.szServer, strMailServ.c_str());
	m_st_socket.nPort = 25;
	m_bConnected = FALSE;	
	//m_sHeloAddress = "";
	m_sHeloAddress = strMailServ;	//modified yyx 2010-06-25
	m_UserName = strUserName;
	m_PassWord = strUserPasswd;
/*	strcpy(m_st_socket.szServer, "192.168.1.100");
	m_st_socket.nPort = 25;
	m_bConnected = FALSE;	
	m_sHeloAddress = "";
	m_UserName = "weiming";
	m_PassWord = "111111";*/
}

CSMTPConnection::~CSMTPConnection()
{
	
}

bool CSMTPConnection::Connect()
{
  if (!m_SMTP.Connect(&m_st_socket))
  {
	return FALSE;
  }
  else
  {
    //连接成功
    m_bConnected = TRUE;
    if (!ReadCommandResponse(220))
    {
      printf("An unexpected SMTP login response was received\n");
      Disconnect();
      return FALSE;
    }
	// 进行下一步连接，即进行SMTP会话
	 bool bConnectOk = FALSE;
	 
     struct sockaddr_in ClientAddress;
     socklen_t ClientAddressLen;
     getsockname(m_SMTP.m_hSocket, (SA*)&ClientAddress, &ClientAddressLen);
     m_sHeloAddress = inet_ntoa(ClientAddress.sin_addr);
	 bConnectOk = ConnectESMTP(m_sHeloAddress.c_str(), m_UserName.c_str(), m_PassWord.c_str());
     //bConnectOk = ConnectSMTP(m_sHeloAddress.c_str());
	 if (!bConnectOk)
	 {
	 	printf("ConnectSMTP failure\n");
        Disconnect();
        return FALSE;
	 }
     return TRUE;
  }
}

bool CSMTPConnection::ConnectESMTP(LPCSTR pszLocalName, LPCSTR pszUsername, LPCSTR pszPassword)
{
	//发送EHLO命令
	char sBuf[MAX_LINE];
	sprintf(sBuf, "EHLO %s\r\n", pszLocalName);
	
	if (!m_SMTP.Send(sBuf, strlen(sBuf)))
	{
		return FALSE;
	}
	
	//检查Hello命令的响应
	if (!ReadCommandResponse(250))
	{
		printf("EHLO FAILED\n");
		return FALSE;
	}
	bool bLoginOk = FALSE;
    bLoginOk = AuthLogin(pszUsername, pszPassword);
	return bLoginOk;
}


bool CSMTPConnection::AuthLogin(LPCSTR pszUsername, LPCSTR pszPassword)
{
	//发送AUTH LOGIN命令
	std::string sBuf;
	sBuf = "AUTH LOGIN\r\n";
	if (!m_SMTP.Send((char *)sBuf.c_str(), sBuf.size()))
	{
	  printf("An unexpected error occurred while sending the AUTH command\n");
	  return FALSE;
	}

	//初始化
	CBase64Coder Coder;
	if (!ReadCommandResponse(334))
	{
		printf("Server does not support AUTH LOGIN!\n");
		return FALSE;
	}
	else
	{
		//发送base64编码后的用户名
		std::string sBuf_User;
		std::string sLastCommandString;
		sLastCommandString = &m_sLastCommandResponse[4];
		Coder.Decode(sLastCommandString.c_str());
		//if (strcmp(Coder.DecodedMessage(), "username:") == 0)
		if( ResponseParamCmpEqual( Coder.DecodedMessage(), "username:", 1 ) )
		{
			Coder.Encode(pszUsername);
			sBuf_User = Coder.EncodedMessage();
			sBuf_User += "\r\n";
//			Coder.Decode(sBuf_User.c_str());
			if (!m_SMTP.Send((char*)sBuf_User.c_str(), sBuf_User.size()))
//			if (!m_SMTP.Send((char*)pszUsername, strlen(pszUsername) ))
			{
			  printf("An unexpected error occurred while sending the username\n");
			  return FALSE;
			}
		}
		else
		{
			printf("An unexpected request received when expecting username request");
			return FALSE;
		}
	}

	//检查返回用户名返回码 
	if (!ReadCommandResponse(334))
    {
	   printf("Server did not response correctly to AUTH LOGIN username field!\n");
	   return FALSE;
    }
	else
	{
		//发送密码到服务器
		std::string sLastCommandString;
		sLastCommandString = &m_sLastCommandResponse[4];
		Coder.Decode(sLastCommandString.c_str());
		//if (strcmp(Coder.DecodedMessage(), "Password:") == 0)
		if ( ResponseParamCmpEqual( Coder.DecodedMessage(), "password:", 1 ) )
		{
			std::string sBuf_passwd;
			Coder.Encode(pszPassword);
			sBuf_passwd = Coder.EncodedMessage();
			sBuf_passwd += "\r\n";
			if (!m_SMTP.Send((char*)sBuf_passwd.c_str(), sBuf_passwd.size()))
//			if (!m_SMTP.Send((char*)pszPassword, strlen(pszPassword) ))
			{
			  printf("An unexpected error occurred while sending the password\n");
			  return FALSE;
			}

			//检查是否成功
			if (!ReadCommandResponse(235))
			{
				printf("AUTH LOGIN authentication was unsuccessful\n");
				return FALSE;
			}
		}
		else
		{
			printf("An unexpected request received when expecting password request\n");
			return FALSE;
		}
	}

	return TRUE;
}

bool CSMTPConnection::ConnectSMTP(LPCSTR pszLocalAdress)
{
   //Send the HELO command
	char Helo_Buf[MAX_LINE];
	sprintf(Helo_Buf, "HELO %s\r\n", pszLocalAdress);
	int nCmdLength = strlen(Helo_Buf);
	if (!m_SMTP.Send(Helo_Buf, nCmdLength))
	{ 
	  return FALSE;
	}
   //check the response to the HELO command
	return ReadCommandResponse(250);
}

bool CSMTPConnection::SendRCPTForRecipient(CSMTPMessage& Message)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
  char RCPT_buf[MAX_LINE];
  sprintf(RCPT_buf, "RCPT TO:<%s>\r\n", Message.m_ReceiveEmailAddress.c_str());

  int nCmdLength = strlen(RCPT_buf);

  if (!m_SMTP.Send(RCPT_buf, nCmdLength))
  {
    printf("%s\n", "Failed in call to send RCPT command, GetLastError returns: %d\n");
    return FALSE;
  }

  //check the response to the RCPT command
  if (!ReadCommandResponse(250))
  {
    printf("%s\n", "An unexpected RCPT response was received\n");
    return FALSE;
  } 
  
  return TRUE;
}

bool CSMTPConnection::SendMessage(CSMTPMessage& Message)
{
	//发送Email命令
	char FromAddress[MAX_LINE];
	sprintf(FromAddress, "MAIL FROM:<%s>\r\n", Message.m_SendEmailAddress.c_str());
	int nCmdLength = strlen(FromAddress);

	if (!m_SMTP.Send(FromAddress, nCmdLength))
	{
		printf("%s\n", "Failed in call to send MAIL command, GetLastError returns: %d\n");
		return FALSE;
	}

	//检查服务器响应
	if (!ReadCommandResponse(250))
	{
		printf("%s\n", "An unexpected MAIL response was received\n");
		return FALSE;
	} 
    
	//发送RCPT命令,每一个接受者都需要发送一次,首先"TO"发送明接收者
	/*
	抄送：所谓抄送（CC）指的是该电子邮件除了向发件人地址发送出去以外，还可以向其他一些电子邮件地址发送出去。
    暗送：暗送（BCC）的功能跟抄送基本相似，只不过在电子邮件发送到对方的时候，对方只知道这一封电子邮件发送给了自己，而至于这一封电子邮件还向其他哪些人发送了，就不得而知了
	*/	
	if (!SendRCPTForRecipient(Message))
	{
		return FALSE;
    }

	//发送DATA命令
	char* pszDataCommand = "DATA\r\n";
	nCmdLength = strlen(pszDataCommand);
	if (!m_SMTP.Send(pszDataCommand, nCmdLength))
	{
		printf("%s", "Failed in call to send MAIL command, GetLastError returns: %d\n");
		return FALSE;
	}

	//检查DATA命令的响应
	if (!ReadCommandResponse(354))
	{
		printf("%s", "An unexpected DATA response was received\n");
		return FALSE;
	} 

	//发送信息头
	std::string szHeader = Message.GetHeader();
	nCmdLength = szHeader.size();
	if (!m_SMTP.Send((char*)szHeader.c_str(), nCmdLength))
	{
	    printf("Failed in call to send the header\n");
		return FALSE;
	}
    
	//Send the Header for the body (Note in the case of non mime emails, it is just a simple line separaror
	char* pszBodyHeader = "\r\n";
	nCmdLength = strlen(pszBodyHeader);
	if (!m_SMTP.Send(pszBodyHeader, nCmdLength))
	{
		printf("Failed in call to send the body header\n");
		return FALSE;
	}

    VECTORBYTE s_body;
	if (Message.GetBody(s_body, TRUE))
	{
	   s_body[s_body.size()] = 0;
	   //发送信体
	   if (!m_SMTP.Send((char*)&s_body[0], s_body.size()))
	   {
	       printf("Failed in call to send the body\n");
	       return FALSE;
	   }
    }
    else{
        printf("Failed in call to getbody\n");
        return FALSE;
    }
	//发送邮件结束指示符
	char* pszEOM = "\r\n.\r\n";
	nCmdLength = strlen(pszEOM);
	if (!m_SMTP.Send(pszEOM, nCmdLength))
	{
		printf("Failed in call to send end of message indicator");
		return FALSE;
	}

	//检查结束符响应
	if (!ReadCommandResponse(250))
	{
		printf("An unexpected end of message response was received\n");
		return FALSE;
	} 
	return TRUE;
}

bool CSMTPConnection::Disconnect()
{
  bool bSuccess = FALSE;      

  //disconnect from the SMTP server if connected 
  if (m_bConnected)
  {
    char sBuf[10];
    strcpy(sBuf, "QUIT\r\n");
    int nCmdLength = strlen(sBuf);
    if (!m_SMTP.Send(sBuf, nCmdLength))
      printf("Failed in call to send QUIT comman\n");

    //Check the reponse
    bool bSuccess = ReadCommandResponse(221);
    if (!bSuccess)
      printf("An unexpected QUIT response was received\n"); 
    m_bConnected = FALSE;
  }
  else
    printf("%s\n","Already disconnected from SMTP server, doing nothing\n");
 
  //free up our socket
  m_SMTP.CloseSocket();
 
  return bSuccess;
}

bool CSMTPConnection::ReadCommandResponse(int nExpectedCode)
{
  LPSTR pszOverFlowBuffer = NULL;
  char sBuf[256];
  bool bSuccess = ReadResponse(sBuf, 256, "\r\n", nExpectedCode, &pszOverFlowBuffer,256);
  if (pszOverFlowBuffer)
    delete [] pszOverFlowBuffer;

  return bSuccess;
}

bool CSMTPConnection::ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, int nExpectedCode, LPSTR* ppszOverFlowBuffer, int nGrowBy)
{

	//获取结束符的长度
	int nTerminatorLen = strlen(pszTerminator);

	//接收获得的数据
	LPSTR pszRecvBuffer = pszBuffer;
	int nBufSize = nInitialBufSize;

	//循环获得响应知道遇到结束符或者超时
	bool bFoundTerminator = FALSE;
	int nReceived = 0;
	while (!bFoundTerminator)
	{

		//检查socket是否可读,60秒超时
		bool bReadible;
		if (!m_SMTP.IsReadable(bReadible))
		{
			printf("m_hsocket is't readable\n");
			pszRecvBuffer[nReceived] = '\0';
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return FALSE;
		}
		else if (!bReadible) //没有数据接受到
		{
			printf("m_hsocket is't readable(breadible = false)\n");
			return FALSE;
		}
        
		//从socket接收数据
		int nBufRemaining = nBufSize-nReceived-1; //Allows allow one space for the NULL terminator
		if (nBufRemaining<0)
			nBufRemaining = 0;
		int nData = m_SMTP.Receive(pszRecvBuffer+nReceived, nBufRemaining);
		
        
		//如果接收数据时错误产生
		if (nData > 0)
		{
			//增加获得的数据
			nReceived += nData;						   
		}
		if (nData < 1)
		{
			//以空字符结束字符串
			if (pszRecvBuffer)
			pszBuffer[nReceived] = '\0';
			m_sLastCommandResponse = pszRecvBuffer; //分离出最后一个响应
			return FALSE; 
		}
		else
		{
			//以空字符结束字符串
			if (pszRecvBuffer)
			  pszRecvBuffer[nReceived] = '\0';
			if (nBufRemaining-nData == 0) //检查是否还有空间
			{
				//申请新的缓冲区
				nBufSize += nGrowBy; //增加缓冲区
				LPSTR pszNewBuf = new char[nBufSize];

				//将原来的内容复制到新的缓冲区中，并用新的缓冲区接收数据
				if (pszRecvBuffer)
				  strcpy(pszNewBuf, pszRecvBuffer);
				pszRecvBuffer = pszNewBuf;

				//删除原来缓冲区
				if (*ppszOverFlowBuffer)
				  delete [] *ppszOverFlowBuffer;

				//将新的缓冲区内容存入溢出缓冲区
				*ppszOverFlowBuffer = pszNewBuf;        
			}
		}

		// 检查结束符是否被找到
		// 检查是否到了响应结束，否则提前停止循环读取数据，如果有可能会有多行响应
		bFoundTerminator = (strncmp( &pszRecvBuffer[ nReceived - nTerminatorLen ], pszTerminator, nTerminatorLen ) == 0);
	}

	//去掉结束符
	pszRecvBuffer[ nReceived - nTerminatorLen ] = '\0';
//	printf("pszRecvBuffer_endterminate: %s\n", pszRecvBuffer);
	//处理多行响应
	bool bSuccess = FALSE;
	do
	{
		// 检查响应是否是错误码
		char sCode[4];
		strncpy(sCode, pszRecvBuffer, 3 );
		sCode[3] = '\0';		
		bSuccess = (atoi(sCode) == nExpectedCode);
     //   if (bSuccess)  printf("ret_scode = %s\n", sCode);  
		// 分离上一次响应
		m_sLastCommandResponse = pszRecvBuffer;

		// 检查是否有多行响应
		pszRecvBuffer = strstr( pszRecvBuffer, pszTerminator );
		if (pszRecvBuffer)
			pszRecvBuffer += nTerminatorLen;	// 跳过结束符

	} while ( !bSuccess && pszRecvBuffer );

	return bSuccess;
}

//added yyx 2010-06-25
bool CSMTPConnection::ResponseParamCmpEqual( const std::string strParam, const std::string strCmp, bool bIgnorecase )
{
	std::string strTemp = strParam;
	if( bIgnorecase )
	{
		for ( int i=0; i < strTemp.length(); i++ )
		{
			if( strTemp[i] != strCmp[i] )
			{
				if( strTemp[i] > strCmp[i] && strTemp[i] != (strCmp[i] + 32) )
					return 0;
				else if( (strTemp[i] + 32) != strCmp[i] )
					return 0;
			}
		}
		
		return 1;
	}

	return strTemp.compare( strCmp ) ? 0 : 1;
}
