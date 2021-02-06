// Definitions
#define SHMKEY 893422
#define SHMSIZE 8
#define SEMNAME "vecratsem"

// Vars
int shmId;
int *shmPntr;
sem_t *semPntr;

int memSetup(int *shmId, int **shmPntr, int **semPntr){
  // Map pointer to memory w/ SHMKEY
  if((*shmId=shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0770)) < 0){
    printf("Unable to get memory space...\n");
    perror("shmget");
    return(-1);
  }
  else if((*shmPntr=shmat(*shmId, NULL, IPC_CREAT | 0770)) < 0){
    printf("Unable to get memory space...\n");
    perror("shmmat");
    return(-1);
  }

  // Create semaphore for locking
  if((*semPntr=sem_open(SEMNAME, IPC_CREAT, S_IRWXU, 0)) < SEM_FAILED && semPntr!=NULL){
    printf("Lock memory failed...\n");
    perror("sem_open");
    return(-1);
  }

  // Initialize semaphore
  if((sem_init(semPntr, 1, 1)) < 0){
    printf("Initializing semaphore...\n");
    perror("sem_init");
    return(-1);
  }

  return(0);
}
