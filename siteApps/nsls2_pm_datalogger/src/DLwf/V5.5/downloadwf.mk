EROOT = /usr/lib/epics
TOP = $(EROOT)
SHARED_LIBRARIES = NO

HOST=$(shell hostname)
SHOST = $(findstring box64, $(HOST))
CC = /usr/bin/gcc
LD = /usr/bin/g++

ifeq ($(SHOST), box64)
	OBJDIR = box64
        LIBDIR =  -L/home/yntang/sys.programs/lib -L$(EROOT)/lib/linux-x86_64
	LIBS=	-luif_64 -lXm -lXt -lX11 -lm
        CCF = -DBOX64 -D_POSIX_C_SOURCE=199506L -D_POSIX_THREADS -D_XOPEN_SOURCE=500\
                -D_X86_ -DUNIX -D_BSD_SOURCE -Dlinux -D_REENTRANT -O3 -g -Wall
        LDF = -L$(EROOT)/lib/linux-x86_64 -L/home/yntang/sys.programs/lib
        INCDIR = -I. -I../Common -I/usr/include -I$(EROOT)/include -I$(EROOT)/include/os/Linux \
                -I/usr/include/x86_64-linux-gnu -I/home/yntang/sys.programs/Include
else
	OBJDIR = box32
        LIBDIR =  -L/home/yntang/sys.programs/lib -L$(EROOT)/lib/linux-x86
	LIBS=	-luif -lXm -lXt -lX11 -lm
        CCF = -DBOX32 -D_POSIX_C_SOURCE=199506L -D_POSIX_THREADS -D_XOPEN_SOURCE=500\
        -D_X86_ -DUNIX -D_BSD_SOURCE -Dlinux -D_REENTRANT -O3 -g -Wall -m32
        LDF = -L$(EROOT)/lib/linux-x86 -L/home/yntang/sys.programs/lib
        INCDIR = -I. -I../Common -I$(EROOT)/include -I$(EROOT)/include/os/Linux \
		-I/home/yntang/sys.programs/Include
endif

LDLIBS = $(LIBS) -lutil -lca -lCom -lpthread  -lreadline -lm -lrt -ldl -lgcc -lgfortran

CFLAGS =  -I/usr/include/X11R6 -I$(INCDIR)

SRC = downloadwf.c \
	ddinit.c \
	dddata.c \
	ddca.c \
	ddutil.c

OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

ifeq ($(SHOST), box64)
all: downloadwf_v5.5
else
all: downloadwf_32
endif

downloadwf_v5.5: $(OBJ) /home/yntang/sys.programs/lib/libuif_64.a downloadwf.mk
	@echo $(OBJ)
	$(LD) $(LDF) -o downloadwf_v5.5 $(OBJ) $(LDLIBS)

downloadwf_32: $(OBJ) /home/yntang/sys.programs/lib/libuif.a downloadwf.mk
	@echo $(OBJ)
	$(LD) $(LDF) -o downloadwf_32 $(OBJ) $(LDLIBS)

ins: downloadwf_v5.5
	cp downloadwf_v5.5 /WFdata/bin

$(OBJ): downloadwf.h
$(OBJDIR)/%.o: %.c
	$(CC) $(CCF) $(INCDIR) -c $< -o $@
