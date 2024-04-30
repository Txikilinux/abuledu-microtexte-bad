!contains(DEFINES, .*__ABULEDU_MULTIMEDIA_AVAILABLE__.*) {
  DEFINES += __ABULEDU_MULTIMEDIA_AVAILABLE__=1
}

#pour l'instant (qt5.1) pas de support multimedia pour android
#!android {
 QT += multimedia multimediawidgets
#}


#####################################################################
#QT5 pour ubuntu/utopic ... ils ont un peu tout decoupe en morceau ...
#trusty
exists(/usr/include/qt5/QtMultimediaWidgets) {
  INCLUDEPATH += /usr/include/qt5/QtMultimediaWidgets
  LIBS     += -lQt5MultimediaWidgets
}
#trusty
exists(/usr/include/qt5/QtMultimedia) {
  INCLUDEPATH += /usr/include/qt5/QtMultimedia
  LIBS     += -lQt5Multimedia
}

#################################### i386  #################################
exists(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri) {
  include(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri)
  #specific utopic
  exists(/usr/include/i386-linux-gnu/qt5/QtMultimedia) {
    INCLUDEPATH += /usr/include/i386-linux-gnu/qt5/QtMultimedia
    LIBS     += -lQt5Multimedia
  }
}
exists(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri) {
  include(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri)
  #specific utopic
  exists(/usr/include/i386-linux-gnu/qt5/QtMultimediaWidgets) {
    INCLUDEPATH += /usr/include/i386-linux-gnu/qt5/QtMultimediaWidgets
    LIBS     += -lQt5MultimediaWidgets
  }
}

#################################### AMD 64  #################################
exists(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri) {
  include(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri)
  #specific utopic
  exists(/usr/include/x86_64-linux-gnu/qt5/QtMultimedia) {
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtMultimedia
    LIBS     += -lQt5Multimedia
  }
}

exists(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri) {
  include(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri)
  #specific utopic
  exists(/usr/include/x86_64-linux-gnu/qt5/QtMultimediaWidgets) {
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtMultimediaWidgets
    LIBS     += -lQt5MultimediaWidgets
  }
}

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS += \
        $$PWD/abuledumultimediav1.h\
        $$PWD/abuledumediaviewerinterface.h \
        $$PWD/abuleduslidesviewerv1.h \
        $$PWD/abuleducontrolaudiov1.h \
        $$PWD/abuleduvideoviewerv1.h\
        $$PWD/abuledusoundplayerv1.h \
        $$PWD/abuleduanimatedgifviewerv1.h \
        $$PWD/abuledumediabuttonv1.h \
        $$PWD/abuledusoundrecorderv1.h \
        $$PWD/abuleduaudiolevelwidgetv1.h \
        $$PWD/abuledumediagraphicsbuttonv1.h \
        $$PWD/abuledupictureplayerv1.h \
        $$PWD/abuleduaudiodecoderv1.h \
        $$PWD/abuledusounddevices.h \
        $$PWD/abuleduslidesrecorderv1.h \
        $$PWD/abuleduaudioplayerv1.h \
        $$PWD/abuledumultimediawidgetcontainer.h \
        $$PWD/abuledumultimediasettingsv1.h \
        $$PWD/abuledumultimediadevicechooserv1.h \
        $$PWD/abuledumediamedias.h

SOURCES += \
        $$PWD/abuledumultimediav1.cpp\
        $$PWD/abuleducontrolaudiov1.cpp \
        $$PWD/abuleduslidesviewerv1.cpp \
        $$PWD/abuleduvideoviewerv1.cpp \
        $$PWD/abuledusoundplayerv1.cpp \
        $$PWD/abuleduanimatedgifviewerv1.cpp \
        $$PWD/abuledumediabuttonv1.cpp \
        $$PWD/abuledusoundrecorderv1.cpp \
        $$PWD/abuleduaudiolevelwidgetv1.cpp \
        $$PWD/abuledumediagraphicsbuttonv1.cpp \
        $$PWD/abuledupictureplayerv1.cpp \
        $$PWD/abuleduaudiodecoderv1.cpp \
        $$PWD/abuleduslidesrecorderv1.cpp \
        $$PWD/abuleduaudioplayerv1.cpp \
        $$PWD/abuledumultimediawidgetcontainer.cpp \
        $$PWD/abuledumultimediasettingsv1.cpp \
        $$PWD/abuledumultimediadevicechooserv1.cpp \
        $$PWD/abuledumediamedias.cpp


#RESOURCES += \
#        $$PWD/abuleducontrolaudiov1.qrc

FORMS += \
        $$PWD/abuledumultimediav1.ui \
        $$PWD/abuleducontrolaudiov1.ui \
        $$PWD/abuledumultimediadevicechooserv1.ui

# on inclut le fichier ressources du developpeur de l'application en priorite
# mais seulement s'il existe. S'il n'existe pas on fallback sur le fichier
# de ressources proposé par la lib
#!exists($$PWD/../../data/abuledumultimediav1/abuledumultimediav1.qrc) {
#  RESOURCES += $$PWD/abuledumultimediav1.qrc
#}
#exists($$PWD/../../data/abuledumultimediav1/abuledumultimediav1.qrc) {
#  RESOURCES += $$PWD/../../data/abuledumultimediav1/abuledumultimediav1.qrc
#}
!exists($$PWD/../../data/abuledumultimediav1/abuleducontrolaudiov1.qrc) {
  RESOURCES += $$PWD/abuleducontrolaudiov1.qrc
}
exists($$PWD/../../data/abuledumultimediav1/abuleducontrolaudiov1.qrc) {
  RESOURCES += $$PWD/../../data/abuledumultimediav1/abuleducontrolaudiov1.qrc
}

!exists($$PWD/../abuleduflatboutonv1/abuleduflatboutonv1.pri ) {
  error("la lib AbulEduMultiMediaV1 dépend de abuleduflatboutonv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduFlatBoutonV1")
}

!exists($$PWD/../abuledupicottsv1/abuledupicottsv1.pri) {
  error("la lib AbulEduMultiMediaV1 dépend de abuledupicottsv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduPicottsV1")
}

!exists($$PWD/../abuledusingletonv1/abuledusingletonv1.pri) {
  error("la lib AbulEduMultiMediaV1 dépend de abuledusingletonv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduSingletonV1")
}

!exists($$PWD/../abuleduaudiov1/abuleduaudiov1.pri) {
  error("la lib AbulEduMultiMediaV1 dépend de abuleduaudiov1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduAudioV1")
}
