#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "reader.h"

void readFile()
{
	FILE *in = fopen("data.txt", "r");
	int sum = 0;
	char *line = malloc(sizeof(char) * 100);
	while (fgets(line, 100, in) != NULL)
	{

		// sum += atoi(line);
		// printf("%d\n", atoi(line));

		// Child process closes up input side of pipe
		// close(pipefd[0]);
		printf("Writing this line to pipe : %s\n", line);
	}

	fclose(in);
	free(line);
	// return sum;
}

// int test = 2;
int main(int argc, char *argv[])
{
	// char s[1000];
	int pipefd[2];
	FILE *in = fopen("data.txt", "r");
	char *line [100];

	fgets(line, 100, in);
	// printf("Alt program:getpid: %d  getpppid: %d\n", getpid(), getppid());
	printf("Writing this line to pipe : %s\n", line);
	
	write(4, line, strlen(line) );
	// printf("\nthe above line written to pipe\n");
	return 0;
}
