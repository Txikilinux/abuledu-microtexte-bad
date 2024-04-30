#ifndef ABULEDUFILEV1TESTS_H
#define ABULEDUFILEV1TESTS_H

#include <QTest>
#include "abuledufilev1.h"

class AbulEduFileV1Tests : public QObject
{
    Q_OBJECT

private:
    AbulEduFileV1 *m_abuleduFile;

private Q_SLOTS:

    /** Dezippage d'un abe pour test d'intégrité @see dataTests/abuledufile.demo*/
    void initTestCase();

    /** Test de la présence su dossier temporaire */
    void cleanupTestCase();

    /** On test si la chaine retournée n'est pas vide */
    void test_abeUniqIDTemp();

    /** Test de l'existence du dossier temporaire retourné */
    void test_abeFileGetDirectoryTemp();

    /** Test présence dossier "META-INF" */
    void test_MetaInf();

    /** Test présence fichier "abuledu" */
    void test_fileAbulEdu();

    /** Test présence fichier "content.xml" */
    void test_fileContentXML();

    /** Test présence fichier "lom.xml" */
    void test_fileLomXML();

    /** Test présence fichier "meta.xml" */
    void test_fileMetaXML();

    /** Test présence fichier "mimetype" */
    void test_fileMimetype();

    /** Test présence fichier "manifest.rdf" */
    void test_fileManifest();

    /** Test présence fichier "version" */
    void test_fileVersion();


    /***********************************************
                Getters & Setters
      *********************************************/

    /** Test nettoyage recursif de dossier */
    void test_abeCleanDirectoryRecursively();

    /** Test Getter & Setter du nom AbeFile */
    void test_abeFileSetFilename();

    /** Test Getter & Setter identifier AbeFile */
    void test_abeFileSetIdentifier();

    /** Test Getter & Setter creator AbeFile */
    void test_abeFileSetCreator();

    /** Test Getter & Setter title AbeFile */
    void test_abeFileSetTitle();

    /** Test Getter & Setter rights AbeFile */
    void test_abeFileSetRights();

    /** Test Getter & Setter description AbeFile */
    void test_abeFileSetDescription();

    /** Test Getter & Setter publisher AbeFile */
    void test_abeFileGetPublisher();

    /** Test Getter & Setter publisher AbeFile */
    void test_abeFileGetContributor();

    /** Test Getter & Setter keywords AbeFile */
    void test_abeFileGetKeywords();

    /** Test Getter & Setter format AbeFile */
    void test_abeFileGetFormat();

    /** Test Getter & Setter source AbeFile */
    void test_abeFileGetSource();

    /** Test Getter & Setter relation AbeFile */
    void test_abeFileGetRelation();

    /** Test Getter & Setter coverturage AbeFile */
    void test_abeFileGetCoverage();

    /** Test Getter & Setter lom AbeFile */
    void test_abeFileGetLOM();

    /** Test Getter & Setter type AbeFile */
    void test_abeFileGetType();

    /** Test Getter & Setter MD5 AbeFile */
    void test_abeFileGetMD5();

    /** Test Getter & Setter base64 AbeFile */
    void test_abeFileGetBase64Encoded();

    /** Test Getter & Setter unitVersion AbeFile */
    void test_abeFileGetUnitVersion();
};

#endif // ABULEDUFILEV1TESTS_H
