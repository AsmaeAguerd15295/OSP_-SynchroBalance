#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphores.h"  
#define KEY 12345
#define SIZE (2 * sizeof(int))  // Space for 2 integers: semaphore and balance

int main() {
    int shmid;
    int *shared_data;
    int *semaphore, *balance;

    // access the shared memory segment
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

    // Initialize pointers for semaphore and balance
    semaphore = shared_data;   // First integer as semaphore
    balance = shared_data + 1; // Second integer as balance

    // Manipulate the balance
    wait(semaphore);      // Wait to acquire the semaphore before accessing balance
    sleep(5);
    *balance -= 10;       // Modify the shared balance
    signal(semaphore);    // Signal to release the semaphore after modifying the balance

    printf("The updated balance is: %d from client 2\n", *balance);

    // Optionally print the semaphore value for debugging
    printf("Semaphore value after signal: %d\n", *semaphore);

    // Detach from the shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
