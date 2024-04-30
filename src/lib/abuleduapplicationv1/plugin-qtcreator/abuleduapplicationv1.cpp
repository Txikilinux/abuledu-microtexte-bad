/** Classe AbulEduApplicationV1
  *
  * Classe de base en version une pour les applications AbulEdu permettant de structurer
  * nos logiciels et respecter ainsi un certain nombre de choses dont l'interface publique
  * à laquelle le plugin peut se connecter pour échanger des données (logs pour le moment)
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduApplicationV1
  * @author 2009-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2011 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @see The GNU Public License (GPL)
  *
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


/** =========================================== VERSION FAKE POUR PLUGIN QT CREATOR ===========================================  */

#include "abuleduapplicationv1.h"

/** constructeur ... */
AbulEduApplicationV1::AbulEduApplicationV1(int &argc, char *argv[], QString appName, QString appVersion, QString organizationDomain, QString organizationName, QString unitVersion):
    QObject()
{
    m_localDebug = false;
    setObjectName("AbulEduApplicationV1Plugin");
    if(m_localDebug) {
        qDebug() << "AbulEduApplicationV1: constructeur en mode Plugin de QtCreator";
    }

    if(organizationName == "")
        qWarning() << "L'appel au constructeur AbulEduApplicationV1 ne specifie pas d'organizationName ... le chargement de plugins personnels ne sera pas automatique.";

    //Initialisation des différentes variables
    m_applicationLongName  = "";
    m_username = "";
    p_setUserName(); //on affecte la variable m_username dans cette fonction qui gere les OS
    m_decorRatio       = 1;
    int already_requested = 0;

    m_nam = new AbulEduNetworkAccessManagerV1(this);
    m_identite = new AbulEduIdentitesV1(this);
    m_identite->setObjectName("AbulEduApplicationV1/AbulEduIdentitesV1");
}

AbulEduApplicationV1::~AbulEduApplicationV1()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
}

void AbulEduApplicationV1::loadPlugins()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
}

void AbulEduApplicationV1::linuxAnalyse()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    QStringList dists_debian;
    QStringList dists_ubuntu_lts;
    QStringList dists_ubuntu;
    QStringList dists_fedora;
    QStringList dists_mageia;

    dists_debian     << "squeeze" << "wheezy" << "sid";
    dists_ubuntu_lts << "dapper"  << "hardy"  << "lucid"  << "precise";
    dists_ubuntu     << "warty"   << "hoary"  << "breezy" << "edgy"    << "feisty" << "gutsy" << "intrepid" << "jaunty" << "karmik" << "maverick" << "natty" << "oneiric" << "quantal" << "raring";
    dists_mageia     << "thornicroft";

    //Signature des fedora
    QFile fedora("/etc/fedora-release");

    //Pour l'instant c'est uniquement ubuntu qui propose ca
    //Et mageia 2 aussi ...
    QFile lsb("/etc/lsb-release");
    if(lsb.exists()) {
        if (lsb.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QRegExp release("DISTRIB_CODENAME=(.*)");
            while (!lsb.atEnd() && m_linuxDistrib == "") {
                QByteArray line = lsb.readLine();
                release.indexIn(line.trimmed());
                if(release.cap(1) != "" && m_localDebug) {
                    qDebug() << "AbulEduApplicationV1::linuxPackageFormat On a trouve le LSB :: " << line << " ici : " << release.cap(1).trimmed();
                }
                if(dists_debian.contains(release.cap(1)) || dists_ubuntu.contains(release.cap(1)) || dists_ubuntu_lts.contains(release.cap(1))) {
                    m_linuxDistrib = release.cap(1).trimmed();
                    m_linuxPackageFormat = "deb";
                }
                if(dists_mageia.contains(release.cap(1))) {
                    m_linuxDistrib = release.cap(1).trimmed();
                    m_linuxPackageFormat = "rpm";
                }

            }
            lsb.close();
        }
    }
    else if(fedora.exists()) {
        m_linuxPackageFormat = "rpm";
    }
    else {
        if(m_localDebug)
            qDebug() << "Lancer la commande 'lsb_release -a";
    }

    if(m_linuxPackageFormat == "" || m_linuxDistrib == "") {
        qWarning() << trUtf8("La distribution...");
    }
}

QString AbulEduApplicationV1::os()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    QString os("");
#if defined(Q_OS_MAC)
    os="osx";
#elif defined(Q_OS_WIN)
    os="windows";
#elif defined(Q_OS_LINUX)
    QString arch(""); //pour l'instant i386 / amd64
    //L'architecture ... pour l'instant on ne fait pas les ARM / Sparc etc.
#ifdef __x86_64__
    arch = "amd64";
#else
    arch = "i386";
#endif

    //On regroupe tout ca linux-deb-lucid-amd64
    os="linux-" + linuxDistrib() + "-" + linuxPackageFormat() + "-" + arch;
#endif
    return os;
}

QString AbulEduApplicationV1::uniqueUID()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

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
    return retour.toHex().toBase64();
}

QString AbulEduApplicationV1::getAbeUserLogin()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__ << " : " << this->objectName();
    }

    return QString("");
}

