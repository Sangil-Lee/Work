APP	= poll_app

#KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR	:= /work/android_kernel_2.6.32_sv210
KDIR	:= /work_dd/linux_kernel_2.6.32_sv210
#CC = arm-linux-gcc
CC = arm-none-linux-gnueabi-gcc

PWD	:= $(shell pwd)

all : app

app:
#	$(CC) -static -o $(APP) $(APP).c
	$(CC) -o $(APP) $(APP).c
	cd led_POLL; make clean; make
	cd key_POLL; make clean; make
	cp led_POLL/led_drv.ko .
	cp key_POLL/key_drv.ko .

clean:
	-rm -rf $(APP) *.ko
	cd led_POLL; make clean
	cd key_POLL; make clean
