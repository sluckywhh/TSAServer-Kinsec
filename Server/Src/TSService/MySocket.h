// MySocket.h : used to compatible with windows socket.
#ifndef __DID_MYSOCKET__
#define __DID_MYSOCKET__

#ifdef _LINUX

typedef int SOCKET; 

#define INVALID_SOCKET	(SOCKET)(~0)
#define SOCKET_ERROR	(-1)
#ifndef INADDR_NONE
#define INADDR_NONE		0xffffffff
#endif

/* Microsoft Windows Extended data types */
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr *PSOCKADDR;

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr_in *PSOCKADDR_IN;

typedef struct linger LINGER;
typedef struct linger *PLINGER;

typedef struct in_addr IN_ADDR;
typedef struct in_addr *PIN_ADDR;

typedef fd_set FD_SET;
typedef fd_set *PFD_SET;

typedef struct hostent HOSTENT;
typedef struct hostent *PHOSTENT;

typedef struct servent SERVENT;
typedef struct servent *PSERVENT;

typedef struct protoent PROTOENT;
typedef struct protoent *PPROTOENT;

typedef struct timeval TIMEVAL;
typedef struct timeval *PTIMEVAL;

/* #define s_addr  S_un.S_addr			/ can be used for most tcp & ip code */
extern int errno;
#define WSAGetLastError()  (errno)
#define GetLastError()  (errno)
#define closesocket(a)  close(a)

/* define the WAS*** for windows compatible */
#define WSAEWOULDBLOCK             EWOULDBLOCK
#define WSAEINPROGRESS             EINPROGRESS
#define WSAEALREADY                EALREADY
#define WSAENOTSOCK                ENOTSOCK
#define WSAEDESTADDRREQ            EDESTADDRREQ
#define WSAEMSGSIZE                EMSGSIZE
#define WSAEPROTOTYPE              EPROTOTYPE
#define WSAENOPROTOOPT             ENOPROTOOPT
#define WSAEPROTONOSUPPORT         EPROTONOSUPPORT
#define WSAESOCKTNOSUPPORT         ESOCKTNOSUPPORT
#define WSAEOPNOTSUPP              EOPNOTSUPP
#define WSAEPFNOSUPPORT            EPFNOSUPPORT
#define WSAEAFNOSUPPORT            EAFNOSUPPORT
#define WSAEADDRINUSE              EADDRINUSE
#define WSAEADDRNOTAVAIL           EADDRNOTAVAIL
#define WSAENETDOWN                ENETDOWN
#define WSAENETUNREACH             ENETUNREACH
#define WSAENETRESET               ENETRESET
#define WSAECONNABORTED            ECONNABORTED
#define WSAECONNRESET              ECONNRESET
#define WSAENOBUFS                 ENOBUFS
#define WSAEISCONN                 EISCONN
#define WSAENOTCONN                ENOTCONN
#define WSAESHUTDOWN               ESHUTDOWN
#define WSAETOOMANYREFS            ETOOMANYREFS
#define WSAETIMEDOUT               ETIMEDOUT
#define WSAECONNREFUSED            ECONNREFUSED
#define WSAELOOP                   ELOOP
#define WSAENAMETOOLONG            ENAMETOOLONG
#define WSAEHOSTDOWN               EHOSTDOWN
#define WSAEHOSTUNREACH            EHOSTUNREACH
#define WSAENOTEMPTY               ENOTEMPTY
#define WSAEPROCLIM                EPROCLIM
#define WSAEUSERS                  EUSERS
#define WSAEDQUOT                  EDQUOT
#define WSAESTALE                  ESTALE
#define WSAEREMOTE                 EREMOTE

/* added by hand */
#define WSAEINTR	EINTR
#define WSAEBADF	EBADF
#define WSAEACCES	EACCES
#define WSAEFAULT	EFAULT
#define WSAEINVAL	EINVAL
#define WSAEMFILE	EMFILE

#endif

#endif
