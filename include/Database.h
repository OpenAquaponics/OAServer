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
#include <string.h>
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
    int OpenDatabase(void);
    int CloseDatabase(void);

  public:
    Database(void);
   ~Database(void);

    int ExecuteCommand();
    int ExecuteCommand(char *mCmd); /* TODO : - Add callback pointer */

    char pSQL[512]; /* TODO - This definitely needs to change!! */

  protected:


  private:
    const char *mDbName;
    sqlite3 *mDbPtr;
    char *mDbErrMsg;

};

#endif /* _DATABASE_H_ */
