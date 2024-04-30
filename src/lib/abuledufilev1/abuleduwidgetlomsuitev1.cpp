/** Classe AbulEduWidgetLOMSuiteV1
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/Format_LOM_LOMFR_ScoLOMFR
  * @see http://www.lom-fr.fr/lomfr-unt/unisciel/modeles-de-fiche/LOMFRTemplate-Unisciel-v1.0-complet.xml/view
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2012 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program. If not, see <http://www.gnu.org/licenses/>.
  */

#include "abuleduwidgetlomsuitev1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduWidgetLOMSuiteV1::AbulEduWidgetLOMSuiteV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduWidgetLOMSuiteV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    /* Masquage des éléments non requis pour l'instant */
    hideUnused(true);
    setFont(abeApp->font());
    ui->glLOM5_10->setAlignment(ui->cbLangLOM5_10,Qt::AlignTop);
    ui->glLOM5_10->setAlignment(ui->btnPlusLOM5_10,Qt::AlignTop);

    m_numberKeyWord = 1;
    m_numberCheminTaxon = 1;
    m_numberLearningRessourceType = 0;
    m_numberEducationalContext = 0;
    m_numberEducationalTypicalAgeRange = 0;
    m_numberEducationalDescriptions = 0;
    m_numberEducationalActivities = 0;
    m_numberEducationalValidations = 0;
    m_numberEducationalLocations = 0;
    m_numberEducationalPedagogicModalities = 0;
    m_numberEducationalTools = 0;
    m_previousEducationalTypicalAgeRange = "0-0";
}

