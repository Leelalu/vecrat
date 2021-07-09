// Includes //
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
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


// Vars //
int vecArrOffset[2]={0};
Display *XDisplay;
Window XRootWin;
int quitRequest = 0;

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
  // If only the command to launch prog entered, check for dups, if none continue
  if(argc==1){
    // Check if shmfile exists
    if(access(SHMFILE, F_OK)==0){
      printf("Found shared memory file at %s...\n", SHMFILE);
      printf("Determined other program running, exiting...\n");
      return(-1);
    }
    // Setup shared memory
    else{
      open(SHMFILE, O_RDWR | O_CREAT, 0777);
      shmPntr=getShmPntr();
      if(shmPntr<0){
        printf("Failed setting up shared memory...");
        return(-1);
      }
      printf("Starting vecrat...\n");
      return(0);
    }
  }
  // If not just starting, got through args
  // Program flags
  else{
    // Setup shared memory
    shmPntr=getShmPntr();
    if(shmPntr<0){
      printf("Failed setting up shared memory...");
      return(-1);
    }
    else if(argc==2){
    // If only one additional args parse
      if(!(strcmp(argv[1], "--stop"))){
        shmPntr[0]=shmPntr[1]=CODESTOP;
        shmdt(shmPntr);
        return(-1);
      }
      else if(!(strcmp(argv[1], "--exit"))){
        shmPntr[0]=shmPntr[1]=CODEEXIT;
        shmdt(shmPntr);
        return(-1);
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
        shmdt(shmPntr);
        return(-1);
      }
      else{
        printf("Vecrat mouse offset must be two integers\n");
        return(-1);
      }
     }
    // Else, print options and leave
    else{
      printf("Start vecrat with      'vecrat'\n"
             "Add acceleration with  'vecrat xoffet yoffset'\n"
             "Stop acceleration with 'vecrat -stop'\n"
             "Exit vecrat with       'vecrat --exit'\n");
      return(-1);
    }
  }
}
