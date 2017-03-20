#include "channel.h"

void connecting(struct IpcData *data)
{
	data->semaphore = CreateSemaphore(NULL, 0, 1, "Semaphore1");

	data->channel = CreateNamedPipe(TEXT("\\\\.\\pipe\\$MyPipe$"), PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, 100, 100, NMPWAIT_USE_DEFAULT_WAIT, NULL);

	ConnectNamedPipe(data->channel, NULL);			
}

void run(struct IpcData *data)
{
	char buffer[100];
	int k;
	
	while(1)
	{
		ReleaseSemaphore(data->semaphore, 1, NULL);
		fflush(stdin);
		fgets(buffer,100, stdin);
		k = strlen(buffer);
		buffer[k-1] = '\0';
		DWORD  cbWritten;
	WriteFile(data->channel, buffer, strlen(buffer) + 1, 
  &cbWritten, NULL);
		ReleaseSemaphore(data->semaphore, 0, NULL);
		WaitForSingleObject(data->semaphore, INFINITE);
		if(!strcmp(buffer, "exit\0"))
			break;
		
	}
}	

void clean(struct IpcData *data)
{
	CloseHandle(data->channel);
	CloseHandle(data->semaphore);
}
