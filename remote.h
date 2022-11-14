#ifndef __REMOTE_H__
#define __REMOTE_H__

#include <unistd.h>


#define Q_LEN 1
#define MSG_NUM_START 0

const int CLIENT = 0;
const int SERVER = 1;

// Each message is one cache line
union Message {
  struct Request {
    volatile int arg;
  } req;
  struct Reply {
    volatile int arg;
  } reply;
  char padding[CACHE_LINE_SIZE];
};

#ifdef REMOTE_READ_SHARED

// Qinfo is one cache line that is an message index
union QInfo{
  volatile int idx;
  char padding[CACHE_LINE_SIZE];
};

// Queue of messages index indicates next free message
// Two queues :
// Request Q: written by client read by server
// Reply Q: read by client written by server
struct MessageQ {
  union QInfo info;
  union Message messages[Q_LEN];
}  __attribute__ ((aligned (CACHE_LINE_SIZE))) reqQ =
  {
   .info.idx = MSG_NUM_START
  },
  __attribute__ ((aligned (CACHE_LINE_SIZE))) replyQ =
  {
   .info.idx = MSG_NUM_START				
  };

static inline void clientAction ()
{
  // local copy of request queue info
  static union QInfo __attribute__ ((aligned (CACHE_LINE_SIZE))) replyQinfo =
    {
     .idx = MSG_NUM_START
    };

    // Add request to queue (copy volatile to nonvolatile local)
    //  we are single writer so we know they will stay insync
    // Eg. int curIdx = reqQ.info.idx;
    //     reqQ.messages[curIdx%Q_LEN].req.arg = SomeValue;
  
    // publish message
    __sync_fetch_and_add(&(reqQ.info.idx),1);

#ifdef LOCAL_WORK_WITH_REMOTE
    doWork();
#endif
    
    // spin for reply -- wait for until global version changes
    // compared to local verison
    while (replyQ.info.idx == replyQinfo.idx) {}
    // process reply
#ifndef SILENT
    (void) !write(1, "Client has the ball\n",20);
#endif
    // we assume here only one reply added at a time!
    //   eg. no more writes to shared idx so we can make a nonvolatile copy
    // copy global into local
    replyQinfo.idx = replyQ.info.idx;
}
  
static inline void serverAction ()
{
  static union QInfo __attribute__ ((aligned (CACHE_LINE_SIZE))) reqQinfo =
    {
      .idx = MSG_NUM_START
    };
  
  // spin for request
  while (reqQ.info.idx == reqQinfo.idx) {}
  // we know things have changed
  // assume single request enqueued so we can safely make a nonvolatile copy
  reqQinfo.idx = reqQ.info.idx;
#ifndef SILENT
  (void) !write(1, "Server has the request\n",23);
#endif
  // process one message at a time
#ifdef REMOTE_WORK
  doWork();
#endif    
  
  // we are single writer so we can make nonvolatile copy
  int curIdx = replyQ.info.idx;
  // add message data to queue
  // eg. replyQ.messages[curIdx%Q_LEN].req.arg = myVal;
  // publish message
  __sync_fetch_and_add(&(replyQ.info.idx),1);

}

#elif REMOTE_RW_SHARED

// read/write shared by both client and server
union SharedLine {
  char padding[CACHE_LINE_SIZE];
  volatile int theBall;
} __attribute__ ((aligned (CACHE_LINE_SIZE))) shared = { .theBall = CLIENT };

void clientAction ()
{

  while (shared.theBall != CLIENT) {}
#ifndef SILENT
    (void) !write(1, "Client has the ball\n",20);
#endif
    __sync_bool_compare_and_swap(&shared.theBall, CLIENT, SERVER);
#ifdef LOCAL_WORK_WITH_REMOTE
    doWork();
#endif
}


void serverAction ()
{
  while (shared.theBall != SERVER) {}
#ifndef SILENT
  (void) !write(1, "Server has the ball\n",20);
#endif
#ifdef REMOTE_WORK
  doWork();
#endif    
  __sync_bool_compare_and_swap(&shared.theBall, SERVER, CLIENT);
}

#endif

#endif
