/** Classe AbulEduNetworkAccessManagerV1
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#ifndef ABULEDUNETWORKACCESSMANAGERV1_H
#define ABULEDUNETWORKACCESSMANAGERV1_H

#include <QtNetwork/QNetworkInterface>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QRegExp>
#include <QStringList>
#include <QUrl>
#include <QHostInfo>
#include <QSharedPointer>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QNetworkReply>
#include <QObject>
#include <QSettings>

#ifndef QT_NO_SSL
#include <QSslCertificate>
#endif

#include "maiaObject.h"
#include "abuledunetworkthreadv1.h"
#include "abuleduproxypassworddialog.h"
#include "abuledulocalsocketipcserverv1.h"
#include "abuledulocalsocketipcclientv1.h"
#include "abuledunetworktests.h"
#include "abuleduconstantesv1.h"
#include "abuleduidentitesv1.h"
#include "abuleduloggerv1.h"


/* 20140321 Bug #3454 Forward Declaration car dépendance cyclique*/
class AbulEduSSOLoginV1;

/**
  * Dialogue IPC Client / Serveur
  * HELLO (client) -> serveur, ajoute ce client dans la liste des alertes
  * QUIT (client) -> serveur, supprime ce client de la liste des alertes
  * IPCSSOLOGIN (client) -> serveur, lance une procédure de mise à jour du ticket SSO vers toutes les applications
  * IPCSSOLOGOUT (client) -> serveur, lance la procédure de fermeture de session SSO pour toute les applications
  *
  */

class AbulEduNetworkAccessManagerV1 : public QNetworkAccessManager
{
    Q_OBJECT
    Q_ENUMS(enumAbulEduNetworkAccessManagerProxySettings)
    //    Q_PROPERTY(enumAbulEduNetworkAccessManagerProxySettings proxySettings READ abeGetProxySettingsEnum WRITE abeGetProxySettingsEnum USER true)

public:
    enum enumAbulEduNetworkAccessManagerProxySettings {
        abeProxyDisabled  = 0x0,
        abeProxyAuto      = 0x1,
        abeProxyManual    = 0x2,
        abeProxyAutoURI   = 0x3,
        abeProxySystemEnv = 0x4
    };

    AbulEduNetworkAccessManagerV1( QObject *parent = 0 );
    virtual ~AbulEduNetworkAccessManagerV1();

    /** Setter Login/Pass */
    void setAbeLoginPass(const QString& login, const QString& pass);
    void abeSetSSOLogin(const QString& login){ m_login = login;}
    void abeSetSSOPassword(const QString& pass){ m_pass = pass;}

    /** Getter Login/Pass */
    inline const QString& abeGetSSOLogin() {return m_login;}
    inline const QString& abeGetSSOPassword() {return m_pass;}

    /** Permet d'ouvrir la session SSO */
    void abeSSOLogin();

    /** Permet de fermer la session SSO */
    void abeSSOLogout();

    /** Permet de savoir si l'authentification SSO est ok
      * @return int: 0 on ne sait pas, 1 ok, -1 erreur
      */
    int abeSSOAuthenticationStatus(){ return m_ssoAuthentifiedStatus;}

    /** Permet de connecter un slot et de le lancer si l'authentification se passe bien
      * Exemple d'utilisation:
      * @code
      * void MainWindow::on_action_Mon_compte_triggered()
      * {
      *     if(m_nam->abeSSOAuthenticationStatus() != 1) {
      *         m_nam->abeOnLoginSuccessGoto(this, SLOT(on_action_Mon_compte_triggered()));
      *         ui->stackedWidget->setCurrentWidget(ui->pageLogin);
      *     }
      *     else {
      *         ui->stackedWidget->setCurrentWidget(ui->pageIdentite);
      *     }
      * }
      * @endcode
      *
      */
    void abeOnLoginSuccessGoto(QObject *objectDestination, const char* slotDestination);

    /** Permet de connecter un slot et de le lancer si l'authentification est rejetée */
    void abeOnLoginFailureGoto(QObject *objectDestination, const char* slotDestination);

    enumAbulEduNetworkAccessManagerProxySettings abeGetProxySettingsEnum() const;
    void abeSetProxySettingsEnum(enumAbulEduNetworkAccessManagerProxySettings settings);

    /** Sauvegarde la configuration proxy */
    void abeSaveProxySettings();

    /** Applique la configuration proxy precedement sauvegardée */
    void abeActivateProxySettings();

    /** Affecte la configuration proxy */
    void abeSetProxyManualSettings(const QString& serveurHTTP, const QString& portHTTP);

    /** Autorise uniquement une liste de domaines, cette fonction permet de n'autoriser que quelques domaines.
      * Pratique si on veut faire un logiciel client pour babytwit.fr et qu'on ne souhaite pas que les utilisateurs
      * puissent "partir" sur d'autres sites en suivant des liens (par exemple)
      * @warning: si vous voulez appliquer la liste blanche a un webview il ne faut pas oublier de
      * a) ui->webView->page()->setNetworkAccessManager(m_nam);
      * b) ne pas mettre d'url dans qt-designer: a partir du moment ou un webview a ouvert une page on ne peut plus
      *    changer son networkmanager !
      */
    void abeSetNetworkWhiteList(const QStringList& domains);

