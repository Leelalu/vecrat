#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "memsetup.h"

#define SHMKEY 893422
#define SHMSIZE 8
#define SEMNAME "vecratsem"

int shmId;
int *shmPntr;
sem_t *semPntr;


int main(int argc, char **argv){
  // Set shared memory/semaphore
  if(memSetup(&shmId, &shmPntr, &semPntr) < 0){
    printf("Initalizing shared memory and semaphor failed...");
    return(-1);
  }

  // Lock semaphore while reading/writing
  sem_wait(&semPntr);

  // Write args to var
  shmPntr[0]=atoi(argv[1]);
  shmPntr[1]=atoi(argv[2]);

  // Unlock semaphore for reading/writing
  sem_post(&semPntr);

  // Close out memory
  shm_unlink("shmPntr");
  sem_close(semPntr);

  // Exit
  return(0);
}
