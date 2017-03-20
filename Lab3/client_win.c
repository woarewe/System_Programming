#include "channel.h"

void connecting(struct IpcData *data)
{
	data->semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("Semaphore"));
	data->channel = CreateFile(TEXT("\\\\.\\pipe\\$MyPipe$"), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	ConnectNamedPipe(data->channel, NULL);
}

void run(struct IpcData *data)
{
	char buffer[100];
	while(1)
	{
		ReleaseSemaphore(data->semaphore, 1, NULL);
		DWORD  cbRead;
		ReadFile(data->channel, buffer, 100, &cbRead, NULL);
		if(!strcmp(buffer, "exit\0"))
		{
			ReleaseSemaphore(data->semaphore, 0, NULL);
			break;
		}
		else
		{
			puts(buffer);
			ReleaseSemaphore(data->semaphore, 0, NULL);
		}
		WaitForSingleObject(data->semaphore, INFINITE);
	}
}	

