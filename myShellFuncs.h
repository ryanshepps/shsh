#ifndef _MYSHELL_FUNCS
    #define _MYSHELL_FUNCS
    #define BUFFERSIZE 1024

    void parse_buffer(char buffer[], char* parameters[]);
    void new_process(char* command, char* parameters[]);
    char* cur_dir(char dir[]);
    int new_custom_process(char* command, char* parameters[]);
    int command_cd(char* new_dir);
#endif