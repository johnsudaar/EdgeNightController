#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T17:05:50
#
#-------------------------------------------------

QT       += core gui
QT       += network

#LIBS+=-L. -L../release/build/i386 -Wl,-rpath /usr/local/lib -llumax -lftd2xx -lpthread -ldl

QMAKE_CXXFLAGS+=-fpermissive

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EdgeNightLaser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    displayableobject.cpp \
    screen.cpp \
    line.cpp \
    point.cpp \
    circle.cpp \
    rectangle.cpp \
    client.cpp \
    router.cpp \
    calibrer.cpp \
    parameters.cpp

HEADERS  += mainwindow.h \
    displayableobject.h \
    screen.h \
    line.h \
    point.h \
    circle.h \
    rectangle.h \
    const.h \
    client.h \
    router.h \
    calibrer.h \
    lumax.h \
    parameters.h

FORMS    += mainwindow.ui \
            calibrer.ui
