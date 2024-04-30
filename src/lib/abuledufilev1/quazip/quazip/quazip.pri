INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

VERSION = 1.0.0

DEFINES += QUAZIP_BUILD
CONFIG(staticlib): DEFINES += QUAZIP_STATIC


HEADERS += \
        $$PWD/crypt.h \
        $$PWD/ioapi.h \
        $$PWD/JlCompress.h \
        $$PWD/quaadler32.h \
        $$PWD/quachecksum32.h \
        $$PWD/quacrc32.h \
        $$PWD/quagzipfile.h \
        $$PWD/quaziodevice.h \
        $$PWD/quazipdir.h \
        $$PWD/quazipfile.h \
        $$PWD/quazipfileinfo.h \
        $$PWD/quazip_global.h \
        $$PWD/quazip.h \
        $$PWD/quazipnewinfo.h \
        $$PWD/unzip.h \
        $$PWD/zip.h

SOURCES += $$PWD/qioapi.cpp \
           $$PWD/JlCompress.cpp \
           $$PWD/quaadler32.cpp \
           $$PWD/quacrc32.cpp \
           $$PWD/quagzipfile.cpp \
           $$PWD/quaziodevice.cpp \
           $$PWD/quazip.cpp \
           $$PWD/quazipdir.cpp \
           $$PWD/quazipfile.cpp \
           $$PWD/quazipfileinfo.cpp \
           $$PWD/quazipnewinfo.cpp \
           $$PWD/unzip.c \
           $$PWD/zip.c

macx {
   LIBS += -lz
}

win32 {
    # a voir, notes eric 23.07.2012
    # amelioration du 13.03.2013 : tentative de solution pour que ca marche sur tt version
    # il faut compiler la quazip.dll a l'aide du quazip.pro du meme repertoire, ensuite copier
    # ce fichier quazip.dll dans le repertoire c:/code/ et c'est tout ...
    exists(C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib.pri) {
      include(C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib.pri)
      include(C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib_dependency.pri)
      INCLUDEPATH += D:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib
    }
    
    exists(C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib.pri) {
#      include(C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib.pri)
#      include(C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib_dependency.pri)
      INCLUDEPATH += C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib
    }


    exists(C:\code\zlib-1.2.8) {
        LIBS += -LC:\code\zlib-1.2.8 -lz
        INCLUDEPATH += C:\code\zlib-1.2.8
    }

    #  error("Message d'erreur " + $$[QT_INSTALL_PREFIX])
    #    INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib
    #include($$[QT_INSTALL_PREFIX]/src/3rdparty/zlib/zlib.pri)
}

linux-g++|linux-g++-32|linux-g++-64 {
   LIBS += -lz
}
