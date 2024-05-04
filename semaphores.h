void wait(int *sem) {
    while (*sem <= 0)
        ;
    (*sem)--;
}

void signal(int *sem) {
    (*sem)++;
}
