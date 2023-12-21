#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stddef.h>
/* define buffer */
#define buffer 1024

extern char **environ;

/* path checker function */
int check_path(char *path);

/* custom _which function  */
char *_which(char *filename, char *path);

/* string lenght function */
int _strlen(char *str);

/* string copy function */
char *_strcpy(char *dest, char *src);

/* concatenate string */
char *_strcat(char *dest, char *src);

/* duplicate string */
char *_strdup(char *str);

/* compare strings */
int _strcmp(char *str1, char *str2);

/* execute command using execve */
int exec_command(int *exit_status, char *fullpath, char *tokens[]);

/* Print prompt and request input*/
int handle_prompt(size_t *length, char **line);

/* check for builtins */
int check_builtins(int cnt, char **tokens, int *exit_status, char **argv);

/* check if file is executable */
int _ch(char **argv, char **tokens, char **fullpath, int *exit_status);

/* get an environment variable*/
char *_getenv(char *name);

/*compare n characters of strings*/
int _strncmp(char *str1, char *str2, int n);

/* unset environment variable */
int _unsetenv(char *name);

/* Realloc implementation*/
void *_realloc(void *ptr, size_t size);

/*getline implementation*/
ssize_t _getline(char **linePtr, size_t *buff_size, FILE *my_file);

/* convert string to integer */
int _atoi(char *str);

/* error message */
void error_message(char **tokens, char **argv, int *exit_status);

#endif
/**
 * _realloc - Entry point
 * Description: 'to reallocate memory'
 *
 * @ptr: pointer
 * @size: size
 * Return: 'the new ptr'
 */
void *_realloc(void *ptr, size_t size)
{
	void *new_ptr;
	char *next_ptr;
	char *new_next_ptr;
	size_t dup_size;
	size_t index = 0; /* counter */

	if (size == 0)
	{
		free(ptr);
		return (NULL);
	} /* end if */
	/* we allocate a new block of memory */
	new_ptr = malloc(size);
	/* validate allocation  */
	if (new_ptr == NULL)
	{
		return (NULL);
	} /* end if */
	/* copy contents to new blocks */
	next_ptr = (char *) ptr;
	new_next_ptr = (char *) next_ptr;
	if (size < sizeof(ptr))
	{
		dup_size = size;
	} /* end if */
	else
	{
		dup_size = sizeof(ptr);
	} /* end else */

	for (; index <= dup_size; index++)
	{
		new_next_ptr[index] = next_ptr[index];
	} /* end for */

	free(ptr);

	return (next_ptr);

} /* end function */


/**
 * _getline - Entry point
 * Description: 'to read a line of char.'
 *
 * @linePtr: 'pointer to the line'
 * @buff_size: 'buffer'
 * @my_file: 'the file'
 * Return: raed line
 */

ssize_t _getline(char **linePtr, size_t *buff_size, FILE *my_file)
{
	ssize_t bytes_read = 0;
	size_t index = 0;
	ssize_t result;
	char *newSizePtr;

	while (1)
	{
		if (index >= *buff_size)
		{
			*buff_size += 8;
			newSizePtr = _realloc(*linePtr, *buff_size);
			if (newSizePtr == NULL)
			{
				perror("Memory allocation error");
				return (-1);
			} /* End if */
			*linePtr = newSizePtr;
		} /* end if */
		result = read(fileno(my_file), &((*linePtr)[index]), 1);
		if (result == -1)
		{
			perror("Error reading file");
			return (-1);
		} /* end if */
		else if (result == 0)
		{
			break;
		} /* end else if */
		bytes_read++;

		if ((*linePtr)[index] == '\n')
		{
			break;
		} /* end if */

		index++;
	} /* end while */

	return (bytes_read);
} /* end function */
/**
 * error_message - prints out error message
 * @tokens: array containing user input
 * @argv: command line arguments array
 * @exit_status: exit status
 * Return: void
 */
void error_message(char **tokens, char **argv, int *exit_status)
{
	char newline = '\n';

	write(STDERR_FILENO, argv[0], _strlen(argv[0]));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, tokens[0], _strlen(tokens[0]));
	write(STDERR_FILENO, ": Illegal number: ", 18);
	write(STDERR_FILENO, tokens[1], _strlen(tokens[1]));
	write(STDERR_FILENO, &newline, 1);
	*exit_status = 2;
}
/**
 * _strlen - Entry point
 * Description: 'find the string lenght'
 *
 * @str: 'string'
 * Return: lenght of string
 */

int _strlen(char *str)
{
	int i = 0;

	if (str == NULL)
		str = ""; /* end if */
	while (*str != '\0')
	{
		i++;
		str++;
	} /* end while */
	return (i);
} /* End function */


