#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sched.h>
#include <err.h>
#include <pthread.h>
#include <stdint.h>

#include "cacheline.h"
#include "now.h"
#include "work.h"
#include "remote.h"

union WorkTime WorkTSC = {
  .TSC = 0
};

#define USAGE "%s <iterations> <cpu0> <cpu1>\n"
// #define VERBOSE

int pinCpu(int cpu)
{
  cpu_set_t  mask;
  CPU_ZERO(&mask);

  if (cpu == -1 ) {
    cpu = sched_getcpu();
    if (cpu == -1) {
      err(1, "could not determine current cpu");
    }
  }

  CPU_SET(cpu, &mask);
  if (sched_setaffinity(0, sizeof(mask), &mask) != 0) {
    err(1, "could not pin to cpu=%d",cpu);
  }
  
#ifdef VERBOSE
    fprintf(stderr, "PINNED TO CPU: %d\n", cpu);
#endif
    
}

inline void preWork() {}
inline void postWork() {}

union ServerThreadArg {
  char padding[CACHE_LINE_SIZE];
  struct {
    volatile uint64_t ready;
    int cpu;
  };
};

void * serverThread(void *arg) {
  union ServerThreadArg *sarg = arg;
  pinCpu(sarg->cpu);

  __sync_fetch_and_add(&(sarg->ready),1);

  while (1) {
#ifndef LOCAL_WORK
    serverAction();
#endif    
  }
  return NULL;
}

pthread_t 
startServerThread(int cpu) {  
  pthread_t serverTid;
  union ServerThreadArg sarg = { .ready = 0, .cpu = cpu };
  pthread_create(&serverTid, NULL, serverThread, &sarg);
  while (sarg.ready==0) {}
  return serverTid;
}

int
main(int argc, char *argv[])
{

  int count;
  register int iters;
  int cpu0, cpu1;
  uint64_t startTSC, endTSC, totalTSC;
  
  if (argc != 4) {
    fprintf(stderr, USAGE, argv[0]);
    return(-1);
  }
  
  iters = atoi(argv[1]);
  count = iters;
  cpu0 = atoi(argv[2]);
  cpu1 = atoi(argv[3]);

  pinCpu(cpu0);

#ifndef NO_SERVER_THREAD
  // start server thread on the right core and barrier until
  // it is running
  startServerThread(cpu1);
#endif

  workSetup();
  
  startTSC = now();
  while (iters) {
    preWork();
#ifdef LOCAL_WORK
    doWork();
#else
    clientAction();
#endif
    postWork();    
    iters--;
  }
  endTSC = now();

  totalTSC = endTSC - startTSC;
  fprintf(stderr, "%s,%d,%" PRIu64 ",%" PRIu64 ",%d,%d\n",
	  argv[0],count,totalTSC, WorkTSC.TSC, cpu0, cpu1);
  workCleanup();
  return 0;
}  
