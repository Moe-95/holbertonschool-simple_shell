#include "shell.h"
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