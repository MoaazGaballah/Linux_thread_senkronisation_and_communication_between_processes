#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "reader.h"

pthread_mutex_t threadReaderLock;
pthread_mutex_t threadCalculatorLock;

int generalAddition;
int square;
int lineCounter;
int pipefd[2];
int readFileStatus = 1;

int isNumber(char *str)
{
    // printf("this is strlen(str) : %d\n", strlen(str));
    for (int i = 0; i < strlen(str); i++)
    {
        // printf("This is str[i] : %c\n",
        //        str[i]);
        if (isdigit(str[i]) == 0 && i + 1 != strlen(str))
        {
            // printf("This is not valid in number %c\n", str[i]);
            return 0;
        }
    }

    return 1;
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void *threadReader()
{
    char allNumbers[1000];
    read(pipefd[0], allNumbers, sizeof(allNumbers));
    char line[2] = "\n";
    char *numberToken = strtok(allNumbers, line);
    // numberToken[strlen(numberToken) - 1] = 0;
    for (int i = 0; i < lineCounter; i++)
    {
        // printf("isNumber(numberToken) is %d\n", isNumber(numberToken));
        if (isNumber(numberToken) == 0)
        {
            // printf("i am here\n");
            // bittiStatus = 0;
            readFileStatus = 0;
            // return 1;
            pthread_exit(1);
            // pthread_kill(pthread_self());

            // pthread_detach(pthread_self());
        }

        pthread_mutex_lock(&threadReaderLock);
        // printf("i am locking reader\n");
        square = atoi(numberToken);
        // printf("Square before sqrt in reader is : %d\n", square);
        // threadCalculator();
        // printf("i is  is %d\t", i);
        // printf("lineCounter is %d\n", lineCounter);
        printf("numberToken is %s\n", numberToken);

        numberToken = strtok(NULL, line);
        // numberToken[strlen(numberToken) - 1] = 0;

        // do this to the last number in data.txt
        // if (i + 2 == lineCounter)
        // {
        //     numberToken[strlen(numberToken) - 1] = 0;
        // }

        // printf("i am unlocking calculator\n");
        pthread_mutex_unlock(&threadCalculatorLock);
    }
}

void *threadCalculator()
{
    for (int i = 0; i < lineCounter; i++)
    {
        pthread_mutex_lock(&threadCalculatorLock);
        // if (threadStatus == 0)
        // {
        //     // return 1;
        //     pthread_exit(1);
        //     // pthread_kill(pthread_self());
        //     // pthread_detach(pthread_self());
        // }

        // printf("i am locking calculator\n");
        // printf("Square before sqrt in calculator is : %d\n", square);
        generalAddition += square * square;
        printf("Your general addition now is : %d\n", generalAddition);
        // printf("i am unlocking reader\n");
        pthread_mutex_unlock(&threadReaderLock);
    }
}

int main()
{

    int pid, status;

    if (pipe(pipefd) < 0)
    {
        perror("pipe is not working !");
        exit(1);
    }

    while (readFileStatus == 1)
    {

        pid = fork();

        if (pid == 0)
        {
            status = execve("reader", NULL, NULL);
            perror("execve: execve failed\n");
            close(pipefd[1]);
        }
        else
        {
            char allNumbers[1000];
            wait(&status);
            // get the line counter exist in data file
            lineCounter = WEXITSTATUS(status);
            if (lineCounter == 0)
            {
                exit(1);
            }

            // printf("lineCounter is from main %d\n", lineCounter);

            // printf("reading from pipe ...\n");

            pthread_mutex_init(&threadReaderLock, NULL);
            pthread_mutex_init(&threadCalculatorLock, NULL);

            pthread_mutex_lock(&threadCalculatorLock);

            pthread_t readerThread;
            pthread_t calculatorThread;

            generalAddition = 0;
            square = 0;

            void *status;

            pthread_create(&readerThread, NULL, threadReader, NULL);
            pthread_create(&calculatorThread, NULL, threadCalculator, NULL);

            pthread_join(readerThread, &status);
            // pthread_join(calculatorThread, &status);

            // pthread_mutex_destroy(&threadReaderLock);
            // pthread_mutex_destroy(&threadCalculatorLock);

            // // calling thread reader
            // threadReader(lineCounter, pipefd);

            // read(pipefd[0], allNumbers, sizeof(allNumbers));
            // // s[strlen(s) - 1] = 0;

            // char line[2] = "\n";

            // char *numberToken = strtok(allNumbers, line);
            // // printf("this is the result from Token %d");
            // // printf("This is singleOrRangeToken : %s\n", singleOrRangeToken);

            // // printf("this is read value : %d\n", number);
            // // close(pipefd[0]);

            // // printf("sizeof lineCounter %d\n", lineCounter);
            // for (int i = 0; i < lineCounter; i++)
            // {
            //     printf("Token now is : %d\n", atoi(numberToken));

            //     numberToken = strtok(NULL, line);
            // }

            // // using fopen() with "w" mode will do the job for you.
            // // When you open a file with "w" flag it creates an empty file
            // // for writing. If a file with the same name already exists
            // // its contents are erased and the file is treated as an empty
            // // new file.

            // fopen("data.txt", "w");
            // // printf("program reads : \n%d", atoi(numberToken));

            // // printf("Read from pipe ... Length is:  %s\n", s);
        }

        // deleting the content of data.txt file
        fopen("data.txt", "w");

        // wait for 10 seconds
        // for (int i = 0; i < 250; i++)
        // {
            // delay(1);
            sleep(15);
        // }
    }
    // printf("this is from main (global reader): %d\n", test);
    printf("BİTTİ\n");
    return 0;
}