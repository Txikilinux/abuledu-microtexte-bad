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

#include "abuledussologinv1.h"
#include "maiaXmlRpcClient.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
#include "abuledudisableloggerv1.h"
#endif

/**
 * @brief AbulEduSSOLoginV1::AbulEduSSOLoginV1
 * @todo
 *     - hideEvent -> Stopper les tests de reseau
 *     - truUtf8 pour les setText
 */

AbulEduSSOLoginV1::AbulEduSSOLoginV1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduSSOLoginV1),
    m_rpcWebadminResetPass(0),
    m_logoutInProgress(0),
    m_nam(0),
    m_webadminPassAsked(0),
    m_abuleduNetworkTest(0),
    m_ssoLoginSettings(0),
    m_mailRegExp(0),
    m_domaineRegExp(0),
    m_loginCompleter(0),
    m_domainCompleter(0)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    /* Instanciation RegExp pour les mails */
    m_mailRegExp = new QRegExp("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z0-9.-]{2,}\\b");
    m_mailRegExp->setCaseSensitivity(Qt::CaseInsensitive);
    m_mailRegExp->setPatternSyntax(QRegExp::RegExp);

    /* Instanciation RegExp pour les domaines */
    m_domaineRegExp = new QRegExp("@[A-Z0-9.-]+\\.[A-Z0-9.-]{2,}\\b");
    m_domaineRegExp->setCaseSensitivity(Qt::CaseInsensitive);
    m_domaineRegExp->setPatternSyntax(QRegExp::RegExp);

    /* Récupèration le network access manager */
    m_nam = abeApp->getAbeNetworkAccessManager();
    connect(m_nam, SIGNAL(ssoAuthStatus(int)), this, SLOT(slotSSOAuthStatus(int)), Qt::UniqueConnection);

    /* Un flag pour savoir si l'action de login ou logout est en cours */
    m_logoutInProgress = false;

    /* Les URL magiques pour changer le mot de passe */
    m_rpcWebadminResetPass = new MaiaXmlRpcClient(QUrl(abe::ABE_WAN_SERVERWEBADMINURI + "resetpassword.php"), this);

    m_petitOuGrand = abePetitSSO;

    /* Gestion du QSetting */
#ifdef Q_OS_ANDROID
    /* Comme je veux que ca soit partage entre toutes les applications ... sous android il faut ruser */
    QDir dirLoginSettings(abeApp->getAbeHomepath() + "/leterrier/");
    if(! dirLoginSettings.exists()) {
        dirLoginSettings.mkpath(abeApp->getAbeHomepath() + "/leterrier/");
    }
    m_ssoLoginSettings = new QSettings(abeApp->getAbeHomepath() + "/leterrier/ssoLogin.conf", QSettings::IniFormat, this);
    ABULEDU_LOG_DEBUG() << "  fichier login: " << m_ssoLoginSettings->fileName();
#else
    m_ssoLoginSettings = new QSettings("abuledu","ssologin",this);
#endif

    /* Gestion Completer Login */
    QStringList loginDejaUtilises(m_ssoLoginSettings->value("LastLogins").toStringList());
    m_loginCompleter = new QCompleter(loginDejaUtilises, this);
    m_loginCompleter->setCaseSensitivity(Qt::CaseInsensitive);
#ifdef __ABULEDUTABLETTEV1__MODE__
    m_loginCompleter->setCompletionMode(QCompleter::InlineCompletion);
#endif
    ui->leLogin->setCompleter(m_loginCompleter);

    /* Gestion Completer Domaine */
    QStringList domainesDejaUtilises(m_ssoLoginSettings->value("LastDomains").toStringList());
    m_domainCompleter = new QCompleter(domainesDejaUtilises, this);
    m_domainCompleter->setCaseSensitivity(Qt::CaseInsensitive);
#ifdef __ABULEDUTABLETTEV1__MODE__
    m_domainCompleter->setCompletionMode(QCompleter::InlineCompletion);
#endif
    ui->leSSODomainValue->setCompleter(m_domainCompleter);

    /* On affiche le dernier domaine connu */
    if(!abeGetDomainesList().isEmpty())
        ui->lblAbulEduSSODomainDisplay->setText(abeGetDomainesList().last());
}

