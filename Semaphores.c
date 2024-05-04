#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>



sem_t S1; // Declare semaphore S1
sem_t S2; // Declare semaphore S2

void initialize_semaphore(sem_t *sem, int value) {
    if (sem_init(sem, 0, value) == -1) {
        perror("Failed to initialize semaphore");
        exit(EXIT_FAILURE);
    }
}


//Wait operation on a semaphore
void wait(sem_t *sem) {
    if (sem_wait(sem) == -1) {
        perror("Failed to wait on semaphore");
        exit(EXIT_FAILURE);
    }
}

// Signal operation on a semaphore
void signal(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Failed to signal semaphore");
        exit(EXIT_FAILURE);
    }
}


