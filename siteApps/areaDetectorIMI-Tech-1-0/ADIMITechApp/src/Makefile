TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
DBD += ADIMITechDriver.dbd

LIBRARY_IOC += ADIMITechEpicsSupport 
USR_CPPFLAGS += -std=c++11 -ggdb -Wno-format -Wno-reorder

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd

#ADIMITechEpicsSupport_HEADERS += NeptuneAPI.h
ADIMITechEpicsSupport_SRCS += ADIMITechDriver.cpp
ADIMITechEpicsSupport_LIBS += NeptuneAPI 
ADIMITechEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(ADCORE)/ADApp/commonDriverMakefile

#=============================
# Build the IOC application

PROD_IOC = ADIMITech
# ADIMITech.dbd will be created and installed
DBD += ADIMITech.dbd

USR_CXXFLAGS += -fPIC
#INC += NeptuneAPI.h

# ADIMITech.dbd will be made up from these files:
ADIMITech_DBD += base.dbd

# Include dbd files from all support applications:
#ADIMITech_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#ADIMITech_LIBS += xxx

# ADIMITech_registerRecordDeviceDriver.cpp derives from ADIMITech.dbd
ADIMITech_SRCS += ADIMITech_registerRecordDeviceDriver.cpp
#ADIMITech_SRCS += pvaDriverIMITech.cpp

# Build the main IOC entry point on workstation OSs.
ADIMITech_SRCS_DEFAULT += ADIMITechMain.cpp
ADIMITech_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ADIMITech_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ADIMITech_LIBS += NeptuneAPI 
ADIMITech_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(ADCORE)/ADApp/commonDriverMakefile

#g++ -I/usr/local/include/neptune -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF NeptuneSampleApp.d -MT NeptuneSampleAPP.o -o NeptuneSampleApp.o NeptuneSampleApp.cpp
#g++ -L/usr/local/lib -o NeptuneSampleApp NeptuneSampleApp.o -lNeptuneAPI

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