AbulEduSSOLoginV1::~AbulEduSSOLoginV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    delete ui;

    /* Force l'ecriture des donnees */
    m_ssoLoginSettings->sync();
    QFileInfo fi(m_ssoLoginSettings->fileName());
    if(! fi.exists()) {
        ABULEDU_LOG_DEBUG() << " error : " << fi.absoluteFilePath() << " does not exists !";
    }
    else {
        ABULEDU_LOG_DEBUG() << " OK : " << fi.absoluteFilePath() << " exist !";
    }

    if(m_rpcWebadminResetPass)
        delete m_rpcWebadminResetPass;
    if(m_abuleduNetworkTest)
        delete m_abuleduNetworkTest;
    if(m_ssoLoginSettings)
        delete m_ssoLoginSettings;
}

void AbulEduSSOLoginV1::updateLoginCompleter(const QStringList &listeLogins)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(! m_loginCompleter)
        return ;
    QStringListModel *model = (QStringListModel*)(m_loginCompleter->model());
    if(model)
        model->setStringList(listeLogins);
}

void AbulEduSSOLoginV1::updateDomainCompleter(const QStringList &listeDomains)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(! m_domainCompleter)
        return ;
    QStringListModel *model = (QStringListModel*)(m_domainCompleter->model());
    if(model)
        model->setStringList(listeDomains);
}

void AbulEduSSOLoginV1::abeFocus()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(m_petitOuGrand == abePetitSSO) {
        ui->leLogin->setFocus();
    }
}

void AbulEduSSOLoginV1::abeSetPetitOuGrandEnum(AbulEduSSOLoginV1::enumAbulEduSSOLOgin petitOuGrand)
{
    Q_UNUSED(petitOuGrand)
}

void AbulEduSSOLoginV1::abeSetButtonCloseVisible(const bool visible)
{
    ABULEDU_LOG_TRACE() << visible;
    ui->btnClose->setVisible(visible);
}

// //////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       GESTION RPC
//
// //////////////////////////////////////////////////////////////////////////////////////////////////////
void AbulEduSSOLoginV1::rpcNewPasswordRequested(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QApplication::restoreOverrideCursor();

    switch (reply->error()) {
    case QNetworkReply::NoError:
        /* cas 1 : utilisateur connu */
        if(v.toMap().value("status").toString() == "200") {
            ABULEDU_LOG_TRACE() << "Demande nouveau pass utilisateur connu";
            abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/ok"), QString::fromLatin1(v.toMap().value("Message").toByteArray()), ui->pageAbulEduSSOLogin);
        }
        /* cas 2 : utilisateur inconnu */
        else {
            ABULEDU_LOG_TRACE() << "Demande nouveau pass utilisateur inconnu";
            abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur)"), QString::fromLatin1(v.toMap().value("Message").toByteArray()), ui->pageAbulEduSSOLogin);
        }

        break;
    default:
        /* On log l'erreur */
        ABULEDU_LOG_ERROR() << reply->error() << reply->errorString();
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur)"), trUtf8(("Erreur requête réseau. Veuillez vérifier vos paramètres réseau")) , ui->pageAbulEduSSOLogin);
        break;
    }
}

void AbulEduSSOLoginV1::rpcNewAccountRequested(QVariant &v, QNetworkReply *reply)
{
    QApplication::restoreOverrideCursor();
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    switch (reply->error()) {
    case QNetworkReply::NoError:
        /* cas 1 : utilisateur non connu -> demande ok */
        if(v.toMap().value("status").toString() == "200") {
            ABULEDU_LOG_TRACE() << "Demande nouveau pass utilisateur connu";
            abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/ok"), QString::fromLatin1(v.toMap().value("Message").toByteArray()), ui->pageAbulEduSSOLogin);
        }
        /* cas 2 : utilisateur deja connu -> demande ko (c'est une demande de nouveau mdp deguisée) */
        else {
            ABULEDU_LOG_TRACE() << "Création de compte utilisateur connu ";
            abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), QString::fromLatin1(v.toMap().value("Message").toByteArray()), ui->pageAbulEduSSOLogin);
        }

        break;
    default:
        /* On log l'erreur */
        ABULEDU_LOG_ERROR() << reply->error() << reply->errorString();
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), trUtf8(("Erreur requête réseau.\n Veuillez vérifier vos paramètres réseau")), ui->pageAbulEduSSOLogin);
        break;
    }
}

