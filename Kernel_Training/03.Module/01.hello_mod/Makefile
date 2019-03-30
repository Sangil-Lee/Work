obj-m	:= hello_mod.o

KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR	:= /work_kernel/linux
PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
#$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
#or
	$(MAKE) -C $(KDIR) M=$(PWD) clean

#	Don't do that follow:
#	rm -rf *.ko
#	rm -rf *.mod.*
#	rm -rf .*.cmd
#	rm -rf *.o
#	rm -rf *.symvers
#	rm -rf *.order
