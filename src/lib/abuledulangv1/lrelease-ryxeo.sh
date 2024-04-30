#!/bin/bash

QMAKE_FILE_IN_BASE=${1}
QMAKE_LRELEASE=${2}
QM_DEST=${3}

if [ -z "{$QMAKE_FILE_IN_BASE}" -o -z "${QMAKE_LRELEASE}" -o -z "${QM_DEST}" ]; then
    echo "Erreur un fichier ou plus n'a pas ete indique sur la ligne de commande"
    exit -2
fi

LALANGUE=`echo ${QMAKE_FILE_IN_BASE} | cut -d "_" -f2-3`
FICTS=`ls lib/*/lang/*_${LALANGUE}.ts`

if [ -n "${FICTS}" ]; then
    echo "on est ici : *${FICTS}*"
    ${QMAKE_LRELEASE} -compress -silent lang/${QMAKE_FILE_IN_BASE}.ts lib/*/lang/*_${LALANGUE}.ts -qm ${QM_DEST}
fi
