
#include "OAServer.h"

using namespace std;

/****************************************/
OAServer::OAServer(unsigned int mPortNum, char *mDbName, unsigned int mNumThreads) {
/****************************************/

  pSock = new Ethernet(SOCKET_TYPE_TCP_SERVER, (char*)"", mPortNum);
  pDb   = new Database(mDbName);

  SetNumThreads(mNumThreads);
  for(int i = 0; i < mNumThreads; i++) {
    lpHandler.push_back(boost::shared_ptr<HandlerThread>(new HandlerThread(i)));
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
  cout << endl;
  for(std::list< boost::shared_ptr<HandlerThread> >::const_iterator cIter = lpHandler.begin(); cIter != lpHandler.end(); cIter++) {
    cout << (*cIter)->GetThreadPriority() << endl;
  }
  cout << endl;

  return(0);
}


#if 0
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
#endif


/****************************************/
int OAServer::ExecuteSQLQuery(void) {
/****************************************/
  pDb->ExecuteCommand();

  return(0);
}


/****************************************/
/* TODO - I still like having all of the Ethernet functionality within
   the class, but it's getting better */
int OAServer::PollMasterSocket(void) {
/****************************************/
  /* Variable Declarations */
  int mFd = pSock->GetSocketFd();
  Socket_t mSock;
  fd_set mReadFds;

  /* Reset the select variables */
  FD_ZERO(&mReadFds);
  FD_SET(pSock->GetSocketFd(), &mReadFds);

  /* TODO - This needs to have a timeout period so other operations
     on the main thread can occur.  Thread prioritization, housekeeping, etc */
  if((select(mFd + 1, &mReadFds , NULL , NULL , NULL) < 0) && (errno != EINTR)) {
    printf("ERR:  OAServer master socket selection error\n");
  }

  /* If something happened on the master socket, then its an incoming connection */
  if(FD_ISSET(mFd, &mReadFds)) {
    if((mSock.fd = accept(mFd, (struct sockaddr *)&mSock.mAddr, (socklen_t*)&mSock.mAddrLen)) < 0) {
      printf("ERR:  Accepting new socket connection\n");
      return(-1);
    }

    /* Add the incoming socket to the thread with the least utilization */
    std::list< boost::shared_ptr<HandlerThread> >::const_iterator cIter = lpHandler.begin();
    (*cIter)->AddSocket(SOCKET_TYPE_TCP_SERVER, mSock);

    /* Inform the user of new incoming socket number */
    printf("INFO: New connection(%d) %s:%d\n" , mSock.fd, inet_ntoa(mSock.mAddr.sin_addr) , ntohs(mSock.mAddr.sin_port));
    printf("INFO:   Adding socket %d to thread %d\n", mSock.fd, (*cIter)->GetThreadID());
  }


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
    PollMasterSocket();

#if 0
    this->ParseSocketData();
    this->ExecuteSQLQuery();
#endif
  }

  return(0);
}



