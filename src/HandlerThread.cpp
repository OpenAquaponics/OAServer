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

}


/****************************************/
void* HandlerThread::Run(void *pParam) {
/****************************************/
  HandlerThread *pHandle = static_cast<HandlerThread*>(pParam);

  pHandle->SetThreadPID(pthread_self());

  while(1) {
    pHandle->PollSocketList();
  }

}


/****************************************/
int HandlerThread::AddSocket(SOCKET_TYPE_e mSockType, Socket_t mSock) {
/****************************************/
  printf("(%d)\n", EthernetList::GetSocketListSize());
  EthernetList::AddSocket(mSockType, mSock);

  SetThreadPriority(EthernetList::GetSocketListSize());
  printf("  %u -> %d  (%d)\n", GetThreadPID(), GetThreadPriority(), EthernetList::GetSocketListSize());
  return(0);
}


/****************************************/
int HandlerThread::ProcessData(Ethernet *pSock) {
/****************************************/
  /* Variable Declaration */
  int mRetVal = 0;
  int s = pSock->GetSocketFd();
  char buff[1000];

  /* Recv the data from the socket */
    /* Remove back sockets */
  /* Parse request */
  /* Write/Read from DB */

  /* Check the make sure the stream is not closed */
  if((mRetVal = recv(s, buff, 1, MSG_PEEK)) == 0) {
    /* Somebody disconnected, get details and close */
    printf("INFO: Host disconnecting socket: %d\n", s);
    RemoveSocket(s);
  }
  /* Process the incoming data */
  else if(mRetVal == 1) {
    printf("Processing data from: %d\n", s);
    if((mRetVal = read(s, buff, sizeof(buff))) > 0) { printf("  Got %d bytes\n", mRetVal); };
  }


#if 0
  /* TODO - Need to figure out how to get the Recv/Xmit socket stuff properly encapulated */
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
#endif

}



