/** Classe AbulEduApplicationV1
  *
  * Classe de base en version une pour les applications AbulEdu permettant de structurer
  * nos logiciels et respecter ainsi un certain nombre de choses dont l'interface publique
  * à laquelle le plugin peut se connecter pour échanger des données (logs pour le moment)
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1
  * @author 2009-2014 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2011 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2013-2014 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
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

#include <iostream>
#include "abuleduapplicationv1.h"
#include "abuleduconstantesv1.h"
#include "abuleduproxypassworddialog.h"
#include "abuledunetworkthreadv1.h"
#include "abuledunetworkaccessmanagerv1.h"
#include "abuledumessageboxv1.h"
#include "abuleduidentitesv1.h"
#include "abuledupluginloaderv1.h"

/* Les 2 includes pour le log */
using namespace AbulEduLoggingV1;
#include "abuleduloggerv1.h"
#include "abuledudestinationfilev1.h"
#ifndef DEBUG_ABULEDUAPPLICATIONV1
    #include "abuledudisableloggerv1.h"
#endif

#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
    #include "abuledumultimediasettingsv1.h"
#endif

class AbulEduApplicationV1Private
{

public:
    AbulEduApplicationV1Private(AbulEduApplicationV1 *parent, const QString& appName,
                                const QString& appVersion,
                                const QString& organizationDomain = "",
                                const QString& organizationName = "",
                                const QString& unitVersion = "1.0",
                                bool isSingleApp=true):
        q_ptr(parent)
    {
        QCoreApplication::setApplicationName(appName);
        QCoreApplication::setApplicationVersion(appVersion);
        QCoreApplication::setOrganizationName(organizationName);
        QCoreApplication::setOrganizationDomain(organizationDomain);
        m_unitVersion = unitVersion;
        m_decorRatio = 1.0;
        m_interfaceLang = QLocale::system().name().section('_', 0, 0);
        m_browserLaunched = false;
#if QT_VERSION >= 0x050200
        m_commandLineParser = 0;
#endif
    }


    ~AbulEduApplicationV1Private(){;}

private:
    AbulEduApplicationV1                     *const q_ptr;
    AbulEduNetworkAccessManagerV1            *m_nam;
    AbulEduIdentitesV1                       *m_identite;
    QString                                  m_lauchingApp;
    QTranslator                              qtTranslator;
    QTranslator                              m_appTranslator;
    QString                                  m_applicationLongName;  /** nom de l'application */
    QString                                  m_unitVersion;          /** numéro de version des modules généré par l'application */
    QString                                  m_interfaceLang;        /** Langue interface, de type "fr" */
    float                                    m_decorRatio;           /** Facteur de mise a l'echelle */
    bool                                     m_browserLaunched;      /** Evite d'ouvrir plusieurs fenetres de navigateur */
    QString                                  m_linuxPackageFormat;   /** Le format du package Linux */
    QString                                  m_linuxDistrib;         /** La distribution Linux */
    QString                                  m_homePath;             /** Home User compat profil itinérant */
    QString                                  m_userName;             /** Le login de l'utilisateur */
    QString                                  m_os;                   /** Le système */
    QString                                  m_uniqueID;             /** Un ID unique */
    QSharedMemory                            *m_sharedMemory;        /** Espace memoire partagee (interne) */
    AbulEduApplicationV1::AbulEduNetworkInfo m_networkInfo;          /** Structure privée pour gérer les infos network */
    int m_argc;
    char **m_argv;

#if QT_VERSION >= 0x050200
    QCommandLineParser              *m_commandLineParser;
#endif

#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
    /// Un pointeur vers un objet partagé de paramétrage pour l'infra multimedia
    QSharedPointer<AbulEduMultiMediaSettingsV1> m_abuleduMultiMediaSettings;
#endif

    Q_DECLARE_PUBLIC(AbulEduApplicationV1)
};


//20140312 android + qt 5.3 et abuledulogger ... ca plante mechant
#ifndef Q_OS_ANDROID
/* Gestion du debug avec un QMessageLogContext only Qt5 */
#if QT_VERSION > 0x050000
void abulEduLogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toUtf8();
    switch (type) {
    case QtDebugMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::DebugLevel).stream() << context.file << '@' << context.line << localMsg.constData();
        break;
    case QtWarningMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::WarnLevel).stream() << context.file << '@' << context.line << localMsg.constData();
        break;
    case QtCriticalMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::ErrorLevel).stream() << context.file << '@' << context.line << localMsg.constData();
        break;
    // case QtInfoMsg:
    //    AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::InfoLevel).stream() << context.file << '@' << context.line << localMsg.constData();
    //    break;
    case QtFatalMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::FatalLevel).stream() << context.file << '@' << context.line << localMsg.constData();
        abort();
    }
}
#else
/* @notes Sous Qt4, le contexte du message n'existe pas, donc à vous de gérer le debug de la fonction@line */
void abulEduLogOutput(QtMsgType type, const char *msg){
    switch (type) {
    case QtDebugMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::DebugLevel).stream() /*<< context.file << '@' << context.line */<< msg;
        break;
    case QtWarningMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::WarnLevel).stream() /*<< context.file << '@' << context.line */<< msg;
        break;
    case QtCriticalMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::ErrorLevel).stream() /*<< context.file << '@' << context.line */<< msg;
        break;
    case QtFatalMsg:
        AbulEduLoggingV1::AbulEduLoggerV1::Helper(AbulEduLoggingV1::FatalLevel).stream() /*<< context.file << '@' << context.line */<< msg;
        abort();
    }

}
#endif
#endif

