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

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
    if (isatty(STDIN_FILENO))
    {
        write(STDOUT_FILENO, "#cisfun$", 9);
        fflush(stdout);
    }
}

/**
 * hsh_exit - Implementation of the exit built-in command
 */
int hsh_exit(char **args, char *input_stdin, int *exit_status) {
    // Your implementation for the exit builtin
    // Update exit status if needed
    *exit_status = 0;
    return 0;
}

/**
 * hsh_env - Implementation of the env built-in command
 */
int hsh_env(char **args, char *input_stdin, int *exit_status) {
    // Your implementation for the env builtin
    // Update exit status if needed
    return 1;
}

/**
 * hsh_cd - Implementation of the cd built-in command
 */
int hsh_cd(char **args, char *input_stdin, int *exit_status) {
    // Your implementation for the cd builtin
    // Update exit status if needed
    return 1;
}

/**
 * hsh_setenv - Implementation of the setenv built-in command
 */
int hsh_setenv(char **args, char *input_stdin, int *exit_status) {
    // Your implementation for the setenv builtin
    // Update exit status if needed
    return 1;
}

/**
 * hsh_unsetenv - Implementation of the unsetenv built-in command
 */
int hsh_unsetenv(char **args, char *input_stdin, int *exit_status) {
    // Your implementation for the unsetenv builtin
    // Update exit status if needed
    return 1;
}

/**
 * hsh_execute - Fork process and replace the child with a new program
 * @arguments: String of arguments (tokens)
 * @argv: Array of arguments
 * @exit_status: Exit status
 * Return: Always 1 (success).
 */
int hsh_execute(char **arguments, char **argv, int *exit_status)
{
    pid_t pid;
    int status; /* this will be used with waitpid syscall */
    char *new_arguments;

    /* check if PATH exists and can be accessed, also tokenize PATH*/
    new_arguments = validate_input(arguments, argv);
    if (strcmp(new_arguments, "Fail access") == 0)
        return (1);

    pid = fork(); /* create a duplicate process (child)*/
    if (pid == 0) /* child process */
    {
        /* execute a completely new program instead of the child */
        if (execve(new_arguments, arguments, environ) == -1)
        {
            perror("execve fail");
            /*free(new_arguments);*/
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) /* check if fork fails */
    {
        perror("Error forking");
        free(new_arguments);
        return (1);
    }
    else /* parent process */
    {
        /* this is equivalent to wait(&status) */
        waitpid(-1, &status, 0);
        /* check if the child terminated normally */
        if (WIFEXITED(status))
            /* return the exit status of the child */
            *exit_status = WEXITSTATUS(status);
        /* evaluate the first element of the first token */
        if (arguments[0][0] != '/' && arguments[0][0] != '.')
            free(new_arguments);
        return (1);
    }
    return (1);
}

/**
 * hsh_execute_builtins - Choose from an array of builtin functions
 * @args: String of arguments (tokens)
 * @input_stdin: String from input
 * @argv: Array of arguments
 * @exit_status: Exit status
 * Return: Status to stay or exit the main loop, 1 or 0
 */
int hsh_execute_builtins(char **args, char *input_stdin,
                         char **argv, int *exit_status)
{
    int i = 0;
    /* builtin functions array of type 'structure choose_builtin' (see main.h) */
    choose_builtins_t options[] = {
        {"exit", hsh_exit},
        {"env", hsh_env},
        {"cd", hsh_cd},
        {"setenv", hsh_setenv},
        {"unsetenv", hsh_unsetenv},
        {NULL, NULL}
    };

    while (options[i].name_builtin)
    {
        /* compare if args[0] matches the builtin function name */
        if (strcmp(options[i].name_builtin, args[0]) == 0)
        {
            /* return the pointer to function (2nd member of array) and execute it */
            return ((int)((*options[i].func_builtin)(args, input_stdin, exit_status)));
        }
        i++;
    }
    /* if no builtin function is encountered execute a process */
    return (hsh_execute(args, argv, exit_status));
}

/**
 * validate_input - Validate and modify input arguments
 * @arguments: String of arguments (tokens)
 * @argv: Array of arguments
 * Return: Modified arguments or error message
 */
char *validate_input(char **arguments, char **argv)
{
    // Your implementation for validate_input
    return NULL;
}

/**
 * execute_command - Executes the given command using execve
 * @command: The command to be executed
 * @path_array: Array of strings containing directories in the PATH
 *
 * Return: No return value
 */
void execute_command(char *command, char **path_array)
{
    int i;
    char **args = malloc(BUF_SIZE * sizeof(char *));
    if (args == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    args[0] = strtok(command, " ");
    for (i = 1; i < BUF_SIZE && args[i - 1] != NULL; i++)
    {
        args[i] = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (isatty(STDIN_FILENO))
    {
        display_prompt();
    }

    if (args[0] == NULL || args[0][0] == '\0')
    {
        free(args);
        return;
    }

    if (strchr(args[0], '/') != NULL)
    {
        execve(args[0], args, NULL);
        perror("execve");
        free(args);
        exit(EXIT_FAILURE);
    }

    hsh_execute_builtins(args, NULL, path_array, NULL);

    free(args);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    char *input_buffer = NULL;
    size_t buffer_size = 0;

    // Main loop
    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            display_prompt();
        }

        // Read input
        ssize_t characters_read = getline(&input_buffer, &buffer_size, stdin);
        if (characters_read == -1)
        {
            perror("getline");
            free(input_buffer);
            exit(EXIT_FAILURE);
        }

        // Remove newline character from input
        input_buffer[strcspn(input_buffer, "\n")] = '\0';

        // Tokenize input
        char *token = strtok(input_buffer, " ");
        char **args = malloc(BUF_SIZE * sizeof(char *));
        if (args == NULL)
        {
            perror("malloc");
            free(input_buffer);
            exit(EXIT_FAILURE);
        }

        int i;
        for (i = 0; i < BUF_SIZE && token != NULL; i++)
        {
            args[i] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Execute command
        hsh_execute_builtins(args, input_buffer, argv, NULL);

        // Free allocated memory
        free(args);
    }

    // Free allocated memory
    free(input_buffer);

    return 0;
}