void AbulEduSSOLoginV1::rpcFault(const int code, const QString &errorMessage)
{
    ABULEDU_LOG_ERROR() << code << errorMessage;
    QApplication::restoreOverrideCursor();

    /* Voici le type d'erreur :
     * -32300 "Error downloading https://webadminxml.abuledu.net/resetpassword.php - server replied: Internal Server Error" */

    /* Tentative de réouverture d'une session SSO en cas de timeout */
    if(code == -32300) {
        m_nam->abeSSOLogin();
    }
}

// ////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       GESTION NETWORK TEST
//
// //////////////////////////////////////////////////////////////////////////////////////////////////////
void AbulEduSSOLoginV1::slotNetworkTestError(int code, int elapsedTime)
{
    Q_UNUSED(code)
    Q_UNUSED(elapsedTime)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->lblServeurPing->setPixmap(QPixmap(":/abuledussologinv1/serveur-rouge"));

    ui->btnLogin->setEnabled(false);
    ui->btnOuvrirUnCompte->setEnabled(false);
    ui->btnPasswordOublie->setEnabled(false);

    ui->btnLogin->setStyleSheet("color : gray;");
    ui->btnOuvrirUnCompte->setStyleSheet("color : gray;");
    ui->btnPasswordOublie->setStyleSheet("color : gray;");
}

void AbulEduSSOLoginV1::slotNetworkTestOk(int elapsedTime)
{
    Q_UNUSED(elapsedTime)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->lblServeurPing->setPixmap(QPixmap(":/abuledussologinv1/serveur-vert"));

    ui->btnLogin->setEnabled(true);
    ui->btnOuvrirUnCompte->setEnabled(true);
    ui->btnPasswordOublie->setEnabled(true);

    ui->btnLogin->setStyleSheet("color : white;");
    ui->btnOuvrirUnCompte->setStyleSheet("color : white;");
    ui->btnPasswordOublie->setStyleSheet("color : white;");
}

// ////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       GESTION AUTHENTIFICATION
//
// //////////////////////////////////////////////////////////////////////////////////////////////////////
void AbulEduSSOLoginV1::slotSSOAuthStatus(const int code)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << code;

    QApplication::restoreOverrideCursor();

    switch (code) {
    case -1:
        /* Login incorrect */
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), trUtf8("Identifiant ou mot de passe incorrect ..."), ui->pageAbulEduSSOLogin);
        break;

    case 1:
        /* Gestion du statut */
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/ok"),
                                        trUtf8("Connexion réussie pour l'utilisateur %1.").arg(m_nam->abeGetSSOLogin()), ui->pageAbulEduSSOLogin);
        /* On ajoute le login */
        abeAddLoginForNextLogin(ui->leLogin->text());
        /* On ajoute le domaine */
        ABULEDU_LOG_DEBUG() << ui->leLogin->text();
        if(!ui->leLogin->text().isEmpty()) {
            if(ui->leLogin->text().contains('@')){
                abeAddDomaineForNextLogin('@' + ui->leLogin->text().split('@', QString::SkipEmptyParts).last());
            }
        }
        /* C'est bon, on peut le cacher car on le construit qu'une fois cf bug #3454 (https://redmine.ryxeo.com/issues/3454) */
        if(!parent()) { hide(); }
        /* On nettoie l'interface (login/pass) */
        slotCleanSSOLoginPass();
        break;

    default:
        break;
    }

    emit signalSSOAuthDone(code);
}

