/** Classe AbulEduWidgetLOMV1
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

#include "abuleduwidgetlomv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduWidgetLOMV1::AbulEduWidgetLOMV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduWidgetLOMV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    setFont(abeApp->font());
    m_numberFreeKeyword = 0;
    m_numberGeneralRessourceType = 0;
    m_numberTechnicalFormat = 0;
    m_abeLomFrV1 = QSharedPointer<AbulEduSCOLOMFrV1>(new AbulEduSCOLOMFrV1(), &QObject::deleteLater);
}

AbulEduWidgetLOMV1::~AbulEduWidgetLOMV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
    m_abeLomFrV1.clear();
}

void AbulEduWidgetLOMV1::abeWLOMsetABELOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abeLomFrV1 = scoLomFrV1;
    QFontMetrics mesureur(font());

    /* Section 1.5 Mot-clé libre */
    ABULEDU_LOG_DEBUG()<<"1.5 Mot-clé libre";
    foreach(QString lang, m_abeLomFrV1->abeLOMgetGeneralKeywordKeys())
    {
        ABULEDU_LOG_DEBUG()<<m_abeLomFrV1->abeLOMgetGeneralKeywords(lang);
    }
    ui->cbLOM1_5->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
    if (!m_abeLomFrV1->abeLOMgetGeneralKeywordKeys().count() == 0)
    {
        foreach(QString lang,m_abeLomFrV1->abeLOMgetGeneralKeywordKeys())
        {
            foreach(QString keyword,m_abeLomFrV1->abeLOMgetGeneralKeywords(lang))
            {
                addFreeKeyword(lang,keyword);
            }
        }
    }
    else
    {
        addFreeKeyword();
    }

    /* Section 1.10 Typologie générale de document */
    ABULEDU_LOG_DEBUG() <<"1.10 Typologie générale de document"
                        <<m_abeLomFrV1->abeLOMgetGeneralDocumentTypology();

    if (m_abeLomFrV1->abeLOMgetGeneralDocumentTypology().count() == 0)
    {
        addCBGeneralDocumentTypology();
    }
    else
    {
        foreach(QString typology,m_abeLomFrV1->abeLOMgetGeneralDocumentTypology())
        {
            typology.remove("ScoLOMFRv1.0;");
            addCBGeneralDocumentTypology(typology);
        }
    }

    /* Section 4.1 Format */
    ABULEDU_LOG_DEBUG() << "4.1 Format"
                        << m_abeLomFrV1->abeLOMgetTechnicalFormat();
    if (m_abeLomFrV1->abeLOMgetTechnicalFormat().count() == 0)
    {
        addLEtechnicalFormat();
    }
    else
    {
        foreach(QString format,m_abeLomFrV1->abeLOMgetTechnicalFormat())
        {
            format.remove("LOMv1.0;");
            addLEtechnicalFormat(format);
        }
    }

    /* Section 4.2 Taille */
    ABULEDU_LOG_DEBUG() << "4.2 Taille"
                        << m_abeLomFrV1->abeLOMgetTechnicalSize();

    if (!m_abeLomFrV1->abeLOMgetTechnicalSize().isEmpty())
    {
        ui->leLOM4_2->setText(m_abeLomFrV1->abeLOMgetTechnicalSize());
        ui->leLOM4_2->setFixedWidth(mesureur.boundingRect(ui->leLOM4_2->text()).width()+10);
    }
    ui->leLOM4_2->setReadOnly(true);

    /* Section 4.7 Durée */
    QString lectureTemps = m_abeLomFrV1->abeLOMgetTechnicalDuration().remove("PT");
    QStringList temps = lectureTemps.split("H");
    int heures = temps.first().toInt();
    lectureTemps = lectureTemps.remove(temps.first()+"H");
    temps = lectureTemps.split("M");
    int minutes = temps.first().toInt();
    lectureTemps = lectureTemps.remove(temps.first()+"M");
    temps = lectureTemps.split("S");
    int secondes = temps.first().toInt();
    ui->teLOM4_7->setTime(QTime(heures,minutes,secondes));
    abeWLOMDisplayAuthenticated(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus());
}

