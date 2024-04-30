#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include "abuleduscolomfrv1testsvcard.h"

abuleduscolomfrv1TestsVCard::abuleduscolomfrv1TestsVCard()
{

}

void abuleduscolomfrv1TestsVCard::initTestCase()
{
    m_vcardCombo = new AbulEduVCardComboBox();
}

void abuleduscolomfrv1TestsVCard::cleanupTestCase()
{
}

void abuleduscolomfrv1TestsVCard::test_abeGetVCard()
{
    vCard myvcard;
    vCardProperty name_prop  = vCardProperty::createName("Eric", "Seigne");
    myvcard.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, "Eric Seigne");
    myvcard.addProperty(fn_prop);
    vCardProperty tel_prop   = vCardProperty(VC_TELEPHONE, "05 35 54 01 18");
    myvcard.addProperty(tel_prop);
    vCardProperty email_prop = vCardProperty(VC_EMAIL, "eric.seigne@ryxeo.com");
    myvcard.addProperty(email_prop);

    m_vcardCombo->abeAddVCard(myvcard);

    QCOMPARE(m_vcardCombo->abeGetVCard("Eric Seigne"), myvcard);
}

