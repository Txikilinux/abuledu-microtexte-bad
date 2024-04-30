INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/abuledupluginloaderv1.h

SOURCES += $$PWD/abuledupluginloaderv1.cpp

# Dépendance sur AbulEduSingletonV1
!exists($$PWD/../abuledusingletonv1/abuledusingletonv1.pri) {
  error("la lib abuledupluginloaderv1 dépend de abuledusingletonv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduSingletonV1")
}

