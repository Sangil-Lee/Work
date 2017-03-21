obj-m	:= sk_drv.o
APP	= sk_app


KDIR	:= /lib/modules/$(shell uname -r)/build
KBUILD_CFLAGS += -DHOST
#CC = gcc -DHOST
#KDIR	:= /work_kernel/linux
#CC = arm-none-linux-gnueabi-gcc

PWD	:= $(shell pwd)

all : module app task1 task2

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
	$(CC) -o $(APP) $(APP).c

task1:
	$(CC) -o $@ task1.c

task2:
	$(CC) -o $@ task2.c

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -rf $(APP)
	rm task1 task2
