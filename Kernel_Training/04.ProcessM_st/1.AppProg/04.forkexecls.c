#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	pid_t pid;

	switch (pid=fork())
	{
		case -1:
				perror("fork failed");
				break;
		case 0:				// child process
				printf("CHILD PROCESS : %d\n", getpid());
				//execl("/bin/ls", "ls", "-l", (char *)0);
				execl("./mytask", "mytask", "10", (char *)0);
				perror("execl failed");
		default:
				printf("PARENT PROCESS : %d\n", getpid());
				wait((int *)0);
				printf("parent : ls completed!\n");
				exit(0);
	}
	return 0;
}
