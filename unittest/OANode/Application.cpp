

#include "OANode.h"



/****************************************/
int OANodeMain(void) {
/****************************************/

  /* Variable Declaration */
  OANode mOANode((char*)"127.0.0.1", 50000, (char*)"", 2);

  for(int i = 0; i < 10; i++) {
    mOANode.Debug();
  }

  return(0);
}


/****************************************/
int main(int argc, char **argv) {
/****************************************/
  OANodeMain();
}



