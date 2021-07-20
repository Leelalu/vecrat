#include <stdlib.h>
#include <signal.h>
#include "memsetup.h"


void sigHandler(int sig){
  removeShmSemFile();
  exit(0);
}

int applySignalHandlers(){
  signal(SIGINT, sigHandler);
  signal(SIGKILL, sigHandler);
  signal(SIGSTOP, sigHandler);
  return(0);
}
