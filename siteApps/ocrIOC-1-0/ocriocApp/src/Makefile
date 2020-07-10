TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

DBD += ocrAsynEpics.dbd

LIBRARY_IOC += ocrAsynEpicsSupport 

#USR_CPPFLAGS += -std=c++11 -ggdb -D_POSIX_C_SOURCE=200809L
DEFINES       = -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_CORE_LIB
USR_CPPFLAGS += -std=c++11 -Wno-stringop-truncation -Wno-stringop-overflow -Wno-unused-variable -Wno-deprecated-declarations

USR_DBDFLAGS  += -I$(RAON_SITELIBS)/dbd
timeStamp_DIR += $(RAON_SITELIBS)/lib/$(T_A)

#CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
#CXXFLAGS      = -pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC $(DEFINES)

USR_DIR = /usr/local/lib
LINUX_SYS_DIR = /lib/x86_64-linux-gnu
QT_LIB_DIR = /home/ctrluser/Qt5.12.2/5.12.2/gcc_64/lib
INCPATH = -I. -I. -isystem /usr/local/include -I. -isystem /usr/local/include/opencv4 -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include/QtWidgets -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include/QtGui -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include/QtCore -I. -isystem /usr/include/libdrm -I. -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/mkspecs/linux-g++
USR_INCLUDES = -I. -I. -isystem /usr/local/include -I. -isystem /usr/local/include/opencv4 -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include/QtWidgets -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include/QtGui -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/include/QtCore -I. -isystem /usr/include/libdrm -I. -I/home/ctrluser/Qt5.12.2/5.12.2/gcc_64/mkspecs/linux-g++

tesseract_DIR = $(USR_DIR)
leptonica_DIR = $(USR_DIR)
z_DIR = $(LINUX_SYS_DIR)
png_DIR = $(LINUX_SYS_DIR)
jpeg_DIR = $(LINUX_SYS_DIR)
tiff_DIR = $(LINUX_SYS_DIR)
gif_DIR = $(LINUX_SYS_DIR)
opencv_features2d_DIR=$(USR_DIR)
opencv_ml_DIR=$(USR_DIR)
opencv_flann_DIR=$(USR_DIR)
opencv_objdetect_DIR=$(USR_DIR)
opencv_gapi_DIR=$(USR_DIR)
opencv_photo_DIR=$(USR_DIR)
opencv_calib3d_DIR=$(USR_DIR)
opencv_highgui_DIR=$(USR_DIR)
opencv_stitching_DIR=$(USR_DIR)
opencv_core_DIR=$(USR_DIR)
opencv_imgcodecs_DIR=$(USR_DIR)
opencv_videoio_DIR=$(USR_DIR)
opencv_dnn_DIR=$(USR_DIR)
opencv_imgproc_DIR=$(USR_DIR)
opencv_video_DIR=$(USR_DIR)
Qt5Core_DIR = $(QT_LIB_DIR)
Qt5Widgets_DIR = $(QT_LIB_DIR)

INC += ocrAsynEpics.h
ocrAsynEpicsSupport_SRCS += ocrAsynEpics.cpp
ocrAsynEpicsSupport_LIBS += asyn 
#ocrAsynEpicsSupport_LIBS += tesseract leptonica z png jpeg tiff gif
ocrAsynEpicsSupport_LIBS += tesseract z png jpeg tiff gif
ocrAsynEpicsSupport_LIBS += opencv_features2d opencv_ml opencv_flann opencv_objdetect opencv_gapi opencv_photo opencv_calib3d opencv_highgui opencv_stitching opencv_core opencv_imgcodecs opencv_videoio opencv_dnn opencv_imgproc opencv_video 
ocrAsynEpicsSupport_LIBS += Qt5Core Qt5Widgets
ocrAsynEpicsSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = ocrioc
# ocrioc.dbd will be created and installed
DBD += ocrioc.dbd

# ocrioc.dbd will be made up from these files:
ocrioc_DBD += base.dbd
ocrioc_DBD += ocrAsynEpics.dbd

# Include dbd files from all support applications:
#ocrioc_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#ocrioc_LIBS += xxx

# ocrioc_registerRecordDeviceDriver.cpp derives from ocrioc.dbd
ocrioc_SRCS += ocrioc_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ocrioc_SRCS_DEFAULT += ocriocMain.cpp
ocrioc_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ocrioc_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
ocrioc_LIBS += ocrAsynEpicsSupport
ocrioc_LIBS += asyn
ocrioc_LIBS += leptonica
ocrioc_LIBS += tesseract z png jpeg tiff gif
ocrioc_LIBS += opencv_features2d opencv_ml opencv_flann opencv_objdetect opencv_gapi opencv_photo opencv_calib3d opencv_highgui opencv_stitching opencv_core opencv_imgcodecs opencv_videoio opencv_dnn opencv_imgproc opencv_video 
ocrioc_LIBS += Qt5Core Qt5Widgets
ocrioc_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

