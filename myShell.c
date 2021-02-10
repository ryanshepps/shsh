#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include "myShellFuncs.h"

int main (int argc, char *argv[]) {
    char buffer[BUFFERSIZE];
    char *parameters[10];
    char dir[PATH_MAX];
    char action;
    int status;
    bgprocess processes[MAXPROCESSES] = { 0 };

    while (1) {
        // DEBUGGING
        // Printing the processes struct array to make sure all is good
        // for (int i = 0; i < MAXPROCESSES; i++) {
        //     printf("PROCESS %d:\npid: %d\ncommand: %s\n\n", processes[i].id, processes[i].pid, processes[i].command);
        // }

        // Displaying background processes when they have completed
        pid_t pid = waitpid(-1, &status, WNOHANG);
        for (int i = 0; i < MAXPROCESSES; i++) {
            if (processes[i].pid == pid && pid != -1 && pid != 0) {
                printf("[%d]+  Done\t\t\t%s\n", processes[i].id, processes[i].command);
                // Clean up processes struct
                processes[i].pid = 0;
                processes[i].id = 0;
                strcpy(processes[i].command, " ");
                break;
            }
        }

        printf("%s> ", cur_dir(dir));

        fgets(buffer, BUFFERSIZE, stdin);
        
        action = parse_buffer(buffer, parameters);

        if (strncmp(parameters[0], "exit", 4) != 0) {
            if (action == ' ') {
                // DEBUGGING
                // printf("Executing bash process...\n");
                new_process(parameters[0], parameters, action);
            } else {
                // DEBUGGING
                // printf("Executing custom process...\n");
                if (action == '>') {
                    command_redirect_to(parameters[0], parameters);
                } else if (action == '<') {
                    command_redirect_from(parameters[0], parameters);
                } else if (action == '&') {
                    command_background(parameters[0], parameters, processes);
                }
            }
        } else {
            printf("myShell terminating...\n\n[Process completed]\n");
            exit(0);
        }
    }
}
