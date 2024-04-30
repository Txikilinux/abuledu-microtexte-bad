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

#include <QtCore/QString>
#include <QtTest/QtTest>
#include "vcard.h"
#include "errorhandler.h"
#include "debug.h"

class VcardTest : public QObject
{
    Q_OBJECT
    
public:
    VcardTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
    void equal();
    void nameEqual();
    void nameNotEqual();
    void nameNotEqual2();
    void exportName();
    void exportUTF();
    void exportNormal();
};

VcardTest::VcardTest()
{
    qInstallMsgHandler(errorHandler);
    //juste pour tester le debug
    DEBUG("debug output");
    WARN("warning output");
    CRITICAL("critical output");
//    FATAL("fatal output");

//    qDebug() << "debug output";
//    qWarning() << "warning output";
//    qCritical() << "critical output";
}

void VcardTest::initTestCase()
{
}

void VcardTest::cleanupTestCase()
{
}

void VcardTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void VcardTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

//Vérificiation de l'opérateur == nouvellement arrivé dans vcard
void VcardTest::equal()
{
    vCard vcardB;
    vCardProperty name_prop  = vCardProperty::createName("Tom", "Pouce");
    vcardB.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Tom pouce");
    vcardB.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    vcardB.addProperty(tel_prop);
    vCardProperty logo_prop  = vCardProperty(VC_LOGO, "http:///www.abuledu.net");
    vcardB.addProperty(logo_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "adresse@none.net");
    vcardB.addProperty(email_prop);
    vCardProperty note_prop  = vCardProperty(VC_NOTE, "");
    vcardB.addProperty(note_prop);
    vCardProperty org_prop   = vCardProperty::createOrganization("");
    vcardB.addProperty(org_prop);
    vCardProperty url_prop   = vCardProperty(VC_URL, "");
    vcardB.addProperty(url_prop);

    QCOMPARE(vcardB, vcardB);
}

//Il faudrait vérifier chaque champ un par un (?)
void VcardTest::nameEqual()
{
    vCard vcardB;
    vCardProperty name_prop  = vCardProperty::createName("Tom", "Pouce");
    vcardB.addProperty(name_prop);

    vCard vcardC;
    name_prop  = vCardProperty::createName("Tom", "Pouce");
    vcardC.addProperty(name_prop);

    QCOMPARE(vcardB, vcardC);
}

//Vérification sur le nom
void VcardTest::nameNotEqual()
{
    vCard vcardB;
    vCardProperty name_prop  = vCardProperty::createName("Tom", "Pouce");
    vcardB.addProperty(name_prop);

    vCard vcardC;
    name_prop  = vCardProperty::createName("Tom", "Leon");
    vcardC.addProperty(name_prop);

    QEXPECT_FAIL("", "Test d'erreur, vcard differentes (prenom)", Continue);
    QCOMPARE(vcardB, vcardC);
}

//Vérification sur le prénom
void VcardTest::nameNotEqual2()
{
    vCard vcardB;
    vCardProperty name_prop  = vCardProperty::createName("Tom", "Pouce");
    vcardB.addProperty(name_prop);

    vCard vcardC;
    name_prop  = vCardProperty::createName("Tam", "Pouce");
    vcardC.addProperty(name_prop);

    QEXPECT_FAIL("", "Test d'erreur, vcard differentes (nom)", Continue);
    QCOMPARE(vcardB, vcardC);
}

//Vérification que l'export donne ce qu'on espère avoir, le fichier vcard/tompouce_minimal.vcard est une vcard
//de référence
void VcardTest::exportName()
{
    vCard vcardB;
    vCardProperty name_prop  = vCardProperty::createName("Tom", "Pouce");
    vcardB.addProperty(name_prop);

    QFile input("vcard/tompouce_minimal.vcard");
    QByteArray verif;
    if (input.open(QFile::ReadOnly | QFile::Text))
    {
        verif = input.readAll();
        input.close();
    }
    QCOMPARE(vcardB.toByteArray(), verif);
}

//Vérification de l'export pour une vcard un peu plus complète
//vcard avec des majuscules accentuées
void VcardTest::exportUTF()
{
    vCard vcardB;
    vCardProperty name_prop  = vCardProperty::createName("Éric", "Seigne");
    vcardB.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Éric Seigne");
    vcardB.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    vcardB.addProperty(tel_prop);
    vCardProperty logo_prop  = vCardProperty(VC_LOGO, "http:///www.ryxeo.com");
    vcardB.addProperty(logo_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "eric.seigne@ryxeo.com");
    vcardB.addProperty(email_prop);
    vCardProperty note_prop  = vCardProperty(VC_NOTE, "");
    vcardB.addProperty(note_prop);
    vCardProperty org_prop   = vCardProperty::createOrganization("");
    vcardB.addProperty(org_prop);
    vCardProperty url_prop   = vCardProperty(VC_URL, "");
    vcardB.addProperty(url_prop);

    //juste pour "voir"
    //    vcardB.saveToFile("/tmp/toto");
    QFile input("vcard/eric.vcard");
    QByteArray verif;
    if (input.open(QFile::ReadOnly | QFile::Text))
    {
        verif = input.readAll();
        input.close();
    }

    QEXPECT_FAIL("err1", "Test de vcard ayant des majuscules accentuees ... ça ne marche pas pour l'instant, c'est un vrai bug.", Continue);
    QCOMPARE(vcardB.toByteArray(), verif);
}

//Vérification de l'export pour une vcard un peu plus complète
//vcard avec des majuscules accentuées
void VcardTest::exportNormal()
{

    vCard vcardB;
    vCardProperty name_prop  = vCardProperty::createName("Philippe", "Cadaugade");
    vcardB.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Philippe Cadaugade");
    vcardB.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    vcardB.addProperty(tel_prop);
    vCardProperty logo_prop  = vCardProperty(VC_LOGO, "http:///www.ryxeo.com");
    vcardB.addProperty(logo_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "philippe.cadaugade@ryxeo.com");
    vcardB.addProperty(email_prop);
    vCardProperty note_prop  = vCardProperty(VC_NOTE, "");
    vcardB.addProperty(note_prop);
    vCardProperty org_prop   = vCardProperty::createOrganization("");
    vcardB.addProperty(org_prop);
    vCardProperty url_prop   = vCardProperty(VC_URL, "");
    vcardB.addProperty(url_prop);

    vcardB.saveToFile("/tmp/toto");

    QFile input("vcard/philippe.vcard");
    QByteArray verif;
    if (input.open(QFile::ReadOnly | QFile::Text))
    {
        verif = input.readAll();
        input.close();
    }
    QCOMPARE(vcardB.toByteArray(), verif);
}

QTEST_APPLESS_MAIN(VcardTest)

#include "tst_vcardtest.moc"
