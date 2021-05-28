#include "vector"
#include "string"
#include "stdio.h"
#include <stdlib.h>
#include <malloc.h> 
#include "string.h"
#include "string"
#include <ctype.h>
#ifndef __H_SMTPMESSAGE_H__
#define __H_SMTPMESSAGE_H__

#define SMTP_MAXLINE  76
#define MAX_LINE 260
#ifndef LPSTR
   #define LPSTR char*

#endif
#ifndef LPCSTR
   #define LPCSTR const char*
#endif
#define BYTE unsigned char
typedef std::vector<BYTE> VECTORBYTE;
#define FALSE false
#define TRUE true
#endif

#ifndef STRING
   #define STRING std::string 
#endif
class CSMTPMessage
{
	public:
	   CSMTPMessage(STRING& strSendAddress, STRING& strReceiveAddress,STRING& strMailSubject,STRING strMailText);
	   ~CSMTPMessage();
	   std::string GetHeader();
	   bool GetBody(VECTORBYTE &sOut, bool bQuotedPrintable);
	private:
	   bool HeaderEncode(LPCSTR sText,  VECTORBYTE &sOut);
	   bool QuotedPrintableEncode(LPCSTR sText, VECTORBYTE &sOut);
	   bool _replace(std::string& oldstr, char* changestr, char* newstr, std::vector<char>& retvector);
	   char HexDigit(int nDigit);	   
	private:
	   std::string m_sSubject; 
	   std::string m_sText;
	public:
	   std::string m_SendEmailAddress;
	   std::string m_ReceiveEmailAddress;
	   bool m_bMime;
};
