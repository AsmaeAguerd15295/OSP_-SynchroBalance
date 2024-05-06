#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <sys/types.h>
#define KEY 3214
// Structure to be placed in shared memory
typedef struct {
    int balance;
     int semaphore1; // Manual semaphore
     int semaphore2; // Manual semaphore
} shared_data;



#endif // SHARED_DATA_H
