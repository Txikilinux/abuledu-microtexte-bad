/** Classe AbulEduIdentitesV1
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2014 Icham Sirat <icham.sirat@ryxeo.com>
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

#include "abuleduidentitesv1.h"

#ifndef DEBUG_ABULEDUIDENTITESV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduIdentitesV1::AbulEduIdentitesV1(QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_photo = new QPixmap(":/abuleduidentitesv1/avatars/anonyme");
    m_abuleduIdentiteNetwork = new AbulEduIdentitesNetworkV1(this);
    connect(m_abuleduIdentiteNetwork,SIGNAL(signalServerReturnIdentite(QVariant&,QNetworkReply*)),this,SLOT(slotSetFromNetworkReply(QVariant&,QNetworkReply*)),Qt::UniqueConnection);
    connect(m_abuleduIdentiteNetwork,SIGNAL(signalErrorPDFDownload(QString)),this,SLOT(slotErrorPDFDownload(QString)),Qt::UniqueConnection);
    connect(m_abuleduIdentiteNetwork,SIGNAL(signalPDFDownloaded(QString)),this,SLOT(slotPDFDownloaded(QString)),Qt::UniqueConnection);
    connect(m_abuleduIdentiteNetwork,SIGNAL(signalServerMessageAvailable(QString)), this, SLOT(slotMessageAvailable(QString)),Qt::UniqueConnection);
    connect(this,SIGNAL(signalGetMyLogsAsPDF()),m_abuleduIdentiteNetwork,SLOT(slotGetMyLogsAsPDF()),Qt::UniqueConnection);
}

AbulEduIdentitesV1::~AbulEduIdentitesV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    delete m_photo;
    delete m_abuleduIdentiteNetwork;
}

void AbulEduIdentitesV1::abeUpdateIdentitie()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QVariantMap tableau;
    tableau["mobile"]     = m_mobile;
    tableau["telephone"]  = m_telephone;
    tableau["nom"]        = m_nom;
    tableau["prenom"]     = m_prenom;
    tableau["email"]      = m_email.at(0);
    tableau["adresse"]    = m_adresse;
    tableau["codepostal"] = m_codepostal;
    tableau["ville"]      = m_ville;
    tableau["manager"]    = m_mes_responsables_legaux.at(0);
    tableau["motdepasse"] = m_password;

    QByteArray image;
    QBuffer buffer(&image);
    buffer.open(QIODevice::WriteOnly);
    m_photo->save(&buffer,"JPG");
    tableau["photo"]      = QString(image.toBase64());

    m_abuleduIdentiteNetwork->abeUpdateIdentitie(tableau);
}

void AbulEduIdentitesV1::abeSet(const QString &nom, const QString &prenom, const QString &domaine,
                                const QString &telephone, const QString &mobile, const QString &email,
                                const QString &adresse, const QString &codepostal, const QString &ville,
                                const QStringList &responsables, const QString &login, QPixmap *photo, const QString &groupes)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "objet : " << objectName() << " gestion de : " << m_login << " nom : " << nom;

    m_nom = nom;
    m_prenom = prenom;
    m_photo = photo;
    m_login = login;
    m_mobile = mobile;
    m_adresse = adresse;
    m_codepostal = codepostal;
    m_ville = ville;
    m_telephone = telephone;

    m_email.clear();
    m_email.append(email);

    m_domaine = domaine;
    m_mes_responsables_legaux = responsables;
    m_groupes = groupes.split(",");

    //    QString     commentaire;
    //    QString     usager_or_responsable_title;
    //    QString     usager_or_responsable_group;
    //    QString     unifiedEmail;
    //    QStringList groupes;
    //    QString     groupes_last_flag;
    //    QStringList responsabilites;
    //    QString     responsabilites_last_flag;
    //    QStringList invitations;
    //    QString     invitations_last_flag;
    //    QString     home;
    //    QString     uid;
    //    QString     gid;
    //    QString     profil;
    //    QString     config;
    //    QStringList system_logins;
    //    QString     quota_connex;
    //    QStringList responsable_legal_de_qui;
    //    QString     responsablelegalinvitationcode;
}



void AbulEduIdentitesV1::abeClear()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_nom.clear();
    m_prenom.clear();
    m_photo = new QPixmap(":/abuleduidentitesv1/avatars/anonyme");
    m_login.clear();
    m_mobile.clear();
    m_adresse.clear();
    m_codepostal.clear();
    m_ville.clear();
    m_telephone.clear();
    m_email.clear();
    m_domaine.clear();
    m_mes_responsables_legaux.clear();
    m_groupes.clear();
}

vCard AbulEduIdentitesV1::abeGetVCard()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    vCard vcard;
    vCardProperty name_prop  = vCardProperty::createName(m_prenom,m_nom);
    vcard.addProperty(name_prop);
    vCardProperty fn_prop    = vCardProperty(VC_FORMATTED_NAME, m_prenom+" "+m_nom);
    vcard.addProperty(fn_prop);
    vCardProperty note_prop  = vCardProperty(VC_NOTE, "");
    vcard.addProperty(note_prop);
    vCardProperty org_prop   = vCardProperty::createOrganization("");
    vcard.addProperty(org_prop);
    vCardProperty mail_prop   = vCardProperty(VC_EMAIL, m_email);
    vcard.addProperty(mail_prop);
    /* Structure du champ adresse : BP;adresse;rue;ville;région;codepostal;pays */
    vCardProperty adress_prop   = vCardProperty(VC_ADDRESS, ";"+m_adresse+";"+";"+m_ville+";"+";"+m_codepostal+";");
    vcard.addProperty(adress_prop);
    vCardProperty telephone_prop   = vCardProperty(VC_TELEPHONE, m_telephone);
    vcard.addProperty(telephone_prop);
    vCardProperty url_prop   = vCardProperty(VC_URL, "");
    vcard.addProperty(url_prop);

    return vcard;
}

