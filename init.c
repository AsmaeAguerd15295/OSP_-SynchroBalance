#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345

int main() {
    int shmid;
    int *shared_data;
    int choice;

    // Access the shared memory segment
    shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666);
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

    // Initialize balance to 100
    *shared_data = 100;

    // Detach from the shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Prompt for choice
    printf("Choose program to run:\n");
    printf("1. pro1\n");
    printf("2. pro2\n");
    printf("3. pro3\n");
    scanf("%d", &choice);

    // Run selected program
    switch (choice) {
        case 1:
            system("./pro1");
            break;
        case 2:
            system("./pro2");
            break;
        case 3:
            system("./pro3");
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }

    return 0;
}
