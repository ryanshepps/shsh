CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

all: myShell.out

clean:
	rm *.out *.o .CIS3110_history .CIS3110_profile

myShell.out: myShell.c myShellFuncs.o
	$(CC) $(CFLAGS) myShellFuncs.o myShell.c -o myShell.out

myShellFuncs.o: myShellFuncs.c
	$(CC) $(CFLAGS) -c myShellFuncs.c -o myShellFuncs.o