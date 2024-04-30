@echo off
set QMAKE_FILE_IN_BASE=%1
set QMAKE_LRELEASE=%2
set QM_DEST=%3
set LALANGUE=%QMAKE_FILE_IN_BASE:~-2%

dir lib /s /b *_%LALANGUE%.ts > tmp.txt
set FICHIERS=<tmp.txt
echo %FICHIERS%
del tmp.txt

%QMAKE_LRELEASE% -compress -silent lang/%QMAKE_FILE_IN_BASE%.ts %FICHIERS% -qm %QM_DEST%
