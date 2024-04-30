HEADERS += $$PWD/abuleduaproposv1.h
 
 SOURCES += $$PWD/abuleduaproposv1.cpp
 
 FORMS += $$PWD/abuleduaproposv1.ui
 
 INCLUDEPATH += $$PWD
 DEPENDPATH += $$PWD
 
 #si fichier ressource, on inclut le fichier ressources du developpeur de 
 #l'application en priorite mais seulement s'il existe. S'il n'existe pas on 
 #fallback sur le fichier de ressources propose par la lib
 !exists($$PWD/../../data/abuleduaproposv1/abuleduaproposv1.qrc) {
   RESOURCES += $$PWD/abuleduaproposv1.qrc
 }
 exists($$PWD/../../data/abuleduaproposv1/abuleduaproposv1.qrc) {
   RESOURCES += $$PWD/../../data/abuleduaproposv1/abuleduaproposv1.qrc
 }

!exists($$PWD/../abuleduflatboutonv1/abuleduflatboutonv1.h ) {
  error("la lib AbulEduAproposV1 dépend de AbulEduFlatBoutonV1: https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduFlatBoutonV1")
}
