/** Classe AbulEduMediathequeGetV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
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
  * with this program. If not, see <http://www.gnu.org/licenses/>.
  */


#ifndef ABULEDUMEDIATHEQUEGET_H
#define ABULEDUMEDIATHEQUEGET_H

#include <QWidget>
#include <QFlags>

#include "abuledunetworkaccessmanagerv1.h"
#include "ui_abuledumediathequegetv1.h"
#include "abuledumediathequecommon.h"
#include "abuledufilev1.h"
#include "maiaXmlRpcClient.h"
#include "abuleduvisionneuseimagev1.h"
#include "abuleduloadinganimationv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduMediathequeGetV1;
}

class AbulEduMediathequeGetV1 : public QWidget, public AbulEduMediathequeCommon
{
    Q_OBJECT
    Q_ENUMS(enumMediathequeOrData)
    Q_ENUMS(enumMediathequeView)
    Q_ENUMS(enumMediathequeSearchFileMime)
    Q_PROPERTY(enumMediathequeOrData serviceDistant READ abeGetSourceEnum WRITE abeSetSourceEnum USER true)

public:
    enum enumMediathequeOrData { abeMediatheque, abeData, abeFichiers };
    enum enumMediathequeView { abeMediathequeList, abeMediathequeThumbnails };
    enum enumMediathequeSearchFileMime { abeMediathequeFileMimeAudio = 0x1,
                                         abeMediathequeFileMimeImage = 0x2,
                                         abeMediathequeFileMimeText = 0x4,
                                         abeMediathequeFileMimeVideo = 0x8
                                       };
    Q_DECLARE_FLAGS(enumMediathequeSearchFileMimes, enumMediathequeSearchFileMime)
    Q_FLAGS(enumMediathequeSearchFileMimes)

    explicit AbulEduMediathequeGetV1(QWidget *parent = 0, const QString &mediathequeOrData = "mediatheque");
    ~AbulEduMediathequeGetV1();
    void abeSetFile(QSharedPointer<AbulEduFileV1> file);

    /** Retourne le fichier en cours (séléctionné)
      * @todo: expliquer un peu mieux, ça retourne le pointeur mais uniquement si le fichier est téléchargé ?
      */
    QSharedPointer <AbulEduFileV1> abeGetFile(){ return m_abuledufile;}

    /** Retourne la notice LOM du fichier en cours (séléctionné) */
    QSharedPointer<AbulEduSCOLOMFrV1> abeGetLOM(){ return m_abuleduLOM;}

    /** Lance le téléchargement de la notice LOM */
    void abeStartDownloadLOM();

    /** Permet de rendre la boite de recherche masquee (utile pour l'integration dans abuledufilemanagerv1) */
    void abeSearchVisible(bool visible);

    /** Lance la recherche depuis un objet externe (ex abuledufilemanagerv1) */
    void abeSearch(const QString &search);

    /** Permet de basculer de data.abuledu.org a mediatheque.abuledu.org
      * Je la conserve le temps que tous les développeurs passent sur abeSetSourceEnum
      * mais avec l'enum (plus propre)
      * @param mediathequeOrData "mediatheque" ou "data"
      */
    void abeSetSource(const QString &mediathequeOrData);

    /** Permet de basculer de data.abuledu.org a mediatheque.abuledu.org
      * @param mediathequeOrData cf enumMediathequeOrData
      */
    void abeSetSourceEnum(const enumMediathequeOrData &mediathequeOrData);

    /** Permet de retourner l'information */
    enumMediathequeOrData abeGetSourceEnum() const{ return m_mediathequeOrData;}

    /** Permet de choisir la vue par defaut (vignette ou liste)
      * @param mediathequeView : abeMediathequeList, abeMediathequeThumbnails
      */
    void abeSetDefaultView(const enumMediathequeView &mediathequeView);

