/***************************************
 * Filename: led_app.c
 * Title: LED Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int retn;
    int fd;

    int flag = 0;
    int led_no, menu;
    char dev_name[100];
    
    
    while(1)
    {
        printf("\n\n===== LED Test Program ====\n");
        printf(" 0. LED0 Test\n");
        printf(" 1. LED1 Test\n");
        printf(" 2. LED2 Test\n");
        printf(" 3. LED3 Test\n");
        printf(" 4. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &led_no);
        } while(led_no<0 || led_no>4);
        if(led_no == 4) break;

        while(1)
        {
            printf("\nLED%d Test...\n", led_no);
            do {
                printf("Select(1.LED On,2.LED Off,3.All LED Off,4.Quit) => ");
                scanf("%d", &menu);
            } while(menu<1 || menu>4);
            if(menu == 4) break;
    
            sprintf(dev_name, "/dev/led%d", led_no);
            fd = open(dev_name, O_RDWR);
            if (fd<0) {
                perror(dev_name);
                exit(-1);
            }
            else printf("/dev/led%d detected(fd:%d)...\n", led_no, fd);
    
            if(menu==3) ioctl(fd, menu, flag);
            else write(fd, &menu, sizeof(menu));

            close(fd);
        }
    }
    
    return 0;
}
