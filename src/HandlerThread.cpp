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
  static PacketHeader_t mHdr;
  static unsigned char mBuff[1024];
  unsigned char *pData = mBuff;

  /* Check if the client disconnected */
  if((mRetVal = pSock->Peek()) == 0) {
    /* Somebody disconnected, get details and close */
    printf("INFO: Host disconnecting socket: %d\n", pSock->GetSocketFd());
    RemoveSocket(pSock->GetSocketFd());
  }
  /* There is data on the socket, so process it */
  else if((mRetVal = pSock->Recv((unsigned char*)&mHdr, sizeof(mHdr))) == sizeof(mHdr)) {
    /* Determine if more memory is needed */
    if((sizeof(mHdr) + mHdr.mNumBytes) > sizeof(mBuff)) {
      pData = (unsigned char*)malloc(sizeof(mHdr) + mHdr.mNumBytes);
    }

    /* If the buffer pointer is valid, then read away */
    if(pData) {
      memcpy(pData, &mHdr, sizeof(mHdr));
      if((mRetVal = pSock->Recv((unsigned char*)&pData[sizeof(mHdr)], mHdr.mNumBytes)) != mHdr.mNumBytes) {
        printf("ERR:  Error reading socket data (%d of %d)\n", mRetVal, mHdr.mNumBytes);
      }
      else {
        ProcessPacket(pData);
      }
    }
  }
  else {
    printf("ERR: Read %d bytes of %d\n", mRetVal, sizeof(mHdr));
  }

  /* Free memory if it was allocated */
  if((pData != mBuff) && (pData)) {
    free(pData);
  }


}