AbulEduWidgetLOMSuiteV1::~AbulEduWidgetLOMSuiteV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduWidgetLOMSuiteV1::abeWLOMsetABELOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abeLomFrV1 = scoLomFrV1;

    /* Section 5.2 Type de ressource pédagogique */
    ABULEDU_LOG_DEBUG() << "5.2 Type de ressource pédagogique"
                        << m_abeLomFrV1->abeLOMgetEducationalLearningResourceType();

    if (m_abeLomFrV1->abeLOMgetEducationalLearningResourceType().count() == 0)
    {
        addCBLearningRessourceType();
    }
    else
    {
        foreach(QString type,m_abeLomFrV1->abeLOMgetEducationalLearningResourceType())
        {
            type.remove("ScoLOMFRv1.0;");
            type.remove("LOMFRv1.0;");
            type.remove("LOMv1.0;");
            addCBLearningRessourceType(type);
        }
    }

    /* Section 5.6 Niveau du public cible */
    ABULEDU_LOG_DEBUG() << "5.6 Niveau du public cible"
                        << m_abeLomFrV1->abeLOMgetEducationalContext();
    if (m_abeLomFrV1->abeLOMgetEducationalContext().count() == 0)
    {
        addCBEducationalContext();
    }
    else
    {
        foreach(QString type,m_abeLomFrV1->abeLOMgetEducationalContext())
        {
            type.remove("LOMFRv1.0;");
            type.remove("LOMv1.0;");
            addCBEducationalContext(type);
        }
    }

    /* Section 5.7  Tranche d'âge */
    ui->cbLangLOM5_7->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
    connect(ui->cbLangLOM5_7, SIGNAL(currentIndexChanged(int)), this,SLOT(abeWLOMChangeLang5_7(int)), Qt::UniqueConnection);
    ABULEDU_LOG_DEBUG() << "5.7 Tranche d'âge";
    foreach(QString lang, m_abeLomFrV1->abeLOMgetEducationalTypicalAgeRangeKeys())
    {
        ABULEDU_LOG_DEBUG()<<m_abeLomFrV1->abeLOMgetEducationalTypicalAgeRange(lang);
    }

    if (!m_abeLomFrV1->abeLOMgetEducationalTypicalAgeRangeKeys().count() == 0)
    {
        foreach(QString lang,m_abeLomFrV1->abeLOMgetEducationalTypicalAgeRangeKeys())
        {
            foreach(QString range,m_abeLomFrV1->abeLOMgetEducationalTypicalAgeRange(lang))
            {
                addEducationalTypicalAgeRange(lang,range);
            }
        }
    }
    else
    {
        addEducationalTypicalAgeRange();
    }

    /* Section 5.10 Description */
    ui->cbLangLOM5_10->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
    connect(ui->cbLangLOM5_10, SIGNAL(currentIndexChanged(int)), this,SLOT(abeWLOMChangeLang5_10(int)), Qt::UniqueConnection);
    ABULEDU_LOG_DEBUG() <<"5.10 Description éducative";
    foreach(QString lang, m_abeLomFrV1->abeLOMgetEducationalDescriptionKeys())
    {
        ABULEDU_LOG_DEBUG()<<m_abeLomFrV1->abeLOMgetEducationalDescription(lang);
    }
    if (!m_abeLomFrV1->abeLOMgetEducationalDescriptionKeys().count() == 0)
    {
        foreach(QString lang,m_abeLomFrV1->abeLOMgetEducationalDescriptionKeys())
        {
            foreach(QString description,m_abeLomFrV1->abeLOMgetEducationalDescription(lang))
            {
                addEducationalDescription(lang,description);
            }
        }
    }
    else
    {
        addEducationalDescription();
    }

    /* Section 5.12 Activités induites */
    ABULEDU_LOG_DEBUG() << "5.12 Activités induites"
                        << m_abeLomFrV1->abeLOMgetEducationalActivities();

    if (m_abeLomFrV1->abeLOMgetEducationalActivities().count() == 0)
    {
        addCBEducationalActivities();
    }
    else
    {
        foreach(QString activity,m_abeLomFrV1->abeLOMgetEducationalActivities())
        {
            activity.remove("ScoLOMFRv1.0;");
            activity.remove("LOMFRv1.0;");
            addCBEducationalActivities(activity);
        }
    }

    /* Section 5.13 Validation */
    ui->cbLangLOM5_13->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
    connect(ui->cbLangLOM5_13, SIGNAL(currentIndexChanged(int)), this,SLOT(abeWLOMChangeLang5_13(int)), Qt::UniqueConnection);
    ABULEDU_LOG_DEBUG()<<"5.13 Validation";
    foreach(QString lang, m_abeLomFrV1->abeLOMgetEducationalValidationKeys())
    {
        ABULEDU_LOG_DEBUG() << m_abeLomFrV1->abeLOMgetEducationalValidation(lang);
    }

    if (!m_abeLomFrV1->abeLOMgetEducationalValidationKeys().count() == 0)
    {
        foreach(QString lang,m_abeLomFrV1->abeLOMgetEducationalValidationKeys())
        {
            foreach(QString validation,m_abeLomFrV1->abeLOMgetEducationalValidation(lang))
            {
                addEducationalValidation(lang,validation);
            }
        }
    }
    else
    {
        addEducationalValidation();
    }

    /* Section 5.14 Lieu */
    ABULEDU_LOG_DEBUG() << "5.14 Lieu"
             << m_abeLomFrV1->abeLOMgetEducationalLocation();

    if (m_abeLomFrV1->abeLOMgetEducationalLocation().count() == 0)
    {
        addCBEducationalLocation();
    }
    else
    {
        foreach(QString location,m_abeLomFrV1->abeLOMgetEducationalLocation())
        {
            location.remove("ScoLOMFRv1.0;");
            addCBEducationalLocation(location);
        }
    }

    /* Section 5.15 Modalités pédagogiques */
    ABULEDU_LOG_DEBUG() << "5.15 Modalités pédagogiques"
                        << m_abeLomFrV1->abeLOMgetEducationalPedagogicModality();

    if (m_abeLomFrV1->abeLOMgetEducationalPedagogicModality().count() == 0)
    {
        addCBEducationalPedagogicModality();
    }
    else
    {
        foreach(QString modality,m_abeLomFrV1->abeLOMgetEducationalPedagogicModality())
        {
            modality.remove("ScoLOMFRv1.0;");
            addCBEducationalPedagogicModality(modality);
        }
    }

    /* Section 5.16 Outils */
    ui->cbLOM5_16->addItems(m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values());
    ABULEDU_LOG_DEBUG() << "5.16 Outils"
                        << m_abeLomFrV1->abeLOMgetEducationalTools();

    if (m_abeLomFrV1->abeLOMgetEducationalTools().count() == 0)
    {
        addCBEducationalTool();
    }
    else
    {
        foreach(QString tool,m_abeLomFrV1->abeLOMgetEducationalTools())
        {
            tool.remove("ScoLOMFRv1.0;");
            addCBEducationalTool(tool);
        }
    }


    /* Section 6.1 Coût */
    ui->cbLOM6_1->addItems(m_abeLomFrV1->abeLOMgetRightsCostVocabLOM().values());
    if (!m_abeLomFrV1->abeLOMgetRightsCost().isEmpty())
    {
        int i=m_abeLomFrV1->abeLOMgetRightsCostVocabLOM().count();
        while (ui->cbLOM6_1->currentText() != m_abeLomFrV1->abeLOMgetRightsCostVocabLOM().value(m_abeLomFrV1->abeLOMgetRightsCost().remove("LOMv1.0;")) && i>0)
        {
            i--;
            ui->cbLOM6_1->blockSignals(true);
            ui->cbLOM6_1->setCurrentIndex(i);
            ui->cbLOM6_1->blockSignals(false);
        }
        ABULEDU_LOG_DEBUG() <<" RightsCoast affiche : "<<ui->cbLOM6_1->currentText();
    }
    else
    {
        ui->cbLOM6_1->setCurrentIndex(0);
    }

    /* Section 6.2 Copyright et autres restrictions */
    ui->cbLOM6_2->addItems(m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM().values());
    if (!m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictions().isEmpty())
    {
        int i=m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM().count();
        while (ui->cbLOM6_2->currentText() != m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM().value(m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictions().remove("LOMv1.0;")) && i>0)
        {
            i--;
            ui->cbLOM6_2->blockSignals(true);
            ui->cbLOM6_2->setCurrentIndex(i);
            ui->cbLOM6_2->blockSignals(false);
        }
        ABULEDU_LOG_DEBUG() << " RightsCopyright affiche : "<<ui->cbLOM6_2->currentText();
    }
    else
    {
        ui->cbLOM6_2->setCurrentIndex(0);
    }

    /* Section 6.3 Description des droits */
    ui->cbLangLOM6_3->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
    abeWLOMChangeLang6_3(0);
    connect(ui->cbLangLOM6_3, SIGNAL(currentIndexChanged(int)),this, SLOT(abeWLOMChangeLang6_3(int)), Qt::UniqueConnection);
    if (!m_abeLomFrV1->abeLOMgetRightsDescriptionKeys().isEmpty())
    {
        QString lang = m_abeLomFrV1->abeLOMgetRightsDescriptionKeys().first();
        int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
        while (ui->cbLangLOM6_3->currentText() != lang && i>0)
        {
            i--;
            ui->cbLangLOM6_3->blockSignals(true);
            ui->cbLangLOM6_3->setCurrentIndex(i);
            ui->cbLangLOM6_3->blockSignals(false);
        }
        ui->teLOM6_3->setText(m_abeLomFrV1->abeLOMgetRightsDescription(lang));
        ABULEDU_LOG_DEBUG()<<" RightsDescription affiche : "<<ui->teLOM6_3->toPlainText();
    }

    /* Section 9.1 Objectif -> non utilisé à ce jour */
    ui->cbLOM9_1->addItems(m_abeLomFrV1->abeLOMgetClassificationPurposeVocabSCOLOMFR().values());
    ui->cbLOM9_1->addItems(m_abeLomFrV1->abeLOMgetClassificationPurposeVocabLOMFR().values());
    ui->cbLOM9_1->addItems(m_abeLomFrV1->abeLOMgetClassificationPurposeVocabLOM().values());
    if (!m_abeLomFrV1->abeLOMgetClassificationPurpose(2).isEmpty())
    {
        int i=m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM().count();
        while (ui->cbLOM6_2->currentText() != m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictionsVocabLOM().value(m_abeLomFrV1->abeLOMgetRightsCopyrightAndOtherRestrictions().remove("LOMv1.0;")) && i>0)
        {
            i--;
            ui->cbLOM6_2->blockSignals(true);
            ui->cbLOM6_2->setCurrentIndex(i);
            ui->cbLOM6_2->blockSignals(false);
        }
        ABULEDU_LOG_DEBUG()<<" RightsCopyright affiche : "<<ui->cbLOM6_2->currentText();
    }
    else
    {
        ui->cbLOM9_1->setCurrentIndex(0);
    }

    /* Section 9.4 Mot clé */
    ui->cbLOM9_4->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
    abeWLOMSuiteDisplayAuthenticated(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus());
}

