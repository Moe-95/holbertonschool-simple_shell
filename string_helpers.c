#include "shell.h"

/**
 * _strlen - Finds the length of a string.
 * @str: The input string.
 * Return: Length of the string.
 */
int _strlen(char *str)
{
int i = 0;

if (str == NULL)
str = "";

while (*str != '\0')
{
i++;
str++;
}

return (i);
}

/**
 * _strcpy - Copies strings from source to destination.
 * @dest: Destination.
 * @src: Source.
 * Return: Copied string.
 */
char *_strcpy(char *dest, char *src)
{
int i = 0;

while (i <= _strlen(src))
{
dest[i] = src[i];
i++;
}

return (dest);
}

/**
 * _strcat - Concatenates two strings.
 * @dest: Destination.
 * @src: Source.
 * Return: Concatenated string.
 */
char *_strcat(char *dest, char *src)
{
int i = 0;
char *temp = dest;

while (*temp != '\0')
{
temp++;
}

while (i < _strlen(src))
{
*temp = src[i];
temp++;
i++;
}

*temp = '\0';

return (dest);
}

/**
 * _strdup - Duplicates a string.
 * @str: The input string.
 * Return: Pointer to a newly allocated memory block containing a duplicate.
 */
char *_strdup(char *str)
{
char *myStr;
int i = 0;

if (str == NULL)
return (NULL);

myStr = (char *)malloc(_strlen(str) * sizeof(char) + 1);

if (myStr == NULL)
return (NULL);

while (i <= (int)_strlen(str))
{
myStr[i] = str[i];
i++;
}

return (myStr);
}
