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


/* class */
class HandlerThread : public EthernetList {
  private:
    void Init(void);

  public:
    HandlerThread(int mID);
    HandlerThread(void);
   ~HandlerThread(void);

    bool operator < (const HandlerThread& rhs) {
      return mThreadPriority < rhs.mThreadPriority;
    }

    int GetThreadID(void) { return(mThreadId); };
    int GetThreadPriority(void) { return(mThreadPriority); };

    int AddSocket(SOCKET_TYPE_e mSockType, Socket_t mSock);
    virtual int ProcessData(Ethernet *pSock);

  protected:
    int SetThreadID(int mId) { this->mThreadId = mId; };
    int SetThreadPriority(int p) { this->mThreadPriority = p; };

  private:
    int mThreadId;
    int mThreadPriority;

};



#endif /* _HANDLER_THREAD_H_ */
