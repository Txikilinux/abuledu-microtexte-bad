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

#include "abuledunetworkaccessmanagerv1.h"
#include "abuledussologinv1.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduNetworkAccessManagerV1::AbulEduNetworkAccessManagerV1(QObject *parent) :
    QNetworkAccessManager(parent),
    m_abuleduSSOLogin(0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_isInternetAvailable = false;
    clear();

    m_abuleduNetworkTests = new AbulEduNetworkTests(this);
    connect(m_abuleduNetworkTests, SIGNAL(signalError(int,int)) , this, SLOT(slotInternetUnavailable(int,int)),Qt::UniqueConnection);
    connect(m_abuleduNetworkTests, SIGNAL(signalOk(int))        , this, SLOT(slotInternetAvailable(int)), Qt::UniqueConnection);

    m_networkWhiteList.clear();
    setObjectName("abeNetworkAccessManager");

    /* Par défaut on ouvre une popup si on a besoin d'une authentification SSO */
    m_ssoUsePopup = true;

#ifndef QT_NO_SSL
    /* Ajout des certificats SSL racine de nos serveurs https (la version de Qt utilisée sous windows ne semble pas avoir ces certificats) */
    QStringList certificats;
    certificats << ":/abuledunetworkaccessmanagerv1/sslrapidssl-01" << ":/abuledunetworkaccessmanagerv1/sslgeotrust-01"
                << ":/abuledunetworkaccessmanagerv1/ssladdtrust-01" << ":/abuledunetworkaccessmanagerv1/sslpositive-01";

    QListIterator<QString> i(certificats);
    while (i.hasNext()) {
        QFile certif(i.next());
        if (certif.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QSslCertificate cert(certif.readAll());
            QList<QSslCertificate> certificates = QSslSocket().caCertificates();
            if(!certificates.contains(cert)) {
                ABULEDU_LOG_DEBUG() << "Ajout du certificat : " << certif.fileName();
                QSslSocket().addDefaultCaCertificate(cert);
            }
            else {
                ABULEDU_LOG_DEBUG() << "Certificat deja present " << certif.fileName();
            }
            certif.close();
        }
    }
#endif

    /* Passage de ticket SSO entre alacarte et les clients
     * Si on est dans un logiciel alors on est client de abuledu-alacarte-srv et on lance un "serveur" sous notre nom */
    if(abeApp->applicationName() != "abuledu-alacarte") {
        m_localSocketServer = new AbulEduLocalSocketIpcServerV1(abeApp->applicationName() + "-" + abeApp->getAbeUserLogin(), this);
        m_localSocketClient = new AbulEduLocalSocketIpcClientV1("abuledu-alacarte-serveur-" + abeApp->getAbeUserLogin(), this);
        connect(m_localSocketServer, SIGNAL(abeSignalMessageReceived(QString)), this, SLOT(slotLocalSocketMessageReceived(QString)),Qt::UniqueConnection);

        /* Et on essaye de demander a alacarte s'il n'a pas les infos SSO */
        if(m_login.isEmpty() || m_pass.isEmpty()) {
            /* On essaye de demander a alacarte s'il a pas les infos ? */
            m_localSocketClient->abeSlotSendMessageToServer("HELLO:" + abeApp->applicationName() + ":ssoRequest:","abuledu-alacarte-serveur-" + abeApp->getAbeUserLogin());
        }
    }
    else {
        m_localSocketServer = new AbulEduLocalSocketIpcServerV1("abuledu-alacarte-serveur-" + abeApp->getAbeUserLogin(), this);
        m_localSocketClient = new AbulEduLocalSocketIpcClientV1("none", this);
        connect(m_localSocketServer, SIGNAL(abeSignalMessageReceived(QString)), this, SLOT(slotLocalSocketMessageReceived(QString)),Qt::UniqueConnection);
    }

    /* Connexion sur la gestion de l'authentification si c'est necessaire */
    connect(this, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)), this, SLOT(slotProxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),Qt::UniqueConnection);

    /* Connexion sur la gestion des erreurs LDAP */
#ifndef QT_NO_SSL
    connect(this, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(slotSslErrors(QNetworkReply*,QList<QSslError>)),Qt::UniqueConnection);
