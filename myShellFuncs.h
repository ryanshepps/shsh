#ifndef _MYSHELL_FUNCS
    #define _MYSHELL_FUNCS
    #define BUFFERSIZE 1024
    #define MAXPROCESSES 10

    typedef struct process {
        int id;
        int pid;
        char command[1024];
    } bgprocess;

    char parse_buffer(char buffer[], char* parameters[]);
    void new_process(char* command, char* parameters[], char action);
    void command_chdir(char* parameters[]);
    void command_redirect_to(char* command, char* parameters[]);
    void command_redirect_from(char *command, char* parameters[]);
    void command_background(char *command, char* parameters[], bgprocess* processes);
    void sig_background(int signo);

    /* Helper Functions */
    char* cur_dir(char dir[]);
    void reap_processes(bgprocess processes[MAXPROCESSES]);
#endif