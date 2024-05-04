#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <semaphore.h>

extern sem_t S1;
extern sem_t S2;


void initialize_semaphore(sem_t *sem, int value);
void wait(sem_t *sem);
void signal(sem_t *sem);

#endif

