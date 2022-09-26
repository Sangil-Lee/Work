TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
DBD += devlongPXIe.dbd
LIBRARY_IOC += nidaqDevSupport
nidaqDevSupport_SRCS += devlongPXIe.c
nidaqDevSupport_LIBS += nidaqmx
nidaqDevSupport_LIBS += $(EPICS_BASE_IOC_LIBS)
#=============================
# Build the IOC application

PROD_IOC = pxieioc
# pxieioc.dbd will be created and installed
DBD += pxieioc.dbd

# pxieioc.dbd will be made up from these files:
pxieioc_DBD += base.dbd
pxieioc_DBD += devlongPXIe.dbd

# Include dbd files from all support applications:
#pxieioc_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#pxieioc_LIBS += xxx

# pxieioc_registerRecordDeviceDriver.cpp derives from pxieioc.dbd
pxieioc_SRCS += pxieioc_registerRecordDeviceDriver.cpp
#pxieioc_SRCS += devaoPXIe.c
#pxieioc_SRCS += devlongPXIe.c
# pxieioc_SRCS += nidaqDriver.cpp

# Build the main IOC entry point on workstation OSs.
pxieioc_SRCS_DEFAULT += pxieiocMain.cpp
pxieioc_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#pxieioc_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
#pxieioc_LIBS += asyn nidaqmx
pxieioc_LIBS += nidaqDevSupport
pxieioc_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

