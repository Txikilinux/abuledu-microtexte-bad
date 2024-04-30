HEADERS += $$PWD/abuledusplashscreenv1.h
SOURCES += $$PWD/abuledusplashscreenv1.cpp

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# on inclut le fichier ressources du developpeur de l'application en priorite
# mais seulement s'il existe. S'il n'existe pas on fallback sur le fichier
# de ressources propose par la lib
!exists($$PWD/../../data/abuledusplashscreenv1/abuledusplashscreenv1.qrc) {
  RESOURCES += $$PWD/abuledusplashscreenv1.qrc
}
exists($$PWD/../../data/abuledusplashscreenv1/abuledusplashscreenv1.qrc) {
  RESOURCES += $$PWD/../../data/abuledusplashscreenv1/abuledusplashscreenv1.qrc
}

# même chose pour le fichier conf
!exists($$PWD/../../data/abuledusplashscreenv1/abuledusplashscreenv1.conf) {
  OTHER_FILES += $$PWD/abuledusplashscreenv1.conf
}
exists($$PWD/../../data/abuledusplashscreenv1/abuledusplashscreenv1.conf) {
  OTHER_FILES += $$PWD/../../data/abuledusplashscreenv1/abuledusplashscreenv1.conf
}

!exists($$PWD/../abuleduflatboutonv1/abuleduflatboutonv1.h ) {
  error("la lib abuledusplashscreenv1 dépend de abuleduflatboutonv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduFlatBoutonV1")
}

RESOURCES += $$PWD/abuledusplashscreenv1-lib.qrc
