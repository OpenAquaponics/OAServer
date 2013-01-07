
#include <list>
#include "Ethernet.h"

using namespace std;

#define CLIENT_CNT  (10)

/****************************************/
int main(int argc, char **argv) {
/****************************************/
  /* Variable Declaration */
  std::list< boost::shared_ptr<Ethernet> > lpClients;

  printf("INFO: Spawning the client sockets\n");
  for(int i = 0; i < CLIENT_CNT; i++) {
    lpClients.push_back(boost::shared_ptr<Ethernet>(new Ethernet(SOCKET_TYPE_TCP_CLIENT, (char*)"127.0.0.1", 50000)));
  }

  lpClients.erase(lpClients.begin(), lpClients.end());

  /* Have the sockets send a bunch of data */

  return(0);
}



