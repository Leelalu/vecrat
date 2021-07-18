// Includes //
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/sem.h>
#include <semaphore.h>
#include "memsetup.h"


// Definitions //
// Ingrained Definitions
#define CODEEXIT 999
#define CODESTOP 111


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

// Execute program arguments
int argHandler(int argc, char **argv){
  int *shmPntr;
  sem_t *semPntr;

  shmPntr=createShmWriter();
  semPntr=createSemWriter();

  // Go through program flags
  if(shmPntr<0 || semPntr<0){
    printf("Exiting...");
    return(-1);
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
    }
    // Alter velocity if 2 additional args
      else if(argc==3){
        if(isStrInt(argv[1]) && isStrInt(argv[2])){
          shmPntr[0]=atoi(argv[1]);
          shmPntr[1]=atoi(argv[2]);
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
      destroyShmSemWriter(shmPntr, semPntr);
  }
      return(-1);
}

// Check for requests
int checkForExitRequest(int xoffset, int yoffset){
  if(xoffset==CODEEXIT && yoffset==CODEEXIT){
    return(-1);
  }
  else{
    return(0);
  }
}

int checkForStopRequest(int xoffset, int yoffset){
  if(xoffset==CODESTOP && yoffset==CODESTOP){
    return(-1);
  }
  else{
    return(0);
  }
}

int trimValue(int value, int bounds){
  if(value>bounds){
    value=bounds;
  }
  else if(value<bounds*-1){
    value=bounds*-1;
  }

  return(value);
}

int pullIntTowardsZero(int value){
  if(value>0){
    value--;
  }
  else if(value<0){
    value++;
  }
  return(value);
}
