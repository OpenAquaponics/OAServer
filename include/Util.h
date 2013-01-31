/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _UTIL_H_
#define _UTIL_H_

/* system includes */

/* user includes */
#include "pkt_types.h"

/* defines */

/* enums */

/* typedef */

/* The class definitions */
class Util {

  private:
    int Init(void);

  public:
    Util(void);
   ~Util(void);

    /* Make this a template so any data type can be used */
    int ComputeChecksum(int *pBuff, int mNumBytes);

#if 0
    template <class A>
<A> Util::ComputeChecksum(<A> *pBuff, int mNumElems) {
#endif

  protected:

  private:

};

#endif /* _UTIL_H_ */
