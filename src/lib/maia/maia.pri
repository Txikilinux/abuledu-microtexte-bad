QT += xml network

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/maiaObject.h \
            $$PWD/maiaFault.h \
            $$PWD/maiaXmlRpcClient.h \
            $$PWD/maiaXmlRpcServer.h \
            $$PWD/maiaXmlRpcServerConnection.h

SOURCES += $$PWD/maiaObject.cpp \
            $$PWD/maiaFault.cpp \
            $$PWD/maiaXmlRpcClient.cpp \
            $$PWD/maiaXmlRpcServer.cpp \
            $$PWD/maiaXmlRpcServerConnection.cpp


!exists($$PWD/../abuleduapplicationv1/abuleduapplicationv1.pri ) {
  error("la lib maia(ryxeo) dépend de abuleduapplicationv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1")
}


!exists($$PWD/../abuledunetworkaccessmanagerv1/abuledunetworkaccessmanagerv1.h ) {
  error("la lib maia(ryxeo) dépend de abuledunetworkaccessmanagerv1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduNetworkAccessManagerV1")
}

