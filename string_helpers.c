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

/**
 * _strcmp - Compares two strings.
 * @str1: String 1.
 * @str2: String 2.
 * Return: If str1 is lexicographically less than str2, returns a negative integer value.
 * If str1 is lexicographically greater than str2, returns a positive integer value.
 * If str1 is lexicographically equal to str2, returns zero.
 */
int _strcmp(char *str1, char *str2)
{
int i = 0;
int len1, len2;

if (str1 == NULL)
str1 = "";

if (str2 == NULL)
str2 = "";

len1 = _strlen(str1);
len2 = _strlen(str2);

if (len1 > len2)
return (1);
else if (len1 < len2)
return (-1);

while (i < len1)
{
if (str1[i] != str2[i])
return (-1);

i++;
}

return (0);
}
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