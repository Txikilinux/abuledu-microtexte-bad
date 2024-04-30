#-------------------------------------------------
#
# Project created by QtCreator 2014-01-29T15:46:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wav2OggExample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

include(../abuleduaudiov1.pri)
