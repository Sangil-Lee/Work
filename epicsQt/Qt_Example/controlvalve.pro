TEMPLATE = app
TARGET = ControlValve
QT += widgets
CONFIG += c++11

INCLUDEPATH += /home/ctrluser/epicsQt/qeframework/include
INCLUDEPATH += /home/ctrluser/Qt5.12.8/5.12.8/gcc_64/include/QtXml
LIBS += -L/home/ctrluser/epicsQt/qeframework/lib/linux-x86_64 -lQEFramework
#SOURCES += main.cpp
#SOURCES += pump.cpp
SOURCES += widget.cpp
