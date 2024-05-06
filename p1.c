#include "shared_data.h"
#include "semaphores.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    int shmid;
    shared_data *data;

    // Attach to the shared memory
    shmid = shmget(KEY, sizeof(shared_data), 0666);
    data = (shared_data *)shmat(shmid, NULL, 0);

    wait(&data->semaphore1);
    data->balance -= 20;
    printf("Process 1 modified balance to: %d\n", data->balance);
    signal(&data->semaphore1);

    shmdt(data);
    return 0;
}
