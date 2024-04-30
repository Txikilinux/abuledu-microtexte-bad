/** Classe de base en version une pour les applications AbulEdu permettant de structurer
  * nos logiciels et respecter ainsi un certain nombre de choses
  *
  * @file abuleduapplicationv1.h
  * @version 1
  * @brief L'utilisation de abuleduapplicationv1 permet d'automatiser les différentes choses suivantes:
  * - spécifier le "nom long" des applications à l'aide de setAbeApplicationLongName
  * - vérifier si une nouvelle version du logiciel est disponible en téléchargement
  * - centraliser un connecteur réseau (QNetworkAccessManager) et les détections de proxy et autres authentifications
  * - charger des éventuels plugins
  * - stocker la valeur du ratio de mise à l'échelle des objets graphiques
  * ... et plus encore dès que vous nous donnez des idées :D
  * @link https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2012-2014 ICham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * #AbulEduApplicationV1
  * include(lib/abuleduapplicationv1/abuleduapplicationv1.pri)
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

#ifndef ABULEDUAPPLICATIONV1_H
#define ABULEDUAPPLICATIONV1_H

#include <QNetworkProxy>
#include <QSharedMemory>
#include <QProxyStyle>
#include <QMessageBox>
#include <QtXml/QtXml>
#include <QNetworkReply>
#include <QSettings>
#include <QHostInfo>
#include <QThread>
#include <QString>
#include <QEvent>
#include <QDate>
#include <QHash>
#include <QDir>
#include <QDesktopWidget>
#include <QFontDatabase>
#if QT_VERSION >= 0x050200
    #include <QCommandLineParser>
#endif


#include "qtsingleapplication/src/qtsingleapplication.h"

class AbulEduNetworkAccessManagerV1;
class AbulEduIdentitesV1;
class AbulEduMultiMediaSettingsV1;

/** Remplace qApp par abeApp (pensez à inclure le fichier "abuleduapplicationv1.h" */
#ifndef abeApp
    #define abeApp ((AbulEduApplicationV1 *) qApp)
#endif

class AbulEduApplicationV1Private;
class AbulEduApplicationV1: public QtSingleApplication
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AbulEduApplicationV1)

private :
    AbulEduApplicationV1Private* const d;

public:

    /** Structure pour stocker des infos relatives au réseau */
    struct AbulEduNetworkInfo {
        QHostAddress  HostIp;       /// l'adresse IP
        QHostAddress  Subnet;       /// le masque de ss-réseau
        QString       MacAdress;    /// l'adresse MAC associée
    };

    /** Constructeur de la classe
      * @param QString appName, chaine de caractere, le nom de l'application
      * @param QString appVersion, chaine de caractere, la version de l'application
      * @param QString organizationDomain, chaine de caractere, le domaine de l'organisation, initialise par defaut a ""
      * @param QString organizationName, chaine de caractere, le nom de l'organisation, initialise par defaut a ""
      * @param QString unitVersion version du module abe genere
      */
    AbulEduApplicationV1(int &argc, char *argv[], const QString& appName,
                         const QString& appVersion,
                         const QString& organizationDomain = "",
                         const QString& organizationName = "",
                         const QString& unitVersion = "1.0",
                         bool isSingleAPP = true);

    /** Destructeur par defaut de la classe */
    ~AbulEduApplicationV1();

    /** Retourne le nom 'long' de l'application */
    const QString& getAbeApplicationLongName();

    /** Permet d'affecter le nom 'long' de l'application, ce qu'on avait en variable globale du temps de abuleduexercicev0
      * remplace extern const QString abeApplicationLongName = QObject::trUtf8("AbulEdu - Livret de compétences");
      * @param QString s, chaine de caractere, le nom 'long' que l'on veut donner à l'application
      */
    void setAbeApplicationLongName(const QString&);

    /** Retourne la version abe gérée par l'application */
    const QString &getAbeApplicationUnitVersion();

    void setAbeApplicationUnitVersion(const QString&);

    /** Retourne la langue à utiliser dans les interfaces */
    const QString &getAbeApplicationInterfaceLang();

    /** Enregistre la langue à utiliser dans les interfaces */
    void setAbeApplicationInterfaceLang(const QString&);

    /** Permet d'affecter le ratio de mise a l'echelle des objets graphiques
      * @param float ratio, flottant, le ratio de mise a l'echelle des objets graphiques
      */
    void setAbeApplicationDecorRatio(const float&);

    /** Retourne  le ratio a appliquer sur l'affichage pour respecter la coherence de l'ensemble */
    const float &getAbeApplicationDecorRatio();

    /** Retourne les informations du systeme d'exploitation en cours */
    const QString& getAbeApplicationThisOS();

    /** Retourne le pointeur sur le networkaccessmanager "global" */
    AbulEduNetworkAccessManagerV1* getAbeNetworkAccessManager();

    /** Retourne le pointeur sur l'identité de l'utilisateur, un logiciel ne pouvant être utilisé que par une seule personne */
    AbulEduIdentitesV1 *getAbeIdentite();

    /** Retourne le chemin du répertoire utilisateur sans le / final */
    QString& getAbeHomepath();

    /** Emet le signal de fin de download
      * @see AbulEduApplicationV1::signalDownloaded(QString s)
      * @param QString s, chaine de caractere, !!
      */
    virtual void sendSignalDownloaded(const QString& s){ emit signalDownloaded(s);}

    /** Retourne un numero unique, permet de remonter des statistiques d'utilisation anonymes */
    const QString& getAbeUniqueUID();

    /** Retourne le nom de l'utilisateur actuellement loggué (ouverture de session de l'OS, pas le login SSO) */
    const QString &getAbeUserLogin();

    /** Active ou désactive le clavier virtuel, pratique si on passe une appli sur le TBI et qu'on souhaite
      * bénéficier du clavier virtuel temporairement
      */
    void setAbeApplicationVirtualKeyboard(bool enabled);

