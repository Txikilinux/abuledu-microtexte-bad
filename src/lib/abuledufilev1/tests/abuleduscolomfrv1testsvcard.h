#ifndef TST_LOMTESTS_VCARD_H
#define TST_LOMTESTS_VCARD_H
#include "abuleduvcardcombobox.h"

class abuleduscolomfrv1TestsVCard : public QObject
{
    Q_OBJECT

public:
    explicit abuleduscolomfrv1TestsVCard();


private:
    AbulEduVCardComboBox *m_vcardCombo;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void test_abeGetVCard();

};

#endif // TST_LOMTESTS_VCARD_H
