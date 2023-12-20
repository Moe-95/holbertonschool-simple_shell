#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define BUF_SIZE 100
#define PATH_DELIMITER ":"

void display_prompt(void);
void execute_command(char *command, char **path_array);
char **get_path(void);
int main(void);

void display_prompt(void)
{
if(isatty(STDIN_FILENO))
{
write(STDOUT_FILENO, "#cisfun$", 9);
fflush(stdout);
}
}

void execute_command(char *command, char **path_array)
{
int i;
char **args = malloc(BUF_SIZE * sizeof(char *));
if(args == NULL)
{
perror("malloc");
exit(EXIT_FAILURE);
}
args[0] = command;
args[1] = NULL;
if(isatty(STDIN_FILENO))
{
display_prompt();
}
if(strchr(command, '/') != NULL)
{
execve(command, args, NULL);
perror("execve");
free(args);
exit(EXIT_FAILURE);
}
for(i = 0; path_array[i] != NULL; i++)
{
snprintf(args[0], BUF_SIZE, "%s/%s", path_array[i], command);
if(access(args[0], F_OK | X_OK) == 0)
{
execve(args[0], args, NULL);
perror("execve");
}
}
fprintf(stderr, "%s: command not found\n", command);
free(args);
exit(EXIT_FAILURE);
}

char **get_path(void)
{
char *path_env = getenv("PATH");
char *path_copy, *token;
char **path_array;
int count = 0;
if(path_env == NULL)
{
perror("getenv");
return (NULL);
}
path_copy = strdup(path_env);
if(path_copy == NULL)
{
perror("strdup");
return (NULL);
}
token = strtok(path_copy, PATH_DELIMITER);
while(token != NULL)
{
count++;
token = strtok(NULL, PATH_DELIMITER);
}
path_array = malloc((count + 1) * sizeof(char *));
if(path_array == NULL)
{
perror("malloc");
free(path_copy);
return (NULL);
}
count = 0;
token = strtok(path_env, PATH_DELIMITER);
while(token != NULL)
{
path_array[count] = token;
count++;
token = strtok(NULL, PATH_DELIMITER);
}
path_array[count] = NULL;
free(path_copy);
return (path_array);
}

int main(void)
{
char command[BUF_SIZE];
pid_t pid;
int status;
char **path_array = get_path();
while(1)
{
display_prompt();
if(fgets(command, BUF_SIZE, stdin) == NULL)
{
if(feof(stdin))
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
else if(pid == 0)
{
execute_command(command, path_array);
exit(EXIT_FAILURE);
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
free(path_array);
return (0);
}
