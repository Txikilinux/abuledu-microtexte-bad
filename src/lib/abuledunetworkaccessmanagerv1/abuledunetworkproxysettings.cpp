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

#include "abuledunetworkproxysettings.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduNetworkProxySettings::AbulEduNetworkProxySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduNetworkProxySettings)
{
    ui->setupUi(this);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_nam = abeApp->getAbeNetworkAccessManager();
    slotRefresh();
}

AbulEduNetworkProxySettings::~AbulEduNetworkProxySettings()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduNetworkProxySettings::slotRefresh()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    switch(m_nam->abeGetProxySettingsEnum()) {
    case AbulEduNetworkAccessManagerV1::abeProxyDisabled:
        ui->rbProxyDesactive->setChecked(true);
        break;
    case AbulEduNetworkAccessManagerV1::abeProxyAuto:
        ui->rbProxyAutomatique->setChecked(true);
        break;
    case AbulEduNetworkAccessManagerV1::abeProxyManual:
        ui->rbProxyManuel->setChecked(true);
        ui->leProxyHTTP->setText(m_nam->proxy().hostName());
        ui->sbPort1->setValue(m_nam->proxy().port());
        break;
    case AbulEduNetworkAccessManagerV1::abeProxyAutoURI:
        ui->rbProxyAutoURI->setChecked(true);
        break;
    case AbulEduNetworkAccessManagerV1::abeProxySystemEnv:
        ui->rbProxySysteme->setChecked(true);
        break;
    }
}

void AbulEduNetworkProxySettings::on_btnAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(parent() == 0) {
        close();
    }
    else {
        emit signalBtnCancel();
    }
}

void AbulEduNetworkProxySettings::on_btnOK_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if(ui->rbProxyManuel->isChecked()) {
        m_nam->abeSetProxyManualSettings(ui->leProxyHTTP->text(),ui->sbPort1->text());
        m_nam->abeProxyConfigTest("update");
    }
    else if(ui->rbProxyAutomatique->isChecked()){
        /* Ici on passe par une autre requete (wpad) */
        m_nam->abeProxyConfigTest("WPADRequest");
    }
    else if(ui->rbProxyDesactive->isChecked() || ui->rbProxySysteme->isChecked()){
        m_nam->abeProxyConfigTest("update");
    }
//    m_nam->abeSaveProxySettings();

    QApplication::restoreOverrideCursor();
    if(parent() == 0) {
        close();
    }
    else {
        emit signalBtnOk();
    }
}

void AbulEduNetworkProxySettings::on_rbProxyDesactive_toggled(bool checked)
{
    ABULEDU_LOG_TRACE() << checked;

    if(checked) {
        m_nam->abeSetProxySettingsEnum(AbulEduNetworkAccessManagerV1::abeProxyDisabled);
    }
}

void AbulEduNetworkProxySettings::on_rbProxyAutomatique_toggled(bool checked)
{
    ABULEDU_LOG_TRACE() << checked;

    if(checked) {
        m_nam->abeSetProxySettingsEnum(AbulEduNetworkAccessManagerV1::abeProxyAuto);
    }
}

void AbulEduNetworkProxySettings::on_rbProxyManuel_toggled(bool checked)
{
    ABULEDU_LOG_TRACE() << checked;

    ui->frameProxyManuel->setEnabled(checked);
    if(checked) {
        m_nam->abeSetProxySettingsEnum(AbulEduNetworkAccessManagerV1::abeProxyManual);
    }
}

void AbulEduNetworkProxySettings::on_rbProxyAutoURI_toggled(bool checked)
{
    ABULEDU_LOG_TRACE() << checked;

    ui->frameProxyAutoURI->setEnabled(checked);
    if(checked) {
        m_nam->abeSetProxySettingsEnum(AbulEduNetworkAccessManagerV1::abeProxyAutoURI);
    }
}

void AbulEduNetworkProxySettings::on_rbProxySysteme_toggled(bool checked)
{
    ABULEDU_LOG_TRACE() << checked;

    if(checked) {
        m_nam->abeSetProxySettingsEnum(AbulEduNetworkAccessManagerV1::abeProxySystemEnv);
    }
}
