#!/bin/bash
# script qui extrait les langues de toutes les libs mais de maniere modulaire
# 
#
langues="fr es en de it oc_leng ar"

if [ ! -f *.pro ]; then
  echo "Ce script doit etre lance depuis le repertoire ou se trouve le fichier .pro"
  exit -1
fi

#pour eviter le include des moc_*
make distclean

lelogiciel=`grep TARGET *.pro | cut -d "=" -f2 | tr -d " "`

if [ -z "${lelogiciel}" ]; then
    echo "il manque le TARGET dans le fichier .pro"
    exit -1
fi

for langue in ${langues}
do
  lupdate-qt4 -no-recursive . -ts lang/${lelogiciel}_${langue}.ts
done

cd lib
for lalib in *
do
  (
  cd ${lalib}
  if [ ! -d lang ]; then
    mkdir lang
  fi
  if [ -d lang ]; then
    for langue in ${langues}
    do
      lupdate-qt4 . -ts lang/${lalib}_${langue}.ts
    done
  fi
  )
done
