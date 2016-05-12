#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T07:56:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gravity
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datapoint.cpp \
    pointcollection.cpp \
    pointview.cpp

HEADERS  += mainwindow.h \
    datapoint.h \
    pointcollection.h \
    pointview.h

FORMS    += mainwindow.ui
