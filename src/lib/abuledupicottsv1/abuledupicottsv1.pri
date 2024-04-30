lessThan(QT_MAJOR_VERSION, 5) {
  QT += phonon
}
else {
  QT += multimedia multimediawidgets
}

#Qt5 pour ubuntu/utopic ... ils ont un peu tout decoupes en morceau ...
# i386
exists(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri) {
  include(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri)
  INCLUDEPATH += /usr/include/i386-linux-gnu/qt5/QtMultimedia
  LIBS     += -lQt5Multimedia
}

exists(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri) {
  include(/usr/lib/i386-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri)
  INCLUDEPATH += /usr/include/i386-linux-gnu/qt5/QtMultimediaWidgets
  LIBS     += -lQt5MultimediaWidgets
}

#Qt5 pour ubuntu/utopic ... ils ont un peu tout decoupes en morceau ...
# amd64
exists(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri) {
  include(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri)
  INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtMultimedia
  LIBS     += -lQt5Multimedia
}

exists(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri) {
  include(/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri)
  INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtMultimediaWidgets
  LIBS     += -lQt5MultimediaWidgets
}

#Qt5 sur trusty ...
exists(/usr/include/qt5/QtMultimedia/QMediaPlayer) {
  INCLUDEPATH += /usr/include/qt5/QtMultimedia
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/abuledupicottsv1.h
SOURCES += $$PWD/abuledupicottsv1.cpp

# pour picoTTS
win32 {
  exists(C:/code/svox-git) {
    INCLUDEPATH  += $$quote(C:/code/svox-git/pico/lib)
    LIBS         += -L$$quote(C:/code/svox-git/pico/.libs) -lttspico
  }
  ! exists(C:/code/svox-git) {
    error("LibTTS manquante !")
  }
}
macx {

}

linux-g++-32|linux-g++-64|linux-g++ {
  ! exists( /usr/include/picoapi.h ) {
    error("LibTTS manquante ! assurez vous d'avoir les paquets suivants d'installés: libttspico-dev libttspico0 libttspico-data")
  }
  LIBS     += -L/usr/lib/ -lttspico
  INCLUDEPATH += /usr/include
}


android {
  exists(/opt/android/aosp-4.0/out/target/product/generic/system/lib/libttspico.so) {
    LIBS     += -L/opt/android/aosp-4.0/out/target/product/generic/system/lib/ -lttspico
    INCLUDEPATH +=
    ANDROID_EXTRA_LIBS += /opt/android/aosp-4.0/out/target/product/generic/system/lib/libttspico.so
  }
  exists(/opt/android/svox/libs/armeabi/libttspico.so) {
    LIBS     += -L/opt/android/svox/libs/armeabi/ -lttspico
    INCLUDEPATH +=
    ANDROID_EXTRA_LIBS += /opt/android/svox/libs/armeabi/libttspico.so
    ANDROID_EXTRA_LIBS += /opt/android/svox/libs/armeabi/libttscompat.so
  }
  #!exists(/opt/android/aosp-4.0/out/target/product/generic/system/lib/libttspico.so) {
  #  LIBS     += -L/opt/android/svox/obj/local/armeabi -lsvoxpico
  #  INCLUDEPATH += /opt/android/svox/pico
  #}
}
