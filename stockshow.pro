#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T09:45:34
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stockshow
TEMPLATE = app


SOURCES += main.cpp\
        dlgmain.cpp \
    stockparser.cpp

HEADERS  += dlgmain.h \
    stockparser.h

FORMS    += dlgmain.ui

RESOURCES += \
    stockshow.qrc
