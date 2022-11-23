CPUTYPE=x86-64
O=3
SILENT=-DSILENT

.PHONY: clean
CFLAGS := -g -march=$(CPUTYPE) -O$(O) $(SILENT)

%.o: %.c
	gcc ${CFLAGS}  -c $< -o $@

%.o: %.s
	gcc ${CFLAGS}  -c $< -o $@ 

%.s: %.c
	gcc ${CFLAGS}  -S  $<

all: calibrate bm.4NOPLOOP_LOCAL_WORK bm.4NOPLOOP_LOCAL_WORK_NO_SERVERTHREAD bm.NOPLOOP_LOCAL_WORK bm.NOPLOOP_LOCAL_WORK_NO_SERVERTHREAD bm.NULLLOOP_LOCAL_WORK bm.NULLLOOP_LOCAL_WORK_NO_SERVERTHREAD bm.NOPLOOP_REMOTE_WORK_READ_SHARED bm.NOPLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED bm.NOPLOOP_REMOTE_WORK_RW_SHARED bm.NOPLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED bm.4NOPLOOP_REMOTE_WORK_READ_SHARED bm.4NOPLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED bm.4NOPLOOP_REMOTE_WORK_RW_SHARED bm.4NOPLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED bm.NULLLOOP_REMOTE_WORK_READ_SHARED bm.NULLLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED bm.NULLLOOP_REMOTE_WORK_RW_SHARED bm.NULLLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED

calibrate: calibrate.c
	gcc ${CFLAGS} calibrate.c -o calibrate

bm.NOPLOOP_LOCAL_WORK: DEFINES=-DWORK=Work_NOPLOOP -DLOCAL_WORK 
bm.NOPLOOP_LOCAL_WORK: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NOPLOOP_LOCAL_WORK_NO_SERVERTHREAD: DEFINES=-DWORK=Work_NOPLOOP -DLOCAL_WORK -DNO_SERVER_THREAD
bm.NOPLOOP_LOCAL_WORK_NO_SERVERTHREAD: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NULLLOOP_LOCAL_WORK: DEFINES=-DWORK=Work_NULLLOOP -DLOCAL_WORK
bm.NULLLOOP_LOCAL_WORK: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NULLLOOP_LOCAL_WORK_NO_SERVERTHREAD: DEFINES=-DWORK=Work_NULLLOOP -DLOCAL_WORK -DNO_SERVER_THREAD
bm.NULLLOOP_LOCAL_WORK_NO_SERVERTHREAD: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.4NOPLOOP_LOCAL_WORK: DEFINES=-DWORK=Work_4NOPLOOP -DLOCAL_WORK
bm.4NOPLOOP_LOCAL_WORK: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.4NOPLOOP_LOCAL_WORK_NO_SERVERTHREAD: DEFINES=-DWORK=Work_4NOPLOOP -DLOCAL_WORK -DNO_SERVER_THREAD
bm.4NOPLOOP_LOCAL_WORK_NO_SERVERTHREAD: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread



bm.NOPLOOP_REMOTE_WORK_READ_SHARED: DEFINES=-DWORK=Work_NOPLOOP -DREMOTE_WORK -DREMOTE_READ_SHARED
bm.NOPLOOP_REMOTE_WORK_READ_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NOPLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED: DEFINES=-DWORK=Work_NOPLOOP -DLOCAL_WORK_WITH_REMOTE -DREMOTE_READ_SHARED
bm.NOPLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NOPLOOP_REMOTE_WORK_RW_SHARED: DEFINES=-DWORK=Work_NOPLOOP -DREMOTE_WORK -DREMOTE_RW_SHARED
bm.NOPLOOP_REMOTE_WORK_RW_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NOPLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED: DEFINES=-DWORK=Work_NOPLOOP -DLOCAL_WORK_WITH_REMOTE -DREMOTE_RW_SHARED
bm.NOPLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread


bm.4NOPLOOP_REMOTE_WORK_READ_SHARED: DEFINES=-DWORK=Work_4NOPLOOP -DREMOTE_WORK -DREMOTE_READ_SHARED
bm.4NOPLOOP_REMOTE_WORK_READ_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.4NOPLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED: DEFINES=-DWORK=Work_4NOPLOOP -DLOCAL_WORK_WITH_REMOTE -DREMOTE_READ_SHARED
bm.4NOPLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.4NOPLOOP_REMOTE_WORK_RW_SHARED: DEFINES=-DWORK=Work_4NOPLOOP -DREMOTE_WORK -DREMOTE_RW_SHARED
bm.4NOPLOOP_REMOTE_WORK_RW_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.4NOPLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED: DEFINES=-DWORK=Work_4NOPLOOP -DLOCAL_WORK_WITH_REMOTE -DREMOTE_RW_SHARED
bm.4NOPLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread



bm.NULLLOOP_REMOTE_WORK_READ_SHARED: DEFINES=-DWORK=Work_NULLLOOP -DREMOTE_WORK -DREMOTE_READ_SHARED
bm.NULLLOOP_REMOTE_WORK_READ_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NULLLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED: DEFINES=-DWORK=Work_NULLLOOP -DLOCAL_WORK_WITH_REMOTE -DREMOTE_READ_SHARED
bm.NULLLOOP_LOCAL_WORK_WITH_REMOTE_READ_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NULLLOOP_REMOTE_WORK_RW_SHARED: DEFINES=-DWORK=Work_NULLLOOP -DREMOTE_WORK -DREMOTE_RW_SHARED
bm.NULLLOOP_REMOTE_WORK_RW_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread

bm.NULLLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED: DEFINES=-DWORK=Work_NULLLOOP -DLOCAL_WORK_WITH_REMOTE -DREMOTE_RW_SHARED
bm.NULLLOOP_LOCAL_WORK_WITH_REMOTE_RW_SHARED: main.c work.c work.h now.h remote.h
	gcc ${CFLAGS}  ${DEFINES} main.c work.c -o $@ -lpthread





threads.o: threads.s
threads2.o: threads2.s

threads: threads.o
	gcc ${LDFLAGS} threads.o -o $@ -lpthread

threads2: threads2.o
	gcc ${LDFLAGS} threads2.o -o $@ -lpthread


threadsSilent: CFLAGS+=-DSILENT
threadsSilent: threads.o 
	gcc ${LDFLAGS} threads.o -o $@ -lpthread

threads2Silent: CFLAGS=-DSILENT
threads2Silent: threads2.o 
	gcc ${LDFLAGS} threads2.o -o $@ -lpthread

clean:
	rm -rf $(wildcard *.o bm_* bm.* threads threads2 threadsSilent threads2Silent)

distclean: clean
	rm -rf $(wildcard *.times)
