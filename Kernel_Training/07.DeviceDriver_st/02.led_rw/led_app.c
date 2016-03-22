#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd;

    int menu=0, led_func=0, led_no=0;
    int rst;
    
    fd = open("/dev/led", O_RDWR);
    printf("fd = %d\n", fd);
    
    if (fd<0) {
        perror("/dev/led");
        exit(-1);
    }
    else
        printf("[APP]LED detected...\n");
    
    while(1)
    {
        printf("\n\n===== LED Test Program ====\n");
        printf(" 1. LED On\n");
        printf(" 2. LED Off\n");
        printf(" 3. LED Status\n");
        printf(" 0. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &menu);
        } while(menu<0 || menu>3);

        if(menu == 0) break;

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

    close(fd);
    
    return 0;
}
