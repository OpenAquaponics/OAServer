
#include "OAServer.h"



/****************************************/
OAServer::OAServer(unsigned int mPortNum, char *mDbName, unsigned int mNumThreads) {
/****************************************/

  pSock = new Ethernet(SOCKET_TYPE_TCP_SERVER, (char*)"", mPortNum);
  pDb   = new Database(mDbName);

  SetNumThreads(mNumThreads);
  for(int i = 0; i < mNumThreads; i++) {
    lpHandler.push_back(spHandler(new HandlerThread()));
  }
}


/****************************************/
OAServer::~OAServer(void) {
/****************************************/
  delete pSock;
  delete pDb;

}


/****************************************/
int OAServer::PrintHandlerThread(void) {
/****************************************/
  for(list<spHandler>::const_iterator cIter = lpHandler.begin(); cIter != lpHandler.end(); cIter++) {
    cout << (*cIter)->GetThreadPriority() << endl;
  }

  return(0);
}

/****************************************/
int OAServer::ParseSocketData(void) {
/****************************************/
  PacketHeader_t *pHdr = (PacketHeader_t*)pSock->pData;

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
    case OASERVER_STATISTICS_DATA: {
      PktStatistics_t *pPkt = (PktStatistics_t*)pSock->pData;
      sprintf(pDb->pSQL, 
        "INSERT INTO OANodeData (mNodeId, mSampleTimetag, mVal01, mVal02, mVal03) VALUES ('%d', '%d', '%.2f', '%.2f', '%.2f');",
         0x1234567890ABCDEF, pPkt->mHdr.mTimeTagSec, 123.4, 543.3, -978.23);
      };
      break;
#if 0
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

  if(mVerbose) { printf("%s\n", pDb->pSQL); };

  return(0);
}


/****************************************/
int OAServer::ExecuteSQLQuery(void) {
/****************************************/
  pDb->ExecuteCommand();

  return(0);
}


/****************************************/
int OAServer::Run(void) {
/****************************************/

  lpHandler.sort(SortSharedPtr<HandlerThread>);
  PrintHandlerThread();

  /* Accept the incoming connections and handle the data */
  printf("INFO: OASever running, waiting for remote data to process ...\n");
  while(TRUE) {
    pSock->PollOpenSockets();

    /* The packet header and data are public variables from the class, this is a kludge */
    if(pSock->pData) {
      this->ParseSocketData();
      this->ExecuteSQLQuery();

      /* Converts the seconds timetag into human readable!!! */
      /* SELECT datetime(mSampleTime, 'unixepoch') FROM Statistics; */
    }
  }

  return(0);
}