#endif

    /* connexion sur l'authentification SSO */
    connect(this,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this,SLOT(slotSSOAuth(QNetworkReply*,QAuthenticator*)), Qt::UniqueConnection);

    /* Gestion PROXY */
    abeActivateProxySettings();

    m_ssoAuthentifiedStatus = -1;
}

AbulEduNetworkAccessManagerV1::~AbulEduNetworkAccessManagerV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Plantage si on laisse le delete */
    delete m_localSocketServer;
    delete m_localSocketClient;
    delete m_abuleduNetworkTests;
    delete m_maiaObject;
}

void AbulEduNetworkAccessManagerV1::setAbeLoginPass(const QString &login, const QString &pass)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_login = login;
    m_pass  = pass;
    m_ssoAuthentifiedStatus = m_passAsked = 0;
}

void AbulEduNetworkAccessManagerV1::abeSSOLogin()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Cas particulier : appelle au slot depuis une appli sans AbulEduSSOLogin */
    /* Si le m_login et m_pass est vide, la popUp n'est pas visible (car c'est elle qui a les champs à remmplir :p) */
    if(m_login.isEmpty() || m_pass.isEmpty()){

        if(!m_abuleduSSOLogin){
            m_abuleduSSOLogin = new AbulEduSSOLoginV1();
            m_ssoPopupIsShow = true;
            connect(m_abuleduSSOLogin, SIGNAL(destroyed()), this, SLOT(slotSSOPopupClosed()),Qt::UniqueConnection);
            connect(m_abuleduSSOLogin, SIGNAL(signalClosed()), this, SLOT(slotSSOPopupClosed()),Qt::UniqueConnection);
            ABULEDU_LOG_DEBUG() << "POP Up Authentification cree";
        }

        m_abuleduSSOLogin->showMaximized();
        return;
    }

    /* 20121105 : on profite de l'authentification pour récupérer l'identité de l'utilisateur vu que c'est possible de faire tout d'un coup ! */
    QNetworkRequest request;
    request.setRawHeader("User-Agent", QString(abeApp->applicationName() + "/" + abeApp->applicationVersion()).toLatin1());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    if(m_login.contains("@" + abe::ABE_LAN_SERVERNAME)) {
        request.setUrl(QUrl(abe::ABE_LAN_SERVERWEBADMINURI + "login.php"));
    }
    else {
        request.setUrl(QUrl(abe::ABE_WAN_SERVERWEBADMINURI + "login.php"));
    }
    request.url().setUserName(m_login);
    request.url().setPassword(m_pass);

    QList<QVariant> params = QList<QVariant>();
    m_maiaObject = new MaiaObject(this);
    QNetworkReply* reply = this->post(request, m_maiaObject->prepareCall("abuledu.sso.login", params).toUtf8());
    ABULEDU_LOG_DEBUG() << request.url().userName() /*<< " et " << request.url().password()*/;
    reply->setObjectName("ssoLogin");
    /*Un seul slot pour gérer les retours de requetes SSO */
    connect(this, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotFinishedReplies(QNetworkReply*)),Qt::UniqueConnection);
}

void AbulEduNetworkAccessManagerV1::abeSSOLogout()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(m_ssoAuthentifiedStatus != -1) {
        clear();
        abeApp->getAbeIdentite()->abeClear();
        m_ssoAuthentifiedStatus = -1;

        /* Fermeture de la "session" websso */
        QNetworkRequest request;
        /* Si on est sur le LAN @servecole.abuledu on se deconnecte donc sur ce serveur LAN */
        if(abeGetSSOLogin().contains("@"+abe::ABE_LAN_SERVERNAME)) {
            request.setUrl(QUrl(abe::ABE_LAN_SERVERWEBADMINURI + "logout.php"));
        }
        else {
            request.setUrl(QUrl(abe::ABE_WAN_SERVERWEBADMINURI + "logout.php"));
        }

        request.setRawHeader("User-Agent", QString(abeApp->applicationName() + "/" + abeApp->applicationVersion()).toLatin1());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        connect(this, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotFinishedReplies(QNetworkReply*)),Qt::UniqueConnection);
        QNetworkReply *reply = this->get(request);
        reply->setObjectName("ssoLogout");
    }
}

