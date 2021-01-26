#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SHMKEY 893422
#define SHMSIZE 8
#define INTSIZE 4

int shmId;
int *shmPntr;

int main(int argc, char **argv){
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

  // Write args to var
  shmPntr[0]=atoi(argv[1]);
  shmPntr[1]=atoi(argv[2]);

  // Exit
  return(0);
}
