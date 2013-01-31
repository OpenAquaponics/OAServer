/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _OANODE_H_
#define _OANODE_H_

/* system includes */
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <vector>

#include <boost/lexical_cast.hpp>

/* user includes */
#include "Ethernet.h"
#include "Database.h"
#include "OASensor.h"
#include "Util.h"

#include "json/json.h"

/* defines */


/* enums */


/* typedef */
typedef struct {
  void* pVariable;
  std::string sParamName;
  std::string  sDfltValue;
  Json::ValueType eType;
  bool  bRequired;
} JSON_PARAM_t;


/* namespace */

/* global const */


/* class */
class OANodeCfg {

  private:
    void Init(void);

  public:
    OANodeCfg(std::string sFilename);
   ~OANodeCfg(void);

    int  LoadJSONFile(std::string sFilename);
    void PrintJSONFile(void) { std::cout << root.toStyledString(); };
    std::string SampleSensors(void);

    void SetSystemId(unsigned int id) { this->mSystemId = id; };
    void SetDeviceId(unsigned int id) { this->mDeviceId = id; };
    void SetOAServerIp(std::string s) { this->sOAServerIp = s; };
    void SetOAServerPort(unsigned int p) { this->mOAServerPort = p; };

    unsigned int GetSystemId(void) { return(mSystemId); };
    unsigned int GetDeviceId(void) { return(mDeviceId); };
    std::string  GetOAServerIp(void) { return(sOAServerIp); };
    unsigned int GetOAServerPort(void) { return(mOAServerPort); };

  protected:
    int  LoadParams(void);
    int  LoadSensors(void);

  private:
    std::string  sAcctHash;
    unsigned int mSystemId;
    unsigned int mDeviceId;
    std::string  sOAServerIp;
    unsigned int mOAServerPort;

    std::string  sName;
    unsigned int mSampleTimeSecs;
    std::string  sDescription;
    bool bSaveFile;

  private: 
    Json::Value root;

    std::string sRootNameCfg;
    std::string sRootNameSensor;

    std::vector<JSON_PARAM_t> vsParams;
    std::vector<OASensor*> vpOASensor;

};


class OANode : public Util {

  private:
    void Init(void);

  public:
    OANode(char *sIpAddr = "127.0.0.1", unsigned int mPortNum = 50000, char *sDbName = "./OANode.sqlite", unsigned int mNumThreads = 2);
   ~OANode(void);


    int Run(void);

    /* GET/SET */
    void SetVerbose(int v) { this->mVerbose = v; };
    void SetNumThreads(unsigned int t) { this->mNumThreads = t; };
    int  GetVerbose(void)  { return(this->mVerbose); };
    int  GetNumthreads(void)  { return(this->mNumThreads); };


    int Debug(void);

  protected:
    int Step(void);
//    std::string BuildJSONMessage(int mType);

  private:
    OANodeCfg *pOANodeCfg;
    Ethernet  *pSock;

    int mVerbose;
    int mNumThreads; 

  private:
    //std::list< boost::shared_ptr<HandlerThread> > lpHandler;

};

#endif /* _OANODE_H_ */


