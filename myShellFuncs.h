#ifndef _MYSHELL_FUNCS
    #define _MYSHELL_FUNCS
    #define BUFFERSIZE 1024

    char parse_buffer(char buffer[], char* parameters[]);
    int new_process(char* command, char* parameters[], char action);
    char* cur_dir(char dir[]);
    int new_custom_process(char* command, char* parameters[], char action);
    int command_cd(char* new_dir);
    int command_redirect_to(char *command, char *parameters[]);
#endif