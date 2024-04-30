#utilisation generalisee et factorisee de ccache pour toutes les applications abuledu
BASE_CXX = $$QMAKE_CXX
system(ccache -V):QMAKE_CXX = ccache $$BASE_CXX

android {
  DEFINES += QT_NO_PRINTER=1
  DEFINES += ABULEDU_SSOLOGIN_TABLET=1
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += core network xml script

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/activityfilter.h
SOURCES += $$PWD/activityfilter.cpp
RESOURCES += $$PWD/abuleduapplicationv1.qrc

###############################################################################
#               GESTION MODE PLUGIN (fake pour Qt Creator)
###############################################################################
pluginQtcreator {
  INCLUDEPATH += $$PWD/plugin-qtcreator
  DEPENDPATH += $$PWD/plugin-qtcreator

  SOURCES += $$PWD/plugin-qtcreator/abuleduapplicationv1.cpp
  HEADERS += $$PWD/plugin-qtcreator/abuleduapplicationv1.h
}
!pluginQtcreator {
  INCLUDEPATH += $$PWD/normal
  DEPENDPATH += $$PWD/normal

  SOURCES += $$PWD/normal/abuleduapplicationv1.cpp
  HEADERS += $$PWD/normal/abuleduapplicationv1.h
}

###############################################################################
#               GESTION MODE TABLETTE
#               example : qmake -config tablette
###############################################################################
tablette {
    DEFINES += __ABULEDUTABLETTEV1__MODE__=1
}

###############################################################################
#               GESTION MODE DEMO
#               example : qmake -config demo "DEMO_TIME=2"
###############################################################################
demo {
    message(MODE DEMO ACTIF)
    if($$DEMO_TIME){
        message(PAS DE TEMPS DEFINI (defaut = 5 min(s)))
        DEFINES += __ABULEDU_MODE_TIMED__=\\\"0\\\"
    }
    else{
        message(TEMPS DEFINI = $${DEMO_TIME} min(s))
        DEFINES += __ABULEDU_MODE_TIMED__=\\\"$${DEMO_TIME}\\\"
    }
}

###############################################################################
#               GESTION LOCATION OBJETS COMPILES (shadow/.build)
###############################################################################
unix {
  # nos appli ont un fichier src.pro ...
  exists($${OUT_PWD}/src.pro) {
     # quand on est pas en shadow build
     OBJECTS_DIR = $$PWD/../../.build/
     MOC_DIR     = $$PWD/../../.build/
     RCC_DIR     = $$PWD/../../.build/
     UI_DIR      = $$PWD/../../.build/
  } else {
     #on est en shadow build
     #message("On est en shadow build je ne specifie donc pas le .build comme repertoire de stockage des fichiers temp")
  }
}

!exists($$PWD/../../localdebug.pri) {
    message("Prise en compte debug.pri application")
    include($$PWD/debug.pri)
} else {
  message("Utilisation du localdebug.pri de l'application")
  include($$PWD/../../localdebug.pri)
}



###############################################################################
#               INCLUSION ET CONTROLE LIBRAIRIES
###############################################################################
include($$PWD/qtsingleapplication/src/qtsingleapplication.pri)

!exists($$PWD/../abuleduconstantesv1/abuleduconstantesv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuleduconstantesv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduConstantesV1")
}

!exists($$PWD/../abuledunetworkaccessmanagerv1/abuledunetworkaccessmanagerv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuledunetworkaccessmanagerv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduNetworkAccessManagerV1")
}

!exists($$PWD/../abuledumessageboxv1/abuledumessageboxv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuledumessageboxv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduMessageBoxV1")
}

!exists($$PWD/../abuleduidentitesv1/abuleduidentitesv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuleduidentitesv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduIdentitesV1")
}

!exists($$PWD/../abuledupluginloaderv1/abuledupluginloaderv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuledupluginloaderv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduPluginLoaderV1")
}

!exists($$PWD/../abuleduloggerv1/abuleduloggerv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuleduloggerv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduLoggerV1")
}
