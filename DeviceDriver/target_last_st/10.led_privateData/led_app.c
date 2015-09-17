/***************************************
 * Filename: led_app.c
 * Title: LED Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/types.h>
#include <signal.h>

#define MYDEBUG
#ifdef MYDEBUG
#define	mprintf(x...)	printf("[APP]"x)
#else
#define	mprintf(x...)	do {}while(0)
#endif

int main(void)
{
	pid_t pid;
    int fd1=-1, fd2=-1;
	int i=0;

    int menu;

	mprintf("Main Start!!\n");

    switch(pid = fork())
    {
      case -1:    // Fork Error
        perror("LED_app");
        exit(-1);
      case 0:    // Chile Process
        mprintf("[C]Child_%d...\n", i);
    	if((fd1 = open("/dev/led0", O_RDWR))<0) {
          perror("/dev/led0");
          exit(-1);
		}
        for(i=0; ;i++)
        {
          sleep(3);
		  //if(i%2) continue;
          mprintf("[C]write(%d)...\n", i);
		  menu = 1;
		  write(fd1, &menu, sizeof(menu));
        }
    	close(fd1);
        exit(0);
      default:    // Parent Process
        mprintf("[P]Parent%d...\n", i);
    	if((fd2 = open("/dev/led0", O_RDWR))<0) {
          perror("/dev/led0");
          exit(-1);
		}
        for(i=0; ;i++)
        {
          sleep(3);
		  //if(!(i%2)) continue;
		  menu = 2;
          mprintf("[P]write(%d)...\n", i);
		  write(fd2, &menu, sizeof(menu));
        }
    	close(fd2);
        exit(0);
    }
    
	kill(pid, SIGKILL);

    return 0;
}
