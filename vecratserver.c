// Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "memsetup.h"


// Definitions
#define SOCKADDR "vecratserversocket"
#define CODEEXIT 999
#define CODESTOP 111


// Vars
int vecArr[25] = {00, 05, 05, 10, 10,
                  15, 15, 20, 20, 25,
                  25, 30, 35, 40, 40,
                  43, 46, 50, 55, 60,
                  65, 65, 75, 80, 85};
int vecArrPos[2];
int vecArrLen;
Display *XDisplay;
Window XRootWin;
int quitRequest = 0;


int main() {
  // Setup
  // Set quit request
  quitRequest = 0;

  // Set vecArrLen
  vecArrLen=sizeof(vecArr)/sizeof(int);

  // Setup shared memory for server
  if((memServSetup(&shmId, &shmPntr, &shmStruct))< 0){
    printf("Initializing shared memory failed...\n");
    return(-1);
  }

  // Setup semaphore
  if((semSetup(&semPntr)) < 0){
    printf("Initializing semaphore failed...\n");
    return(-1);
  }

  // Zero out shmPntr and vecArrPos
  shmPntr[0]=shmPntr[1]=0;
  vecArrPos[0]=vecArrPos[1]=0;

  // Setup XDisplay/Window for xwarppointer
  if((XDisplay = XOpenDisplay(0)) == NULL){
    printf("Opening x display failed...\n");
    perror("XOpenDisplay");
    return(1);
  }
  XRootWin = XRootWindow(XDisplay, 0);
  XSelectInput(XDisplay, XRootWin, KeyReleaseMask);


  // Main program loop
  // Program loop
  while(!quitRequest){
    // Lock semaphore while reading/writing
    //    sem_wait(&semPntr);
    // Check for codes
    // Exit Code
    if(shmPntr[0]==CODEEXIT && shmPntr[1]==CODEEXIT){
      quitRequest=1;
    }
    else{
      // Stop/Zero out Code
      if(shmPntr[0]==CODESTOP && shmPntr[1]==CODESTOP){
        shmPntr[0]=shmPntr[1]=0;
        vecArrPos[0]=vecArrPos[1]=0;
      }
      // Add shmPntr to vecArrPos
      vecArrPos[0]=vecArrPos[0]+shmPntr[0];
      vecArrPos[1]=vecArrPos[1]+shmPntr[1];
      // Zero out shmPntr
      shmPntr[0]=shmPntr[1]=0;
      // Unlock semaphore for reading/writing
      //      sem_post(&semPntr);
      // Trim vecArrPos to bounds of vecArr
      if(vecArrPos[0]<(vecArrLen*-1)){vecArrPos[0]=vecArrLen*-1;}
      if(vecArrPos[1]<(vecArrLen*-1)){vecArrPos[1]=vecArrLen*-1;}
      if(vecArrPos[0]>vecArrLen){vecArrPos[0]=vecArrLen;}
      if(vecArrPos[1]>vecArrLen){vecArrPos[1]=vecArrLen;}
      // Apply mouse velocity
      XWarpPointer(XDisplay, None, None, 0, 0, 0, 0, vecArrPos[0], vecArrPos[1]);
      XFlush(XDisplay);
      // Increment/Decrament vecArrPos
      if(vecArrPos[0]>0){vecArrPos[0]--;}
      if(vecArrPos[1]>0){vecArrPos[1]--;}
      if(vecArrPos[0]<0){vecArrPos[0]++;}
      if(vecArrPos[1]<0){vecArrPos[1]++;}
      // Sleep/Pause
      usleep(50000);
    }
  }

  // Close shared/allocated memory and exit
  shm_unlink(&shmPntr);
  shmdt(shmPntr);
  sem_close(semPntr);
  sem_unlink(&semPntr);
  XDestroyWindow(XDisplay, XRootWin);
  XCloseDisplay(XDisplay);

  // Exit
  return(0);
}
