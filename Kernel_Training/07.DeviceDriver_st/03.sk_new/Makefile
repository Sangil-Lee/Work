obj-m	:= sk_drv.o
APP	= sk_app


KDIR	:= /lib/modules/$(shell uname -r)/build
CC = gcc
#KDIR	:= /work_kernel/linux
#CC = arm-none-linux-gnueabi-gcc

PWD	:= $(shell pwd)

all : module app

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
	$(CC) -o $(APP) $(APP).c

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	-rm $(APP)
