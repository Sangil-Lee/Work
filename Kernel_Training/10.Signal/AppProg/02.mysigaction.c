#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int count=0;
void handler(int signo)
{
	count++;
	sleep(3);
}
int main(void)
{
	int i=0;
	struct sigaction act, oldact;
	
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGINT);
	sigaddset(&act.sa_mask,SIGQUIT);
	act.sa_flags=SA_RESTART;
	sigaction(SIGINT,&act,&oldact );
	while(i<5)
	{
		printf("signal test\n");
		sleep(1);
		i++;
	}
	sigaction(SIGINT,&oldact, NULL);
	printf("count = %d\n",count);
}

