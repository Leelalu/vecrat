// Includes //
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>


// Definition //
#define SHMFILE "/dev/shm/vecratoffset"
#define SHMSIZE 8
#define SEMNAME "vecrat.sem"


// Functions
// Get block of shared memory
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
  }

  return(shmBlkId);
}

// Setup pntr to shared memory
int *getShmPntr(char *fileName){
  int shmBlkId;
  int *shmBlkPntr;

  shmBlkId=getShmBlk(fileName);

  shmBlkPntr=shmat(shmBlkId, NULL, 0);
  if(shmBlkPntr < 0){
    perror("shmat");
  }

  return(shmBlkPntr);
}

// Create shared memory
int *createShmReader(){
  int *shmPntr;
  if(access(SHMFILE, F_OK)==0){
    printf("Found shared memory file at %s...\n", SHMFILE);
  }
  else{
    open(SHMFILE, O_RDWR | O_CREAT, 0777);
  }

  shmPntr=getShmPntr(SHMFILE);
  if(shmPntr<0){
    printf("Failed setting up shared memory...");
  }

  return(shmPntr);
}

int *createShmWriter(){
  int *shmPntr;

  shmPntr=getShmPntr(SHMFILE);
  if(shmPntr<0){
    printf("Failed setting up shared memory...");
  }

  return(shmPntr);
}

// Create semaphore
sem_t *createSemReader(){
  sem_t *semPntr;

  sem_unlink(SEMNAME);
  semPntr=sem_open(SEMNAME, O_CREAT, 0643, 1);
  if(semPntr<0){
    printf("Failed setting up semaphore...");
  }

  return(semPntr);
}

sem_t *createSemWriter(){
  sem_t *semPntr;
  semPntr=sem_open(SEMNAME, 0);
  if(semPntr<0){
    printf("Failed setting up semaphore...");
  }
  return(semPntr);
}

// Destroy Shared memory and semaphore
int destroyShmSemReader(int *shmPntr, sem_t *semPntr){
  shmdt(shmPntr);
  shmctl(getShmBlk(SHMFILE), IPC_RMID, NULL);
  sem_close(semPntr);
  sem_unlink(SEMNAME);
  remove(SHMFILE);
  return(0);
}

int destroyShmSemWriter(int *shmPntr, sem_t *semPntr){
  shmdt(shmPntr);
  sem_close(semPntr);
  return(0);
}

// Check for existing shared memory file
int checkIfShmExists(){
  if(access(SHMFILE, F_OK)==0){
    printf("Found shared memory file at %s...\n", SHMFILE);
    return(1);
  }
  else{
    return(0);
  }
}
