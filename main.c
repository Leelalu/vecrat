// Includes //
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <X11/Xlib.h>
#include <math.h>
#include "memsetup.h"
#include "vecrat.h"


// Customizable Definitions //
#define OFFSETFORMULA(X) pow(X/3, 3)/3
#define OFFSETTRIM 20


// Vars //
int offset[2]={0};
int *shmPntr;
sem_t *semPntr;
Display *XDisplay;
Window XRootWin;
int quitRequest = 0;


// Main Function //
int main(int argc, char **argv){
  // Handle args and exit if args present
  if(argc>1){
    argHandler(argc, argv);
    return(0);
  }

  // Setup shared memory and semaphore
  shmPntr=createShmReader();
  semPntr=createSemReader();
  if(shmPntr<0 || semPntr<0){
    printf("Exitting...\n");
  }

  // Setup XDisplay/Window for xwarppointer
  XDisplay=XOpenDisplay(0);
  if(XDisplay==NULL){
    printf("Opening x display failed...\n");
    perror("XOpenDisplay");
    return(1);
  }
  XRootWin = XRootWindow(XDisplay, 0);
  XSelectInput(XDisplay, XRootWin, KeyReleaseMask);

  // Main vector alteration/application loop
  while(!quitRequest){
    printf("%d-%d\n", OFFSETFORMULA(offset[0]), OFFSETFORMULA(offset[1]));
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
  XDestroyWindow(XDisplay, XRootWin);
  XCloseDisplay(XDisplay);
  destroyShmSemReader(shmPntr, semPntr);

  // Exit
  return(0);
}
