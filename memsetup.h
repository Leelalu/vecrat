// Includes //
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>


// Definitions //
#define SHMFILE "/dev/shm/vecratoffset"
#define SHMSIZE 8


// Functions //
int getShmBlk(char *fileName){
  key_t key;
  int shmBlkId;

  key=ftok(fileName, 0);
  if(key < 0){
    perror("ftok");
    return(-1);
  }

  shmBlkId=shmget(key, SHMSIZE, 0644 | IPC_CREAT);
  if(shmBlkId < 0){
    perror("shmget");
    return(-1);
  }

  return(shmBlkId);
}

int *getShmPntr(char *fileName){
  int shmBlkId;
  int *shmBlkPntr;

  shmBlkId=getShmBlk(fileName);

  shmBlkPntr=shmat(shmBlkId, NULL, 0);
  if(shmBlkPntr < 0){
    perror("shmat");
    return(-1);
  }

  return(shmBlkPntr);
}
