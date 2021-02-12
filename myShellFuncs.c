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
        } else if (strcmp(token, "cd") == 0) {
            return_val = 'c';
        } else if (strcmp(token, "history") == 0 || strcmp(token, "history\n") == 0) {
            return_val = 'h';
        } else if (strcmp(token, "echo") == 0) {
            return_val = 'e';
        } else if (strcmp(token, "export") == 0) {
            return_val = 'x';
        }

        token = strtok(NULL, " ");
        while (token == " ") {
            token = strtok(NULL, " ");
        }
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

void command_chdir(char* parameters[]) {
    int ret;
    if (strcmp(parameters[1], "~") == 0) {
        ret = chdir(getenv("HOME"));
    } else {
        ret = chdir(parameters[1]);
        if (ret = -1) {
            if (errno == 2) {
                fprintf(stderr, "ERROR %d: Directory does not exist!\n", errno);
            }
        }
    }
}

void command_history(int* num_commands, char* parameters[], bgprocess* processes) {
    char hist_file_location[1024];
    strcpy(hist_file_location, getenv("HISTFILE"));
    hist_file_location[strlen(hist_file_location)] = '\0';
    if (parameters[1] == NULL) {
        FILE* fp = fopen(hist_file_location, "r");
        char c;
        while ((c = getc(fp)) != EOF) {
            printf("%c", c);
        }
        fclose(fp);
     } else if (strcmp(parameters[1], "-c") == 0) {
        FILE* fp = fopen(hist_file_location, "w+");
        fclose(fp);
        *num_commands = 1;
     } else if (atoi(parameters[1]) != 0) {
        FILE* fp = fopen(hist_file_location, "r");

        int index = atoi(parameters[1]);

        char command[1024];
        while (fgets(command, 1024, fp) != NULL) {
            if (atoi(command) == index) {
                char* parameters[10];
                char action = parse_buffer(command, parameters);

                // Moving everything to the left (To write over the first parameter which is a number)
                for (int i = 0; parameters[i] != NULL; i++) {
                    parameters[i] = parameters[i + 1];
                }

                // DEBUGGING
                // printf("BACKGROUND: Executing %s with:\n", parameters[0]);
                // for (int i = 0; parameters[i] != NULL; i++) {
                //     printf("%d: %s\n", i, parameters[i]);
                // }
                
                if (strncmp(parameters[0], "exit", 4) == 0) {
                    printf("myShell terminating...\n\n[Process completed]\n");
                    exit(0);
                }

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
                        command_history(num_commands, parameters, processes);
                    } else if (action == 'e') {
                        command_echo(parameters);
                    }
                }
                fclose(fp);
                return;
            }
        } printf("Index %d does not exist in .CIS3110_history\n");
        
        fclose(fp);
     } else {
         printf("-myShell: %s: Unrecognized string\n", parameters[1]);
     }
}

void initialize_profile(bgprocess* processes) {
    char buffer[1024];
    char* home = malloc(sizeof(char) * 1024);
    char* path = malloc(sizeof(char) * 1024);
    char* histfile = malloc(sizeof(char) * 1024);

    FILE *fp = fopen(".CIS3110_profile", "r");
    // If it doesn't exist, create the profile
    if (fp == NULL) {
        fp = fopen(".CIS3110_profile", "w+");
        if (fp == NULL) {
            fprintf(stderr, "There was an error in opening the .CIS3110_profile file\n");
            return;
        }
        
        char dir[PATH_MAX];
        sprintf(home, "HOME=%s", cur_dir(dir));
        fprintf(fp, "export %s\n", home);
        if (putenv(home) != 0) {
            printf("There was an error setting your home directory!\n");
            exit(1);
        }

        sprintf(path, "PATH=/usr/bin:/bin:%s", getenv("HOME"));
        fprintf(fp, "export %s\n", path);
        printf("PATH: %s\n", path);
        if (putenv(path) != 0) {
            printf("There was an error setting your home directory!\n");
            exit(1);
        }

        sprintf(histfile, "HISTFILE=%s.CIS3110_history", getenv("HOME"));
        fprintf(fp, "export %s\n", histfile);
        printf("HISTFILE: %s\n", histfile);
        if (putenv(path) != 0) {
            printf("There was an error setting your home directory!\n");
            exit(1);
        }
    } else {
        char action;
        char *parameters[10];

        while (fgets(buffer, 1024, fp) != NULL){
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
                    } else if (action == 'c') {
                        command_chdir(parameters);
                    } else if (action == 'h') {
                        command_history(0, parameters, processes);
                    } else if (action == 'e') {
                        command_echo(parameters);
                    } else if (action == 'x') {
                        command_export(parameters);
                    }
                }
            } else {
                printf("myShell terminating...\n\n[Process completed]\n");
                exit(0);
            }
        }
    }

    fclose(fp);
}

/**************************** HELPER FUNCTIONS *******************************/
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

void reap_processes(bgprocess processes[MAXPROCESSES]) {
        int status;
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
}

void append_to_history(int* num_commands, char* buffer[]) {
    FILE* history_file = fopen(getenv("HISTFILE"), "a+");
    if (history_file == NULL) {
        fprintf(stderr, "There was an error in opening the .CIS3110_history file\n");
        return;
    }
    
    char string_to_append[1024] = "";
    sprintf(string_to_append, " %d  ", *num_commands);
    for (int i = 0; buffer[i] != NULL; i++) {
        strcat(string_to_append, buffer[i]);
        strcat(string_to_append, " ");
    } 
    string_to_append[strlen(string_to_append) - 1] = '\n';

    fprintf(history_file, string_to_append);
    fclose(history_file);

    (*num_commands)++;
}

int get_current_num_commands() {
    FILE* fp = fopen(getenv("HISTFILE"), "r");
    if (fp == NULL) {
        fp = fopen(getenv("HISTFILE"), "w+");
        if (fp == NULL) {
            fprintf(stderr, "There was an error in opening the .CIS3110_history file\n");
            return 0;
        }

        return 0;
    }
    char str_curr_num[1024];
    int curr_num;
    
    while (fgets(str_curr_num, 1024, fp) != NULL) {
        curr_num = atoi(str_curr_num);
    }
    
    return curr_num;
}

void command_echo(char* parameters[]) {
    if (strcmp(parameters[1], "$HOME") == 0) {
        if (getenv("HOME") == NULL) {
            printf("There is an error with retreiving this environment variable\n");
            return;
        }
        printf("%s\n", getenv("HOME"));
    } else if (strcmp(parameters[1], "$PATH") == 0) {
        if (getenv("PATH") == NULL) {
            printf("There is an error with retreiving this environment variable\n");
            return;
        }
        printf("%s\n", getenv("PATH"));
    } else if (strcmp(parameters[1], "$HISTFILE") == 0) {
        if (getenv("HISTFILE") == NULL) {
            printf("There is an error with retreiving this environment variable\n");
            return;
        }
        printf("%s\n", getenv("HISTFILE"));
    } else {
        new_process(parameters[0], parameters, ' ');
    }
}

void command_export(char* parameters[]) {
    if (parameters[1] == NULL) {
        printf("ERROR: Must have a second argument\n");
        return;
    }

    char* newEnvironVar = malloc(sizeof(char) * 1024);
    strcpy(newEnvironVar, parameters[1]);
    if (putenv(newEnvironVar) != 0) {
        printf("Error setting your environment variable!\n");
    }
}