void AbulEduNetworkAccessManagerV1::slotSSOAuth(QNetworkReply *networkReply, QAuthenticator *authenticator)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "AUTHENTIFICATION DEMANDE !" << authenticator->options();

    if(networkReply->error()) {
        ABULEDU_LOG_DEBUG() << "  Erreur : " << networkReply->errorString();
    }
    if(!m_boolSsoAuthenticatorFailed){
        authenticator->setUser(m_login);
        authenticator->setPassword(m_pass);
        m_boolSsoAuthenticatorFailed = true;
    }
    else {
        networkReply->abort();
    }
}

/* @note Toutes les requetes de la classes terminent ici.
  * Il faut gérer lire l'objectName et aiguiller en fonction.
  */
void AbulEduNetworkAccessManagerV1::slotFinishedReplies(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Traitement réponse requete SSO LOGIN */
    if(reply->objectName() == "ssoLogin") {
        slotHandleSSoLogin(reply);
        return;
    }
    /* Traitement réponse requete SSO LOGOUT */
    else if(reply->objectName() == "ssoLogout"){
        slotHandleSSoLogout(reply);
        return;
    }
    /* Traitement réponse requete test proxy */
    else if(reply->objectName().contains("proxyTesting-")){
        slotHandleProxyTesting(reply);
        return;
    }
    /* Traitement réponse requete test proxy WPAD */
    else if(reply->objectName().contains("WPADRequest")){
        slotHandleProxyWPAD(reply);
        return;
    }
    /* traitement réponse requete non attendue (objectName() non connu) */
    else{
        ABULEDU_LOG_DEBUG() << "La requete n'est pas conforme au slotFinishedSSOReply" << reply->objectName();
        return; /* On drop */
    }
}

void AbulEduNetworkAccessManagerV1::slotSSOPopupClosed()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_ssoPopupIsShow = false;
    /* Si on ne s'est pas authentifié (erreur de mot de passe par exemple) et qu'on ferme la fenêtre
     * il faut remettre les variables a vide pour provoquer l'affichage de la popup lors du prochain
     * appel a SSOLogin */
    if(m_ssoAuthentifiedStatus != 1) {
        m_login = "";
        m_pass  = "";
    }
}

void AbulEduNetworkAccessManagerV1::slotLocalSocketMessageReceived(const QString &message)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Si on est alacarte */
    if(abeApp->applicationName() == "abuledu-alacarte") {
        /* Contenu du message =  HELLO:leterrier-calculment:ssoRequest*/
        QStringList params = message.split(":");

        if(params.size() > 0) {
            /* Alacarte recoit un HELLO -> on fait suivre le login/pass et on ajoute ce destinataire à notre liste de diff */
            if(params.at(0) == "HELLO") {
                ABULEDU_LOG_DEBUG() << params;

                /* On ajoute ce client dans notre liste de contacts */
                if(params.size() > 1) {
                    if(params.at(1) != "none") {
                        m_localSocketClient->abeSlotAddRemoteServer(params.at(1) + "-" + abeApp->getAbeUserLogin());
                    }
                }
                QByteArray login;
                QByteArray pass;
                if(!m_login.isEmpty()) {
                    login = m_login.toUtf8();
                    pass = m_pass.toUtf8();
                }
                else {
                    login = "anonymous";
                    pass = "none";
                }
                /* Reponse en privé = HELLO:ssoAnswer:jgkeljgieajgiaejgera6432654 */
                m_localSocketClient->abeSlotSendMessageToServer("HELLO:ssoAnswer:" + login.toBase64() + ":" + pass.toBase64(), params.at(1) + "-" + abeApp->getAbeUserLogin());
                emit ssoAuthPropagated(m_login);
            }
            /* Alacarte recoit un QUIT -> on supprime ce destinataire de notre liste de diff */
            else if(params.at(0) == "QUIT") {
                ABULEDU_LOG_DEBUG() << params;

                /* On supprime ce client de notre liste */
                if(params.size() > 1)
                    m_localSocketClient->abeSlotDelRemoteServer(params.at(1) + "-" + abeApp->getAbeUserLogin());
                QByteArray login(m_login.toUtf8());
                /* Réponse en privé au revoir = QUIT:ssoAnswer:erics */
                m_localSocketClient->abeSlotSendMessageToServer("QUIT:ssoAnswer:" + login.toBase64() + ":", params.at(1) + "-" + abeApp->getAbeUserLogin());
            }
            /* Alacarte recoit un SSOLOGIN */
            else if(params.at(0) == "IPCSSOLOGIN") {
                p_ipcSSOLogin(params);
                /* Et on propage à toutes les applications */
                /** @todo optimiser: ne pas re-envoyer a celui qui nous l'a envoyé */
                m_localSocketClient->abeSlotSendMessageToServers(message);
            }
            /* Alacarte recoit un SSOLOGOUT */
            else if(params.at(0) == "SSOLOGOUT") {

            }
        }
    }
    /* Sinon c'est qu'on est une application autre que alacarte */
    else {
        QStringList params = message.split(":");
        if(params.size() > 0) {
            /* On reçoit un HELLO, normalement le login & pass suivent */
            if(params.at(0) == "HELLO") {
                /* Contenu du message */
                QByteArray login = QByteArray::fromBase64(params.at(2).toLatin1());
                QByteArray pass  = QByteArray::fromBase64(params.at(3).toLatin1());
                ABULEDU_LOG_DEBUG()  << " Apres le Base64Decode : " << login << pass;

                /* Si alacarte a retourné des choses intéressantes ... */
                if(!login.isEmpty() && login != "anonymous") {
                    setAbeLoginPass(login,pass);
                    abeSSOLogin();
                }
            }
            /* Alerte nouvelle ouverture de session SSO */
            else if(params.at(0) == "IPCSSOLOGIN") {
                p_ipcSSOLogin(params);
            }
            /* Alerte nouvelle ouverture de session SSO */
            else if(params.at(0) == "SSOLOGOUT") {
                abeSSOLogout();
            }
        }
    }
}

