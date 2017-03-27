#include "channel.h"

void connecting(struct IpcData *data)
{
	key_t key = ftok("server.c", 228);
	data->semid = semget(key, 1, 0666 | IPC_CREAT);
	semctl(data->semid, 0, SETVAL, 0);

	data->lock_res.sem_num = 0;
	data->lock_res.sem_op = -1;
	data->rel_res.sem_num = 0;
	data->rel_res.sem_op = 1;
	data->lock_res.sem_flg = IPC_NOWAIT;
	data->rel_res.sem_flg = IPC_NOWAIT;

	unlink("fifo1");
	mkfifo("fifo1", O_RDWR);

}

void run(struct IpcData *data)
{
	char buffer[100];
	int k;
	while(1)
	{
		semop(data->semid, &data->lock_res, 1);
		fflush(stdin);
		fgets(buffer,100, stdin);
		k = strlen(buffer);
		buffer[k-1] = '\0';
		data->fd_fifo = open("fifo1", O_WRONLY);
		write(data->fd_fifo,buffer,100);
		close(data->fd_fifo);
		semop(data->semid, &data->rel_res, 1);
		if(!strcmp(buffer, "exit\0"))
			break;
	}
}

void clean(struct IpcData *data)
{
	semctl(data->semid, 0, IPC_RMID);
	unlink("fifo1");
}
