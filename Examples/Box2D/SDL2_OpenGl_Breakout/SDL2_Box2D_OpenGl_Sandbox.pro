TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


win32{

    INCLUDEPATH += libs/SDL2/includes

    debug{

        LIBS += -L$$PWD/libs/SDL2/Debug/x64
    }

    release{

        LIBS += -L$$PWD/libs/SDL2/Release/x64
    }

}


LIBS += -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx -lGL -lGLEW -ldl -lSOIL


SOURCES += main.cpp \
    global.cpp \
    mainwindow.cpp \
    sdlapplication.cpp \
    sdleventdelegate.cpp \
    timer.cpp \
    shader.cpp \
    texture.cpp \
    resourcemanager.cpp \
    spriterenderer.cpp


DISTFILES += \
    README.md


HEADERS += \
    global.h \
    mainwindow.h \
    sdlapplication.h \
    sdleventdelegate.h \
    timer.h \
    shader.h \
    texture.h \
    resourcemanager.h \
    spriterenderer.h


copydata.commands = $(COPY_DIR) $$PWD/Resources $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
