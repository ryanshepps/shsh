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

    // History file stuff
    int num_commands = get_current_num_commands() + 1;
    int* ptr_num_commands = &num_commands;

    char* intro = "\n"
                  "Ryan's\n"
                  "    ___  _ _________   ___  _ _____ __  __\n"
                  "  ,' _/ /// / /_  _/ ,' _/ /// / _// / / /\n"
                  " _\\ `. / ` / / / /  _\\ `. / ` / _// /_/ /_\n"
                  "/___,'/_n_/_/ /_/  /___,'/_n_/___/___/___/\n"
                  "\n"
                  "Some of the worst code I've ever written lies in the source code.\n"
                  "View the README for limitations and implementations.\n"
                  "\n"
                  "I'm sorry you have to mark this. Please email rshepp02@uoguelph.ca if\n"
                  "I can clear anything up or do something to make your marking easier.\n";

    printf("%s\n", intro);



    while (1) {
        // DEBUGGING
        // Printing the processes struct array to make sure all is good
        // for (int i = 0; i < MAXPROCESSES; i++) {
        //     printf("PROCESS %d:\npid: %d\ncommand: %s\n\n", processes[i].id, processes[i].pid, processes[i].command);
        // }

        reap_processes(processes);

        printf("%s> ", cur_dir(dir));

        fgets(buffer, BUFFERSIZE, stdin);
        
        action = parse_buffer(buffer, parameters);

        append_to_history(ptr_num_commands, parameters);

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
                } else if (action == 'c') {
                    command_chdir(parameters);
                } else if (action == 'h') {
                    command_history(ptr_num_commands, parameters, processes);
                }
            }
        } else {
            printf("myShell terminating...\n\n[Process completed]\n");
            exit(0);
        }
    }
}
