#include <stdint.h>


#ifdef Work_WRITEDEVNULLWORK
int writeFd;
#endif

void workSetup()
{
#ifdef Work_WRITEDEVNULLWORK
  int writeFd = open("/dev/null", O_RDWR);
#endif
}

void workCleanup()
{
#ifdef Work_WRITEDEVNULLWORK
  close(writeFd);
#endif
}
