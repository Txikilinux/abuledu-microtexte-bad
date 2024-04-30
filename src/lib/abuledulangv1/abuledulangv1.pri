# ====================================================================================
# AbulEduLangV1
# astuce pour faciliter la vie des developpeurs pour le chapitre international des
# logiciels du terrier
#
# pensez a lire la doc: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduLangV1
#
# ====================================================================================
# et ne touchez plus a tout ce qui se passe ci-dessous, merci VLC
# http://code.google.com/p/vlc-record/source/browse/trunk/vlc-record/language.pri?spec=svn448&r=448

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt4 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

# create ts files, make sure to NOT delete them on clean!
#PRO_FILE = $$basename(TARGET).pro
!exists( $$PWD/../../lang/lang.pri ) {
  error("il manque le fichier lang/lang.pri !")
}
else {
  include($$PWD/../../lang/lang.pri)
}

# create qm files from ts files.
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
win32 {
      #updateqm.commands = $$PWD/lrelease-ryxeo.bat ${QMAKE_FILE_IN_BASE} $$QMAKE_LRELEASE ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
      updateqm.commands = $$PWD/lrelease-ryxeo.msys.sh ${QMAKE_FILE_IN_BASE} $$QMAKE_LRELEASE ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
}
unix {
     updateqm.commands = $$PWD/lrelease-ryxeo.sh ${QMAKE_FILE_IN_BASE} $$QMAKE_LRELEASE ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
}
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm
PRE_TARGETDEPS += compiler_updateqm_make_all
