/** Classe AbulEduBoxFileManagerV1
  *
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Icham Sirat <icham.sirat@ryxeo.com>
  * @copyright The GNU Public License (GNU/GPL) v3
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

#ifndef ABULEDUBOXFILEMANAGERV1_H
#define ABULEDUBOXFILEMANAGERV1_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QDebug>
#include <QFileDialog>
#include "ui_abuleduboxfilemanagerv1.h"
#include "maiaXmlRpcClient.h"
#include "abuledufilev1.h"
#include "abuleduloggerv1.h"
#include "abuledufilemanagertablewidgetitemv1.h"

namespace Ui {
class AbulEduBoxFileManagerV1;
}

/**
  * La classe AbulEduBoxFileManagerV1 se propose de remplacer la boite QFileOpen de Qt qui affiche une boite systeme.
  * La AbulEduBoxFileManagerV1 offre des spécificités intéressantes pour notre objectif:
  *   - Affichage simple (on affiche que les vignettes, plutôt à destination du public non lecteur)
  *
  *
  * @image html "Capture-Gestionnaire de fichiers AbulÉduBox.png"
  *
  *
  *   - Affichage complet qui permet de faire des recherches sur sa box perso, la box du groupe, la médiathèque ...
  *
  *
  * @image html "Capture-Gestionnaire de fichiers AbulÉduBox-1.png"
  */

class AbulEduBoxFileManagerV1 : public QWidget
{
    Q_OBJECT
    Q_ENUMS(enumAbulEduBoxFileManagerOpenOrSave)
    Q_ENUMS(enumAbulEduBoxFileManagerDisplaySimpleOrComplete)
    Q_ENUMS(enumAbulEduBoxFileManagerSavingLocation)
    Q_PROPERTY(enumAbulEduBoxFileManagerOpenOrSave openOrSave READ abeGetOpenOrSaveEnum WRITE abeSetOpenOrSaveEnum USER true)
    Q_PROPERTY(enumAbulEduBoxFileManagerDisplaySimpleOrComplete displaySimpleOrComplete READ abeGetDisplaySimpleOrCompleteEnum WRITE abeSetDisplaySimpleOrCompleteEnum USER true)

public:
    enum enumAbulEduBoxFileManagerOpenOrSave { abeOpen, abeSave, abeImport };
    enum enumAbulEduBoxFileManagerDisplaySimpleOrComplete { abeDisplaySimple, abeDisplayComplete};
    enum enumAbulEduBoxFileManagerSavingLocation {abePC, abeBoxPerso, abeBoxShare, abeMediatheque};

    /** Astuce pour pouvoir passer des noms textuels aux enums + gestion des traductions ? */
    static inline QMap<enumAbulEduBoxFileManagerSavingLocation,QString> ABE_ENUMTOSTRING() {
        QMap<enumAbulEduBoxFileManagerSavingLocation,QString> map;
        map.insert(abePC,"personnel-pc");
        map.insert(abeBoxPerso,"abebox-personnelle");
        map.insert(abeBoxShare,"abebox-partagee");
        map.insert(abeMediatheque,"abe-mediatheque");
        return map;
    }

    /** Constructeur */
    explicit AbulEduBoxFileManagerV1(QWidget *parent = 0, enumAbulEduBoxFileManagerOpenOrSave openOrSave = abeOpen);

    /** Destructeur */
    ~AbulEduBoxFileManagerV1();

    /** Pour passer un AbulEduFileV1 a cette classe, à quoi ça sert ?
      * -> ça évite d'en créer un nouveau si le développeur de l'application a déjà un objet de ce type
      * @param *file pointeur sur un objet de type AbulEduFileV1
      */
    void abeSetFile(QSharedPointer<AbulEduFileV1> file);

    /** Retourne le pointeur sur un objet AbulEduFileV1 que l'utilisateur a sélectionné
      * @return *AbulEduFileV1
      */
    const QSharedPointer<AbulEduFileV1> abeGetFile(){ return m_abuleduFile;}

    /** Permet de basculer d'une ouverture de fichier à une boite utilisable pour choisir
      * de sauvegarder un fichier ou d'importer des fichiers depuis une autre application
      * @param openOrSave cf enumAbulEduBoxFileManagerOpenOrSave
      * @param importApplicationName nom de l'application dont on veut importer des fichiers ex: leterrier-racont
      */
    void abeSetOpenOrSaveEnum(enumAbulEduBoxFileManagerOpenOrSave openOrSave, const QString &importApplicationName = "");

