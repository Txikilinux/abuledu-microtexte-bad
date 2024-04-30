#-------------------------------------------------
#
# Project created by QtCreator 2012-04-18T10:11:16
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_vcardtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_vcardtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../vcard.pri)

HEADERS += \
    errorhandler.h \
    debug.h
