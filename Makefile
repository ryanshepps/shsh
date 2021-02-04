CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

all: myShell.out

clean:
	rm *.out *.o

myShell.out: myShell.c myShellFuncs.o
	$(CC) $(CFLAGS) myShellFuncs.o myShell.c -o myShell.out

myShellFuncs.o: myShellFuncs.c
	$(CC) $(CFLAGS) -c myShellFuncs.c -o myShellFuncs.o