/* Gestion de l'authentification proxy ... */
void AbulEduNetworkAccessManagerV1::slotProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(proxy)

    /* On cherche voir si on a pas deja l'info sauvegardee */
    QSettings settings("abuledu","AbulEduProxySettings");
    settings.beginGroup("proxy");

    /* Si c'est la 1ere fois qu'on essaye ces parametres ... ok mais
     * sinon il faut la popup (dans ce cas c'est que les enregistrements sauvegardes ne sont plus bon) */
    if((settings.value("server").toString() == this->proxy().hostName()) && (m_proxyPassAsked == 0)) {
        m_proxyLogin->clear();
        m_proxyLogin->append(settings.value("login").toString());
        m_proxyPass->clear();
        m_proxyPass->append(settings.value("pass").toString());
        QByteArray passc;
        passc.append(settings.value("pass").toString());
        authenticator->setUser(settings.value("login").toString());
        authenticator->setPassword(QByteArray::fromHex(QByteArray::fromBase64(passc)));
        QNetworkRequest redirection(m_networkReply->url());
        m_networkReply = this->get(redirection);
        /* Detection loop */
        m_proxyPassAsked++;
    }
    /* et sinon direction popup */
    else if(m_proxyPassAsked == 1) {
        AbulEduProxyPasswordDialog *dlg = new AbulEduProxyPasswordDialog(m_proxyLogin,m_proxyPass);
        dlg->setAbeLoginPassDialogNetworkReply(m_networkReply);
        dlg->setAbeLoginPassDialogAuthenticator(authenticator);
        dlg->show();
        m_proxyPassAsked++;
    }
}

#ifndef QT_NO_SSL
void AbulEduNetworkAccessManagerV1::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << errors
                        << reply->errorString() << reply->url()
                        << reply->sslConfiguration().peerCertificateChain();

    /** @todo: attention, j'active cette option a cause de  QT 4.7 qui n'a pas nos certificats racine cheap (comodo) mais
        c'est clairement une mauvaise "solution" ! */
    reply->ignoreSslErrors();
}
#endif

void AbulEduNetworkAccessManagerV1::slotInternetUnavailable(int errorCode, int elapsedTimeInMs)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(errorCode)
    Q_UNUSED(elapsedTimeInMs)
    m_isInternetAvailable = false;
}

void AbulEduNetworkAccessManagerV1::slotInternetAvailable(int elapsedTimeInMs)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(elapsedTimeInMs)
    m_isInternetAvailable = true;
}

void AbulEduNetworkAccessManagerV1::clear()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_ssoAuthentifiedStatus = m_passAsked = 0;
    m_ssoPopupIsShow = false;
    m_login.clear();
    m_pass.clear();
    m_destinationOnLoginSuccess = m_destinationOnLoginFailure = "";
    m_destinationOnLoginSuccessObject = m_destinationOnLoginFailureObject = NULL;
    m_boolSsoAuthenticatorFailed = false;
}

