QT += core gui

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/abuledumenufeuillev1.h

SOURCES += $$PWD/abuledumenufeuillev1.cpp

FORMS += $$PWD/abuledumenufeuillev1.ui

#on inclus le fichier ressources du developpeur de l'application en priorite
#mais seulement s'il existe. S'il n'existe pas on fallback sur le fichier
#de ressources propose par la lib
!exists($$PWD/../../data/abuledumenufeuillev1/abuledumenufeuillev1.qrc) {
  RESOURCES += $$PWD/abuledumenufeuillev1.qrc
} else {
  RESOURCES += $$PWD/../../data/abuledumenufeuillev1/abuledumenufeuillev1.qrc
}

!exists($$PWD/../../data/abuledumenufeuillev1/abuledumenufeuillev1.conf) {
  OTHER_FILES = $$PWD/abuledumenufeuillev1.conf
} else {
  OTHER_FILES = $$PWD/../../data/abuledumenufeuillev1/abuledumenufeuillev1.conf
}
