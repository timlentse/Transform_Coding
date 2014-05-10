#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T16:40:07
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = tranform-coding
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tranformcoding.cpp

INCLUDEPATH += /usr/include/opencv2

LIBS += -L/usr/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui

HEADERS += \
    tranformcoding.h