/* Gestion des styles specifiques pour tablettes */
#ifdef __ABULEDUTABLETTEV1__MODE__
/** styles speciaux pour tablettes, en particulier les scrollbar qu'on double de largeur (au moins) */
class MyTabletStyle : public QProxyStyle
{
public:
    int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
    {
        if (metric == PM_ScrollBarExtent)
            return 45;
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
    QSize sizeFromContents(QProxyStyle::ContentsType type, const QStyleOption* option, const QSize& contentsSize, const QWidget* widget) const
    {
        if (type == QProxyStyle::CT_ScrollBar)
            return contentsSize;
        return QProxyStyle::sizeFromContents(type, option, contentsSize, widget);
    }
};

#endif
/// ==========================================================================================================

/* /////////////////////////////////////////////////////////////////////////////////////////////////
 *                                     CONSTRUCTEUR DE LA CLASSE
 *  @WARNING IS 20143007 -> L'ordre d'instanciation est hyper-important !
 *  Certains objets nécessitent que d'autres soient instanciés avant eux (example m_identité à besoin de m_nam,
 *      mais m_nam doit attendre que toutes les variables locales (en fait au moins m_username soit définie)
 * ///////////////////////////////////////////////////////////////////////////////////////////////*/

AbulEduApplicationV1::AbulEduApplicationV1(int &argc, char *argv[], const QString &appName,
                                           const QString &appVersion,
                                           const QString &organizationDomain,
                                           const QString &organizationName,
                                           const QString &unitVersion, bool isSingleAPP):
    QtSingleApplication(argc, argv),
    d(new AbulEduApplicationV1Private(this, appName, appVersion, organizationDomain, organizationName, unitVersion, isSingleAPP))
{

    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    d->m_argc = argc;
    d->m_argv = argv;
    p_automaticGetUserNameFromENV(); /* on affecte la variable m_username dans cette fonction qui gere les OS */

    d->m_nam        = new AbulEduNetworkAccessManagerV1(this);
    d->m_identite   = new AbulEduIdentitesV1(this);

#ifndef Q_OS_ANDROID
    d->m_sharedMemory = new QSharedMemory("abeUpdateRequest-" + appName, this);
#endif

#ifdef __ABULEDU_MODE_TIMED__
    ABULEDU_LOG_DEBUG() << "MODE DEMO ON";
    int finCompteur = 0;
    if(__ABULEDU_MODE_TIMED__){
        if(QString(__ABULEDU_MODE_TIMED__).toInt() <= 0){
            ABULEDU_LOG_DEBUG() << "MODE DEMO: temps defini <= 0 --> temps defaut = 5 min(s)";
            finCompteur = (5*60000);
        }
        else{
            finCompteur = QString(__ABULEDU_MODE_TIMED__).toInt() * 60000;
            ABULEDU_LOG_DEBUG() << "MODE DEMO: temps defini " << finCompteur / 60000 << " min(s)";
        }
    }
    else{
        ABULEDU_LOG_DEBUG() << "MODE DEMO: pas de temps defini --> temps defaut = 5 min(s)";
        ABULEDU_LOG_DEBUG() << "NO TIME";
        finCompteur = (5*60000);
    }
    QTimer::singleShot(finCompteur, this, SLOT(slotDemoQuit()));
#endif

    /* /////////////////////////////////////////////////////////////////////////////////////////////////
     *                                     GESTION VARIABLE m_homepath
     * Note: sous windows poste client authentifié sur le réseau abuledu, le P: pointe sur servecole\\login
     * ///////////////////////////////////////////////////////////////////////////////////////////////*/
#if defined Q_OS_WIN
    QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
    settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
    d->m_homePath = settings.value("Personal").toString();
#elif defined Q_OS_ANDROID
    //Sur android on se place sur la sdcard et on fait un sous dossier abuledu
    d->m_homePath = QString(getenv("EXTERNAL_STORAGE")) + "/abuledu";
    QDir homeTest;
    homeTest.setPath(d->m_homePath);
    if(!homeTest.exists()) {
        homeTest.mkpath(d->m_homePath);
    }
#else
    d->m_homePath = QDir::homePath();
#endif
    ABULEDU_LOG_DEBUG() << "Valeur m_homePath [" << d->m_homePath << "]";

    /* Eric 5.08.13 : pour android pas de clavier, qt appelle celui d'android tout seul */
#if !defined(Q_OS_ANDROID)
#ifdef __ABULEDUTABLETTEV1__MODE__
    /* Si on est en mode tablette on cache le curseur de la souris
     * et on active automatiquement le clavier virtuel */
    QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    //setStyle(new MyTabletStyle);
    setAbeApplicationVirtualKeyboard(true);
#else
    setAbeApplicationVirtualKeyboard(false);
#endif
#endif

    /* CHARGEMENT FONT ANDIKA */
    if(!installFontAndika()){
        ABULEDU_LOG_DEBUG() << "Chargement police Andika [KO]";
    }

    if(organizationName.isEmpty()){
        ABULEDU_LOG_WARN() << "L'appel au constructeur AbulEduApplicationV1 ne specifie pas d'organizationName ... le chargement de plugins personnels ne sera pas automatique.";
    }

    /* Initialisation des différentes variables */
    if( ! d->m_appTranslator.load(appName + "_" + d->m_interfaceLang, "./lang")) {
        ABULEDU_LOG_DEBUG() << "Erreur de chargement du traducteur : " << appName + "_" + d->m_interfaceLang;
    }
    else {
        this->installTranslator(&d->m_appTranslator);
        ABULEDU_LOG_DEBUG() << "Chargement du traducteur : " << appName + "_" + d->m_interfaceLang;
    }

    /* Pour avoir les boutons des boîtes de dialogue dans la langue locale (fr par défaut) */
    QString systemLocaleStorage;
#ifdef Q_OS_WIN
    /* Sous windows on stocke toutes les traductions "systeme" dans c:/windows/abuledu-alacarte/lang/qt5 pour qt5 */
    /* C'est le paquet de alacarte qui se charge de copier tous les fichiers dedans */
    QString qt_version(QT_VERSION_STR);
    systemLocaleStorage = "c:/windows/abuledu-alacarte/lang/qt" + qt_version.left(1);
#else
    systemLocaleStorage = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
    if( ! d->qtTranslator.load("qt_" + d->m_interfaceLang, systemLocaleStorage)) {
        ABULEDU_LOG_DEBUG() << "Erreur de chargement du traducteur systeme : " << "qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    }
    else {
        this->installTranslator(&d->qtTranslator);
        ABULEDU_LOG_DEBUG() << "Chargement du traducteur : qt_" <<  d->m_interfaceLang;
    }

    int already_requested = 0;

    getAbeIdentite()->setObjectName("AbulEduApplicationV1/AbulEduIdentitesV1");
    connect(getAbeIdentite(), SIGNAL(signalLoaded()), this, SLOT(slotIdentityLoaded()));

    /* Pas la peine d'aller plus loin si le developpeur n'a pas fait son boulot ! */
    if(applicationName().isEmpty() || applicationVersion().isEmpty() ) {
        ABULEDU_LOG_WARN() << trUtf8("AbulEduApplicationV1:: Attention: applicationName ou applicationVersion non défini !!!");
        ABULEDU_LOG_WARN() << applicationName() << "::" << applicationVersion();
        return;
    }

    // ============================================================================== memoire partagee et requete reseau update
    /* @note :android n'a pas de gestion de memoire partagee */
#ifndef Q_OS_ANDROID
    if (d->m_sharedMemory->isAttached())
        detach();

    /* Creation du segment de memoire partagee */
    if(d->m_sharedMemory->create(sizeof(already_requested))) {
        ABULEDU_LOG_DEBUG() << "taille mem partagee :" << d->m_sharedMemory->size();
        /* On cherche pour voir s'il y a une mise a jour de l'application de disponible
         * On lance notre requete reseau et on gere la suite
         * Il faudrait pouvoir lancer plusieurs requetes en // sur differents supports
         * avec ou sans proxy, en http et en https, voir meme sur d'autres domaines */
        QUrl urlUpdates("http://updates.ryxeo.com/application/" + applicationName() + "/version/" + applicationVersion() + "/os/" + getAbeApplicationThisOS() + "/dist/" + linuxDistrib() + "/format/xml/uid/" + getAbeUniqueUID() + "/age/" + QString::number(execAge()).toLatin1().toBase64() + "/");
        connect(getAbeNetworkAccessManager(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotOnlineUpdateRequest(QNetworkReply*)), Qt::UniqueConnection);
        QNetworkReply *networkReply = getAbeNetworkAccessManager()->get(QNetworkRequest(urlUpdates));
        networkReply->setObjectName("OnlineUpdateRequest");
        connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotReplyError(QNetworkReply::NetworkError)), Qt::UniqueConnection);
        /* On écrit le flag comme quoi la requete reseau est deja partie ... merci de ne pas relancer cette requete :) */
        d->m_sharedMemory->lock();
        already_requested = 1;
        ABULEDU_LOG_DEBUG() << "ecriture dans la mem partagee: requete reseau lancee";
        memcpy(d->m_sharedMemory->data(), &already_requested, sizeof(already_requested));
    }
    else {
        ABULEDU_LOG_DEBUG() << "zone mem partagee deja existante, la requete reseau a deja ete lancee";
        /* si le segment existe deja on l'utilise */
        if(d->m_sharedMemory->error() == QSharedMemory::AlreadyExists) {
            if(d->m_sharedMemory->attach()) {
                /* lecture des donnees */
                d->m_sharedMemory->lock();
                already_requested = *(bool *)d->m_sharedMemory->constData();
            }
            else {
                ABULEDU_LOG_DEBUG() << "erreur sur sharedMemory :: " << d->m_sharedMemory->errorString();
            }
        }
        else {
            ABULEDU_LOG_DEBUG() << "erreur sur sharedMemory :: " << d->m_sharedMemory->errorString();
        }
    }
    d->m_sharedMemory->unlock();
    // ============================================================================== fin de gestion memoire partagee et requete reseau update
#endif