//! Icham 13/03/2013
//! @todo = A verifier mais il me semble qu'il y a 10 boucles for identiques (signalées par des commentaires)..
//! Il y a surement de la factorisation à effectuer sur cette méthode
void AbulEduWidgetLOMSuiteV1::saveOnExit()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Sauvegarde des types de ressources pédagogiques 5.2 */
    m_abeLomFrV1->abeLOMclearEducationalLearningResourceType();
    m_abeLomFrV1->abeLOMaddEducationalLearningResourceType(ui->cbLOM5_2->currentText());
    //! 1ere boucle for
    for (int i= m_addedWidgets.count()-1; i>=0 ;i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;

        if (m_addedWidgets[i].first == "QComboBoxLearningRessourceType")
        {
            m_abeLomFrV1->abeLOMaddEducationalLearningResourceType(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
        }
    }

    /* Sauvegarde des Niveaux de public ciblé 5.6 */
    m_abeLomFrV1->abeLOMclearEducationalContext();
    m_abeLomFrV1->abeLOMaddEducationalContext(ui->cbLOM5_6->currentText());
    ABULEDU_LOG_DEBUG() << "Educational context "<<ui->cbLOM5_6->currentText()
                        << m_abeLomFrV1->abeLOMgetEducationalContext();

    //! 2eme boucle for identique
    for (int i=m_addedWidgets.count()-1; i>=0; i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;

        if (m_addedWidgets[i].first == "QComboBoxEducationalContext")
        {
            m_abeLomFrV1->abeLOMaddEducationalContext(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
            ABULEDU_LOG_DEBUG() << "j'ai écrit pour Educational context "<<static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText()
                                << m_abeLomFrV1->abeLOMgetEducationalContext();

        }
    }

    /* Sauvegarde des tranches d'âge 5.7 */
    m_abeLomFrV1->abelomclearEducationalTypicalAgeRange();
    QString builtRange = QString::number(ui->sbMinLOM5_7->value())+"-"+QString::number(ui->sbMaxLOM5_7->value());
    ABULEDU_LOG_DEBUG() <<"langue : "<<ui->cbLangLOM5_7->currentText()
                        <<"EducationalAgeRange : "<<builtRange;

    qDebug()<<m_abeLomFrV1->abeLOMaddEducationalTypicalAgeRange(ui->cbLangLOM5_7->currentText(),builtRange);
    //! 3eme boucle for identique
    for (int i=m_addedWidgets.count()-1; i >= 0; i--)
    {
        ABULEDU_LOG_DEBUG() << "Widget "<<i<<" : "<<m_addedWidgets[i].second->objectName();
        if (m_addedWidgets[i].first == "QComboBoxLangRange")
        {
            QString langue = static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText();
            int minLu = static_cast<QSpinBox*> (m_addedWidgets[i-2].second)->value();
            int maxLu = static_cast<QSpinBox*> (m_addedWidgets[i-1].second)->value();
            QString valeur = QString::number(minLu)+"-"+QString::number(maxLu);
            ABULEDU_LOG_DEBUG() << "langue : "<<langue
                     << "EducationalAgeRange : "<<valeur
                     << m_abeLomFrV1->abeLOMaddEducationalTypicalAgeRange(langue,valeur);
        }
        else
        {
            ABULEDU_LOG_DEBUG()<<"N'est pas un QComboBoxLangRange";
        }
    }

    /* Sauvegarde des tranches d'âge 5.10 */
    m_abeLomFrV1->abeLOMclearEducationalDescription();
    ABULEDU_LOG_DEBUG() <<"langue : "<<ui->cbLangLOM5_10->currentText()
                        <<"EducationalDescription : "<<ui->teLOM5_10->toPlainText()
                        << m_abeLomFrV1->abeLOMaddEducationalDescription(ui->cbLangLOM5_10->currentText(),ui->teLOM5_10->toPlainText());
    //! 4eme boucle for identique
    for (int i=m_addedWidgets.count()-1; i>=0 ;i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].second->objectName();

        if (m_addedWidgets[i].first == "QComboBoxDescription")
        {
            QString langue = static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText();
            QString valeur = static_cast<QTextEdit*> (m_addedWidgets[i-1].second)->toPlainText();
            ABULEDU_LOG_DEBUG() << "langue : "<<langue
                                <<"EducationalDescription : "<<valeur
                                << m_abeLomFrV1->abeLOMaddEducationalDescription(langue,valeur);
        }
        else
        {
            ABULEDU_LOG_DEBUG() << "N'est pas un QComboBoxDescription";
        }
    }

    /* Sauvegarde des activités induites 5.12 */
    m_abeLomFrV1->abeLOMclearEducationalActivities();
    m_abeLomFrV1->abeLOMaddEducationalActivities(ui->cbLOM5_12->currentText());
    for (int i=m_addedWidgets.count()-1;i>=0;i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;

        if (m_addedWidgets[i].first == "QComboBoxEducationalActivity")
        {
            m_abeLomFrV1->abeLOMaddEducationalActivities(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
        }
    }

    /* Sauvegarde des validations 5.13 */
    m_abeLomFrV1->abeLOMclearEducationalValidations();

    ABULEDU_LOG_DEBUG() << "langue : "<<ui->cbLangLOM5_13->currentText()
             << "EducationalValidation : "<<ui->leLOM5_13->text()
             << m_abeLomFrV1->abeLOMaddEducationalValidation(ui->cbLangLOM5_13->currentText(),ui->leLOM5_13->text());
    //! 5eme boucle for identique
    for (int i=m_addedWidgets.count()-1; i>=0 ; i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].second->objectName();
        if (m_addedWidgets[i].first == "QComboBoxValidation")
        {
            QString langue = static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText();
            QString valeur = static_cast<QLineEdit*> (m_addedWidgets[i-1].second)->text();
            ABULEDU_LOG_DEBUG() << "langue : "<<langue
                                << "EducationalValidation : "<<valeur
                                << m_abeLomFrV1->abeLOMaddEducationalValidation(langue,valeur);
        }
        else
        {
            ABULEDU_LOG_DEBUG()<<"N'est pas un QComboBoxValidation";
        }
    }

    /* Sauvegarde des lieux 5.14 */
    m_abeLomFrV1->abeLOMclearEducationalLocation();
    m_abeLomFrV1->abeLOMaddEducationalLocation(ui->cbLOM5_14->currentText());
    //! 6eme boucle for identique
    for (int i=m_addedWidgets.count()-1; i>=0 ; i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;
        if (m_addedWidgets[i].first == "QComboBoxEducationalLocation")
        {
            m_abeLomFrV1->abeLOMaddEducationalLocation(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
        }
    }

    /* Sauvegarde des modalités pédagogiques 5.15 */
    m_abeLomFrV1->abeLOMclearEducationalPedagogicModality();
    m_abeLomFrV1->abeLOMaddEducationalPedagogicModality(ui->cbLOM5_15->currentText());
    //! 7eme boucle for identique
    for (int i=m_addedWidgets.count()-1;i>=0;i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;

        if (m_addedWidgets[i].first == "QComboBoxEducationalPedagogicModality")
        {
            m_abeLomFrV1->abeLOMaddEducationalPedagogicModality(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
        }
    }

    /* Sauvegarde des outils 5.16 */
    m_abeLomFrV1->abeLOMclearEducationalTools();
    m_abeLomFrV1->abeLOMaddEducationalTools(ui->cbLOM5_16->currentText());
    //! 8eme boucle for identique
    for (int i=m_addedWidgets.count()-1;i>=0;i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;

        if (m_addedWidgets[i].first == "QComboBoxEducationalTool")
        {
            m_abeLomFrV1->abeLOMaddEducationalTools(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
        }
    }

    /* Section 6.1 Coût */
    m_abeLomFrV1->abeLOMsetRightsCost(ui->cbLOM6_1->currentText());

    /* Section 6.2 Copyright et autres restrictions */
    m_abeLomFrV1->abeLOMsetRightsCopyrightAndOtherRestrictions(ui->cbLOM6_2->currentText());

    /* Section 6.3 Description des droits */
    m_abeLomFrV1->abeLOMsetRightsDescription(ui->cbLangLOM6_3->currentText(),ui->teLOM6_3->toPlainText());

    /* Section 9.2 Chemin taxon */
    ui->abeWidgetTaxon->getInfoTaxon();
    //! 9eme boucle for identique
    for (int i=m_addedWidgets.count()-1;i>=0;i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;
        if (m_addedWidgets[i].first == "AbulEduWidgetTaxon")
        {
            static_cast<AbulEduWidgetTaxon*> (m_addedWidgets[i].second)->getInfoTaxon();
        }
    }
    /* Section 9.4 Mot-clé */
    m_abeLomFrV1->abeLOMsetClassificationKeyword(ui->cbLOM9_4->currentText(),ui->leLOM9_4->text());
    //! 10eme boucle for identique
    for (int i=m_addedWidgets.count()-1 ; i>0 ; i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;
        if (m_addedWidgets[i].first == "QComboBoxKeyword")
        {
            QString langue = static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText();
            QString valeur = static_cast<QLineEdit*> (m_addedWidgets[i-1].second)->text();
            m_abeLomFrV1->abeLOMsetClassificationKeyword(langue,valeur);
        }
    }
}


void AbulEduWidgetLOMSuiteV1::on_btnAuthenticate_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduSSOLoginV1* logWidget = new AbulEduSSOLoginV1();
    connect(logWidget, SIGNAL(signalSSOAuthDone(int)),this,SLOT(abeWLOMSuiteDisplayAuthenticated(int)), Qt::UniqueConnection);
    logWidget->setAttribute(Qt::WA_StyleSheet);
    logWidget->show();
}

void AbulEduWidgetLOMSuiteV1::abeWLOMSuiteDisplayAuthenticated(int returnAuthenticated)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool isAuthenticated;
    if (returnAuthenticated == 1)
        isAuthenticated = true;
    else isAuthenticated = false;
    ui->frAuthenticate->setVisible(!isAuthenticated);

}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM9_2_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addGBtaxon();
}

