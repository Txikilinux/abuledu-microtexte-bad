/** Classe AbulEduSSOLoginV1 : gestion de l'authentification SSO AbulEdu
  * @author 2012-2014 Eric Seigne <eric.seigne@ryxeo.com>
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
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#ifndef ABULEDUSSOLOGINV1_H
#define ABULEDUSSOLOGINV1_H

#include <QPainter>
#include <QBitmap>
#include <QWidget>
#include <QDesktopWidget>
#include <QCompleter>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QScreen>
#include "abuleduapplicationv1.h"
#include "abuleduloggerv1.h"
#include "abuledunetworktests.h"
#include "ui_abuledussologinv1.h"
#include "abuledunetworkaccessmanagerv1.h"


class AbulEduNetworkAccessManagerV1;
class MaiaXmlRpcClient;

namespace Ui {
    class AbulEduSSOLoginV1;
}

class AbulEduSSOLoginV1 : public QWidget
{
    Q_OBJECT
    Q_ENUMS(enumAbulEduSSOLOgin)

public:
    enum enumAbulEduSSOLOgin { abePetitSSO, abeGrandSSO };

    /** Constructeur */
    explicit AbulEduSSOLoginV1(QWidget *parent = 0);
    virtual ~AbulEduSSOLoginV1();
    void abeFocus();
    void abeAddDomaineForNextLogin(const QString& domaine);
    void abeAddLoginForNextLogin(const QString& login);

    QStringList abeGetDomainesList();
    bool abeDelDomaine(QString domaine);

    /** Permet de basculer d'une sso login simple a une sso login avec tt les vignettes
      * @param petitOuGrand cf enumAbulEduSSOLOgin
      */
    void abeSetPetitOuGrandEnum(enumAbulEduSSOLOgin petitOuGrand);
    void abeSetButtonCloseVisible(const bool visible);

signals:
    void signalSSOAuthDone(int code);
    void signalClosed();

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    /* SLOTS de gestion de page (recuperable depuis l'extérieur pour création de compte par exemple) */
    void slotAbeSSOShowPageLogin();
    void slotAbeSSOShowPageCreateAccount();
    void slotAbeSSOShowPageRetrievePassword();
    void slotAbeSSOShowPageParameters();

private slots:
    void actualiseListeVignettesUtilisateurs(QListWidget *lw);
    void rpcNewPasswordRequested(QVariant &v, QNetworkReply *reply);
    void rpcNewAccountRequested(QVariant &v, QNetworkReply *reply);
    void rpcFault(const int code, const QString &errorMessage);
    void slotSSOAuthStatus(const int code);
    void slotNetworkTestError(int code, int elapsedTime);
    void slotNetworkTestOk(int elapsedTime);
    void on_btnLogin_clicked();
    void on_btnPasswordOublie_clicked();
    void on_btnCreerUnCompte_clicked();
    void on_btnOuvrirUnCompte_clicked();
    void on_btnClose_clicked();
    void on_lePassword_returnPressed();

    /** Permet de detecter un changement de domaine et donc de changer l'uri de webadminxml */
    void slotSSOStatusChanged(const int code);
    void slotCleanSSOLoginPass();
    void on_btnMotDePasseOublieAnnuler_clicked();
    void on_btnCreerUnCompteAnnuler_clicked();
    void on_btnMotDePasseOublie_clicked();

    void abeSSOLoginDisplayStatusMessage(const QPixmap &icone, const QString &message, QWidget *pageAReafficher);
    void slotCleanStatusMessage();

    void on_btnAbulEduSSOParametres_clicked();
    void on_btnParametresAnnuler_clicked();
    void on_btnParametresValider_clicked();

private:
    Ui::AbulEduSSOLoginV1         *ui;

    QPoint                         m_offset;
    MaiaXmlRpcClient              *m_rpcWebadminResetPass;
    bool                           m_logoutInProgress;
    AbulEduNetworkAccessManagerV1 *m_nam;
    int                            m_webadminPassAsked;
    QString                        m_situation; //pour savoir si on est en train de demander un mot de passe par email etc.
    enumAbulEduSSOLOgin            m_petitOuGrand;
    AbulEduNetworkTests           *m_abuleduNetworkTest;
    QSettings                     *m_ssoLoginSettings;
    QRegExp                       *m_mailRegExp;
    QRegExp                       *m_domaineRegExp;
    QCompleter                    *m_loginCompleter;
    QCompleter                    *m_domainCompleter;

    void updateLoginCompleter(const QStringList& listeLogins);
    void updateDomainCompleter(const QStringList& listeDomains);
};

#endif // ABULEDUSSOLOGINV1_H
