/***************************************
 * Filename: led_app.c
 * Title: LED Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <signal.h>

#include <sys/poll.h>

int main(void)
{
    int fd_led, fd_key;
    int menu=0, led_func=0, led_no=0;
    int rst;
    int timerPeriod=1;
	int key_state = 0;

    struct pollfd Events[2], EventKey, EventLed;
	int retval;

	pid_t pid;
	int i, loop;

    printf("\nPOLL Test Program Start...\n");

#if 1
    fd_led = open("/dev/led0", O_RDWR);
    if (fd_led<0) {
        perror("/dev/led0");
        exit(-1);
    }
    else
        printf("[APP]LED detected...\n");
#endif


#if 1
    fd_key = open("/dev/key0", O_RDWR);
    if (fd_key<0) {
        perror("/dev/key0");
        exit(-1);
    }
    else
        printf("[APP]KEY detected...\n");
#endif

    for(loop=0; loop<=7; loop++)
    {
		//All LED Off
        ioctl(fd_led, 3, 0);

#if 1
		memset(Events, 0, sizeof(Events));
    	Events[0].fd = fd_key;
    	Events[0].events = POLLIN;
    	Events[1].fd = fd_led;
    	Events[1].events = POLLOUT;
#endif
#if 0
		memset(&EventKey, 0, sizeof(EventKey));
    	EventKey.fd = fd_key;
    	EventKey.events = POLLIN;
#endif
#if 0
		memset(&EventLed, 0, sizeof(EventLed));
    	EventLed.fd = fd_led;
    	EventLed.events = POLLOUT;
#endif

#if 1
        //LED Int. Period Setting
        //timerPeriod = loop%4 + 1;	
		if(loop%2) timerPeriod = 1;
		else timerPeriod = 9;
		//timerPeriod = 2;
        ioctl(fd_led, 5, timerPeriod);
#endif


#if 1
        //printf("-------------->[APP]Poll_Waiting!!!\n");
        printf("-------------->[APP]Poll_Waiting(%x,%x)!!!\n", Events[0].revents, Events[1].revents);
        retval = poll(Events, 2, 10000);    // Event waiting
        printf("-------------->[APP]Poll_Wakeup(%x,%x)!!!\n", Events[0].revents, Events[1].revents);
#endif
#if 0
        printf("-------------->[APP]Poll_Waiting(%x,%x,%x)!!!\n", EventKey.fd,EventKey.events,EventKey.revents);
        retval = poll(&EventKey, 1, -1);    // Event waiting
        printf("-------------->[APP]Poll_Wakeup(%x,%x,%x)!!!\n", EventKey.fd,EventKey.events,EventKey.revents);
#endif
#if 0
        printf("-------------->[APP]Poll_Waiting(%x,%x,%x)!!!\n", EventLed.fd,EventLed.events,EventLed.revents);
        retval = poll(&EventLed, 1, 5000);    // Event waiting
        printf("-------------->[APP]Poll_Wakeup(%x,%x,%x)!!!\n", EventLed.fd,EventLed.events,EventLed.revents);
#endif

        if(retval < 0) {
            perror("Poll_Test");
            exit(EXIT_FAILURE);
        }
        if(retval == 0) {
            printf("[APP]Wakeup_Poll_Event:No Event!!\n");
            continue;
            //exit(EXIT_FAILURE);
        }


#if 1
        if(Events[0].revents & POLLIN)
        {
            printf("--------------->[APP]Wakeup_Poll_KEY_Event\n");
            if(read(Events[0].fd, (void *)&key_state, sizeof(key_state)) > 0) 
                printf("-------->[APP]Wakeup_Poll_Event:Event_KEY(%x)!!\n", key_state);
            else 
                printf("[APP]Wakeup_Poll_Event:Event_KEY(Data Read Error)!!\n");
        }

        if(Events[1].revents & POLLOUT)
        {
            printf("--------------->[APP]Wakeup_Poll_LED_Event\n");
			menu = 1 << 8;		//1: Led On
			menu |= loop;		//led_no : loop
            if(write(Events[1].fd, (void *)&menu, sizeof(menu)) > 0) 
                printf("-------->[APP]Wakeup_Poll_Event:Event_LED!!\n");
            else 
                printf("[APP]Wakeup_Poll_Event:Event_LED(Data Write Error)!!\n");
        }
		sleep(1);
    }
#endif
#if 0
        if(EventKey.revents & POLLIN)
        {
            printf("--------------->[APP]Wakeup_Poll_KEY_Event\n");
            if(read(EventKey.fd, (void *)&key_state, sizeof(key_state)) > 0) 
                printf("-------->[APP]Wakeup_Poll_Event:Event_KEY(%x)!!\n", key_state);
            else 
                printf("[APP]Wakeup_Poll_Event:Event_KEY(Data Read Error)!!\n");
        }
    }
#endif
#if 0
        if(EventLed.revents & POLLOUT)
        {
            printf("--------------->[APP]Wakeup_Poll_LED_Event\n");
			menu = 1 << 8;		//1: Led On
			menu |= loop;		//led_no : loop
            if(write(EventLed.fd, (void *)&menu, sizeof(menu)) > 0) 
                printf("-------->[APP]Wakeup_Poll_Event:Event_LED!!\n");
            else 
                printf("[APP]Wakeup_Poll_Event:Event_LED(Data Write Error)!!\n");
        }
    }
#endif

    close(fd_key);
    close(fd_led);
    
    return 0;
}
