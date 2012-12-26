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
int Util::ComputeChecksum(int *pBuff, int mNumBytes) {
/****************************************/
  int mChksum = 0;

  for( ; mNumBytes > 4; mNumBytes -= sizeof(*pBuff)) {
    mChksum += *pBuff++;
  }
  for( ; mNumBytes > 0; mNumBytes--) {
    mChksum += *(char*)pBuff++;
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

