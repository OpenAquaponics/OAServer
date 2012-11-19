/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#include "Ethernet.h"


/****************************************/
int32_t Ethernet::Init(void) {
/****************************************/
  this->mSockType     = SOCKET_TYPE_INVALID;
  this->mSockProto    = SOCKET_PROTOCOL_INVALID;
  this->mBlocking     = -1;
  this->mRecvTimeout  = -1;
  this->mXmitTimeout  = -1;
  this->mRecvBuffSize = -1;
  this->mXmitBuffSize = -1;

  memset(&this->mSock, 0, sizeof(this->mSock));
  for(int i = 0; i < NUM_LISTEN; i++) {
    memset(&this->mClientSock[i], 0, sizeof(Socket_t));
  }

  return(0);
}

/****************************************/
Ethernet::Ethernet(SOCKET_TYPE_e mSockType, char *mIpAddr, uint32_t mPortNum, int32_t mBlocking,
                   int32_t mRecvTimeout, int32_t mXmitTimeout,
                   int32_t mRecvBuffSize, int32_t mXmitBuffSize) {
/****************************************/
  /* Initialize all of the class data members */
  Init();

  /* Call the 'C' implementation */
  if(::InitSocket(&mSock, mSockType, mIpAddr, mPortNum) < 0) {
    ::CloseSocket(&mSock);
    for(int i = 0; i < NUM_LISTEN; i++) {
      ::CloseSocket(&mClientSock[i]);
    }
  }
  else {
    /* Save the incoming data */
    this->mSockType = mSockType;
    if((mSockType == SOCKET_TYPE_UDP_SERVER) || (mSockType == SOCKET_TYPE_UDP_CLIENT)) {
      this->mSockProto = SOCKET_PROTOCOL_UDP;
    }
    else if((mSockType == SOCKET_TYPE_TCP_SERVER) || (mSockType == SOCKET_TYPE_TCP_CLIENT)) {
      this->mSockProto = SOCKET_PROTOCOL_TCP;
    }

    ConfigBlockingState(mBlocking);
    ConfigRecvBuffSize(mRecvBuffSize);
    ConfigXmitBuffSize(mXmitBuffSize);
    ConfigRecvTimeout(mRecvTimeout);
    ConfigXmitTimeout(mXmitTimeout);
  }

}

/****************************************/
Ethernet::~Ethernet(void) {
/****************************************/

  /* Call the 'C' implementation */
  ::CloseSocket(&mSock);
  for(int i = 0; i < NUM_LISTEN; i++) {
    ::CloseSocket(&mClientSock[i]);
  }
}

/****************************************/
int32_t Ethernet::ConfigBlockingState(int32_t mBlocking) {
/****************************************/
  /* Sanity Check */
  if(mBlocking == -1) {
    return(0);
  }

  /* Configure the device */
  SetBlockingState(mBlocking);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(::ConfigBlockingState(&this->mSock, this->mBlocking));
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigBlockingState(&this->mSockAcc, this->mBlocking));
#endif
  }

  return(-1);
}

/****************************************/
int32_t Ethernet::ConfigRecvBuffSize(int32_t mBuffSize) {
/****************************************/
  /* Sanity Check */
  if(mBuffSize == -1) {
    return(0);
  }

  /* Configure the device */
  SetRecvBuffSize(mBuffSize);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(::ConfigRecvBuffSize(&this->mSock, this->mRecvBuffSize));
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigRecvBuffSize(&this->mSockAcc, this->mRecvBuffSize));
#endif
  }

  return(-1);
}

/****************************************/
int32_t Ethernet::ConfigXmitBuffSize(int32_t mBuffSize) {
/****************************************/
  /* Sanity Check */
  if(mBuffSize == -1) {
    return(0);
  }

  /* Configure the device */
  SetXmitBuffSize(mBuffSize);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(::ConfigXmitBuffSize(&this->mSock, this->mXmitBuffSize));
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigXmitBuffSize(&this->mSockAcc, this->mXmitBuffSize));
#endif
  }

  return(-1);
}

/****************************************/
int32_t Ethernet::ConfigRecvTimeout(int32_t msec) {
/****************************************/
  /* Sanity Check */
  if(msec == -1) {
    return(0);
  }

  /* Configure the device */
  SetRecvTimeout(msec);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(::ConfigRecvSocketTimeout(&this->mSock, this->mRecvTimeout));
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigRecvSocketTimeout(&this->mSockAcc, this->mRecvTimeout));
#endif
  }

  return(-1);
}

