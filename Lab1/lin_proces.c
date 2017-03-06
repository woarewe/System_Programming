#include "proces.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

struct ProcessData
{
	pid_t processID;
};

struct ProcessData *runProcess()
{
	struct ProcessData *data = (struct ProcessData *)malloc(sizeof(struct ProcessData));
	data->processID = fork();
	switch(data->processID)
	{
		case 0:
		{
			printf("It's child process. ID = %d\n",getpid());
			execl("child_lin","child_lin",NULL);
			exit(getpid());	
		}
		case -1:
		{
			printf("Error!");
		}
		default:
		{
			printf("It's parent process. ID = %d\n",getpid());
		}	
	}
	return data;
}

void deleteProcess(struct ProcessData *data)
{
	wait(&data->processID);
}