TEMPLATE = lib
CONFIG += qt warn_on
QT -= gui
DEPENDPATH += .
INCLUDEPATH += .

VERSION = 1.0.0

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

DEFINES += QUAZIP_BUILD
CONFIG(staticlib): DEFINES += QUAZIP_STATIC

unix:!symbian {
    headers.path=$$PREFIX/include/quazip
    headers.files=$$HEADERS
    target.path=$$PREFIX/lib
    INSTALLS += headers target

	OBJECTS_DIR=.obj
	MOC_DIR=.moc
	
}

win32 {
    headers.path=$$PREFIX/include/quazip
    headers.files=$$HEADERS
    target.path=$$PREFIX/lib
    INSTALLS += headers target
    # workaround for qdatetime.h macro bug
    DEFINES += NOMINMAX

    #erics
    exists(C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib.pri) {
      include(C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib.pri)
      include(C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib_dependency.pri)
      INCLUDEPATH += D:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib
    }
    
    exists(C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib.pri) {
      include(C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib.pri)
      include(C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib_dependency.pri)
      INCLUDEPATH += C:/Qt/5.1.1/Src/qtbase/src/3rdparty/zlib
    }

    #include(C:/Qt/2010.05/qt/src/3rdparty/zlib.pri)
    #INCLUDEPATH += C:/Qt/2010.05/qt/src/3rdparty/zlib

    #*-g++*: LIBS += -lz.dll
    #*-msvc*: LIBS += -lzlibwapi
    #*-msvc*: QMAKE_LFLAGS += /IMPLIB:$$DESTDIR\\quazip.lib
}


symbian {

    # Note, on Symbian you may run into troubles with LGPL.
    # The point is, if your application uses some version of QuaZip,
    # and a newer binary compatible version of QuaZip is released, then
    # the users of your application must be able to relink it with the
    # new QuaZip version. For example, to take advantage of some QuaZip
    # bug fixes.

    # This is probably best achieved by building QuaZip as a static
    # library and providing linkable object files of your application,
    # so users can relink it.

    CONFIG += staticlib
    CONFIG += debug_and_release

    LIBS += -lezip

    #Export headers to SDK Epoc32/include directory
    exportheaders.sources = $$HEADERS
    exportheaders.path = quazip
    for(header, exportheaders.sources) {
        BLD_INF_RULES.prj_exports += "$$header $$exportheaders.path/$$basename(header)"
    }
}