void AbulEduNetworkAccessManagerV1::p_ipcSSOLogin(const QStringList &params)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(params.at(0) == "IPCSSOLOGIN") {
        /* Contenu du message */
        QByteArray login = QByteArray::fromBase64(params.at(1).toLatin1());
        QByteArray pass  = QByteArray::fromBase64(params.at(2).toLatin1());
        ABULEDU_LOG_DEBUG() << "  Apres le Base64Decode : " << login << " et " << pass;

        /* Si le login n'est pas vide, qu'il n'est pas anonymous */
        if((!login.isEmpty()) && (login != "anonymous")) {
            /* Evite de tourner en récursif (auth -> ipc message auth -> auth -> ipc message auth etc.) */
            if(login != m_login && pass != m_pass) {
                setAbeLoginPass(login,pass);
                abeSSOLogin();
            }
        }
    }
}

void AbulEduNetworkAccessManagerV1::slotHandleSSoLogin(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Gestion des erreurs requete login */
    switch (reply->error()) {
    case QNetworkReply::NoError:{
        /* code 200 */
        ABULEDU_LOG_DEBUG() << "Reponse de la requete SSO LOGIN OK";
        m_ssoAuthentifiedStatus = 1; /* On est identifié */
        /* Si on a donné une destination automatique apres authentification réussie on y va */
        /** @warning Icham les objets m_destinationOnLoginFailureObject & m_destinationOnLoginSuccessObject sont toujours nuls ?! */
        /* Si on a donné une destination automatique apres authentification réussie on y va */
        if(m_destinationOnLoginSuccess != QByteArray("")) {
            QTimer::singleShot(10, m_destinationOnLoginSuccessObject, m_destinationOnLoginSuccess);
            m_destinationOnLoginSuccess = "";
        }

        /* Gestion de l'identité pour l'application;  On passe l'identité a l'application via l'objet MaiaXML */
        QString response = QString::fromUtf8(reply->readAll());
        if(m_maiaObject) {
            connect(m_maiaObject, SIGNAL(aresponse(QVariant&,QNetworkReply*)),
                    abeApp->getAbeIdentite(), SLOT(slotSetFromNetworkReply(QVariant&,QNetworkReply*)),Qt::UniqueConnection);
            m_maiaObject->parseResponse(response, reply); /* attention parseResponse supprime l'objet */
        }
        /* Reponse en privé HELLO:ssoAnswer:jgkeljgieajgiaejgera6432654 ; Si alacarte on propage pour tous les logiciels */
        QByteArray login(m_login.toUtf8());
        QByteArray pass(m_pass.toUtf8());
        if(abeApp->applicationName() == "abuledu-alacarte"){
            ABULEDU_LOG_DEBUG() << "MESSAGE IPC ALACARTE";
            m_localSocketClient->abeSlotSendMessageToServers("IPCSSOLOGIN:" + login.toBase64() + ":" + pass.toBase64());
        }
        /* Sinon, on remonte à alacarte l'information */
        else{
            ABULEDU_LOG_DEBUG()  << "MESSAGE IPC AUTRE";
            m_localSocketClient->abeSlotSendMessageToServer("IPCSSOLOGIN:" + login.toBase64() + ":" + pass.toBase64(),"abuledu-alacarte-serveur-" + abeApp->getAbeUserLogin());
        }
    }
        break;

    case QNetworkReply::OperationCanceledError:
    case QNetworkReply::AuthenticationRequiredError: /* 204 Authentification avec mauvais user/pass*/
        ABULEDU_LOG_DEBUG() << "Reponse de la requete SSO LOGIN KO, le credential n'est pas reconnu";
        m_ssoAuthentifiedStatus = -1;
        m_boolSsoAuthenticatorFailed = false;
        if(m_destinationOnLoginFailure!= QByteArray("")) {
            QTimer::singleShot(10, m_destinationOnLoginFailureObject, m_destinationOnLoginFailure);
            m_destinationOnLoginFailure = "";
        }

        break;
    default:
        qWarning("CAS DEFAUT A TRAITER pour SSOLogIn >> AbulEduNetworkAccessManagerV1::slotFinishedSSOReplies(QNetworkReply *reply)");
        break;
    } /* FIN SWITCH reply->error() */
    /* On peux supprimer la requete */
    reply->deleteLater();
    /* C'est ce signal qui nous fait changer de page (connecté à la MainWindow) */
    emit ssoAuthStatus(m_ssoAuthentifiedStatus);
}

