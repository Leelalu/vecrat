#ifndef _VECRATH
#define _VECRATH


// Includes //
#include <stdbool.h>


// Func declarations //
bool isStrInt(char *str);
int argHandler(int argc, char **argv);
int checkForExitRequest(int xoffset, int yoffset);
int checkForStopRequest(int xoffset, int yoffset);


#endif
