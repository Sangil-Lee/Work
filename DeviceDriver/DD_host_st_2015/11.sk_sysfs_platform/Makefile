obj-m	:= sk_drv.o
APP	= sk_app


KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR	:= /work/linux		#NOK
#CC = arm-linux-gcc

PWD	:= $(shell pwd)

all : module app

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
	$(CC) -o $(APP) $(APP).c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	-rm *.symvers
	rm $(APP)
