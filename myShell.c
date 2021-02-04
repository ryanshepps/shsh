#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFFERSIZE 1024

int main (int argc, char *argv[]) {
    pid_t childpid; // Child's process ID
    int status = 0; // The Child's exit status
    char buffer[BUFFERSIZE];
    char *parameters[10] = { "ls", "-l", NULL };

    while(1) {
    childpid = fork();
    if (childpid >= 0) {
        if (childpid == 0) {
            // Getting user's command
            char c;
            int position = 0;
            while (c != '\n' && c != EOF) {
                c = getchar();
                buffer[position] = c;
                position++;
            }
            
            // Replacing the \n with an EOF
            buffer[--position] = '\0';

            status = execvp(buffer, parameters);
        } else {
            waitpid(childpid, &status, 0);
            printf("parent process stuff\n");
            printf("child's exit code is %d\n", WEXITSTATUS(status));
        }
    } else {
        perror("fork");
        exit(-1);
    }
    }

    exit(0);
}
