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
    parameters[++iterator] = (char *)NULL;

    // DEBUGGING
    // printf("Parameters:\n");
    // for (int i = 0; parameters[i] != NULL; i++) {
    //     printf("%d: %s\n", i, parameters[i]);
    // }

    // printf("Action: %c\n", return_val);

    return return_val;
}

void new_process(char* command, char* parameters[], char action) {
    pid_t childpid;
    int status;

    childpid = fork();

    if (childpid >= 0) {
        // Child process
        if (childpid == 0) {
            status = execvp(command, parameters);
            printf("-bash: %s: command not found\n", command);
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

void command_redirect_to(char *command, char *parameters[]) {
    char* newParameterList[10];
    char* filename;
    
    // Parsing the parameter list
    for (int i = 0; strcmp(parameters[i], ">") != 0; i++) {
        newParameterList[i] = parameters[i];
        newParameterList[i + 1] = NULL; // NULL terminating the new parameter list
        filename = parameters[i + 2]; 
    }

    // DEBUGGING
    // printf("REDIRCT_TO: Executing %s with:\n", newParameterList[0]);
    // for (int i = 0; newParameterList[i] != NULL; i++) {
    //     printf("%d: %s\n", i,newParameterList[i]);
    // }

    pid_t childpid;
    int status;
    FILE *fp;

    childpid = fork();

    if (childpid >= 0) {
        // Child process
        if (childpid == 0) {
            fp = freopen(filename, "w+", stdout);
            if (fp == NULL) {
                printf("-myShell: %s: Error writing to file\n");
                exit(-1);
            }
            status = execvp(newParameterList[0], newParameterList);
            printf("-bash: %s: command not found\n", newParameterList[0]);
            exit(status);
        } else {
            waitpid(childpid, &status, 0);
        }
    } else {
        perror("There was an error in forking your process :(\n");
        exit(-1);
    }
}

void command_redirect_from(char* command, char* parameters[]) {
    char* newParametersList[10];
    char* filename;

    // Parsing the parameter list
    for (int i = 0; strcmp(parameters[i], "<") != 0; i++) {
        newParametersList[i] = parameters[i];
        newParametersList[i + 1] = NULL; // NULL terminating the parameter list
        filename = parameters[i + 2];
    }

    // DEBUGGING
    // printf("REDIRCT_FROM: Executing %s with:\n", newParametersList[0]);
    // for (int i = 0; newParametersList[i] != NULL; i++) {
    //     printf("%d: %s\n", i,newParametersList[i]);
    // }
    
    pid_t childpid;
    int status;
    FILE *fp;

    childpid = fork();

    if (childpid >= 0) {
        if (childpid == 0) {
            fp = freopen(filename, "r", stdin);
            if (fp == NULL) {
                printf("-myShell: %s: No such file or directory\n", filename);
                exit(-1);
            }
            status = execvp(newParametersList[0], newParametersList);
            printf("-bash: %s: command not found\n", newParametersList[0]);
            exit(status);
        } else {
            waitpid(childpid, &status, 0);
        }
    } else {
        perror("There was an error in forking your process :(\n");
        exit(-1);
    }
}

void command_background(char *command, char* parameters[], bgprocess* processes) {
    // DEBUGGING
    // printf("Executing command_background...\n");
    pid_t childpid;
    int status;

    // Finding & and removing it
    for (int i = 0; parameters[i] != NULL; i++) {
        if (strcmp(parameters[i], "&") == 0) {
            parameters[i] = NULL;
        }
    }

    // Creating a new background process
    int iterator = 0;
    while (processes[iterator].id != 0) {
        iterator++;
    }
    processes[iterator].id = iterator + 1;

    char entire_command[1024] = "";
    for (int i = 0; parameters[i] != NULL; i++) {
        strcat(entire_command, parameters[i]);
        strcat(entire_command, " ");
    }
    strcpy(processes[iterator].command, entire_command);

    // DEBUGGING
    // printf("BACKGROUND: Executing %s with:\n", command);
    // for (int i = 0; parameters[i] != NULL; i++) {
    //     printf("%d: %s\n", i, parameters[i]);
    // }
    childpid = fork();

    if (childpid >= 0) {
        // Child process
        if (childpid == 0) {
            status = execvp(command, parameters);
            printf("-bash: %s: command not found\n", command);
            exit(status);
        } else {
            printf("[%d] %d\n", iterator + 1, childpid);
            processes[iterator].pid = childpid;
        }
    } else {
        perror("There was an error in forking your process :(\n");
        exit(-1);
    }
} 