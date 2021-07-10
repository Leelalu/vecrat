#include "vecrat.h"


int main(int argc, char **argv){
  // Vars
  int vecArrOffset[2]={0};
  Display *XDisplay;
  Window XRootWin;
  int quitRequest = 0;
  int *shmPntr;

  // Determine current usage from args
  if(argHandler(argc, argv, &shmPntr)==-1){
    return(1);
  }

  // Open shared memory
  open(SHMFILE, O_RDWR | O_CREAT, 0777);
  shmPntr=getShmPntr(SHMFILE);
  if(shmPntr<0){
    printf("Failed setting up shared memory...");
    return(-1);
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
    printf("shm1:%d shm2:%d va1:%d va2:%d\n", shmPntr[0], shmPntr[1], vecArrOffset[0], vecArrOffset[1]);
    // Handle Exit Code
    if(shmPntr[0]==CODEEXIT && shmPntr[1]==CODEEXIT){
      quitRequest=1;
    }
    // Handle Zero out Code
    else if(shmPntr[0]==CODESTOP && shmPntr[1]==CODESTOP){
      vecArrOffset[0]=vecArrOffset[1]=0;
      shmPntr[0]=shmPntr[1]=0;
    }
    // Otherwise get shmPntr and add to vecArrOffset if any have non-zero value
    else if(vecArrOffset[0]!=0 || vecArrOffset[1]!=0 || shmPntr[0]!=0 || shmPntr[1]!=0){
      // Set vec arr
      vecArrOffset[0]=vecArrOffset[0]+shmPntr[0];
      vecArrOffset[1]=vecArrOffset[1]+shmPntr[1];
      // Reset shm
      shmPntr[0]=shmPntr[1]=0;
      // Trim vecArrOffset to bounds of vecArr
    	if(vecArrOffset[0]<OFFSETMIN){vecArrOffset[0]=OFFSETMIN;}
    	if(vecArrOffset[1]<OFFSETMIN){vecArrOffset[1]=OFFSETMIN;}
    	if(vecArrOffset[0]>OFFSETMAX){vecArrOffset[0]=OFFSETMAX;}
    	if(vecArrOffset[1]>OFFSETMAX){vecArrOffset[1]=OFFSETMAX;}
    	// Apply mouse velocity
    	XWarpPointer(XDisplay, None, None, 0, 0, 0, 0, OFFSETFORMULA(vecArrOffset[0]), vecArrOffset[1]);
    	XFlush(XDisplay);
    	// Increment/Decrament vecArrOffset towards 0
    	if(vecArrOffset[0]>0){vecArrOffset[0]--;}
    	if(vecArrOffset[1]>0){vecArrOffset[1]--;}
    	if(vecArrOffset[0]<0){vecArrOffset[0]++;}
    	if(vecArrOffset[1]<0){vecArrOffset[1]++;}
    }
    // Wait
    usleep(50000);
  }



  // Close shared/allocated memory and exit
  XDestroyWindow(XDisplay, XRootWin);
  XCloseDisplay(XDisplay);
  shmdt(shmPntr);
  shmctl(getShmBlk(SHMFILE), IPC_RMID, NULL);
  remove(SHMFILE);

  // Exit
  return(0);
}