    /** Permet de retourner l'information */
    enumAbulEduBoxFileManagerOpenOrSave abeGetOpenOrSaveEnum(){return m_openOrSave;}

    void abeBoxFileManagerSetSavingLocation(enumAbulEduBoxFileManagerSavingLocation location) {m_quelleBox = location;}

    enumAbulEduBoxFileManagerSavingLocation abeBoxFileManagerGetSavingLocation() {return m_quelleBox;}

    /** Getteur public de l'attribut privé m_sender
      * @note permet de récupérer le nom de l'objet qui a fait appel à l'AbulEduBoxFileManagerV1, et donc d'y retourner par un connect
      * sur les signaux signalAbeFileSaved() ou signalAbeFileCloseOrHide()
      */
    const QObject* abeGetSender(){return m_sender;}

    /** Getteur public de l'attribut privé m_sender
      * @note permet de récupérer le nom de l'objet qui a fait appel à l'AbulEduBoxFileManagerV1, et donc d'y retourner par un connect
      * sur les signaux signalAbeFileSaved() ou signalAbeFileCloseOrHide()
      */
    void abeSetSender(QObject* s);

    /**
      * Permet de basculer d'une ouverture de fichier à une boite simple ou complete
      * de sauvegarder un fichier
      * @param openOrSave cf enumAbulEduBoxFileManagerOpenOrSave
      */
    void abeSetDisplaySimpleOrCompleteEnum(const enumAbulEduBoxFileManagerDisplaySimpleOrComplete &);

    /** Permet de retourner l'information "boite ouverte en mode simple ou complet" */
    enumAbulEduBoxFileManagerDisplaySimpleOrComplete abeGetDisplaySimpleOrCompleteEnum(){return m_displaySimpleOrComplete;}

    /** Permet de forcer l'affichage du contenu d'une box */
    void abeRefresh(enumAbulEduBoxFileManagerSavingLocation location);

    /** Passe à l'objet le nom du projet */
    void abeSetProjectName(const QString& name);

    /** Permet d'afficher autre chose que les .abe */
    void abeSetFileFilters(const QStringList& filters) {m_filters = filters;}

    /**
      * Permet de cacher le bouton close (à côté des boutons de choix de vue)
      * @param const bool yesNo, true pour afficher le bouton close, false pour le cacher
      * @brief  Cette méthode s'avère necessaire lorsque l'objet abuledumediathequeget est embarqué dans
      *         une page (stackedWidget) d'un éditeur de logiciel
      */
    void abeMediathequeGetHideCloseBouton(const bool &yesNo);

private:
    Ui::AbulEduBoxFileManagerV1 *ui;
    MaiaXmlRpcClient *m_rpcAbulEduBox;
    AbulEduNetworkAccessManagerV1 *m_nam;
    QSharedPointer<AbulEduFileV1> m_abuleduFile;
    enumAbulEduBoxFileManagerSavingLocation m_quelleBox;
    QDir m_dirABE;
    QDir m_dirThumbnailsABE;
    enumAbulEduBoxFileManagerOpenOrSave m_openOrSave;
    enumAbulEduBoxFileManagerDisplaySimpleOrComplete m_displaySimpleOrComplete;
    QWidget* m_parent;
    QFrame *m_frmBandeauBackground;
    QString m_importApplicationName;
    bool m_iAmSure;

    /** Permet de garder en mémoire un QObject*. Dans le cas où l'affichage de cette AbulEduBoxFileManagerV1 est déclenchée
      * dans un slot, ce QObject* est l'objet dont un signal a provoqué l'exécution de ce slot
      * @note: exemple d'utilisation ? je ne sais toujours pas à quoi ça sert :)
      */
    QObject* m_sender;

    /** Liste d'extensions dont on permet l'affichage. Elle est initialisée à QStringList(). Dans ce cas, seuls les .abe seront affichés */
    QStringList m_filters;

    void abeSwitchPageSimpleOrComplete();
    void updateLabelInfo();
    
public slots:
    /** Factorisation du code d'ouverture */
    void ouvertureFichier(const QString& filename, const QString& groupe = "", bool progressBarVisible = true);

