/** Jeu d'essai pour valider la classe LOM en EXPORT
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
#include "abuleduscolomfrv1testsexport.h"

abuleduscolomfrv1TestsExport::abuleduscolomfrv1TestsExport()
{
}

void abuleduscolomfrv1TestsExport::initTestCase()
{
}

void abuleduscolomfrv1TestsExport::cleanupTestCase()
{
}

void abuleduscolomfrv1TestsExport::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void abuleduscolomfrv1TestsExport::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void abuleduscolomfrv1TestsExport::test_abeLOMsetFile()
{
    m_lom = new AbulEduSCOLOMFrV1();
    //m_lom->abeLOMsetLocalDebug(0);
}

void abuleduscolomfrv1TestsExport::test_abeLOMExportAsXML_mini()
{
    m_lom->abeLOMsetGeneralIdentifierCatalog("URI");
    m_lom->abeLOMsetGeneralIdentifierEntry("http://mediatheque.abuledu.org/URI/CODETEST");
    QFile file("dataTests/abuledulomfrv1-demo-mini.xml");
    file.open(QIODevice::ReadOnly);

    QByteArray verif(file.readAll());
    file.close();

//    qDebug() << m_lom->abeLOMExportAsXML();
//    qDebug() << verif;
    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}

void abuleduscolomfrv1TestsExport::test_abeLOMExportAsXML_miniLOMFR()
{
    m_lom->abeLOMsetGeneralIdentifierCatalog("URI");
    m_lom->abeLOMsetGeneralIdentifierEntry("http://mediatheque.abuledu.org/URI/CODETEST");
    m_lom->abeLOMsetGeneralTitle("fre","Le titre de cette ressource avec des accents");
    m_lom->abeLOMsetGeneralTitle("eng","the same in english");

    QFile file("dataTests/abuledulomfrv1-demo-miniLOMFR.xml");
    file.open(QIODevice::ReadOnly);
    QByteArray verif(file.read(file.size()));
    file.close();

//    qDebug() << m_lom->abeLOMExportAsXML();
    QFile fileT("/tmp/abuledulomfrv1-demo-miniLOMFR.test.xml");

    if(fileT.open(QIODevice::WriteOnly)) {
        QByteArray temp = m_lom->abeLOMExportAsXML();
        fileT.write(temp, temp.size());
        fileT.close();
    }

    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}

void abuleduscolomfrv1TestsExport::test_abeLOMExportAsXML_miniSCOLOMFR()
{
    m_lom->abeLOMsetGeneralIdentifierCatalog("URI");
    m_lom->abeLOMsetGeneralIdentifierEntry("http://mediatheque.abuledu.org/URI/CODETEST");
    m_lom->abeLOMsetGeneralTitle("fre","Le titre de cette ressource avec des accents");
    m_lom->abeLOMsetGeneralTitle("eng","the same in english");

    m_lom->abeLOMaddGeneralLanguage("fre");
    m_lom->abeLOMaddGeneralLanguage("eng");

    m_lom->abeLOMsetGeneralDescription("fre","Ceci est la description de la ressource ...");


    QString role   = QString::fromUtf8("author");
    QDate   ladate = QDate(2011,05,26);
    vCard verifA;
    vCardProperty name_prop  = vCardProperty::createName("Eric", "Seigne");
    verifA.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Eric Seigne");
    verifA.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verifA.addProperty(tel_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "eric.seigne@ryxeo.com");
    verifA.addProperty(email_prop);
    m_lom->abeLOMaddLifeCycleContributionRole(role,verifA,ladate);

    role = QString::fromUtf8("author");
    ladate = QDate(2011,05,26);
    vCard verifB;
    name_prop  = vCardProperty::createName("Stephane", "Hays");
    verifB.addProperty(name_prop);
    fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Stephane Hays");
    verifB.addProperty(fn_prop);
    tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verifB.addProperty(tel_prop);
    email_prop = vCardProperty(VC_EMAIL, "stephane.hays@ryxeo.com");
    verifB.addProperty(email_prop);
    m_lom->abeLOMaddLifeCycleContributionRole(role,verifB,ladate);

    role = QString::fromUtf8("validator");
    ladate = QDate(2007,03,13);
    vCard verifC;
    name_prop  = vCardProperty::createName("Philippe", "Cadaugade");
    verifC.addProperty(name_prop);
    fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Philippe Cadaugade");
    verifC.addProperty(fn_prop);
    tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verifC.addProperty(tel_prop);
    email_prop = vCardProperty(VC_EMAIL, "philippe.cadaugade@ryxeo.com");
    verifC.addProperty(email_prop);
    m_lom->abeLOMaddLifeCycleContributionRole(role,verifC,ladate);

    m_lom->abeLOMaddTechnicalFormat("iso");
    m_lom->abeLOMaddTechnicalFormat("jpg");
    m_lom->abeLOMsetTechnicalSize("500000");
    m_lom->abeLOMaddTechnicalLocation("/home/fichier.iso");

    QFile file("dataTests/abuledulomfrv1-demo-miniSCOLOMFR.xml");
    file.open(QIODevice::ReadOnly);
    QByteArray verif(file.read(file.size()));
    file.close();

//    qDebug() << m_lom->abeLOMExportAsXML();
    QFile fileT("/tmp/abuledulomfrv1-demo-miniSCOLOMFR.test.xml");

    if(fileT.open(QIODevice::WriteOnly)) {
        QByteArray temp = m_lom->abeLOMExportAsXML();
        fileT.write(temp, temp.size());
        fileT.close();
    }

    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}

void abuleduscolomfrv1TestsExport::test_abeLOMExportAsXML_miniABULEDU()
{
    m_lom->abeLOMsetGeneralIdentifierCatalog("URI");
    m_lom->abeLOMsetGeneralIdentifierEntry("http://mediatheque.abuledu.org/URI/CODETEST");
    m_lom->abeLOMsetGeneralTitle("fre",QString::fromUtf8("Le titre de cette ressource avec des accents: ééé"));
    m_lom->abeLOMsetGeneralTitle("eng","Same title in english");

    m_lom->abeLOMaddGeneralLanguage("fre");
    m_lom->abeLOMaddGeneralLanguage("spa");

    m_lom->abeLOMsetGeneralDescription("fre","Ceci est la description de la ressource ...");

    m_lom->abeLOMaddGeneralKeyword("fre",QString::fromUtf8("sciences de la vie"));
    m_lom->abeLOMaddGeneralKeyword("fre",QString::fromUtf8("svt"));
    m_lom->abeLOMaddGeneralKeyword("fre",QString::fromUtf8("biologie humaine"));
    m_lom->abeLOMaddGeneralKeyword("fre",QString::fromUtf8("caractère sexuel"));
    m_lom->abeLOMaddGeneralCoverage("fre",QString::fromUtf8("lieu, région, époque"));

    m_lom->abeLOMaddGeneralDocumentTypology(QString::fromUtf8("banque d'images"));
    m_lom->abeLOMaddGeneralDocumentTypology(QString::fromUtf8("image numérique"));
    m_lom->abeLOMaddGeneralDocumentTypology(QString::fromUtf8("collection de documents"));

    //1.7
    m_lom->abeLOMsetGeneralStructure(QString::fromUtf8("atomic"));

    //1.9
    m_lom->abeLOMaddGeneralDocumentType(QString::fromUtf8("ressource interactive"));
    m_lom->abeLOMaddGeneralDocumentType(QString::fromUtf8("ensemble de données"));
    m_lom->abeLOMaddGeneralDocumentType(QString::fromUtf8("image en mouvement"));
    m_lom->abeLOMaddGeneralDocumentType(QString::fromUtf8("son"));

    m_lom->abeLOMsetLifeCycleVersion("fre","Un nom de version ...");
    m_lom->abeLOMsetLifeCycleStatus("final");

    QString role   = QString::fromUtf8("author");
    QDate   ladate = QDate(2011,05,26);
    vCard verifA;
    vCardProperty name_prop  = vCardProperty::createName("Eric", "Seigne");
    verifA.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Eric Seigne");
    verifA.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verifA.addProperty(tel_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "eric.seigne@ryxeo.com");
    verifA.addProperty(email_prop);
    m_lom->abeLOMaddLifeCycleContributionRole(role,verifA,ladate);

    role = QString::fromUtf8("validator");
    ladate = QDate(2007,03,13);
    vCard verifC;
    name_prop  = vCardProperty::createName("Philippe", "Cadaugade");
    verifC.addProperty(name_prop);
    fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Philippe Cadaugade");
    verifC.addProperty(fn_prop);
    tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verifC.addProperty(tel_prop);
    email_prop = vCardProperty(VC_EMAIL, "philippe.cadaugade@ryxeo.com");
    verifC.addProperty(email_prop);
    m_lom->abeLOMaddLifeCycleContributionRole(role,verifC,ladate);

    //-------- branche 2
    m_lom->abeLOMaddTechnicalFormat("iso");
    m_lom->abeLOMaddTechnicalFormat("jpg");
    m_lom->abeLOMsetTechnicalSize("500000");
    m_lom->abeLOMaddTechnicalLocation("/home/fichier.iso");

    //-------- branche 5
    m_lom->abeLOMaddEducationalLearningResourceType("simulation");
    m_lom->abeLOMaddEducationalLearningResourceType("animation");
    m_lom->abeLOMaddEducationalLearningResourceType("lecture");
    m_lom->abeLOMaddEducationalIntendedEndUserRole("learner");
    m_lom->abeLOMaddEducationalIntendedEndUserRole("teacher");

    //5.6
    m_lom->abeLOMaddEducationalContext("enseignement secondaire");
    //Un autre cas particulier avec un mot traduit
    m_lom->abeLOMaddEducationalContext(QString::fromUtf8("enseignement supérieur"));
    //Un cas particulier avec une cle en langue originale
    m_lom->abeLOMaddEducationalContext("other");

    m_lom->abeLOMaddEducationalTypicalAgeRange("fre","13-18 ans");
    m_lom->abeLOMaddEducationalTypicalAgeRange("fre","ado");
    m_lom->abeLOMaddEducationalTypicalAgeRange("fre","jeunes adultes");
    m_lom->abeLOMaddEducationalTypicalAgeRange("eng","kids");

    //5.8
    m_lom->abeLOMsetEducationalDifficulty("facile");

    //Note: il faudra faire le necessaire pour que le format de la date se fasse tout seul je pense ?
    m_lom->abeLOMsetEducationalTypicalLearningTime(15);
    m_lom->abeLOMaddEducationalDescription("fre","Description de l'utilisation de cette ressource ...");
    m_lom->abeLOMaddEducationalLanguage("fre");
    m_lom->abeLOMaddEducationalActivities("apprendre");
    m_lom->abeLOMaddEducationalActivities("observer");
    m_lom->abeLOMaddEducationalActivities("s'informer");
    m_lom->abeLOMaddEducationalLocation("en salle de classe");
    m_lom->abeLOMaddEducationalLocation("en CDI");
    m_lom->abeLOMaddEducationalPedagogicModality(QString::fromUtf8("aide didactique individualisée"));
    m_lom->abeLOMaddEducationalPedagogicModality(QString::fromUtf8("en autonomie"));
    m_lom->abeLOMaddEducationalPedagogicModality(QString::fromUtf8("en groupe de compétences"));
    m_lom->abeLOMaddEducationalPedagogicModality(QString::fromUtf8("en classe entière"));
    m_lom->abeLOMaddEducationalTools(QString::fromUtf8("vidéoprojecteur"));
    m_lom->abeLOMaddEducationalTools(QString::fromUtf8("logiciels RIP"));

    //6.1
    m_lom->abeLOMsetRightsCost("yes");
    m_lom->abeLOMsetRightsCopyrightAndOtherRestrictions("yes");
    m_lom->abeLOMsetRightsDescription("fre","cc-by-sa");

    //7.1
    m_lom->abeLOMsetRelationKind("references");
    m_lom->abeLOMaddRelationResourceIdentifier("URI", "http://data.abuledu.org/URI/501f1858-crabe-tourteau");
    m_lom->abeLOMaddRelationResourceIdentifier("URI", "http://data.abuledu.org/URI/501f1908-crabe-royal");
    m_lom->abeLOMaddRelationResourceDescription("fre",QString::fromUtf8("Présentation de la ressource.\nLà c'est un test avec un lien vers le site de la société."));

    //9.1 & 9.2
    m_lom->abeLOMaddClassificationPurpose("competency",LomLS("fre","ScoLomFr-voc016"),"scolomfr-voc-016-num-0012",LomLS("fre",QString::fromUtf8("compétence 1 - la maîtrise de la langue française/écrire")));
    m_lom->abeLOMaddClassificationPurpose("competency",LomLS("fre","ScoLomFr-voc016"),"scolomfr-voc-016-num-0011",LomLS("fre",QString::fromUtf8("compétence 1 - la maîtrise de la langue française/lire")));
    m_lom->abeLOMaddClassificationPurpose("competency",LomLS("fre","ScoLomFr-voc016"),"scolomfr-voc-016-num-0062",LomLS("fre",QString::fromUtf8("compétence 6 - les compétences sociales et civiques/avoir un comportement responsable")));
    m_lom->abeLOMaddClassificationPurpose("discipline",LomLS("fre","ScoLomFr-voc015"),"scolomfr-voc-015-num-035",LomLS("fre",QString::fromUtf8("domaines d'enseignement du premier degré/français")));
    m_lom->abeLOMaddClassificationPurpose(QString::fromUtf8("public cible détaillé"),LomLS("fre","ScoLomFr-voc021"),"scolomfr-voc-021-num-00231",LomLS("fre",QString::fromUtf8("professeur des écoles/instituteur")));

    //9.3
    m_lom->abeLOMsetClassificationDescription("fre","La description");
    //9.4
    m_lom->abeLOMsetClassificationKeyword("fre",QString::fromUtf8("mot clé"));

    QFile file("dataTests/abuledulomfrv1-demo-miniABULEDU.xml");
    file.open(QIODevice::ReadOnly);
    QByteArray verif(file.read(file.size()));
    file.close();

//    qDebug() << m_lom->abeLOMExportAsXML();

    QFile fileT("/tmp/abuledulomfrv1-demo-miniABULEDU.test.xml");

    if(fileT.open(QIODevice::WriteOnly)) {
        QByteArray temp = m_lom->abeLOMExportAsXML();
        fileT.write(temp, temp.size());
        fileT.close();
    }

    QCOMPARE(m_lom->abeLOMExportAsXML(), verif);
}
