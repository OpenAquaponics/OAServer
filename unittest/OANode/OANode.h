/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _OANODE_H_
#define _OANODE_H_

/* system includes */
#include "stdlib.h"
#include "time.h"

#include <list>

/* user includes */
#include "Ethernet.h"
#include "Database.h"
#include "Util.h"

/* defines */


/* enums */


/* typedef */

/* namespace */

/* class */
class OANode : public Util {

  private:
    void Init(void);

  public:
    OANode(char *sIpAddr = "127.0.0.1", unsigned int mPortNum = 50000, char *sDbName = "./OANode.sqlite", unsigned int mNumThreads = 2);
   ~OANode(void);

    int Run(void);

    /* GET/SET */
    int SetVerbose(int v) { this->mVerbose = v; };
    int GetVerbose(void)  { return(this->mVerbose); };
    int SetNumThreads(unsigned int t) { this->mNumThreads = t; };
    int GetNumthreads(void)  { return(this->mNumThreads); };


    int Debug(void);

  protected:

  private:
    Ethernet *pSock;
    Database *pDb;

    int mVerbose;
    int mNumThreads; 

  private:
    //std::list< boost::shared_ptr<HandlerThread> > lpHandler;

};

#endif /* _OANODE_H_ */