    /** Permet d'activer le bouton Custom1
      * @param si le titre est vide, le bouton est caché
      */
    void abeSetCustomBouton1(const QString &titre);

    /** Permet de modifier le comportement pour lancer le téléchargement sur un simple clic
      * (utile par exemple pour une tablette)
      */
    void abeSetDownloadOnSingleClick(bool enable);

    /** Permet de cacher le bouton close (à côté des boutons de choix de vue)
      * @param const bool yesNo, true pour afficher le bouton close, false pour le cacher
      * @brief  Cette méthode s'avère necessaire lorsque l'objet abuledumediathequeget est embarqué dans
      *         une page (stackedWidget) d'un éditeur de logiciel
      */
    void abeHideCloseBouton(const bool &yesNo);

    /** Permet de lancer le récupération des ressources en attente de modéation */
    void abeSearchWaitingModeration();

    /** Permet de lancer le récupération des ressources en attente d'e modéation d'amélioration */
    void abeSearchMyRessourcesToModify();

    /** Permet de vérifier si cette ressource n'est pas déjà indexée */
    void abeCheckDoublon(const QString &uri);

    /** Retourne l'URI de l'item actuellement sélectionné */
    QString abeGetCurrentSelectedItemURI();

    /** Supprime de la liste l'item actuellement sélectionné (par exemple quand on valide une ressource en attente de modération) */
    void abeRemoveCurrentSelectedItem();

    /** Permet de cacher le bouton Télécharger */
    void abeHideBoutonTelecharger();

    /** Permet de cacher le panneau d'information
      * Attention si le panneau est caché les boutons télécharger et custom1
      * ne seront plus visibles et donc innutilisables, il faut alors passer
      * sur le drag & drop (ou autre)
      */
    void abeHideInfoPanel(bool hide);

    /** Permet d'activer le téléchargement transparent pour le bouton custom
      * si enable = true alors le bouton custom télécharge la ressource lorsqu'on clique dessus
      * et inversement
      */
    void abeSetCustomBouton1Download(bool enable=true);

    /** @deprecated */
    void abeCustomBouton1SetDownload(bool enable=true);

    /** Lance le téléchargement du fichier qui se trouve sous l'item actuellement sélectionné
      * (permet par exemple d'implémenter le drag & drop depuis un objet externe)
      */
    void abeStartDownload();

    /** Vide la liste de recherche */
    void abeClear(bool clearMotsCles=false);

    /** Permet de choisir le type de ressource qu'on cherche (type mime) */
    void abeSetSearchFileMime(const AbulEduMediathequeGetV1::enumMediathequeSearchFileMimes &type);

    void abeSetSearchWidgetsVisible(const bool visible);

signals:

    /** API a réfléchir / terminer ...
      * Retourne
      *  -1 en cas d'erreur
      *   0 si fichier ouvrable par le systeme
      *   1 si fichier abe passé à abuledufilev1
      */
    void signalMediathequeFileDownloaded(QSharedPointer<AbulEduFileV1> abeFile, int code);

    /** Téléchargement de la fiche LOM ok */
    void signalMediathequeLomDownloaded();

    /** On fais suivre le clic sur le bouton custom1 pour y accrocher ce que le développeur souhaite */
    void signalMediathequeCustomBouton1Clicked();

    void signalAbeMediathequeGetCloseOrHide();

public slots:
    void abeSetListView();
    void abeSetIconeView();

private slots:
    void on_btnRecherche_clicked();
    void slotLanceRecherche();

    /** Télécharge la liste des ressources répondant à la recherche */
    void rpcGetSearchList(QVariant &v, QNetworkReply *reply);

    /** Télécharge le fichier */
    void rpcGetFile(QVariant &v, QNetworkReply *reply);

    /** Télécharge les vignettes */
    void rpcGetThumbnails(QVariant &v, QNetworkReply *reply);

    /** Envoie une alerte sur recherche infructueuse */
    void rpcSendAlertNoRessource(QVariant &v, QNetworkReply *reply);