    // ============================================================================== logiciel de plus de six mois
    /* @note: android n'aime pas les threads et celui ci fait decrocher l'application */
#ifndef Q_OS_ANDROID
    QFileInfo fi(qApp->applicationFilePath());
    QDateTime ageDuFichier(fi.created());
    QDateTime agePlusSixMois(ageDuFichier.addMonths(24));
    QUrl urlSixMois("http://updates.ryxeo.com/application/" + applicationName() + "/version/" + applicationVersion() + "/os/" + getAbeApplicationThisOS() + "/dist/" + linuxDistrib() + "/format/xml/uid/" + getAbeUniqueUID() + "/age/" + QString::number(execAge()).toLatin1().toBase64() + "/");
    /* Si l'application a plus de six mois ... on ouvre un browser sur la page speciale
     * pour s'assurer que des très vieux logiciels ne sont plus utilisés
     * passé a 24 mois, on reviendra sur l'idée de 6 mois mais ça fait quand meme un peu court 6 mois, 2 ans c'est déjà
     * un peu plus compréhensible
     */

    if(agePlusSixMois < QDateTime::currentDateTime()) {
        AbulEduNetworkThreadV1 *browser = new AbulEduNetworkThreadV1(urlSixMois);
        browser->start();
        d->m_browserLaunched = true;
    }
#endif
    // ============================================================================== fin logiciel de plus de six mois


