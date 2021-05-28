#include "SmtpSocket.h"
#include "SmtpMessage.h"
#include "Base64Code.h"
#ifndef LPCSTR
   #define LPCSTR const char*
#endif
#define MAX_LINE 260

#ifndef STRING
   #define STRING std::string 
#endif
class CSMTPConnection
{
	public:
	     CSMTPConnection(STRING& strMailServ, STRING& strUserName, STRING& strUserPasswd);
	     ~CSMTPConnection();
	     bool Connect();  //m_bConnected
	     bool SendMessage(CSMTPMessage& Message); //m_SendEmailAddress	     
	     bool Disconnect();	     
	private:
	     bool ConnectESMTP(LPCSTR pszLocalName, LPCSTR pszUsername, LPCSTR pszPassword);
	     bool AuthLogin(LPCSTR pszUsername, LPCSTR pszPassword);
	     bool ConnectSMTP(LPCSTR pszLocalAdress);
	     bool SendRCPTForRecipient(CSMTPMessage& Message);
	     bool ReadCommandResponse(int nExpectedCode);	     
         bool ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, int nExpectedCode, LPSTR* ppszOverFlowBuffer, int nGrowBy);
		 bool ResponseParamCmpEqual( const std::string strParam, const std::string strCmp, bool bIgnorecase = 0 );
	private:
	     CSMTPSocket m_SMTP;
	     Socket_S m_st_socket;
	     bool m_bConnected;
	     std::string m_sHeloAddress;
	     std::string m_UserName;
	     std::string m_PassWord;
	public:
	     std::string m_sLastCommandResponse;
};

