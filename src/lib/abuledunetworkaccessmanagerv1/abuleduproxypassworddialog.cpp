/** Classe AbulEduProxyPasswordDialog: utilisée lorsque le proxy demande une authentification
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
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

#include "abuleduproxypassworddialog.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduProxyPasswordDialog::AbulEduProxyPasswordDialog(QString *login, QString *pass, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AbulEduProxyPasswordDialog),
    m_login(login),
    m_pass(pass)
{
    ui->setupUi(this);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

AbulEduProxyPasswordDialog::~AbulEduProxyPasswordDialog()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
}

void AbulEduProxyPasswordDialog::setAbeLoginPassDialogAuthenticator(QAuthenticator *authenticator)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_authenticator = authenticator;
}

void AbulEduProxyPasswordDialog::setAbeLoginPassDialogNetworkReply(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_networkReply = reply;
}

void AbulEduProxyPasswordDialog::on_buttonBox_accepted()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_login->clear();
    m_login->append(ui->leLogin->text());

    m_pass->clear();
    m_pass->append(ui->lePassword->text());

    m_authenticator->setUser(m_login->toLocal8Bit());
    m_authenticator->setPassword(m_pass->toLocal8Bit());

    //On stocke le login/pass pour eviter de le demander a chaque fois ...
    QByteArray pass = ui->lePassword->text().toUtf8();
    QByteArray passc = pass.toHex().toBase64();
    QSettings settings("abuledu","AbulEduProxySettings");
    settings.beginGroup("proxy");
    settings.setValue("login",ui->leLogin->text());
    settings.setValue("pass",passc);
    settings.setValue("server",abeApp->getAbeNetworkAccessManager()->proxy().hostName());

    //On relance la requete ... pour provoquer la reouverture de la popup si le mot de passe
    //n'est pas bon
    QNetworkRequest redirection(m_networkReply->url());
    m_networkReply = abeApp->getAbeNetworkAccessManager()->get(redirection);
}
