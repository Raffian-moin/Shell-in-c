#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

#define COMMAND_MAX_LENGTH 100
#define MAX_TOKEN 256
#define DELIMITTER " \t\n"

static char *tokens[MAX_TOKEN];

static char *external_commands[] = {
    "ls",
    "touch"
};

char **parse_command(char command[]) {
    printf("parse command: %s\n", command);
    char *token = strtok(command, DELIMITTER);
    int token_index = 0;

    while(token != NULL) {
        tokens[token_index] = token;
        token = strtok(NULL, DELIMITTER);
        token_index++;
    }
    
    tokens[token_index] = NULL;

    return tokens;
}

void handlecommand(char **tokens) {
    size_t external_command_size = sizeof(external_commands) / sizeof(external_commands[0]);
    for(int i = 0; i < (int) external_command_size; i++) {
        if (strcmp(external_commands[i], tokens[0]) == 0) {
            int status;
            pid_t child = fork();
            if (child == 0) {
                if (execvp(tokens[0], tokens) == -1) {
                    perror("error occured");
                    exit(1);
                }
            } else {
                waitpid(child, &status, 0);
            }
        }
    }
}

int main() {
    while(1) {
        printf(">> ");
        fflush(stdout);

        char command[COMMAND_MAX_LENGTH];
        if (fgets(command, sizeof(command), stdin) != NULL) {
            char **tokens = parse_command(command);
            handlecommand(tokens);
        }
    }
}
