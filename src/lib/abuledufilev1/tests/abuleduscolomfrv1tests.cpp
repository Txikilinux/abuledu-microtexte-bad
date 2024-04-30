/** Jeu d'essai pour valider la classe LOM
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

abuleduscolomfrv1Tests::abuleduscolomfrv1Tests()
{
}

void abuleduscolomfrv1Tests::initTestCase()
{
    m_lom = new AbulEduSCOLOMFrV1();
    m_lom->abeLOMsetLocalDebug(0);
    QFileInfo fi("dataTests/abuledulomfrv1-demo.xml");
    if(fi.exists()) {
//        qDebug() << "fichier ok : " << fi.absoluteFilePath();
        m_lom->abeLOMsetFile(fi.absoluteFilePath());
    }
    else {
//        qDebug() << "ERROR de fichier";
    }
}

void abuleduscolomfrv1Tests::cleanupTestCase()
{
}

void abuleduscolomfrv1Tests::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void abuleduscolomfrv1Tests::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void abuleduscolomfrv1Tests::test_abeLOMsetFile()
{

}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralIdentifierCatalog()
{
    //remarque, dans ce jeu d'essai on ne devrait avoir que des tests comme celui ci
    m_lom->abeLOMsetGeneralIdentifierCatalog(QString("URI"));
    QCOMPARE(m_lom->abeLOMgetGeneralIdentifierCatalog(), QString("URI"));

//    QCOMPARE(m_lom->abeLOMgetGeneralIdentifierCatalog(), QString("URI"));
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralIdentifierEntry()
{
    QCOMPARE(m_lom->abeLOMgetGeneralIdentifierEntry(), QString("http://mediatheque.abuledu.org/URI/CODETEST"));
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralTitle()
{
    QCOMPARE(m_lom->abeLOMgetGeneralTitle(trUtf8("Français")), QString::fromUtf8("Le titre de cette ressource avec des accents: ééé"));
    QCOMPARE(m_lom->abeLOMgetGeneralTitle("fre"), QString::fromUtf8("Le titre de cette ressource avec des accents: ééé"));
    QCOMPARE(m_lom->abeLOMgetGeneralTitle("eng"), QString("Same title in english"));
    QCOMPARE(m_lom->abeLOMgetGeneralTitle(trUtf8("Anglais")), QString("Same title in english"));
    QCOMPARE(m_lom->abeLOMgetGeneralTitle("spa"), QString(""));
    QCOMPARE(m_lom->abeLOMgetGeneralTitle(trUtf8("Espagnol")), QString(""));
}


void abuleduscolomfrv1Tests::test_abeLOMgetGeneralLanguage()
{
    QStringList langues;
    langues << "fre" << "spa";
    QCOMPARE(m_lom->abeLOMgetGeneralLanguage(), langues);
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralDescription()
{
    QStringList verif;
    verif << QString::fromUtf8("Ceci est la description de la ressource ...");
    QCOMPARE(m_lom->abeLOMgetGeneralDescription("fre"), verif);
}

/** On teste si la description vide est prise en charge */
void abuleduscolomfrv1Tests::test_abeLOMgetGeneralDescriptionEmpty()
{
    QStringList verif("");

    m_lom->abeLOMclearGeneralDescription();
    m_lom->abeLOMsetGeneralDescription("fre","");
    QCOMPARE(m_lom->abeLOMgetGeneralDescription("fre"), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralKeywords()
{
    QStringList verif;
    verif << "sciences de la vie" << "svt" << "biologie humaine" << QString::fromUtf8("caractère sexuel");
    QCOMPARE(m_lom->abeLOMgetGeneralKeywords("fre"), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralCoverage()
{
    QString verif = QString::fromUtf8("lieu, région, époque");

//    qDebug() << " ===================================== On cherche " << m_lom->abeLOMgetGeneralCoverage("fre");

    QCOMPARE(m_lom->abeLOMgetGeneralCoverage("fre"), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralStructure()
{
    QString verif = QString::fromUtf8("LOMv1.0;hierarchical");
    QCOMPARE(m_lom->abeLOMgetGeneralStructure(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralAggregationLevel()
{
    QString verif = QString::fromUtf8("LOMv1.0;2");
    QCOMPARE(m_lom->abeLOMgetGeneralAggregationLevel(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralDocumentType()
{
    QStringList verif;
    verif << QString::fromUtf8("LOMFRv1.0;ensemble de données") << QString::fromUtf8("LOMFRv1.0;ressource interactive");
    QCOMPARE(m_lom->abeLOMgetGeneralDocumentType(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetGeneralDocumentTypology()
{
    QStringList verif;
    verif << QString::fromUtf8("ScoLOMFRv1.0;banque d'images") << QString::fromUtf8("ScoLOMFRv1.0;image numérique") << QString::fromUtf8("ScoLOMFRv1.0;collection de documents");
    QCOMPARE(m_lom->abeLOMgetGeneralDocumentTypology(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetLifeCycleVersion()
{
    QString verif = QString::fromUtf8("Un nom de version ...");
    QCOMPARE(m_lom->abeLOMgetLifeCycleVersion("fre"), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetLifeCycleStatus()
{
    QString verif = QString::fromUtf8("LOMv1.0;final");
    QCOMPARE(m_lom->abeLOMgetLifeCycleStatus(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMaddLifeCycleContributionEntities()
{
    AbulEduSCOLOMFrV1 toto;

    QString role   = QString::fromUtf8("author");
    QDate   ladate = QDate(2011,05,26);

    vCard myvcard;
    vCardProperty name_prop  = vCardProperty::createName("Eric", "Seigne");
    myvcard.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Eric Seigne");
    myvcard.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    myvcard.addProperty(tel_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "eric.seigne@ryxeo.com");
    myvcard.addProperty(email_prop);

    toto.abeLOMaddLifeCycleContributionRole(role, myvcard, ladate);

    QList<vCard> liste;
    liste << myvcard;

    QCOMPARE(toto.abeLOMgetLifeCycleContributionEntities(role,ladate), liste);
}

void abuleduscolomfrv1Tests::test_abeLOMgetLifeCycleContributionEntities()
{
    QString role   = QString::fromUtf8("author");
    QDate   ladate = QDate(2011,05,26);
    QList<vCard> liste = m_lom->abeLOMgetLifeCycleContributionEntities(role,ladate);
    vCard verif;
    vCardProperty name_prop  = vCardProperty::createName("Eric", "Seigne");
    verif.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Eric Seigne");
    verif.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verif.addProperty(tel_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "eric.seigne@ryxeo.com");
    verif.addProperty(email_prop);
    QCOMPARE(liste.at(0).toByteArray(), verif.toByteArray());

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
    liste.clear();
    liste = m_lom->abeLOMgetLifeCycleContributionEntities(role,ladate);
    QCOMPARE(liste.at(1).toByteArray(), verifB.toByteArray());

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
    liste.clear();
    liste = m_lom->abeLOMgetLifeCycleContributionEntities(role,ladate);
    QCOMPARE(liste.at(0).toByteArray(), verifC.toByteArray());
}

void abuleduscolomfrv1Tests::test_abeLOMgetLifeCycleContributionDates()
{
    QString role   = QString::fromUtf8("author");
    QDate   ladate = QDate(2011,05,26);
    vCard verif;
    vCardProperty name_prop  = vCardProperty::createName("Eric", "Seigne");
    verif.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Eric Seigne");
    verif.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verif.addProperty(tel_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "eric.seigne@ryxeo.com");
    verif.addProperty(email_prop);
    m_lom->abeLOMaddLifeCycleContributionRole(role, verif, ladate);

    role = QString::fromUtf8("auteur");
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
    m_lom->abeLOMaddLifeCycleContributionRole(role, verifB, ladate);

    role = QString::fromUtf8("contributeur");
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
    m_lom->abeLOMaddLifeCycleContributionRole(role, verifC, ladate);

    role = QString::fromUtf8("author");
    ladate = QDate(2007,03,13);
    vCard verifD;
    name_prop  = vCardProperty::createName("Jean-Louis", "Frucot");
    verifD.addProperty(name_prop);
    fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Jean-Louis Frucot");
    verifD.addProperty(fn_prop);
    tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    verifD.addProperty(tel_prop);
    email_prop = vCardProperty(VC_EMAIL, "jlf@ryxeo.com");
    verifD.addProperty(email_prop);
    m_lom->abeLOMaddLifeCycleContributionRole(role, verifD, ladate);

    QList<QDate> liste;
    QList<QDate> listeVerif;
    liste << QDate(2007,03,13) << QDate(2011,05,26);

    //1er test avec un role de type clé
    listeVerif = m_lom->abeLOMgetLifeCycleContributionDates("author");
    QCOMPARE(liste, listeVerif);

//    qDebug() << liste;
//    qDebug() << "===========";
//    qDebug() << listeVerif;


    //2° test avec un role de type valeur
    listeVerif.clear();
    listeVerif = m_lom->abeLOMgetLifeCycleContributionDates("auteur");
    QCOMPARE(liste, listeVerif);

    //3° test avec un role de type valeur
    listeVerif.clear();
    liste.clear();
    liste << QDate(2007,03,13);
    listeVerif = m_lom->abeLOMgetLifeCycleContributionDates("contributeur");
//    qDebug() << liste;
//    qDebug() << "===========";
//    qDebug() << listeVerif;
    QCOMPARE(liste, listeVerif);
}


void abuleduscolomfrv1Tests::test_abeLOMgetTechnicalFormat()
{
    QStringList verif;
    verif << "text/html" << "image/jpeg" << "video/msvideo";
    QCOMPARE(m_lom->abeLOMgetTechnicalFormat(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetTechnicalSize()
{
    QString verif("125");
    QCOMPARE(m_lom->abeLOMgetTechnicalSize(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetTechnicalLocation()
{
    QString verif("http://mediatheque.abuledu.org/URI/CODETEST");
    QCOMPARE(m_lom->abeLOMgetTechnicalLocation().at(0), verif);
    verif = "http://www.ryxeo.com/";
    QCOMPARE(m_lom->abeLOMgetTechnicalLocation().at(1), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalLearningResourceTypeWithKeys()
{
    QStringList verif;
    verif << QString::fromUtf8("ScoLOMFRv1.0;cyberquête") << QString::fromUtf8("LOMFRv1.0;démonstration") << QString::fromUtf8("LOMv1.0;slide");
    m_lom->abeLOMclearEducationalLearningResourceType();
    m_lom->abeLOMaddEducationalLearningResourceType(QString::fromUtf8("cyberquête"));
    m_lom->abeLOMaddEducationalLearningResourceType(QString::fromUtf8("démonstration"));
    m_lom->abeLOMaddEducationalLearningResourceType(QString::fromUtf8("slide"));
    QCOMPARE(m_lom->abeLOMgetEducationalLearningResourceType(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalLearningResourceTypeWithValues()
{
    QStringList verif;
    verif << QString::fromUtf8("ScoLOMFRv1.0;cyberquête") << QString::fromUtf8("LOMFRv1.0;démonstration") << QString::fromUtf8("LOMv1.0;slide");
    m_lom->abeLOMclearEducationalLearningResourceType();
    m_lom->abeLOMaddEducationalLearningResourceType(QString::fromUtf8("cyberquête"));
    m_lom->abeLOMaddEducationalLearningResourceType(QString::fromUtf8("démonstration"));
    m_lom->abeLOMaddEducationalLearningResourceType(QString::fromUtf8("diapositive"));
    QCOMPARE(m_lom->abeLOMgetEducationalLearningResourceType(), verif);

    //    verif << "LOMv1.0;simulation" << "LOMFRv1.0;animation" << "LOMv1.0;lecture";
//    QCOMPARE(m_lom->abeLOMgetEducationalLearningResourceType(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalIntendedEndUserRole()
{
    QStringList verif;
    verif << "LOMv1.0;learner" << "LOMv1.0;teacher";
    QCOMPARE(m_lom->abeLOMgetEducationalIntendedEndUserRole(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalContext()
{
    QStringList verif;
    verif << "LOMFRv1.0;enseignement secondaire" << "LOMv1.0;other";
    QCOMPARE(m_lom->abeLOMgetEducationalContext(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalTypicalAgeRange()
{
    QStringList verif;
    verif << "13-18 ans" << "ado" << "jeunes adultes";
    QCOMPARE(m_lom->abeLOMgetEducationalTypicalAgeRange("fre"), verif);

    verif.clear();
    verif << "kids";
    QCOMPARE(m_lom->abeLOMgetEducationalTypicalAgeRange("eng"), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalDifficulty()
{
    QString verif("LOMv1.0;easy");
    QCOMPARE(m_lom->abeLOMgetEducationalDifficulty(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalDescription()
{
    QString verif("Description de l'utilisation de cette ressource ...");
    QCOMPARE(m_lom->abeLOMgetEducationalDescription("fre").at(0), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalLanguage()
{
    QStringList verif;
    verif << "fre";
    QCOMPARE(m_lom->abeLOMgetEducationalLanguage(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalActivities()
{
    QStringList verif;
    verif << "LOMFRv1.0;apprendre" << "LOMFRv1.0;observer" << "LOMFRv1.0;s'informer";
    QCOMPARE(m_lom->abeLOMgetEducationalActivities(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalLocation()
{
    QStringList verif;
    verif << "ScoLOMFRv1.0;en salle de classe" << "ScoLOMFRv1.0;en CDI";
    QCOMPARE(m_lom->abeLOMgetEducationalLocation(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalPedagogicModality()
{
    QStringList verif;
    verif << QString::fromUtf8("ScoLOMFRv1.0;aide didactique individualisée") << "ScoLOMFRv1.0;en autonomie" << QString::fromUtf8("ScoLOMFRv1.0;en groupe de compétences") << QString::fromUtf8("ScoLOMFRv1.0;en classe entière");
    QCOMPARE(m_lom->abeLOMgetEducationalPedagogicModality(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetEducationalTools()
{
    QStringList verif;
    verif << QString::fromUtf8("ScoLOMFRv1.0;vidéoprojecteur") << "ScoLOMFRv1.0;logiciels RIP";
    QCOMPARE(m_lom->abeLOMgetEducationalTools(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetRightsCost()
{
    QString verif("LOMv1.0;yes");
    QCOMPARE(m_lom->abeLOMgetRightsCost(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetRightsCopyrightAndOtherRestrictions()
{
    QString verif("LOMv1.0;yes");
    QCOMPARE(m_lom->abeLOMgetRightsCopyrightAndOtherRestrictions(), verif);
}

void abuleduscolomfrv1Tests::test_abeLOMgetRightsDescription()
{
    QString verif("cc-by-sa");
    QCOMPARE(m_lom->abeLOMgetRightsDescription("fre"), verif);
}

//QTEST_MAIN(lomTests)
//#include "tst_lomtests.moc"
