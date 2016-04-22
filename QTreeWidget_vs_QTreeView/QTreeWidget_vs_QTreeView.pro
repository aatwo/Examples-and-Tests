#-------------------------------------------------
#
# Project created by QtCreator 2016-04-19T11:02:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTreeWidget_vs_QTreeView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ccustommodel.cpp \
    ccustommodelitem.cpp \
    crandomstringmanager.cpp \
    cfiltergroup.cpp \
    cfilteritem.cpp \
    ccustomsortfilterproxymodel.cpp \
    ccommon.cpp

HEADERS  += mainwindow.h \
    ccustommodel.h \
    ccustommodelitem.h \
    crandomstringmanager.h \
    cfiltergroup.h \
    cfilteritem.h \
    ccustomsortfilterproxymodel.h \
    ccommon.h

FORMS    += mainwindow.ui
