#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void p_handler(int), c_handler(int);
int i=0;

int main(void)
{
	pid_t pid, ppid;
	struct sigaction act;
	
	pid = fork();
	if(pid == 0) {
		act.sa_handler = c_handler;
		//sigfillset(&(act.sa_mask));
		sigaction(SIGUSR1, &act, NULL);
		ppid = getppid();
		while(1) {
			sleep(2);
			kill(ppid, SIGUSR1);
			pause();
		}
	} else if(pid > 0) {
		act.sa_handler = p_handler;
		//sigfillset(&(act.sa_mask));
		sigaction(SIGUSR1, &act, NULL);
		while(1) {
			pause();
			sleep(1);
			kill(pid, SIGUSR1);
		}
	} else {
		perror("fork");
		exit(1);
	}
	
	return 0;
}

void p_handler(int signo)
{
	printf("Parent handler(%d): called %d times.\n", signo, ++i);
}

void c_handler(int signo)
{
	printf("Child handler(%d): called %d times.\n", signo, ++i);
}