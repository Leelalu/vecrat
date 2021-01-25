#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <X11/Xlib.h>

#define SHAREDFILE "/.vecratsharedfile"

// Setup vars
int quitRequest = 0;
int vecArr[10] = {0, 1, 5, 5, 8, 10, 10, 20, 40, 50};
int vecArrPos[2] = {0, 0};
int vecArrOffsetArr[2] = {0, 0};
void *vecArrOffset;
char *sharedFileDir;
void *mmapFileDescript;
int fileDescript;
Display *XDisplay;
Window XRootWin;

int main() {
  // Malloc space holding vector offset
  vecArrOffset = malloc((sizeof(int)*2));

  // Set pointer to vecArrOffsetArr;
  vecArrOffset = &vecArrOffsetArr;

  // Get shared file directory
  sharedFileDir = strcat(getenv("HOME"), SHAREDFILE);

  // Create a file descriptor and open file

  // Map file to memory

  // Setup for xwarppointer
  Display *XDisplay = XOpenDisplay(0);
  Window XRootWin = XRootWindow(XDisplay, 0);
  XSelectInput(XDisplay, XRootWin, KeyReleaseMask);

  // Program loop
  while(!quitRequest){
    // Get array offset from shared file
    // Apply mouse velocity
    // XWarpPointer(XDisplay, None, XRootWin, 0, 0, 0, 0, vecArr[0], vecArr[1]);
    // XFlush(XDisplay);
    if(getchar()=='q'){
      quitRequest = 1;
    }
  }

  // Close Pointers/file
  close(fileDescript);
  free(vecArrOffset);

  return(0);
}
