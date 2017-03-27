#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#else

#include <conio.h>
#include <Windows.h>

#endif

struct IpcData 
{
#ifdef __linux__
	int fd_fifo;
	int semid;
	struct sembuf lock_res;
	struct sembuf rel_res;
#else
	HANDLE semaphore;
	HANDLE channel;
#endif
};

void connecting(struct IpcData *);
void run(struct IpcData *);
void clean(struct IpcData *);
#endif