#include <stdbool.h>
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define MAX_ARGS 100
/**
 *custom_shell - function
 */
void jj(void);

void custom_shell(void)
{
	char *input = NULL;
	size_t input_size = 0;
	ssize_t input_bytes;
	char *args[MAX_ARGS];
	int argc = 0;
	char *arg;
	int exit_status = 0;
	bool interactive_mode = isatty(STDIN_FILENO);

	while (1)
	{
		char *cmd;

		if (interactive_mode)
		{
			write(STDOUT_FILENO, "simple_shell$ ", 14);
		}

		input_bytes = getline(&input, &input_size, stdin);

		if (input_bytes == -1)
		{
			if (feof(stdin))
			{
				if (interactive_mode)
				{
					write(STDOUT_FILENO, "\n", 2);
				}
				break;
			}
			else
			{
				write(STDERR_FILENO, "read failed\n", 12);
				continue;
			}
		}

		input[input_bytes - 1] = '\0';

		if (strlen(input) == 0)
		{
			continue;
		}

		if (input[0] == '#')
		{
			continue;
		}

		cmd = strtok(input, ";");

		while (cmd != NULL)
		{
			arg = strtok(cmd, " ");
			argc = 0;

			while (arg != NULL)
			{
				args[argc] = arg;
				argc++;
				arg = strtok(NULL, " ");
			}

			args[argc] = NULL;

			if (strcmp(args[0], "exit") == 0)
			{
				if (argc == 1)
				{
					free(input);
					exit(exit_status);
				}
				else if (argc == 2)
				{
					int status;

					if (isdigit(args[1][0]))
					{
						status = atoi(args[1]);
						exit_status = status;
						free(input);
						exit(status);
					}
					else
					{
						char errMsg[50];

						strcpy(errMsg, "exit: ");
						strcat(errMsg, args[1]);
						strcat(errMsg, ": numeric argument required\n");
						write(STDERR_FILENO, errMsg, strlen(errMsg));
						exit_status = 2;
					}
				}
				else
				{
					write(STDERR_FILENO, "exit: too many arguments\n", 25);
					exit_status = 2;
				}
			}
			else if (strcmp(args[0], "setenv") == 0)
			{
				if (argc == 3)
				{
					if (setenv(args[1], args[2], 1) != 0)
					{
						perror("setenv");
					}
				}
				else
				{
					write(STDERR_FILENO, "setenv: Invalid number of arguments\n", 36);
				}
			}
			else if (strcmp(args[0], "unsetenv") == 0)
			{
				if (argc == 2)
				{
					if (unsetenv(args[1]) != 0)
					{
						perror("unsetenv");
					}
				}
				else
				{
					write(STDERR_FILENO, "unsetenv: Invalid number of arguments\n", 38);
				}
			}
			else if (strcmp(args[0], "cd") == 0)
			{
				if (argc == 1 || (argc == 2 && strcmp(args[1], "-") == 0))
				{
					char *home_dir = getenv("HOME");

					if (home_dir == NULL)
					{
						write(STDERR_FILENO, "cd: HOME not set\n", 17);
					}
					else if (chdir(home_dir) != 0)
					{
						perror("cd");
					}
					else
					{
						setenv("PWD", home_dir, 1);
					}
				}
				else if (argc == 2)
				{
					if (chdir(args[1]) != 0)
					{
						perror("cd");
					}
					else
					{
						setenv("PWD", args[1], 1);
					}
				}
				else
				{
					write(STDERR_FILENO, "cd: Invalid number of arguments\n", 32);
				}
			}
			else
			{
				execute_command(args);
			}

			cmd = strtok(NULL, ";");
		}
	}

	free(input);
}

