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

int main(void)
{
    int fd;
    int menu=0, led_func=0, led_no=0;
    int rst;
    int timerPeriod=1;

	pid_t pid;
	int i, loop;
    
    fd = open("/dev/led0", O_RDWR);
    printf("fd = %d\n", fd);
    
    if (fd<0) {
        perror("/dev/led0");
        exit(-1);
    }
    else
        printf("[APP]LED detected...\n");
    
#if 0
    while(1)
    {
        printf("\n\n===== LED Test Program ====\n");
        printf(" 1. LED On\n");
        printf(" 2. LED Off\n");
        printf(" 3. LED Status\n");
        printf(" 4. OSTimer_1 Interrupt Period Setting\n");
        printf(" 0. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &menu);
        } while(menu<0 || menu>4);

        if(menu == 0) break;

	if(menu == 4) {
	    printf("Input OSTimer_1 Int. Period Value(1~10) => ");
	    scanf("%d", &timerPeriod);
	    ioctl(fd, 5<<8, timerPeriod);
	    continue;
	}

        do {
	    printf("\nInput LED Number(0~7) => ");
            scanf("%d", &led_no);
        } while(menu<0 || menu>7);

	led_func = menu;
	menu = menu << 8;
	menu |= led_no;

	switch(led_func) {
	case 1:
	case 2:
        	printf("write call(menu:%x)\n", menu);
		write(fd, &menu, sizeof(menu));
		break;
	case 3:
        	printf("read call(menu:%x)\n", menu);
		read(fd, &rst, sizeof(rst));
        	printf("read result(rst:%x)\n", rst);
		break;
	}
    }
#endif

	//menu = 2 << 8;	//2: Led Offf
	//menu |= 8;		//led_no : 8(all)
	//write(fd, &menu, sizeof(menu));

	ioctl(fd, 5, 3);

    close(fd);


    switch(pid = fork())
    {
      case -1:    // Fork Error
        perror("LED_app");
        exit(-1);
      case 0:    // Chile Process
        for(i=0; ;i++)
        {
          printf("[C]Child_%d...\n", i);
          sleep(1);
        }
        exit(0);
    }

    // Parent Process

    for(loop=0; loop<8; loop++)
    {
    	fd = open("/dev/led0", O_RDWR);
        //fd = open("/dev/led0", O_RDWR|O_NONBLOCK);
        if (fd<0) {
            perror("/dev/led0");
            exit(-1);
        }
        else printf("[APP]LED detected...\n");

		menu = 2 << 8;	//2: Led Offf
		menu |= 8;		//led_no : 8(all)
		write(fd, &menu, sizeof(menu));

        while(1)
        {
            printf("[P]Waiting LED Write Operation...\n");
			menu = 1 << 8;	//2: Led On
			menu |= loop;		//led_no : loop 
			if(write(fd, &menu, sizeof(menu)) > 0) break;
            sleep(1);
        }
        printf("[P]LED Write Ok(%d)!!\n", loop);
        close(fd);
    }

	kill(pid, SIGKILL);
    
    return 0;
}
