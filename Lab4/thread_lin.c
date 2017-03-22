#include "header.h"

int get_button()
{
	struct termios old, new;
	char ch;
	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	ch = getchar();
	tcsetattr(0, TCSANOW, &old);
	return ch;
}

void *printID(void *arg)
{
	while(1)
	{
		pthread_mutex_lock((pthread_mutex_t *)arg);
		printf("Current thread id: %d\n",(unsigned int)pthread_self());
		pthread_mutex_unlock((pthread_mutex_t *)arg);
		usleep(1000000);
	}
	return NULL;
}

void deleteOneThread(struct ThreadInfo info)
{
	pthread_cancel(info.thread);
}
struct ThreadInfo createNewThread(struct MutexInfo *mInfo)
{
	struct ThreadInfo info;
	pthread_create(&info.thread, NULL, printID, (void *)&mInfo->mutex);
	return info;
}

void init(struct MutexInfo *info)
{
	pthread_mutex_init(&info->mutex, NULL);
}

void deleteAll(struct Cell **head)
{
	while(*head)
		pthread_cancel(pop(head).thread);
}
