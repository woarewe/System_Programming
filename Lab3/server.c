#include "channel.h"


int main()
{
	struct IpcData data;

	connecting(&data);
	run(&data);
	
	return 0;
}