#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
    /** Retourne le pointeur sur l'objet Multimedia partagé */
    QSharedPointer <AbulEduMultiMediaSettingsV1> getAbeApplicationMultiMediaSettings();
#endif

    const AbulEduNetworkInfo &abeGetNetworkInfos();

    /** Centre le QWidget donné en paramètre sur l'écran de l'utilisateur
      * @note dans le cas d'une configuration multi-écran, la fenêtre est centrée sur l'écran numéro 1 de la configuration système
      * @paramQWidget *, pointeur de QWidget, le widget à centrer sur l'écran
      */
    void abeCenterWindow(QWidget *);

    ///
    /// \brief runQProcess
    /// \param executable est le nom de la commande à lancer
    /// \param name est le nom "userfriend", passé en objectName() au QProcess pour pouvoir être utilisé (pour affichage par exemple)
    ///
    void runQProcess(const QString &executable, const QString &name = QString(), const QString &workingDirectory = QString(), const QStringList &arguments = QStringList());

    ///
    /// \brief sluggify
    /// \param str la chaine de caractères à nettoyer des accents, ponctuations,caractères particuliers, espaces, majuscules
    /// \return la chaine de caractères nettoyée
    ///
    QString sluggify(QString str);

#if QT_VERSION >= 0x050200
    void initCommandLineParser();
    QCommandLineParser* getAbeCommandLineParser();
#endif
    void initLogger();

protected slots:
    void slotProcessErrorHandler(QProcess::ProcessError processError);
    void slotAppliFinished(int exitCode, QProcess::ExitStatus status);

private slots:
    /** Gestion de la requete reseau pour voir s'il existe une mise a jour de l'application
      * @param QNetworkReply*, un pointeur sur QNetworkReply
      */
    void slotOnlineUpdateRequest(QNetworkReply*);

    /** Gestion de l'erreur reseau
      * @see enum QNetworkReply::NetworkError
      * @param QNetworkReply::NetworkError code, le code erreur
      */
    void slotReplyError(QNetworkReply::NetworkError code);

    /** Si compiler avec l'option -demo */
    void slotDemoQuit();

    void slotIdentityLoaded();

private:
    /// Permet de détacher la zone de mémoire partagée lors de l'appel au destructeur
    void detach();

    /// Retourne le nom du format de paquets utilises par ce linux
    const QString& linuxPackageFormat();

    /// Retourne le nom de la distribution linux -> @todo Convention de nommage accesseur, a modifier en getLinuxDistrib()
    const QString& linuxDistrib();

    void p_linuxAnalyse();

    void p_automaticGetUserNameFromENV();

    /// Retourne l'age de du fichier executable de l'application (en nombre de jours) ->@todo Convention de nommage accesseur, a modifier en getExecAge()
    int execAge();

#if !defined(Q_OS_ANDROID)
  #if QT_VERSION < 0x050000
    /// Le clavier virtuel
    AbulEduVirtualKeyboardV1 *m_abuleduVirtualKeyboard;
  #endif
#endif


    /** Charge et installe la police "Andika"
     * @return bool, true si OK, false sinon
     */
    bool installFontAndika();

signals:
    /// Un signal pour indiquer que l'application est prête
    void signalApplicationReady();

    /** Lancement du signal par le plugin
      * @see AbulEduApplicationV1::sendSignalDownloaded()
      * @brief Permet d'accrocher un slot de traitement via la méthode publique sendSignalDownloaded()
      * lorsque le telechargement est termine... (par ex. rafraichir la liste d'une combobox)
      */
    void signalDownloaded(const QString& s);

    /** L'application à un message a faire passer, merci de cacher un splashscreen ou tout autre fenêtre qui
      * pourrait venir s'intercaller !
      */
    void signalApplicationMessageStart();

    /// Fin du message a faire passer, merci de reafficher le splashscreen ou tout autre fenêtre
    void signalApplicationMessageEnd();
    ///
    /// \brief signalAbeApplicationProcessError
    /// \param error l'erreur retournée
    /// \param name l'objectName() du QProcess
    ///
    void signalAbeApplicationProcessError(QProcess::ProcessError error,QString name);

#ifdef Q_OS_WIN
    ///
    /// \brief signalAppLaunched
    /// \param name le nom de l'application passé comme objectName au QProcess qui l'a lancée
    ///
    void signalAppLaunched(QString name);
#endif
};

/** Astuce pour faire un sleep, pratique pour le splashscreen mais probablement a d'autres moments aussi
  * d'ou sa presence ici pour qu'elle soit accessible depuis n'importe quel endroit avec un simple
  * #include abuleduapplicationv1.h :)
  */
class AbulEduSleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs) {QThread::msleep(msecs);}
};

#endif //end ABULEDUAPPLICATIONV1_H
