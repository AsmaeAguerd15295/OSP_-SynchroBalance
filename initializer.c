#include "shared_data.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    int shmid;
    shared_data *data;
    shmid = shmget(KEY, sizeof(shared_data), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget failed");
        exit(1);
    }

    // Attach to the shared memory segment
    data = (shared_data *)shmat(shmid, NULL, 0);
    if (data == (void *)-1)
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    printf("here");
    // Initialize the shared data
    data->balance = 100;  // Initial balance
    data->semaphore1 = 1; // Initial value for semaphore1 should be 1 to allow access
    data->semaphore2 = 1; // Initial value for semaphore2 should be 1 to allow access

    // Detach from the shared memory (not destroy, just detach)
    shmdt(data);

    printf("Shared memory and semaphores initialized.\n");
    return 0;
}
