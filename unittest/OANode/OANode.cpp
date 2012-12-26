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
int OANode::Step(void) {
/****************************************/
  /* Step OANode by sampling the data and sending it over the socket */
  if(pOANodeCfg) {
    /* Variable Declaration */
    unsigned char  *pBuff = NULL;
    PacketHeader_t *pHdr  = NULL;
    char *pStr  = NULL;
    std::string sSampleMsg = pOANodeCfg->SampleSensors(); /* Poll the sensors */
    int mNumBytes = sSampleMsg.size() + sizeof(PacketHeader_t) + 1;

    /* Build the packet to be written */
    if((pBuff = (unsigned char*)malloc(mNumBytes))) {
      /* Load pointers */
      pHdr = (PacketHeader_t*)pBuff;
      pStr = (char*)&pBuff[sizeof(PacketHeader_t)];

      /* Copy JSON sample data and build header */
      memcpy(pStr, sSampleMsg.c_str(), sSampleMsg.size() + 1);
      pHdr->mSync       = SYNC;
      pHdr->mNumBytes   = mNumBytes - sizeof(PacketHeader_t);
      pHdr->mTimeTagSec = 0; /* Add system time NTP */
      pHdr->mDeviceId   = pOANodeCfg->GetDeviceId();
      pHdr->mMsgType    = 123; /* Create a message type name */
      pHdr->mChecksum   = ComputeChecksum((int*)pStr, pHdr->mNumBytes);

      /* Send data over the socket */
      pSock->Send((unsigned char*)pBuff, mNumBytes);

      free(pBuff);
    }
  }

  return(0);
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

#if 0
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
#endif

  return(0);
}

