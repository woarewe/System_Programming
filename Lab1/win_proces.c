#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

struct ProcessData
{
	STARTUPINFO cif;
	PROCESS_INFORMATION pi;
};

struct ProcessData *runProcess()
{
	struct ProcessData *data = (struct ProcessData *)malloc(sizeof(struct ProcessData));
	TCHAR newProcessName[] = TEXT("child_win.exe");
	ZeroMemory(&data->cif, sizeof(STARTUPINFO));
	printf("It's child process. ID = %lu\n", GetCurrentProcessId());
	if (CreateProcess(NULL, newProcessName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &data->cif, &data->pi) == TRUE)		{
		system("pause");
		return data;
	}
	else return NULL;
}

void deleteProcess(struct ProcessData *data)
{
	WaitForSingleObject(data->pi.hProcess, INFINITE);
}