void AbulEduWidgetLOMSuiteV1::addGBtaxon(QString taxon)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_numberCheminTaxon++;
    if (taxon.isNull())
    {
        if (m_numberCheminTaxon == 1)
        {
            ui->abeWidgetTaxon->setLEsourceText("Ne doit pas arriver");
        }
        else
        {
            AbulEduWidgetTaxon* taxon = new AbulEduWidgetTaxon(0,m_numberCheminTaxon);
            ui->verticalLayout->addWidget(taxon);
            m_addedWidgets << QPair<QString,QWidget*>("AbulEduWidgetTaxon", taxon);
        }
    }
}

void AbulEduWidgetLOMSuiteV1::hideUnused(bool yesNo)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->cbLOM9_1->setHidden(yesNo);
    ui->lblLOM9_1->setHidden(yesNo);
    ui->lblAcacher->setHidden(yesNo);
    ui->btnPlusLOM9_2->setHidden(yesNo);
    ui->lblLOM9_3->setHidden(yesNo);
    ui->leLOM9_3->setHidden(yesNo);
    ui->abeWidgetTaxon->setHidden(yesNo);
    ui->frLOM9->setHidden(yesNo);
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM9_4_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduWidgetLOMSuiteV1::abeWLOMChangeLang5_7(int index)
{
    QString lang = ui->cbLangLOM5_7->itemText(index);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << index << lang;

    if (index == 0)
    {
        ui->sbMinLOM5_7->setEnabled(false);
        ui->sbMaxLOM5_7->setEnabled(false);
    }
    else
    {
        ui->sbMinLOM5_7->setEnabled(true);
        ui->sbMaxLOM5_7->setEnabled(true);
    }
}

void AbulEduWidgetLOMSuiteV1::abeWLOMChangeLang5_10(int index)
{
    QString lang = ui->cbLangLOM5_10->itemText(index);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << index << lang;

    if (index == 0)
    {
        ui->teLOM5_10->setEnabled(false);
    }
    else
    {
        ui->teLOM5_10->setEnabled(true);
    }
    //j'enregistre ce que j'y avais si ce n'est pas nul
    if (!ui->teLOM5_10->toPlainText().isEmpty())
    {
        m_abeLomFrV1->abeLOMaddEducationalDescription(m_previousEducationalDescriptionLang,m_previousEducationalDescription);
        ABULEDU_LOG_DEBUG() <<"Ecriture de ("<<m_previousEducationalDescriptionLang<<";"<<m_previousEducationalDescription<<")";
    }

    m_previousEducationalDescriptionLang = lang;
    QString description = m_previousEducationalDescription;
    if (!m_abeLomFrV1->abeLOMgetEducationalDescription(lang).isEmpty())
    {
         description = m_abeLomFrV1->abeLOMgetEducationalDescription(lang).first();//puisqu'on a dit qu'on n'autorise qu'une valeur par langue
    }
    else
    {
        description = QString();
    }
    ui->teLOM5_10->setText(description);
    m_previousEducationalDescription = description;
}

void AbulEduWidgetLOMSuiteV1::abeWLOMChangeLang5_13(int index)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << index;
    if (index == 0)
    {
        ui->leLOM5_13->setEnabled(false);
    }
    else
    {
        ui->leLOM5_13->setEnabled(true);
        ui->leLOM5_13->setReadOnly(false);
    }
}