//! Icham 13/02/2013
//! @todo A verifier, mais il y 2 boucles for identiques
void AbulEduWidgetLOMV1::saveOnExit()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Sauvegarde des mots-clé libres 1.5 */
    m_abeLomFrV1->abeLOMsetGeneralKeyword(ui->cbLOM1_5->currentText(),ui->leLOM1_5->text());
    //! 1ere boucle
    for (int i=m_addedWidgets.count()-1; i>=0 ;i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;
        if (m_addedWidgets[i].first == "QComboBoxKeyword")
        {
            QString langue = static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText();
            QString valeur = static_cast<QLineEdit*> (m_addedWidgets[i-1].second)->text();
            m_abeLomFrV1->abeLOMaddGeneralKeyword(langue,valeur);
        }
    }

    /* Sauvegarde des typologies générales de document 1.10 */
    m_abeLomFrV1->abeLOMclearGeneralDocumentTypology();
    m_abeLomFrV1->abeLOMaddGeneralDocumentTypology(ui->cbLOM1_10->currentText());
    //! 2eme boucle identique
    for (int i=m_addedWidgets.count()-1; i>=0; i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].first;
        if (m_addedWidgets[i].first == "QComboBoxGeneralRessourceType")
        {
            m_abeLomFrV1->abeLOMaddGeneralDocumentTypology(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
        }
    }

    /* Sauvegarde de la durée 4.7 - La structure de données n'est pas sauvegardée pour l'instant par p_abeLOMExportAsXML_section4()  */
    m_abeLomFrV1->abeLOMsetTechnicalDuration("PT"
                                             +QString::number(ui->teLOM4_7->time().hour())
                                             + "H"
                                             + QString::number(ui->teLOM4_7->time().minute())
                                             + "M"
                                             + QString::number(ui->teLOM4_7->time().second())
                                             + "S");
    ABULEDU_LOG_DEBUG()<<" Duree : "<<m_abeLomFrV1->abeLOMgetTechnicalDuration();
}

void AbulEduWidgetLOMV1::on_btnAuthenticate_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduSSOLoginV1* logWidget = new AbulEduSSOLoginV1();
    connect(logWidget, SIGNAL(signalSSOAuthDone(int)),this,SLOT(abeWLOMDisplayAuthenticated(int)), Qt::UniqueConnection);
    logWidget->setAttribute(Qt::WA_StyleSheet);
    logWidget->show();
}

void AbulEduWidgetLOMV1::abeWLOMDisplayAuthenticated(int returnAuthenticated)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool isAuthenticated;
    if (returnAuthenticated == 1)
        isAuthenticated = true;
    else isAuthenticated = false;
    ui->frAuthenticate->setVisible(!isAuthenticated);

}

