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
  sRootNameCfg    = "OANodeCfg";
  sRootNameSensor = "OASensor";

  static const JSON_PARAM_t tOptions[] = {
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
  #define NUM_PARAMS    (sizeof(tOptions) / sizeof(JSON_PARAM_t))

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
  ifstream infile;

  infile.open(sFilename.c_str(), ifstream::in);
  if(!reader.parse(infile, root, false)) {
    cout << "ERR:  Failed to parse JSON file: " << reader.getFormatedErrorMessages() << endl;
    /* TODO - Load some default vales */
  }
  else {
    cout << "INFO: Building OANodeCfg from JSON file: " << sFilename << endl;
    LoadParams();
    cout << "INFO: Building OASensor from JSON file: " << sFilename << endl;
    LoadSensors();
  }
  infile.close();
}


/****************************************/
int OANodeCfg::LoadParams(void) {
/****************************************/
  /* Build the OANodeCfg object */
  if(root.isMember(sRootNameCfg)) {
    /* Sanity check on the required parameters */
    for(vector<JSON_PARAM_t>::iterator pIter = vsParams.begin(); pIter < vsParams.end(); pIter++) {
      if((*pIter).bRequired && !root[sRootNameCfg].isMember((*pIter).sParamName)) {
        cout << "ERR:  OANodeCfg missing required parameter: " << (*pIter).sParamName << endl;
        exit(1);
      }
    }
 
    /* Load all of the parameters */
    for(vector<JSON_PARAM_t>::iterator pIter = vsParams.begin(); pIter < vsParams.end(); pIter++) {

      switch((*pIter).eType) {
        case Json::stringValue:
          {
          string *p = (string*)((*pIter).pVariable);
          *p = root[sRootNameCfg].get((*pIter).sParamName, (*pIter).sDfltValue).asString();
          }
          break;
        case Json::uintValue:
          {
          unsigned int *p = (unsigned int*)((*pIter).pVariable);
          *p = atoi(root[sRootNameCfg].get((*pIter).sParamName, (*pIter).sDfltValue).asString().c_str());
          }
          break;
        case Json::booleanValue:
          {
          bool *p = (bool*)((*pIter).pVariable);
          *p = root[sRootNameCfg].get((*pIter).sParamName, atoi((*pIter).sDfltValue.c_str())).asBool();
          }
          break;
        default:
          cout << "ERR:  Unhandled JSON parsing type for parameter: " << (*pIter).sParamName << endl;
          break;
      }

      cout << "INFO:   Extracted " << (*pIter).sParamName << ": " << root[sRootNameCfg].get((*pIter).sParamName, "").asString() << endl;
    }
  }
}


/****************************************/
int OANodeCfg::LoadSensors(void) {
/****************************************/
  /* Build the OANodeCfg object */
  if(root.isMember(sRootNameSensor)) {
    /* Variable Declaration */
    unsigned int mType;
    Json::Value mArray = root[sRootNameSensor];

    for(int i = 0; i < mArray.size(); i++) {
      if(!mArray[i].isMember("mType")) {
        cout << "ERR:  OASensor mssing required parameter: mType\n";
      }
      else if(i >= MAX_NUM_OASENSOR) {
        cout << "ERR:  OASensor reached max sensor list size: " << MAX_NUM_OASENSOR << ". Uanble to add: " << mArray[i].get("sName", "").asString() << endl;
      }
      else {
        switch(mType = atoi(mArray[i].get("mType", "").asString().c_str())) {
          case OASENSOR_AI:
            {
            vpOASensor.push_back(new SensorAI(
              mArray[i].get("sName", "").asString(),
              mType,
              (unsigned int)atoi(mArray[i].get("mPin", "").asString().c_str()),
              mArray[i].get("sUnits", "").asString(),
              mArray[i].get("sDescription", "").asString()
            ));
            }
            break;
          case OASENSOR_DO:
            vpOASensor.push_back(new SensorDO(
              mArray[i].get("sName", "").asString(),
              mType,
              (unsigned int)atoi(mArray[i].get("mPin", "").asString().c_str()),
              mArray[i].get("sUnits", "").asString(),
              mArray[i].get("sDescription", "").asString()
            ));
            break;
          default:
            cout << "ERR:  Unhandled JSON parsing type for parameter: mType == " << mType << endl;
            break;
        }

        cout << "INFO:   Extracted " << vpOASensor[i]->GetName() << " : " << vpOASensor[i]->GetDescription() << endl;
      }
    }
  }

}


/****************************************/
std::string OANodeCfg::SampleSensors(void) {
/****************************************/
  /* Variable Declaration */
  Json::FastWriter mWrite;
  Json::Value vMsg;
  Json::Value vSample(Json::arrayValue);
  Json::Value vUnits(Json::arrayValue);

  /* Poll all of the sensors */
  for(int i = 0; i < vpOASensor.size(); i++) {
    switch(vpOASensor[i]->GetType()) {
      case OASENSOR_AI:
        /* Can this be 2 significant digits?? */
        vSample.append(((SensorAI*)vpOASensor[i])->ProcessSensor());
        vUnits.append(vpOASensor[i]->GetUnits());
        break;
      case OASENSOR_DO:
        vSample.append(((SensorDO*)vpOASensor[i])->ProcessSensor());
        vUnits.append(vpOASensor[i]->GetUnits());
        break;
      default:
        {
        static int print = vpOASensor.size();
        if(print) {
          cout << "INFO: OANodeCfg->Step() unhandled sensor type: " << vpOASensor[i]->GetType() << endl;
          print--;
        }
        }
    }
  }

  /* Build the JSON message */
  vMsg[sRootNameCfg]["mSystemId"] = GetSystemId();
  vMsg[sRootNameCfg]["mDeviceId"] = GetDeviceId();
  vMsg[sRootNameSensor]["Sample"] = vSample;
  vMsg[sRootNameSensor]["Units"]  = vUnits;
  //cout << vMsg << endl;

  return(mWrite.write(vMsg));
}



