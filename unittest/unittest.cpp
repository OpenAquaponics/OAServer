
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

  lpClients.erase(lpClients.begin(), lpClients.end());

  /* Have the sockets send a bunch of data */

  return(0);
}