void AbulEduSSOLoginV1::abeAddLoginForNextLogin(const QString &login)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << login;

    /* Récupération de la liste des logins utilisés */
    QStringList listeLoginConnus(m_ssoLoginSettings->value("LastLogins").toStringList());

    /* On vérifie que le login n'est pas contenu */
    if(!listeLoginConnus.contains(login)){
        ABULEDU_LOG_DEBUG() <<  "Login inconnu - Tentative d'ajout...";
        /* On garde que 30 logins -> suppression du dernier */
        if(listeLoginConnus.count() > 30)
            listeLoginConnus.removeFirst();

        /* Puis on ajoute le login */
        listeLoginConnus << login;

        m_ssoLoginSettings->setValue("LastLogins", listeLoginConnus);
        m_ssoLoginSettings->sync();
        ABULEDU_LOG_DEBUG() <<  "Ajout [" << login << "]";

        /* Comme on a ajouté un login, il faut en informer le completer */
        updateLoginCompleter(listeLoginConnus);
    }
}

void AbulEduSSOLoginV1::on_btnLogin_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QString login = ui->leLogin->text().trimmed();

    /* Ajout du domaine à la volée si non communiqué */
    QString domaine = ui->lblAbulEduSSODomainDisplay->text();
    /* Alors on a un domaine */
    if(domaine.contains('@')) {
        /* Si le login n'a pas de domaine, on ajoute le dernier connu (celui affiché) */
        if(!login.contains('@')){
            if(!login.endsWith(domaine)){
                login += domaine;
            }
        }
    }

    /* On ne lance pas d'authentification si le mail est pas valide est que le mot de passe est vide */
    if(!m_mailRegExp->exactMatch(login)){
        ABULEDU_LOG_DEBUG() << "REGEXP MAIL KO";
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), QString("Veuillez entrer une adresse mail valide"), ui->pageAbulEduSSOLogin);
        return;
    }
    ABULEDU_LOG_DEBUG() << "REGEXP MAIL OK";

    const QString mdp = ui->lePassword->text().trimmed();
    if(mdp.trimmed().isEmpty()){
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), QString("Veuillez saisir votre mot de passe"), ui->pageAbulEduSSOLogin);
        return;
    }

    m_nam->setAbeLoginPass(login, mdp);
    m_nam->abeSSOLogin();

    /* On supprime le mot de passe mais on garde le login (on peut se tromper) */
    ui->lePassword->clear();
}

void AbulEduSSOLoginV1::on_btnPasswordOublie_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOMotDePasseOublie);
}

void AbulEduSSOLoginV1::on_btnOuvrirUnCompte_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOCreerCompte);
}

void AbulEduSSOLoginV1::on_btnCreerUnCompte_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* On ne lance pas d'authentification si le mail est pas valide est que le mot de passe est vide */
    const QString login = ui->leMailOuvrirCompte->text().trimmed();
    if(!m_mailRegExp->exactMatch(login)){
        ABULEDU_LOG_DEBUG() << "REGEXP MAIL KO";
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), trUtf8("Veuillez entrer une adresse mail valide"), ui->pageAbulEduSSOCreerCompte);
        return;
    }
    ABULEDU_LOG_DEBUG() << "REGEXP MAIL OK";

    /* On lance la requête de création de compte */
    QList<QVariant> params;
    params << login;
    m_rpcWebadminResetPass->call("abuledu.resetpassword.createNewAccount", params,
                                 this, SLOT(rpcNewAccountRequested(QVariant&, QNetworkReply*)),
                                 this, SLOT(rpcFault(int, const QString &)));

    //1. côté serveur -> vérifie que l'email est ok
    //1.bis le serveur retourne un mot de passe dans son XMLRPC (comme ça l'utilisateur peut bosser tout de suite)
    //1.ter envoie un mail avec une adresse pour confirmer l'ouverture du compte
    //2. lorsque l'utilisateur confirme l'ouverture du compte -> ouvre le compte sur la boutique
    //2.bis et demande prénom + nom
    //2.ter affiche offre spéciale
}

