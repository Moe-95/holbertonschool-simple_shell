#include "shell.h"

/**
 * _getenv - Get an environment variable given the name.
 * @name: Name of the environment variable.
 * Return: Environment variable value on success, otherwise NULL.
 */
char *_getenv(char *name)
{
int namelen = _strlen(name);
int i;

for (i = 0; environ[i] != NULL; i++)
{
if (_strncmp(name, environ[i], namelen) == 0 && environ[i][namelen] == '=')
{
return (environ[i] + namelen + 1);
}
}

return (NULL);
}
/**
 * _realloc_env - Reallocates the environment variable array.
 * @new_arr: Pointer to the new environment variable array.
 * @env: Pointer to the original environment variable array.
 * @i: Index to track the current position in the new array.
 * @environ_cnt: Pointer to the environment variable count.
 * @nameLen: Length of the name of the variable to unset.
 * @name: Name of the variable to unset.
 *
 * Return: 0 on success, -1 on failure.
 */
static int _realloc_env(char ***new_arr, char **env, int i,
int environ_cnt, int nameLen, char *name)
{
while (*env != NULL)
{
if (_strncmp(name, *env, nameLen) != 0 || (*env)[nameLen] != '=')
{
int len, j;
len = _strlen(*env);
(*new_arr)[i] = malloc((len + 1) * sizeof(char));
if ((*new_arr)[i] == NULL)
{
for (j = 0; j < i; j++)
{
free((*new_arr)[j]);
}
free(*new_arr);
return (-1);
}
_strcpy((*new_arr)[i], *env);
i++;
}
env++;
}
(*new_arr)[i] = NULL;
environ = *new_arr;
return (0);
}

/**
 * _unsetenv - Remove an environment variable.
 * @name: Variable name.
 *
 * Return: 0 on success, -1 on failure.
 */
int _unsetenv(char *name)
{
char **env = environ;
int nameLen, environ_cnt = 0, i = 0;
char **new_arr;
if (name == NULL)
return (-1);
while (*env != NULL)
{
environ_cnt++;
env++;
}
new_arr = malloc((environ_cnt + 1) * sizeof(char *));
if (new_arr == NULL)
return (-1);
env = environ;
nameLen = _strlen(name);
if (_realloc_env(&new_arr, env, i, environ_cnt, nameLen, name) == -1)
return (-1);
return (0);
}

/**
* * _which - Entry point
* Description: Find the full path name
*
* @filename: File name
* @path: PATH environment variable
* Return: Full path name if found, NULL if not found.
*/
char *_which(char *filename, char *path)
{
char *path1 = _getenv("PATH1");
char *path_cpy;
char *tokens[1024];
char *pathname;
int i;
if (path == NULL && path1 == NULL)
{
return (NULL);
}
if (path == NULL)
{
path_cpy = strdup(path1);
}
else
{
path_cpy = strdup(path);
}
if (path_cpy == NULL)
{
return (NULL);
}
i = 0;
tokens[i] = strtok(path_cpy, ":");
while (tokens[i] != NULL)
{
pathname = malloc(strlen(tokens[i]) + strlen(filename) + 2);
_strcpy(pathname, tokens[i]);
_strcat(pathname, "/");
_strcat(pathname, filename);
if (check_path(pathname))
{
free(path_cpy);
return (pathname);
}
free(pathname);
i++;
tokens[i] = strtok(NULL, ":");
}
free(path_cpy);
return (NULL);
}
/**
 * check_path - Entry point
 *
 * Descritpion: 'checks for path'
 *
 * @path: 'path'
 * Return: 1 - true & 0 - false
 */
int check_path(char *path)
{
struct stat buff;
return ((stat(path, &buff) == 0));
}
