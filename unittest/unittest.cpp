
#include <list>
#include "OANode.h"

using namespace std;

#define CLIENT_CNT  (30)

/****************************************/
int main(int argc, char **argv) {
/****************************************/
  /* Variable Declaration */
  std::list< boost::shared_ptr<OANode> > lpClients;

  printf("INFO: Spawning the OANodes\n");
  for(int i = 0; i < CLIENT_CNT; i++) {
    lpClients.push_back(boost::shared_ptr<OANode>(new OANode((char*)"127.0.0.1", 50000, (char*)"", 2)));
  }

  for(int i = 0; i < 10; i++) {
    for(std::list< boost::shared_ptr<OANode> >::iterator pIter = lpClients.begin(); pIter != lpClients.end(); pIter++) {
      (*pIter).get()->Debug();
    }
  }

#if 0
  /* Traverse the vector to find the matching fd */
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
#endif


  lpClients.erase(lpClients.begin(), lpClients.end());

  /* Have the sockets send a bunch of data */

  return(0);
}



