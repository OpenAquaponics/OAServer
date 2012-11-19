/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _DATABASE_H_
#define _DATABASE_H_

/* system includes */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

#include "sqlite3.h"

/* user includes */


/* defines */


/* enums */


/* typedef */


/* class */
class Database {

  private:
    int Init(void);
    int InitDatabse(void);
    int CloseDatabase(void);

  public:
    Database(void);
   ~Database(void);

  protected:


  private:


};

#endif /* _DATABASE_H_ */