void AbulEduWidgetLOMV1::addFreeKeyword(QString lang,QString keyword)
{
    lang = m_abeLomFrV1->abeLOMgetAvailableLanguages().value(lang);
    m_numberFreeKeyword++;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << m_numberFreeKeyword <<lang<<","<<keyword;
    if (lang.isNull() || keyword.isNull())
    {
        if(m_numberFreeKeyword == 1)
        {
            //rien
        }
        else
        {
            QLineEdit* AbulEduMediathequePushleMotCle = new QLineEdit();
            AbulEduMediathequePushleMotCle->setObjectName("QLineEditKeyword"+QString::number(m_numberFreeKeyword));
            QComboBox* cbMotCle = new QComboBox();
            cbMotCle->setObjectName("QComboBoxKeyword"+QString::number(m_numberFreeKeyword));
            cbMotCle->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            ui->glLOM1_5->addWidget(AbulEduMediathequePushleMotCle,m_numberFreeKeyword-1,2);
            ui->glLOM1_5->addWidget(cbMotCle,m_numberFreeKeyword-1,1);
            m_addedWidgets << QPair<QString,QWidget*>("QLineEditKeyword",AbulEduMediathequePushleMotCle);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxKeyword",cbMotCle);
            AbulEduMediathequePushleMotCle->show();
            cbMotCle->show();
        }
    }
    else
    {
        if(m_numberFreeKeyword == 1)
        {
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            ui->leLOM1_5->setText(keyword);
            while (ui->cbLOM1_5->currentText() != lang && i>0)
            {
                i--;
                ui->cbLOM1_5->blockSignals(true);
                ui->cbLOM1_5->setCurrentIndex(i);
                ui->cbLOM1_5->blockSignals(false);
            }
        }
        else
        {
            QLineEdit* AbulEduMediathequePushleMotCle = new QLineEdit();
            AbulEduMediathequePushleMotCle->setObjectName("QLineEditKeyword"+QString::number(m_numberFreeKeyword));
            AbulEduMediathequePushleMotCle->setText(keyword);
            QComboBox* cbMotCle = new QComboBox();
            cbMotCle->setObjectName("QComboBoxKeyword"+QString::number(m_numberFreeKeyword));
            cbMotCle->addItems(m_abeLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (cbMotCle->currentText() != lang && i>0)
            {
                i--;
                cbMotCle->blockSignals(true);
                cbMotCle->setCurrentIndex(i);
                cbMotCle->blockSignals(false);
            }
            ui->glLOM1_5->addWidget(AbulEduMediathequePushleMotCle,m_numberFreeKeyword-1,2);
            ui->glLOM1_5->addWidget(cbMotCle,m_numberFreeKeyword-1,1);
            m_addedWidgets << QPair<QString,QWidget*>("QLineEditKeyword",AbulEduMediathequePushleMotCle);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxKeyword",cbMotCle);
            AbulEduMediathequePushleMotCle->show();
            cbMotCle->show();
        }
    }
}

void AbulEduWidgetLOMV1::addCBGeneralDocumentTypology(QString type)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_numberGeneralRessourceType++;
    if (type.isNull())
    {
        if (m_numberGeneralRessourceType == 1)
        {
            ui->cbLOM1_10->clear();
            ui->cbLOM1_10->addItems(m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values());
            int i=m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values().count();
            while (ui->cbLOM1_10->currentText() != m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                ui->cbLOM1_10->blockSignals(true);
                ui->cbLOM1_10->setCurrentIndex(i);
                ui->cbLOM1_10->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberGeneralRessourceType<<" GeneralRessourceType affiche : "<<ui->cbLOM1_10->currentText();
        }
        else
        {
            QComboBox* cbType = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxGeneralRessourceType",cbType);
            cbType->addItems(m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values());
            int i=m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values().count();
            while (cbType->currentText() != m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().value("") && i>0)
            {
                i--;
                cbType->blockSignals(true);
                cbType->setCurrentIndex(i);
                cbType->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberGeneralRessourceType<<"GeneralRessourceType affiche : "<<cbType->currentText();
            ui->glLOM1_10->addWidget(cbType,m_numberGeneralRessourceType-1,1);
            cbType->show();
        }
    }
    else
    {
        if (m_numberGeneralRessourceType == 1)
        {
            ui->cbLOM1_10->clear();
            ui->cbLOM1_10->addItems(m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values());
            int i=m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values().count();
            while (ui->cbLOM1_10->currentText() != type && i>0)
            {
                i--;
                ui->cbLOM1_10->blockSignals(true);
                ui->cbLOM1_10->setCurrentIndex(i);
                ui->cbLOM1_10->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberGeneralRessourceType<<" GeneralRessourceType affiche : "<<ui->cbLOM1_10->currentText();
        }
        else
        {
            QComboBox* cbType = new QComboBox();
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxGeneralRessourceType",cbType);
            cbType->addItems(m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values());
            int i=m_abeLomFrV1->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values().count();
            while (cbType->currentText() != type && i>0)
            {
                i--;
                cbType->blockSignals(true);
                cbType->setCurrentIndex(i);
                cbType->blockSignals(false);
            }
            ABULEDU_LOG_DEBUG()<<m_numberGeneralRessourceType<<"GeneralRessourceType affiche : "<<cbType->currentText();
            ui->glLOM1_10->addWidget(cbType,m_numberGeneralRessourceType-1,1);
            cbType->show();
        }
    }
}

void AbulEduWidgetLOMV1::addLEtechnicalFormat(QString format)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << format;
    m_numberTechnicalFormat++;
    if (format.isNull())
    {
        if (m_numberTechnicalFormat == 1)
        {
            ui->leLOM4_1->setReadOnly(true);
        }
        else
        {
            QLineEdit* leFormat = new QLineEdit();
            leFormat->setReadOnly(true);
            m_addedWidgets << QPair<QString,QWidget*>("QLineEditTechnicalFormat",leFormat);
            ui->glLOM4_1->addWidget(leFormat,m_numberTechnicalFormat-1,1);
            leFormat->show();
        }
    }
    else
    {
        if (m_numberTechnicalFormat == 1)
        {
            ui->leLOM4_1->setText(format);
            ui->leLOM4_1->setReadOnly(true);
        }
        else
        {
            QLineEdit* leFormat = new QLineEdit(format);
            leFormat->setReadOnly(true);
            m_addedWidgets << QPair<QString,QWidget*>("QLineEditTechnicalFormat",leFormat);
            ui->glLOM4_1->addWidget(leFormat,m_numberTechnicalFormat-1,1);
            leFormat->show();
        }
    }
}

void AbulEduWidgetLOMV1::on_btnPlusLOM1_5_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addFreeKeyword();
}

void AbulEduWidgetLOMV1::on_btnPlusLOM1_10_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBGeneralDocumentTypology();
}

void AbulEduWidgetLOMV1::on_btnPlusLOM4_1_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addLEtechnicalFormat();
}
