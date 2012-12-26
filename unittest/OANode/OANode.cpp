
#include "OANode.h"


/****************************************/
void OANode::Init(void) {
/****************************************/
  srand(time(NULL));
}


/****************************************/
OANode::OANode(char *sIpAddr, unsigned int mPortNum, char *sDbName, unsigned int mNumThreads) {
/****************************************/
  Init();

  SetNumThreads(mNumThreads);
  pSock = new Ethernet(SOCKET_TYPE_TCP_CLIENT, sIpAddr, mPortNum);
}


/****************************************/
OANode::~OANode(void) {
/****************************************/
  if(pSock) {
    delete pSock;
  }
}


/****************************************/
int OANode::Run(void) {
/****************************************/


  return(0);
}


/****************************************/
int OANode::Debug(void) {
/****************************************/
  /* Variable Declaration */
  int mNumBytes = 0;
  char *pBuff = NULL;
  PacketHeader_t mHdr;

  /* Randomly decide if something should be transmitted */
  if(rand() % 2) {
    mNumBytes = rand() % 2048;
    if((pBuff = (char*)malloc(mNumBytes)) != NULL) {
      mHdr.mSync     = SYNC;
      mHdr.mNumBytes = mNumBytes;
      mHdr.mChecksum = ComputeChecksum((int*)pBuff, mHdr.mNumBytes);

      pSock->Send((unsigned char*)&mHdr, sizeof(mHdr));
      pSock->Send((unsigned char*)pBuff, mHdr.mNumBytes);
    }

    if(pBuff) free(pBuff);
    pBuff = NULL;
  }

  return(0);
}