void AbulEduSSOLoginV1::showEvent(QShowEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(!m_abuleduNetworkTest){
        ABULEDU_LOG_TRACE() << "Construction du testNetwork";
        m_abuleduNetworkTest = new AbulEduNetworkTests(this);
        connect(m_abuleduNetworkTest,SIGNAL(signalError(int,int)), this, SLOT(slotNetworkTestError(int,int)), Qt::UniqueConnection);
        connect(m_abuleduNetworkTest,SIGNAL(signalOk(int)), this, SLOT(slotNetworkTestOk(int)), Qt::UniqueConnection);
    }

    ABULEDU_LOG_TRACE() << "On lance les tests Network";
    m_abuleduNetworkTest->abeRunTest("auth.abuledu.net",443,10);

    /* A chaque showEvent
     * Page par defaut -> pageLogin
     * Nettoyage des champs login mdp
     * Nettoyage des messages de status */

    /* Page par défaut */
    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOLogin);
    ui->leLogin->clear();
    ui->lePassword->clear();

    /* Gestion Affichage pop-up ou intégré */
    if(parent() == 0) {
        ABULEDU_LOG_TRACE()  << "NO Parent: on ouvre une popup ...";
        /* pas de |Qt::SplashScreen); sinon un clic dessus le cache & pas de |Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);
         * sinon on garde la sso sur tous les bureaux linux ! */
        setWindowState(Qt::WindowActive);
        QImage background;
#if (QT_VERSION < 0x050000)
        background = QPixmap(QPixmap::grabWindow(QApplication::desktop()->winId())).toImage();
#else
        if(abeApp->screens().size() > 0)
        {
            background = abeApp->screens()[0]->grabWindow(QApplication::desktop()->winId()).toImage();
        }
#endif
        // algo de "grisage des pixels" par ligne
        for (int ii = 0; ii < background.height(); ii++) {
            QRgb *pixel = reinterpret_cast<QRgb*>(background.scanLine(ii));
            QRgb *end = pixel + background.width();
            for (; pixel != end; pixel++) {
                int gray = qGray(*pixel);
                *pixel = QColor(qGray(gray), qGray(gray), qGray(gray)).rgb();
            }
        }

        QPalette palette = this->palette();
        QBrush brush(background);
        palette.setBrush(QPalette::Background,brush);
        setPalette(palette);
        raise();
    }
    else{
        ABULEDU_LOG_TRACE()  << "Parent: on s'integre dans le paysage ...";
        /*Dans le cas contraire c'est qu'on est embarqué en tant que widget, il faut donc s'assurer que le bouton close n'est pas visible */
        ui->btnClose->hide();
    }
    event->accept();
}

void AbulEduSSOLoginV1::hideEvent(QHideEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(m_abuleduNetworkTest){
        ABULEDU_LOG_TRACE() << "Coupure tests NetWork" ;
        m_abuleduNetworkTest->abeStopTest();
    }

    event->accept();
}

void AbulEduSSOLoginV1::resizeEvent(QResizeEvent *event)
{
    if(parent() == 0){
        setGeometry(QApplication::desktop()->availableGeometry());
        abeApp->abeCenterWindow(ui->frameCentral);
    }
    QWidget::resizeEvent(event);
}

void AbulEduSSOLoginV1::slotAbeSSOShowPageLogin()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOLogin);
}

void AbulEduSSOLoginV1::slotAbeSSOShowPageCreateAccount()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOCreerCompte);
}

void AbulEduSSOLoginV1::slotAbeSSOShowPageRetrievePassword()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOMotDePasseOublie);
}

void AbulEduSSOLoginV1::slotAbeSSOShowPageParameters()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOParametres);
}

void AbulEduSSOLoginV1::on_btnClose_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_abuleduNetworkTest->abeStopTest();

    emit signalClosed();
    close();
}

void AbulEduSSOLoginV1::on_lePassword_returnPressed()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    on_btnLogin_clicked();
}

