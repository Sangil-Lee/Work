obj-m	:= sk_drv.o
APP	= sk_app


KDIR	:= /lib/modules/$(shell uname -r)/build
#CC = arm-linux-gcc
#KDIR	:= /work/android_kernel_2.6.32_sv210
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
	-rm -rf $(APP)
