INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

QT       += multimedia

HEADERS += $$PWD/waveheaderreader.h\
           $$PWD/wavetoogg.h \
           $$PWD/oggvorbisfileinfo.h \
           $$PWD/wavefilewriter.h \
           $$PWD/oggtowave.h \
           $$PWD/wavetooggthread.h

SOURCES += $$PWD/waveheaderreader.cpp\
           $$PWD/wavetoogg.cpp \
           $$PWD/oggvorbisfileinfo.cpp \
           $$PWD/wavefilewriter.cpp \
           $$PWD/oggtowave.cpp \
           $$PWD/wavetooggthread.cpp

win32{
    # inclusion lib ogg + vorbis
#    INCLUDEPATH += $$quote(C:/code/vorbis/libvorbis-1.3.4/include)
#    INCLUDEPATH += $$quote(C:/code/vorbis/libogg-1.3.1/include)
#    LIBS += -L$$quote(C:/code/vorbis/libogg-1.3.1/src/.libs) -logg
#    LIBS += -L$$quote(C:/code/vorbis/libvorbis-1.3.4/lib/.libs) -lvorbis -lvorbisenc -lvorbisfile

    INCLUDEPATH += $$quote(C:/code/vorbis/libvorbis-1.3.5/include)
    INCLUDEPATH += $$quote(C:/code/vorbis/libogg-1.3.2/include)
    LIBS += -L$$quote(C:/code/vorbis/libogg-1.3.2/src/.libs) -logg
    LIBS += -L$$quote(C:/code/vorbis/libvorbis-1.3.5/lib/.libs) -lvorbis -lvorbisenc -lvorbisfile
}

android {
    #installer avec
    #cd /opt/android
    #git clone https://github.com/vincentjames501/libvorbis-libogg-android.git
    INCLUDEPATH +=  /opt/android/libvorbis-libogg-android/jni/include
    LIBS += -L/opt/android/libvorbis-libogg-android/libs/armeabi -logg -lvorbis
    #pas de -lvorbisfile pour android
    # -lvorbisenc
    ANDROID_EXTRA_LIBS += /opt/android/libvorbis-libogg-android/libs/armeabi/libogg.so
    ANDROID_EXTRA_LIBS += /opt/android/libvorbis-libogg-android/libs/armeabi/libvorbis.so
    ANDROID_EXTRA_LIBS += /opt/android/libvorbis-libogg-android/libs/armeabi/libvorbis-jni.so
}

!android{
  linux{
   !exists(/usr/include/vorbis/codec.h) {
     warning("Les fichiers d'entetes de la lib vorbis ne semblent pas etre presents ... suggestion apt-get install libvorbis-dev")
   }

   INCLUDEPATH += $$PWD/lib/libogg \
                  $$PWD/lib/libvorbis

   LIBS += -logg -lvorbis -lvorbisenc -lvorbisfile
  }
}
