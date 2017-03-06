#include "proces.h"

int main()
{
	struct ProcessData *data = runProcess();
	deleteProcess(data);
	return 0;
}