#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 100
#define PATH_DELIMITER ":"

extern char **environ;

typedef struct {
    char *name_builtin;
    int (*func_builtin)(char **args, int *exit_status);
} choose_builtins_t;

void display_prompt(void);

char *validate_input(char **arguments, char **argv);

int hsh_exit(char **args, int *exit_status);

int hsh_env(char **args, int *exit_status);

int hsh_cd(char **args, int *exit_status);

int hsh_setenv(char **args, int *exit_status);

int hsh_unsetenv(char **args, int *exit_status);

int hsh_execute(char **arguments, char **argv, int *exit_status);

int hsh_execute_builtins(char **args, int *exit_status);

char **tokenize_input(char *input);

int main(int argc, char **argv) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int exit_status = 0;
    char **tokens;

    while (1) {
        display_prompt();
        read = getline(&line, &len, stdin);
        if (read == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }

        tokens = tokenize_input(line);
        hsh_execute_builtins(tokens, &exit_status);
        free(tokens);
    }

    free(line);
    return exit_status;
}

void display_prompt(void) {
    if (isatty(STDIN_FILENO)) {
        write(STDOUT_FILENO, "#cisfun$", 9);
        fflush(stdout);
    }
}

char *validate_input(char **arguments, char **argv) {
    return strdup("/bin/ls");
}

int hsh_exit(char **args, int *exit_status) {
    *exit_status = 0;
    return 0;
}

int hsh_env(char **args, int *exit_status) {
    return 1;
}

int hsh_cd(char **args, int *exit_status) {
    return 1;
}

int hsh_setenv(char **args, int *exit_status) {
    return 1;
}

int hsh_unsetenv(char **args, int *exit_status) {
    return 1;
}

int hsh_execute(char **arguments, char **argv, int *exit_status) {
    pid_t pid;
    int status;
    char *new_arguments;

    new_arguments = validate_input(arguments, argv);
    if (strcmp(new_arguments, "Fail access") == 0)
        return 1;

    pid = fork();
    if (pid == 0) {
        if (execve(new_arguments, arguments, environ) == -1) {
            perror("execve fail");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("Error forking");
        free(new_arguments);
        return 1;
    } else {
        waitpid(-1, &status, 0);
        if (WIFEXITED(status))
            *exit_status = WEXITSTATUS(status);
        if (arguments[0][0] != '/' && arguments[0][0] != '.')
            free(new_arguments);
        return 1;
    }
    return 1;
}

int hsh_execute_builtins(char **args, int *exit_status) {
    int i = 0;
    choose_builtins_t options[] = {
        {"exit", hsh_exit},
        {"env", hsh_env},
        {"cd", hsh_cd},
        {"setenv", hsh_setenv},
        {"unsetenv", hsh_unsetenv},
        {NULL, NULL}
    };

    while (options[i].name_builtin) {
        if (strcmp(options[i].name_builtin, args[0]) == 0) {
            return ((int)((*options[i].func_builtin)(args, exit_status)));
        }
        i++;
    }
    return hsh_execute(args, NULL, exit_status);
}

char **tokenize_input(char *input) {
    char **tokens = malloc(BUF_SIZE * sizeof(char *));
    char *token;
    int i = 0;

    if (!tokens) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, " \t\r\n\a");
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\r\n\a");
    }

    tokens[i] = NULL;
    return tokens;
}
