/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#include "Util.h"


/****************************************/
int Util::Init(void) {
/****************************************/

  return(0);
}


/****************************************/
Util::Util(void) {
/****************************************/
  /* Initialize all of the class data members */
  Init();

}


/****************************************/
Util::~Util(void) {
/****************************************/
}


/****************************************/
unsigned short ComputeChecksum(unsigned short *pBuff, int mNumBytes) {
/****************************************/
  unsigned short mChksum = 0;

  for( ; mNumBytes > 1; mNumBytes -= sizeof(*pBuff)) {
    mChksum += *pBuff++;
  }
  if(mNumBytes) {
    mChksum += *(unsigned char*)pBuff;
  }

  return(mChksum);
}


#if 0
/****************************************/
template <class A>
<A> Util::ComputeChecksum(<A> *pBuff, int mNumElems) {
/****************************************/
  return(mNumElems);
}
#endif

