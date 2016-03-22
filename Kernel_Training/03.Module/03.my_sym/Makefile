obj-m	:= my_sym1_module.o my_sym2_module.o

KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR	:= /work_kernel/linux
PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
