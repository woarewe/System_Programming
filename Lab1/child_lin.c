#include <stdio.h>
#include <time.h>

int main()
{
	time_t cTime = time(0);
	printf("Current time - %s",ctime(&cTime));
	return 0;
}