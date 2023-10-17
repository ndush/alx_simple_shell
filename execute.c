
#include "shell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
void execute_command(char *args[]) {
	pid_t pid;
	int x;

	if ((pid = fork()) < 0) {
		perror("fork failed");
		return;
	} else if (pid == 0) {
		if (strcmp(args[0], "echo") == 0) {
			for (x = 1; args[x] != NULL; x++) {
				write(STDOUT_FILENO, args[x], strlen(args[x]));
				if (args[x + 1] != NULL) {
					write(STDOUT_FILENO, " ", 1);
				}
			}
			write(STDOUT_FILENO, "\n", 1);
			exit(0);
		}

		if (execvp(args[0], args) == -1) {
			perror("execution failed");
			exit(1);
		}
	} else {
		int status;
		wait(&status);
	}
}


