
#include <list>
#include <iostream>
#include <fstream>
#include "OANode.h"

#include "json/json.h"

using namespace std;

#define CLIENT_CNT  (30)

/****************************************/
int main(int argc, char **argv) {
/****************************************/
  /* Variable Declaration */

  printf("INFO: Reading JSON files\n");
  Json::Value root;
  Json::Reader reader;
  Json::Value::Members members;
  ifstream infile;
  infile.open("./OANode.json", ifstream::in);
  if(!reader.parse(infile, root, false)) {
    cout << "Failed to parse JSON file: " << reader.getFormatedErrorMessages() << endl;
    return(0);
  }

  std::cout << root.toStyledString();

  /* Build OANodeCfg */
  if(root.isMember("OANodeCfg")) {
    char param[][32] = {{"mAcctHash"}, {"mSystemId"}, {"mDeviceId"}, {"mOAServerIp"}, {"mOAServerPort"}, {"Hello"}};
    for(int i = 0; i < 6; i++) {
      if(!root["OANodeCfg"].isMember(param[i])) {
        cout << "ERR: Missing required param: " << param[i] << endl;
      }
    }

    /* OANodeCfg mCfg = new OANodeCfg(); */
    for(int i = 0; i < 5; i++) {
      cout << "Extracted: " << root["OANodeCfg"].get(param[i], "").asString() << endl;
    }
  }

#if 0
  printf("INFO: Spawning the OANodes\n");
  std::list< boost::shared_ptr<OANode> > lpClients;
  for(int i = 0; i < CLIENT_CNT; i++) {
    lpClients.push_back(boost::shared_ptr<OANode>(new OANode((char*)"127.0.0.1", 50000, (char*)"", 2)));
  }

  for(int i = 0; i < 10; i++) {
    for(std::list< boost::shared_ptr<OANode> >::iterator pIter = lpClients.begin(); pIter != lpClients.end(); pIter++) {
      (*pIter).get()->Debug();
    }
  }

  lpClients.erase(lpClients.begin(), lpClients.end());
#endif



  return(0);
}