void AbulEduIdentitesV1::slotSetFromNetworkReply(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    switch (reply->error()) {
    case QNetworkReply::NoError:
        for(int i = 0; i < v.toMap().count(); i++) {
            QMap<QString,QVariant> monItem = v.toMap().value(QString::number(i)).toMap();
            if(monItem.count() > 0 && (!monItem.value("login").toString().isEmpty())) {
                    ABULEDU_LOG_DEBUG() << "***********************************"
                                        << "ITEM : " << monItem
                                        << "***********************************";
                QPixmap *photo = new QPixmap();
                if(!photo->loadFromData(QByteArray::fromBase64(monItem.value("photo").toByteArray()))) {
                    ABULEDU_LOG_DEBUG() << "Erreur de chargement de la photo ...";
                }

                /* On enquille toutes les identités les unes apres les autres dans notre structure de données */
                abeSet( QString::fromLatin1(monItem.value("nom").toByteArray()),
                        QString::fromLatin1(monItem.value("prenom").toByteArray()),
                        QString::fromLatin1(monItem.value("domaine").toByteArray()),
                        QString::fromLatin1(monItem.value("telephone").toByteArray()),
                        QString::fromLatin1(monItem.value("mobile").toByteArray()),
                        QString::fromLatin1(monItem.value("email").toByteArray()),
                        QString::fromLatin1(monItem.value("adresse").toByteArray()),
                        QString::fromLatin1(monItem.value("codepostal").toByteArray()),
                        QString::fromLatin1(monItem.value("ville").toByteArray()),
                        monItem.value("manager").toStringList(),
                        QString::fromLatin1(monItem.value("login").toByteArray()),
                        photo,
                        monItem.value("groupes").toStringList().join(","));
            }
        }
        emit signalLoaded();

        break;
    default:
        /* On a eu un problème réseau */
        break;
    }
}

void AbulEduIdentitesV1::abeGetMyLogsAsPDF()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(!m_login.isEmpty()) {
        emit signalGetMyLogsAsPDF();
    }
    else {
        QMessageBox::warning(0, trUtf8("Erreur de téléchargement"),
                             trUtf8("Vous devez être authentifié pour télécharger votre journal d'activités AbulÉdu"),
                             QMessageBox::Ok);
    }
}

void AbulEduIdentitesV1::slotErrorPDFDownload(const QString &message)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QMessageBox::warning(0, trUtf8("Erreur de téléchargement"),
                         message,
                         QMessageBox::Ok);

    emit signalMessageAvailable(message);
}

void AbulEduIdentitesV1::slotPDFDownloaded(const QString &filename)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QDesktopServices::openUrl(QUrl(filename, QUrl::TolerantMode));
}

void AbulEduIdentitesV1::slotMessageAvailable(const QString &message)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << message;

    emit signalMessageAvailable(message);
}

//void AbulEduIdentitesV1::abeUnifiedWith(QString email)
//{
//    if(m_localDebug) {
//        qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__ << " (1)";
//    }
//    //On fait une demande d'unification du compte
//    //La structure de mise à jour
//    QVariantMap tableau;
//    tableau["unifiedMail"] = email;
//    QList<QVariant> params = QList<QVariant>() << tableau;

//    m_rpc->call("abuledu.sso.updateLoginUnifiedEmail", params,
//                this, SLOT(rpcUpdated(QVariant&, QNetworkReply*)),
//                this, SLOT(rpcFault(int, const QString &)));

//}

