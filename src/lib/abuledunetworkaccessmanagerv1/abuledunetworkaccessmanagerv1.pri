QT += core network gui

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/abuledunetworkaccessmanagerv1.h \
           $$PWD/abuleduproxypassworddialog.h \
           $$PWD/abuledunetworkthreadv1.h \
           $$PWD/abuledussologinv1.h \
           $$PWD/abuledulocalsocketipcclientv1.h \
           $$PWD/abuledulocalsocketipcserverv1.h \
           $$PWD/simplecrypt.h \
           $$PWD/abuledudownloadprogressbar.h \
           $$PWD/abuledunetworkproxysettings.h \
           $$PWD/abuledunetworktests.h

SOURCES += $$PWD/abuledunetworkaccessmanagerv1.cpp \
           $$PWD/abuleduproxypassworddialog.cpp \
           $$PWD/abuledunetworkthreadv1.cpp \
           $$PWD/abuledussologinv1.cpp \
           $$PWD/abuledulocalsocketipcclientv1.cpp \
           $$PWD/abuledulocalsocketipcserverv1.cpp \
           $$PWD/simplecrypt.cpp \
           $$PWD/abuledudownloadprogressbar.cpp \
           $$PWD/abuledunetworkproxysettings.cpp \
           $$PWD/abuledunetworktests.cpp

FORMS += $$PWD/abuleduproxypassworddialog.ui \
         $$PWD/abuledussologinv1.ui \
         $$PWD/abuledudownloadprogressbar.ui \
         $$PWD/abuledunetworkproxysettings.ui

RESOURCES += $$PWD/abuledunetworkaccessmanagerv1.qrc \
             $$PWD/abuleduproxypassworddialog.qrc \
             $$PWD/abuledussologinv1.qrc

win32 {

}
macx {

}
linux-g++|linux-g++-32|linux-g++-64 {

}

!exists($$PWD/../abuleduapplicationv1/abuleduapplicationv1.pri ) {
  error("la lib abuledunetworkaccessmanagerv1 dépend de abuleduapplicationv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1")
}

!exists($$PWD/../abuleduflatboutonv1/abuleduflatboutonv1.h ) {
  error("la lib abuledunetworkaccessmanagerv1 dépend de abuleduflatboutonv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduFlatBoutonV1")
}

!exists($$PWD/../maia/maia.pri ) {
  error("la lib abuledunetworkaccessmanagerv1 dépend de maia: https://redmine.ryxeo.com/projects/ryxeo/wiki/maia")
}

!exists($$PWD/../abuleduidentitesv1/abuleduidentitesv1.h ) {
  error("la lib abuledunetworkaccessmanagerv1 dépend de abuleduidentitesv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduIdentitesV1")
}

!exists($$PWD/../abuleduloggerv1/abuleduloggerv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuleduloggerv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduLoggerV1")
}
