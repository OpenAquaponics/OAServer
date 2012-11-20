
#include "Database.h"

/****************************************/
int Database::Init(void) {
/****************************************/

  mDbName = "./temp.db";
  mDbPtr = NULL;
  mDbErrMsg = NULL;
  memset(pSQL, 0, sizeof(pSQL));

  return(0);
}


/****************************************/
int Database::OpenDatabase(void) {
/****************************************/
  /* Variable Declaration */
  int mRetVal = 0;

  /* TODO - Need to check if the database exists, and if not, create a new one */
  /* CreateDatabase() */

  /* Attempt to open the database */
  if(mRetVal = sqlite3_open(mDbName, &mDbPtr)) {
    printf("ERR: Failed to open database: %s\n", mDbName);
    exit(-1);
  }

  return(0);
}


/****************************************/
int Database::CloseDatabase(void) {
/****************************************/

  if(mDbPtr) {
    sqlite3_close(mDbPtr);
    mDbPtr = NULL;
  }

  return(0);
}

