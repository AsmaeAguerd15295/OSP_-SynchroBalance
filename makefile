CC=gcc
CFLAGS=-I.
DEPS = semaphores.h shared_data.h
OBJ = semaphores.o initializer.o process1.o process2.o process3.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: initializer process1 process2 process3

initializer: initializer.o semaphores.o
	$(CC) -o $@ $^ $(CFLAGS)

process1: process1.o semaphores.o
	$(CC) -o $@ $^ $(CFLAGS)

process2: process2.o semaphores.o
	$(CC) -o $@ $^ $(CFLAGS)

process3: process3.o semaphores.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o initializer process1 process2 process3

.PHONY: all clean
