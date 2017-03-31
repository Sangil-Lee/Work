#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	
	printf("Calling fork\n");	
	
	.....
	
	printf("After fork call(%d)!\n", getpid());
	
	return 0;
}
