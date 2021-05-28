#include "SmtpMessage.h"
CSMTPMessage::CSMTPMessage(STRING& strSendAddress, STRING& strReceiveAddress,STRING& strMailSubject,STRING strMailText)
//CSMTPMessage::CSMTPMessage()
{
	//m_SendEmailAddress = "weiming@kinsec.com.cn";
	//m_ReceiveEmailAddress = "weiming@kinsec.com.cn";
	//m_SendEmailAddress = "jackme99@sina.com.cn";
	//m_ReceiveEmailAddress = "jackme99@sina.com.cn";	
	//m_sSubject = "LINUX测试邮件";
	//m_sText = "LINUX测试邮件内容摘要我的第一封邮件来我的第一;;;;;;;;\n封邮件来我的第一封邮件来我的\n第一封邮件来";
	m_SendEmailAddress = strSendAddress;
	m_ReceiveEmailAddress = strReceiveAddress;
	m_sSubject = strMailSubject;
	m_sText = strMailText;
	m_bMime = TRUE;
}

CSMTPMessage::~CSMTPMessage()
{
	
}

std::string CSMTPMessage::GetHeader()
{
	
  VECTORBYTE v_SendMail, v_RecevieMail, v_Subject, v_Date, v_Xmial;
  HeaderEncode(m_SendEmailAddress.c_str(), v_SendMail);
  v_SendMail[v_SendMail.size()] = 0;
  HeaderEncode(m_ReceiveEmailAddress.c_str(), v_RecevieMail);
  v_RecevieMail[v_RecevieMail.size()] = 0;
  HeaderEncode(m_sSubject.c_str(), v_Subject);
  v_Subject[v_Subject.size()] = 0;

  std::string strReply;
  
  if (v_SendMail.size())  strReply += "FROM: ";  strReply += (char*)&v_SendMail[0];  strReply += "\r\n";
  if (v_RecevieMail.size())  strReply += "To: ";  strReply += (char*)&v_RecevieMail[0];  strReply += "\r\n";
  if (v_Subject.size())  strReply += "Subject: ";  strReply += (char*)&v_Subject[0];  strReply += "\r\n";
  if (m_bMime)  
  { 
  	strReply += "MIME-Version: 1.0\r\n";
  	//strReply += "Content-Type: text/plain; charset: iso-8859-1\r\n";
	strReply += "Content-Type: text/plain; charset: GB2312\r\n";		//modified yyx 2010-07-08
  	strReply += "Content-Transfer-Encoding: quoted-printable\r\n";
  }
  return strReply;
}

bool CSMTPMessage::_replace(std::string& oldstr, char* changestr, char* newstr, std::vector<char>& retvector)
{
	 int nlen = strlen(changestr);
     int i_find = -1;
     int j = 0, k = 0;
	 do
	 {
	     i_find = oldstr.find(changestr);
	     if (i_find != -1)
	     {
	        for(j = 0; j<i_find; j++)
	        {
	           retvector.push_back(oldstr[j]);
	        }
	        char* pnewstr = newstr;
            while(*pnewstr != '\0')
            {
            	retvector.push_back(*pnewstr);
            	pnewstr++;
            }
	        oldstr = &oldstr[i_find+nlen];
	     }
	     else{
	        for(k = 0; k<oldstr.size(); k++)
	           retvector.push_back(oldstr[k]);
	     }
	 }while(i_find != -1);
	 retvector[retvector.size()] = 0;
     return true;
}

bool CSMTPMessage::GetBody(VECTORBYTE &sOut, bool bQuotedPrintable)
{
	bool bSuccess = FALSE;
	//如果是文本内容
	if (bQuotedPrintable)
	{
		//形成发送体
		std::vector<char> Replace_str;
		_replace(m_sText, "\r\n", "\n", Replace_str);
		m_sText = &Replace_str[0];
		Replace_str.clear();
		_replace(m_sText, "\r", "\n", Replace_str);
		m_sText = &Replace_str[0];
		Replace_str.clear();
		_replace(m_sText, "\n", "\r\n", Replace_str);
		m_sText = &Replace_str[0];
		Replace_str.clear();
		_replace(m_sText, "\n.", ".", Replace_str);
		m_sText = &Replace_str[0];
		Replace_str.clear();				
		QuotedPrintableEncode(m_sText.c_str(), sOut); //Q编码
	}
	else
	{
		LPCSTR p = m_sText.c_str();
		while( *p!= '\0')
		{
			sOut.push_back(*p);
			p++;
		}
	}
	bSuccess = TRUE;
	return bSuccess;
}

