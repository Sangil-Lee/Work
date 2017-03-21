TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = rfq
# rfq.dbd will be created and installed
DBD += rfq.dbd

# rfq.dbd will be made up from these files:
rfq_DBD += base.dbd

# Include dbd files from all support applications:
rfq_DBD += devSorensen.dbd
rfq_DBD += devLakeShore.dbd
rfq_DBD += drvAsynIPPort.dbd
rfq_DBD += ether_ip.dbd
# rfq_DBD += seqRfq.dbd

# Add all the support libraries needed by this IOC
#rfq_LIBS += xxx

# rfq_registerRecordDeviceDriver.cpp derives from rfq.dbd
rfq_SRCS += rfq_registerRecordDeviceDriver.cpp
# rfq_SRCS += seqRfq.st

# Build the main IOC entry point on workstation OSs.
rfq_SRCS_DEFAULT += rfqMain.cpp
rfq_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#rfq_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
rfq_LIBS += asyn
rfq_LIBS += devSorensen
rfq_LIBS += devLakeShore
rfq_LIBS += ether_ip
#rfq_LIBS += seq pv
rfq_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

