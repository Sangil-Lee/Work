QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


unix:!macx: LIBS += -L/usr/local/lib/ -ltesseract

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv4
DEPENDPATH += /usr/local/include

unix:!macx: LIBS += -L/usr/local/lib/ -lleptonica -lz -lpng -ljpeg -ltiff -lgif

unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libleptonica.a

unix:!macx: LIBS += -L/usr/local/lib/ -lleptonica -lz -lpng -ljpeg -ltiff -lgif -lopencv_features2d -lopencv_imgproc -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_flann -lopencv_ml -lopencv_flann -lopencv_objdetect -lopencv_flann -lopencv_objdetect -lopencv_gapi -lopencv_objdetect -lopencv_gapi -lopencv_photo -lopencv_calib3d -lopencv_gapi -lopencv_photo -lopencv_calib3d -lopencv_highgui -lopencv_photo -lopencv_calib3d -lopencv_highgui -lopencv_stitching -lopencv_core -lopencv_highgui -lopencv_stitching -lopencv_core -lopencv_imgcodecs -lopencv_stitching -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_dnn -lopencv_imgcodecs -lopencv_videoio -lopencv_dnn -lopencv_imgproc -lopencv_videoio -lopencv_dnn -lopencv_imgproc -lopencv_video
