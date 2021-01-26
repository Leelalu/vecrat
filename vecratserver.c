#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/Xlib.h>

// Definitions
#define SHMKEY 893422
#define SHMSIZE 8
#define INTSIZE 4
#define CODEEXIT 111

// Setup vars
int quitRequest = 0;
int vecArr[10] = {0, 10, 5, 10, 10, 15, 15, 20, 20, 25, 25, 30, 35};
int vecArrPos[2] = {0, 0};
int vecArrLen;
int shmId;
int *shmPntr;
Display *XDisplay;
Window XRootWin;

int main() {
  // Set vecArrLen
  vecArrLen=sizeof(vecArr)/sizeof(int);
  printf("%d\n", vecArrLen);

  // Map pointer to memory w/ SHMKEY
  if((shmId=shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0770)) < 0){
    printf("Unable to get memory space...\n");
    perror("shmget");
    return(0);
  }
  else if((shmPntr=shmat(shmId, NULL, IPC_CREAT | 0770)) < 0){
    printf("Unable to get memory space...\n");
    perror("shmmat");
    return(0);
  }

  // Zero out shmPntr
  shmPntr[0]=0;
  shmPntr[1]=0;

  // Setup for xwarppointer
  XDisplay = XOpenDisplay(0);
  XRootWin = XRootWindow(XDisplay, 0);
  XSelectInput(XDisplay, XRootWin, KeyReleaseMask);

  // Program loop
  //  while(!quitRequest){
  while(!quitRequest){
    printf("%d\n", shmPntr[0]);
    printf("%d\n", shmPntr[1]);
    // Check for codes
    if(shmPntr[0]==CODEEXIT){
      if(shmPntr[1]==CODEEXIT){
        quitRequest=1;
      }
    }
    // Add shmPntr to vecArrPos
    vecArrPos[0]=vecArrPos[0]+shmPntr[0];
    vecArrPos[1]=vecArrPos[1]+shmPntr[1];
    // Trim vecArrPos to bounds of vecArr
    if(vecArrPos[0]<0){vecArrPos[0]=0;}
    if(vecArrPos[1]<0){vecArrPos[1]=0;}
    if(vecArrPos[0]>vecArrLen){vecArrPos[0]=vecArrLen;}
    if(vecArrPos[1]>vecArrLen){vecArrPos[1]=vecArrLen;}
    printf("%d\n", vecArrPos[0]);
    printf("%d\n", vecArrPos[1]);
    // Apply mouse velocity
    XWarpPointer(XDisplay, None, None, 0, 0, 0, 0, vecArrPos[0], vecArrPos[1]);
    XFlush(XDisplay);
    // Zero out shmPntr
    shmPntr[0]=0;
    shmPntr[1]=0;
    // Decrament vecArrPos
    vecArrPos[0]--;
    vecArrPos[1]--;
    // Sleep/Pause
    usleep(7500);
  }

  // Exit
  return(0);
}
