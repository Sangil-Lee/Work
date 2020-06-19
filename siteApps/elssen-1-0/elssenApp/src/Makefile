TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
DBD += serialAsynEpicsSupport.dbd

LIBRARY_IOC += serialAsynEpicsSupport 

USR_CPPFLAGS += -Wno-deprecated-declarations 
CXX_FLAGS += -std=gnu++11 -ggdb 

ifeq ($(WINDOWS_QT), YES)
USR_CPPFLAGS += -DWINDOWS_QT=1
USR_INCLUDES += -I$(WINDOWS_QT_DIR)/include
USR_INCLUDES += -I$(WINDOWS_QT_DIR)/include/QtSerialPort
USR_INCLUDES += -I$(WINDOWS_QT_DIR)/include/QtCore
Qt5SerialPort_DIR = $(WINDOWS_QT_DIR)/lib.dll
Qt5Core_DIR = $(WINDOWS_QT_DIR)/lib.dll
INC += serialAsynEpics.h
serialAsynEpicsSupport_SRCS += serialAsynEpics.cpp
serialAsynEpicsSupport_LIBS += asyn Qt5SerialPort Qt5Core
else
INC += SerialPortConstants.h  SerialPort.h    SerialStreamBuf.h    SerialStream.h 	serialAsynEpics.h
serialAsynEpicsSupport_SRCS += SerialPort.cpp  SerialStreamBuf.cpp  SerialStream.cpp serialAsynEpics.cpp
serialAsynEpicsSupport_LIBS += asyn 
endif

serialAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = elssen
# elssen.dbd will be created and installed
DBD += elssen.dbd

# elssen.dbd will be made up from these files:
elssen_DBD += base.dbd
elssen_DBD += serialAsynEpicsSupport.dbd

# Include dbd files from all support applications:
#elssen_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#elssen_LIBS += xxx

# elssen_registerRecordDeviceDriver.cpp derives from elssen.dbd
elssen_SRCS += elssen_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
elssen_SRCS_DEFAULT += elssenMain.cpp
elssen_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#elssen_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary
# Link QSRV (pvAccess Server) if available
ifdef EPICS_QSRV_MAJOR_VERSION
    elssen_LIBS += qsrv
    elssen_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    elssen_DBD += PVAServerRegister.dbd
    elssen_DBD += qsrv.dbd
endif

# Finally link to the EPICS Base libraries
elssen_LIBS += serialAsynEpicsSupport
ifeq ($(WINDOWS_QT), YES)
elssen_LIBS += asyn Qt5SerialPort Qt5Core
else
elssen_LIBS += asyn
endif
elssen_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

