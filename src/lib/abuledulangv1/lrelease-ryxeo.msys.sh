#!/bin/bash
export PATH="$PATH:/C/MinGW/msys/1.0/bin:/C/Code:/C/Program Files/Bazaar/:"

QMAKE_FILE_IN_BASE=${1}
QMAKE_LRELEASE=${2}
QM_DEST=${3}

LALANGUE=`echo ${QMAKE_FILE_IN_BASE} | cut -d "_" -f2-3`
${QMAKE_LRELEASE} -compress -silent lang/${QMAKE_FILE_IN_BASE}.ts lib/*/lang/*_${LALANGUE}.ts -qm ${QM_DEST}