    // ============================================================================== création du pointeur vers l'abuledu multimedia
#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
    d->m_abuleduMultiMediaSettings = QSharedPointer<AbulEduMultiMediaSettingsV1>(new AbulEduMultiMediaSettingsV1(), &QObject::deleteLater);
    d->m_abuleduMultiMediaSettings->setObjectName("abuleduapplicationmultimediasettingsv1");
#endif

    // ============================================================================== plus de x lancements sans reponse de serveur

    // ============================================================================== fin plus de x lancements sans reponse de serveur
    if(AbulEduPluginLoaderV1::getInstance()->loadPlugins(qApp->applicationDirPath() + "/plugins")){
        ABULEDU_LOG_DEBUG() << "Plugins:" << AbulEduPluginLoaderV1::getInstance()->getPluginsAvailable();
    }
}

void AbulEduApplicationV1::initLogger()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* 1. Initialisation dans le constructeur (obligatoire car constructeur privé et appel au construteur par défaut =) ) */
    AbulEduLoggerV1::instance().setLoggingLevel(AbulEduLoggingV1::TraceLevel);

    /* 2. On va lui donner 2 destinations (file & console)-> abeApp->getAbeHomepath() + "/leterrier/abuledu-alacarte/logs/" */
    /* 2.1 Console Output */
    DestinationPtrV1 debugDestination(AbulEduLogDestinationFactoryV1::MakeDebugOutputDestination());
    AbulEduLoggerV1::instance().addDestination(debugDestination);

    /* 2.2 File Output */
    QDir logsDirectory(abeApp->getAbeHomepath() + "/leterrier/abuledu-alacarte/logs/");
    if(! logsDirectory.exists()) {
        ABULEDU_LOG_DEBUG() << logsDirectory.absolutePath();
        ABULEDU_LOG_DEBUG() << logsDirectory.mkpath(logsDirectory.absolutePath());
    }

    if(logsDirectory.exists() && logsDirectory.isReadable()) {
        const QString logFilePath(logsDirectory.absolutePath() + QDir::separator() + abeApp->applicationName()+".log");

        /* Petite protection, il faut que le fichier soit ouvrable */
        if(QFile(logFilePath).open(QIODevice::ReadWrite)){
            ABULEDU_LOG_DEBUG() << "Ouverture fichier log OK...[" << logFilePath << "]";
            /* 3. Activation de la rotation, Taille Max 1Mo, Fichier Log MAX = 5 */
            DestinationPtrV1 fileDestinatination(AbulEduLogDestinationFactoryV1::MakeFileDestination(logFilePath, EnableLogRotation, MaxSizeBytes(4096*1024), MaxOldLogCount(5)));
            AbulEduLoggerV1::instance().addDestination(fileDestinatination);

        }
        else{
            ABULEDU_LOG_WARN() << "Ouverture fichier log impossible...[" << logFilePath << "]";
            ABULEDU_LOG_WARN() << "Log Fichier impossible...";
        }
    }
    else{
        ABULEDU_LOG_WARN() << "Tentative de création Arborescence Logs impossible...";
        ABULEDU_LOG_WARN() << "Log Fichier impossible...";
    }

    ABULEDU_LOG_INFO() << "DEMARRAGE DU PROGRAMME";
    ABULEDU_LOG_INFO() << "---------------------------------------------------";
    ABULEDU_LOG_INFO() << "INFO BUILD     :"  << __DATE__ << __TIME__ << "Qt Version :" << QT_VERSION_STR ;
    ABULEDU_LOG_INFO() << "INFO PROGRAM   :" << abeApp->applicationName() << abeApp->applicationVersion();
    ABULEDU_LOG_INFO() << "---------------------------------------------------";

    //    /* Pour la desactivation =) */
    //        m_abuledulogger.setLoggingLevel(AbulEduLoggingV1::OffLevel);
    //    for (int i = 0;i < 10000000;++i) {
    //        ABULEDU_LOG_ERROR() << QString::fromUtf8("Ce message ne sera pas visible non plus");
    //    }
    //    ABULEDU_LOG_FATAL() << "Fatal error qu'on voit pas!";
    //    /* Reactivation */
    //    m_abuledulogger.setLoggingLevel(AbulEduLoggingV1::TraceLevel);
}

bool AbulEduApplicationV1::installFontAndika()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* QFontDatabase::addApplicationFont() return -1 si echec
     * @see https://redmine.ryxeo.com/issues/3792 */
    int idFontAndika = QFontDatabase::addApplicationFont(":/abuleduapplicationv1/fonts/andika");
    if(idFontAndika != -1){
        foreach (const QString var, QFontDatabase::applicationFontFamilies(idFontAndika)) {
            if(var == "Andika"){
                QApplication::setFont(QFont(var));
                return true;
            }
        }
    }
    return false;
}

const AbulEduApplicationV1::AbulEduNetworkInfo &AbulEduApplicationV1::abeGetNetworkInfos()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    foreach(const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces())
    {
        foreach (const QNetworkAddressEntry &addrEntry, netInterface.addressEntries()) {
            if(addrEntry.ip().protocol() == QAbstractSocket::IPv4Protocol && addrEntry.ip() != QHostAddress(QHostAddress::LocalHost)){
                if(!addrEntry.broadcast().isNull()){ /* #4232 l'adresse sous Windows n'est pas correcte (plusieurs IP remontées) */
                    d->m_networkInfo.HostIp    = addrEntry.ip();
                    d->m_networkInfo.Subnet    = addrEntry.netmask().toString();
                    d->m_networkInfo.MacAdress = netInterface.hardwareAddress();
                }
            }
        }
    }
    ABULEDU_LOG_DEBUG() << QString("IP: %1 ; Subnet: %2 ; MAC : %3").
                           arg(d->m_networkInfo.HostIp.toString()).
                           arg(d->m_networkInfo.Subnet.toString()).
                           arg(d->m_networkInfo.MacAdress);
    return d->m_networkInfo;
}

void AbulEduApplicationV1::abeCenterWindow(QWidget *w)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!w){
        ABULEDU_LOG_WARN() << "widgetToCenter est un pointeur NULL";
        return;
    }

    /* evite de deplacer la fenetre sur la barre du bas windows */
#ifdef Q_OS_WIN
    QDesktopWidget *widget = QApplication::desktop();
    int desktop_width = widget->width();
    int desktop_height = widget->height();
    w->move((desktop_width - w->width())/2, (desktop_height - w->height())/2);
#else
    /* et prise en compte des multi ecrans sous linux */
    QDesktopWidget *widget = QApplication::desktop();
    int desktop_width  = widget->screen(widget->screenNumber(w))->width();
    int desktop_height = widget->screen(widget->screenNumber(w))->height();
    w->move((desktop_width - w->width())/2, (desktop_height - w->height())/2);
#endif
}

void AbulEduApplicationV1::slotDemoQuit()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QMessageBox::information(abeApp->topLevelWidgets().first(),trUtf8("Mode Démo"), trUtf8("L'application va maintenant quitter !"), QMessageBox::Ok);
    abeApp->quit();
}

void AbulEduApplicationV1::slotIdentityLoaded()
{
    //    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
    //                        << getAbeIdentite()->abeGetLogin()
    //                        << getAbeIdentite()->abeGetEmail()
    //                        << getAbeIdentite()->abeGetNom()
    //                        << getAbeIdentite()->abeGetPreNom()
    //                        << getAbeIdentite()->abeGetVille();
}


AbulEduApplicationV1::~AbulEduApplicationV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* On detache la memoire partagee */
    detach();

    d->m_identite->deleteLater();
    d->m_nam->deleteLater();
#if QT_VERSION >= 0x050200
    if(d->m_commandLineParser) delete(d->m_commandLineParser);
#endif
#ifndef Q_OS_ANDROID
    d->m_sharedMemory->deleteLater();
#endif
    d->q_ptr->deleteLater();

    delete(d);
}

const QString &AbulEduApplicationV1::getAbeApplicationLongName()
{
    return d->m_applicationLongName;
}

void AbulEduApplicationV1::setAbeApplicationLongName(const QString &s)
{
    d->m_applicationLongName = s;
}

const QString &AbulEduApplicationV1::getAbeApplicationUnitVersion()
{
    return d->m_unitVersion;
}

void AbulEduApplicationV1::setAbeApplicationUnitVersion(const QString &version)
{
    d->m_unitVersion = version;
}

const QString &AbulEduApplicationV1::getAbeApplicationInterfaceLang()
{
    return d->m_interfaceLang;
}

void AbulEduApplicationV1::setAbeApplicationInterfaceLang(const QString &lang)
{
    d->m_interfaceLang = lang;
}

void AbulEduApplicationV1::setAbeApplicationDecorRatio(const float &ratio)
{
    d->m_decorRatio = ratio;
}

const float &AbulEduApplicationV1::getAbeApplicationDecorRatio()
{
    return d->m_decorRatio;
}

const QString &AbulEduApplicationV1::getAbeApplicationThisOS()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#if defined(Q_OS_MAC)
    d->m_os="osx";
#elif defined(Q_OS_WIN)
    d->m_os="windows";
#elif defined(Q_OS_ANDROID)
    d->m_os="android";
#elif defined(Q_OS_LINUX)
    QString arch(""); //pour l'instant i386 / amd64
    /* L'architecture ... pour l'instant on ne fait pas les ARM / Sparc etc. */
#ifdef __x86_64__
    arch = "amd64";
#elif __arm__
    arch = "arm";
#else
    arch = "i386";
#endif
    /* On regroupe tout ca linux-deb-lucid-amd64 */
    d->m_os="linux-" + linuxDistrib() + "-" + linuxPackageFormat() + "-" + arch;
#endif
    return d->m_os;
}

