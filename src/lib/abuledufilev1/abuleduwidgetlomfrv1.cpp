/** Classe AbulEduWidgetLOMFrV1
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

#include "abuleduwidgetlomfrv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduWidgetLOMFrV1::AbulEduWidgetLOMFrV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduWidgetLOMFrV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->setupUi(this);
}

AbulEduWidgetLOMFrV1::~AbulEduWidgetLOMFrV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduWidgetLOMFrV1::abeWLOMsetABELOMFrV1(QSharedPointer<AbulEduSCOLOMFrV1> scoLomFrV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_abeLomFrV1 = scoLomFrV1;
    /* 1.1.1 Catalogue */
    ui->leLOMFR1_1_1->setText(m_abeLomFrV1.data()->abeLOMgetGeneralIdentifierCatalog());

    /* 1.1.2 Entrée */

    if (!m_abeLomFrV1.data()->abeLOMgetGeneralIdentifierEntry().isEmpty())
        ui->leLOMFR1_1_2->setText(m_abeLomFrV1.data()->abeLOMgetGeneralIdentifierEntry());

    /* 1.2 Titre */
    ui->cbLOMFR1_2Lang->addItems(m_abeLomFrV1.data()->abeLOMgetAvailableLanguages().values());
    if (m_abeLomFrV1.data()->abeLOMgetGeneralLanguage().size() > 0)
    {
        ui->cbLOMFR1_2Lang->setCurrentIndex(ui->cbLOMFR1_2Lang->findText(m_abeLomFrV1.data()->abeLOMgetGeneralLanguage().first()));
    }
    connect(ui->cbLOMFR1_2Lang, SIGNAL(currentIndexChanged(int)), this,SLOT(abeWLOMchangeTitle(int)),Qt::UniqueConnection);
    abeWLOMFrDisplayAuthenticated(abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus());

}

void AbulEduWidgetLOMFrV1::changeEvent(QEvent *e)
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

void AbulEduWidgetLOMFrV1::abeWLOMchangeTitle(int indexLang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << indexLang;

    if (indexLang == 0)
    {
        ui->leLOMFR1_2->setEnabled(false);
    }
    else
    {
        ui->leLOMFR1_2->setEnabled(true);
        ui->leLOMFR1_2->setFocus();
//        ui->leLOMFR1_2->grabKeyboard();
    }
    QString lang = ui->cbLOMFR1_2Lang->itemText(indexLang);
    if (abeApp->getAbeNetworkAccessManager()->abeSSOAuthenticationStatus() == 1 && !ui->leLOMFR1_2->text().isEmpty())
    {
        m_abeLomFrV1.data()->abeLOMsetGeneralTitle(m_previousTitleLanguage,ui->leLOMFR1_2->text());
        ABULEDU_LOG_DEBUG() << ui->leLOMFR1_2->text()<<" ecrit pour "<<m_previousTitleLanguage;
    }
    ui->leLOMFR1_2->setText(m_abeLomFrV1.data()->abeLOMgetGeneralTitle(lang));
    m_previousTitleLanguage = m_abeLomFrV1.data()->abeLOMgetAvailableLanguages().key(lang);
}

void AbulEduWidgetLOMFrV1::abeWLOMFrDisplayAuthenticated(int returnAuthenticated)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << returnAuthenticated;
    bool isAuthenticated;
    if (returnAuthenticated == 1)
    {
        isAuthenticated = true;
    }
    else
    {
        isAuthenticated = false;
    }
    ui->leLOMFR1_1_1->setReadOnly(!isAuthenticated);
    ui->leLOMFR1_2->setReadOnly(!isAuthenticated);
    ui->frAuthenticate->setVisible(!isAuthenticated);
}

void AbulEduWidgetLOMFrV1::on_cbLOMFR1_2Lang_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(arg1);
}

void AbulEduWidgetLOMFrV1::saveOnExit()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeWLOMchangeTitle(ui->cbLOMFR1_2Lang->currentIndex());
    m_abeLomFrV1.data()->abeLOMsetGeneralIdentifierCatalog(ui->leLOMFR1_1_1->text());
    m_abeLomFrV1.data()->abeLOMsetGeneralIdentifierEntry(ui->leLOMFR1_1_2->text());
}


void AbulEduWidgetLOMFrV1::on_btnAuthenticate_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduSSOLoginV1* logWidget = new AbulEduSSOLoginV1();
    connect(logWidget, SIGNAL(signalSSOAuthDone(int)),this,SLOT(abeWLOMFrDisplayAuthenticated(int)),Qt::UniqueConnection);
    logWidget->setAttribute(Qt::WA_StyleSheet);
    logWidget->show();
}
