// Includes //
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <X11/Xlib.h>
#include "memsetup.h"


// Definitions //
// Customizable Definitions
#define OFFSETFORMULA(X) X*3
#define OFFSETMAX 40
#define OFFSETMIN -40

// Ingrained Definitions
#define CODEEXIT 999
#define CODESTOP 111
#define SEMNAME "sem.vecrat"


// Func definitions //
// Determine if multi char string is int
bool isStrInt(char *str){
  int strSize=strlen(str);
  for(int i=0; i<strSize; i++){
    if(!(isdigit(str[i])) && !(i==0 && str[i]=='-')){
      return(false);
    }
  }
  return(true);
}

// Determin how program is implimented based on args
int argHandler(int argc, char **argv, int *shmPntr){
  sem_t *semPntr;
  // If only the command to launch prog entered, check for dups, if none continue
  if(argc==1){
    // Check if shmfile exists
    if(access(SHMFILE, F_OK)==0){
      printf("Found shared memory file at %s...\n", SHMFILE);
      printf("Determined other program running, exiting...\n");
    }
    else{
      printf("Starting vecrat...\n");
      return(0);
    }
  }
  // If not just starting, got through args
  // Program flags
  else{
    // Setup shared memory & semaphore
    shmPntr=getShmPntr(SHMFILE);
    semPntr=sem_open(SEMNAME, 0);
    if(shmPntr<0){
      printf("Failed setting up shared memory...");
    }
    else if(semPntr<0){
      printf("Failed setting up semaphore...");
    }
    else{
      // Close semaphore
      sem_wait(semPntr);
      // If only one additional args parse
      if(argc==2){
        if(!(strcmp(argv[1], "--stop"))){
          printf("Stoping velocity...\n");
          shmPntr[0]=shmPntr[1]=CODESTOP;
        }
        else if(!(strcmp(argv[1], "--exit"))){
          printf("Exiting...\n");
          shmPntr[0]=shmPntr[1]=CODEEXIT;
        }
        else{
          printf("Acceptable arguments are --stop(acceleration), and --exit(quit program)\n");
        }
      }
      // Alter velocity if 2 additional args
      else if(argc==3){
        if(isStrInt(argv[1]) && isStrInt(argv[2])){
          shmPntr[0]=atoi(argv[1]);
          shmPntr[1]=atoi(argv[2]);
        }
        else{
          printf("Vecrat mouse offset must be two integers\n");
        }
       }
      // Else, print options and leave
      else{
        printf("Start vecrat with      'vecrat'\n"
               "Add acceleration with  'vecrat xoffet yoffset'\n"
               "Stop acceleration with 'vecrat -stop'\n"
               "Exit vecrat with       'vecrat --exit'\n");
      }
      // Open semaphore
      sem_post(semPntr);
  }
      return(-1);
  }
}
