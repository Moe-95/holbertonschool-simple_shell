#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 100

/**
 * Displays the shell prompt.
 */
void display_prompt(void);

/**
 * The main function for the simple shell.
 *
 * Returns: Always 0.
 */
int main(void);

void display_prompt(void)
{
printf("#cisfun$ ");
}

int main(void)
{
char command[BUF_SIZE];
pid_t pid;
int status;
char *empty_args[1] = {NULL};
while(1)
{
display_prompt();

if(fgets(command, BUF_SIZE, stdin) == NULL)
{
if(feof(stdin))
{
printf("\n");
break;
}
else
{
perror("fgets");
continue;
}
}

if(command[strlen(command)-1] == '\n')
{
command[strlen(command)-1] = '\0';
}

if(strlen(command) == 0)
{
continue;
}

pid = fork();
if(pid == -1)
{
perror("fork");
continue;
}

if(pid == 0)
{
execve(command, empty_args, NULL);
{
printf("%s: No such file or directory\n", command);
exit(EXIT_FAILURE);
}
}
else
{
waitpid(pid, &status, 0);

if(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
{
printf("%s: No such file or directory\n", command);
}
}
}

return(0);
}