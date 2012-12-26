/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/

/* user includes */
#include "OANode.h"

using namespace std;


/****************************************/
void OANodeCfg::Init(void) {
/****************************************/
  /* Root JSON object name */
  sRootName = "OANodeCfg";

  static const json_param_t tOptions[] = {
    { (void*)&sAcctHash,       "mAcctHash",       "TESTING",   Json::stringValue,  TRUE},
    { (void*)&mSystemId,       "mSystemId",       "1234567",   Json::uintValue,    TRUE},
    { (void*)&mDeviceId,       "mDeviceId",       "2987423",   Json::uintValue,    TRUE},
    { (void*)&sOAServerIp,     "sOAServerIp",     "127.0.0.1", Json::stringValue,  TRUE},
    { (void*)&mOAServerPort,   "mOAServerPort",   "50000",     Json::uintValue,    TRUE},
    { (void*)&sName,           "sName",           "Indoor",    Json::stringValue,  FALSE},
    { (void*)&mSampleTimeSecs, "mSampleTimeSecs", "10",        Json::uintValue,    FALSE},
    { (void*)&sDescription,    "sDescription",    "",          Json::stringValue,  FALSE},
    { (void*)&bSaveFile,       "bSaveFile",       "1",         Json::booleanValue, FALSE},
  };
  #define NUM_PARAMS    (sizeof(tOptions) / sizeof(json_param_t))

  /* Load the parameters into a vector */
  for(int i = 0; i < NUM_PARAMS; i++) {
    vsParams.push_back(tOptions[i]);
  }
}


/****************************************/
OANodeCfg::OANodeCfg(std::string sFilename) {
/****************************************/
  Init();

  LoadJSONFile(sFilename);
}


/****************************************/
int OANodeCfg::LoadJSONFile(std::string sFilename) {
/****************************************/
  /* Variable Declaration */
  Json::Reader reader;
  Json::Value::Members members;
  ifstream infile;

  cout << "INFO: Building OANodeCfg from JSON file: " << sFilename << endl;
  infile.open(sFilename.c_str(), ifstream::in);
  if(!reader.parse(infile, root, false)) {
    cout << "ERR:  Failed to parse JSON file: " << reader.getFormatedErrorMessages() << endl;
    /* TODO - Load some default vales */
  }
  else {
    LoadParams();
  }

}


/****************************************/
int OANodeCfg::LoadParams(void) {
/****************************************/
  /* Build the OANodeCfg object */
  if(root.isMember(sRootName)) {
    /* Sanity check on the required parameters */
    for(vector<json_param_t>::iterator pIter = vsParams.begin(); pIter < vsParams.end(); pIter++) {
      if((*pIter).bRequired && !root[sRootName].isMember((*pIter).sParamName)) {
        cout << "ERR:  Missing required parameter: " << (*pIter).sParamName << endl;
        exit(1);
      }
    }
 
    /* Load all of the parameters */
    for(vector<json_param_t>::iterator pIter = vsParams.begin(); pIter < vsParams.end(); pIter++) {

      switch((*pIter).eType) {
        case Json::stringValue:
          {
          string *p = (string*)((*pIter).pVariable);
          *p = root[sRootName].get((*pIter).sParamName, (*pIter).sDfltValue).asString();
          }
          break;
        case Json::uintValue:
          {
          unsigned int *p = (unsigned int*)((*pIter).pVariable);
          *p = atoi(root[sRootName].get((*pIter).sParamName, (*pIter).sDfltValue).asString().c_str());
          }
          break;
        case Json::booleanValue:
          {
          bool *p = (bool*)((*pIter).pVariable);
          *p = root[sRootName].get((*pIter).sParamName, atoi((*pIter).sDfltValue)).asBool();
          }
          break;
        default:
          cout << "ERR:  Unhandled JSON parsing type for parameter: " << (*pIter).sParamName << endl;
          break;
      }

      cout << "INFO:   Extracted " << (*pIter).sParamName << ": " << root[sRootName].get((*pIter).sParamName, "").asString() << endl;
    }
  }
}

