/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _OASERVER_H_
#define _OASERVER_H_

/* system includes */


/* user includes */
#include "HandlerThread.h"
#include "Ethernet.h"
#include "Database.h"

/* defines */


/* enums */


/* typedef */

/* namespace */

/* class */
class OAServer {

  private:

  public:
    OAServer(unsigned int mPortNum = 50000, char *mDbName = "./OAServer.sqlite", unsigned int mNumThreads = 2);
   ~OAServer(void);

    int Run(void);
    int ParseSocketData(void);
    int ExecuteSQLQuery(void);
    int PollMasterSocket(void);

    int PrintHandlerThread(void);

    /* GET/SET */
    int SetVerbose(int v) { this->mVerbose = v; };
    int GetVerbose(void)  { return(this->mVerbose); };
    int SetNumThreads(unsigned int t) { this->mNumThreads = t; };
    int GetNumthreads(void)  { return(this->mNumThreads); };



  protected:

  private:
    Ethernet *pSock;
    Database *pDb;

    int mVerbose;
    int mNumThreads; 

  private:
    std::list< boost::shared_ptr<HandlerThread> > lpHandler;

};

#endif /* _OASERVER_H_ */
