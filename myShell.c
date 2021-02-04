#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "myShellFuncs.h"

int main (int argc, char *argv[]) {
    char buffer[BUFFERSIZE];
    char *parameters[10];
    char dir[PATH_MAX];

    while (1) {
        printf("%s> ", cur_dir(dir));

        fgets(buffer, BUFFERSIZE, stdin);
        
        parse_buffer(buffer, parameters);

        if (strncmp(parameters[0], "exit", 4) != 0) {
            new_process(parameters[0], parameters);
        } else {
            printf("myShell terminating...\n\n[Process completed]\n");
            exit(0);
        }
    }
}
