QT += core xml svg

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += printsupport
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/abuledufilev1.h \
            $$PWD/abuledulomv1.h \
            $$PWD/abuledumediathequegetv1.h \
            $$PWD/abuledumediathequepushv1.h \
            $$PWD/abuledumediathequecommon.h \
            $$PWD/abuledulomfrv1.h \
            $$PWD/abuleduscolomfrv1.h \
            $$PWD/abuleduwidgetlomfrv1.h \
            $$PWD/abuleduwidgetscolomfrv1.h \
            $$PWD/abuleduvcardcombobox.h \
            $$PWD/abuleduboxfilemanagerv1.h \
            $$PWD/abuleduwidgetlomv1.h \
            $$PWD/abuleduwidgetlomsuitev1.h \
            $$PWD/abuledumimetypev1.h \
            $$PWD/abuleduwidgettaxon.h \
            $$PWD/abuleduwidgetcontribution.h \
            $$PWD/abuledusearchkeywordslineedit.h \
            $$PWD/frmdisquelocal.h \
            $$PWD/abuledufileaboutv1.h \
            $$PWD/abuleduvisionneuseimagev1.h \
            $$PWD/abuledufilemanagertablewidgetitemv1.h

SOURCES += $$PWD/abuledufilev1.cpp \
            $$PWD/abuledulomv1.cpp \
            $$PWD/abuledumediathequegetv1.cpp \
            $$PWD/abuledumediathequepushv1.cpp \
            $$PWD/abuledumediathequecommon.cpp \
            $$PWD/abuledulomfrv1.cpp \
            $$PWD/abuleduscolomfrv1.cpp \
            $$PWD/abuleduwidgetlomfrv1.cpp \
            $$PWD/abuleduwidgetscolomfrv1.cpp \
            $$PWD/abuleduvcardcombobox.cpp \
            $$PWD/abuleduboxfilemanagerv1.cpp \
            $$PWD/abuleduwidgetlomv1.cpp \
            $$PWD/abuleduwidgetlomsuitev1.cpp \
            $$PWD/abuledumimetypev1.cpp \
            $$PWD/abuleduwidgettaxon.cpp \
            $$PWD/abuleduwidgetcontribution.cpp \
            $$PWD/abuledusearchkeywordslineedit.cpp \
            $$PWD/frmdisquelocal.cpp \
            $$PWD/abuledufileaboutv1.cpp \
            $$PWD/abuleduvisionneuseimagev1.cpp \
            $$PWD/abuledufilemanagertablewidgetitemv1.cpp

FORMS +=    $$PWD/abuledumediathequegetv1.ui \
            $$PWD/abuledumediathequepushv1.ui \
            $$PWD/abuleduwidgetlomfrv1.ui \
            $$PWD/abuleduwidgetscolomfrv1.ui \
            $$PWD/abuleduboxfilemanagerv1.ui \
            $$PWD/abuleduwidgetlomv1.ui \
            $$PWD/abuleduwidgetlomsuitev1.ui \
            $$PWD/abuleduwidgettaxon.ui \
            $$PWD/abuleduwidgetcontribution.ui \
            $$PWD/frmdisquelocal.ui \
            $$PWD/abuledufileaboutv1.ui

RESOURCES += \
    $$PWD/AbulEduWidgetLOM.qrc \
    $$PWD/abuleduboxfilemanagerv1.qrc \
    $$PWD/abuledufilev1.qrc \
    $$PWD/abuledumediathequegetv1.qrc \
    $$PWD/abuledumediathequepushv1.qrc

 exists($$PWD/../../data/abuledufilev1/abuleduboxfilemanagerv1.conf) {
   OTHER_FILES += $$PWD/../../data/abuledufilev1/abuleduboxfilemanagerv1.conf
 }

# la lib quazip: les fichiers sont des archives zip
include($$PWD/quazip/quazip/quazip.pri)

linux-g++-32|linux-g++-64|linux-g++ {
  ! exists( /usr/include/exiv2 ) {
    error("La lib Exiv2 est manquante ! assurez vous d'avoir les paquets suivants d'installés: libexiv2-dev")
  }
}


!exists($$PWD/../abuleduapplicationv1/abuleduapplicationv1.pri) {
  error("la lib abuledufilev1 dépend de abuleduapplicationv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1")
}

!exists($$PWD/../abuledunetworkaccessmanagerv1/abuledunetworkaccessmanagerv1.h) {
  error("la lib abuledufilev1 dépend de abuledunetworkaccessmanagerv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduNetworkAccessManagerV1")
}

!exists($$PWD/../maia/maia.pri) {
  error("la lib abuledufilev1 dépend de maia: https://redmine.ryxeo.com/projects/ryxeo/wiki/XMLRPC_avec_Maia")
}

!exists($$PWD/../abuledumessageboxv1/abuledumessageboxv1.h ) {
  error("la lib abuleduapplicationv1 dépend de abuledumessageboxv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduMessageBoxV1")
}

!exists($$PWD/../abuleduloadinganimationv1/abuleduloadinganimationv1.pri) {
  error("la lib abuledufilev1 dépend de abuleduloadinganimationv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduLoadingAnimationV1")
}

include($$PWD/vcard/vcard.pri)

#pourrait permettre la compilation de cette lib sans la lib exiv2 qui est utilisee par abuledumediathequepush
#et abuledufilev1 pour ajouter les meta info correctement dans les images ... a terme ca sera probablemene une
#lib obligatoire au meme titre que la zlib je pense
!android {
  DEFINES      += __WITHEXIV2__
  unix:LIBS += -lexiv2

  win32 {
    DEFINES      += __WINDOWS__=1

#    exists(C:/code/exiv2) {
#      INCLUDEPATH  += $$quote(C:/code/exiv2/include)
#      LIBS         += -L$$quote(C:/code/exiv2/lib) -lexiv2
#    }
    exists(C:/code/exiv2-0.25) {
      INCLUDEPATH  += $$quote(C:/code/exiv2-0.25/include)
      LIBS         += -L$$quote(C:/code/) -lexiv2
    }
 }
}
