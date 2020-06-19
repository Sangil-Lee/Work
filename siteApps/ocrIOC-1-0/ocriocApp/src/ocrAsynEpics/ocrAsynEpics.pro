TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ocrasynepics.cpp

unix:!macx: LIBS += -L/home/ctrluser/epics/7.0.3/base/lib/linux-x86_64/ -lca

INCLUDEPATH += /home/ctrluser/epics/7.0.3/base/include
INCLUDEPATH += /home/ctrluser/epics/7.0.3/epicsLibs/synApps/support/asyn/include
DEPENDPATH +=  /home/ctrluser/epics/7.0.3/base/include
DEPENDPATH +=  /home/ctrluser/epics/7.0.3/epicsLibs/synApps/support/asyn/include

unix:!macx: PRE_TARGETDEPS += /home/ctrluser/epics/7.0.3/base/lib/linux-x86_64/libca.a

unix:!macx: LIBS += -L/home/ctrluser/epics/7.0.3/epicsLibs/synApps/support/asyn/lib/linux-x86_64/ -lasyn

unix:!macx: PRE_TARGETDEPS += /home/ctrluser/epics/7.0.3/epicsLibs/synApps/support/asyn/lib/linux-x86_64/libasyn.a

HEADERS += \
    ocrasynepics.h
