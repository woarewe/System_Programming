#include "header.h"
#include <string.h>

char *files[5] = { "Files\\1", "Files\\2", "Files\\3", "Files\\4", "Files\\5"};

DWORD WINAPI threadReader(LPVOID t)
{
  struct Info *info = (struct Info *)t;
	ReadDataFromFile readDataFromFile = (ReadDataFromFile) GetProcAddress(info->library,"ReadDataFromFile");

	for(int i = 0; i < 5; i++)
	{
    while(info->flag) {}
		WaitForSingleObject(info->mutex, INFINITE);
		HANDLE hFile = CreateFile(info->fileNames[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		(*readDataFromFile)(hFile, info->buffer);
		CloseHandle(hFile);
    info->flag = 1;
		ReleaseMutex(info->mutex);
}
  info->threadsNumber--;
  return 0;
}

DWORD WINAPI threadWriter(LPVOID t)
{
  struct Info *info = (struct Info *)t;
	WaitForSingleObject(info->mutex, INFINITE);
	HANDLE hFile = CreateFile("Files\\All", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	WriteDataInFile writeDataInFile = (WriteDataInFile) GetProcAddress(info->library,"WriteDataInFile");
	ReleaseMutex(info->mutex);

	for(int i = 0; i < 5; i++)
	{
    while(!info->flag) {}
		WaitForSingleObject(info->mutex, INFINITE);
		(*writeDataInFile)(hFile, info->buffer);
		ReleaseMutex(info->mutex);
    info->flag = 0;
	}
	CloseHandle(hFile);
  info->threadsNumber--;
  return 0;
}

void initInfo(struct Info *info)
{
  info->fileNames = files;
  info->library = LoadLibrary("lib_win.dll");
  info->threadsNumber = 0;
  info->flag = 0;

}

void createMutex(struct Info *info)
{
  info->mutex =  CreateMutex(NULL, FALSE, NULL);
}

void runThreads(struct Info *info)
{
	info->readThread = CreateThread(NULL, 0, threadReader, (void *)info, 0, NULL);
  info->threadsNumber++;
  info->writeThread = CreateThread(NULL, 0, threadWriter, (void *)info, 0, NULL);
  info->threadsNumber++;
}

void waitThreads(struct Info *info)
{
  while(info->threadsNumber);
  CloseHandle(info->readThread);
	CloseHandle(info->writeThread);
	CloseHandle(info->mutex);
	FreeLibrary(info->library);
  printf("All good!\n");

}
