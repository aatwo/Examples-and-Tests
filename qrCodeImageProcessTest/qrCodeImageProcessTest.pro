#-------------------------------------------------
#
# Project created by QtCreator 2018-01-17T17:51:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QrCodeImageProcessTest
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


INCLUDEPATH += opencv/include


SOURCES +=  main.cpp \
            mainwindow.cpp \
            libdecodeqr/bitstream.cpp \
            libdecodeqr/codedata.cpp \
            libdecodeqr/container.cpp \
            libdecodeqr/ecidecoder.cpp \
            libdecodeqr/formatinfo.cpp \
            libdecodeqr/galois.cpp \
            libdecodeqr/imagereader.cpp \
            libdecodeqr/libdecodeqr.cpp \
            imagelabel.cpp

HEADERS +=  mainwindow.h \
            libdecodeqr/bitstream.h \
            libdecodeqr/codedata.h \
            libdecodeqr/container.h \
            libdecodeqr/decodeqr.h \
            libdecodeqr/ecidecoder.h \
            libdecodeqr/formatinfo.h \
            libdecodeqr/galois.h \
            libdecodeqr/imagereader.h \
            libdecodeqr/qrerror.h \
            libdecodeqr/qrtypes.h \
            libdecodeqr/version.h \
            imagelabel.h

LIBS += -L$$PWD/opencv/x64/vc15/staticlib -lopencv_core340d \
        -L$$PWD/opencv/x64/vc15/staticlib -lippicvmt \
        -L$$PWD/opencv/x64/vc15/staticlib -lippiwd \
        -L$$PWD/opencv/x64/vc15/staticlib -littnotifyd \
        -L$$PWD/opencv/x64/vc15/staticlib -lopencv_imgproc340d \
        -L$$PWD/opencv/x64/vc15/staticlib -lzlibd \
        -lWs2_32 \
        -lUser32

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc
