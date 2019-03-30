$(shell ./fortrancompile.sh)

TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = helium
# helium.dbd will be created and installed
DBD += helium.dbd

# helium.dbd will be made up from these files:
helium_DBD += base.dbd
helium_DBD += ether_ip.dbd
helium_DBD += s7plc.dbd
helium_DBD += s7plcBase.dbd s7plcCalcout.dbd s7plcReg.dbd

#helium_DBD += stream.dbd
helium_DBD += asyn.dbd
helium_DBD += drvAsynIPPort.dbd
helium_DBD += calcSupport.dbd
helium_DBD += FortranCallSub.dbd


# Include dbd files from all support applications:
#helium_DBD += xxx.dbd

gfortran_DIR=/usr/lib/gcc/x86_64-linux-gnu/4.9/

# Add all the support libraries needed by this IOC
#helium_LIBS += xxx

# helium_registerRecordDeviceDriver.cpp derives from helium.dbd
helium_SRCS += helium_registerRecordDeviceDriver.cpp
helium_SRCS += FortranCallSub.cpp
helium_SRCS += Dnst6501.f
helium_SRCS += Dnst6505.f
helium_SRCS += TempK5301.f
helium_SRCS += DnstL5301.f
helium_SRCS += Dnst5301.f
helium_SRCS += Heprop.f

# Build the main IOC entry point on workstation OSs.
helium_SRCS_DEFAULT += heliumMain.cpp
helium_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#helium_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
#helium_LIBS += stream
helium_LIBS += asyn
helium_LIBS += calc
helium_LIBS += ether_ip
helium_LIBS += s7plc
helium_LIBS += gfortran
helium_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

