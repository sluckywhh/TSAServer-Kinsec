#include "SmtpSocket.h"


CSMTPSocket::CSMTPSocket():m_hSocket(0)
{
	
}

CSMTPSocket::~CSMTPSocket()
{
	
}

bool CSMTPSocket::Connect(Socket_S *pHeader)
{
	struct sockaddr_in ServerAddress;
		
    m_hSocket = socket( AF_INET,SOCK_STREAM,0 );
	if( m_hSocket == INVALID_SOCKET )  // Can't make a socket
	{		
		return FALSE;
	}
    
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(pHeader->szServer);
    ServerAddress.sin_port = htons( (short)pHeader->nPort );
    if (ServerAddress.sin_addr.s_addr == INADDR_NONE)
    {
    	struct hostent* pHostent = gethostbyname(pHeader->szServer);
        if (pHostent != NULL)
        {
			ServerAddress.sin_addr.s_addr = *((unsigned long *)pHostent->h_addr_list[0]);
		}
    }
	if(( connect( m_hSocket,(SA*)&ServerAddress,sizeof( ServerAddress ))) == SOCKET_ERROR )
	{
		CloseSocket();
		m_hSocket = INVALID_SOCKET;
		return FALSE;
	}
	return true;
}

bool CSMTPSocket::Send(LPSTR pszBuf, int nBuf)
{
  return (send(m_hSocket, pszBuf, nBuf, 0) != SOCKET_ERROR);
}

int CSMTPSocket::Receive(LPSTR pszBuf, int nBytes)
{
	int		nBytesRead;
    /*fd_set	fsSet;
	int		nResult;
	struct	timeval tvTimeOut;
  
	FD_ZERO( &fsSet );
    FD_SET( m_hSocket,&fsSet );
	tvTimeOut.tv_sec = 30;
	tvTimeOut.tv_usec = 0;

    nResult = select( m_hSocket+1,&fsSet,NULL,NULL,&tvTimeOut );
	switch( nResult )
	{
	case SOCKET_ERROR:
		return SOCKET_ERROR;

	case SOCKET_TIMEOUT:
		return SOCKET_TIMEOUT;

	default:
        if(( nBytesRead = recv( m_hSocket,pszBuf,nBytes,0 )) == SOCKET_ERROR )
        {
          	printf("m_socket_Receive = %p\n", m_hSocket);
		     return SOCKET_ERROR; 
		}
	    return nBytesRead; 
	}*/
	return recv( m_hSocket,pszBuf,nBytes,0 );
}

void CSMTPSocket::CloseSocket()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		shutdown(m_hSocket, 2);
		close(m_hSocket);
	}
}

bool CSMTPSocket::IsReadable(bool& bReadible)
{
  timeval timeout = {60, 0};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(m_hSocket, &fds);
  int nStatus = select(m_hSocket+1, &fds, NULL, NULL, &timeout);
  if (nStatus == SOCKET_ERROR)
  {
  	printf("select error\n");
    return FALSE;
  }
  else
  {
    bReadible = !(nStatus == 0);
    return TRUE;
  }
}


