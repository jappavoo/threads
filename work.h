#ifndef __WORK_H__
#define __WORK_H__

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define WORK_NOPLOOP_COUNT 40000
#define WORK_WRITE_COUNT 1

union WorkTime {
  uint64_t  TSC;
  char padding[CACHE_LINE_SIZE];
};

extern union WorkTime WorkTSC;

inline  __attribute__((always_inline)) int 
Work_NOPLOOP()
{
  int i;
  for (i=0; i<WORK_NOPLOOP_COUNT;i++) {
    asm("nop");
  }
  return i;
}

inline  __attribute__((always_inline)) int
Work_4NOPLOOP()
{
  int i;
  for (i=0; i<(WORK_NOPLOOP_COUNT/4); i++) {
    asm("nop;nop;nop;nop");
  }
  return i;
}

inline  __attribute__((always_inline)) int 
Work_NULLLOOP()
{
  int i;
  for (i=0; i<(WORK_NOPLOOP_COUNT); i++) {}
  return i;
}

extern int writeFd;

inline  __attribute__((always_inline)) int
Work_writeDevNull() {
  char buf[1];
  int i;
  for (i=0; i<WORK_WRITE_COUNT; i++) {
    write(writeFd, buf, sizeof(buf));
  }
  return i;	       
}


inline  __attribute__((always_inline)) int doWork()
{
  int rc;
  uint64_t wstart, wend;
  wstart = now();
  rc = WORK();
  wend = now();
  WorkTSC.TSC += (wend - wstart);
}

extern void workSetup();
extern void workCleanup();
#endif