void AbulEduNetworkAccessManagerV1::slotHandleSSoLogout(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /** @note pour Eric : pourquoi se soucier du redirect, on a déjà la réponse à notre demande de logout =) */
    switch (reply->error()) {

    case QNetworkReply::NoError:
        ABULEDU_LOG_DEBUG() << "Reponse de la requete SSO LOGOUT OK";
        m_ssoAuthentifiedStatus = 0;
        /* Si on est alacarte on propage le sso logout */
/*         if(abeApp->applicationName() == "abuledu-alacarte") {
            m_localSocketClient->abeSlotSendMessageToServers("SSOLOGOUT:");
        }
        else {
            m_localSocketClient->abeSlotSendMessageToServer("SSOLOGOUT:","abuledu-alacarte-serveur-" + abeApp->getAbeUserLogin());
        }
*/
        clear();
        break;
    default:
        ABULEDU_LOG_WARN() << "CAS DEFAUT A TRAITER pour SSOLogOut >> AbulEduNetworkAccessManagerV1::slotFinishedSSOReplies(QNetworkReply *reply)";
        break;
    }
    clearAccessCache();
   reply->deleteLater(); /* On peux supprimer la requete */
    /* C'est ce signal qui nous fait changer de page (connecté à la MainWindow) */
    emit ssoAuthStatus(m_ssoAuthentifiedStatus);
}

void AbulEduNetworkAccessManagerV1::slotHandleProxyTesting(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QApplication::restoreOverrideCursor();
    ABULEDU_LOG_DEBUG() << "Requete proxyTesting : retour de requete de test proxy recue";

    switch (reply->error()) {
        case QNetworkReply::NoError:
        {
            QApplication::restoreOverrideCursor();
            QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Configuration enregistrée"),
                                                  trUtf8("La configuration du proxy est enregistrée \n Host : %1 \n Port : %2").arg(this->proxy().hostName()).arg(this->proxy().port()));
            alertBox->show();
            /* Tout va bien, il faut enregistrer la conf proxy */
            abeSaveProxySettings();
        }
    break;

    default: /* Tous les autres cas sont des erreurs */
        ABULEDU_LOG_DEBUG() << "proxy : "<< reply->error() << reply->objectName();
        ABULEDU_LOG_WARN() << "CAS DEFAUT A TRAITER pour PROXYTEST";
        QString complement;
        if(abeApp->applicationName() != "abuledu-alacarte") {
            complement = trUtf8("\nNote: Veuillez utiliser le logiciel abuledu-alacarte pour reconfigurer vos paramètres proxy.");
        }
        /// @todo: uniquement si pas de réseau
        //        QMessageBox *alertBox=new QMessageBox(QMessageBox::Warning,trUtf8("Problème !!"),trUtf8("Impossible de joindre votre serveur proxy.\nCode d'erreur %1.%2").arg(code).arg(complement));
        //        alertBox->show();
        QApplication::restoreOverrideCursor();
        break;
    }

    reply->deleteLater();
}

void AbulEduNetworkAccessManagerV1::slotHandleProxyWPAD(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (reply->error() == QNetworkReply::NoError)
    {
        /* On en déduis que la connexion avec le proxy est ok */
        m_isInternetAvailable = true;

        /* Lectures des données issues du "reply" */
        QByteArray contenu = reply->readAll();
        if(contenu.contains("PROXY")) {
            QByteArray temp = contenu.right(contenu.length() - contenu.indexOf("PROXY ") - 6); //6 = longueur du mot "PROXY "
            QString envproxy = temp.left(temp.indexOf(";"));
            ABULEDU_LOG_DEBUG()<<"Proxy trouvé: " << envproxy;

            QRegExp exp(":");
            QStringList split = envproxy.split(exp);

            ABULEDU_LOG_DEBUG() << "Proxy detected :: " << split;

            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(split.at(0));
            proxy.setPort(split.at(1).toInt());
            QNetworkProxy::setApplicationProxy(proxy);
            this->setProxy(proxy);
            //On lance ici le test
            //abeProxyConfigTest("start");
            abeSaveProxySettings();
        }
    }
    else {
        m_isInternetAvailable = false;
    }
    reply->deleteLater();
}