void AbulEduApplicationV1::p_linuxAnalyse()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QStringList dists_debian;
    QStringList dists_ubuntu_lts;
    QStringList dists_ubuntu;
    QStringList dists_mint;
    QStringList dists_mint_parent;
    QStringList dists_fedora;
    QStringList dists_mageia;

    dists_debian     << "squeeze" << "wheezy" << "jessy" << "sid";
    dists_ubuntu_lts << "dapper"  << "hardy"  << "lucid"  << "precise" << "trusty";
    dists_ubuntu     << "warty"   << "hoary"  << "breezy" << "edgy"    << "feisty" << "gutsy" << "intrepid" << "jaunty" << "karmik" << "maverick" << "natty" << "oneiric" << "quantal" << "raring" << "saucy" << "trusty";
    dists_mint       << "maya"    << "nadia"  << "olivia";
    dists_mint_parent<< "precise" << "quantal"<< "raring";
    dists_mageia     << "thornicroft";

    /* Signature des fedora */
    QFile fedora("/etc/fedora-release");

    /* Pour l'instant c'est uniquement ubuntu qui propose ca et mageia 2 aussi */
    QFile lsb("/etc/lsb-release");
    if(lsb.exists()) {
        if (lsb.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QRegExp release("DISTRIB_CODENAME=(.*)");
            while (!lsb.atEnd() && d->m_linuxDistrib.isEmpty()) {
                QByteArray line = lsb.readLine();
                release.indexIn(line.trimmed());
                if(release.cap(1) != "") {
                    ABULEDU_LOG_DEBUG() << "linuxPackageFormat On a trouve le LSB :: " << line << " ici : " << release.cap(1).trimmed();
                }
                if(dists_debian.contains(release.cap(1)) || dists_ubuntu.contains(release.cap(1)) || dists_ubuntu_lts.contains(release.cap(1))) {
                    d->m_linuxDistrib = release.cap(1).trimmed();
                    d->m_linuxPackageFormat = "deb";
                }
                if(dists_mint.contains(release.cap(1))) {
                    /* On recupere le nom de code de la ubuntu compatible */
                    int indice = dists_mint.indexOf(release.cap(1));
                    d->m_linuxDistrib = dists_mint_parent.at(indice);
                    d->m_linuxPackageFormat = "deb";
                }
                if(dists_mageia.contains(release.cap(1))) {
                    d->m_linuxDistrib = release.cap(1).trimmed();
                    d->m_linuxPackageFormat = "rpm";
                }

            }
            lsb.close();
        }
    }
    else if(fedora.exists()) {
        d->m_linuxPackageFormat = "rpm";
    }
    else {
        ABULEDU_LOG_DEBUG() << "lancer la commande 'lsb_release -a";
    }

    if(d->m_linuxPackageFormat.isEmpty() || d->m_linuxDistrib.isEmpty()) {
        ABULEDU_LOG_WARN() << trUtf8("La distribution ou le format de paquet ne sont pas connus ... merci de nous contacter pour qu'on puisse ajouter votre systeme dans notre liste: devteam@ryxeo.com");
    }
}

const QString& AbulEduApplicationV1::getAbeUniqueUID()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QByteArray retour("000102030405");
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
                if ( interface.hardwareAddress() != "00:00:00:00:00:00" && entry.ip().toString().contains(".")) {
                    retour.clear();
                    retour.append(interface.hardwareAddress().remove(":").toLower());
                }
            }
        }
    }
    d->m_uniqueID = retour.toHex().toBase64();
    return d->m_uniqueID;
}

const QString &AbulEduApplicationV1::getAbeUserLogin()
{
    /** @todo 20160302:ich:debug en Android plante au QASSERT */
    //Q_ASSERT(!d->m_userName.isEmpty());
    return d->m_userName;
}

int AbulEduApplicationV1::execAge()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFileInfo fi(qApp->applicationFilePath());
    int ageDuFichier(fi.created().daysTo(QDateTime::currentDateTime()));
    ABULEDU_LOG_DEBUG() << "age du fichier :: " << ageDuFichier;
    return ageDuFichier;
}

const QString& AbulEduApplicationV1::linuxDistrib()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(d->m_linuxDistrib.isEmpty())
        p_linuxAnalyse();
    return d->m_linuxDistrib;
}

const QString& AbulEduApplicationV1::linuxPackageFormat()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(d->m_linuxPackageFormat.isEmpty())
        p_linuxAnalyse();
    return d->m_linuxPackageFormat;
}

AbulEduNetworkAccessManagerV1 *AbulEduApplicationV1::getAbeNetworkAccessManager()
{
    return d->m_nam;
}

AbulEduIdentitesV1 *AbulEduApplicationV1::getAbeIdentite()
{
    return d->m_identite;
}

QString &AbulEduApplicationV1::getAbeHomepath()
{
    return d->m_homePath;
}

void AbulEduApplicationV1::slotOnlineUpdateRequest(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Si ce n'est pas pour moi je droppe */
    if(reply->objectName() != "OnlineUpdateRequest") {
        return;
    }

    disconnect(d->m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotOnlineUpdateRequest(QNetworkReply*)));

    /* Pas d'erreur ? */
    if (reply->error() == QNetworkReply::NoError) {
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            QString titre("");
            QString desc("");
            QString link("");
            QString lang("");
            QDomDocument *xml = new QDomDocument();
            xml->setContent(reply->readAll());
            ABULEDU_LOG_DEBUG() << "  download XML update " << xml->toString();

            QDomElement xmlElem = xml->documentElement();
            QDomNode node = xmlElem.firstChild();
            while(!node.isNull()) {
                QDomElement element = node.toElement(); /* On essaie de convertir le node en element */
                if(!element.isNull()) {
                    /* Le node est bien un élément. */
                    if(element.tagName()=="application") {
                        QDomNode node=element.firstChild(); /* On parcourt le document à partir du tag "application" */
                        while(!node.isNull()) {
                            QDomElement elem=node.toElement(); /* On cherche le titre du "Channel" */
                            if(elem.tagName()=="title") {
                                titre = elem.text();
                            }
                            else if((elem.tagName()=="link")) {
                                link = elem.text();
                            }
                            else if((elem.tagName()=="description")) {
                                desc = elem.text();
                            }
                            else if((elem.tagName()=="language")) {
                                lang = elem.text();
                            }
                            node=node.nextSibling();
                        }
                    }
                    node = node.nextSibling();
                }
            }

            /* On affiche une boite d'information (popup) à l'utilisateur ... */
            if(titre.length()) {
                QWidget* top = 0;
                if(qApp->topLevelWidgets().size() > 0){
                    top = qApp->topLevelWidgets().last();
                }
                AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(titre, desc,true,top);
                msgBox->show();
                /* Quand on ferme la fenetre on lance le signal END -- evite une collision entre le splascreen et une popup de mise a jour disponible */
                connect(msgBox, SIGNAL(destroyed()), this, SIGNAL(signalApplicationMessageEnd()),Qt::UniqueConnection);
                /* Fenetre de message ouverte, on informe tout le monde */
                emit signalApplicationMessageStart();
            }
            delete xml;
        }
        /* S'il s'agit de redirections ... */
        else if (v >= 300 && v < 400)
        {
            /* Get the redirection url */
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            /* Because the redirection url can be relative, we have to use the previous one to resolve it */
            newUrl = reply->url().resolved(newUrl);
            QNetworkRequest redirection(newUrl);
            QNetworkReply *networkReply = d->m_nam->get(redirection);
            connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotReplyError(QNetworkReply::NetworkError)),Qt::UniqueConnection);
            return; /* to keep the manager for the next request */
        }
    }
    reply->deleteLater();
}

