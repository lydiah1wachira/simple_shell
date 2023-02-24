#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *command = NULL;
    size_t bufsize = 0;

    while (1) {
        printf("$ "); // display prompt
        if (getline(&command, &bufsize, stdin) == EOF) {
            printf("\n"); // handle end of file condition
            break;
        }
        command[strcspn(command, "\n")] = '\0'; // remove newline character from input
        pid_t pid = fork(); // create child process
        if (pid == -1) {
            perror("fork"); // handle error
        } else if (pid == 0) {
            // execute command in child process
            if (execlp(command, command, NULL) == -1) {
                printf("%s: command not found\n", command); // handle executable not found error
                exit(1);
            }
        } else {
            wait(NULL); // wait for child process to finish
        }
    }

    free(command); // free memory allocated by getline()
    return 0;
}
