/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#include "Ethernet.h"


/****************************************/
int Ethernet::Init(void) {
/****************************************/
  mSockType     = SOCKET_TYPE_INVALID;
  mSockProto    = SOCKET_PROTOCOL_INVALID;
  mBlocking     = -1;
  mRecvTimeout  = -1;
  mXmitTimeout  = -1;
  mRecvBuffSize = -1;
  mXmitBuffSize = -1;
  
  memset(&mSock, 0, sizeof(mSock));

  return(0);
}


/****************************************/
Ethernet::Ethernet(SOCKET_TYPE_e mSockType, char *mIpAddr, unsigned int mPortNum, int mBlocking,
                   int mRecvTimeout, int mXmitTimeout,
                   int mRecvBuffSize, int mXmitBuffSize) {
/****************************************/
  /* Initialize all of the class data members */
  Init();

  /* Save the incoming data */
  this->mSockType = mSockType;
  if((mSockType == SOCKET_TYPE_UDP_SERVER) || (mSockType == SOCKET_TYPE_UDP_CLIENT)) {
    this->mSockProto = SOCKET_PROTOCOL_UDP;
  }
  else if((mSockType == SOCKET_TYPE_TCP_SERVER) || (mSockType == SOCKET_TYPE_TCP_CLIENT)) {
    this->mSockProto = SOCKET_PROTOCOL_TCP;
  }

  /* Initialize the socket */
  if(InitSocket(mIpAddr, mPortNum) < 0) {
    CloseSocket();
  }
  else {
    ConfigBlockingState(mBlocking);
    ConfigRecvBuffSize(mRecvBuffSize);
    ConfigXmitBuffSize(mXmitBuffSize);
    ConfigRecvTimeout(mRecvTimeout);
    ConfigXmitTimeout(mXmitTimeout);
  }

}


/****************************************/
Ethernet::Ethernet(SOCKET_TYPE_e mSockType, Socket_t mSock) {
/****************************************/
  /* Initialize all of the class data members */
  Init();

  /* Save the incoming data */
  this->mSockType = mSockType;
  if((mSockType == SOCKET_TYPE_UDP_SERVER) || (mSockType == SOCKET_TYPE_UDP_CLIENT)) {
    this->mSockProto = SOCKET_PROTOCOL_UDP;
  }
  else if((mSockType == SOCKET_TYPE_TCP_SERVER) || (mSockType == SOCKET_TYPE_TCP_CLIENT)) {
    this->mSockProto = SOCKET_PROTOCOL_TCP;
  }

  /* Save the incoming parameters */
  this->mSock = mSock;
}


/****************************************/
int Ethernet::CloseSocket(void) {
/****************************************/
  /* Close the socket */
  if(mSock.fd) {
    CLOSE_SOCKET(mSock.fd);
  }
  memset(&mSock, 0, sizeof(mSock));


  return(0);
}


/****************************************/
Ethernet::~Ethernet(void) {
/****************************************/
  CloseSocket();
}


/****************************************/
int Ethernet::Send(unsigned char *mBuff, int mNumBytes) {
/****************************************/
  /* Variable Declaration */
  int mXmitBytes = 0;

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    /* Send the data over the socket */
    if((mXmitBytes = sendto(mSock.fd, mBuff, mNumBytes, 0, (struct sockaddr *)&mSock.mAddr, (int)mSock.mAddrLen)) < 0) {
      printf("ERR:  Failed to xmit socket data: %d of %d  (%s)\n", mXmitBytes, mNumBytes, strerror(errno));
      return(-1);
    }
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
    /* Send the data over the socket */
    if((mXmitBytes = send(mSock.fd, mBuff, mNumBytes, 0)) < 0) {
      printf("ERR:  Failed to xmit socket data: %d of %d  (%s)\n", mXmitBytes, mNumBytes, strerror(errno));
      mSock.mConnected = FALSE;
      return(-1);
    }
  }

  return(mXmitBytes);
}


/****************************************/
int Ethernet::Recv(unsigned char *mBuff, int mNumBytes) {
/****************************************/
  /* Variable Declaration */
  int i = 0;
  int mRetVal = 0;
  int mRecvBytes = 0;

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    for(i = 0, mRecvBytes = 0; (mRecvBytes < mNumBytes) && (i < MAX_TCP_READS); mRecvBytes += mRetVal, i++) {
      if((mRetVal = recvfrom(mSock.fd, &mBuff[mRecvBytes], (mNumBytes - mRecvBytes), 0, (struct sockaddr *)&mSock.mAddr, (socklen_t*)&mSock.mAddrLen)) < 0) {
        printf("ERR:  Failed to recieve socket data: %d of %d  (%s)\n", mRecvBytes, mNumBytes, strerror(errno));
        return(-1);
      }
    }
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
    for(i = 0, mRecvBytes = 0; (mRecvBytes < mNumBytes) && (i < MAX_TCP_READS); mRecvBytes += mRetVal, i++) {
      if((mRetVal = recv(mSock.fd, &mBuff[mRecvBytes], (mNumBytes - mRecvBytes), 0)) < 0) {
        printf("ERR:  Failed to recieve socket data: %d of %d  (%s)\n", mRecvBytes, mNumBytes, strerror(errno));
        mSock.mConnected = FALSE;
        return(-1);
      }
    }
  }

  return(mRecvBytes);
}


/****************************************/
int Ethernet::Peek(int mNumBytes) {
/****************************************/
  char *pData = (char*)malloc(mNumBytes);

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(-1);
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
    return(recv(mSock.fd, pData, mNumBytes, MSG_PEEK));
  }

  return(-1);
}


/****************************************/
int Ethernet::Peek(void) {
/****************************************/
  char pBuff = 0;

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(-1);
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
    return(recv(mSock.fd, &pBuff, sizeof(pBuff), MSG_PEEK));
  }

  return(-1);
}


/****************************************/
bool Ethernet::Seek(int mVal, int mAttempts) {
/****************************************/
  int mBuff = mVal - 1;

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(FALSE);
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
    for(int i = mAttempts; (i > 0) && (mBuff != mVal); i--) {
      if(recv(mSock.fd, &mBuff, sizeof(mBuff), MSG_PEEK) != sizeof(mBuff)) {
        printf("ERR: Ethernet::Seek() -> Failed on MSG_PEEK\n");
        return(FALSE);
      }
      else if(mBuff == mVal) {
        return(TRUE);
      }
      else {
        recv(mSock.fd, &mBuff, sizeof(mBuff), 0);
      }
    }
  }

  return(FALSE);
}





