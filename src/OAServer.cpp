


#include <iostream>

#include "Ethernet.h"
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char**szColName) {
  for(int i = 0; i < argc; i++) {
    std::cout << szColName[i] << " = " << argv[i] << std::endl;
  }
  std::cout << "\n";
  return(0);
}


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

#include "common_types.h"
#include "pkt_types.h"
#include "Ethernet.h"
#include "Database.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>



using namespace std;



#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h>  //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#if 0
  int sock

typedef struct {
  socket_t 
  void (*sockCallback) (socket_t *sock);

}
#endif


int rpiServerMain(void) {

  /* Variable Declaration */
  int mRetVal = 0;
  int i;
  char buffer[1025];  //data buffer of 1K
  char eof[16];
  sqlite3 *mDbPtr = NULL;
  char *mDbErrMsg = NULL;
  char pSQL[512];

  int32_t retVal = 0;
  unsigned char *data = NULL;
  FILE *outfile = NULL;

  Ethernet mSock = Ethernet(SOCKET_TYPE_TCP_SERVER, (char*)"127.0.0.1", 50000);


  //accept the incoming connection
  puts("Waiting for connections...");
  while(TRUE) {

    mSock.PollOpenSockets();

    /* The packet header and data are public variables from the class, this is a kludge */
    if(mSock.mData) {

      if(mRetVal = sqlite3_open("./temp.db", &mDbPtr)) {
        printf("ERR: Failed to open database\n");
        exit(-1);
      }
 
 
      //strftime('%s','2004-01-01 02:34:57')
      meas_data_t *measData = (meas_data_t*)mSock.mData;
      sprintf(pSQL, "INSERT INTO Statistics(mNodeId, mSampleTime, mWaterLevel) VALUES ('%d', '%d-%d-%d', '%f')", 
              1, measData->rpiHdr.year, measData->rpiHdr.month, measData->rpiHdr.day, *((unsigned int*)mSock.mData));
 
      if(sqlite3_exec(mDbPtr, pSQL, callback, 0, &mDbErrMsg)) {
        std::cout << "Sql Error: " << mDbErrMsg << std::endl;
        sqlite3_free(mDbErrMsg);
      }
    }

  }

  return 0;
}

int main() {

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


