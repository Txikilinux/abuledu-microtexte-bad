/** Jeu d'essai pour valider la classe LOM en IMPORT
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
#include "abuleduscolomfrv1testsimport.h"

AbulEduSCOLOMFRv1TestsImport::AbulEduSCOLOMFRv1TestsImport()
{
}

void AbulEduSCOLOMFRv1TestsImport::initTestCase()
{
}

void AbulEduSCOLOMFRv1TestsImport::cleanupTestCase()
{
}

void AbulEduSCOLOMFRv1TestsImport::test_abeLOMsetFile()
{
    m_lom = new AbulEduSCOLOMFrV1();
}

void AbulEduSCOLOMFRv1TestsImport::test_abeLOMImportXML_mini()
{
    m_lom = new AbulEduSCOLOMFrV1();
    //On ouvre le fichier xml -> il devrait remplir le m_lom
    m_lom->abeLOMsetFile("dataTests/abuledulomfrv1-demo-mini.xml");

    QFile file("dataTests/abuledulomfrv1-demo-mini.xml");
    file.open(QIODevice::ReadOnly);
    QByteArray verif(file.read(file.size()));
    file.close();

    //On exporte le meme m_lom pour verifier si c'est ok
    QFile fileT("/tmp/import-abuledulomfrv1-demo-mini.test.xml");
    if(fileT.open(QIODevice::WriteOnly)) {
        QByteArray temp = m_lom->abeLOMExportAsXML();
        fileT.write(temp, temp.size());
        fileT.close();
    }

    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}

void AbulEduSCOLOMFRv1TestsImport::test_abeLOMImportXML_miniLOMFR()
{
    m_lom = new AbulEduSCOLOMFrV1();
    //On ouvre le fichier xml -> il devrait remplir le m_lom
    m_lom->abeLOMsetFile("dataTests/abuledulomfrv1-demo-miniLOMFR.xml");

    QFile file("dataTests/abuledulomfrv1-demo-miniLOMFR.xml");
    file.open(QIODevice::ReadOnly);
    QByteArray verif(file.read(file.size()));
    file.close();

    //On exporte le meme m_lom pour verifier si c'est ok
    QFile fileT("/tmp/import-abuledulomfrv1-demo-miniLOMFR.test.xml");
    if(fileT.open(QIODevice::WriteOnly)) {
        QByteArray temp = m_lom->abeLOMExportAsXML();
        fileT.write(temp, temp.size());
        fileT.close();
    }

    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}

void AbulEduSCOLOMFRv1TestsImport::test_abeLOMImportXML_miniSCOLOMFR()
{
    m_lom = new AbulEduSCOLOMFrV1();
    //On ouvre le fichier xml -> il devrait remplir le m_lom
    m_lom->abeLOMsetFile("dataTests/abuledulomfrv1-demo-miniSCOLOMFR.xml");

    QFile file("dataTests/abuledulomfrv1-demo-miniSCOLOMFR.xml");
    file.open(QIODevice::ReadOnly);
    QByteArray verif(file.read(file.size()));
    file.close();

    //On exporte le meme m_lom pour verifier si c'est ok
    QFile fileT("/tmp/import-abuledulomfrv1-demo-miniSCOLOMFR.test.xml");
    if(fileT.open(QIODevice::WriteOnly)) {
        QByteArray temp = m_lom->abeLOMExportAsXML();
        fileT.write(temp, temp.size());
        fileT.close();
    }

    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}

void AbulEduSCOLOMFRv1TestsImport::test_abeLOMImportXML_miniABULEDU()
{
    m_lom = new AbulEduSCOLOMFrV1();
    //On ouvre le fichier xml -> il devrait remplir le m_lom
    m_lom->abeLOMsetFile("dataTests/abuledulomfrv1-demo-miniABULEDU.xml");

    QFile file("dataTests/abuledulomfrv1-demo-miniABULEDU.xml");
    file.open(QIODevice::ReadOnly);
    QByteArray verif(file.read(file.size()));
    file.close();

    //On exporte le meme m_lom pour verifier si c'est ok
    QFile fileT("/tmp/import-abuledulomfrv1-demo-miniABULEDU.test.xml");
    if(fileT.open(QIODevice::WriteOnly)) {
        QByteArray temp = m_lom->abeLOMExportAsXML();
        fileT.write(temp, temp.size());
        fileT.close();
    }

    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}

