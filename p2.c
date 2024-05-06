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
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    data = (shared_data *)shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Wait on semaphore S1 (between P1 and P2)
    wait(&data->semaphore1);
    // Perform necessary operation on the balance
    data->balance += 10; // Example operation
    printf("P2 after P1, balance: %d\n", data->balance);
    // Signal semaphore S1 to possibly let P1 continue
    signal(&data->semaphore1);

    // Wait on semaphore S2 (between P2 and P3)
    wait(&data->semaphore2);
    // Perform necessary operation on the balance
    data->balance -= 5; // Example operation
    printf("P2 before P3, balance: %d\n", data->balance);
    // Signal semaphore S2 to allow P3 to proceed
    signal(&data->semaphore2);

    // Detach from shared memory
    shmdt(data);
    return 0;
}
