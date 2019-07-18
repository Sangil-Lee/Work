TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
DBD += lvfpgaAsynEpics.dbd

LIBRARY_IOC += lvfpgaAsynEpicsSupport

USR_CPPFLAGS += -fno-strict-aliasing -std=c++11 -ggdb

lvfpga_DIR = /usr/local/natinst/lib

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

#nidaqAsynEpicsSupport_SRCS += niacq_IntClk.cpp nidaqAsynEpics.cpp nidaqDriver.cpp
lvfpgaAsynEpicsSupport_SRCS += lvfpgaAsynEpics.cpp lvfpgaDriver.cpp NiFpga.cpp
lvfpgaAsynEpicsSupport_LIBS += asyn
lvfpgaAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = lvfpga
# lvfpga.dbd will be created and installed
DBD += lvfpga.dbd

# lvfpga.dbd will be made up from these files:
lvfpga_DBD += base.dbd lvfpgaAsynEpics.dbd timestampRecord.dbd

# Include dbd files from all support applications:
#lvfpga_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#lvfpga_LIBS += xxx

# lvfpga_registerRecordDeviceDriver.cpp derives from lvfpga.dbd
lvfpga_SRCS += lvfpga_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
lvfpga_SRCS_DEFAULT += lvfpgaMain.cpp
lvfpga_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#lvfpga_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
lvfpga_LIBS += lvfpgaAsynEpicsSupport
lvfpga_LIBS += asyn timeStamp
lvfpga_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

