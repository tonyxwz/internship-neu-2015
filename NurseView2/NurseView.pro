#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T11:04:07
#
#-------------------------------------------------

QT       += core gui sql network script

TARGET = NurseView
TEMPLATE = app


SOURCES += main.cpp\
        nurseviewwindow.cpp \
    logindialog.cpp \
    sethostdialog.cpp \
    cJSON.c \
    patient.cpp

HEADERS  += nurseviewwindow.h \
    logindialog.h \
    sethostdialog.h \
    cJSON.h \
    patient.h

FORMS    += nurseviewwindow.ui \
    logindialog.ui \
    sethostdialog.ui

RESOURCES += \
    picture.qrc
