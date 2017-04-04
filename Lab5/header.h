#ifndef HEADER_H
#define HEADER_H

#define BUFFER_SIZE 512
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <aio.h>
#include <dlfcn.h>

struct Info
{
  int threadsNumber;
  pthread_t readThread;
  pthread_t writeThread;
  pthread_mutex_t mutex;
  void *library;
  char **fileNames;
  int size;
  char buffer[512];
  volatile int flag;
};

void *threadReader(void *);
void *threadWriter(void *);

#else

#include <Windows.h>
#include <conio.h>
#include <process.h>
#include <locale.h>

struct Info
{
  HANDLE mutex;
  HANDLE readThread, writeThread;
  HINSTANCE library;
  char buffer[512];
  volatile int flag;
  char **fileNames;
  int threadsNumber;
};

DWORD WINAPI threadReader(LPVOID t);
DWORD WINAPI threadWriter(LPVOID t);

typedef void (*ReadDataFromFile)(HANDLE, char*);
typedef void (*WriteDataInFile)(HANDLE, char*);

#endif

void initInfo(struct Info *);
void createMutex(struct Info *);
void runThreads(struct Info *);
void waitThreads(struct Info *);


#endif
