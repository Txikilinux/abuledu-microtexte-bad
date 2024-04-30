
#include <QtCore/QString>
#include <QtTest/QtTest>

#include "abuledumimetypev1.h"
#include "abuledumimetypev1tests.h"

AbulEduMimeTypeV1Tests::AbulEduMimeTypeV1Tests(QObject *parent) :
    QObject()
{

}

void AbulEduMimeTypeV1Tests::initTestCase()
{
}

void AbulEduMimeTypeV1Tests::cleanupTestCase()
{
}

void AbulEduMimeTypeV1Tests::test_abeMimeTypeGetODT()
{
    QCOMPARE(AbulEduMimeTypeV1::get("odt"), QString("application/vnd.oasis.opendocument.text"));
}

void AbulEduMimeTypeV1Tests::test_abeMimeTypeGetPNG()
{
    QCOMPARE(AbulEduMimeTypeV1::get("png"), QString("image/png"));
}

void AbulEduMimeTypeV1Tests::test_abeMimeTypeGetJPEG()
{
    QCOMPARE(AbulEduMimeTypeV1::get("jpeg"), QString("image/jpeg"));
}

