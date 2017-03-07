#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

int main()
{
	while(1)
	{
		printf("%d\n",GetCurrentProcessId());
		Sleep(800);
	}
	return 0;
}