/****************************************/
int32_t Ethernet::ConfigXmitTimeout(int32_t msec) {
/****************************************/
  /* Sanity Check */
  if(msec == -1) {
    return(0);
  }

  /* Configure the device */
  SetXmitTimeout(msec);
  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(::ConfigXmitSocketTimeout(&this->mSock, this->mXmitTimeout));
  }
  else if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::ConfigXmitSocketTimeout(&this->mSockAcc, this->mXmitTimeout));
#endif
  }

  return(-1);
}

/****************************************/
int32_t Ethernet::XmitData(unsigned char *mBuff, int32_t mNumBytes) {
/****************************************/

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(::XmitData(&mSock, mBuff, mNumBytes));
  }
  if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::XmitData(&mSockAcc, mBuff, mNumBytes));
#endif
  }

  return(-1); 
}

/****************************************/
int32_t Ethernet::RecvData(unsigned char *mBuff, int32_t mNumBytes) {
/****************************************/

  if(this->mSockProto == SOCKET_PROTOCOL_UDP) {
    return(::RecvData(&mSock, mBuff, mNumBytes));
  }
  if(this->mSockProto == SOCKET_PROTOCOL_TCP) {
#if 0
    return(::RecvData(&mSockAcc, mBuff, mNumBytes));
#endif
  }

  return(-1); 
}


#include "pkt_types.h" 
/****************************************/
int32_t Ethernet::PollOpenSockets(void) {
/****************************************/
  /* Variable Declaration */
  int i = 0;
  int s = 0;
  int mRetVal = 0;
  int mNewSock = 0;
  char eof[16];
  PacketHeader_t hdr;

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
  /* TODO - Why the +3?? */
  if((select(NUM_LISTEN + 3 , &mReadFds , NULL , NULL , NULL) < 0) && (errno != EINTR)) {
    printf("ERR: Selection error\n");
  }

  /* If something happened on the master socket , then its an incoming connection */
  if(FD_ISSET(mSock.fd, &mReadFds)) {
    if((mNewSock = accept(mSock.fd, (struct sockaddr *)&mSock.mAddr, (socklen_t*)&mSock.mAddrLen))<0) {
      printf("ERR: Accepting new socket connection\n");
      return(-1);
    }

    /* Inform the user of new incoming socket number */
    printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , mClientSock[i].fd, inet_ntoa(mClientSock[i].mAddr.sin_addr) , ntohs(mClientSock[i].mAddr.sin_port));

    /* Add new incoming socket connections to the array */
    for(i = 0; i < NUM_LISTEN; i++) {
      if(mClientSock[i].fd == 0) {
        mClientSock[i].fd = mNewSock;
        mClientSock[i].mAddrLen = sizeof(mClientSock[i].mAddr);
        printf("INFO: Adding to list of sockets as %d\n" , i);
        i = NUM_LISTEN;
      }
      else if(i == (NUM_LISTEN - 1)) {
        printf("INFO: Need to handle rejected the incoming connction\n");
      }
    }
  }

  /* Process all of the remaining sockets for any data */
  for(i = 0; i < NUM_LISTEN; i++) {
    s = mClientSock[i].fd;

    if(FD_ISSET(s ,&mReadFds)) {
      /* Check the make sure the stream is not closed */
      if((mRetVal = recv(s, eof, sizeof(eof), MSG_PEEK)) == 0) {
        /* Somebody disconnected , get his details and print */
        getpeername(s, (struct sockaddr*)&mClientSock[i].mAddr, (socklen_t*)&mClientSock[i].mAddrLen);
        printf("INFO: Host disconnected , ip %s , port %d \n" , inet_ntoa(mClientSock[i].mAddr.sin_addr) , ntohs(mClientSock[i].mAddr.sin_port));

        /* Close the socket and mark as 0 in list for reuse */
        close(s);
        memset(&mClientSock[i], 0, sizeof(Socket_t));
      }
      /* Process the incoming data */
      /* TODO - This needs to have a functional callback, inherited method, or msq in order to send data to processing task */
      else {
        if((mRetVal = read(s , &hdr, sizeof(hdr))) == sizeof(hdr)) {
          unsigned char *data = NULL;

          if((data = (unsigned char*)malloc(hdr.mNumBytes)) != NULL) {
            mRetVal = read(s ,data, hdr.mNumBytes);
          }

/************************/
          if(hdr.mMsgType == ADD_MEASUREMENT) {
            meas_data_t  *measData = (meas_data_t*)data;
            printf("%4d,%02d,%02d,%s,%.2f,%s\n",
              measData->rpiHdr.year, measData->rpiHdr.month, measData->rpiHdr.day,
              meas_type[measData->measType].abbr, measData->measData, measData->comment);
          }
/***********************/
        }
      }
    }
  }

  return(0);
}



