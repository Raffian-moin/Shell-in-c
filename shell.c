#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

#define COMMAND_MAX_LENGTH 100
#define MAX_TOKEN 256
#define DELIMITTER " \t\r\n\a"

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

int built_in_help(char *commands[]) {
    printf("Supported external commands:\n");
    printf("\n");
    printf("%-10s list of files in directory\n", "ls");
    printf("%-10s create files\n", "touch");
    printf("\n");
    printf("\n");
    printf("Supported built-in commands:\n");
    printf("\n");
    printf("%-10s see the available commands\n", "help");
    printf("%-10s history of commands typed by user\n", "history");
    printf("%-10s exit the shell\n", "exit");

    return 1;
}

int built_in_exit(char *commands[]) {
    printf("Exiting the shell...\n");
    return 0;
}

static int (*built_in_command_func[]) (char *commands[]) = {
    &built_in_help,
    &built_in_exit
};

static char *built_in_command_name[] = {
    "help",
    "exit",
    "history",
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

int get_command_type(char **tokens) {
    size_t external_command_size = sizeof(external_commands) / sizeof(external_commands[0]);
    for(int i = 0; i < (int) external_command_size; i++) {
        if (strcmp(external_commands[i], tokens[0]) == 0) {
            return EXTERNAL_COMMAND;
        }
    }

    size_t built_in_command_size = sizeof(built_in_command_name) / sizeof(built_in_command_name[0]);
    for(int i = 0; i < (int) built_in_command_size; i++) {
        if (strcmp(built_in_command_name[i], tokens[0]) == 0) {
            return BUILT_IN_COMMAND;
        }
    }

    return COMMAND_NOT_FOUND;
}

int handle_built_in_command(char *command[]) {
    size_t built_in_command_size = sizeof(built_in_command_name) / sizeof(built_in_command_name[0]);

    for(int i = 0; i < (int) built_in_command_size; i++) {
        if (strcmp(built_in_command_name[i], tokens[0]) == 0) {
            return built_in_command_func[i](command);
        }
    }
}

int main() {
    int shell_status = 1;
    do {
        printf(">> ");
        fflush(stdout);

        char command[COMMAND_MAX_LENGTH];
        if (fgets(command, sizeof(command), stdin) != NULL) {
            char **tokens = parse_command(command);
            int command_type = get_command_type(tokens);
            switch(command_type) {
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
                    shell_status = handle_built_in_command(tokens);
                    break;
                case COMMAND_NOT_FOUND:
                    printf("%s command not found\n", tokens[0]);
                    break;
            }
        }
    } while(shell_status);
}
