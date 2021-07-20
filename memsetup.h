#ifndef _MEMSETUPH
#define _MEMSETUPH


#include <semaphore.h>


int getShmBlk(char *fileName);
int *getShmPntr(char *fileName);
int *createShmReader();
int *createShmWriter();
sem_t *createSemWriter();
sem_t *createSemReader();
int destroyShmSemReader(int *shmPntr, sem_t *semPntr);
int destroyShmSemWriter(int *shmPntr, sem_t *semPntr);
int checkIfShmFileExists();
int removeShmSemFile();


#endif // _MEMSETUPH
