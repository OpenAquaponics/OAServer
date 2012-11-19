

#include "Ethernet.h"


/****************************************/
int Ethernet::InitSocket(char *mIpAddr, unsigned int mPortNum) {
/****************************************/
  /* Variable Declaration */
  int mRetVal = 0;
  int mOpt = TRUE;
  
  /* UDP_SERVER */
  if(mSockType == SOCKET_TYPE_UDP_SERVER) {
    /* Open the socket file descriptor */
    if((mSock.fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
      printf("ERR: Opening UDP server socket\n");
      return(-1);
    }

    /* Load the socket structure */
    memset(&mSock.mAddr, 0, sizeof(mSock.mAddr));
    mSock.mAddr.sin_family      = AF_INET;
    mSock.mAddr.sin_port        = htons(mPortNum);
    mSock.mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    mSock.mAddrLen = sizeof(mSock.mAddr);
    
    /* Bind on the socket port */
    if((mRetVal = bind(mSock.fd, (struct sockaddr*)&mSock.mAddr, sizeof(mSock.mAddr))) < 0 ) {
      printf("ERR: Binding on port %d\n", mPortNum);
      CloseSocket();
      return(-1);
    }
  }
  /* UDP_CLIENT */
  else if(mSockType == SOCKET_TYPE_UDP_CLIENT) {
    /* Open the socket file descriptor */
    if((mSock.fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
      printf("ERR: Opening UDP client socket\n");
      return(-1);
    }
    
    /* Load the socket structure */
    memset(&mSock.mAddr, 0, sizeof(mSock.mAddr));
    mSock.mAddr.sin_family      = AF_INET;
    mSock.mAddr.sin_port        = htons(mPortNum);
    mSock.mAddr.sin_addr.s_addr = inet_addr(mIpAddr);
    mSock.mAddrLen = sizeof(mSock.mAddr);
  }
  /* TCP_SERVER */
  else if(mSockType == SOCKET_TYPE_TCP_SERVER) {
    /* Open the socket file descriptor */
    if((mSock.fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
      printf("ERR: Opening TCP server socket\n");
      return(-1);
    }

    /* Set master socket to allow multiple connections */
    if(setsockopt(mSock.fd, SOL_SOCKET, SO_REUSEADDR, (char *)&mOpt, sizeof(mOpt)) < 0 ) {
      printf("ERR: Configuring socket to reuseable\n");
      CloseSocket();
      return(-1);
    }
    
    /* Load the socket structure */
    memset(&mSock.mAddr, 0, sizeof(mSock.mAddr));
    mSock.mAddr.sin_family      = AF_INET;
    mSock.mAddr.sin_port        = htons(mPortNum);
    mSock.mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    mSock.mAddrLen = sizeof(mSock.mAddr);

    /* Bind on the socket port */
    if((mRetVal = bind(mSock.fd, (struct sockaddr*)&mSock.mAddr, sizeof(mSock.mAddr))) < 0 ) {
      printf("ERR: Binding on port %d\n", mPortNum);
      CloseSocket();
      return(-1);
    }

    /* Listen on the port */
    if(listen(mSock.fd, NUM_LISTEN) < 0) {
      printf("ERR: Listen on port %d\n", mPortNum);
      CloseSocket();
      return(-1);
    }
  }
  /* TCP_CLIENT */
  else if(mSockType == SOCKET_TYPE_TCP_CLIENT) {
    /* Open the socket file descriptor */
    if((mSock.fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
      printf("ERR: Opening TCP client socket\n");
      return(-1);
    }
    
    /* Load the socket structure */
    memset(&mSock.mAddr, 0, sizeof(mSock.mAddr));
    mSock.mAddr.sin_family      = AF_INET;
    mSock.mAddr.sin_port        = htons(mPortNum);
    mSock.mAddr.sin_addr.s_addr = inet_addr(mIpAddr);
    mSock.mAddrLen = sizeof(mSock.mAddr);

    mSock.mConnected = FALSE;
    /* Connect the socket */
    if(connect(mSock.fd, (struct sockaddr*)&mSock.mAddr, sizeof(mSock.mAddr)) < 0) {
      printf("ERR: Connecting to remote TCP server\n");
      CloseSocket();
      return(-1);
    }
    mSock.mConnected = TRUE;
  }
  else {
    printf("ERR: Invalid socket type: %d\n", mSockType);
    return(-1);
  }

  /* Set the select maximum socket number */
  mMaxSelectNum = mSock.fd;

  return(0);
}


/****************************************/
int Ethernet::ConfigBlockingState(int mBlocking) {
/****************************************/
  /* Sanity Check */
  if(mBlocking == -1) {
    return(0);
  }

  /* Configure the device */
  SetBlockingState(mBlocking);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    /* Set the socket to be non-mBlocking */
    if(IOCTL(mSock.fd, FIONBIO, &mBlocking) < 0) {
      printf("ERR: Configuring socket to non-mBlocking  (%d)\n", mSock.fd);
      return(-1);
    }
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigBlockingState(&this->mSockAcc, this->mBlocking));
#endif
  }

  return(0);
}

/****************************************/
int Ethernet::ConfigRecvBuffSize(int mBuffSize) {
/****************************************/
  /* Sanity Check */
  if(mBuffSize == -1) {
    return(0);
  }

  /* Variable Declaration */
  int fd = 0;
  int mRetVal = 0;
  int mParamLen = sizeof(mRetVal);

  /* Configure the device */
  SetRecvBuffSize(mBuffSize);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    fd = this->mSock.fd;
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigRecvBuffSize(&this->mSockAcc, this->mRecvBuffSize));
#endif
  }
    
  /* Sent the buffer size */
  if(setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&mBuffSize, mParamLen) < 0) {
    printf("ERR: Configuring socket recv buffer size to %d\n", mBuffSize);
    return(-1);
  }

  /* Make sure the buffer was set to proper size */
  getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&mRetVal, (socklen_t*)&mParamLen);
  if(mRetVal != mBuffSize) {
    printf("ERR: Configuring socket recv buffer size to %d, current size %d\n", mBuffSize, mRetVal);
    return(-1);
  }

  return(0);
}

