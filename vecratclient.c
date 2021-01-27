#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHMKEY 893422
#define SHMSIZE 8
#define SEMNAME "vecratsem"

int shmId;
int *shmPntr;
sem_t *semPntr;


int main(int argc, char **argv){
  // Map pointer to memory w/ SHMKEY
  if((shmId=shmget(SHMKEY, SHMSIZE, IPC_CREAT | O_RDWR)) < 0){
    printf("Unable to get memory space...\n");
    perror("shmget");
    return(0);
  }
  else if((shmPntr=shmat(shmId, NULL, IPC_CREAT | O_RDWR)) < 0){
    printf("Unable to get memory space...\n");
    perror("shmmat");
    return(0);
  }

  // Lock memory
  if((semPntr=sem_open(SEMNAME, IPC_CREAT, S_IRWXU, 0)) < 0){
    printf("Unable to set semaphore struct...\n");
    perror("sem_open");
    return(0);
  }

  // Initialize semaphore
  if(sem_init(&semPntr, 1, 1) < 0){
    printf("Initializing semaphore...\n");
    perror("sem_init");
    return(0);
  }

  // Lock semaphore while reading/writing
  sem_wait(&semPntr);

  // Write args to var
  shmPntr[0]=atoi(argv[1]);
  shmPntr[1]=atoi(argv[2]);

  // Unlock semaphore for reading/writing
  sem_post(&semPntr);

  // Close out memory
  shm_unlink(shmPntr);
  sem_close(semPntr);

  // Exit
  return(0);
}