void AbulEduWidgetLOMSuiteV1::abeWLOMChangeLang6_3(int index)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << index;
    if (index == 0)
    {
        ui->teLOM6_3->setEnabled(false);
    }
    else
    {
        ui->teLOM6_3->setEnabled(true);
        ui->teLOM6_3->setReadOnly(false);
    }
}

void AbulEduWidgetLOMSuiteV1::addCBLearningRessourceType(QString type)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << type;
    m_numberLearningRessourceType++;
    if (type.isNull())
    {
        if (m_numberLearningRessourceType == 1)
        {
            ui->cbLOM5_2->clear();
            ui->cbLOM5_2->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values());
            ui->cbLOM5_2->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values());
            ui->cbLOM5_2->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values());
            int i=m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values().count();
            while (ui->cbLOM5_2->currentText() != m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                ui->cbLOM5_2->blockSignals(true);
                ui->cbLOM5_2->setCurrentIndex(i);
                ui->cbLOM5_2->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() << m_numberLearningRessourceType<<" A LearningRessourceType affiche : "<<ui->cbLOM5_2->currentText();
        }
        else
        {
            QComboBox* cbType = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxLearningRessourceType",cbType);
            cbType->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values());
            cbType->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values());
            cbType->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values());
            int i=m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values().count();
            while (cbType->currentText() != m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                cbType->blockSignals(true);
                cbType->setCurrentIndex(i);
                cbType->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberLearningRessourceType<<" B LearningRessourceType affiche : "<<cbType->currentText();

            ui->gridLayout_7->addWidget(cbType,m_numberLearningRessourceType-1,2);
            cbType->show();
        }
    }
    else
    {
        if (m_numberLearningRessourceType == 1)
        {
            ui->cbLOM5_2->clear();
            ui->cbLOM5_2->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values());
            ui->cbLOM5_2->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values());
            ui->cbLOM5_2->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values());
            int i=m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values().count();
            while (ui->cbLOM5_2->currentText() != type && i>0)
            {
                i--;
                ui->cbLOM5_2->blockSignals(true);
                ui->cbLOM5_2->setCurrentIndex(i);
                ui->cbLOM5_2->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberLearningRessourceType<<" C LearningRessourceType affiche : "<<ui->cbLOM5_2->currentText();
        }
        else
        {
            QComboBox* cbType = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxLearningRessourceType",cbType);
            cbType->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values());
            cbType->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values());
            cbType->addItems(m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values());
            int i=m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalLearningResourceTypeVocabLOM().values().count();
            while (cbType->currentText() != type && i>0)
            {
                i--;
                cbType->blockSignals(true);
                cbType->setCurrentIndex(i);
                cbType->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberLearningRessourceType<<" D LearningRessourceType affiche : "<<cbType->currentText();
            ui->gridLayout_7->addWidget(cbType,m_numberLearningRessourceType-1,2);
            cbType->show();
        }
    }


}
void AbulEduWidgetLOMSuiteV1::addCBEducationalContext(QString context)
{

    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<context;

    m_numberEducationalContext++;
    if (context.isNull())
    {
        if(m_numberEducationalContext == 1)
        {
            ui->cbLOM5_6->clear();
            ui->cbLOM5_6->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values());
            ui->cbLOM5_6->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values().count()
                  + m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values().count();
            while(ui->cbLOM5_6->currentText() != m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().value("") && i>0)
            {
                i--;
                ui->cbLOM5_6->blockSignals(true);
                ui->cbLOM5_6->setCurrentIndex(i);
                ui->cbLOM5_6->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberEducationalContext<<" A Educational context affiche : "<<ui->cbLOM5_6->currentText();
        }
        else
        {
            QComboBox* cbContext = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalContext",cbContext);
            cbContext->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values());
            cbContext->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values().count()
                  + m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values().count();
            while (cbContext->currentText() != m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().value("") && i>0)
            {
                i--;
                cbContext->blockSignals(true);
                cbContext->setCurrentIndex(i);
                cbContext->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberEducationalContext<<" B Educational context affiche : "<<cbContext->currentText();
            ui->gridLayout_7->addWidget(cbContext,m_numberEducationalContext-1,6);
            cbContext->show();
        }
    }
    else
    {
        if(m_numberEducationalContext == 1)
        {
            ui->cbLOM5_6->clear();
            ui->cbLOM5_6->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values());
            ui->cbLOM5_6->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values().count()
                  + m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values().count();
            while(ui->cbLOM5_6->currentText() != context && i>0)
            {
                i--;
                ui->cbLOM5_6->blockSignals(true);
                ui->cbLOM5_6->setCurrentIndex(i);
                ui->cbLOM5_6->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberEducationalContext<<" C Educational context affiche : "<<ui->cbLOM5_2->currentText();
        }
        else
        {
            QComboBox* cbContext = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalContext",cbContext);
            cbContext->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values());
            cbContext->addItems(m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalContextVocabLOMFR().values().count()
                  + m_abeLomFrV1->abeLOMgetEducationalContextVocabLOM().values().count();
            while (cbContext->currentText() != context && i>0)
            {
                i--;
                cbContext->blockSignals(true);
                cbContext->setCurrentIndex(i);
                cbContext->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalContext<<" D Educational context affiche : "<<cbContext->currentText();
            ui->gridLayout_7->addWidget(cbContext,m_numberEducationalContext-1,6);
            cbContext->show();
        }
    }
}

void AbulEduWidgetLOMSuiteV1::addCBEducationalActivities(QString activity)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << activity;
    m_numberEducationalActivities++;
    if (activity.isNull())
    {
        if(m_numberEducationalActivities == 1)
        {
            ui->cbLOM5_12->clear();
            ui->cbLOM5_12->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values());
            ui->cbLOM5_12->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values().count()
                  + m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values().count();
            while(ui->cbLOM5_12->currentText() != m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                ui->cbLOM5_12->blockSignals(true);
                ui->cbLOM5_12->setCurrentIndex(i);
                ui->cbLOM5_12->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberEducationalActivities<<" A Educational activity affiche : "<<ui->cbLOM5_12->currentText();
        }
        else
        {
            QComboBox* cbActivity = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalActivity",cbActivity);
            cbActivity->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values());
            cbActivity->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values().count()
                  + m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values().count();
            while (cbActivity->currentText() != m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                cbActivity->blockSignals(true);
                cbActivity->setCurrentIndex(i);
                cbActivity->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() << m_numberEducationalActivities<<" B Educational activity affiche : "<<cbActivity->currentText();
            ui->gridLayout_8->addWidget(cbActivity,m_numberEducationalActivities-1,1);
            cbActivity->show();
        }
    }
    else
    {
        if (m_numberEducationalActivities == 1)
        {
            ui->cbLOM5_12->clear();
            ui->cbLOM5_12->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values());
            ui->cbLOM5_12->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values());
            int i=m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values().count()
                + m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values().count();
            while (ui->cbLOM5_12->currentText() != activity && i>0)
            {
                i--;
                ui->cbLOM5_12->blockSignals(true);
                ui->cbLOM5_12->setCurrentIndex(i);
                ui->cbLOM5_12->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalActivities<<" C Educational activity affiche : "<<ui->cbLOM5_12->currentText();
        }
        else
        {
            QComboBox* cbActivity = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalActivity",cbActivity);
            cbActivity->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values());
            cbActivity->addItems(m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values().count()
                  + m_abeLomFrV1->abeLOMgetEducationalActivitiesVocabLOMFR().values().count();
            while (cbActivity->currentText() != activity && i>0)
            {
                i--;
                cbActivity->blockSignals(true);
                cbActivity->setCurrentIndex(i);
                cbActivity->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalActivities<<" D Educational activity affiche : "<<cbActivity->currentText();
            ui->gridLayout_8->addWidget(cbActivity,m_numberEducationalActivities-1,1);
            cbActivity->show();
        }
    }
}

