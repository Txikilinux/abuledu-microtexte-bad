#-------------------------------------------------
#
# Project created by QtCreator 2012-04-19T12:09:57
#
#-------------------------------------------------
system(ccache -V):QMAKE_CXX = ccache g++

QT       += testlib

QT       += gui

TARGET = abuledufilev1-tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
           abuleduscolomfrv1tests.cpp \
           abuleduscolomfrv1testsexport.cpp \
           abuleduscolomfrv1testsimport.cpp \
           abuleduscolomfrv1testsvcard.cpp \
           abuledumimetypev1tests.cpp \
           abuledufilev1tests.cpp

HEADERS += abuleduscolomfrv1tests.h \
           abuleduscolomfrv1testsexport.h \
           abuleduscolomfrv1testsimport.h \
           abuleduscolomfrv1testsvcard.h \
           abuledumimetypev1tests.h \
           abuledufilev1tests.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(lib/abuleduapplicationv1/abuleduapplicationv1.pri)
include(lib/abuleduconstantesv1/abuleduconstantesv1.pri)
include(lib/abuledunetworkaccessmanagerv1/abuledunetworkaccessmanagerv1.pri)
include(lib/abuleduflatboutonv1/abuleduflatboutonv1.pri)
include(lib/maia/maia.pri)
include(lib/abuledufilev1/abuledufilev1.pri)
include(lib/abuledumessageboxv1/abuledumessageboxv1.pri)
include(lib/abuleduidentitesv1/abuleduidentitesv1.pri)
include(lib/abuledupluginloaderv1/abuledupluginloaderv1.pri)
include(lib/abuledusingletonv1/abuledusingletonv1.pri)

OTHER_FILES += dataTests/demo.xml \
    dataTests/demo-mini.xml \
    dataTests/demo-miniLOMFR.xml \
    dataTests/demo-miniSCOLOMFR.xml \
    dataTests/demo-miniABULEDU.xml
