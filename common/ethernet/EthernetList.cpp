
#include "Ethernet.h"

/****************************************/
int EthernetList::InitList(void) {
/****************************************/
  /* Init the select maximum socket number */
  mMaxSelectNum = 0;
  FD_ZERO(&mListFds);

  return(0);
}


/****************************************/
EthernetList::EthernetList(void) {
/****************************************/

  InitList();
}


/****************************************/
EthernetList::~EthernetList(void) {
/****************************************/

}


/****************************************/
int EthernetList::AddSocket(SOCKET_TYPE_e mSockType, Socket_t mNewSock) {
/****************************************/
  /* Variable Declaration */
  int mSize = vpSockList.size();

  /* Push the incoming socket into the vector and perform housekeeping */  
  vpSockList.push_back(boost::shared_ptr<Ethernet>(new Ethernet(mSockType, mNewSock)));

  /* Verify the new socket was properly added */
  if(vpSockList.size() == mSize + 1) {
    boost::shared_ptr<Ethernet> pLast = vpSockList.back();
 
    /* Save the max select number */
    if(pLast.get()->GetSocketFd() > mMaxSelectNum) {
      mMaxSelectNum = pLast.get()->GetSocketFd();
    }
 
    /* Preload the fd list for the select */
    FD_SET(pLast.get()->GetSocketFd(), &mListFds);

    return(0);
  }

  /* TODO - How you handle this error? */
  printf("ERR:  Failed to add incoming socket to vector list\n");
  return(-1);
}


/****************************************/
/* TODO - This function could be optimized better for speed */
int EthernetList::RemoveSocket(int mFd) {
/****************************************/

  /* INFO: Interesting note is that you have to assign the
      iterator when performing an erase and you shouldn't increment
      everytime like a normal for loop. Still don't know why exactly. */

  /* Traverse the vector to find the matching fd */
  for(std::vector< boost::shared_ptr<Ethernet> >::iterator pIter = vpSockList.begin(); pIter != vpSockList.end(); ) {
    /* Found the matching socket */
    if((*pIter).get()->GetSocketFd() == mFd) {

      pIter = vpSockList.erase(pIter);

      /* Reset the select variables */
      FD_ZERO(&mListFds);
      for(int i = 0; i < vpSockList.size(); i++) {
        FD_SET(vpSockList[i].get()->GetSocketFd(), &mListFds);
      }

      return(0);
    }
    else {
      pIter++;
    }
  }

  printf("ERR:  Failed to find matching socket: %d\n", mFd);
  return(-1);
}


/****************************************/
int EthernetList::PollSocketList(void) {
/****************************************/
  /* Variable Declaration */
  int i = 0, s = 0;
  int mRetVal = 0;
  fd_set   mReadFds;
  struct timeval tv = {1, 0};

  /* Wait for activity on one of the sockets , timeout is NULL , so wait indefinitely */
  mReadFds = mListFds;
  if((select(mMaxSelectNum + 1, &mReadFds, NULL, NULL, &tv) < 0) && (errno != EINTR)) {
    printf("ERR:  Selection error\n");
  }

  /* Process all of the remaining sockets for any data */
  for(i = 0; i < vpSockList.size(); i++) {
    if(FD_ISSET(vpSockList[i].get()->GetSocketFd(), &mReadFds)) {
      ProcessData(vpSockList[i].get());
    }
  }

  return(0);
}



