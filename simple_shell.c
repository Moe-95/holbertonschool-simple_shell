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
    int (*func_builtin)(char **args, char *input_stdin, int *exit_status);
} choose_builtins_t;

void display_prompt(void) {
    if (isatty(STDIN_FILENO)) {
        write(STDOUT_FILENO, "#cisfun$", 9);
        fflush(stdout);
    }
}

int hsh_exit(char **args, char *input_stdin, int *exit_status) {
    *exit_status = 0;
    return 0;
}

int hsh_env(char **args, char *input_stdin, int *exit_status) {
    return 1;
}

int hsh_cd(char **args, char *input_stdin, int *exit_status) {
    return 1;
}

int hsh_setenv(char **args, char *input_stdin, int *exit_status) {
    return 1;
}

int hsh_unsetenv(char **args, char *input_stdin, int *exit_status) {
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

int hsh_execute_builtins(char **args, char *input_stdin,
                         char **argv, int *exit_status) {
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
            return ((int)((*options[i].func_builtin)(args, input_stdin, exit_status)));
        }
        i++;
    }
    return hsh_execute(args, argv, exit_status);
}