void AbulEduWidgetLOMSuiteV1::addEducationalValidation(QString lang, QString validation)
{
    lang = m_abeLomFrV1->abeLOMgetAvailableLanguages().value(lang);
    m_numberEducationalValidations++;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << m_numberEducationalValidations <<lang <<","<<validation;
    if (lang.isNull() || validation.isNull())
    {
        if(m_numberEducationalValidations == 1)
        {
            //rien
        }
        else
        {
            QLineEdit* leValidation = new QLineEdit();
            leValidation->setObjectName("QLineEditValidation"+QString::number(m_numberEducationalValidations));
            QComboBox* cbValidation = new QComboBox();
            cbValidation->setObjectName("QComboBoxValidation"+QString::number(m_numberEducationalValidations));
            cbValidation->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            ui->glValidation->addWidget(leValidation,m_numberEducationalValidations-1,2);
            ui->glValidation->addWidget(cbValidation,m_numberEducationalValidations-1,1);
            m_addedWidgets << QPair<QString,QWidget*>("QLineEditValidation",leValidation);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxValidation",cbValidation);
            leValidation->show();
            cbValidation->show();
        }
    }
    else
    {
        if(m_numberEducationalValidations == 1)
        {
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            ui->leLOM5_13->setText(validation);
            while (ui->cbLangLOM5_13->currentText() != lang && i>0)
            {
                i--;
                ui->cbLangLOM5_13->blockSignals(true);
                ui->cbLangLOM5_13->setCurrentIndex(i);
                ui->cbLangLOM5_13->blockSignals(false);
            }
        }
        else
        {
            QLineEdit* leValidation = new QLineEdit();
            leValidation->setObjectName("QLineEditValidation"+QString::number(m_numberEducationalValidations));
            leValidation->setText(validation);
            QComboBox* cbValidation = new QComboBox();
            cbValidation->setObjectName("QComboBoxValidation"+QString::number(m_numberEducationalValidations));
            cbValidation->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (cbValidation->currentText() != lang && i>0)
            {
                i--;
                cbValidation->blockSignals(true);
                cbValidation->setCurrentIndex(i);
                cbValidation->blockSignals(false);
            }
            ui->glValidation->addWidget(leValidation,m_numberEducationalValidations-1,2);
            ui->glValidation->addWidget(cbValidation,m_numberEducationalValidations-1,1);
            m_addedWidgets << QPair<QString,QWidget*>("QLineEditValidation",leValidation);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxValidation",cbValidation);
            leValidation->show();
            cbValidation->show();
        }
    }
}

