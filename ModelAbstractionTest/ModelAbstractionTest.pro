#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T16:06:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModelAbstractionTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ccustomtreeview.cpp \
    icustommodel.cpp \
    ccustommodel.cpp \
    ccustomdatatype.cpp

HEADERS  += mainwindow.h \
    ccustomtreeview.h \
    icustommodel.h \
    ccustommodel.h \
    ccustomdatatype.h

FORMS    += mainwindow.ui
