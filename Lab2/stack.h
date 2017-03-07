#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#ifdef __linux__

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>

struct ProcessInfo
{
	pid_t id;
};

#else

#include <Windows.h>

struct ProcessInfo
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION procInfo;
};

#endif

struct Cell
{
	struct ProcessInfo information;
	struct Cell *next;
};

void push(struct Cell**, struct ProcessInfo);
struct ProcessInfo pop(struct Cell**);
void free_stack(struct Cell**);

#endif