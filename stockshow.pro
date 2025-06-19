#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T09:45:34
#
#-------------------------------------------------

QT       += core gui network

# greaterThan(QT_MAJOR_VERSION,5): QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stockshow
TEMPLATE = app


SOURCES += main.cpp\
        dlgmain.cpp \
    encode.cpp \
    stockparser.cpp \
    dlgaddstock.cpp \
    dlgdetail.cpp

HEADERS  += dlgmain.h \
    encode.h \
    stockparser.h \
    dlgaddstock.h \
    dlgdetail.h \
    stockurls.h

FORMS    += dlgmain.ui \
    dlgaddstock.ui \
    dlgdetail.ui

RESOURCES += \
    stockshow.qrc
