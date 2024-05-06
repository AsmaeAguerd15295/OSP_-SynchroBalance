#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    key_t key = ftok(".", 'a');
    int shmid = shmget(key, sizeof(int), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    int* balance = shmat(shmid, NULL, 0);
    if (balance == (int*)-1) {
        perror("shmat");
        exit(1);
    }

    sem_t *sem1 = sem_open("/semaphore1", 0);
    if (sem1 == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    sem_t *sem2 = sem_open("/semaphore2", 0);
    if (sem2 == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    int flag = 1;

    while (flag) {
        sem_wait(sem1);
        sem_wait(sem2);
        *balance -= 5;
        printf("Process 2: Balance is now %d\n", *balance);
        sem_post(sem2);
        sem_post(sem1);
        flag = 0; // Set flag to 0 to exit after one iteration
        // Remove this line if you want the process to run indefinitely
        sleep(1); // Simulate some work
    }

    shmdt(balance);

    return 0;
}
