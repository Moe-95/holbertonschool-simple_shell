#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * Displays the shell prompt.
 */
void display_prompt(void);

/**
 * The main function for the simple shell.
 *
 * Return: Always 0.
 */
int main(void);

void display_prompt(void)
{
printf("simple_shell$ ");
}

int main(void)
{
char command[MAX_COMMAND_LENGTH];
size_t len;
pid_t pid;
char *args[2];
int status;

while (1) {
display_prompt();

if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
printf("\n");
continue;
}

len = strlen(command);
if (len > 0 && command[len - 1] == '\n') {
command[len - 1] = '\0';
}

pid = fork();
if (pid == -1) {
perror("fork");
exit(EXIT_FAILURE);
}

if (pid == 0) {
args[0] = command;
args[1] = NULL;
if (execve(command, args, NULL) == -1) {
perror("execve");
exit(EXIT_FAILURE);
}
} else {
waitpid(pid, &status, 0);

if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) {
printf("Command execution failed\n");
}
}
}

return(0);
}