    /** Gestion de l'upload pour expédier un fichier sur la box
      * si groupe est non nul ou ne vaut pas "perso" c'est qu'on souhaite
      * sauvegarder le fichier dans un dossier partagé
      */
    void slotUploadFile(const QString& fileName, const QString& groupe = "", const QString& destName = "same");

private slots:
    /** Réception du flux RPC de liste des fichiers */
    void rpcAbulEduBoxFileList(QVariant &v, QNetworkReply *reply);

    /** Réception du flux RPC de récupération d'un fichier */
    void rpcAbulEduBoxGetFile(QVariant &v, QNetworkReply *reply);

    /** Gestion des erreurs XMLRPC */
    void rpcFault(int code, const QString &errorMessage);

    /** Gestion des erreurs XMLRPC: upload de fichier
      * @todo: vérifier si ce slot est bien appellé en cas de timeout
      */
    void rpcFaultFileUpload(int code, const QString &errorMessage);

    /** Gestion de l'upload de fichiers */
    void rpcFileUpload(QVariant &v, QNetworkReply *reply);

    /** Récupère les vignettes sur le serveur de fichiers distants */
    void rpcGetThumbnails(QVariant&, QNetworkReply*);

    /** Gestion du double clic sur le listing des fichiers */
    void on_twComplet_itemDoubleClicked(QTableWidgetItem *item);

    /** Clic sur le bouton rechercher */
    void on_leRecherche_returnPressed();

    /** Double clic sur un item de la box simple */
    void on_lwSimple_itemDoubleClicked(QListWidgetItem *item);

    /** Gestion du changement de box quand on est en mode simple */
//    void on_btnSimpleMonPC_clicked();
//    void on_btnSimpleBoxPerso_clicked();
//    void on_btnSimpleBoxPartagee_clicked();
//    void on_btnSimpleOuvrir_clicked();

    /** Gestion du changement de box quand on est en mode complet */
    void on_btnMonPC_clicked();
    void on_btnMediatheque_clicked();
    void on_btnBoxPerso_clicked();
    void on_btnBoxPartagee_clicked();
    void on_btnOuvrir_clicked();

    /** Supprimer un fichier */
    void on_btnSupprimer_clicked();

    void nettoyageComplet();

    /** Clic sur "sauvegarder" le fichier */
    void on_btnNomFichierSauvegarder_clicked();

    /** Special en mode "sauvegarde", le clic sur un item -> nom du fichier dans la zone sauvegarde en bas */
    void on_twComplet_itemClicked(QTableWidgetItem *item);

    /** Gestion de la demande de confirmation de sauvegarde */
    void demandeConfirmationSauvegardeFichier(const QString& filename);

    /** Gestion du bouton fermer */
    void on_btnFermer_clicked();

    /** Redirection en cas d'échec d'authentification */
    void ifAuthenticateFailed();

    /** Appelé pour confirmer la sauvegarde, quand on réutilise un nom de fichier */
    void slotAbeBoxFileManagerConfirmSave();

    void on_cbChoixAppliImporter_currentIndexChanged(int index);

    /** Code de suppression d'un item de la QListView ET du fichier qui y est attaché */
    void slotDeleteFile();

    /** Permet de detecter un changement de domaine et donc de changer l'uri du serveur de stockage de fichiers */
    void slotSSOStatusChanged(const int code);

    void on_btnChangeAffichage_clicked();

    void slotAbeBoxFileManagerSaveOnPC();
    void slotAbeBoxFileManagerSaveOnBox(enumAbulEduBoxFileManagerSavingLocation location);

    void slotManageOpenButton();

    void on_btnRechercher_clicked();

signals:
    void signalAbeFileSelected(QSharedPointer<AbulEduFileV1>);
    void signalAbeFileSelectedImport(QSharedPointer<AbulEduFileV1>);
    /** Nouvelle implementation, on pousse a migrer de la deprecated vers celle ci */
    void signalAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation quelleBox, const QString& fileName, bool reussi);

    /** On ferme la fenêtre ou on la cache */
    void signalAbeFileCloseOrHide();

    void signalAbeBoxFileManagerDownloadError(QNetworkReply::NetworkError);

 protected:
    void showEvent(QShowEvent *);
};

#endif // ABULEDUBOXFILEMANAGERV1_H
