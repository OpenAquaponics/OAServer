/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/

/* user includes */
#include "HandlerThread.h"

using namespace std;

/****************************************/
void HandlerThread::Init(void) {
/****************************************/
  /* Initialize Variables */
  mThreadPriority = 0;
  mThreadPID = 0;

  pDb = new Database((char*)"./OAServer.sqlite");
}


/****************************************/
HandlerThread::HandlerThread(void) : EthernetList() {
/****************************************/
  Init();

  if(pthread_create(&thread, NULL, Run, static_cast<void*>(this))) {
    printf("ERR:  Failed to spawn handler thread\n");
  }
}


/****************************************/
HandlerThread::~HandlerThread(void) {
/****************************************/
  delete pDb;
}


/****************************************/
void* HandlerThread::Run(void *pParam) {
/****************************************/
  HandlerThread *pHandle = static_cast<HandlerThread*>(pParam);

  pHandle->SetThreadPID(pthread_self());

  while(1) {
    pHandle->PollSocketList();
    pHandle->UpdateThreadPriority();
  }

}


/****************************************/
int HandlerThread::UpdateThreadPriority(void) {
/****************************************/
  SetThreadPriority(EthernetList::GetSocketListSize());
}


/****************************************/
int HandlerThread::AddSocket(SOCKET_TYPE_e mSockType, Socket_t mSock) {
/****************************************/
  EthernetList::AddSocket(mSockType, mSock);
  UpdateThreadPriority();
  return(0);
}


/****************************************/
int HandlerThread::ProcessData(Ethernet *pSock) {
/****************************************/
  /* Variable Declaration */
  int mRetVal = 0;
  int s = pSock->GetSocketFd();
  PacketHeader_t mPktHdr;
  unsigned char *pData = NULL;


#if 0
  /* Check the make sure the stream is not closed */
  if((mRetVal = recv(s, buff, 1, MSG_PEEK)) == 0) {
    /* Somebody disconnected, get details and close */
    printf("INFO: Host disconnecting socket: %d\n", s);
    RemoveSocket(s);
  }
  /* Process the incoming data */
  else if(mRetVal == 1) {
    mRetVal = read(s, buff, sizeof(buff));
    printf("Processing data from %d: %d bytes\n", s, mRetVal);
  }
#endif


  /* TODO - Need to figure out how to get the Recv/Xmit socket stuff properly encapulated */
  /* Check the make sure the stream is not closed */
  if((mRetVal = recv(s, NULL, sizeof(mPktHdr), MSG_PEEK)) == 0) {
    /* Somebody disconnected, get details and close */
    printf("INFO: Host disconnecting socket: %d\n", s);
    RemoveSocket(s);
  }
  /* Process the incoming data (verifies size before continuing) */
  else if((mRetVal == sizeof(mPktHdr)) && 
          ((mRetVal = recv(s, NULL, sizeof(mPktHdr) + mPktHdr.mNumBytes, MSG_PEEK)) >= sizeof(mPktHdr) + mPktHdr.mNumBytes)) {
    if((mRetVal = read(s, &mPktHdr, sizeof(mPktHdr))) == sizeof(mPktHdr)) {
      if((pData = (unsigned char*)malloc(sizeof(mPktHdr) + mPktHdr.mNumBytes)) != NULL) {
        memcpy(pData, &mPktHdr, sizeof(mPktHdr));
        if((mRetVal = read(s, (void*)&pData[sizeof(mPktHdr)], mPktHdr.mNumBytes)) != mPktHdr.mNumBytes) {
          printf("ERR:  Error reading socket data\n");
        }
        else {
          ProcessPacket(pData);
        }
      }
    }
  }

  /* Make sure memory is cleared */
  if(pData) free(pData);


}