void AbulEduApplicationV1::setAbeApplicationVirtualKeyboard(bool enabled)
{
    return;
}

int AbulEduApplicationV1::execAge()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    QFileInfo fi(qApp->applicationFilePath());
    int ageDuFichier(fi.created().daysTo(QDateTime::currentDateTime()));
    if(m_localDebug)
        qDebug() << "Age du fichier :: " << ageDuFichier;
    return 0;
}

QString AbulEduApplicationV1::linuxDistrib()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    if(m_linuxDistrib == "")
        linuxAnalyse();
    return QString("");
}

QString AbulEduApplicationV1::linuxPackageFormat()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    if(m_linuxPackageFormat == "")
        linuxAnalyse();
    return QString("");
}

void AbulEduApplicationV1::setAbeApplicationLongName(QString s)
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    m_applicationLongName = s;
}

QString AbulEduApplicationV1::getAbeApplicationLongName()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    return QString("");
}

void AbulEduApplicationV1::slotOnlineUpdateRequest(QNetworkReply *reply)
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }

    //Si ce n'est pas pour moi je droppe
    if(reply->objectName() != "OnlineUpdateRequest") {
        return;
    }

    disconnect(m_nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotOnlineUpdateRequest(QNetworkReply*)));

    // Pas d'erreur ?
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
            if(m_localDebug)
                qDebug() << "AbulEduApplicationV1 : download XML update " << xml->toString();

            QDomElement xmlElem = xml->documentElement();
            QDomNode node = xmlElem.firstChild();
            while(!node.isNull()) {
                QDomElement element = node.toElement(); // On essaie de convertir le node en element
                if(!element.isNull()) {
                    // Le node est bien un élément.
                    if(element.tagName()=="application") {
                        QDomNode node=element.firstChild(); //On parcourt le document à partir du tag "application"
                        while(!node.isNull()) {
                            QDomElement elem=node.toElement(); //On cherche le titre du "Channel"
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

            //On affiche une boite d'information (popup) à l'utilisateur ...
            if(titre.length()) {
                QWidget* top = 0;
                if(qApp->topLevelWidgets().size() > 0){
                    top = qApp->topLevelWidgets().last();
                }
                AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(titre, desc,true,top);
                msgBox->show();
                //Quand on ferme la fenetre on lance le signal END -- evite une collision entre le splascreen et une popup de mise a jour disponible
                connect(msgBox, SIGNAL(destroyed()), this, SIGNAL(signalApplicationMessageEnd()),Qt::UniqueConnection);
                //fenetre de message ouverte, on informe tout le monde
                emit signalApplicationMessageStart();
            }
            delete xml;
        }
        //S'il s'agit de redirections ...
        else if (v >= 300 && v < 400)
        {
            // Get the redirection url
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative,
            // we have to use the previous one to resolve it
            newUrl = reply->url().resolved(newUrl);
            QNetworkRequest redirection(newUrl);
            m_networkReply = m_nam->get(redirection);
            connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotReplyError(QNetworkReply::NetworkError)),Qt::UniqueConnection);
            return; // to keep the manager for the next request
        }
    }
    reply->deleteLater();
}

void AbulEduApplicationV1::slotReplyError(QNetworkReply::NetworkError code)
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
        qDebug() << "  " << code;
    }
    m_networkReply->deleteLater();
}

void AbulEduApplicationV1::p_setUserName()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__ << m_username << " (start) " << objectName();
    }

#ifdef Q_OS_WIN
    m_username = getenv("USER");
    if(m_username == "") {
        m_username = getenv("USERNAME");
    }
#endif
#ifdef Q_OS_LINUX
    m_username = getenv("USER");
    if(m_username == "") {
        m_username = getenv("USERNAME");
    }
#endif
#ifdef Q_WS_MAC
    m_username = getenv("USER");
    if(m_username == "") {
        m_username = getenv("USERNAME");
    }
#endif
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__ << m_username << " (end) " << objectName();
    }
}

void AbulEduApplicationV1::detach()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
}

void AbulEduApplicationV1::setAbeApplicationDecorRatio(float ratio)
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
    m_decorRatio = ratio;
}

float AbulEduApplicationV1::getAbeApplicationDecorRatio()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
    return 0;
}

AbulEduNetworkAccessManagerV1 *AbulEduApplicationV1::getAbeNetworkAccessManager()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__ << " : " << objectName();
    }
    return new AbulEduNetworkAccessManagerV1(this);
}

AbulEduIdentitesV1 *AbulEduApplicationV1::getAbeIdentite()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
    return new AbulEduIdentitesV1(this);
}

QString AbulEduApplicationV1::getAbeHomepath()
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
    return QString("/tmp");
}

void AbulEduApplicationV1::sendSignalDownloaded(QString s)
{
    if(m_localDebug) {
        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
    }
    emit signalDownloaded(s);
}

QFont AbulEduApplicationV1::font()
{
    QFontDatabase andika;
    andika.addApplicationFont(":/AbulEduApplicationV1/fonts/andika");
    return QFont("andika");
}
