


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
  int opt = TRUE;
  int master_socket = 0;
  int addrlen = 0;
  int new_socket = 0;
  int client_socket[30]; 
  int max_clients = 30;
  int activity;
  int i;
  int valread;
  int s;
  struct sockaddr_in address;
  char buffer[1025];  //data buffer of 1K
  char eof[16];
  sqlite3 *mDbPtr = NULL;
  char *mDbErrMsg = NULL;
  char pSQL[512];

  int32_t retVal = 0;
  PacketHeader_t hdr;
  meas_data_t  *measData = NULL;
  water_data_t *waterData = NULL;
  fish_data_t  *fishData = NULL;
  plant_data_t *plantData = NULL;
  unsigned char *data = NULL;
  FILE *outfile = NULL;

  //set of socket descriptors
  fd_set readfds;

  //initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++) {
    client_socket[i] = 0;
  }

  //create a master socket
  if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
    perror("socket failed"); exit(EXIT_FAILURE);
  }

  //set master socket to allow multiple connections , this is just a good habit, it will work without this
  if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
    perror("setsockopt"); exit(EXIT_FAILURE);
  }

  //type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(50000);

  //bind the socket to localhost port 8888
  if(bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed"); exit(EXIT_FAILURE);
  }

  //try to specify maximum of 3 pending connections for the master socket
  if(listen(master_socket, 3) < 0) {
    perror("listen"); exit(EXIT_FAILURE);
  }

  //accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections...");
  while(TRUE) {

    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);

    //add child sockets to set
    for(i = 0 ; i < max_clients ; i++) {
      s = client_socket[i];
      if(s > 0) {
        FD_SET( s , &readfds);
      }
    }

    //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
    activity = select(max_clients + 3 , &readfds , NULL , NULL , NULL);

    if((activity < 0) && (errno!=EINTR)) {
      printf("select error");
    }

    //If something happened on the master socket , then its an incoming connection
    if (FD_ISSET(master_socket, &readfds)) {
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept"); exit(EXIT_FAILURE);
      }

      //inform user of socket number - used in send and receive commands
      printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

      //add new socket to array of sockets
      for (i = 0; i < max_clients; i++) {
        s = client_socket[i];
        if(s == 0) {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n" , i);
          i = max_clients;
        }
      }
    }

    //else its some IO operation on some other socket :)
    for(i = 0; i < max_clients; i++) {
      s = client_socket[i];

      if(FD_ISSET(s ,&readfds)) {

        /* Check the make sure the stream is not closed */
        if((retVal = recv(s, eof, sizeof(eof), MSG_PEEK)) == 0) {
          //Somebody disconnected , get his details and print
          getpeername(s , (struct sockaddr*)&address , (socklen_t*)&addrlen);
          printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

          //Close the socket and mark as 0 in list for reuse
          close( s );
          client_socket[i] = 0;
        }
        //Echo back the message that came in
        else {
          if((retVal = read(s , &hdr, sizeof(hdr))) == sizeof(hdr)) {
            if((data = (unsigned char*)malloc(hdr.mNumBytes)) != NULL) {
            //  retVal = sockSer->RecvData((unsigned char*)data, hdr.mNumBytes);
              retVal = read(s , data, hdr.mNumBytes);
            }
   
            if(mRetVal = sqlite3_open("./temp.db", &mDbPtr)) {
              printf("ERR: Failed to open database\n");
              exit(-1);
            }

            if(hdr.mMsgType == ADD_MEASUREMENT) {
              measData = (meas_data_t*)data;
              //strftime('%s','2004-01-01 02:34:57')
              sprintf(pSQL, "INSERT INTO Statistics(mNodeId, mSampleTime, mWaterLevel) VALUES ('%d', '%d-%d-%d', '%f')", 
                      1, measData->rpiHdr.year, measData->rpiHdr.month, measData->rpiHdr.day, measData->measData);

              printf("%4d,%02d,%02d,%s,%.2f,%s\n",
                measData->rpiHdr.year, measData->rpiHdr.month, measData->rpiHdr.day,
                meas_type[measData->measType].abbr, measData->measData, measData->comment);

              if(sqlite3_exec(mDbPtr, pSQL, callback, 0, &mDbErrMsg)) {
                std::cout << "Sql Error: " << mDbErrMsg << std::endl;
                sqlite3_free(mDbErrMsg);
              }

            }
            else if(hdr.mMsgType == ADD_WATER) {
              waterData = (water_data_t*)data;
              printf("%4d,%02d,%02d,%.2f,%s\n",
                waterData->rpiHdr.year, waterData->rpiHdr.month, waterData->rpiHdr.day,
                waterData->waterData, waterData->comment);
            }
            else if(hdr.mMsgType == ADD_FISH) {
              fishData = (fish_data_t*)data;
              printf("%4d,%02d,%02d,%s,%s,%s\n",
                fishData->rpiHdr.year, fishData->rpiHdr.month, fishData->rpiHdr.day,
                fish_type[fishData->fishType].abbr, fish_size[fishData->fishSize].abbr, fishData->comment);
            }
            else if(hdr.mMsgType == ADD_PLANT) {
              plantData = (plant_data_t*)data;
              printf("%4d,%02d,%02d,%s,%s,%s\n",
                plantData->rpiHdr.year, plantData->rpiHdr.month, plantData->rpiHdr.day,
                plant_type[plantData->plantType].abbr, plant_size[plantData->plantSize].abbr, plantData->comment);
            }

            if(mDbPtr) {
              sqlite3_close(mDbPtr);
            }
 
            if(data) {
              free(data);
            }
            data = NULL;
          }
        }
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


