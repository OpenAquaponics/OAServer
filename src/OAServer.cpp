
#include "OAServer.h"

using namespace std;

/****************************************/
OAServer::OAServer(unsigned int mPortNum, char *mDbName, unsigned int mNumThreads) {
/****************************************/

  pSock = new Ethernet(SOCKET_TYPE_TCP_SERVER, (char*)"", mPortNum);
  pDb   = new Database(mDbName);

  SetNumThreads(mNumThreads);
  for(int i = 0; i < mNumThreads; i++) {
    lpHandler.push_back(boost::shared_ptr<HandlerThread>(new HandlerThread()));
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
    cout << (*cIter)->GetThreadPID() << "  " << (*cIter)->GetThreadPriority() << endl;
  }
  cout << endl;

  return(0);
}


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
  struct timeval tv = {1, 0};

  /* Reset the select variables */
  FD_ZERO(&mReadFds);
  FD_SET(pSock->GetSocketFd(), &mReadFds);

  /* TODO - This needs to have a timeout period so other operations
     on the main thread can occur.  Thread prioritization, housekeeping, etc */
  if((select(mFd + 1, &mReadFds, NULL, NULL, &tv) < 0) && (errno != EINTR)) {
    printf("ERR:  OAServer master socket selection error\n");
  }

  /* If something happened on the master socket, then its an incoming connection */
  if(FD_ISSET(mFd, &mReadFds)) {
    if((mSock.fd = accept(mFd, (struct sockaddr *)&mSock.mAddr, (socklen_t*)&mSock.mAddrLen)) < 0) {
      printf("ERR:  Accepting new socket connection\n");
      return(-1);
    }

    /* Add the incoming socket to the thread with the least utilization */
    lpHandler.sort(SortSharedPtr<HandlerThread>);
    std::list< boost::shared_ptr<HandlerThread> >::const_iterator cIter = lpHandler.begin();
    (*cIter)->AddSocket(SOCKET_TYPE_TCP_SERVER, mSock);

    /* Inform the user of new incoming socket number */
    printf("INFO: New connection(%d) %s:%d\n" , mSock.fd, inet_ntoa(mSock.mAddr.sin_addr) , ntohs(mSock.mAddr.sin_port));
    printf("INFO:   Adding socket %d to thread %u\n", mSock.fd, (*cIter)->GetThreadPID());
  }

  return(0);
}


/****************************************/
int OAServer::Run(void) {
/****************************************/

  /* Accept the incoming connections and handle the data */
  printf("INFO: OAServer running, waiting for remote data to process ...\n");
  while(TRUE) {
    PollMasterSocket();

#if 0
    this->ParseSocketData();
    this->ExecuteSQLQuery();
#endif
  }

  return(0);
}



