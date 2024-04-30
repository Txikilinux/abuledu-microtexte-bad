#include "abuledufilev1tests.h"

//* @todo dossier thumbs, data, conf

void AbulEduFileV1Tests::initTestCase()
{
    m_abuleduFile = new AbulEduFileV1();

    /* Ouverture d'un abe de démo */
    QVERIFY(m_abuleduFile->abeFileOpen("dataTests/abuledufilev1-demo.abe"));
}

void AbulEduFileV1Tests::cleanupTestCase()
{
    /* Test du nettoyage du dossier à la destruction de l'abeFile */
    QDir temp(m_abuleduFile->abeFileGetDirectoryTemp());
    delete m_abuleduFile;
    QVERIFY(!temp.exists());
}

void AbulEduFileV1Tests::test_abeUniqIDTemp()
{
    QVERIFY(!m_abuleduFile->abeUniqIDTemp().isEmpty());
}

void AbulEduFileV1Tests::test_abeFileGetDirectoryTemp()
{
    QVERIFY(m_abuleduFile->abeFileGetDirectoryTemp().exists());
}

void AbulEduFileV1Tests::test_MetaInf()
{
    QVERIFY(QDir(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/META-INF").exists());
}

void AbulEduFileV1Tests::test_fileAbulEdu()
{
    QVERIFY(QFile::exists(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/abuledu"));
}

void AbulEduFileV1Tests::test_fileContentXML()
{
    QVERIFY(QFile::exists(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/content.xml"));
}

void AbulEduFileV1Tests::test_fileLomXML()
{
    QVERIFY(QFile::exists(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/lom.xml"));
}

void AbulEduFileV1Tests::test_fileMetaXML()
{
    QVERIFY(QFile::exists(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/meta.xml"));
}

void AbulEduFileV1Tests::test_fileMimetype()
{
    QVERIFY(QFile::exists(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/mimetype"));
}

void AbulEduFileV1Tests::test_fileManifest()
{
    QVERIFY(QFile::exists(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/manifest.rdf"));
}

void AbulEduFileV1Tests::test_fileVersion()
{
    QVERIFY(QFile::exists(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath() + "/version"));
}

void AbulEduFileV1Tests::test_abeCleanDirectoryRecursively()
{
    QDir erasedDir = m_abuleduFile->abeFileGetDirectoryTemp();
    AbulEduFileV1::abeCleanDirectoryRecursively(m_abuleduFile->abeFileGetDirectoryTemp().absolutePath());
    QVERIFY(!erasedDir.exists());
}

/**************************************************************************************
                                 GETTERS & SETTERS
  ************************************************************************************/

void AbulEduFileV1Tests::test_abeFileSetFilename()
{
    m_abuleduFile->abeFileSetFilename("monAbulEduFile");
    QCOMPARE(m_abuleduFile->abeFileGetFileName().fileName(), QString("monAbulEduFile"));
}

void AbulEduFileV1Tests::test_abeFileSetIdentifier()
{
    m_abuleduFile->abeFileSetIdentifier("monID");
    QCOMPARE(m_abuleduFile->abeFileGetIdentifier(), QString("monID"));
}

void AbulEduFileV1Tests::test_abeFileSetCreator()
{
    m_abuleduFile->abeFileSetCreator("Jean-Jacques");
    QCOMPARE(m_abuleduFile->abeFileGetCreator(), QString("Jean-Jacques"));
}

void AbulEduFileV1Tests::test_abeFileSetTitle()
{
    m_abuleduFile->abeFileSetTitle("Mon titre");
    QCOMPARE(m_abuleduFile->abeFileGetTitle(), QString("Mon titre"));
}

void AbulEduFileV1Tests::test_abeFileSetRights()
{
    m_abuleduFile->abeFileSetRights("cc-by-sa");
    QCOMPARE(m_abuleduFile->abeFileGetRights(), QString("cc-by-sa"));
}

void AbulEduFileV1Tests::test_abeFileSetDescription()
{
    m_abuleduFile->abeFileSetDescription("ma description de test, pour tester cette fonctionnalite");
    QCOMPARE(m_abuleduFile->abeFileGetDescription(), QString("ma description de test, pour tester cette fonctionnalite"));
}

void AbulEduFileV1Tests::test_abeFileGetPublisher()
{
    m_abuleduFile->abeFileSetPublisher("el publicator");
    QCOMPARE(m_abuleduFile->abeFileGetPublisher(), QString("el publicator"));
}

void AbulEduFileV1Tests::test_abeFileGetContributor()
{
    m_abuleduFile->abeFileSetContributor("el contributor");
    QCOMPARE(m_abuleduFile->abeFileGetContributor(), QString("el contributor"));
}

void AbulEduFileV1Tests::test_abeFileGetKeywords()
{
    m_abuleduFile->abeFileSetKeywords("mon mot clef");
    QCOMPARE(m_abuleduFile->abeFileGetKeywords(), QString("mon mot clef"));
}

void AbulEduFileV1Tests::test_abeFileGetFormat()
{
    m_abuleduFile->abeFileSetFormat("mon format kivabien");
    QCOMPARE(m_abuleduFile->abeFileGetFormat(), QString("mon format kivabien"));
}

void AbulEduFileV1Tests::test_abeFileGetSource()
{
    m_abuleduFile->abeFileSetSource("ma belle source");
    QCOMPARE(m_abuleduFile->abeFileGetSource(), QString("ma belle source"));
}

void AbulEduFileV1Tests::test_abeFileGetRelation()
{
    m_abuleduFile->abeFileSetRelation("ma relation avec celui-ci");
    QCOMPARE(m_abuleduFile->abeFileGetRelation(), QString("ma relation avec celui-ci"));
}

void AbulEduFileV1Tests::test_abeFileGetCoverage()
{
    m_abuleduFile->abeFileSetCoverage("ma couverture");
    QCOMPARE(m_abuleduFile->abeFileGetCoverage(), QString("ma couverture"));
}

void AbulEduFileV1Tests::test_abeFileGetLOM()
{
    /* On vérifie si le QSharedPointer est instancié (i.e. = true) */
    QVERIFY(m_abuleduFile->abeFileGetLOM());
}

void AbulEduFileV1Tests::test_abeFileGetType()
{
    m_abuleduFile->abeFileSetType("mon type");
    QCOMPARE(m_abuleduFile->abeFileGetType(), QString("mon type"));
}

void AbulEduFileV1Tests::test_abeFileGetMD5()
{
    /* le MD5 du fichier ABE demo "ee926929a1c374974ac45a1ad6616316" */
    m_abuleduFile->abeFileSetFilename("dataTests/abuledufilev1-demo.abe");
    QCOMPARE(m_abuleduFile->abeFileGetMD5(), QString("ee926929a1c374974ac45a1ad6616316"));
}

void AbulEduFileV1Tests::test_abeFileGetBase64Encoded()
{
    /* le base64 du fichier "dataTests/abuledu" "bGV0ZXJyaWVyLWltYWdlbw=="*/
    m_abuleduFile->abeFileSetFilename("dataTests/abuledu");
    QCOMPARE(m_abuleduFile->abeFileGetBase64Encoded(), QString("bGV0ZXJyaWVyLWltYWdlbw=="));
}

void AbulEduFileV1Tests::test_abeFileGetUnitVersion()
{
    /* le base64 du fichier "dataTests/abuledufilev1-demo/abuledu" "ee926929a1c374974ac45a1ad6616316"*/
    m_abuleduFile->abeFileSetUnitVersion("1.0.0");
    QCOMPARE(m_abuleduFile->abeFileGetUnitVersion(), QString("1.0.0"));
}
