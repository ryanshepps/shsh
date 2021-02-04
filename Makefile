CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

all: myShell.out

clean:
	rm *.out

myShell.out: myShell.c
	$(CC) $(CFLAGS) myShell.c -o myShell.out