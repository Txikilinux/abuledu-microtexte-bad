#!/bin/bash
#script de creation de l'image pour OSX
#Eric Seigne 2011-2012 <eric.seigne@ryxeo.com>
#documentation https://redmine.ryxeo.com/projects/ryxeo/wiki/Cr%C3%A9er_un_paquet_avec_OSX
APPNAME="abuledu-microtexte"
VOLICON="macos/icones/abuledu-microtexte.icns"
#recupere la version dans le fichier version.h
if [ -f version.h ]; then
    LAVERSIONH=`grep VER_FILEVERSION_STR version.h | awk '{print $3}' | cut -d "\\\\" -f1 | tr -d '"'`
    if [ "x${LAVERSIONH}" != "x" ]; then
	APPVERSION="${LAVERSIONH}${1}"
    fi
else 
    APPVERSION=${1}
fi
VOLNAME="AbulEdu MicroTexte ${APPVERSION}"

#On stoppe dès qu'on rencontre un problème
set -e

if [ -d /tmp/build-dmg-${APPNAME} ]; then
    rm -rf /tmp/build-dmg-${APPNAME}
fi
rm -f ${APPNAME}-*.dmg
if [ -e ${APPNAME}.app ]; then
    rm -rf ${APPNAME}.app
fi

#on fabrique une arborescence "dmg" temporaire
mkdir /tmp/build-dmg-${APPNAME}

#on cherche ou on se trouve et on change de repertoire le cas echeant
if [ -f ../${APPNAME}.pro ]; then
    cd ..
fi

bzr revert
qmake ${APPNAME}.pro -r -spec macx-g++ CONFIG+=release

#compilation
make -w -j4

#on utilise les outils de Qt pour trimbaler le Qt framework et les dependances Qt
macdeployqt ${APPNAME}.app

#note 20121021: je ne sais pas si c'est encore utile
ln -s /Applications /tmp/build-dmg-${APPNAME}/Applications

#copie des donnees
if [ -d data ]; then
    cp -a data ${APPNAME}.app/Contents/Resources/
fi
if [ -d conf ]; then
    cp -a conf ${APPNAME}.app/Contents/Resources/
fi
if [ -d lang ]; then
    mkdir ${APPNAME}.app/Contents/Resources/lang
    lrelease *.pro
    cp -a lang/*.qm ${APPNAME}.app/Contents/Resources/lang/ || true
fi

#creation du fichier dmg
~/create-dmg/create-dmg --window-size 415 295 --volname "${VOLNAME}" --volicon ${VOLICON} --background "macos/.background/background.png" ${APPNAME}-${APPVERSION}-osx.dmg ${APPNAME}.app

#nettoyage
rm -rf ${APPNAME}.app
rm -rf /tmp/build-dmg-${APPNAME}
