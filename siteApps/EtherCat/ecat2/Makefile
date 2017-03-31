include /ioc/tools/driver.makefile
PROJECT=ecat2
BUILDCLASSES += Linux
EXCLUDE_VERSIONS = 3.13 3.14.8


ARCH_FILTER = eldk52-e500v2 SL6%

USR_CFLAGS += -fPIC
USR_LDFLAGS += -L /ioc/modules/ecat2/ifc/lib/
USR_LDFLAGS += -lethercat

#---------- e500v2
USR_LDFLAGS_eldk52-e500v2 += -Wl,-rpath=/ioc/modules/ecat2/master/3.6.11.5-rt37/lib  
USR_LDFLAGS_eldk52-e500v2 +=         -L /ioc/modules/ecat2/master/3.6.11.5-rt37/lib  

#---------- SL6
USR_LDFLAGS_SL6-x86_64 += -Wl,-rpath=/ioc/modules/ecat2/master/2.6.32-573.3.1.el6.x86_64/lib
USR_LDFLAGS_SL6-x86_64 +=         -L /ioc/modules/ecat2/master/2.6.32-573.3.1.el6.x86_64/lib
  
USR_LDFLAGS_SL6-x86   += -Wl,-rpath=/ioc/modules/ecat2/master/2.6.32-573.3.1.el6.i686/lib  
USR_LDFLAGS_SL6-x86   +=         -L /ioc/modules/ecat2/master/2.6.32-573.3.1.el6.i686/lib  

# verbose compiler, verbose linker output
#USR_CFLAGS += -v -Wl,-v

