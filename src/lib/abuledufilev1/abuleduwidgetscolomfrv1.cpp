/** Classe AbulEduWidgetSCOLOMFrV1
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

#include "abuleduwidgetscolomfrv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduWidgetSCOLOMFrV1::AbulEduWidgetSCOLOMFrV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduWidgetSCOLOMFrV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
    m_addedWidgets.clear();
    m_isCbEntityChanged = false;
    m_numberLangs       = 0;
    m_numberWContrib    = 0;
    m_numberDescription = 0;
    m_numberLocation    = 0;
}

AbulEduWidgetSCOLOMFrV1::~AbulEduWidgetSCOLOMFrV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
    /* Pas lieu de détruire m_currentEntity, elle n'est pas instanciée */
}

void AbulEduWidgetSCOLOMFrV1::abeWLOMsetABESCOLOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abeScoLomFrV1 = scoLomFrV1;
    ui->frScoLOMFR2_3->abeLOMsetABESCOLOMFrV1(m_abeScoLomFrV1);
    QFontMetrics mesureur(qApp->font());

    /* Section 1.3 Langue
    à l'ouverture d'un fichier .abe avec des langues renseignées,
            - ajout des items abeLOMgetAvailableLanguages().values() à la ui->cbScoLOMFR1_3
            - affichage de la première valeur de abeLOMgetGeneralLanguage()
            - addLang(QString laSuivante) jusqu'à épuisement
    à l'ouverture d'un fichier .abe sans langue renseignée OU création (pas d'AbulEduFileV1 instancié)
            - ajout des items abeLOMgetAvailableLanguages().values() à la ui->cbScoLOMFR1_3
            - affichage de l'index 0 : "Non renseignée"

    */
    {
        ui->cbScoLOMFR1_3->setFixedWidth(120);
        ui->lblSCOLOMFR1_3->setFixedWidth(mesureur.boundingRect(ui->lblSCOLOMFR1_3->text()).width()+10);
        ui->glLangues->setAlignment(Qt::AlignLeft);
        QStringList listeLangues;
        if (!m_abeScoLomFrV1->abeLOMgetGeneralLanguage().isEmpty())
        {
            listeLangues = m_abeScoLomFrV1->abeLOMgetGeneralLanguage();
            if (listeLangues.size() > 0)
            {
                for (int i=0; i < listeLangues.size(); i++)
                {
                    addCBLang(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().value(listeLangues[i]));
                }
            }
        }
        else
        {
            listeLangues = m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values();
            addCBLang();
        }

        ABULEDU_LOG_DEBUG()<<"Langues : "<<listeLangues;

    }

    /* Section 1.4 Description */
    {    connect(ui->cbSCOLOMFR1_4Langs, SIGNAL(currentIndexChanged(int)), this,SLOT(abeWLOMchangeDescription(int)),Qt::UniqueConnection);

            ABULEDU_LOG_DEBUG()<<"1.4 Description";
            foreach(QString lang, m_abeScoLomFrV1->abeLOMgetGeneralDescriptionKeys())
            {
                ABULEDU_LOG_DEBUG()<<m_abeScoLomFrV1->abeLOMgetGeneralDescription(lang);
            }
        ui->cbSCOLOMFR1_4Langs->addItems(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values());
        if (!m_abeScoLomFrV1->abeLOMgetGeneralDescriptionKeys().count() == 0)
        {
            foreach(QString lang,m_abeScoLomFrV1->abeLOMgetGeneralDescriptionKeys())
            {
                foreach(QString description,m_abeScoLomFrV1->abeLOMgetGeneralDescription(lang))
                {
                    addDescription(lang,description);
                }
            }
        }
        else
        {
            addDescription();
        }    }

    /* Section 2.3 Contribution */
    {    if (m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoles().size() > 0)
        {
            ABULEDU_LOG_DEBUG()<<"Roles utilises : "<<m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoles();
            foreach(QString role,m_abeScoLomFrV1->abeLOMgetLifeCycleContributionRoles())
            {
                ABULEDU_LOG_DEBUG()<<" -- Role : "<<role;
                foreach (QDate date,m_abeScoLomFrV1->abeLOMgetLifeCycleContributionDates(role))
                {
                    ABULEDU_LOG_DEBUG()<<" -- Date : "<<date;
                    addContribution(role,m_abeScoLomFrV1->abeLOMgetLifeCycleContributionEntities(role,date),date);
                }
            }
        }
        else
        {
            addContribution();
        }
        ABULEDU_LOG_DEBUG()<<"Nombre contributions : "<<m_numberWContrib;
    }

    /* Section 4.3 Localistation */
    {   ui->lblScoLOMFR4_3->setFixedWidth(mesureur.boundingRect(ui->lblScoLOMFR4_3->text()).width()+10);
        if(m_abeScoLomFrV1->abeLOMgetTechnicalLocation().size() > 0)
        {
            foreach(QString loc,m_abeScoLomFrV1->abeLOMgetTechnicalLocation())
            {
                addLELocation(loc);
            }
        }
        else
        {
            addLELocation();
        }

        ABULEDU_LOG_DEBUG() << "AbulEduWidgetSCOLOMFrV1::abeWLOMsetABESCOLOMFrV1 (end)"
                            <<"Nombre langues : "<<m_numberLangs
                            <<"Nombre contributions : "<<m_numberWContrib
                            <<"Nombre descriptions : "<<m_numberDescription
                            <<"Nombre locations : "<<m_numberLocation;
    }
    abeWScoLOMDisplayAuthenticated(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus());
}

