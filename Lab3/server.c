#include "channel.h"


int main()
{
	struct IpcData data;

	connecting(&data);
	run(&data);
	clean(&data);	
	return 0;
}