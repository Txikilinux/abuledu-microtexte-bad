QT += core network gui

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/abuleduidentitesuiv1.h \
           $$PWD/abuleduidentitesv1.h \
           $$PWD/abuleduidentitesnetworkv1.h

SOURCES += $$PWD/abuleduidentitesuiv1.cpp \
           $$PWD/abuleduidentitesv1.cpp \
           $$PWD/abuleduidentitesnetworkv1.cpp

FORMS += $$PWD/abuleduidentitesuiv1.ui

RESOURCES += $$PWD/abuleduidentitesuiv1.qrc

win32 {

}
macx {

}
linux-g++|linux-g++-32|linux-g++-64 {

}

!exists($$PWD/../maia/maia.pri) {
  error("la lib AbulEduIdentitesUIV1 dépend de maia: https://redmine.ryxeo.com/projects/ryxeo/wiki/XMLRPC_avec_Maia")
}

!exists($$PWD/../abuleduflatboutonv1/abuleduflatboutonv1.pri ) {
  error("la lib AbulEduIdentitesUIV1 dépend de abuleduflatboutonv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduFlatBoutonV1")
}

!exists($$PWD/../abuledufilev1/abuledufilev1.pri ) {
  error("la lib AbulEduIdentitesUIV1 dépend de abuledufilev1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduFileV1")
}
