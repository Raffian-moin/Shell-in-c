#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

#define COMMAND_MAX_LENGTH 100
#define MAX_TOKEN 256
#define DELIMITTER " \t\n"

enum command_status {
    COMMAND_NOT_FOUND = -1,
    EXTERNAL_COMMAND = 0, 
    BUILT_IN_COMMAND = 1
};

static char *tokens[MAX_TOKEN];

static char *external_commands[] = {
    "ls",
    "touch"
};

static char *built_in_commands[] = {
    "help",
    "history",
    "exit"
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

int handlecommand(char **tokens) {
    size_t external_command_size = sizeof(external_commands) / sizeof(external_commands[0]);
    for(int i = 0; i < (int) external_command_size; i++) {
        if (strcmp(external_commands[i], tokens[0]) == 0) {
            return EXTERNAL_COMMAND;
        }
    }

    size_t built_in_command_size = sizeof(built_in_commands) / sizeof(built_in_commands[0]);
    for(int i = 0; i < (int) built_in_command_size; i++) {
        if (strcmp(built_in_commands[i], tokens[0]) == 0) {
            return BUILT_IN_COMMAND;
        }
    }

    return -1;

}

int main() {
    while(1) {
        printf(">> ");
        fflush(stdout);

        char command[COMMAND_MAX_LENGTH];
        if (fgets(command, sizeof(command), stdin) != NULL) {
            char **tokens = parse_command(command);
            int command_st = handlecommand(tokens);
            switch(command_st) {
                case EXTERNAL_COMMAND:
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
                    break;
                case BUILT_IN_COMMAND:
                    printf("builtin command\n");
                    break;
                case COMMAND_NOT_FOUND:
                    printf("%s command not found\n", tokens[0]);
                    break;
            }
        }
    }
}
