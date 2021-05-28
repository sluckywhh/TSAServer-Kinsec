#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "stdio.h"
#include <netdb.h>

#define INVALID_SOCKET  (int)(~0)
#define SOCKET_ERROR -1
#define SOCKET_TIMEOUT 0
#define	SA	struct sockaddr 
#ifndef LPSTR
   #define LPSTR char*
#endif
#define FALSE false
#define TRUE  true
typedef struct Socket_Address_port
{
	char	szServer[128];
	int		nPort;
} Socket_S;

class CSMTPSocket 
{
  public:
	   CSMTPSocket();
	   ~CSMTPSocket();
	   bool Connect(Socket_S *pHeader);
	   bool Send(LPSTR pszBuf, int nBuf);
	   int Receive(LPSTR pszBuf, int nBytes);  
	   void CloseSocket(); 
	   bool IsReadable(bool& bReadible);
  public:
       int m_hSocket;
};
