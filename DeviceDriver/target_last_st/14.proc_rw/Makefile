obj-m	:= my_proc.o

#KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR	:= /work/android_kernel_2.6.32_sv210
KDIR	:= /work/linux_kernel_2.6.32_sv210
#CC = arm-linux-gcc
CC = arm-none-linux-gnueabi-gcc

PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
