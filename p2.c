#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "semaphores.h"

#define BALANCE_FILE "balance.txt"

int main() {
    sem_t S1, S2;
    initialize_semaphore(&S1, 1); // Initialize S1 to 1
    initialize_semaphore(&S2, 1); // Initialize S2 to 1

    FILE *file = fopen(BALANCE_FILE, "r+");
    if (file == NULL) {
        perror("Failed to open balance file");
        exit(EXIT_FAILURE);
    }

    int balance;
    fscanf(file, "%d", &balance);
    fclose(file);

    wait(&S1); // Wait to acquire S1 semaphore
    wait(&S2); // Wait to acquire S2 semaphore
    balance -= 10; // Modify the balance
    file = fopen(BALANCE_FILE, "w");
    if (file == NULL) {
        perror("Failed to open balance file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d", balance);
    fclose(file);
    signal(&S1); // Signal to release S1 semaphore
    signal(&S2); // Signal to release S2 semaphore

    printf("The updated balance is: %d from client 2\n", balance);

    return 0;
}
