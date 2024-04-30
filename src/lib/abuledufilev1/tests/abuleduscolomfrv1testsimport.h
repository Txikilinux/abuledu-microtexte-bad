/**
  * Classe
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

#ifndef TST_LOMTESTS_IMPORT_H
#define TST_LOMTESTS_IMPORT_H

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "abuleduscolomfrv1.h"

class AbulEduSCOLOMFRv1TestsImport : public QObject
{
    Q_OBJECT

public:
    explicit AbulEduSCOLOMFRv1TestsImport();

signals:

public slots:

private:
    AbulEduSCOLOMFrV1 *m_lom;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void test_abeLOMsetFile();

    void test_abeLOMImportXML_mini();
    void test_abeLOMImportXML_miniLOMFR();
    void test_abeLOMImportXML_miniSCOLOMFR();
    void test_abeLOMImportXML_miniABULEDU();


};

#endif // TST_LOMTESTS_IMPORT_H
