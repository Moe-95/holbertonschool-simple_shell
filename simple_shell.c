#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void);

/**
 * main - The main function for the simple shell.
 *
 * Return: Always 0.
 */
int main(void);

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
printf("simple_shell$");
}

/**
 * main - The main function for the simple shell.
 *
 * Return: Always 0.
 */
int main(void)
{
char command[MAX_COMMAND_LENGTH];
size_t len;

while (1)
{
display_prompt();
if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
{
printf("\nExiting shell...\n");
break;
}

len = strlen(command);
if (len > 0 && command[len - 1] == '\n')
{
command[len - 1] = '\0';
}

pid_t pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}

if (pid == 0)
{
char *args[] = {command, NULL};
if (execve(command, args, NULL) == -1)
{
perror("execve");
exit(EXIT_FAILURE);
}
}
else
{
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
{
printf("Command execution failed\n");
}
}
}

return (0);
}
