#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define SHAREDFILE "/.vecratsharedfile"

int *arrayPntrOffset[2];
char *sharedFileDir;
int fileDescript;
int *mmapFileDescript;

int main(int argc, char **argv){
  // Create set of integers to write to mem file
  arrayPntrOffset[1] = atoi(argv[1]);
  arrayPntrOffset[2] = atoi(argv[2]);

  // Get shared file directory
  sharedFileDir = strcat(getenv("HOME"), SHAREDFILE);

  // Create a file descriptor and open file

  // Map file to memory

  // Write args to file

  // Close Pointers/file
  close(fileDescript);
  munmap(mmapFileDescript, 20);

  // Exit
  return(0);
}