void AbulEduSSOLoginV1::actualiseListeVignettesUtilisateurs(QListWidget *lw)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Uniquement si on a pas déjà affiché tous les comptes */
    if(lw->count() > 0) {
        return;
    }

    QStringList loginDejaUtilises(m_ssoLoginSettings->value("LastLogins").toStringList());
    for(int i = 0; i < loginDejaUtilises.count(); i++) {
        if(loginDejaUtilises.at(i).split("@").at(0) != "") {
            QListWidgetItem *itemIcone = new QListWidgetItem(loginDejaUtilises.at(i).split("@").at(0));
            itemIcone->setData(Qt::UserRole+1,loginDejaUtilises.at(i));

            /* On cache le texte ... c'est rapide et le moins pire que j'ai trouvé
             * QFont invisible(abeApp->font().family(),12);
             * itemIcone->setFont(invisible); */
            QFileInfo ficone;
            ficone.setFile(abeApp->getAbeHomepath() + "/leterrier/avatars/" + loginDejaUtilises.at(i) + ".jpg");
            /* Si l'icone n'existe pas on ... */
            QIcon icone;
            if(ficone.exists()) {
                icone = QIcon(ficone.absoluteFilePath());//pour la mettre  à coté de l'item
            }
            else {
                icone = QIcon(":/abuleduidentitesv1/avatars/defaut");//pour la mettre  à coté de l'item
            }
            itemIcone->setIcon(icone); // ajout de la petite icone sur l'item
            lw->insertItem(i,itemIcone);
        }
    }
}

void AbulEduSSOLoginV1::slotSSOStatusChanged(const int code)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "code : " << code;

    switch (code) {
    case 1:
        /* Si @servecole -> authentification locale */
        if(m_nam->abeGetSSOLogin().contains("@" + abe::ABE_LAN_SERVERNAME)) {
            m_rpcWebadminResetPass->setUrl(QUrl(abe::ABE_LAN_SERVERWEBADMINURI + "resetpassword.php"));
        }

        break;

    default:
        ABULEDU_LOG_DEBUG() << "DEFAULT SWITCH" <<code;
        break;
    }

}

void AbulEduSSOLoginV1::slotCleanSSOLoginPass()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->leLogin->clear();
    ui->lePassword->clear();
}

void AbulEduSSOLoginV1::on_btnMotDePasseOublieAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOLogin);
    ui->leMailMotDePasseOublie->clear();
}

void AbulEduSSOLoginV1::on_btnCreerUnCompteAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOLogin);
}

void AbulEduSSOLoginV1::on_btnMotDePasseOublie_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    const QString login = ui->leMailMotDePasseOublie->text().trimmed();
    /* Petit contrôle de l'adresse mail */
    if(!m_mailRegExp->exactMatch(login)){
        ABULEDU_LOG_DEBUG() << "REGEXP MAIL KO";
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), trUtf8("Veuillez entrer une adresse mail valide"), ui->pageAbulEduSSOMotDePasseOublie);
        return;
    }
    ABULEDU_LOG_DEBUG() << "REGEXP MAIL OK";

    /* La requete part, petit loading */
    QApplication::setOverrideCursor(Qt::WaitCursor);

    /* Affichage du statut */
    abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), trUtf8("Envoi de la demande en cours..."), ui->pageAbulEduSSOMotDePasseOublie);

    /* On envoie la requete "Mot de passe oublié". Requete a faire vers le webadmin/portail -> XMLRPC */
    QList<QVariant> params;
    params << login;
    m_rpcWebadminResetPass->call("abuledu.resetpassword.sendNewPassword", params,
                                 this, SLOT(rpcNewPasswordRequested(QVariant&, QNetworkReply*)),
                                 this, SLOT(rpcFault(int, const QString &)));
}

void AbulEduSSOLoginV1::abeSSOLoginDisplayStatusMessage(const QPixmap &icone, const QString &message, QWidget *pageAReafficher)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* On affiche le message */
    ui->lblStatusPicto->setPixmap(icone);
    ui->lblStatusMessage->setText(message);
    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOStatusMessage);
    /* Puis on appelle le slot de nettoyage avec la bonne page à réafficher */
    if(pageAReafficher){
        if(pageAReafficher == ui->pageAbulEduSSOLogin)
            connect(ui->btnStatusMessageOK, SIGNAL(clicked()), this, SLOT(slotAbeSSOShowPageLogin()));
        else if(pageAReafficher == ui->pageAbulEduSSOCreerCompte)
            connect(ui->btnStatusMessageOK, SIGNAL(clicked()), this, SLOT(slotAbeSSOShowPageCreateAccount()));
        else if(pageAReafficher == ui->pageAbulEduSSOMotDePasseOublie)
            connect(ui->btnStatusMessageOK, SIGNAL(clicked()), this, SLOT(slotAbeSSOShowPageRetrievePassword()));
        else if(pageAReafficher == ui->pageAbulEduSSOParametres)
            connect(ui->btnStatusMessageOK, SIGNAL(clicked()), this, SLOT(slotAbeSSOShowPageParameters()));
    }

    /* Dans tous les cas, au click sur le bouton ok, on nettoie le status */
    connect(ui->btnStatusMessageOK, SIGNAL(clicked()), this, SLOT(slotCleanStatusMessage()));
}

