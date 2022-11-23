#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sched.h>
#include <err.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#include "now.h"


#define USAGE "%s <seconds> <cpu0> <count>\n"
#define VERBOSE

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


int
main(int argc, char *argv[])
{
  int rc;
  int seconds;
  int count, i;
  int cpu0;
  uint64_t startTSC, endTSC, totalTSC;
  struct timespec req, rem;
  
  if (argc != 4) {
    fprintf(stderr, USAGE, argv[0]);
    return(-1);
  }
  
  seconds = atoi(argv[1]);
  cpu0 = atoi(argv[2]);
  count = atoi(argv[3]);
  
  req.tv_sec = seconds;
  req.tv_nsec = 0;

  pinCpu(cpu0);
  fprintf(stderr, "pgm,i,seconds,tsc,cpu,rem.sec,rem.nsec\n");
  for (i=0; i<count; i++) {
    rem.tv_sec = 0;
    rem.tv_nsec = 0;
    
    startTSC = now();
    
    rc = nanosleep(&req, &rem);
    
    endTSC = now();
    
    totalTSC = endTSC - startTSC;
    
    if (rc != 0) {
      fprintf(stderr,
	      "ERROR: rc: %d rem.tv_sec=%d rem.tv_nsec=%d\n",
	      rc, rem.tv_sec, rem.tv_nsec);
    }
    fprintf(stderr, "%s,%d,%d,%" PRIu64 ",%d,%d,%d\n",
	    argv[0],i,seconds,totalTSC,cpu0,rem.tv_sec, rem.tv_nsec);
  }
  return 0;
}
