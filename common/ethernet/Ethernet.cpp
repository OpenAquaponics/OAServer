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
  
  pData = NULL;

  memset(&mSock, 0, sizeof(mSock));
  for(int i = 0; i < NUM_LISTEN; i++) {
    memset(&mClientSock[i], 0, sizeof(Socket_t));
  }

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
int Ethernet::CloseSocket(void) {
/****************************************/
  /* Close the socket */
  if(mSock.fd) {
    CLOSE_SOCKET(mSock.fd);
  }
  memset(&mSock, 0, sizeof(mSock));

  /* Close the client sockets */
  for(int i = 0; i < NUM_LISTEN; i++) {
    if(mClientSock[i].fd) {
      CLOSE_SOCKET(mClientSock[i].fd);
    }
    memset(&mClientSock[i], 0, sizeof(Socket_t));
  }

  return(0);
}


/****************************************/
Ethernet::~Ethernet(void) {
/****************************************/
  CloseSocket();
}

/****************************************/
int Ethernet::XmitData(unsigned char *mBuff, int mNumBytes) {
/****************************************/
  /* Variable Declaration */
  int mXmitBytes = 0;

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    /* Send the data over the socket */
    if((mXmitBytes = sendto(mSock.fd, mBuff, mNumBytes, 0, (struct sockaddr *)&mSock.mAddr, (int)mSock.mAddrLen)) < 0) {
      printf("ERR:  Failed to xmit socket data: %d of %d  (%d)\n", mXmitBytes, mNumBytes, errno);
      mSock.mConnected = FALSE;
      return(-1);
    }
  }
  if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::XmitData(&mSockAcc, mBuff, mNumBytes));
#endif
  }

  return(mXmitBytes);
}

/****************************************/
int Ethernet::RecvData(unsigned char *mBuff, int mNumBytes) {
/****************************************/
  /* Variable Declaration */
  int i = 0;
  int mRetVal = 0;
  int mRecvBytes = 0;

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    for(i = 0, mRecvBytes = 0; (mRecvBytes < mNumBytes) && (i < MAX_TCP_READS); mRecvBytes += mRetVal, i++) {
      if((mRetVal = recvfrom(mSock.fd, &mBuff[mRecvBytes], (mNumBytes - mRecvBytes), 0, (struct sockaddr *)&mSock.mAddr, (socklen_t*)&mSock.mAddrLen)) < 0) {
        printf("ERR:  Failed to recieve socket data: %d of %d\n", mRecvBytes, mNumBytes);
        return(-1);
      }
    }

  }
  if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::RecvData(&mSockAcc, mBuff, mNumBytes));
#endif
  }

  return(mRecvBytes);
}


/****************************************/
int Ethernet::ClearPacket(void) {
/****************************************/
  /* Clear all of the data structures */
  if(pData) {
    free(pData);
    pData = NULL;
  }

  return(0);
}


/****************************************/
int Ethernet::PollOpenSockets(void) {
/****************************************/
  /* Variable Declaration */
  int i = 0;
  int s = 0;
  int mRetVal = 0;
  int mNewSock = 0;
  char eof[1024];

  /* Clear the socket set */
  FD_ZERO(&mReadFds);
  FD_SET(mSock.fd, &mReadFds);

  /* Add child sockets to the list */
  for(i = 0; i < NUM_LISTEN; i++) {
    if(mClientSock[i].fd > 0) {
      FD_SET(mClientSock[i].fd , &mReadFds);
    }
  }

  /* Wait for activity on one of the sockets , timeout is NULL , so wait indefinitely */
  if((select(mMaxSelectNum + 1, &mReadFds , NULL , NULL , NULL) < 0) && (errno != EINTR)) {
    printf("ERR:  Selection error\n");
  }

  /* If something happened on the master socket , then its an incoming connection */
  if(FD_ISSET(mSock.fd, &mReadFds)) {
    if((mNewSock = accept(mSock.fd, (struct sockaddr *)&mSock.mAddr, (socklen_t*)&mSock.mAddrLen)) < 0) {
      printf("ERR:  Accepting new socket connection\n");
      return(-1);
    }

    /* Inform the user of new incoming socket number */
    printf("INFO: New connection(%d) %s:%d\n" , mNewSock, inet_ntoa(mSock.mAddr.sin_addr) , ntohs(mSock.mAddr.sin_port));

    /* Add new incoming socket connections to the array */
    for(i = 0; i < NUM_LISTEN; i++) {
      if(mClientSock[i].fd == 0) {
        mClientSock[i].fd       = mNewSock;
        mClientSock[i].mAddr    = mSock.mAddr;
        mClientSock[i].mAddrLen = sizeof(mClientSock[i].mAddr);
        /* Detemine the highest file descriptor the select will trigger on */
        if(mNewSock > mMaxSelectNum) {
          mMaxSelectNum = mNewSock;
        }
        printf("INFO:   Adding socket to list as %d\n", i);
        i = NUM_LISTEN;
      }
      else if(i == (NUM_LISTEN - 1)) {
        printf("ERR:    REJECTED incoming socket %s:%d\n", inet_ntoa(mSock.mAddr.sin_addr) , ntohs(mSock.mAddr.sin_port));
      }
    }
  }

  /* Process all of the remaining sockets for any data */
  for(i = 0; i < NUM_LISTEN; i++) {
    s = mClientSock[i].fd;
    if(FD_ISSET(s ,&mReadFds)) {
      /* Clear the buffer data */
      ClearPacket();

      /* Check the make sure the stream is not closed */
      if((mRetVal = recv(s, &mPktHdr, sizeof(mPktHdr), MSG_PEEK)) == 0) {
        /* Somebody disconnected , get his details and print */
        getpeername(s, (struct sockaddr*)&mClientSock[i].mAddr, (socklen_t*)&mClientSock[i].mAddrLen);
        printf("INFO: Host disconnected  %s:%d\n", inet_ntoa(mClientSock[i].mAddr.sin_addr) , ntohs(mClientSock[i].mAddr.sin_port));

        /* Close the socket and mark as 0 in list for reuse */
        close(s);
        memset(&mClientSock[i], 0, sizeof(Socket_t));
      }
      /* Process the incoming data */
      /* TODO - This needs to have a functional callback, inherited method, or msq in order to send data to processing task */
      /* Verify the entire packet is on the buffer before attempting to process it.
         The arduino write() sends each packet individually.
         TODO - This will need to change. */
      else if((mRetVal == sizeof(mPktHdr)) && 
              ((mRetVal = recv(s, eof, sizeof(mPktHdr) + mPktHdr.mNumBytes, MSG_PEEK)) >= sizeof(mPktHdr) + mPktHdr.mNumBytes)) {
        if((mRetVal = read(s , &mPktHdr, sizeof(mPktHdr))) == sizeof(mPktHdr)) {
          ClearPacket();
          if((pData = (unsigned char*)malloc(sizeof(mPktHdr) + mPktHdr.mNumBytes)) != NULL) {
            memcpy(pData, &mPktHdr, sizeof(mPktHdr));
            if((mRetVal = read(s, (void*)&pData[sizeof(mPktHdr)], mPktHdr.mNumBytes)) != mPktHdr.mNumBytes) {
              printf("ERR:  Error reading socket data\n");
              ClearPacket();
            }
          }
        }
      }
    }
  }

  return(0);
}



