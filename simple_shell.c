#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define BUF_SIZE 100

void display_prompt(void);
void execute_command(char *command);

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
 * main - Entry point of the program
 *
 * Return: Always 0 (success)
 */
int main(void)
{
char command[BUF_SIZE];
pid_t pid;
int status;

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
execute_command(command);
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

return (0);
}

/**
 * execute_command - Executes the given command using execve
 * @command: The command to be executed
 */
void execute_command(char *command)
{
char *args[4];

args[0] = "/bin/sh";
args[1] = "-c";
args[2] = command;
args[3] = NULL;
if (isatty(STDIN_FILENO))
display_prompt();
execve(args[0], args, NULL);
perror("execve");
exit(EXIT_FAILURE);
}