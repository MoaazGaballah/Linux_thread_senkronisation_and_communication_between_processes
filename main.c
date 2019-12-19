#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "reader.h"

// test = 1;

int main()
{
    int pipefd[2];
    int pid, status;

    if (pipe(pipefd) < 0)
    {
        perror("pipe is not working !");
        exit(1);
    }

    pid = fork();

    if (pid == 0)
    {
        printf("s4334343434s");
        status = execve("reader", NULL, NULL);
        perror("execve: execve failed\n");
        close(pipefd[1]);
        
    }
    else
    {
        int i;
        // int s ;       // returned value from fork
        char s[1000];
        wait(&status);
        // WEXITSTATUS(status); // this will be send to threadReader
        // Parent process closes up output side of pipe
        // close(pipefd[1]);

        // printf("Ana program:getpid: %d  getpppid: %d\n", getpid(), getppid());

        // Read in a string from the pipe
        // printf("sssss%s\n", s);
        printf("reading from pipe ...\n");
        // while (read(pipefd[0], s + i, 1) == 1)
        // {

        //   if (s[i] == '\n')
        //   {
        //     s[i] = '\0';
        //     printf("%s\n", s);
        //     i = 0;
        //   }
        //   else
        //   {
        //     i++;
        //   }
        // }
        read(pipefd[0], s, strlen(s));
        s[strlen(s) - 1] = 0;
        // printf("this is read value : %d", read(pipefd[0], &s, sizeof(s)));
        //close(pipefd[0]);
        printf("program reads : %s\n", s);

        // printf("Read from pipe ... Length is:  %s\n", s);
    }
    // printf("this is from main (global reader): %d\n", test);
    return 0;
}