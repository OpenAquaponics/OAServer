
#include "OAServer.h"


/****************************************/
OAServer::OAServer(char *mIpAddr, unsigned int mPortNum, char *mDbName) {
/****************************************/

  pSock = new Ethernet(SOCKET_TYPE_TCP_SERVER, mIpAddr, mPortNum);
  pDb   = new Database(mDbName);

}


/****************************************/
OAServer::~OAServer(void) {
/****************************************/
  delete pSock;
  delete pDb;

}


/****************************************/
int OAServer::Run(void) {
/****************************************/

  /* Accept the incoming connections and handle the data */
  printf("Waiting for connections...\n");
  while(TRUE) {
    pSock->PollOpenSockets();

    /* The packet header and data are public variables from the class, this is a kludge */
    if(pSock->pData) {

      //strftime('%s','2004-01-01 02:34:57')
      PktStatistics_t *pStatPkt = (PktStatistics_t*)pSock->pData;
      printf("INSERT INTO Statistics(mNodeId, mSampleTime, mWaterLevel) VALUES ('%d', '%d', '%f')\n", 
              pStatPkt->mHdr.mDeviceId,
              pStatPkt->mHdr.mTimeTagSec,
              pStatPkt->mWaterLevel);

      sprintf(pDb->pSQL, "INSERT INTO Statistics(mNodeId, mSampleTime, mWaterLevel) VALUES ('%d', '%d', '%f')\n", 
              pStatPkt->mHdr.mDeviceId,
              pStatPkt->mHdr.mTimeTagSec,
              pStatPkt->mWaterLevel);

      pDb->ExecuteCommand();

      /* Converts the seconds timetag into human readable!!! */
      /* SELECT datetime(mSampleTime, 'unixepoch') FROM Statistics; */
    }
  }

}



