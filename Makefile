CC=gcc
CFLAGS=-Wall -g -m32
TARGET=threads.o

all: $(TARGET)

threads.o: threads.h threads.c
	$(CC) -c -o threads.o threads.c $(CFLAGS)

clean:
	rm $(TARGET)
