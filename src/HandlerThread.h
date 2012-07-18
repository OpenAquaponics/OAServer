/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _HANDLER_THREAD_H_
#define _HANDLER_THREAD_H_

/* system includes */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <vector>
#include <iostream>

#include <boost/smart_ptr/shared_ptr.hpp>

/* user includes */
#include "Ethernet.h"
#include "Database.h"

/* defines */


/* enums */


/* typedef */


/* templates */
template <class A>
bool SortSharedPtr(const boost::shared_ptr<A>& lhs, const boost::shared_ptr<A>& rhs) {
  return(*lhs.get() < *rhs.get());
}


/* namespace */
//using namespace boost;
using namespace std;

/* class */
class HandlerThread {
  public:
    HandlerThread(void) { mThreadPriority = rand() % 100; cout << mThreadPriority << endl; };
   ~HandlerThread(void) {};

    bool operator < (const HandlerThread& rhs) {
      return mThreadPriority < rhs.mThreadPriority;
    }

    int GetThreadPriority(void) { return(mThreadPriority); };

  private:
    int mThreadPriority;
};


/* typedef */
typedef boost::shared_ptr<HandlerThread> spHandler;



#endif /* _HANDLER_THREAD_H_ */
