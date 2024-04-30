#
# Creation du plugin Qt Creator pour AbulEduFlatBoutonV1
#
# compilation un peu speciale: il faut la version de Qt utilisé pour compiler qt creator,
# direction Aide / À propos de Qt Creator, par exemple chez moi c'est Qt Creator 2.4.1
# "Basé sur Qt 4.7.4 (64 bit)" il me faut donc les sources de Qt 4.7.4
#
# ensuite lancement de la commande suivante (ex. si Qt Creator est installé dans /opt):
# /opt/QtSDK/Desktop/Qt/474/gcc/bin/qmake && make -j2 && sudo cp libabuleduflatboutonv1.so  /opt/QtSDK/QtCreator/lib/qtcreator/plugins/designer/

QT      += designer
CONFIG += debug_and_release plugin
TARGET      = $$qtLibraryTarget(abuleduflatboutonv1)
TEMPLATE    = lib

HEADERS     = abuleduflatboutonv1-plugin.h
SOURCES     = abuleduflatboutonv1-plugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

QT += network

#important pour les creations d'objets specifiques abuledu
DEFINES += ABULEDU_DESIGNER_PLUGIN

include("../abuleduflatboutonv1.pri")
