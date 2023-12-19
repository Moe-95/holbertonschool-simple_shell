#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * display_prompt - Displays the prompt for the shell.
 */
void display_prompt()
{
printf("# simple_shell$ ");
fflush(stdout);
}

/**
 * main - Entry point for the simple shell.
 *
 * Return: Always 0.
 */
int main()
{
pid_t pid = fork();
char command[MAX_COMMAND_LENGTH];

while (1)
{
display_prompt();

if (fgets(command, sizeof(command), stdin) == NULL)
{
printf("\n");
break;
}

command[strcspn(command, "\n")] = '\0';


if (pid == -1)
{
perror("Fork error");
exit(EXIT_FAILURE);
}

if (pid == 0)
{
if (execlp(command, command, (char *)NULL) == -1)
{
perror("Error");
exit(EXIT_FAILURE);
}
}
else
{
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status))
{
printf("Child process exited with status %d\n", WEXITSTATUS(status));
}
else if (WIFSIGNALED(status))
{
printf("Child process terminated by signal %d\n", WTERMSIG(status));
}
}
}

return (0);
}
