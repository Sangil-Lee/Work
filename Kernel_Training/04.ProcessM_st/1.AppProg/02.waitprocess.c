#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int status, exit_status;

	printf("Calling fork\n");
	switch (pid=fork())
	{
		case -1:
				perror("fork failed");
				exit(1);
		case 0:				// child process
				printf("CHILD PROCESS : %d\n", getpid());
				sleep(4);
				exit(5);
	}
	printf("PARENT PROCESS : %d\n", getpid());
	if((pid=wait(&status)) == -1) {
		perror("wait failed");
		exit(2);
	}
	if(WIFEXITED(status)) {
		exit_status = WEXITSTATUS(status);
		printf("Exit status from %d : %d(%d)\n", pid, exit_status, status);
	}
	exit(0);
}
