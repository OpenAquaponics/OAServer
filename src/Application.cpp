

#include "OAServer.h"


#if 0

int OAServerMain(void) {
  /* Variable Declaration */
  int32_t  mRetVal = 0;
  char     mIpAddr[] = "127.0.0.1";
  uint32_t mPortNum = 4335;
  Ethernet *mServerSocket = NULL;
  Ethernet *mClientSocket = NULL;
  sqlite3 *mDbPtr = NULL;

  if((mServerSocket = new Ethernet(SOCKET_TYPE_UDP_SERVER, mIpAddr, mPortNum)) == NULL) {
    printf("ERR: Creating server socket: %s:%d\n", mIpAddr, mPortNum);
    exit(-1);
  }

  if((mClientSocket = new Ethernet(SOCKET_TYPE_UDP_CLIENT, mIpAddr, mPortNum)) == NULL) {
    printf("ERR: Creating client socket: %s:%d\n", mIpAddr, mPortNum);
    exit(-1);
  }

  char xmitStr[] = "Hello World\n";
  char recvStr[256];

  memset(recvStr, 0, sizeof(recvStr));
  mClientSocket->XmitData((unsigned char*)xmitStr, sizeof(xmitStr));
  mServerSocket->RecvData((unsigned char*)recvStr, sizeof(xmitStr));

  printf("%s\n", recvStr); 

  if(mServerSocket) {
    delete mServerSocket;
  }

  if(mClientSocket) {
    delete mClientSocket;
  }

  if(mRetVal = sqlite3_open("temp.db", &mDbPtr)) {
    printf("ERR: Failed to open database\n");
    exit(-1);
  }

  char *mDbErrMsg = NULL;
  const char *pSQL[4];
  pSQL[0] = "CREATE TABLE Employee(Firstname varchar(30), Lastname varchar(30), Age smallint)";
  pSQL[1] = "INSERT INTO Employee(Firstname, Lastname, Age) VALUES ('Woody', 'Alan', 45)";
  pSQL[2] = "INSERT INTO Employee(Firstname, Lastname, Age) VALUES ('Micheal', 'Bay', 38)";
  pSQL[3] = "SELECT * FROM Employee";


  for(int i = 0; i < 4; i++) {
    if(sqlite3_exec(mDbPtr, pSQL[i], callback, 0, &mDbErrMsg)) {
      std::cout << "Sql Error: " << mDbErrMsg << std::endl;
      sqlite3_free(mDbErrMsg);
      break;
    }
  }

  if(mDbPtr) {
    sqlite3_close(mDbPtr);
  }

}


int main(int argc, char *argv[]) {

  OAServerMain();

  return(0);
}


#endif



#if 1




typedef struct {
  int mPortNum;
  char sDbName[256];
  int mNumThreads;
} CommandLine_t;

CommandLine_t mCmdLine;



/****************************************/
int PrintMenu(void) {
/****************************************/
  printf("Usage ./OAServer <--port 50000> <--db ./OAServer.sqlite> <--threads 10>\n");
  printf("  --port <num>    : Port you would like the server to listen on\n");
  printf("  --db <string>   : Name of the database to connect to\n");
  printf("  --threads <num> : Number of handler threads you would like\n");
  printf("  --help          : Print out this menu\n");
  printf("\n");

  return(0);
}


/****************************************/
int ParseCommandLine(int argc, char **argv) {
/****************************************/
  /* Variable Declarations */
  int invalid = FALSE;

  /* Set the default command line arguements */
  mCmdLine.mPortNum = 50000;
  sprintf(mCmdLine.sDbName, "%s", "./OAServer.sqlite");
  mCmdLine.mNumThreads = 10;

  /* Change this to getopts */
  for(int i = 1; i < argc; i++) {
    const char *opt = argv[i];
    if(strcmp(opt, "--db") == 0) {
      i++;
      sprintf(mCmdLine.sDbName, "%s", argv[i]);
    } else if(strcmp(opt, "--port") == 0) {
      i++;
      mCmdLine.mPortNum = atoi(argv[i]);
    } else if(strcmp(opt, "--threads") == 0) {
      i++;
      mCmdLine.mNumThreads = atoi(argv[i]);
    } else if(strcmp(opt, "--help") == 0) {
      PrintMenu();
      exit(0);
    }
    else { invalid = TRUE; }
  }

  if(invalid) {
    PrintMenu();
    exit(-1);
  }

  printf("Executing: ./OAServer --port %d --db %s --threads %d\n", mCmdLine.mPortNum, mCmdLine.sDbName, mCmdLine.mNumThreads);

  return(0);
}


/****************************************/
int rpiServerMain(void) {
/****************************************/

  /* Variable Declaration */
  OAServer mOAServer(mCmdLine.mPortNum, mCmdLine.sDbName, mCmdLine.mNumThreads);

  mOAServer.SetVerbose(TRUE);
  mOAServer.Run();

  return(0);
}


/****************************************/
int main(int argc, char **argv) {
/****************************************/
  ParseCommandLine(argc, argv);
  rpiServerMain();
}


#if 0

/* This spawns the RPiServer task as a daemon */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

using namespace std;

#define DAEMON_NAME "RPiServer"


int main(int argc, char *argv[]) {
  /* Variable declaration */
  pid_t pid, sid;

  /* Set our Logging Mask and open the Log */
  setlogmask(LOG_UPTO(LOG_NOTICE));
  openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

  syslog(LOG_NOTICE, "Starting %s Daemon", DAEMON_NAME);


  /* Fork the Parent Process */
  if((pid = fork()) < 0) {
    syslog(LOG_ERR, "Daemon failed to fork()");
    exit(EXIT_FAILURE);
  }
  /* We got a good pid, Close the Parent Process */
  else if(pid > 0) {
    syslog(LOG_NOTICE, "%s Daemon successfully forked", DAEMON_NAME);
    exit(EXIT_SUCCESS);
  }

  /* Change File Mask */  /* Why this? */
  umask(0);

  /* Save the process ID for the child */
  if((sid = setsid()) < 0) {
    syslog(LOG_ERR, "Daemon failed to get child PID");
    exit(EXIT_FAILURE);
  }

  /* ADD BACK IN ONCE DAEMON IS WORKING */
#if 0
  /*  Close Standard File Descriptors to suppress writing to the terminal */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
#endif

  /* Main Daemon Process */
  while(1) {
    rpiServerMain();
  }

  /* Close the syslog */
  closelog();

  return(0);
}


#endif

#endif


