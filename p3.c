#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphores.h"

#define KEY 12345
#define SIZE (3 * sizeof(int))  // Space for 3 integers: S1, S2, and balance

int main() {
    int shmid;
    int *shared_data;
    int *balance;

    // Access the shared memory segment
    shmid = shmget(KEY, SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory segment
    shared_data = (int *)shmat(shmid, NULL, 0);
    if (shared_data == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphore variables
     initialize_semaphore(&S2,1);
    

    // Initialize pointer for balance
    balance = (int *)(shared_data + 2);  // Third integer as balance

    // Manipulate the balance
    wait(&S2);           // Wait to acquire S2 semaphore
    *balance *= 2;       // Modify the shared balance
    signal(&S2);         // Signal to release S2 semaphore

    printf("The updated balance is: %d from client 3\n", *balance);

    // Detach from the shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
