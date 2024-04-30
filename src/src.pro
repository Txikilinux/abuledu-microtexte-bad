#-------------------------------------------------
#
# Project created by QtCreator 2012-10-07T13:27:11
#
#-------------------------------------------------

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

QT       += core gui script xml network

TARGET   = abuledu-microtexte
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
            version.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc

win32{
  OTHER_FILES += ../windows/windows.rc
  RC_FILE = ../windows/windows.rc
}

macx {
 ICON = ../macos/icones/abuledu-microtexte.icns
}

include(lib/abuleduapplicationv1/abuleduapplicationv1.pri)
include(lib/abuleduconstantesv1/abuleduconstantesv1.pri)
include(lib/abuledunetworkaccessmanagerv1/abuledunetworkaccessmanagerv1.pri)
include(lib/abuledumessageboxv1/abuledumessageboxv1.pri)
include(lib/abuleduidentitesv1/abuleduidentitesv1.pri)
include(lib/abuleduflatboutonv1/abuleduflatboutonv1.pri)
include(lib/maia/maia.pri)
include(lib/abuledufilev1/abuledufilev1.pri)
#include(futurelib/abuledutextev1/abuledutextev1.pri)
include(lib/abuledupicottsv1/abuledupicottsv1.pri)
include(lib/abuledusplashscreenv1/abuledusplashscreenv1.pri)
include(lib/abuledulangv1/abuledulangv1.pri)
include(lib/abuledumenufeuillev1/abuledumenufeuillev1.pri)
include(lib/abuledusingletonv1/abuledusingletonv1.pri)
include(lib/abuledupluginloaderv1/abuledupluginloaderv1.pri)
include(lib/abuleduaproposv1/abuleduaproposv1.pri)
include(lib/abuleduloggerv1/abuleduloggerv1.pri)
include(lib/abuleduandroidsupportv1/abuleduandroidsupportv1.pri)
include(lib/abuleduloadinganimationv1/abuleduloadinganimationv1.pri)
include(lib/abuledumultimediav1/abuledumultimediav1.pri)
include(lib/abuleduaudiov1/abuleduaudiov1.pri)
DEFINES += __ABULEDUPRODUCTIONRELEASE__MODE__=1

#Qt5 sur trusty ...
exists(/usr/include/qt5/QtMultimedia/QMediaPlayer) {
  INCLUDEPATH += /usr/include/qt5/QtMultimedia
}

