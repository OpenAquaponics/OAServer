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
 #include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
 #include <string.h>
#endif


#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>

/* user includes */
#include "pkt_types.h"


/* defines */
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
 
#define MAX_TCP_READS   (200)
#define SOCKET_SYNC     (0xCABE)
#define NUM_LISTEN      (1) /* This will be removed and the listen count will be dynamic */


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
    int fd;
    int mAddrLen;
    int mConnected;
#ifndef _WIN32
    struct sockaddr_in mAddr;
#endif
} Socket_t;


/* The class definitions */
class Ethernet {

  private:
    int Init(void);
    int InitSocket(char *mIpAddr, unsigned int mPortNum);
    int CloseSocket(void);

  public:
    /* Create a new socket from scratch */
    Ethernet(SOCKET_TYPE_e mSockType, char *mIpAddr, unsigned int mPortNum,
                   int mBlocking = -1,
                   int mRecvTimeout = -1, int mXmitTimeout = -1,
                   int mRecvBuffSize = -1, int mXmitBuffSize = -1);

    /* Create a new socket object from this existing socket */
    Ethernet(SOCKET_TYPE_e mSockType, Socket_t mSock);

    Ethernet(void) {};
   ~Ethernet(void);

    int ConfigBlockingState(int mBlocking);
    int ConfigRecvBuffSize(int mBuffSize);
    int ConfigXmitBuffSize(int mBuffSize);
    int ConfigRecvTimeout(int msec);
    int ConfigXmitTimeout(int msec);

    int RecvData(unsigned char *buff, int mNumBytes);
    int XmitData(unsigned char *buff, int mNumBytes);

    SOCKET_TYPE_e     GetSocketType() { return(this->mSockType); }
    SOCKET_PROTOCOL_e GetSocketProtocol() { return(this->mSockProto); }
    int GetSocketFd() { return(this->mSock.fd); }
    int GetBlockingState() { return(this->mBlocking); }
    int GetRecvBuffSize() { return(this->mRecvBuffSize); }
    int GetXmitBuffSize() { return(this->mXmitBuffSize); }
    int GetRecvTimeout() { return(this->mRecvTimeout); }
    int GetXmitTimeout() { return(this->mXmitTimeout); }


  protected:
    SOCKET_TYPE_e     SetSocketType(SOCKET_TYPE_e mSockType) { return(this->mSockType = mSockType); }
    SOCKET_PROTOCOL_e SetSocketProtocol(SOCKET_PROTOCOL_e mSockProto) { return(this->mSockProto = mSockProto); }
    int SetBlockingState(int mBlocking) { return(this->mBlocking = mBlocking); }
    int SetRecvBuffSize(int mBuffSize) { return(this->mRecvBuffSize = mBuffSize); }
    int SetXmitBuffSize(int mBuffSize) { return(this->mXmitBuffSize = mBuffSize); }
    int SetRecvTimeout(int msec) { return(this->mRecvTimeout = msec); }
    int SetXmitTimeout(int msec) { return(this->mXmitTimeout = msec); }

  private:
    Socket_t mSock;
    SOCKET_TYPE_e mSockType;
    SOCKET_PROTOCOL_e mSockProto;
    int mBlocking;
    int mRecvBuffSize;
    int mXmitBuffSize;
    int mRecvTimeout;
    int mXmitTimeout;

};


class EthernetList {
  private:
    int InitList(void);

  public:
    EthernetList(void);
   ~EthernetList(void);

    int AddSocket(SOCKET_TYPE_e mSockType, Socket_t mSock);
    int RemoveSocket(int mFd);
    int PollSocketList(void);

    /* The Derived class must implement a ProcessData function */
    virtual int ProcessData(Ethernet *mSock) = 0;

  private:
    std::vector< boost::shared_ptr<Ethernet> > vpSockList;
    int mMaxSelectNum;
    fd_set   mListFds;

};

#endif /* _ETHERNET_H_ */
