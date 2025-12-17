#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

#define COMMAND_MAX_LENGTH 100

void current_directory() {
    char *current_dir = getcwd(NULL, 0);
    if (current_dir == NULL) {
        perror("Something went wrong");
        return;
    }

    printf("%s\n", current_dir);
    free(current_dir);
}

int main() {
    while(1) {
        printf(">> ");
        fflush(stdout);

        char command[COMMAND_MAX_LENGTH];
        if (fgets(command, sizeof(command), stdin) != NULL) {
            printf("%s\n", command);


            command[strcspn(command, "\n")] = '\0';
            if (strcmp(command, "pwd") == 0) {
                current_directory();
            }
        }

    }
}