    /** Télécharge la liste des documents non validés */
    void rpcGetValidationWaitingList(QVariant &v, QNetworkReply *reply);

    /** Télécharge la liste des documents que je dois améliorer */
    void rpcGetMyRessourcesToModifyList(QVariant &v, QNetworkReply *reply);

    /** Télécharge le fichier LOM */
    void rpcGetLOM(QVariant &v, QNetworkReply *reply);

    /** Vote pour la ressource */
    void rpcVote(QVariant &v, QNetworkReply *reply);

    /** Gestion des doublons */
    void rpcCheckDoublon(QVariant &v, QNetworkReply *reply);

    void lwMediathequeGetSimpleSetIcon(QListWidgetItem *itemIcone, const QFileInfo &ficone);
    void lwMediathequeGetSimpleSetIcon(QString itemName, const QFileInfo& ficone);

    void ajouteItemList(int num, const QString &titre, const QString &type, const QString &date, const QString &size,
                        const QString &docId, const QString &URI, const QFileInfo &absoluteFilePath, const QString &contributeur,
                        int note, const QString &resume, const QString &motscles, const QString &md5, const QString &docFileName, const QString &docMimeType,const QByteArray &docLOM);

    /** Quand on clique sur un item, changement de selection, il y a du code commun a mutualiser */
    void communItemSelectionChange(const QMap<int, QVariant> &val);
    void on_twMediathequeGetComplet_itemSelectionChanged();
    void on_lwMediathequeGetSimple_itemSelectionChanged();

    void slotDownloadFinished(QNetworkReply *reply);
    void rpcFault(int code, const QString &errorMessage);
    void on_btnTelecharger_clicked();
    void handleOpenFile(const QString &filename);
    void on_btnCustom1_clicked();
    void on_cbAvertirQuandDispo_toggled(bool checked);
    void on_btnSignaler_clicked();
    void on_tbVueIcones_clicked();
    void on_tbVueListe_clicked();
    void on_lwRechercheMotsCles_itemClicked(QListWidgetItem *item);
    void on_lwRechercheHierarchie_itemClicked(QListWidgetItem *item);
    void on_lwMediathequeGetSimple_itemDoubleClicked(QListWidgetItem *item);
    void on_twMediathequeGetComplet_itemDoubleClicked(QTableWidgetItem *item);

    void on_btnImage_clicked();

    void on_tbClose_clicked();

    void on_leRecherche_editingFinished();

private:
    Ui::AbulEduMediathequeGetV1   *ui;
    QSharedPointer<AbulEduFileV1>  m_abuledufile;
    MaiaXmlRpcClient              *m_rpc;
    AbulEduNetworkAccessManagerV1 *m_nam;
    QNetworkReply                 *m_networkReply;
    QString                        m_docID;             /* L'identifiant du doc en cours de download */
    enumMediathequeOrData          m_mediathequeOrData;
    enumMediathequeSearchFileMimes m_searchFileMime;
    QDir                           m_dirABB;            /* Répertoire de stockage des fichiers ABB */
    QDir                           m_dirABE;            /* Répertoire de stockage des fichiers ABE */
    QDir                           m_dirABF;            /* Répertoire de stockage des fichiers ABF */
    QDir                           m_dirThumbnailsABB;  /* Répertoire de stockage des vignettes ABB */
    QDir                           m_dirThumbnailsABE;  /* Répertoire de stockage des vignettes ABE */
    QDir                           m_dirThumbnailsABF;  /* Répertoire de stockage des vignettes ABF */
    QSharedPointer<AbulEduSCOLOMFrV1> m_abuleduLOM;
    AbulEduVisionneuseImageV1     *m_visio;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(AbulEduMediathequeGetV1::enumMediathequeSearchFileMimes)
#endif // ABULEDUMEDIATHEQUEGET_H
