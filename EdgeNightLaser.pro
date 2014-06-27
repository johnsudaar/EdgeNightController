#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T17:05:50
#
#-------------------------------------------------

QT       += core gui
QT       += network

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
    calibrer.cpp

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
    calibrer.h

FORMS    += mainwindow.ui \
            calibrer.ui
