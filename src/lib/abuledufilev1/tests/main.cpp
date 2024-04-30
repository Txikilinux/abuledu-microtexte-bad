/** Jeu d'essai pour l'implémentation de LOM/LOMFR/SCOLOM
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "abuleduscolomfrv1.h"
#include "abuleduscolomfrv1tests.h"
#include "abuleduscolomfrv1testsexport.h"
#include "abuleduscolomfrv1testsimport.h"
#include "abuleduscolomfrv1testsvcard.h"
#include "abuledufilev1tests.h"
#include "abuledumimetypev1tests.h"

int main(int argc, char *argv[])
{
    abuleduscolomfrv1Tests test1;
    QTest::qExec(&test1, argc, argv);

    abuleduscolomfrv1TestsExport test2;
    QTest::qExec(&test2, argc, argv);

    AbulEduSCOLOMFRv1TestsImport test3;
    QTest::qExec(&test3, argc, argv);

    AbulEduFileV1Tests test4;
    QTest::qExec(&test4, argc, argv);

    AbulEduMimeTypeV1Tests test5;
    QTest::qExec(&test5, argc, argv);



    return 0;
}
