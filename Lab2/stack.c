#include "stack.h"

void push(struct Cell **head, struct ProcessInfo info)
{
	struct Cell *temp = (struct Cell*)malloc(sizeof(struct Cell));
	
	temp->information = info;
	temp->next = *head;
	*head = temp;
}

struct ProcessInfo pop(struct Cell **head)
{	
	if(!(*head))
		exit(404);

	struct Cell *temp = *head;
	struct ProcessInfo info = temp -> information;

	*head = (*head) -> next;
	
	free(temp);
	
	return info;
}

void free_stack(struct Cell **head)
{
	struct Cell *temp;
	
	while(*head)
	{
		temp = *head;
		*head = (*head) -> next;
		free(temp);
	}			
}