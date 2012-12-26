/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/

/* user includes */
#include "HandlerThread.h"

using namespace std;



/****************************************/
int HandlerThread::ExecuteSQLQuery(void) {
/****************************************/
  pDb->ExecuteCommand();

  return(0);
}


/****************************************/
int HandlerThread::ProcessPacket(unsigned char *pData) {
/****************************************/
  PacketHeader_t *pHdr = (PacketHeader_t*)pData;

  /* Sanity check */
  if(!pData) {
    return(-1);
  }

  if(pHdr->mChecksum != ComputeChecksum((int*)&pData[sizeof(PacketHeader_t)], pHdr->mNumBytes)) {
    return(-1);
  }

  //strftime('%s','2004-01-01 02:34:57')
  /* The brackets are needed to stop a 'crosses initialization' error for the pointers */
  switch(pHdr->mMsgType) {
    /* TODO - Make this OAServer::OANODE_ADD_MEASUREMENT */
    case OANODE_ADD_MEASUREMENT:
      break;
    case OANODE_ADD_WATER:
      break;
    case OANODE_ADD_FISH:
      break;
    case OANODE_ADD_PLANT:
      break;
#if 0
    case OASERVER_STATISTICS_DATA: {
      PktStatistics_t *pPkt = (PktStatistics_t*)pSock->pData;
      sprintf(pDb->pSQL, 
        "INSERT INTO OANodeData (mNodeId, mSampleTimetag, mVal01, mVal02, mVal03) VALUES ('%d', '%d', '%.2f', '%.2f', '%.2f');",
         0x1234567890ABCDEF, pPkt->mHdr.mTimeTagSec, 123.4, 543.3, -978.23);
      };
      break;
    case OASERVER_NODE_CONFIG: {
      PktNodeConfig_t *pPkt = (PktNodeConfig_t*)pSock->pData;
      sprintf(pDb->pSQL, 
        "REPLACE INTO NodeConfig (mNodeId, mDescription, mSamplePeriodSecs, mLastUpdate, mNumFish, mNumPlants) VALUES ('%d', '%s', '%d', '%d', '%d', '%d');",
         pPkt->mHdr.mDeviceId, pPkt->mDescription, pPkt->mHdr.mTimeTagSec,
         pPkt->mLastUpdate, pPkt->mNumFish, pPkt->mNumPlants);
      };
      break;
    case OASERVER_ACCOUNTING: {
      PktAccounting_t *pPkt = (PktAccounting_t*)pSock->pData;
      sprintf(pDb->pSQL, 
        "INSERT INTO Accounting (mDate, mDescription, mAmount) VALUES ('%d', '%s', '%.2f');",
         pPkt->mDate, pPkt->mDescription, pPkt->mAmount);
      };
      break;
#endif
    default: break;
  }

//  if(mVerbose) { printf("%s\n", pDb->pSQL); };

  return(0);
}



