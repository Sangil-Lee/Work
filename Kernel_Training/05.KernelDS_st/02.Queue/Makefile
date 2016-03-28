obj-m	:= kfifoTest.o


#KDIR	:= /lib/modules/$(shell uname -r)/build
KDIR	:= /work_kernel/linux

PWD	:= $(shell pwd)

all : module

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
