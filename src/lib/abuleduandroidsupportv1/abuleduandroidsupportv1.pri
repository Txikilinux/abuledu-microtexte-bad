
#tout ce qui est commun pour le support android

android {
  DEFINES += QT_NO_PRINTER=1
  DEFINES += __ABULEDUTABLETTEV1__MODE__=1

#ne supprimez surtout pas ça, cf http://qt-project.org/forums/viewthread/44797
  !contains(DEFINES, .*Q_OS_ANDROID.*) {
    DEFINES += Q_OS_ANDROID=1
  }

  #auto detection du compilateur
#  system(arm-linux-gnueabi-g++-4.6 --version   > /dev/null):QMAKE_CXX = ccache arm-linux-gnueabi-g++-4.6
#  system(arm-linux-gnueabihf-g++-4.6 --version > /dev/null):QMAKE_CXX = ccache arm-linux-gnueabihf-g++-4.6
#  system(arm-linux-gnueabihf-g++-4.7 --version > /dev/null):QMAKE_CXX = ccache arm-linux-gnueabihf-g++-4.7
#  system(arm-linux-gnueabihf-g++-4.8 --version > /dev/null):QMAKE_CXX = ccache arm-linux-gnueabihf-g++-4.8

  #ajout de la lib SSL dans android ...
  exists(/opt/android/aosp-4.0/out/target/product/generic/system/lib/libssl.so) {
    #dans ce cas desactive le NO_SSL...
    DEFINES -= QT_NO_SSL
    ANDROID_OPENSSL = /opt/android/aosp-4.0/external/openssl/
    ANDROID_EXTRA_LIBS += /opt/android/aosp-4.0/out/target/product/generic/system/lib/libssl.so
    ANDROID_EXTRA_LIBS += /opt/android/aosp-4.0/out/target/product/generic/system/lib/libcrypto.so
    INCLUDEPATH += $$ANDROID_OPENSSL/include/
    LIBS += -L/opt/android/aosp-4.0/out/target/product/generic/system/lib/
  }
  !exists(/opt/android/aosp-4.0/out/target/product/generic/system/lib/libssl.so) {
    exists(/opt/android/OpenSSL1.0.1cForAndroid/libs/armeabi/) {
      #dans ce cas desactive le NO_SSL...
      DEFINES -= QT_NO_SSL
      ANDROID_OPENSSL = /opt/android/OpenSSL1.0.1cForAndroid/
      INCLUDEPATH += $$ANDROID_OPENSSL/include/
      LIBS += -L/opt/android/OpenSSL1.0.1cForAndroid/libs/armeabi/
      LIBS += -lssl

      ANDROID_EXTRA_LIBS += /opt/android/OpenSSL1.0.1cForAndroid/libs/armeabi/libssl.so
      ANDROID_EXTRA_LIBS += /opt/android/OpenSSL1.0.1cForAndroid/libs/armeabi/libcrypto.so

  #    QMAKE_POST_LINK += $$quote(mkdir -p $$PWD/../../../android/libs/armeabi-v7a/;)
  #    QMAKE_POST_LINK += $$quote(cp /opt/android/OpenSSL1.0.1cForAndroid/libs/armeabi/*.so $$PWD/../../../android/libs/armeabi-v7a/;)
    }
  #  warning("La lib SSL n'est pas présente dans /opt/android/aosp-4.0/out/target/product/generic/system/lib/")
  }

  #LIBS +=  -lttspico

  #support de la lib vorbis
  #ANDROID_EXTRA_LIBS += /opt/android/libvorbis-libogg-android/libs/armeabi/libogg.so
  #ANDROID_EXTRA_LIBS += /opt/android/libvorbis-libogg-android/libs/armeabi/libvorbis.so
  #ANDROID_EXTRA_LIBS += /opt/android/libvorbis-libogg-android/libs/armeabi/libvorbis-jni.so

}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../../../android/


OTHER_FILES += \
    $$PWD/../../../android/AndroidManifest.xml \
    $$PWD/../../../android/res/values/strings.xml
