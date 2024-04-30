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
#include <QtCore/QCoreApplication>
#include "abuleduscolomfrv1.h"

class abuleduscolomfrv1Tests : public QObject
{
    Q_OBJECT

public:
    abuleduscolomfrv1Tests();

private:
    AbulEduSCOLOMFrV1 *m_lom;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();

    void test_abeLOMsetFile();

    //Branche 1: General
    //1.1
    void test_abeLOMgetGeneralIdentifierCatalog();
    void test_abeLOMgetGeneralIdentifierEntry();
    //1.2
    void test_abeLOMgetGeneralTitle();
    //1.3
    void test_abeLOMgetGeneralLanguage();
    //1.4
    void test_abeLOMgetGeneralDescription();
    void test_abeLOMgetGeneralDescriptionEmpty();
    //1.5
    void test_abeLOMgetGeneralKeywords();
    //1.6
    void test_abeLOMgetGeneralCoverage();
    //1.7
    void test_abeLOMgetGeneralStructure();
    //1.8
    void test_abeLOMgetGeneralAggregationLevel();
    //1.9 (LOMFR)
    void test_abeLOMgetGeneralDocumentType();
    //1.10 (SCOLOMFR)
    void test_abeLOMgetGeneralDocumentTypology();

    //Branche 2
    void test_abeLOMgetLifeCycleVersion();
    void test_abeLOMgetLifeCycleStatus();
    void test_abeLOMgetLifeCycleContributionEntities();
    void test_abeLOMaddLifeCycleContributionEntities();
    void test_abeLOMgetLifeCycleContributionDates();

    //Branche 3
    //TODO

    //Branche 4
    void test_abeLOMgetTechnicalFormat();
    void test_abeLOMgetTechnicalSize();
    void test_abeLOMgetTechnicalLocation();

    //Branche 5
    //5.2
    void test_abeLOMgetEducationalLearningResourceTypeWithKeys();
    void test_abeLOMgetEducationalLearningResourceTypeWithValues();
    //5.5
    void test_abeLOMgetEducationalIntendedEndUserRole();
    //5.6
    void test_abeLOMgetEducationalContext();
    //5.7
    void test_abeLOMgetEducationalTypicalAgeRange();
    //5.8
    void test_abeLOMgetEducationalDifficulty();
    //5.10
    void test_abeLOMgetEducationalDescription();
    //5.11
    void test_abeLOMgetEducationalLanguage();
    //----lomfr
    //5.12
    void test_abeLOMgetEducationalActivities();
    //5.13 inconnu
    //----scolomfr
    //5.14
    void test_abeLOMgetEducationalLocation();
    //5.15
    void test_abeLOMgetEducationalPedagogicModality();
    //5.16
    void test_abeLOMgetEducationalTools();

    //Branche 6
    //6.1
    void test_abeLOMgetRightsCost();
    //6.2
    void test_abeLOMgetRightsCopyrightAndOtherRestrictions();
    //6.3
    void test_abeLOMgetRightsDescription();

};
