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
 * _unsetenv - Remove an environment variable.
 * @name: Variable name.
 * Return: 0 on success, -1 on failure.
 */
int _unsetenv(char *name)
{
char **env = environ;
int len, nameLen;
int environ_cnt = 0, i = 0, j;
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

while (*env != NULL)
{
if (_strncmp(name, *env, nameLen) != 0 || (*env)[nameLen] != '=')
{
len = _strlen(*env);
new_arr[i] = malloc((len + 1) * sizeof(char));

if (new_arr[i] == NULL)
{
for (j = 0; j < i; j++)
{
free(new_arr[j]);
}

free(new_arr);
return (-1);
}

_strcpy(new_arr[i], *env);
i++;
}

env++;
}

new_arr[i] = NULL;
environ = new_arr;

return (0);
}
