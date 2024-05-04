#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./semaphores.h"

#define BALANCE_FILE "./balance.txt"

int main() {
    sem_t S1;
    initialize_semaphore(&S1, 1); // Initialize S1 to 1

    FILE *file = fopen(BALANCE_FILE, "r+");
    if (file == NULL) {
        perror("Failed to open balance file");
        exit(EXIT_FAILURE);
    }

    int balance;
    fscanf(file, "%d", &balance);
    fclose(file);

    wait(&S1); // Wait to acquire S1 semaphore
    balance -= 20; // Modify the balance
    file = fopen(BALANCE_FILE, "w");
    if (file == NULL) {
        perror("Failed to open balance file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d", balance);
    fclose(file);
    signal(&S1); // Signal to release S1 semaphore

    printf("The updated balance is: %d from client 1\n", balance);

    return 0;
}
void signal(sem_t *sem) {
    if (sem_post(sem) == -1) {
        perror("Failed to signal semaphore");
        exit(EXIT_FAILURE);}}
