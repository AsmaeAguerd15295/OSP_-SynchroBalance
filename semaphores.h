#ifndef SEM_UTILS_H
#define SEM_UTILS_H

void wait(volatile int *sem);
void signal(volatile int *sem);

#endif // SEM_UTILS_H
