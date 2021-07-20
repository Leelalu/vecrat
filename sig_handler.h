#ifndef _SIGHANDLERH
#define _SIGHANDLERH


#include <signal.h>
#include "memsetup.h"


void sigHandler(int sig);
int applySignalHandlers();


#endif // _SIGHANDLERH
