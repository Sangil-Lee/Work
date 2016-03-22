obj-m 	:= hello_param.o

KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR	:= /work_kernel/linux

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
