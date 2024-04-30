/** Classe AbulEduProxyPasswordDialog
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * Cette classe est normalement interne à AbulEduNetworkAccessManager et
  * AbulEduApplication, vous ne devriez pas avoir à vous en servir directement
  * elle permet de gérer automatiquement une authentification éventuelle du
  * serveur proxy ... qu'on trouve parfois dans quelques écoles ou académies
  * par exemple
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

#ifndef ABULEDUPROXYPASSWORDDIALOG_H
#define ABULEDUPROXYPASSWORDDIALOG_H

#include <QAuthenticator>
#include <QDialog>
#include <QNetworkReply>
#include "abuleduapplicationv1.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "ui_abuleduproxypassworddialog.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduProxyPasswordDialog;
}

class AbulEduProxyPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbulEduProxyPasswordDialog(QString *login, QString *pass, QWidget *parent = 0);
    virtual ~AbulEduProxyPasswordDialog();
    void setAbeLoginPassDialogAuthenticator(QAuthenticator *authenticator);
    void setAbeLoginPassDialogNetworkReply(QNetworkReply  *reply);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AbulEduProxyPasswordDialog  *ui;
    QAuthenticator                  *m_authenticator;
    QNetworkReply                   *m_networkReply;
    QString                         *m_login;
    QString                         *m_pass;
};

#endif // ABULEDUPROXYPASSWORDDIALOG_H