void AbulEduApplicationV1::slotReplyError(QNetworkReply::NetworkError code)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << code;
    sender()->deleteLater();
}

void AbulEduApplicationV1::p_automaticGetUserNameFromENV()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << d->m_userName << "(start)" << objectName();
#ifdef Q_OS_WIN
    d->m_userName = getenv("USER");
    if(d->m_userName.isEmpty()) {
        d->m_userName = getenv("USERNAME");
    }
#endif
#ifdef Q_OS_LINUX
    d->m_userName = getenv("USER");
    if(d->m_userName.isEmpty()) {
        d->m_userName = getenv("USERNAME");
    }
#endif
#ifdef Q_WS_MAC
    d->m_userName = getenv("USER");
    if(d->m_userName.isEmpty()) {
        d->m_userName = getenv("USERNAME");
    }
#endif
}

void AbulEduApplicationV1::detach()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#ifndef Q_OS_ANDROID
    if (!d->m_sharedMemory->detach()) {
        ABULEDU_LOG_DEBUG() << "  Unable to detach from shared memory.";
    }
    if(QFile::exists(d->m_sharedMemory->nativeKey())){
            qDebug() << "YES" << d->m_sharedMemory->parent();
    }
#endif
}

void AbulEduApplicationV1::setAbeApplicationVirtualKeyboard(bool enabled)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(enabled) {
#if defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
#if QT_VERSION < 0x050000
        m_abuleduVirtualKeyboard = new AbulEduVirtualKeyboardV1(this);
        qApp->setInputContext(m_abuleduVirtualKeyboard);
        m_abuleduVirtualKeyboard->abeSetEnabled(enabled);
#endif
#endif
    }
    else {
#if defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
#if QT_VERSION < 0x050000
        QInputContext *qic = QInputContextFactory::create(QLatin1String("xim"), this);
        qApp->setInputContext(qic);
#endif
#endif
    }
}

#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
QSharedPointer<AbulEduMultiMediaSettingsV1> AbulEduApplicationV1::getAbeApplicationMultiMediaSettings()
{
    return d->m_abuleduMultiMediaSettings;
}
#endif

void AbulEduApplicationV1::slotProcessErrorHandler(QProcess::ProcessError processError)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeApplicationProcessError(processError,sender()->objectName());
}

void AbulEduApplicationV1::slotAppliFinished(int exitCode, QProcess::ExitStatus status)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        <<"L'application "+d->m_lauchingApp+" a quitté avec le code "<<exitCode << status;
    //Q_ASSERT_X(status == QProcess::NormalExit,"Application","L'application s'est arrêtée brutalement !");
}

void AbulEduApplicationV1::runQProcess(const QString &executable, const QString &name, const QString &workingDirectory, const QStringList &arguments)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(arguments)
    QProcess *p = new QProcess(this);
    p->setObjectName(name);
    connect(p, SIGNAL(started()), this, SLOT(slotAppliStarted()), Qt::UniqueConnection);
    connect(p, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotProcessErrorHandler(QProcess::ProcessError)), Qt::UniqueConnection);
    connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotAppliFinished(int, QProcess::ExitStatus)), Qt::UniqueConnection);

    if(!workingDirectory.isNull()){
        p->setWorkingDirectory(workingDirectory);
    }
    //    p->setStandardErrorFile(QDir::tempPath() + "/" +  m_name.trimmed() + ".log"); //! Peut rendre caduque le QProcess (pas de "/" dans le chemin...)

#ifdef Q_OS_WIN
    QString windowsExecPath = "C:";
    windowsExecPath.append(QDir::separator()).append("Program Files").append(QDir::separator()).append("AbulEdu");
    QString exe = windowsExecPath+QDir::separator()+executable;
    ABULEDU_LOG_DEBUG() << " ------------ 1 ---------------- "<<exe;
    /* Pour le test de l'exitence du fichier,
 * on teste si l'executable se termine par ".exe"
*/
    if(!executable.endsWith(".exe", Qt::CaseSensitive))
    {
        ABULEDU_LOG_DEBUG() << "l'executable ne se termine pas avec .exe";
        exe.append(QString(".exe"));
        ABULEDU_LOG_DEBUG() << " ------------ 2 ---------------- "<<exe;
    }

    QFileInfo fi(exe);
    ABULEDU_LOG_DEBUG() << "Recherche de l'executable";
    if(fi.exists()){
        ABULEDU_LOG_DEBUG() << "L'executable existe - QProcess::setWorkingDirectory... OK ";
        p->setWorkingDirectory(fi.absolutePath()); //! Gestion du répertoire de travail pour les applications ayant des ressources non liés à l'exécutable (qrc)
    }
    p->start(exe);
    emit signalAppLaunched(name); //! le signal QProcess::started() sous windows ne semble pas fonctionner... A voir.
