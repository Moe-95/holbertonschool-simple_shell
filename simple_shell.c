#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 100
#define PATH_DELIMITER ":"

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
if (isatty(STDIN_FILENO))
{
write(STDOUT_FILENO, "#cisfun$ ", 9);
fflush(stdout);
}
}

/**
 * execute_command - Executes the given command using execve
 * @command: The command to be executed
 * @path_array: Array of strings containing directories in the PATH
 */
void execute_command(char *command, char **path_array)
{
char **args = NULL;
int i;

args = malloc(4 * sizeof(char *));
if (args == NULL)
{
perror("malloc");
exit(EXIT_FAILURE);
}

args[0] = malloc(BUF_SIZE + strlen(command) + 2);
if (args[0] == NULL)
{
perror("malloc");
free(args);
exit(EXIT_FAILURE);
}

args[1] = "-c";
args[2] = command;
args[3] = NULL;

if (isatty(STDIN_FILENO))
display_prompt();

for (i = 0; path_array[i] != NULL; i++)
{
snprintf(args[0], BUF_SIZE + strlen(command) + 2, "%s/%s", path_array[i], command);

if (access(args[0], F_OK | X_OK) == 0)
{
execve(args[0], args, NULL);
perror("execve");
free(args[0]);
free(args);
exit(EXIT_FAILURE);
}
}

fprintf(stderr, "%s: command not found\n", command);

free(args[0]);
free(args);

exit(EXIT_FAILURE);
}

/**
 * get_path - Retrieves the PATH environment variable and returns it as an array of strings
 *
 * Return: An array of strings containing the directories in the PATH, or NULL on failure
 */
char **get_path(void)
{
char *path_env = getenv("PATH");
char *path_copy, *token;
char **path_array;
int count = 0;

if (path_env == NULL)
{
perror("getenv");
return (NULL);
}

path_copy = strdup(path_env);
if (path_copy == NULL)
{
perror("strdup");
return (NULL);
}

token = strtok(path_copy, PATH_DELIMITER);
while (token != NULL)
{
count++;
token = strtok(NULL, PATH_DELIMITER);
}

path_array = malloc((count + 1) * sizeof(char *));
if (path_array == NULL)
{
perror("malloc");
free(path_copy);
return (NULL);
}

count = 0;
token = strtok(path_env, PATH_DELIMITER);
while (token != NULL)
{
path_array[count] = token;
count++;
token = strtok(NULL, PATH_DELIMITER);
}

path_array[count] = NULL;

free(path_copy);

return (path_array);
}

/**
 * main - Entry point of the program
 *
 * Return: Always 0 (success)
 */
int main(void)
{
char command[BUF_SIZE];
pid_t pid;
int status;
char **path_array = get_path();

while (1)
{
display_prompt();

if (fgets(command, BUF_SIZE, stdin) == NULL)
{
if (feof(stdin))
{
break;
}
else
{
perror("fgets");
continue;
}
}

command[strcspn(command, "\n")] = '\0';

if (strlen(command) == 0)
{
continue;
}

pid = fork();
if (pid == -1)
{
perror("fork");
continue;
}
else if (pid == 0)
{
execute_command(command, path_array);
exit(EXIT_FAILURE);
}
else
{
waitpid(pid, &status, 0);

if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
{
printf("%s: No such file or directory\n", command);
}
}
}

free(path_array);

return (0);
}
