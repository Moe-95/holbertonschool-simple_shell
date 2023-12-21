#include "shell.h"

/**
 *_realloc - Reallocates memory.
 *@ptr: Pointer to the memory to be reallocated.
 *@size: New size of the memory block.
 *
 *Return: Pointer to the newly allocated memory block.
 */
void *_realloc(void *ptr, size_t size)
{
void *new_ptr;
char *next_ptr;
char *new_next_ptr;
size_t dup_size;
size_t index = 0;

if (size == 0)
{
free(ptr);
return (NULL);
}

new_ptr = malloc(size);

if (new_ptr == NULL)
return (NULL);

next_ptr = (char *)ptr;
new_next_ptr = (char *)next_ptr;

if (size < sizeof(ptr))
dup_size = size;
else
dup_size = sizeof(ptr);

for (; index < dup_size; index++)
new_next_ptr[index] = next_ptr[index];

free(ptr);

return (new_ptr);
}

/**
 *_getline - Reads a line of characters from a file.
 *@linePtr: Pointer to the line.
 *@buff_size: Buffer size.
 *@my_file: The file to read from.
 *
 *Return: Number of bytes read.
 */
ssize_t _getline(char **linePtr, size_t *buff_size, FILE *my_file)
{
ssize_t bytes_read = 0;
size_t index = 0;
ssize_t result;
char *newSizePtr;
while (1)
{
if (index > *buff_size)
{
*buff_size += 8;
newSizePtr = _realloc(*linePtr, *buff_size);
if (newSizePtr == NULL)
{
perror("Memory allocation error");
return (-1);
}
*linePtr = newSizePtr;
}
result = read(fileno(my_file), &((*linePtr)[index]), 1);
if (result == -1)
{
perror("Error reading file");
return (-1);
}
else if (result == 0)
{
break;
}
bytes_read++;
if ((*linePtr)[index] == '\n')
break;
index++;
}
return (bytes_read);
}