#else
    p->start(executable,arguments);
#endif
}

QString AbulEduApplicationV1::sluggify(QString str)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << str;

    /* On fait le ramasse miettes a l'ancienne tant qu'on n'a pas de /TRANSLIT/ */
    str = str.toLower();
    str.replace(QString::fromUtf8("à"),"a");
    str.replace(QString::fromUtf8("â"),"a");
    str.replace(QString::fromUtf8("ä"),"a");
    str.replace(QString::fromUtf8("é"),"e");
    str.replace(QString::fromUtf8("ê"),"e");
    str.replace(QString::fromUtf8("ë"),"e");
    str.replace(QString::fromUtf8("è"),"e");
    str.replace(QString::fromUtf8("ï"),"i");
    str.replace(QString::fromUtf8("î"),"i");
    str.replace(QString::fromUtf8("ö"),"o");
    str.replace(QString::fromUtf8("ô"),"o");
    str.replace(QString::fromUtf8("ù"),"u");
    str.replace(QString::fromUtf8("û"),"u");
    str.replace(QString::fromUtf8("ü"),"u");
    str.replace(QString::fromUtf8("ç"),"c");
    str.replace(QString::fromUtf8("œ"),"oe");

    /* tout ce qui n'est pas de a à z et 0 à 9 devient - */
    str.replace(QRegExp("[^a-z0-9]"), "-");
    /* et on remplace les suites de -- par un seul - */
    str.replace(QRegExp("\\-\\-+"), "-");

    ABULEDU_LOG_DEBUG()  << str;
    return str;
}

#if QT_VERSION >= 0x050200
void AbulEduApplicationV1::initCommandLineParser()
{
    getAbeCommandLineParser()->setApplicationDescription(QCoreApplication::translate("main",d->m_applicationLongName.toLocal8Bit()));
    getAbeCommandLineParser()->addHelpOption();
    getAbeCommandLineParser()->addVersionOption();

    /* Option --nolog -n */
    QCommandLineOption noLogOption(QStringList() << "n" << "nolog", QCoreApplication::translate("main", "No debug and No log."));
    getAbeCommandLineParser()->addOption(noLogOption);

    /* Option --user or -u */
    QCommandLineOption loginOption(QStringList() << "u" << "user",
                                   QCoreApplication::translate("main",
                                                               "The user mail adress for authentification. This option must be followed by the --pass option"),
                                   QCoreApplication::translate("main", "user@provider.xx"), "");
    getAbeCommandLineParser()->addOption(loginOption);

    /* Option --user or -u */
    QCommandLineOption passOption(QStringList()<< "p" << "password",
                                  QCoreApplication::translate("main",
                                                              "The user passworword for authentification."),
                                  QCoreApplication::translate("main", "secretpass"), "");
    getAbeCommandLineParser()->addOption(passOption);

    getAbeCommandLineParser()->process(abeApp->arguments());
    QString login = d->m_commandLineParser->value(loginOption);
    QString pass  = d->m_commandLineParser->value(passOption);

    /* Pas de login, mais un pass --> showHelp() */
    if(login.isEmpty() && !pass.isEmpty())
        getAbeCommandLineParser()->showHelp(0);

    if(!login.isEmpty()){
        QRegExp loginRegExp("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}\\b");
        loginRegExp.setCaseSensitivity(Qt::CaseInsensitive);
        loginRegExp.setPatternSyntax(QRegExp::RegExp);

        /* on a un login mais pas de pass --> showHelp() */
        if(!loginRegExp.exactMatch(login) || pass.isEmpty()){
            getAbeCommandLineParser()->showHelp(0);
        }
        /* sinon on a un pass et un login -> on peut se connecter */
        d->m_nam->abeSetSSOLogin(login);
        d->m_nam->abeSetSSOPassword(pass);

        /* On remplace le pass par des 'x' */
         for(int i = 0; i < d->m_argc; ++i){
            // On récupère le password dans les arguments
            if(pass == d->m_argv[i] ){
                char * passChar = d->m_argv[i];
                // Puis on boucle sur chaque caractère pour le remplacer
                for(char* it = passChar; *it; ++it) {
                    *it = 'x';
                }
            }
        }

       /* On lance une connection  */
        d->m_nam->abeSSOLogin();

        /* Creation d'une QEventLoop pour attendre le statut de la demande d'authentification */
        QEventLoop loopSSOLogin;
        connect(abeApp->getAbeNetworkAccessManager(), SIGNAL(ssoAuthStatus(int)), &loopSSOLogin, SLOT(quit()));
        loopSSOLogin.exec();
    }

    /* IS @warning appel à initLogger après instanciat° variable m_homepath sinon log indisponible */
    /* 20140312 android + qt 5.3 et abuledulogger ... ca plante mechant */
#ifndef Q_OS_ANDROID
    /* @date 20140722 - Sirat Icham - Possibilité de desactiver les logs
     * @see https://redmine.ryxeo.com/issues/3791 */
    if(!getAbeCommandLineParser()->isSet(noLogOption)){ /* Si le parser ne trouve pas l'option == on active le log */
        initLogger();
#if QT_VERSION > 0x050000
        qInstallMessageHandler(abulEduLogOutput);
#else
        qInstallMsgHandler(abulEduLogOutput);
#endif
    }
    else{
        ABULEDU_LOG_DEBUG() << "Option --nolog is active";
    }
#endif
}

QCommandLineParser *AbulEduApplicationV1::getAbeCommandLineParser()
{
    if(!d->m_commandLineParser){
        d->m_commandLineParser = new QCommandLineParser();
    }
    return d->m_commandLineParser;
}
#endif
