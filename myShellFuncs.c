#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include "myShellFuncs.h"

char parse_buffer(char* buffer, char* parameters[]) {
    char return_val = ' ';

    int iterator = 0;
    char* token = strtok(buffer, " ");
    while (token != NULL) {
        parameters[iterator] = token;
        iterator++;

        if (strcmp(token, ">") == 0) {
            return_val = '>';
        } else if (strcmp(token, "<") == 0) {
            return_val = '<';
        }

        token = strtok(NULL, " ");
    }
    
    // Removes the trailing \n
    strtok(parameters[--iterator], "\n");

    // Determines the return_val
    if (strcmp(parameters[iterator], "&") == 0) {
        return_val = '&';
    } else if (strcmp(parameters[iterator], ">") == 0) {
        return_val = '>';
    } else if (strcmp(parameters[iterator], "<") == 0) {
        return_val = '<';
    }
    // Ends the parameter list with a trailing NULL pointer
    parameters[++iterator] = NULL;

    return return_val;
}

int new_process(char* command, char* parameters[], char action) {
    pid_t childpid;
    int status;

    childpid = fork();

    if (childpid >= 0) {
        // Child process
        if (childpid == 0) {
            status = execvp(command, parameters);
            exit(status);
        } else {
            waitpid(childpid, &status, 0);
        }
    } else {
        perror("There was an error in forking your process :(\n");
        exit(-1);
    }
}

char* cur_dir(char dir[]) {
    char* ret_val;

    ret_val = getcwd(dir, PATH_MAX);
    if (ret_val == NULL) {
        fprintf(stderr, "ERROR %d: Something went wrong with getting the current working director\n", strerror(errno));
        return NULL;
    }

    strcat(dir, "/");
    return dir;
}

int new_custom_process(char* command, char* parameters[], char action) {
    if (action == '>') {
        command_redirect_to(command, parameters);
    }

    return 0;
}

int command_redirect_to(char *command, char *parameters[]) {
    printf("FOUND REDIRECT TO FILE ACTION\n");
}

// int command_cd(char* new_dir) {
//     int status;
    
//     status = chdir(new_dir);
//     if (status == 0) {
//         return 1; // Success!
//     } else {
//         fprintf(stderr, "ERROR %d: Directory does not exist!\n", strerror(errno));
//         return -1;
//     }
// }