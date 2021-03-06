#include "channel.h"

void connecting(struct IpcData *data)
{
	key_t key = ftok("server.c", 228);
	data->semid = semget(key, 0, 0666);

	data->lock_res.sem_num = 0;
	data->lock_res.sem_op = -1;
	data->rel_res.sem_num = 0;
	data->rel_res.sem_op = 1;
	data->lock_res.sem_flg = IPC_NOWAIT;
	data->rel_res.sem_flg = IPC_NOWAIT;

}

void run(struct IpcData *data)
{
	char buffer[100];

	while(1)
	{
		semop(data->semid, &data->lock_res, 1);
		data->fd_fifo = open("fifo1", O_RDONLY);
		read(data->fd_fifo, &buffer, 100) ;
		close(data->fd_fifo);
		if(!strcmp(buffer, "exit\0"))
		{
			semop(data->semid, &data->rel_res, 1);
			break;
		}
		puts(buffer);
		semop(data->semid, &data->rel_res, 1);

	}
}
