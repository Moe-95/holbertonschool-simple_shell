#include "shell.h"

/**
 * exec_command - Creates a child process and executes a command.
 * @exit_status: Exit status.
 * @fullpath: Executable command.
 * @tokens: Array of arguments passed.
 * Return: 0 on success, 1 on failure.
 */
int exec_command(int *exit_status, char *fullpath, char *tokens[])
{
pid_t child;
int status;

child = fork();

if (child == -1)
{
perror("fork");
return (1);
}

if (child == 0)
{
execve(fullpath, tokens, environ);
*exit_status = 2;
perror("execve");
exit(*exit_status);
}
else
{
wait(&status);

if (WIFEXITED(status))
{
*exit_status = WEXITSTATUS(status);
}
}

if (fullpath != tokens[0])
free(fullpath);

return (0);
}

/**
 * handle_prompt - Prints prompt and gets input.
 * @length: Length of input.
 * @line: User input stored.
 * Return: 0 on success, 1 on failure.
 */
int handle_prompt(size_t *length, char **line)
{
ssize_t bytes_read;
char newline = '\n';
int is_interactive = isatty(STDIN_FILENO);

if (is_interactive)
{
write(STDOUT_FILENO, "$ ", 2);
}

bytes_read = getline(line, length, stdin);

if (bytes_read == -1)
{
if (is_interactive)
{
write(STDOUT_FILENO, &newline, 1);
}

return (1);
}

return (0);
}

/**
 * check_builtins - Check for builtins.
 * @cnt: Number of arguments passed in the shell.
 * @tokens: List of shell arguments.
 * @exit_status: Exit status.
 * @argv: Command line arguments array.
 * Return: 0 on success, 1 on exit, 2 on continue.
 */
int check_builtins(int cnt, char **tokens, int *exit_status, char **argv)
{
char newline = '\n';

if (cnt < 1)
{
return (2);
}

if (_strcmp(tokens[0], "exit") == 0)
{
if (tokens[1] != NULL)
{
if (_atoi(tokens[1]))
{
if (_atoi(tokens[1]) >= 0)
*exit_status = _atoi(tokens[1]);
else
error_message(tokens, argv, exit_status);
}
else
{
error_message(tokens, argv, exit_status);
}
}

return (1);
}

if (_strcmp(tokens[0], "env") == 0)
{
char **env = environ;

while (*env != NULL)
{
write(STDOUT_FILENO, *env, _strlen(*env));
write(STDOUT_FILENO, &newline, 1);
env++;
}

return (2);
}

if (_strcmp(tokens[0], "unsetenv") == 0)
{
_unsetenv(tokens[1]);
return (2);
}

return (0);
}

/**
 * _ch - Checks for executable file errors.
 * @argv: Command line argument list.
 * @tokens: Array of user input.
 * @fullpath: Full path of command inputed.
 * @exit_status: Exit status.
 * Return: 0 on success, 1 on failure, 2 on continue.
 */
int _ch(char **argv, char **tokens, char **fullpath, int *exit_status)
{
char newline = '\n';
char *path = _getenv("PATH");

if (access(tokens[0], X_OK) == -1)
{
*fullpath = _which(tokens[0], path);

if (*fullpath == NULL)
{
write(STDERR_FILENO, argv[0], _strlen(argv[0]));
write(STDERR_FILENO, ": 1: ", 5);
write(STDERR_FILENO, tokens[0], _strlen(tokens[0]));
write(STDERR_FILENO, ": not found", 11);
write(STDERR_FILENO, &newline, 1);
*exit_status = 127;
return (2);
}

if (access(*fullpath, X_OK) == -1)
{
write(STDERR_FILENO, argv[0], _strlen(argv[0]));
write(STDERR_FILENO, ": 1: ", 5);
write(STDERR_FILENO, tokens[0], _strlen(tokens[0]));
write(STDERR_FILENO, ": not found", 11);
write(STDERR_FILENO, &newline, 1);
*exit_status = 127;
return (2);
}
}

return (0);
}

/**
 * main - Simple shell.
 * @argv: Argument list.
 * @argc: Argument count.
 * Return: Exit status.
 */
int main(int argc __attribute__((unused)), char **argv)
{
char *line = NULL;
size_t length = 0;
char *tokens[buffer];
int cnt, result;
char *fullpath;
int exit_status = 0;

while (1)
{
result = handle_prompt(&length, &line);

if (result == 1)
break;

cnt = 0;
tokens[cnt] = strtok(line, " \t\n");

while (tokens[cnt] != NULL)
{
cnt++;
tokens[cnt] = strtok(NULL, " \t\n");
}

tokens[cnt] = NULL;
fullpath = tokens[0];

result = check_builtins(cnt, tokens, &exit_status, argv);

if (result == 1)
break;
else if (result == 2)
continue;

result = _ch(argv, tokens, &fullpath, &exit_status);

if (result == 1)
break;
else if (result == 2)
continue;

result = exec_command(&exit_status, fullpath, tokens);

if (result == 1)
break;
else if (result == 2)
continue;
}

free(line);
return (exit_status);
}
