
#include "OAServer.h"


/****************************************/
OAServer::OAServer(char *mIpAddr, unsigned int mPortNum, char *mDbName) {
/****************************************/

  pSock = new Ethernet(SOCKET_TYPE_TCP_SERVER, mIpAddr, mPortNum);
  pDb   = new Database(mDbName);

  mVerbose = TRUE;
}


/****************************************/
OAServer::~OAServer(void) {
/****************************************/
  delete pSock;
  delete pDb;

}


/****************************************/
int OAServer::ParseSocketData(void) {
/****************************************/
  PacketHeader_t *pHdr = (PacketHeader_t*)pSock->pData;

  //strftime('%s','2004-01-01 02:34:57')
  /* The brackets are needed to stop a 'crosses initialization' error for the pointers */
  switch(pHdr->mMsgType) {
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
        "INSERT INTO Statistics (mNodeId, mSampleTime, mTempAirIndoor, mTempAirOutdoor, mTempWater, mHumidityIndoor, mHumidityOutdoor, mWaterLevel, mBattaryVoltage, mSolarPanelVoltage) VALUES ('%d', '%d', '%.2f', '%.2f', '%.2f', '%.2f', '%.2f', '%.2f', '%.2f', '%.2f');",
         pPkt->mHdr.mDeviceId, pPkt->mHdr.mTimeTagSec, pPkt->mTempAirIndoor,
         pPkt->mTempAirOutdoor, pPkt->mTempWater, pPkt->mHumidityIndoor,
         pPkt->mHumidityOutdoor, pPkt->mWaterLevel, pPkt->mBattaryVoltage,
         pPkt->mSolarPanelVoltage);
      break; }
    case OASERVER_NODE_CONFIG: {
      PktNodeConfig_t *pPkt = (PktNodeConfig_t*)pSock->pData;
      sprintf(pDb->pSQL, 
        "REPLACE INTO NodeConfig (mNodeId, mDescription, mSamplePeriodSecs, mLastUpdate, mNumFish, mNumPlants) VALUES ('%d', '%s', '%d', '%d', '%d', '%d');",
         pPkt->mHdr.mDeviceId, pPkt->mDescription, pPkt->mHdr.mTimeTagSec,
         pPkt->mLastUpdate, pPkt->mNumFish, pPkt->mNumPlants);
      break; }
    case OASERVER_ACCOUNTING: {
      PktAccounting_t *pPkt = (PktAccounting_t*)pSock->pData;
      sprintf(pDb->pSQL, 
        "INSERT INTO Accounting (mDate, mDescription, mAmount) VALUES ('%d', '%s', '%.2f');",
         pPkt->mDate, pPkt->mDescription, pPkt->mAmount);
      break; }
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



