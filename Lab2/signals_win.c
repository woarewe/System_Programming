#include "signals.h"
#include <conio.h>

void addProcess(struct Cell **head)
{
	struct ProcessInfo temp;
	TCHAR newProcessName[] = TEXT("run.exe");
	ZeroMemory(&temp.startupInfo, sizeof(STARTUPINFO));
	if (CreateProcess(NULL, newProcessName, NULL, NULL, FALSE, 0, NULL, NULL, &temp.startupInfo, &temp.procInfo) == TRUE)
			push(head,temp);
	else exit(404);
	
}

void deleteOneProcess(struct ProcessInfo info)
{
	TerminateProcess(info.procInfo.hProcess, 0);
}

void deleteAll(struct Cell **head)
{
	while(*head)
		TerminateProcess(pop(head).procInfo.hProcess,0);
}

int get_button()
{
	return _getch();
}