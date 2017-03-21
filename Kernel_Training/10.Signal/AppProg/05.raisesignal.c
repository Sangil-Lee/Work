#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[])
{
	int a,b;
	
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	if(b == 0) raise(SIGFPE);
	printf("divide....\n");
	a = a/b;
	
	return 0;
}