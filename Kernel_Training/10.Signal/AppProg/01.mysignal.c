#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
	write(1, "Control_C\n", 10);
}
int main(void)
{
	signal(SIGINT,handler);
	//signal(SIGINT, SIG_IGN);
	//signal(SIGINT, SIG_DFL);
	while(1)
	{
		printf("signal test\n");
		sleep(2);
	}
	return 0;	
}