void AbulEduNetworkAccessManagerV1::abeProxyConfigTest(const QString &contexte)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QUrl url;
    url.setUrl("http://www.google.fr");

    ABULEDU_LOG_DEBUG() << " proxy: on teste : " << proxy().hostName() << ":" << proxy().port() << "  proxy: avec " << url;

    QNetworkReply *reply = get(QNetworkRequest(url));
    reply->setObjectName("proxyTesting-" + contexte);
    connect(this, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotFinishedReplies(QNetworkReply*)),Qt::UniqueConnection);
}

void AbulEduNetworkAccessManagerV1::abeOnLoginSuccessGoto(QObject *objectDestination, const char *slotDestination)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< objectDestination << slotDestination;
    m_destinationOnLoginSuccess = slotDestination;
    m_destinationOnLoginSuccessObject = objectDestination;
}

void AbulEduNetworkAccessManagerV1::abeOnLoginFailureGoto(QObject *objectDestination, const char *slotDestination)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_destinationOnLoginFailure = slotDestination;
    m_destinationOnLoginFailureObject = objectDestination;
}

AbulEduNetworkAccessManagerV1::enumAbulEduNetworkAccessManagerProxySettings AbulEduNetworkAccessManagerV1::abeGetProxySettingsEnum() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_proxySettingsEnum;
}

void AbulEduNetworkAccessManagerV1::abeSetProxySettingsEnum(AbulEduNetworkAccessManagerV1::enumAbulEduNetworkAccessManagerProxySettings settings)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_proxySettingsEnum = settings;
    switch (m_proxySettingsEnum) {
    case abeProxyDisabled:{
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::NoProxy);
        QNetworkProxy::setApplicationProxy(proxy);
        this->setProxy(proxy);
    }
        break;

    case abeProxySystemEnv:{
        QNetworkProxyFactory::setUseSystemConfiguration(true);
        QString envproxy = qgetenv("http_proxy");
        if(envproxy.length() > 0)
        {
            ABULEDU_LOG_DEBUG() << "Proxy detected : " << envproxy;
            envproxy.remove("/");
            /* On découpe le proxy */
            QRegExp exp(":");
            QStringList split = envproxy.split(exp);
            ABULEDU_LOG_DEBUG() << "Proxy detected decoupe tableau : " << split;
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(split[1]);
            proxy.setPort(split[2].toInt());
            QNetworkProxy::setApplicationProxy(proxy);
            this->setProxy(proxy);
        }
    }
        break;

    case abeProxyAuto:{
        /* Détection automatique: WPAD */
        QUrl adresseWPAD("");
        QHostInfo wpadSingleResolve = QHostInfo::fromName("wpad");
        if (wpadSingleResolve.error() != QHostInfo::NoError) {
            ABULEDU_LOG_DEBUG() << "  Lookup failed (1):" << wpadSingleResolve.errorString();

            /* On essaye avec le domaine complet ... */
            QHostInfo wpadDomainResolve = QHostInfo::fromName("wpad." + QHostInfo::localDomainName());
            if (wpadDomainResolve.error() != QHostInfo::NoError) {
                ABULEDU_LOG_DEBUG()<< " Lookup failed (2): " << wpadDomainResolve.errorString();
            }
            else {
                /* On télécharge le fichier wpad pour l'analyser ... */
                adresseWPAD = "http://wpad." + QHostInfo::localDomainName() + "/wpad.dat";
            }
        }
        else {
            /* On télécharge le fichier wpad pour l'analyser ... */
            adresseWPAD = "http://wpad/wpad.dat";
        }
        if(!adresseWPAD.isEmpty()){
            QNetworkReply* reply = this->get(QNetworkRequest(adresseWPAD));
            reply->setObjectName("WPADRequest");
            connect(this, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotFinishedReplies(QNetworkReply*)),Qt::UniqueConnection);
        }
    }
        break;
    case abeProxyManual:{
        QNetworkProxy proxy;
        QSettings settings("abuledu","AbulEduProxySettings");
        settings.beginGroup("proxy");

        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(settings.value("HTTPserver","proxy").toString());
        proxy.setPort(settings.value("HTTPport","3128").toInt());
        proxy.setUser(settings.value("login","").toString());
        proxy.setPassword(settings.value("pass","").toString());

        QNetworkProxy::setApplicationProxy(proxy);
        this->setProxy(proxy);

        /* Verification qu'il est joignable */
//        m_abuleduNetworkTests->abeRunTest(proxy.hostName(),proxy.port(),2);
    }

    case abeProxyAutoURI:{
        ABULEDU_LOG_WARN() << __FILE__ <<  __LINE__ << __FUNCTION__ << "  a faire !!";
    }
        break;
    }
}