void AbulEduWidgetLOMSuiteV1::addCBEducationalLocation(QString location)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << m_numberEducationalLocations << location;
    m_numberEducationalLocations++;
    if (location.isNull())
    {
        if(m_numberEducationalLocations == 1)
        {
            ui->cbLOM5_14->clear();
            ui->cbLOM5_14->addItems(m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values().count();
            while(ui->cbLOM5_14->currentText() != m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                ui->cbLOM5_14->blockSignals(true);
                ui->cbLOM5_14->setCurrentIndex(i);
                ui->cbLOM5_14->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberEducationalLocations<<" A EducationalLocation affiche : "<<ui->cbLOM5_14->currentText();
        }
        else
        {
            QComboBox* cbLocation = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalLocation",cbLocation);
            cbLocation->addItems(m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values().count();
            while (cbLocation->currentText() != m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                cbLocation->blockSignals(true);
                cbLocation->setCurrentIndex(i);
                cbLocation->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberEducationalLocations<<" B EducationalLocation affiche : "<<cbLocation->currentText();
            ui->gridLayout_8->addWidget(cbLocation,m_numberEducationalLocations-1,5);
            cbLocation->show();
        }
    }
    else
    {
        if(m_numberEducationalLocations == 1)
        {
            ui->cbLOM5_14->clear();
            ui->cbLOM5_14->addItems(m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values().count();
            while(ui->cbLOM5_14->currentText() != location && i>0)
            {
                i--;
                ui->cbLOM5_14->blockSignals(true);
                ui->cbLOM5_14->setCurrentIndex(i);
                ui->cbLOM5_14->blockSignals(false);
            }
             ABULEDU_LOG_DEBUG()<<m_numberEducationalLocations<<ui->cbLOM5_14->currentText();
        }
        else
        {
            QComboBox* cbLocation = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalLocation",cbLocation);
            cbLocation->addItems(m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalLocationVocabSCOLOMFR().values().count();
            while (cbLocation->currentText() != location && i>0)
            {
                i--;
                cbLocation->blockSignals(true);
                cbLocation->setCurrentIndex(i);
                cbLocation->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberEducationalLocations<<cbLocation->currentText();

            ui->gridLayout_8->addWidget(cbLocation,m_numberEducationalLocations-1,5);
            cbLocation->show();
        }
    }
}

void AbulEduWidgetLOMSuiteV1::addCBEducationalPedagogicModality(QString modality)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< modality;
    m_numberEducationalPedagogicModalities++;
    if (modality.isNull())
    {
        if(m_numberEducationalPedagogicModalities == 1)
        {
            ui->cbLOM5_15->clear();
            ui->cbLOM5_15->addItems(m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values().count();
            while(ui->cbLOM5_15->currentText() != m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                ui->cbLOM5_15->blockSignals(true);
                ui->cbLOM5_15->setCurrentIndex(i);
                ui->cbLOM5_15->blockSignals(false);
            }
             ABULEDU_LOG_DEBUG()<<m_numberEducationalPedagogicModalities<<" A EducationalPedagogicModality affiche : "<<ui->cbLOM5_15->currentText();
        }
        else
        {
            QComboBox* cbModality = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalPedagogicModality",cbModality);
            cbModality->addItems(m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values().count();
            while (cbModality->currentText() != m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                cbModality->blockSignals(true);
                cbModality->setCurrentIndex(i);
                cbModality->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() << m_numberEducationalPedagogicModalities<<" B EducationalPedagogicModality affiche : "<<cbModality->currentText();
            ui->gridLayout_9->addWidget(cbModality,m_numberEducationalPedagogicModalities-1,1);
            cbModality->show();
        }
    }
    else
    {
        if(m_numberEducationalPedagogicModalities == 1)
        {
            ui->cbLOM5_15->clear();
            ui->cbLOM5_15->addItems(m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values().count();
            while(ui->cbLOM5_15->currentText() != modality && i>0)
            {
                i--;
                ui->cbLOM5_15->blockSignals(true);
                ui->cbLOM5_15->setCurrentIndex(i);
                ui->cbLOM5_15->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalPedagogicModalities<<" C EducationalPedagogicModality affiche : "<<ui->cbLOM5_15->currentText();
        }
        else
        {
            QComboBox* cbModality = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalPedagogicModality",cbModality);
            cbModality->addItems(m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values().count();
            while (cbModality->currentText() != modality && i>0)
            {
                i--;
                cbModality->blockSignals(true);
                cbModality->setCurrentIndex(i);
                cbModality->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalPedagogicModalities<<" D EducationalPedagogicModality affiche : "<<cbModality->currentText();
            ui->gridLayout_9->addWidget(cbModality,m_numberEducationalPedagogicModalities-1,1);
            cbModality->show();
        }
    }
}

void AbulEduWidgetLOMSuiteV1::addCBEducationalTool(QString tool)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << tool;
    m_numberEducationalTools++;
    if (tool.isNull())
    {
        if(m_numberEducationalTools == 1)
        {
            ui->cbLOM5_16->clear();
            ui->cbLOM5_16->addItems(m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values().count();
            while(ui->cbLOM5_16->currentText() != m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                ui->cbLOM5_16->blockSignals(true);
                ui->cbLOM5_16->setCurrentIndex(i);
                ui->cbLOM5_16->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalTools<<" A EducationalTool affiche : "<<ui->cbLOM5_16->currentText();
        }
        else
        {
            QComboBox* cbTool = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalTool",cbTool);
            cbTool->addItems(m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values().count();
            while (cbTool->currentText() != m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                cbTool->blockSignals(true);
                cbTool->setCurrentIndex(i);
                cbTool->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalTools<<" B EducationalTool affiche : "<<cbTool->currentText();
            ui->gridLayout_9->addWidget(cbTool,m_numberEducationalTools-1,5);
            cbTool->show();
        }
    }
    else
    {
        if(m_numberEducationalTools == 1)
        {
            ui->cbLOM5_16->clear();
            ui->cbLOM5_16->addItems(m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values().count();
            while(ui->cbLOM5_16->currentText() != tool && i>0)
            {
                i--;
                ui->cbLOM5_16->blockSignals(true);
                ui->cbLOM5_16->setCurrentIndex(i);
                ui->cbLOM5_16->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalTools<<" C EducationalTool affiche : "<<ui->cbLOM5_16->currentText();
        }
        else
        {
            QComboBox* cbTool = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxEducationalTool",cbTool);
            cbTool->addItems(m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values());
            int i = m_abeLomFrV1->abeLOMgetEducationalToolsVocabSCOLOMFR().values().count();
            while (cbTool->currentText() != tool && i>0)
            {
                i--;
                cbTool->blockSignals(true);
                cbTool->setCurrentIndex(i);
                cbTool->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG() <<m_numberEducationalTools<<" D EducationalTool affiche : "<<cbTool->currentText();
            ui->gridLayout_9->addWidget(cbTool,m_numberEducationalTools-1,5);
            cbTool->show();
        }
    }
}

void AbulEduWidgetLOMSuiteV1::addEducationalDescription(QString lang, QString description)
{
    lang = m_abeLomFrV1->abeLOMgetAvailableLanguages().value(lang);
    m_numberEducationalDescriptions++;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << m_numberEducationalDescriptions<<lang<<","<<description;
    if (lang.isNull() || description.isNull())
    {
        if(m_numberEducationalDescriptions == 1)
        {
            //rien
        }
        else
        {
            QTextEdit* teDescription = new QTextEdit();
            teDescription->setMaximumHeight(60);
            teDescription->setObjectName("QTextEditDescription"+QString::number(m_numberEducationalDescriptions));
            QComboBox* cbDescription = new QComboBox();
            cbDescription->setObjectName("QComboBoxDescription"+QString::number(m_numberEducationalDescriptions));
            cbDescription->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            ui->glLOM5_10->addWidget(teDescription,m_numberEducationalDescriptions-1,2);
            ui->glLOM5_10->addWidget(cbDescription,m_numberEducationalDescriptions-1,1,Qt::AlignTop);
            m_addedWidgets << QPair<QString,QWidget*>("QTextEditDescription",teDescription);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxDescription",cbDescription);
            teDescription->show();
            cbDescription->show();
        }
    }
    else
    {
        if(m_numberEducationalDescriptions == 1)
        {
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            ui->teLOM5_10->setText(description);
            while (ui->cbLangLOM5_10->currentText() != lang && i>0)
            {
                i--;
                ui->cbLangLOM5_10->blockSignals(true);
                ui->cbLangLOM5_10->setCurrentIndex(i);
                ui->cbLangLOM5_10->blockSignals(false);
            }
        }
        else
        {
            QTextEdit* teDescription = new QTextEdit();
            teDescription->setMaximumHeight(60);
            teDescription->setObjectName("QTextEditDescription"+QString::number(m_numberEducationalDescriptions));
            teDescription->setText(description);
            QComboBox* cbDescription = new QComboBox();
            cbDescription->setObjectName("QComboBoxDescription"+QString::number(m_numberEducationalDescriptions));
            cbDescription->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (cbDescription->currentText() != lang && i>0)
            {
                i--;
                cbDescription->blockSignals(true);
                cbDescription->setCurrentIndex(i);
                cbDescription->blockSignals(false);
            }
            ui->glLOM5_10->addWidget(teDescription,m_numberEducationalDescriptions-1,2);
            ui->glLOM5_10->addWidget(cbDescription,m_numberEducationalDescriptions-1,1,Qt::AlignTop);
            m_addedWidgets << QPair<QString,QWidget*>("QTextEditDescription",teDescription);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxDescription",cbDescription);
            teDescription->show();
            cbDescription->show();
        }
    }
}

void AbulEduWidgetLOMSuiteV1::addEducationalTypicalAgeRange(QString lang, QString range)
{
    int minimum = range.split("-").first().toInt();
    int maximum = range.split("-").last().toInt();
    lang = m_abeLomFrV1->abeLOMgetAvailableLanguages().value(lang);
    m_numberEducationalTypicalAgeRange++;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<m_numberEducationalTypicalAgeRange<<lang<<","<<range;
    if (lang.isNull() || range.isNull())
    {
        if(m_numberEducationalTypicalAgeRange == 1)
        {
            //rien
        }
        else
        {
            QSpinBox* sbMinRange = new QSpinBox();
            sbMinRange->setObjectName("QSpinMinRange"+QString::number(m_numberEducationalTypicalAgeRange));
            QLabel* lblMinus = new QLabel("-");
            lblMinus->setObjectName("QLabelMinus"+QString::number(m_numberEducationalTypicalAgeRange));
            QSpinBox* sbMaxRange = new QSpinBox();
            sbMaxRange->setObjectName("QSpinMaxRange"+QString::number(m_numberEducationalTypicalAgeRange));
            QComboBox* cbLangRange = new QComboBox();
            cbLangRange->setObjectName("QComboBoxLangRange"+QString::number(m_numberEducationalTypicalAgeRange));
            cbLangRange->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (cbLangRange->currentText() != lang && i>0)
            {
                i--;
                cbLangRange->blockSignals(true);
                cbLangRange->setCurrentIndex(i);
                cbLangRange->blockSignals(false);
            }
            ui->glTrancheAge->addWidget(sbMinRange,m_numberEducationalTypicalAgeRange-1,2);
            ui->glTrancheAge->addWidget(lblMinus, m_numberEducationalTypicalAgeRange-1,3);
            ui->glTrancheAge->addWidget(sbMaxRange,m_numberEducationalTypicalAgeRange-1,4);
            ui->glTrancheAge->addWidget(cbLangRange,m_numberEducationalTypicalAgeRange-1,1);
            m_addedWidgets << QPair<QString,QWidget*>("QSpinMinRange",sbMinRange);
            m_addedWidgets << QPair<QString,QWidget*>("QSpinMaxRange",sbMaxRange);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxLangRange",cbLangRange);
            sbMinRange->show();
            sbMaxRange->show();
            lblMinus->show();
            cbLangRange->show();
        }
    }
    else
    {
        if(m_numberEducationalTypicalAgeRange == 1)
        {
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            ui->sbMinLOM5_7->setValue(minimum);
            ui->sbMaxLOM5_7->setValue(maximum);
            while (ui->cbLangLOM5_7->currentText() != lang && i>0)
            {
                i--;
                ui->cbLangLOM5_7->blockSignals(true);
                ui->cbLangLOM5_7->setCurrentIndex(i);
                ui->cbLangLOM5_7->blockSignals(false);
            }
        }
        else
        {
            QSpinBox* sbMinRange = new QSpinBox();
            sbMinRange->setObjectName("QSpinMinRange"+QString::number(m_numberEducationalTypicalAgeRange));
            sbMinRange->setValue(minimum);
            QLabel* lblMinus = new QLabel("-");
            lblMinus->setObjectName("QLabelMinus"+QString::number(m_numberEducationalTypicalAgeRange));
            QSpinBox* sbMaxRange = new QSpinBox();
            sbMaxRange->setObjectName("QSpinMaxRange"+QString::number(m_numberEducationalTypicalAgeRange));
            sbMaxRange->setValue(maximum);
            QComboBox* cbLangRange = new QComboBox();
            cbLangRange->setObjectName("QComboBoxLangRange"+QString::number(m_numberEducationalTypicalAgeRange));
            cbLangRange->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (cbLangRange->currentText() != lang && i>0)
            {
                i--;
                cbLangRange->blockSignals(true);
                cbLangRange->setCurrentIndex(i);
                cbLangRange->blockSignals(false);
            }
            ui->glTrancheAge->addWidget(cbLangRange,m_numberEducationalTypicalAgeRange-1,1);
            ui->glTrancheAge->addWidget(sbMinRange,m_numberEducationalTypicalAgeRange-1,2);
            ui->glTrancheAge->addWidget(lblMinus, m_numberEducationalTypicalAgeRange-1,3);
            ui->glTrancheAge->addWidget(sbMaxRange,m_numberEducationalTypicalAgeRange-1,4);
            m_addedWidgets << QPair<QString,QWidget*>("QSpinMinRange",sbMinRange);
            m_addedWidgets << QPair<QString,QWidget*>("QSpinMaxRange",sbMaxRange);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxLangRange",cbLangRange);
            sbMinRange->show();
            sbMaxRange->show();
            lblMinus->show();
            cbLangRange->show();
        }
    }
}


void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_2_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBLearningRessourceType();
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_6_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBEducationalContext();
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_12_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBEducationalActivities();
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_14_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBEducationalLocation();
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_15_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBEducationalPedagogicModality();
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_16_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBEducationalTool();
}

void AbulEduWidgetLOMSuiteV1::on_sbMinLOM5_7_valueChanged(int arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_previousEducationalTypicalAgeRange.replace(0,m_previousEducationalTypicalAgeRange.indexOf("-"),QString::number(arg1));
}

void AbulEduWidgetLOMSuiteV1::on_sbMaxLOM5_7_valueChanged(int arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_previousEducationalTypicalAgeRange.replace(m_previousEducationalTypicalAgeRange.indexOf("-")+1,m_previousEducationalTypicalAgeRange.size(),QString::number(arg1));
}

void AbulEduWidgetLOMSuiteV1::on_teLOM5_10_textChanged()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_previousEducationalDescription = ui->teLOM5_10->toPlainText();
}


void AbulEduWidgetLOMSuiteV1::on_leLOM5_13_textChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
//    m_previousEducationalValidation = ui->leLOM5_13->text();
    m_previousEducationalValidation = arg1;
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_10_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addEducationalDescription();
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_7_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addEducationalTypicalAgeRange();
}

void AbulEduWidgetLOMSuiteV1::on_btnPlusLOM5_13_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addEducationalValidation();
}
