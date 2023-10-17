#include "shell.h"
/**
 * _shell - writes character to stdout
 * @c: the character to print
 * Return: on success return 1
 */
int _shell(char c)
{
	return (write(1, &c, 1));
}