void AbulEduWidgetSCOLOMFrV1::changeEvent(QEvent *e)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AbulEduWidgetSCOLOMFrV1::on_pushBbtnPlusSCOLOMFR1_3utton_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addCBLang();
}

void AbulEduWidgetSCOLOMFrV1::on_btnPlusSCOLOMFR1_4_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addDescription();
}

void AbulEduWidgetSCOLOMFrV1::on_btnPlusScoLOMFR4_3_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addLELocation();
}

void AbulEduWidgetSCOLOMFrV1::addCBLang(QString lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << lang;
    int tailleLE = ui->cbScoLOMFR1_3->width();
    if (tailleLE == 0) tailleLE = 1;
    int largeurUtilePourLE = ui->glLangues->geometry().width() - ui->lblSCOLOMFR1_3->width();
    if (largeurUtilePourLE <=0) largeurUtilePourLE = this->width() - ui->lblSCOLOMFR1_3->width() ;
    if (largeurUtilePourLE == 0) largeurUtilePourLE = 1;
    int division = (double)largeurUtilePourLE/tailleLE;
    if(division == 0) division = 1;
    int numLigne = m_numberLangs / division;
    int numColonne =  m_numberLangs % division + 1;

    m_numberLangs++;
    if (m_numberLangs == 1)
    {
        if (lang.isNull())
        {
            ui->cbScoLOMFR1_3->clear();
            ui->cbScoLOMFR1_3->addItems(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values());
            ui->cbScoLOMFR1_3->blockSignals(true);
            ui->cbScoLOMFR1_3->setCurrentIndex(0);
            ui->cbScoLOMFR1_3->blockSignals(false);
        }
        else
        {
            ui->cbScoLOMFR1_3->clear();
            ui->cbScoLOMFR1_3->addItems(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (ui->cbScoLOMFR1_3->currentText() != lang && i>0)
            {
                i--;
                ui->cbScoLOMFR1_3->blockSignals(true);
                ui->cbScoLOMFR1_3->setCurrentIndex(i);
                ui->cbScoLOMFR1_3->blockSignals(false);
            }
        }
    }
    else
    {
        QComboBox* cbLang = new QComboBox();
        m_addedWidgets << QPair<QString,QWidget*>("QComboBoxLang",cbLang);
        if (lang.isNull())
        {

            cbLang->addItems(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values());
            cbLang->blockSignals(true);
            cbLang->setCurrentIndex(0);
            cbLang->blockSignals(false);
        }
        else
        {
            cbLang->addItems(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (cbLang->currentText() != lang && i>0)
            {
                i--;
                cbLang->blockSignals(true);
                cbLang->setCurrentIndex(i);
                cbLang->blockSignals(false);
            }
        }

        cbLang->setFixedWidth(tailleLE);
        QHBoxLayout* lay = new QHBoxLayout();
        lay->setAlignment(Qt::AlignLeft);

        if (numColonne != 1)
        {
            QLabel* deCadix = new QLabel("");
            deCadix->setFixedWidth(15);
            lay->addWidget(deCadix);
            deCadix->show();
        }
        lay->addWidget(cbLang);

        ui->glLangues->addLayout(lay,numLigne,numColonne);
        cbLang->show();
    }
}

void AbulEduWidgetSCOLOMFrV1::addDescription(QString lang, QString description)
{
    lang = m_abeScoLomFrV1->abeLOMgetAvailableLanguages().value(lang);
    m_numberDescription++;
    ABULEDU_LOG_DEBUG()<<m_numberDescription<<lang<<","<<description;
    if (lang.isNull() || description.isNull())
    {
        if(m_numberDescription == 1)
        {
            //rien
        }
        else
        {
            QTextEdit* teDescription = new QTextEdit();
            teDescription->setMaximumHeight(60);
            teDescription->setObjectName("QTextEditDescription"+QString::number(m_numberDescription));
            QComboBox* cbDescription = new QComboBox();
            cbDescription->setObjectName("QComboBoxDescription"+QString::number(m_numberDescription));
            cbDescription->addItems(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values());
            ui->glDescription->addWidget(teDescription,m_numberDescription-1,2);
            ui->glDescription->addWidget(cbDescription,m_numberDescription-1,1);
            m_addedWidgets << QPair<QString,QWidget*>("QTextEditDescription",teDescription);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxDescription",cbDescription);
            teDescription->show();
            cbDescription->show();
        }
    }
    else
    {
        if(m_numberDescription == 1)
        {
            int i=m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values().count();
            ui->teSCOLOMFR1_4->setText(description);
            while (ui->cbSCOLOMFR1_4Langs->currentText() != lang && i>0)
            {
                i--;
                ui->cbSCOLOMFR1_4Langs->blockSignals(true);
                ui->cbSCOLOMFR1_4Langs->setCurrentIndex(i);
                ui->cbSCOLOMFR1_4Langs->blockSignals(false);
            }
        }
        else
        {
            QTextEdit* teDescription = new QTextEdit();
            teDescription->setMaximumHeight(60);
            teDescription->setObjectName("QTextEditDescription"+QString::number(m_numberDescription));
            teDescription->setText(description);
            QComboBox* cbDescription = new QComboBox();
            cbDescription->setObjectName("QComboBoxDescription"+QString::number(m_numberDescription));
            cbDescription->addItems(m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values());
            int i=m_abeScoLomFrV1->abeLOMgetAvailableLanguages().values().count();
            while (cbDescription->currentText() != lang && i>0)
            {
                i--;
                cbDescription->blockSignals(true);
                cbDescription->setCurrentIndex(i);
                cbDescription->blockSignals(false);
            }
            ui->glDescription->addWidget(teDescription,m_numberDescription-1,2);
            ui->glDescription->addWidget(cbDescription,m_numberDescription-1,1);
            m_addedWidgets << QPair<QString,QWidget*>("QTextEditDescription",teDescription);
            m_addedWidgets << QPair<QString,QWidget*>("QComboBoxDescription",cbDescription);
            teDescription->show();
            cbDescription->show();
        }
    }
}

void AbulEduWidgetSCOLOMFrV1::addContribution(QString role, QList<vCard> contributeurs, QDate date)
{
    m_numberWContrib++;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << m_numberWContrib
                        <<role <<","<<contributeurs<<","<<date;
    if (role.isNull() || contributeurs.isEmpty() || date.isNull())
    {
        if(m_numberWContrib == 1)
        {
        }
        else
        {
            AbulEduWidgetContribution* wContrib = new AbulEduWidgetContribution();
            wContrib->setObjectName("WidgetContrib"+QString::number(m_numberWContrib));
            wContrib->abeLOMsetABESCOLOMFrV1(m_abeScoLomFrV1);
            ui->vlContributions->removeWidget(ui->btnPlus2_3);
            ui->vlContributions->addWidget(wContrib,m_numberWContrib,0);
            ui->vlContributions->addWidget(ui->btnPlus2_3,m_numberWContrib+2);
            wContrib->show();
            m_addedWidgets << QPair<QString,QWidget*>("AbulEduWidgetContribution",wContrib);
        }
    }
    else
    {
        if(m_numberWContrib == 1)
        {
            ui->frScoLOMFR2_3->setRole(role);
            ui->frScoLOMFR2_3->setDate(date);
            ui->frScoLOMFR2_3->resetEntitiesNumber();
            foreach(vCard entite,contributeurs)
            {
                ui->frScoLOMFR2_3->addCBEntity(entite);
            }
        }
        else
        {
            AbulEduWidgetContribution* wContrib = new AbulEduWidgetContribution();
            wContrib->setObjectName("WidgetContrib"+QString::number(m_numberWContrib));
            wContrib->abeLOMsetABESCOLOMFrV1(m_abeScoLomFrV1);
            wContrib->resetEntitiesNumber();
            wContrib->setRole(role);
            wContrib->setDate(date);
            foreach(vCard entite,contributeurs)
            {
                wContrib->addCBEntity(entite);
            }
            ui->vlContributions->removeWidget(ui->btnPlus2_3);
            ui->vlContributions->addWidget(wContrib,m_numberWContrib);
            ui->vlContributions->addWidget(ui->btnPlus2_3,m_numberWContrib+2);
            wContrib->show();
            m_addedWidgets << QPair<QString,QWidget*>("AbulEduWidgetContribution",wContrib);
        }
    }
}

void AbulEduWidgetSCOLOMFrV1::addLELocation(QString location)
{
    m_numberLocation++;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << m_numberLocation
                        <<location;
    if (location.isNull())
    {
        if (m_numberLocation == 1)
        {
            //rien
        }
        else
        {
            QLineEdit* leLocation = new QLineEdit();
            m_addedWidgets << QPair<QString,QWidget*>("QLineEdit",leLocation);
            ui->glLocalisation->addWidget(leLocation,m_numberLocation-1,1);
            leLocation->show();
        }
    }
    else
    {
        if (m_numberLocation == 1)
        {
            ui->leScoLOMFR4_3->setText(location);
        }
        else
        {
            QLineEdit* leLocation = new QLineEdit(location);
            m_addedWidgets << QPair<QString,QWidget*>("QLineEdit",leLocation);
            ui->glLocalisation->addWidget(leLocation,m_numberLocation-1,1);
            leLocation->show();
        }
    }
}

//! Icham 13/03/2013
//! Plusiers boucle for identiques... Surement à optimiser
void AbulEduWidgetSCOLOMFrV1::saveOnExit()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Sauvegarde des langues et des localisations */
    m_abeScoLomFrV1->abeLOMsetGeneralLanguage(ui->cbScoLOMFR1_3->currentText());
    m_abeScoLomFrV1->abeLOMaddTechnicalLocation(ui->leScoLOMFR4_3->text());
    //! 1ere boucle
    for (int i=m_addedWidgets.count()-1; i>=0; i--)
    {
        ABULEDU_LOG_DEBUG() << "Widget "<<i<<" : "<<m_addedWidgets[i].first;
        if (m_addedWidgets[i].first == "QComboBoxLang")
        {
            m_abeScoLomFrV1->abeLOMaddGeneralLanguage(static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText());
        }
        if (m_addedWidgets[i].first == "QLineEdit")
        {
            m_abeScoLomFrV1->abeLOMaddTechnicalLocation(static_cast<QLineEdit*> (m_addedWidgets[i].second)->text());
        }
    }
    /* Sauvegarde des descriptions 1.4 */
    //    m_abeScoLomFrV1->abeLOMclearGeneralDescription();
    ABULEDU_LOG_DEBUG()<<"langue : "<<ui->cbSCOLOMFR1_4Langs->currentText()
                       <<"description : "<<ui->teSCOLOMFR1_4->toPlainText()
                       <<m_abeScoLomFrV1->abeLOMsetGeneralDescription(ui->cbSCOLOMFR1_4Langs->currentText(),ui->teSCOLOMFR1_4->toPlainText());
    //! 2eme boucle
    for (int i=m_addedWidgets.count()-1; i>=0; i--)
    {
        ABULEDU_LOG_DEBUG()<< "Widget "<<i<<" : "<<m_addedWidgets[i].second->objectName();
        if (m_addedWidgets[i].first == "QComboBoxDescription")
        {
            QString langue = static_cast<QComboBox*> (m_addedWidgets[i].second)->currentText();
            QString valeur = static_cast<QTextEdit*> (m_addedWidgets[i-1].second)->toPlainText();
            ABULEDU_LOG_DEBUG() <<"langue : "<<langue
                                <<"description : "<<valeur
                                <<m_abeScoLomFrV1->abeLOMaddGeneralDescription(langue,valeur);
        }
        else
        {
            ABULEDU_LOG_DEBUG() <<"N'est pas un QComboBoxDescription";
        }
    }

    ABULEDU_LOG_DEBUG()<<"Fin de la sauvegarde des descriptions";
    foreach (QString lang,m_abeScoLomFrV1->abeLOMgetGeneralDescriptionKeys())
    {
    ABULEDU_LOG_DEBUG() <<"-- "<<lang
                        <<m_abeScoLomFrV1->abeLOMgetGeneralDescription(lang);
    }

    //    /* Sauvegarde des entités */
    //    on_cbSCOLOMFR2_3_1_currentIndexChanged(ui->cbSCOLOMFR2_3_1->currentText());
    ui->frScoLOMFR2_3->saveContribInformations();
    //! 3eme boucle
    for (int i=m_addedWidgets.count()-1; i>=0; i--)
    {
        if (m_addedWidgets[i].first == "AbulEduWidgetContribution")
        {
            static_cast<AbulEduWidgetContribution*> (m_addedWidgets[i].second)->saveContribInformations();
        }
    }
}

void AbulEduWidgetSCOLOMFrV1::abeWLOMchangeDescription(int indexLang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (indexLang == 0)
    {
        ui->teSCOLOMFR1_4->setEnabled(false);
    }
    else
        ui->teSCOLOMFR1_4->setEnabled(true);
}

void AbulEduWidgetSCOLOMFrV1::abeWScoLOMDisplayAuthenticated(int returnAuthenticated)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << returnAuthenticated;
    bool isAuthenticated;
    if (returnAuthenticated == 1) {
        isAuthenticated = true;
    }
    else {
        isAuthenticated = false;
    }
    ui->pushBbtnPlusSCOLOMFR1_3utton->setVisible(isAuthenticated);
    ui->cbScoLOMFR1_3->setEnabled(isAuthenticated);
    ui->teSCOLOMFR1_4->setReadOnly(!isAuthenticated);
    //    ui->cbSCOLOMFR2_3_2->setEnabled(isAuthenticated);
    //    ui->btnPlusSCOLOMFR2_3_2->setVisible(isAuthenticated);
    ui->leScoLOMFR4_3->setReadOnly(!isAuthenticated);
    ui->btnPlusScoLOMFR4_3->setVisible(isAuthenticated);

    for (int i=0; i<m_addedWidgets.count(); i++)
    {
        if (m_addedWidgets[i].first.left(9) == "QComboBox")
            m_addedWidgets[i].second->setEnabled(isAuthenticated);
        else if(m_addedWidgets[i].first == "QLineEdit")
        {
            QLineEdit* lineEdit = static_cast<QLineEdit*> (m_addedWidgets[i].second);
            lineEdit->setReadOnly(!isAuthenticated);
        }
    }
    ui->frAuthenticate->setVisible(!isAuthenticated);
    ABULEDU_LOG_DEBUG() << "AbulEduWidgetSCOLOMFrV1::abeWScoLOMDisplayAuthenticated : end";
}

void AbulEduWidgetSCOLOMFrV1::on_cbEntityChanged(QString arg)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    arg = "";
    if (abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() == 1) {
        m_isCbEntityChanged = true;
    }
}

void AbulEduWidgetSCOLOMFrV1::on_btnAuthenticate_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduSSOLoginV1* logWidget = new AbulEduSSOLoginV1();
    connect(logWidget, SIGNAL(signalSSOAuthDone(int)),this,SLOT(abeWScoLOMDisplayAuthenticated(int)), Qt::UniqueConnection);
    logWidget->setAttribute(Qt::WA_StyleSheet);
    logWidget->show();
}

void AbulEduWidgetSCOLOMFrV1::on_btnPlus2_3_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    addContribution();
}
