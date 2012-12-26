/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/

/* user includes */
#include "OANode.h"

using namespace std;


/****************************************/
OANodeCfg::OANodeCfg(std::string sFilename) {
/****************************************/
  LoadJSONFile(sFilename);
}


/****************************************/
int OANodeCfg::LoadJSONFile(std::string sFilename) {
/****************************************/
  cout << "INFO: Reading JSON files\n";
  Json::Value root;
  Json::Reader reader;
  Json::Value::Members members;
  ifstream infile;
  infile.open(sFilename.c_str(), ifstream::in);
  if(!reader.parse(infile, root, false)) {
    cout << "Failed to parse JSON file: " << reader.getFormatedErrorMessages() << endl;
    /* TODO - Load some default vales */
  }
  else {

    std::cout << root.toStyledString();
 
    /* Build OANodeCfg */
    if(root.isMember(sRootName)) {
      for(int i = 0; i < 5; i++) {
        if(!root[sRootName].isMember(sOANodeCfgReqParam[i])) {
          cout << "ERR: Missing required parameter: " << sOANodeCfgReqParam[i] << endl;
        }
      }
 
      /* OANodeCfg mCfg = new OANodeCfg(); */
      for(int i = 0; i < 5; i++) {
        cout << "Extracted: " << root[sRootName].get(sOANodeCfgReqParam[i], "").asString() << endl;
      }
    }
  }

}



