#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];

    while (1) {
        printf("$ ");

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        if (system(buffer) == -1) {
            fprintf(stderr, "%s: command not found\n", argv[0]);
        }
    }

    return 0;
}
