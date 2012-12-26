/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/

/* user includes */
#include "OANode.h"

using namespace std;

/****************************************/
void OANode::Init(void) {
/****************************************/
  srand(time(NULL));

}


/****************************************/
OANode::OANode(char *sIpAddr, unsigned int mPortNum, char *sDbName, unsigned int mNumThreads) {
/****************************************/
  Init();

  pOANodeCfg = new OANodeCfg(std::string("../OANode.json"));

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

  /* TODO - There should a debug that exercises off-nominals
      and then one that has valid data, but tons of it */

  Step();

  /* Randomly decide if something should be transmitted */
  if(rand() % 2) {
    mNumBytes = rand() % 2048;
    if((pBuff = (char*)malloc(mNumBytes)) != NULL) {
      mHdr.mSync     = SYNC;
      mHdr.mNumBytes = mNumBytes;
      mHdr.mChecksum = ComputeChecksum((int*)pBuff, mHdr.mNumBytes) + (rand() % 2);

      pSock->Send((unsigned char*)&mHdr, sizeof(mHdr));
      pSock->Send((unsigned char*)pBuff, mHdr.mNumBytes);
    }

    if(pBuff) free(pBuff);
    pBuff = NULL;
  }

  return(0);
}

