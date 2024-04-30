INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/abuleduloadinganimationv1.h

SOURCES += $$PWD/abuleduloadinganimationv1.cpp

# Dépendance sur AbulEduSingletonV1
!exists($$PWD/../abuledusingletonv1/abuledusingletonv1.pri) {
  error("la lib abuleduloadinganimationv1 dépend de abuledusingletonv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduSingletonV1")
}

!exists($$PWD/../abuleduloggerv1/abuleduloggerv1.pri ) {
  error("la lib abuleduapplicationv1 dépend de abuleduloggerv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduLoggerV1")
}

RESOURCES += $$PWD/abuleduloadinganimation.qrc
