#include "header.h"

int main()
{
	struct Cell **head = (struct Cell**)malloc(sizeof(struct Cell*));
	*head = NULL;

	struct MutexInfo mInfo;
	init(&mInfo);

	while(1)
	{
		switch(get_button())
		{
			case '+':
			{
				push(head, createNewThread(&mInfo));
			}
			break;
			case '-':
			{
				if(*head)
					deleteOneThread(pop(head));
			}
			break;
			case 'q':
			{
				deleteAll(head);
				return 0;
			}
			break;
		}
	}
	return 0;
}
