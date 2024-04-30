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

#ifndef ABULEDUIDENTITESV1_H
#define ABULEDUIDENTITESV1_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QMessageBox>
#include "maiaObject.h"
class AbulEduIdentitesNetworkV1;
#include "abuleduidentitesnetworkv1.h"
#include "vcard.h"
#include "abuleduloggerv1.h"

class AbulEduIdentitesV1 : public QObject
{
    Q_OBJECT

public:
    AbulEduIdentitesV1(QObject *parent = 0);
    ~AbulEduIdentitesV1();

    /** Permet de mettre à jour l'identité (sur le serveur central via abuleduidentitesnetwork) */
    void abeUpdateIdentitie();

    /** Ré-Unification avec un autre compte existant */
    void abeUnifiedWith(const QString &email);

    /** Exporte mes logs en un fichier PDF */
    void abeGetMyLogsAsPDF();

    /** Affecte les informations à cet objet en un seul appel */
    void abeSet(const QString& nom, const QString& prenom, const QString& domaine, const QString& telephone,
                const QString& mobile, const QString& email, const QString& adresse, const QString &codepostal, const QString& ville,
                const QStringList& responsables, const QString& login, QPixmap *photo, const QString& groupes);

    /** Gestion du nom de l'utilisateur */
    QString abeGetNom() const {return m_nom;}
    void    abeSetNom(const QString &nom) {m_nom = nom;}

    /** Gestion du prénom de l'utilisateur */
    QString abeGetPreNom() const {return m_prenom;}
    void    abeSetPreNom(const QString& prenom){m_prenom = prenom;}

    /** Gestion du domaine de l'utilisateur */
    QString abeGetDomaine() const {return m_domaine;}
    void    abeSetDomaine(const QString& domaine) {m_domaine = domaine;}

    /** Gestion du numéro de téléphone de l'utilisateur */
    QString abeGetTelephone() const {return m_telephone;}
    void    abeSetTelephone(const QString& telephone) {m_telephone = telephone;}

    /** Gestion du numéro de téléphone mobile */
    QString abeGetMobile() const {return m_mobile;}
    void    abeSetMobile(const QString& mobile) {m_mobile = mobile;}

    /** Retourne l'adresse email */
    QStringList abeGetEmail() {return m_email;}
    void abeSetEmail(const QStringList& emails) {m_email.clear(); m_email=emails;}
    void abeSetEmail(const QString& email) {m_email.clear(); m_email.append(email);}

    /** Retourne l'adresse postale */
    QString abeGetAdresse() {return m_adresse;}
    void    abeSetAdresse(const QString& adresse) {m_adresse = adresse;}

    /** Gestion du code postal */
    QString abeGetCodePostal() const {return m_codepostal;}
    void    abeSetCodePostal(const QString& cp) {m_codepostal=cp;}

    /** Retourne la ville */
    QString abeGetVille() const {return m_ville;}
    void    abeSetVille(const QString& ville) {m_ville=ville;}

    /** Gestion dus responsables legaux */
    QStringList abeGetResponsables() {return m_mes_responsables_legaux;}
    void        abeSetResponsables(const QStringList& responsables) {m_mes_responsables_legaux.clear(); m_mes_responsables_legaux = responsables;}

    /** Gestion du login (identifiant) */
    QString abeGetLogin() const {return m_login;}
    void    abeSetLogin(const QString& login) {m_login=login;}

    /** La photo ou l'avatar */
    QPixmap *abeGetPhoto() {return m_photo;}
    void     abeSetPhoto(QPixmap *p) {m_photo=p;}

    /** La liste des groupes */
    QStringList abeGetGroupesAsList() {return m_groupes;}
    void        abeSetGroupesList(const QStringList& groupes) {m_groupes.clear(); m_groupes = groupes;}

    /** La liste des groupes sous forme d'une QString avec une virgule comme séparateur */
    QString abeGetGroupes() const {return m_groupes.join(",");}

    /** Le mot de passe */
    void abeSetPassword(const QString& password){m_password = password;}

    /** Remise à zéro de l'objet */
    void abeClear();

    /** Retourne les éléments de l'AbulEduIdentitesV1 sous forme de vCard */
    vCard abeGetVCard();

public slots:
    void slotSetFromNetworkReply(QVariant &v, QNetworkReply *reply);
    void slotErrorPDFDownload(const QString &message);
    void slotPDFDownloaded(const QString &filename);
    void slotMessageAvailable(const QString &message);

signals:
    void signalLoaded();
    void signalGetMyLogsAsPDF();
    void signalMessageAvailable(QString message);

private:
    AbulEduIdentitesNetworkV1 *m_abuleduIdentiteNetwork;
    bool m_localDebug;

    QString     m_nom;
    QString     m_prenom;
    QString     m_password;
    QString     m_passwordc;
    QString     m_shell;
    QString     m_locked;
    QString     m_lalocale;
    QPixmap    *m_photo;
    QString     m_login;
    QString     m_commentaire;
    QString     m_usager_or_responsable_title;
    QString     m_usager_or_responsable_group;
    QStringList m_email;
    QString     m_unifiedEmail;
    QStringList m_groupes;
    QString     m_groupes_last_flag;
    QStringList m_responsabilites;
    QString     m_responsabilites_last_flag;
    QStringList m_invitations;
    QString     m_invitations_last_flag;
    QString     m_home;
    QString     m_uid;
    QString     m_gid;
    QString     m_profil;
    QString     m_config;
    QStringList m_system_logins;
    QString     m_quota_connex;
    QStringList m_responsable_legal_de_qui;
    QStringList m_mes_responsables_legaux;
    QString     m_responsablelegalinvitationcode;
    QString     m_telephone;
    QString     m_mobile;
    QString     m_adresse;
    QString     m_codepostal;
    QString     m_ville;
    QString     m_domaine;
};

#endif // ABULEDUIDENTITESV1_H

