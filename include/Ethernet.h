/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _ETHERNET_H_
#define _ETHERNET_H_

/* system includes */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#ifndef _WIN32
 #include <sys/ioctl.h>
 #include <sys/socket.h>
 #include <arpa/inet.h>
 #include <string.h>
#endif

/* user includes */


#ifdef _WIN32
 #define CLOSE_SOCKET(sock)           (closesocket(sock))
 #define WRITE_SOCKET(sock, buf, len) (send(sock, buf, len, 0))
 #define IOCTL(fd, request, val)      (ioctlsocket(fd, request, &val))
 #define SLEEP(x)                     (Sleep(x*1000))
#else
 #define CLOSE_SOCKET(sock)           (close(sock))
 #define WRITE_SOCKET(sock, buf, len) (write(sock, buf, len, 0))
 #define IOCTL(fd, request, val)      (ioctl(fd, request, val))
 #define SLEEP(x)                     (sleep(x))
#endif
 

/**********************/
/* REMOVE THIS */
#ifndef FALSE
 #define FALSE (0)
#endif

#ifndef TRUE
 #define TRUE (!FALSE)
#endif

#ifndef NULL
 #define NULL (0)
#endif
/**********************/
/* defines */
#define MAX_TCP_READS   (200)
#define SOCKET_SYNC     (0xCABE)


/* The 'C' definitions */
#ifdef __cplusplus
extern "C" {
#endif
    

/* enums */
typedef enum {
    SOCKET_TYPE_INVALID = 0,
    SOCKET_TYPE_UDP_SERVER,
    SOCKET_TYPE_UDP_CLIENT,
    SOCKET_TYPE_TCP_SERVER,
    SOCKET_TYPE_TCP_CLIENT
} SOCKET_TYPE_e;

typedef enum {
    SOCKET_PROTOCOL_INVALID = 0,
    SOCKET_PROTOCOL_UDP,
    SOCKET_PROTOCOL_TCP
} SOCKET_PROTOCOL_e;


/* typedef */
typedef struct {
  int32_t mSync;  /* 0xCABE */
  int32_t mNumBytes;
  int32_t mTimeTagSec; /* NTP time */
  int32_t mDeviceId;
  int32_t mMsgType;
  int32_t checkSum; /* Checksum of the data payload */
} PacketHeader_t;


typedef struct {
    int32_t fd;
    int32_t mAddrLen;
    int32_t mConnected;
#ifndef _WIN32
    struct sockaddr_in mAddr;
#endif
    PacketHeader_t mHdr;
} Socket_t;


/* C function declarations */
int32_t InitSocket(Socket_t *mSock, SOCKET_TYPE_e mSockType, char *mIpAddr, uint32_t mPortNum);
int32_t CloseSocket(Socket_t *mSock);
int32_t AcceptConnection(Socket_t *mSock, Socket_t *mSockAcc);
int32_t QueryTCPConnectionState(Socket_t *mSock, Socket_t *mSockAcc);
int32_t RecvData(Socket_t *mSock, unsigned char *buff, int32_t mNumBytes);
int32_t XmitData(Socket_t *mSock, unsigned char *buff, int32_t mNumBytes);

int32_t ConfigBlockingState(Socket_t *mSock, int32_t mBlocking);
int32_t ConfigRecvBuffSize(Socket_t *mSock, int32_t mBuffSize);
int32_t ConfigXmitBuffSize(Socket_t *mSock, int32_t mBuffSize);
int32_t ConfigRecvSocketTimeout(Socket_t *mSock, int32_t msec);
int32_t ConfigXmitSocketTimeout(Socket_t *mSock, int32_t msec);


#ifdef __cplusplus
}
#endif


/* The 'CPP' definitions */
#ifdef __cplusplus

class Ethernet {

  private:
    int32_t Init(void);

  public:
    Ethernet(SOCKET_TYPE_e mSockType, char *mIpAddr, uint32_t mPortNum,
                   int32_t mBlocking = -1,
                   int32_t mRecvTimeout = -1, int32_t mXmitTimeout = -1,
                   int32_t mRecvBuffSize = -1, int32_t mXmitBuffSize = -1);

   ~Ethernet(void);

    int32_t ConfigBlockingState(int32_t mBlocking);
    int32_t ConfigRecvBuffSize(int32_t mBuffSize);
    int32_t ConfigXmitBuffSize(int32_t mBuffSize);
    int32_t ConfigRecvTimeout(int32_t msec);
    int32_t ConfigXmitTimeout(int32_t msec);

    int32_t RecvData(unsigned char *buff, int32_t mNumBytes);
    int32_t XmitData(unsigned char *buff, int32_t mNumBytes);

    SOCKET_TYPE_e     GetSocketType() { return(this->mSockType); }
    SOCKET_PROTOCOL_e GetSocketProtocol() { return(this->mSockProto); }
    int32_t GetBlockingState() { return(this->mBlocking); }
    int32_t GetRecvBuffSize() { return(this->mRecvBuffSize); }
    int32_t GetXmitBuffSize() { return(this->mXmitBuffSize); }
    int32_t GetRecvTimeout() { return(this->mRecvTimeout); }
    int32_t GetXmitTimeout() { return(this->mXmitTimeout); }

  protected:
    SOCKET_TYPE_e     SetSocketType(SOCKET_TYPE_e mSockType) { return(this->mSockType = mSockType); }
    SOCKET_PROTOCOL_e SetSocketProtocol(SOCKET_PROTOCOL_e mSockProto) { return(this->mSockProto = mSockProto); }
    int32_t SetBlockingState(int32_t mBlocking) { return(this->mBlocking = mBlocking); }
    int32_t SetRecvBuffSize(int32_t mBuffSize) { return(this->mRecvBuffSize = mBuffSize); }
    int32_t SetXmitBuffSize(int32_t mBuffSize) { return(this->mXmitBuffSize = mBuffSize); }
    int32_t SetRecvTimeout(int32_t msec) { return(this->mRecvTimeout = msec); }
    int32_t SetXmitTimeout(int32_t msec) { return(this->mXmitTimeout = msec); }

  private:

    Socket_t mSock;
    Socket_t mSockAcc;

    SOCKET_TYPE_e mSockType;
    SOCKET_PROTOCOL_e mSockProto;
    int32_t mBlocking;
    int32_t mRecvBuffSize;
    int32_t mXmitBuffSize;
    int32_t mRecvTimeout;
    int32_t mXmitTimeout;
};

#endif

#endif /* _ETHERNET_H_ */
