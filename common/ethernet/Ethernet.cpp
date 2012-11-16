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
  memset(&this->mSockAcc, 0, sizeof(this->mSockAcc));

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
    ::CloseSocket(&mSockAcc);
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
  ::CloseSocket(&mSockAcc);
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
    return(::ConfigBlockingState(&this->mSockAcc, this->mBlocking));
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
    return(::ConfigRecvBuffSize(&this->mSockAcc, this->mRecvBuffSize));
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
    return(::ConfigXmitBuffSize(&this->mSockAcc, this->mXmitBuffSize));
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
    return(::ConfigRecvSocketTimeout(&this->mSockAcc, this->mRecvTimeout));
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
    return(::ConfigXmitSocketTimeout(&this->mSockAcc, this->mXmitTimeout));
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
    return(::XmitData(&mSockAcc, mBuff, mNumBytes));
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
    return(::RecvData(&mSockAcc, mBuff, mNumBytes));
  }

  return(-1); 
}
