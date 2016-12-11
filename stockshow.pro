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
    stockparser.cpp \
    dlgaddstock.cpp \
    dlgdetail.cpp

HEADERS  += dlgmain.h \
    stockparser.h \
    dlgaddstock.h \
    dlgdetail.h

FORMS    += dlgmain.ui \
    dlgaddstock.ui \
    dlgdetail.ui

RESOURCES += \
    stockshow.qrc
