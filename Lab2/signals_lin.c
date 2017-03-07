#include "signals.h"


int kill(pid_t Pid, int Signal);
int usleep(unsigned int usec);

void addProcess(struct Cell **head)
{
	struct ProcessInfo temp;
	temp.id = fork();
	switch(temp.id)
	{
		case 0:
		{
			while(1)
			{
				printf("%d\n",getpid());
				usleep(1000000);
			}
		}
		break;
		case -1:
		{
			printf("Error!");
			exit(404);
		}
		break;
		default:
			push(head,temp);
		break;
	}
}

void deleteOneProcess(struct ProcessInfo info)
{
	kill(info.id, SIGKILL);
}

void deleteAll(struct Cell **head)
{
	while(*head)
		kill(pop(head).id,SIGKILL);
}

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