/**
 * _strcpy - Entry point
 * Description: 'copy strings from src to dest'
 *
 * @dest: 'destination'
 * @src: 'source'
 * Return: copied string
 */
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	while (i <= _strlen(src))
	{
		dest[i] = src[i];
		i++;
	} /* end while */
	return (dest);
} /* end function */


/**
 * _strcat - Entry poiont
 * Description: 'concatenate string'
 *
 * @dest: 'destination'
 * @src: 'source'
 * Return: concartenated string
 */
char *_strcat(char *dest, char *src)
{
	int i = 0;
	char *temp = dest;

	while (*temp != '\0')
	{
		temp++;
	} /* end while */
	while (i < _strlen(src))
	{
		*temp = src[i];
		temp++;
		i++;
	} /* end while */
	*temp = '\0';
	return (dest);
} /* end function */



/**
 * _strdup - Entry point
 * Description: 'duplicate a string'
 *
 * @str: 'string'
 * Return: returns a pointer to a newly allocated memory
 * block containing a duplicate of the input string
 */
char *_strdup(char *str)
{
	char *myStr;
	int i = 0;

	if (str == NULL)
	{
		return (NULL);
	} /* end if */
	myStr = (char *)malloc(strlen(str) * sizeof(char) + 1);
	if (myStr == NULL)
	{
		return (NULL);
	} /* end if */
	while (i <= (int)strlen(str))
	{
		myStr[i] = str[i];
		i++;
	} /* end if */
	return (myStr);
} /* end function */


/**
 * _strcmp - Entry point
 * Description: 'compares two strings'
 *
 *
 * @str1: 'string 1'
 * @str2: 'stri'
 *
 * Return: 'If str1 is lexicographically less than str2,'
 * 'strcmp returns a negative integer value (usually -1).'
 * 'If str1 is lexicographically greater than str2,'
 * 'strcmp returns a positive integer value (usually 1).'
 * 'If str1 is lexicographically equal to str2,'
 * 'strcmp returns zero.'
 */
int _strcmp(char *str1, char *str2)
{
	int i = 0;
	int len1, len2;

	if (str1 == NULL)
		str1 = ""; /* end if */
	if (str2 == NULL)
		str2 = ""; /* end if */
	len1 = _strlen(str1);
	len2 = _strlen(str2);
	if (len1 > len2)
		return (1); /* end if */
	else if (len1 < len2)
		return (-1); /* end elif */
	while (i < len1)
	{
		if (str1[i] != str2[i])
		{
			return (-1);
		} /* end if */
		i++;
	} /* end while */
	return (0);
} /* end function */
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
} /* End function */



/**
 * _which - Entry point
 * Description: 'to find the full path name'
 *
 * @filename: 'file name'
 * @path: 'path'
 * Return: full path name if found && 0 if not found.
 */
char *_which(char *filename, char *path)
{
	char *path_cpy;
	char *tokens[1024];
	char *pathname;
	int i;

	if (path == NULL)
	{
		return (NULL);
	} /* End if */
	path_cpy = strdup(path);

	/* Tokenize path */
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
		} /* end if */

		free(pathname);
		i++;
		tokens[i] = strtok(NULL, ":");
	} /* end while */

	free(path_cpy);
	return (NULL);
} /* end function */

/**
 * _strncmp - compare n character of str1 and str2
 * @str1: string 1
 * @str2: string 2
 * @n: number of characters
 * Return: 0 on success
 */
int _strncmp(char *str1, char *str2, int n)
{
	int i = 0;

	while (i < n)
	{
		if (str1[i] != str2[i])
		return (-1);
		i++;
	}
	return (0);
}

/**
 * _getenv - get an environment variable given name
 * @name: name of environment variable
 * Return: environment variable value on success
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
 * _unsetenv - remove env variable
 * @name: variable name
 * Return: 0 on success
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
/**
 * _atoi - convert string to integer
 * @str: given string
 * Return: integer
 */
int _atoi(char *str)
{
	int result = 0; /* stores converted int */
	int is_positive = 1; /* track int if it is +ve */
	int i = 0; /* counter */

	/* check if the char at index 0 is -ve / +ve */
	if (str[0] == '-')
	{
		is_positive = -1;
		i = 1;
	} /* End if */

	/* Iterate through the loop */
	while (str[i] != '\0')
	{
		/* Validate if the current char is digit */
		if (str[i] >= '0' && str[i] <= '9')
		{
			/* convert to digit and update result */
			result = result * 10 + (str[i] - '0');
			i++;
		} /* end if */
		else
		{
			/* for a non digit char */
			break;
		} /* end else */
	} /* end while */
	return (is_positive * result);
} /* ens function */
/**
 * exec_command - creates a child process and execute command
 * @exit_status: exit status
 * @fullpath: executable command
 * @tokens: array of arguments passed
 * Return: void
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
	} /* end if */
	if (child == 0)
	{
		execve(fullpath, tokens, environ);
		*exit_status = 2;
		perror("execve");
		exit(*exit_status);
	} /* end if */
	else
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			*exit_status = WEXITSTATUS(status);
		} /* end if */
	} /* end else */
	if (fullpath != tokens[0])
		free(fullpath);
	return (0);
} /* end function */

