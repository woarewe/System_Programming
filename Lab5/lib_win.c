#include <windows.h>

#define BUFFER_SIZE 512

void WriteDataInFile(HANDLE hFile, char* buff)
{
	struct _OVERLAPPED overlapped;
	overlapped.Offset = 0xFFFFFFFF;
	overlapped.OffsetHigh = 0xFFFFFFFF;
	overlapped.hEvent = NULL;
	DWORD cbWritten;
	WriteFile(hFile, buff, strlen(buff), &cbWritten, &overlapped);
	WaitForSingleObject(hFile, INFINITE);
}

void ReadDataFromFile(HANDLE hFile, char* buff)
{
	memset(buff, 0, BUFFER_SIZE * sizeof(char));
	struct _OVERLAPPED overlapped;
	overlapped.Offset = 0;
	overlapped.OffsetHigh = 0;
	overlapped.hEvent = NULL;
	DWORD cbRead;
	ReadFile(hFile, buff, BUFFER_SIZE, &cbRead, &overlapped);
	//WaitForSingleObject(hFile, INFINITE);
}