void AbulEduNetworkAccessManagerV1::abeSaveProxySettings()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QSettings settings("abuledu","AbulEduProxySettings");
    settings.beginGroup("proxy");
    settings.setValue("configuration",m_proxySettingsEnum);
    settings.setValue("HTTPserver",proxy().hostName());
    settings.setValue("HTTPport",proxy().port());
    settings.sync();

    ABULEDU_LOG_DEBUG() << "Parametres proxy == " << proxy().port() << proxy().hostName();
    abeActivateProxySettings();
}

void AbulEduNetworkAccessManagerV1::abeActivateProxySettings()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* On regarde si on a une configuration sauvegardée */
    QSettings settings("abuledu","AbulEduProxySettings");
    settings.beginGroup("proxy");
    const int value = settings.value("configuration",-1).toInt();

    switch(value) {
    case -1:
        ABULEDU_LOG_DEBUG() << "Aucune conf, recherche automatique";
        /* Aucune configuration on lance les recherches (automatique) */
        abeSetProxySettingsEnum(abeProxyAuto);
        break;
    case 0:
        ABULEDU_LOG_DEBUG() << "conf ==  abeProxyDisabled";
        abeSetProxySettingsEnum(abeProxyDisabled);
        break;
    case 1:
        ABULEDU_LOG_DEBUG() << "conf ==  abeProxyAuto";
        abeSetProxySettingsEnum(abeProxyAuto);
        break;
    case 2:
        ABULEDU_LOG_DEBUG() << "conf ==  abeProxyManual";
        abeSetProxySettingsEnum(abeProxyManual);
        break;
    case 3:
        ABULEDU_LOG_DEBUG() << "conf ==  abeProxyAutoURI";
        abeSetProxySettingsEnum(abeProxyAutoURI);
        break;
    case 4:
        ABULEDU_LOG_DEBUG() << "conf ==  abeProxySystemEnv";
        abeSetProxySettingsEnum(abeProxySystemEnv);
        break;
    }
}

void AbulEduNetworkAccessManagerV1::abeSetProxyManualSettings(const QString &serveurHTTP, const QString &portHTTP)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(serveurHTTP);
    proxy.setPort(portHTTP.toInt());
    QNetworkProxy::setApplicationProxy(proxy);
    this->setProxy(proxy);
}

void AbulEduNetworkAccessManagerV1::abeSetNetworkWhiteList(const QStringList &domains)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_networkWhiteList.clear();
    if(domains.count() > 0) {
        m_networkWhiteList.append("abuledu.net");
        m_networkWhiteList.append("abuledu.org");
        m_networkWhiteList.append("ryxeo.com");
        m_networkWhiteList.append(domains);
    }

    ABULEDU_LOG_DEBUG() << m_networkWhiteList;
}

bool AbulEduNetworkAccessManagerV1::abeIsInternetAvailable()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_isInternetAvailable;
}

QNetworkReply *AbulEduNetworkAccessManagerV1::createRequest( const Operation &op,
                                                             const QNetworkRequest &req,
                                                             QIODevice *outgoingData )
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QNetworkRequest myReq( req );
    if ( m_networkWhiteList.count() > 0 && ! m_networkWhiteList.contains(req.url().host()) ) {
        ABULEDU_LOG_DEBUG() << "  on filtre";
        myReq.setUrl( QUrl( QString("forbidden://localhost/") ) );
    }
    else {
        ABULEDU_LOG_DEBUG() << "  on laisse passer";
    }
    QNetworkReply *reply = QNetworkAccessManager::createRequest( op, myReq, outgoingData );
    return reply;
}
