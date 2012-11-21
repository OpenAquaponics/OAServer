
#include "Database.h"

/****************************************/
static int callback(void *NotUsed, int argc, char **argv, char**szColName) {
/****************************************/
  for(int i = 0; i < argc; i++) {
    printf("%s = %s\n", szColName[i], argv[i]);
  }
  printf("\n");
  return(0);
}


/****************************************/
Database::Database(char *mDbName) {
/****************************************/
  Init();

  sprintf(this->mDbName, "%s", mDbName);
  OpenDatabase();
}


/****************************************/
Database::~Database(void) {
/****************************************/
  CloseDatabase();
}


/****************************************/
int Database::ExecuteCommand() {
/****************************************/
  if(sqlite3_exec(mDbPtr, pSQL, callback, 0, &mDbErrMsg)) {
    printf("ERR: SQL Error: %s\n", mDbErrMsg);
    sqlite3_free(mDbErrMsg);
  }

  return(0);
}