/**
 * handle_prompt - prints prompt and gets input
 * @length: length of input
 * @line: user input stored
 * Return: void
 */
int handle_prompt(size_t *length, char **line)
{
	ssize_t bytes_read;
	char newline = '\n';
	int is_interactive = isatty(STDIN_FILENO);

	if (is_interactive)
	{
		write(STDOUT_FILENO, "$ ", 2);
	} /* end if */
	bytes_read = getline(line, length, stdin);
	if (bytes_read == -1)
	{
		if (is_interactive)
		{
			write(STDOUT_FILENO, &newline, 1);
		} /* end if */
	return (1);
	} /* end if */
	return (0);
} /* end function */

/**
 * check_builtins - check for builtins
 *
 * @cnt: number of arguments passed in shell
 * @tokens: list of shell arguments
 * @argv: command line arguments array
 * @exit_status: exit status
 * Return: void
 */
int check_builtins(int cnt, char **tokens, int *exit_status, char **argv)
{
	char **env = environ;
	char newline = '\n';

	if (cnt < 1)
	{
		return (2);
	}
	if (strcmp(tokens[0], "exit") == 0)
	{
		if (tokens[1] != NULL)
		{
			if (atoi(tokens[1]))
				if (atoi(tokens[1]) >= 0)
					*exit_status = _atoi(tokens[1]);
				else
					error_message(tokens, argv, exit_status);
			else
			{
				error_message(tokens, argv, exit_status);
			}
		}
		return (1);
	}
	if (strcmp(tokens[0], "env") == 0)
	{
		env = environ;
		while (*env != NULL)
		{
			write(STDOUT_FILENO, *env, strlen(*env));
			write(STDOUT_FILENO, &newline, 1);
			env++;
		}
		return (2);
	}
	if (strcmp(tokens[0], "unsetenv") == 0)
	{
		_unsetenv(tokens[1]);
		return (2);
	}
	return (0);
}



/**
 * _ch - checks for executable file errors
 * @argv: command line argument list
 * @tokens: array of user input
 * @fullpath: full path of command inputed
 * @exit_status: exit status
 * Return: void
 */
int _ch(char **argv, char **tokens, char **fullpath, int *exit_status)
{
    char newline = '\n';
    char *path = _getenv("PATH");

    if (tokens[0] == NULL)
    {
        // Handle case where command is not provided
        write(STDERR_FILENO, "No command provided\n", 20);
        *exit_status = 2;
        return 2;
    }

    *fullpath = _which(tokens[0], path);

    if (*fullpath == NULL || access(*fullpath, X_OK) == -1)
    {
        // Handle case where command is not found or not executable
        write(STDERR_FILENO, argv[0], strlen(argv[0]));
        write(STDERR_FILENO, ": 1: ", 5);
        write(STDERR_FILENO, tokens[0], strlen(tokens[0]));
        write(STDERR_FILENO, ": not found", 11);
        write(STDERR_FILENO, &newline, 1);
        *exit_status = 127;
        return 2;
    }

    return 0;
}

/**
 * main - simple shell
 * @argv: argument list
 * @argc: argument count
 * Return: exit status
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
			break; /* end  if */
		cnt = 0;
		tokens[cnt] = strtok(line, " \t\n");
		while (tokens[cnt] != NULL)
		{
			cnt++;
			tokens[cnt] = strtok(NULL, " \t\n");
		} /* end while */
		tokens[cnt] = NULL;
		fullpath = tokens[0];
		result = check_builtins(cnt, tokens, &exit_status, argv);
		if (result == 1)
			break; /* end if */
		else if (result == 2)
			continue; /* end else if */
		result = _ch(argv, tokens, &fullpath, &exit_status);
		if (result == 1)
			break;/* end if */
		else if (result == 2)
			continue; /* else if */
		result = exec_command(&exit_status, fullpath, tokens);
		if (result == 1)
			break; /* end if */
		else if (result == 2)
			continue; /*end if*/
	} /* End while */
	free(line);
	return (exit_status);
} /* end function */
