/** Classe AbulEduIdentitesUIV1
  *
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

#ifndef ABULEDUIDENTITESUIV1_H
#define ABULEDUIDENTITESUIV1_H

#include <QWidget>
#include <QPixmap>
#include <QInputDialog>
#include "abuleduapplicationv1.h"
#include "abuleduidentitesv1.h"
#include "ui_abuleduidentitesuiv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduIdentitesUIV1;
}

class AbulEduIdentitesUIV1 : public QWidget
{
    Q_OBJECT

public:
    explicit AbulEduIdentitesUIV1(QWidget *parent = 0);
    ~AbulEduIdentitesUIV1();
    /**
      * Avant de faire appel à cette fonction il faut s'authentifier sur le SSO à l'aide de ce genre de choses:
      *
      * @code
//1. Authentification
m_nam = abeApp->getAbeNetworkAccessManager();
QNetworkRequest request;
request.setUrl(QUrl("https://webadminxml.abuledu.net/login.php"));

QNetworkReply *reply = m_nam->get(request);
connect(m_nam,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this,SLOT(webadminAuth(QNetworkReply*,QAuthenticator*)));

//Avec le slot auth suivant:
void MainWindow::webadminAuth(QNetworkReply *networkReply, QAuthenticator *authenticator)
{
    qDebug() << "MainWindow::webadminAuth() (1)";
    //Si c'est la 1ere fois qu'on essaye ces parametres ... ok mais
    //sinon il faut la popup (dans ce cas c'est que les enregistrements sauvegardes ne sont plus bon)
    if(m_webadminPassAsked == 0) {
        authenticator->setUser(ui->leLogin->text());
        authenticator->setPassword(ui->lePassword->text());
        //Detection loop
        qDebug() << "Demande d'authentification " << networkReply->url() << " :: " << ui->leLogin->text() << " :: " << ui->lePassword->text();
        m_webadminPassAsked++;

        AbulEduIdentitesUIV1 *i = new AbulEduIdentitesUIV1();
        i->load(ui->leLogin->text());
    }
    //et sinon direction popup
    else {
        qDebug() << "Login ou mot de passe incorrect";
    }
    qDebug() << "MainWindow::webadminAuth() (2)";
}
      * @endcode
      */
    void abeLoad();

    /** Refresh */
    void abeRefresh();

public slots:
    /** Retourne un pointeur vers l'identité de l'utilisateur */
    AbulEduIdentitesV1 *abeGetIdentite(const QString &login);

private slots:
    void slotChoixAvatar(const QString &avatar);
    void slotIdentitieLoaded();
    void slotMessageAvailable(const QString &message);
    void on_btnUpdate_clicked();
    void on_btnUnifier_clicked();
    void on_cbIdentite_currentIndexChanged(const int index);
    void on_btnMesLogs_clicked();
    void on_btnPhoto_clicked();

    void on_lePassword1_textChanged(const QString &arg1);

    void on_lePassword2_textEdited(const QString &arg1);

    void on_btnUpdateAvatar_clicked();
    void on_btnAnnuler_clicked();
    void slotSelectAvatar();

protected:
    void hideEvent(QHideEvent *);

private:
    Ui::AbulEduIdentitesUIV1 *ui;
    //La liste de toutes les identitées
    QList<QPointer<AbulEduIdentitesV1> > m_identites;
    //Et mon identitée principale
    AbulEduIdentitesV1  *m_abuleduIdentite;
    QPushButton         *m_selectedAvatar;

    bool m_localDebug;

signals:
    void signalAbeIdentityUpdated();
};

#endif // ABULEDUIDENTITESUIV1_H