/****************************************/
int Ethernet::ConfigXmitBuffSize(int mBuffSize) {
/****************************************/
  /* Sanity Check */
  if(mBuffSize == -1) {
    return(0);
  }

  /* Variable Declaration */
  int fd = 0;
  int mRetVal = 0;
  int mParamLen = sizeof(mRetVal);

  /* Configure the device */
  SetXmitBuffSize(mBuffSize);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    fd = this->mSock.fd;
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigXmitBuffSize(&this->mSockAcc, this->mXmitBuffSize));
#endif
  }

  /* Sent the buffer size */
  if(setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&mBuffSize, mParamLen) < 0) {
    printf("ERR: Configuring socket xmit buffer size to %d\n", mBuffSize);
    return(-1);
  }

  /* Make sure the buffer was set to proper size */
  getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&mRetVal, (socklen_t*)&mParamLen);
  if(mRetVal != mBuffSize) {
    printf("ERR: Configuring socket xmit buffer size to %d, current size %d\n", mBuffSize, mRetVal);
    return(-1);
  }

  return(0);
}

/****************************************/
int Ethernet::ConfigRecvTimeout(int msec) {
/****************************************/
  /* Sanity Check */
  if(msec == -1) {
    return(0);
  }

  /* Variable Declaration */
  int fd = 0;
  int mRetVal = 0;
  int mParamLen = sizeof(mRetVal);

  /* Configure the device */
  SetRecvTimeout(msec);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    fd = this->mSock.fd;
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigRecvSocketTimeout(&this->mSockAcc, this->mRecvTimeout));
#endif
  }

  /* Set the socket to have a timeout */
  if(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void*)&msec, mParamLen) < 0) {
    printf("ERR: Configuring recv socket for timeout\n");
    return(-1);
  }

  /* Make sure the buffer was set to proper size */
  getsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&mRetVal, (socklen_t*)&mParamLen);
  if(mRetVal != msec) {
    printf("ERR: Configuring socket recv timeout to %d, expected %d\n", mRetVal, msec);
    return(-1);
  }

  return(0);
}

/****************************************/
int Ethernet::ConfigXmitTimeout(int msec) {
/****************************************/
  /* Sanity Check */
  if(msec == -1) {
    return(0);
  }

  /* Variable Declaration */
  int fd = 0;
  int mRetVal = 0;
  int mParamLen = sizeof(mRetVal);

  /* Configure the device */
  SetXmitTimeout(msec);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    fd = this->mSock.fd;
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigXmitSocketTimeout(&this->mSockAcc, this->mXmitTimeout));
#endif
  }

  /* Set the socket to have a timeout */
  if(setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void*)&msec, mParamLen) < 0) {
    printf("ERR: Configuring xmit socket for timeout\n");
    return(-1);
  }

  /* Make sure the buffer was set to proper size */
  getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&mRetVal, (socklen_t*)&mParamLen);
  if(mRetVal != msec) {
    printf("ERR: Configuring socket xmit timeout to %d, expected %d\n", mRetVal, msec);
    return(-1);
  }

  return(0);
}


