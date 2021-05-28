#include <unistd.h>
#include "stdio.h"
#include "SmtpConnect.h"
#include "XMLConf.h"
#include "Base64Code.h"

std::string GetMailServName(std::string SendMailAddress);
int main(int argc,char* argv[])
{
	pid_t c_pid;
	if( argc != 7 )
	{
		printf( "Command format: sendmail ReceiveAddress SendAddress UserName UserPasswd MailSubject MailContent\r\n" );
		return -1;
	}
	if ((c_pid = fork()) == 0)
	{
		std::string strMailIp, strSendAddress, strReceiveAddress, strUserName, strUserPasswd, strMailSubject,strMailContent;
//	    CBase64Coder Coder;	    
//		Coder.Decode(strUserPasswd.c_str());
//		strUserPasswd = Coder.DecodedMessage();		    
		strReceiveAddress	= argv[1];
		strSendAddress		= argv[2];
		strUserName			= argv[3];
		strUserPasswd		= argv[4];
		strMailSubject		= argv[5];
		strMailContent		= argv[6];

		strMailIp = GetMailServName(strSendAddress);
		CSMTPConnection m_SmtpConnect(strMailIp, strUserName, strUserPasswd);
		CSMTPMessage m_SmtpMessage(strSendAddress, strReceiveAddress, strMailSubject, strMailContent);
		if (!m_SmtpConnect.Connect())
		{
			printf("connect smtpserv to failure!\n");
			exit(-1);
		}
		if (!m_SmtpConnect.SendMessage(m_SmtpMessage))
		{
			printf("send smtpmessage to failure!\n");
			exit(-1);			
		}
		m_SmtpConnect.Disconnect();
printf("send mail finish from SERVICE: %s.\n", strMailSubject.c_str());
	}else{
	    exit(0);
    }
    return 1;
}

std::string GetMailServName(std::string SendMailAddress)
{
	std::string strServName;
	int i =  SendMailAddress.find('@');

	//strServName += "mail.";
	strServName += "smtp.";	//modified yyx 2010-06-25
	strServName += &SendMailAddress[i+1];

	return strServName;
	
}

