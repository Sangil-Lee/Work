TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

#CVI=F:/epics/7.0.3/siteApps/Ctrl_IOC/EPICS-NetShrVar/CVI
#USR_INCLUDES += -I$(CVI)/include

CXXFLAGS += -std=gnu++11
CXXFLAGS += -D_NI_mswin64_

ifeq ($(STATIC_BUILD),NO)
#USR_LDFLAGS_WIN32 += /NODEFAULTLIB:LIBCMT.LIB /NODEFAULTLIB:LIBCMTD.LIB
#USR_LDFLAGS_WIN32 += -NODEFAULTLIB libcmt libcmtd
LFLAGS += /NODEFAULTLIB:LIBCMT.LIB /NODEFAULTLIB:LIBCMTD.LIB
endif

PROD_IOC += CheckNetVar
LIBRARY_IOC += NetShrVar

ifeq (WIN32,$(OS_CLASS))
NICVIDIR=$(subst /,\,$(ICPBINARYDIR)/CVI2013)
else
NICVIDIR=F:/epics/7.0.3/siteApps/Ctrl_IOC/EPICS-NetShrVar/CVI/include
endif

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install NetShrVar.dbd into <top>/dbd
DBD += NetShrVar.dbd

ifeq (WIN32,$(OS_CLASS))
ifneq ($(findstring windows,$(EPICS_HOST_ARCH)),)
CVILIB = $(CVI)/extlib/msvc64
else
CVILIB = $(CVI)/extlib/msvc
endif
else
CVILIB = /usr/local/lib
endif

# specify all source files to be compiled and added to the library
NetShrVar_SRCS += convertToString.cpp cnvconvert.cpp NetShrVarDriver.cpp NetShrVarInterface.cpp pugixml.cpp
NetShrVar_LIBS += asyn cvinetv cvisupp cvirt
NetShrVar_LIBS += $(EPICS_BASE_IOC_LIBS)

#NetShrVar_SYS_LIBS_WIN32 += $(CVILIB)/cvinetv $(CVILIB)/cvisupp $(CVILIB)/cvirt 
NetShrVar_SYS_LIBS_WIN32 += user32

NetShrVar_SYS_LIBS_Linux += ninetv

CheckNetVar_SRCS += CheckNetVar.cpp
CheckNetVar_LIBS += NetShrVar $(NetShrVar_LIBS) 
CheckNetVar_SYS_LIBS_WIN32 += $(NetShrVar_SYS_LIBS_WIN32)
#CheckNetVar_SYS_LIBS_Linux += $(NetShrVar_SYS_LIBS_Linux)


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

# we ned to make a copy due to spaces in the absolute path
../cnvconvert.cpp : CVI
CVI :
	-mkdir CVI
ifeq (WIN32,$(OS_CLASS))
	xcopy /D /I /E /Q /Y /R $(NICVIDIR)\include $@\include
	xcopy /D /I /E /Q /Y /R $(NICVIDIR)\extlib $@\extlib
endif
ifeq (Linux,$(OS_CLASS))
	cp -r $(NICVIDIR)/include $@
#	cp -r $(NICVIDIR)/extlib $@
endif

