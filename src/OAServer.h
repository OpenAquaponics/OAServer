/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _OASERVER_H_
#define _OASERVER_H_

/* system includes */


/* user includes */
#include "Ethernet.h"
#include "Database.h"

/* defines */


/* enums */


/* typedef */


/* class */
class OAServer {

  private:

  public:
    OAServer(char *mIpAddr, unsigned int mPortNum, char *mDbName);
   ~OAServer(void);

    int Run(void);

  protected:

  private:
    Ethernet *pSock;
    Database *pDb;

};

#endif /* _OASERVER_H_ */
