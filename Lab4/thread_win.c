#include "header.h"


DWORD WINAPI printID(LPVOID t)
{
	while(1)
	{
		WaitForSingleObject(t, INFINITE);
		printf("Current thread id: %d\n",(int)GetCurrentThreadId());
		ReleaseMutex(t);
		Sleep(1000);
	}
	return 0;
}
int get_button()
{
	return getch();
}


void deleteOneThread(struct ThreadInfo info)
{
	TerminateThread(info.thread,0);
}

struct ThreadInfo createNewThread(struct MutexInfo *mInfo)
{
	struct ThreadInfo info;
	info.thread = CreateThread(NULL, 0, printID, (void *)&mInfo->mutex, 0, NULL);
	return info;
}

void init(struct MutexInfo *info)
{
	info->mutex = CreateMutex(NULL, FALSE, "SHIELD");
}

void deleteAll(struct Cell **head)
{
	while(*head)
		deleteOneThread(pop(head));
}