void AbulEduSSOLoginV1::slotCleanStatusMessage()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->lblStatusMessage->clear();
    ui->lblStatusPicto->setPixmap(QPixmap(":/abuledussologinv1/attention"));
}

// //////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       GESTION DOMAINES (PAGE PARAMETRE)
//
// //////////////////////////////////////////////////////////////////////////////////////////////////////
void AbulEduSSOLoginV1::abeAddDomaineForNextLogin(const QString &domaine)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "Domaine : " << domaine;

    QString domaineARajouter = domaine;
    domaineARajouter.remove(".auth.abuledu.net");

    /* Et on sauvegarde le domaine dans la liste */
    QStringList listeDomainesConnus(m_ssoLoginSettings->value("LastDomains").toStringList());

    /* Si on connait déja le domaine, on le supprime (pour le mettre en 1ere position) */
    for(int i = 0; i < listeDomainesConnus.count() ; i++){
        /* On le supprime */
        if(listeDomainesConnus[i] == domaineARajouter) {
            listeDomainesConnus.removeAt(i);
        }
    }

    /* On garde que 5 domaines -> suppression du dernier */
    if(listeDomainesConnus.count() > 5)
        listeDomainesConnus.removeFirst();

    /* Puis ajout du domaine à la fin */
    listeDomainesConnus << domaineARajouter;

    ui->lblAbulEduSSODomainDisplay->setText(listeDomainesConnus.last());

    m_ssoLoginSettings->setValue("LastDomains", listeDomainesConnus);
    m_ssoLoginSettings->sync();
}

QStringList AbulEduSSOLoginV1::abeGetDomainesList()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ ;
    return m_ssoLoginSettings->value("LastDomains").toStringList();
}

bool AbulEduSSOLoginV1::abeDelDomaine(QString domaine)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "Domaine : " << domaine;

    /* Au cas où */
    domaine.remove(".auth.abuledu.net");
    /* Et on sauvegarde le domaine dans la liste */
    QStringList domainesDejaUtilises(m_ssoLoginSettings->value("LastDomains").toStringList());
    domainesDejaUtilises.removeOne(domaine);
    m_ssoLoginSettings->setValue("LastDomains", domainesDejaUtilises);
    m_ssoLoginSettings->sync();
    return true;
}

void AbulEduSSOLoginV1::on_btnAbulEduSSOParametres_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOParametres);
}

void AbulEduSSOLoginV1::on_btnParametresAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->AbulEduSSOLoginStackedWidget->setCurrentWidget(ui->pageAbulEduSSOLogin);
}

void AbulEduSSOLoginV1::on_btnParametresValider_clicked()
{
    QString domaine = ui->leSSODomainValue->text().trimmed();
    /* Petit controle sur le @ du début */
    if(!domaine.startsWith('@')){
        domaine = '@'+ domaine;
    }
    ABULEDU_LOG_DEBUG() << "Domaine" << domaine;

    if(!m_domaineRegExp->exactMatch(domaine)){
        abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/erreur"), trUtf8("Veuillez entrer un domaine valide"), ui->pageAbulEduSSOParametres);
        return;
    }

    /* Ajout du domaine */
    abeAddDomaineForNextLogin(domaine);

    /* Affichage Ajout domaine */
    abeSSOLoginDisplayStatusMessage(QPixmap(":/abuledussologinv1/ok"), trUtf8("Domaine ajouté avec succès"), ui->pageAbulEduSSOLogin);

    ABULEDU_LOG_DEBUG() << abeGetDomainesList();
}
