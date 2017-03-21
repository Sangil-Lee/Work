#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	pid_t pid;

	printf("Calling fork\n");
	
	switch (pid=fork())
	{
		case -1:
				perror("fork failed");
				break;
		case 0:				// child process
				printf("CHILD PROCESS : %d\n", getpid());
				sleep(3);
				system("echo CHILD");
				system("ps -l");
				exit(0);
				break;
		default:
#ifdef ZOM
				while(1) {
					printf("PARENT PROCESS : %d\n", getpid());
					system("ps -l");
					sleep(2);
				}
#else	
				printf("PARENT PROCESS : %d\n", getpid());
				printf("Return Value of fork : %d\n", pid);
				sleep(5);
				system("echo PARENT");
				system("ps -l");
#endif
				break;
	}
	return 0;
}
