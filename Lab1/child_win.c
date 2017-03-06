#include <stdio.h>
#include <time.h>
#include <Windows.h>
int main()
{
	time_t cTime = time(0);
	printf("Current time - %s",ctime(&cTime));
	system("pause");
	return 0;
}