#include "semaphores.h"

void wait(volatile int *sem) {
    while (*sem <= 0)
        ; // Busy wait
    __sync_fetch_and_sub(sem, 1); // Atomically decrement semaphore
}

void signal(volatile int *sem) {
    __sync_fetch_and_add(sem, 1); // Atomically increment semaphore
}
