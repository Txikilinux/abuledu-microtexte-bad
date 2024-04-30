#ifndef ABULEDUMIMETYPEV1TESTS_H
#define ABULEDUMIMETYPEV1TESTS_H

#include <abuledumimetypev1.h>

class AbulEduMimeTypeV1Tests : public QObject
{
    Q_OBJECT
public:
    explicit AbulEduMimeTypeV1Tests(QObject *parent = 0);
    
signals:
    
public slots:
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void test_abeMimeTypeGetODT();
    void test_abeMimeTypeGetPNG();
    void test_abeMimeTypeGetJPEG();

};

#endif // ABULEDUMIMETYPEV1TESTS_H
