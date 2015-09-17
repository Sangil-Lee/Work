/***************************************
 * Filename: key_app.c
 * Title: KEY Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    int menu=0;
    int key_stat = 0;
    int i;
    
    fd = open("/dev/key0", O_RDWR);
    printf("fd = %d\n", fd);
    
    if (fd<0) {
        perror("/dev/key0");
        exit(-1);
    }
    else
        printf("[APP]KEY detected...\n");
    
    while(1)
    {
        printf("\n\n===== KEY Test Program ====\n");
        printf(" 1. KEY Check\n");
        printf(" 0. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &menu);
        } while(menu<0 || menu>1);

        if(menu == 0) break;

	for(i=1; i<=5; i++) {
            printf("\n\nKEY Checking...\n");
	    read(fd, &key_stat, sizeof(key_stat));
            printf("read result(key_stat:%x)\n", key_stat);
	    sleep(1);
	}
    }

    close(fd);
    
    return 0;
}
