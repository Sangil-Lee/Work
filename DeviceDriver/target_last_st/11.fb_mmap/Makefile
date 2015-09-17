#obj-m	:= led_drv.o
APP	= fb_mmap


#KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR	:= /work/android_kernel_2.6.32_sv210
KDIR	:= /work_dd/linux_kernel_2.6.32_sv210
#CC = arm-linux-gcc
CC = arm-none-linux-gnueabi-gcc

PWD	:= $(shell pwd)

#all : module app
all : app

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
#	$(CC) -static -o $(APP) $(APP).c
	$(CC) -o $(APP) $(APP).c

clean:
#	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	-rm -rf $(APP)