char CSMTPMessage::HexDigit(int nDigit)
{
  if (nDigit < 10)
    return (char) (nDigit + '0');
  else
    return (char) (nDigit - 10 + 'A');
}

//Converts header text to its encoded form according to RFC 2047
bool CSMTPMessage::HeaderEncode(LPCSTR sText,  VECTORBYTE &sOut)
{
  int nSize = strlen(sText);
  int i = 0;
  //Determine if a translation is needed
  bool bTranslationNeeded = FALSE;
  for (i=0; i<nSize && !bTranslationNeeded; i++)
  {
    BYTE c = (BYTE) sText[i];
    bTranslationNeeded = (c > 127);
    
  }
  if (bTranslationNeeded)
  {
  	char *charset = new char[256];
    //sprintf(charset, "=?%s?q?" , "iso-8859-1");
	sprintf(charset, "=?%s?q?" , "GB2312");		//modified yyx 2010-07-08
    char *ptmp_charset = charset;
    while(*charset != '\0')
    {
      sOut.push_back(*charset);
      charset++;
    }
    delete []ptmp_charset;
    for (i=0; i<nSize; i++)
    {
      BYTE c = (BYTE) sText[i];
      if (c == ' ') // A space
        sOut.push_back('_');
      else if ((c > 127) || (c == '=') || (c == '?') || (c == '_'))
      {
        //Must Quote the text
        sOut.push_back('=');
        sOut.push_back(HexDigit((c & 0xF0) >> 4));
        sOut.push_back(HexDigit(c & 0x0F));
      }
      else
        sOut.push_back(c);
    }
    sOut.push_back('?');
    sOut.push_back('=');
  }
  else
  {
    //Just pass the text thro unmodified
    while(*sText != '\0')
    {
      sOut.push_back(*sText);
      sText++;
    }
  }
  return true;
}

//Converts text to its Quoted printable equivalent according to RFC 2045
bool CSMTPMessage::QuotedPrintableEncode(LPCSTR sText, VECTORBYTE &sOut)
{
  std::vector<BYTE> sTemp;
  int nSize = strlen(sText);
  int i = 0;
  for (i=0; i<nSize; i++)
  {
    BYTE c = (BYTE) sText[i];
    if (((c >= 33) && (c <= 60)) || ((c >= 62) && (c <= 126)) || (c == '\r') || (c == '\n') || (c == '\t') || (c == ' '))
      sTemp.push_back(c);
    else
    {
      //otherwise must quote the text
      sTemp.push_back('=');
      sTemp.push_back(HexDigit((c & 0xF0) >> 4));
      sTemp.push_back(HexDigit(c & 0x0F));
    }
  }

  //Now insert soft line breaks where appropiate
  int nStartLine = 0;
  int nLen = sTemp.size();
  for (i=0; i<nLen; i++)
  {
    BYTE c = (BYTE) sTemp[i];
    
    if (c == '\n' || c == '\r' || i == (nLen-1))
    {
      for (int p = 0; p<i-nStartLine+1; p++)
      {
          sOut.push_back(sTemp[nStartLine+p]);
      }
      nStartLine = i+1;
      continue;
    }

    if ((i - nStartLine) > SMTP_MAXLINE)
    {
      bool bInWord = TRUE;
      while (bInWord)
      {
        bInWord = (!isspace(c) && sTemp[i-2] != '=');
        if (bInWord)
        {
          --i;
          c = (BYTE) sTemp[i];
        }

		    if (i == nStartLine)
		    {
			    i = nStartLine + SMTP_MAXLINE;
			    break;
		    }
      }

      for (int q = 0; q<i-nStartLine+1; q++)
      {
          sOut.push_back(sTemp[nStartLine+q]);
      }
      sOut.push_back('=');
      sOut.push_back('\r');
      sOut.push_back('\n');
      nStartLine = i+1;
    }
  }

  return true;
}





