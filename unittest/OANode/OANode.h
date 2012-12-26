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
#include <list>
#include <vector>

/* user includes */
#include "Ethernet.h"
#include "Database.h"
#include "OASensor.h"
#include "Util.h"

#include "json/json.h"

/* defines */


/* enums */


/* typedef */

/* namespace */

/* global const */

const std::string sRootName = "OANodeCfg";
const std::string sOANodeCfgReqParam[] = {"mAcctHash", "mSystemId", "mDeviceId", "mOAServerIp", "mOAServerPort"};


/* class */
class OANodeCfg {

  public:
    OANodeCfg(std::string sFilename);
   ~OANodeCfg(void);

    int LoadJSONFile(std::string sFilename);

  protected:
    void SetSystemId(int id) { this->mSystemId = id; };
    void SetDeviceId(int id) { this->mDeviceId = id; };
    void SetOAServerIp(std::string s) { this->sOAServerIp = s; };
    void SetOAServerPort(int p) { this->mOAServerPort = p; };

    int GetSystemId(void) { return(mSystemId); };
    int GetDeviceId(void) { return(mDeviceId); };
    std::string GetOAServerIp(void) { return(sOAServerIp); };
    int GetOAServerPort(void) { return(mOAServerPort); };

  private:
    int mSystemId;
    int mDeviceId;
    std::string sOAServerIp;
    int mOAServerPort;

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

  private:
    OANodeCfg *pOANodeCfg;
    Ethernet  *pSock;
    Database  *pDb;

    int mVerbose;
    int mNumThreads; 

  private:
    //std::list< boost::shared_ptr<HandlerThread> > lpHandler;

};

#endif /* _OANODE_H_ */


