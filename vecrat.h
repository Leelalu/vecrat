#ifndef _VECRATH
#define _VECRATH


#include <X11/Xlib.h>
#include <stdbool.h>


bool isStrInt(char *str);
int argHandler(int argc, char **argv);
int checkForExitRequest(int xoffset, int yoffset);
int checkForStopRequest(int xoffset, int yoffset);
int trimValue(int value, int bounds);
int pullIntTowardsZero(int value);
void destroyMemory(Display *XDis, Window XWin, int *shmPntr, sem_t *semPntr);


#endif // _VECRATH
