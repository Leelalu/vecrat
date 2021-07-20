// Includes //
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <X11/Xlib.h>
#include <math.h>
#include "sig_handler.h"
#include "memsetup.h"
#include "vecrat.h"


#define OFFSETFORMULA(X) pow(X/3, 3)/3
#define OFFSETTRIM 20


int offset[2];
int *shmPntr;
sem_t *semPntr;
Display *XDisplay;
Window XRootWin;
int quitRequest;


int main(int argc, char **argv){
  // If args preseng handle and exit
  if(argc>1){
    argHandler(argc, argv);
    return(0);
  }

  // Apply signal handler to free memory and remove shared mem file
  applySignalHandlers();

  // Check for shared mem file, if exists assume vecrat already running and exit
  if(checkIfShmFileExists()){
    printf("Exitting...\n");
    return(1);
  }

  // Setup shared memory and semaphore
  shmPntr=createShmReader();
  semPntr=createSemReader();
  if(shmPntr<0 || semPntr<0){
    printf("Exitting...\n");
    return(1);
  }

  // Setup XDisplay/Window for xwarppointer
  XDisplay=XOpenDisplay(0);
  XRootWin = XRootWindow(XDisplay, 0);
  if(XDisplay==NULL){
    printf("Opening x display failed...\n");
    return(1);
  }
  XSelectInput(XDisplay, XRootWin, KeyReleaseMask);

  // Main vector alteration/application loop
  while(!quitRequest){
    // Close semaphore
    sem_wait(semPntr);
    // Handle Exit Code
    if(checkForExitRequest(shmPntr[0], shmPntr[1])){
      quitRequest=1;
    }
    // Handle Zero out Code
    else if(checkForStopRequest(shmPntr[0], shmPntr[1])){
      offset[0]=offset[1]=0;
      shmPntr[0]=shmPntr[1]=0;
    }
    // Otherwise get shmPntr and add to offset if any have non-zero value
    else if(offset[0]!=0 || offset[1]!=0 || shmPntr[0]!=0 || shmPntr[1]!=0){
      // Set vec arr
      offset[0]=offset[0]+shmPntr[0];
      offset[1]=offset[1]+shmPntr[1];
      // Reset shm
      shmPntr[0]=shmPntr[1]=0;
      // Trim offset to bounds of vecArr
      offset[0]=trimValue(offset[0], OFFSETTRIM);
      offset[1]=trimValue(offset[1], OFFSETTRIM);
      // Apply mouse velocity
      XWarpPointer(XDisplay, None, None, 0, 0, 0, 0,
                   OFFSETFORMULA(offset[0]),
                   OFFSETFORMULA(offset[1]));
      XFlush(XDisplay);
      // Increment/Decrament offset towards 0
      offset[0]=pullIntTowardsZero(offset[0]);
      offset[1]=pullIntTowardsZero(offset[1]);
    }
    // Open semaphore
    sem_post(semPntr);
    // Wait
    usleep(50000);
  }

  // Close shared/allocated memory and exit
  destroyMemory(XDisplay, XRootWin, shmPntr, semPntr);
  // Exit
  return(0);
}
