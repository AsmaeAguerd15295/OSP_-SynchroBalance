#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_BALANCE 100

volatile sig_atomic_t child_finished = 0;

void handle_signal(int signal) {
    child_finished = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <process_order>\n", argv[0]);
        return 1;
    }

    int process_order = atoi(argv[1]);
    if (process_order < 1 || process_order > 3) {
        fprintf(stderr, "Invalid process order. Use a number between 1 and 3.\n");
        return 1;
    }

    signal(SIGCHLD, handle_signal); // Register signal handler

    key_t key = ftok(".", 'a');
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    int* balance = shmat(shmid, NULL, 0);
    if (balance == (int*)-1) {
        perror("shmat");
        exit(1);
    }

    sem_t *sem1 = sem_open("/semaphore1", O_CREAT, SEM_PERMS, 1);
    sem_t *sem2 = sem_open("/semaphore2", O_CREAT, SEM_PERMS, 1);

    *balance = INITIAL_BALANCE;

    pid_t pid1, pid2, pid3;

    if (process_order == 1) {
        pid1 = fork();

        if (pid1 < 0) {
            perror("fork");
            exit(1);
        } else if (pid1 == 0) {
            execl("./process1", "process1", NULL);
            perror("execl");
            exit(1);
        }
    } else if (process_order == 2) {
        pid2 = fork();

        if (pid2 < 0) {
            perror("fork");
            exit(1);
        } else if (pid2 == 0) {
            execl("./process2", "process2", NULL);
            perror("execl");
            exit(1);
        }
    } else if (process_order == 3) {
        pid3 = fork();

        if (pid3 < 0) {
            perror("fork");
            exit(1);
        } else if (pid3 == 0) {
            execl("./process3", "process3", NULL);
            perror("execl");
            exit(1);
        }
    }

    // Wait for all child processes to finish
    while (!child_finished) {
        wait(NULL);
    
