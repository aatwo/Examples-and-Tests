#-------------------------------------------------
#
# Project created by QtCreator 2018-01-18T11:50:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QrBarcodeImageProcessTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#The following define is required to allow the INTMAX_MAX macro to be defined in stdint.h.
DEFINES += __STDC_LIMIT_MACROS


include( qzxing/QZXing.pri )


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    imagelabel.cpp

HEADERS += \
        mainwindow.h \
    imagelabel.h

FORMS += \
        mainwindow.ui


#LIBS += -L$$PWD/opencv/x64/vc15/staticlib -lopencv_core340d \
#        -L$$PWD/opencv/x64/vc15/staticlib -lippicvmt \
#        -L$$PWD/opencv/x64/vc15/staticlib -lippiwd \
#        -L$$PWD/opencv/x64/vc15/staticlib -littnotifyd \
#        -L$$PWD/opencv/x64/vc15/staticlib -lopencv_imgproc340d \
#        -L$$PWD/opencv/x64/vc15/staticlib -lzlibd \
#        -lWs2_32 \
#        -lUser32

RESOURCES += \
    resources.qrc