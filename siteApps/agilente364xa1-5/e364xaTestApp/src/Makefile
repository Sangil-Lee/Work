TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# build an ioc application

PROD_IOC = e364xaTest

DBD += e364xaTest.dbd
e364xaTest_DBD += base.dbd
e364xaTest_DBD += deve364xa.dbd
e364xaTest_DBD += asynRecord.dbd
e364xaTest_DBD += drvAsynIPPort.dbd
e364xaTest_DBD += drvAsynSerialPort.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
e364xaTest_SRCS += e364xaTest_registerRecordDeviceDriver.cpp
e364xaTest_SRCS_DEFAULT += e364xaTestMain.cpp
e364xaTest_SRCS_vxWorks += -nil-

#The following adds support from base/src/vxWorks
e364xaTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

#add a definition for each support application used by this application
e364xaTest_LIBS += deve364xa
e364xaTest_LIBS += asyn
e364xaTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

