// Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include "memsetup.h"


int main(int argc, char **argv){
  // Setup shared memory for client
  if((memClientSetup(&shmId, &shmPntr)) < 0){
    printf("Initializing shared memory failed...\n");
    return(-1);
  }

  // Setup semaphore
  if(semSetup(&semPntr) < 0){
    printf("Initalizing shared memory and semaphor failed...\n");
    return(-1);
  }


  // Lock semaphore while writing
  sem_wait(&semPntr);

  // Write args to var
  shmPntr[0]=atoi(argv[1]);
  shmPntr[1]=atoi(argv[2]);

  // Unlock semaphore for writing
  sem_post(&semPntr);

  // Close out memory
  shm_unlink("shmPntr");
  sem_close(semPntr);
  sem_unlink(semPntr);


  // Exit
  return(0);
}
