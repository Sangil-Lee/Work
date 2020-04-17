#-------------------------------------------------
#
# Project created by QtCreator 2020-03-27T12:37:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtNeptuneSample
TEMPLATE = app

INCLUDEPATH += /usr/local/include/neptune/
LIBS += -L/usr/local/lib/ -lNeptuneAPI


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
