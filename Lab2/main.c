#include "signals.h"

int main()
{
	struct Cell **head = (struct Cell**)malloc(sizeof(struct Cell*));
	*head = NULL;
	
	while(1)
	{
		switch(get_button())
		{
			case '+':
				addProcess(head);		
			break;
			case '-':
				if(*head)
					deleteOneProcess(pop(head));
			break;
			case 'q':
			{
				deleteAll(head);
				free(head);
				return 0;
			}
			break;
		}
	}
	return 0;
}	