    /** Retourne une information basique : acces internet ok ou pas */
    bool abeIsInternetAvailable();

signals:
    /** Permet de connecter un slot si l'authentification SSO n'est pas bonne
      * ssoAuthErrorValue :  0  par defaut
      *                     -2 autre erreur
      *                     -1 erreur d'authentification
      *                      1  tout va bien, authentification ok
      */
    void ssoAuthStatus(int ssoAuthErrorValue);

    /** Permet d'afficher une popup par exemple si propagation de l'authentification via alacarte */
    void ssoAuthPropagated(const QString& login);

protected:
    /** Permet d'appliquer le filtre de la liste blanche :-) */
    QNetworkReply *createRequest(const Operation& op, const QNetworkRequest &req, QIODevice *outgoingData );

public slots:
    /** Test du proxy (pour verifier s'il est joignable etc.)
      * contexte = update quand on met a jour la conf proxy et qu'on essaye une nouvelle config
      * contexte = start quand on teste le proxy au lancement de l'application
      */
    void abeProxyConfigTest(const QString& contexte);

    /** Permet d'activer ou d'interdire les popup SSO (actif par défaut)*/
    void abeSSOPopupEnable(bool enable){ m_ssoUsePopup = enable;}

private slots:
    /** Gestion de l'erreur "authentification du proxy" */
    void slotProxyAuthenticationRequired(const QNetworkProxy& proxy,QAuthenticator* authenticator);

    /** Gère l'authentification SSO */
    void slotSSOAuth(QNetworkReply* networkReply,QAuthenticator* authenticator);

    /** Gère les retours de requete SSO (login & logout) */
    void slotFinishedReplies(QNetworkReply* reply);

    void slotSSOPopupClosed();
    void slotLocalSocketMessageReceived(const QString& message);

#ifndef QT_NO_SSL
    /** Tentative de gestion des 'erreurs' ssl pour insérer nos certificats -> solution dans le constructeur
      * du manager on ajoute les certificats de manière globale
      */
    void slotSslErrors ( QNetworkReply *reply, const QList<QSslError> & errors );
#endif

    void slotInternetUnavailable(int errorCode, int elapsedTimeInMs);
    void slotInternetAvailable(int elapsedTimeInMs);

private:
    void clear();
    void p_ipcSSOLogin(const QStringList& params);

    /** Handlers pour gérer les retours des requêtes */
    void slotHandleSSoLogin(QNetworkReply *);
    void slotHandleSSoLogout(QNetworkReply *);
    void slotHandleProxyTesting(QNetworkReply *);
    void slotHandleProxyWPAD(QNetworkReply *);

private:

    QPointer<MaiaObject> m_maiaObject;

    bool m_boolSsoAuthenticatorFailed;

    /* Le nombre de fois ou on a demande le mot de passe proxy */
    int m_proxyPassAsked;

    /* Le reply qui permet de relancer la connexion sur la même page en cas de capture proxy par ex */
    QNetworkReply* m_networkReply;

    /* Un flag pour savoir si oui ou non on est authentifie: 0 on ne sait pas, 1 ok, -1 erreur */
    int m_ssoAuthentifiedStatus;

    /* Le nombre de fois ou on a demande le mot de passe sso */
    int m_passAsked;

    /* La liste des domaines DNS autorisés (si whitelist) */
    QStringList m_networkWhiteList;

    /* pour savoir si on est en train de s'authentifier, évite d'avoir les connexions en // */
    bool m_ssoPopupIsShow;

    /* Permet de désactiver totalement les popup, par ex. dans alacarte on gere le sso dans un widget intégré */
    bool    m_ssoUsePopup;
    QString m_login;
    QString m_pass;
    QString *m_proxyLogin;
    QString *m_proxyPass;

    /* Un serveur pour passer le ticket SSO aux clients */
    AbulEduLocalSocketIpcServerV1 *m_localSocketServer;
    /* Un client pour passer le ticket SSO aux clients */
    AbulEduLocalSocketIpcClientV1 *m_localSocketClient;

    /* Pour le système d'aiguillage automatique du système d'authentification */
    const char                    *m_destinationOnLoginSuccess;       /* ou on va apres etre authentifie ?  */
    QObject                       *m_destinationOnLoginSuccessObject; /* et de quel objet ?                 */
    const char                    *m_destinationOnLoginFailure;       /* ou on va apres etre authentifie ?  */
    QObject                       *m_destinationOnLoginFailureObject; /* et de quel objet ?                 */

    /* Le type de proxy */
    enumAbulEduNetworkAccessManagerProxySettings m_proxySettingsEnum;

    AbulEduNetworkTests           *m_abuleduNetworkTests;
    bool                          m_isInternetAvailable; /* Connecté au net ou pas ? */

    AbulEduSSOLoginV1             *m_abuleduSSOLogin;
};

#endif // ABULEDUNETWORKACCESSMANAGERV1_H
