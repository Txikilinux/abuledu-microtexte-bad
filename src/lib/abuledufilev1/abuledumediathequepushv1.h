/** Classe AbulEduMediathequePushV1
  *
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


#ifndef ABULEDUMEDIATHEQUEPUSH_H
#define ABULEDUMEDIATHEQUEPUSH_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QShortcut>
#include <QMenu>

#include "abuledumediathequecommon.h"
#include "abuledufilev1.h"
#include "maiaXmlRpcClient.h"
#include "abuledumimetypev1.h"
#include "abuleduidentitesv1.h"
#include "ui_abuledumediathequepushv1.h"
#include "abuleduloggerv1.h"

namespace Ui {
class AbulEduMediathequePushV1;
}

class AbulEduMediathequePushV1 : public QWidget, public AbulEduMediathequeCommon
{
    Q_OBJECT
    Q_ENUMS(enumMediathequeOrData)
    Q_PROPERTY(enumMediathequeOrData serviceDistant READ abeGetSourceEnum WRITE abeSetSourceEnum USER true)

public:
    enum enumMediathequeOrData { abeMediatheque, abeData, abeFichiers };

    explicit AbulEduMediathequePushV1(QWidget *parent = 0, QString mediathequeOrData = "mediatheque");
    ~AbulEduMediathequePushV1();


    /**
      * Permet d'activer le mode debug de cette classe
      */
    void abeSetDebugMode(bool value=false);

    /**
      * Passe un fichier AbulEduFileV1 à l'objet pour l'uploader
      */
    void abeSetFile(QSharedPointer<AbulEduFileV1> file);

    /**
      * Ou passe un fichier LOM pour mise à jour d'une notice.
      */
    void abeSetLOM(QSharedPointer<AbulEduSCOLOMFrV1> lom);

    /**
      * Permet de spécifier le code mime, c'est particulièrement utile pour expédier des fichiers
      * sur data.abuledu.org: le fichier .abb est un type application/abuledu mais ça ne permet pas
      * de faire des recherche du genre "je veux une photo", donc lors de l'upload on force depuis
      * l'application data le type mime en fonction du fichier qu'on embarque *DANS* le fichier .abb
      *
      * valeur par défaut: application/abuledu
      */
    void abeSetMimeType(QString mimeType);

    /**
      * Permet de basculer de data.abuledu.org a mediatheque.abuledu.org
      * Je la conserve le temps que tous les développeurs passent sur abeSetSourceEnum
      * mais avec l'enum (plus propre)
      * @param mediathequeOrData "mediatheque" ou "data"
      */
    void abeSetSource(QString mediathequeOrData);

    /**
      * Permet de basculer de data.abuledu.org a mediatheque.abuledu.org
      * @param mediathequeOrData cf enumMediathequeOrData
      */
    void abeSetSourceEnum(enumMediathequeOrData mediathequeOrData);

    /**
      * Permet de retourner l'information
      */
    enumMediathequeOrData abeGetSourceEnum() const;

    /**
      * Premet de basculer le push en mise à jour de ressource et non en envoi de nouvelle ressource
      */
    void abeSetUpdateURI(QString uriCode);

    /**
      * Premet de basculer le push en mise à jour du fichier LOM de ressource et non en envoi de nouvelle ressource
      */
    void abeSetUpdateURIforLOMupdate(QString uriCode);

    /**
      * Permet de valider rapidement une ressource en passant simplement l'URI, aucune modification de la ressource est faite,
      */
    void abeFastModerationAccept(QString uriCode);

    /**
      * Permet de refuser une modération
      */
    void abeFastModerationDeny(QString uriCode, QString motif);

    /** @deprecated */
    QString sluggify(QString str);

protected:
  void showEvent(QShowEvent *event);

private slots:
    /** Vide ou remet à zéro les différents éléments à remplir */
  void slotAbeMediathequePushClean();

    /**
      * Permet d'actualiser tous les champs à partir du fichier lom, factorisation de code entre le
      * constructeur, le setfile et setlom
      */
    void slotUpdateInterfaceFromLom();

    /**
      * Permet de mettre à jour le fichier lom à partir des renseignements contenus dans les différents
      * composants de l'interface ...
      */
    void slotUpdateLomFromInterface();

    /**
      * Bouton upload ... attention plusieurs cas de figure possible
      * En definitive on a 3 cas de figure possible:
      * 1. Expédition de fichier
      *   1.a sur DATA
      *   1.b sur MEDIATHEQUE
      * 2. Mise à jour de fichier
      *   2.a sur DATA
      *   2.b sur MEDIATHEQUE
      * 3. Mise à jour de descripteur lom
      *   3.a sur DATA
      *   3.b sur MEDIATHEQUE
      */
    void on_btnAbulEduMediathequePushUpload_clicked();
    void on_btnAbulEduMediathequePushClose_clicked();

    /**
      * Gestion de la réponse à notre demande d'une URI de stockage définitif
      */
    void rpcGetNewURI(QVariant &v, QNetworkReply *reply);

    /**
      * Gestion de la réponse à la demande d'envoi du fichier
      */
    void rpcSetFile(QVariant &v, QNetworkReply *reply);

    /**
      * Gestion des erreurs de communication RPC
      */
    void rpcFault(int code, const QString &errorMessage);


    void gestionBtnSuivantPrecedent(int mouvement);
    void on_btnAbulEduMediathequePushPrecedent_clicked();
    void on_btnAbulEduMediathequePushSuivant_clicked();

    void on_cbLOMLang_currentIndexChanged(const QString &arg1);

    /** les mots cles */
    void slotKeywordsDownloaded();
    void slotDeleteItemKeywords();
    void on_AbulEduMediathequePushbtnAjoutMotCle_clicked();
    void on_AbulEduMediathequePushlwSuggestions_itemDoubleClicked(QListWidgetItem *item);
    void on_AbulEduMediathequePushleMotCleManuel_returnPressed();
    void on_AbulEduMediathequePushbtnAjoutMotCle_2_clicked();

    /** 1.10 */
    void on_cbLOM1_10_currentIndexChanged(const QString &arg1);
    void on_lwLOM1_10_itemClicked(QListWidgetItem *item);

    /** 2.3 */
    void on_btnPlus2_3_clicked();
    void on_lwScoLOMFR2_3_itemClicked(QListWidgetItem *item);

    /** 5.2 */
    void on_cbLOM5_2_currentIndexChanged(const QString &arg1);
    void on_lwLOM5_2_itemClicked(QListWidgetItem *item);

    /** 5.6 */
    void on_cbLOM5_6_currentIndexChanged(const QString &arg1);
    void on_lwLOM5_6_itemClicked(QListWidgetItem *item);

    /** 5.12 */
    void on_cbLOM5_12_currentIndexChanged(const QString &arg1);
    void on_lwLOM5_12_itemClicked(QListWidgetItem *item);

    /** 5.14 */
    void on_cbLOM5_14_currentIndexChanged(const QString &arg1);
    void on_lwLOM5_14_itemClicked(QListWidgetItem *item);

    /** 5.15 */
    void on_cbLOM5_15_currentIndexChanged(const QString &arg1);
    void on_lwLOM5_15_itemClicked(QListWidgetItem *item);

    /** 5.16 */
    void on_cbLOM5_16_currentIndexChanged(const QString &arg1);

    void on_lwLOM5_16_itemClicked(QListWidgetItem *item);

    void on_AbulEduMediathequePushlwMotsCles_customContextMenuRequested(const QPoint &pos);

    /** Fonction récursive de parcours d'arbre XML */
    void examineChildElements(const QDomElement &parentElement, QTreeWidgetItem *parentItem, bool fin = false);

    void examineChildElementsBis(const QDomElement &parentElement, QTreeWidgetItem *parentItem);

    void on_twLOM9_1_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_lwLOM9_1selection_itemClicked(QListWidgetItem *item);

    void on_stackedWidgetAbulEduMediathequePush_currentChanged(int arg1);

    void on_btnAbulEduMediathequePushQuit_clicked();

private:
    Ui::AbulEduMediathequePushV1    *ui;
    QSharedPointer<AbulEduSCOLOMFrV1>  m_lom;           /** pointeur vers ScoLOMFR */
    QSharedPointer<AbulEduFileV1>    m_abuledufile;   /** pointeur vers fichier abuledu */
    MaiaXmlRpcClient                *m_rpc;           /** pointeur pour les connexions XMLRPC */
    AbulEduNetworkAccessManagerV1   *m_nam;           /** pointeur vers le network access manager partagé */
    QNetworkReply                   *m_networkReply;
    enumMediathequeOrData            m_mediathequeOrData; /** permet de savoir s'il faut se connecter sur le service data.abuledu.org ou mediatheque.abuledu.org */
    QString                          m_mimetype;          /** permet de stocker le type mime du document à envoyer, utile en particulier pour les fichier .abb qui embarquent une image/jpeg, on stocke le type mime image/jpeg plutot que application/abuledu qui ne permet pas de savoir ce qu'on embarque à l'interrieur */
    QString                          m_updateFileWithThisURICode;           // Stocke le code URI du type 47f4bea9 dans le cas ou on MET A JOUR une ressource
    QString                          m_updateFileLOMWithThisURICode;           // Stocke le code URI du type 47f4bea9 dans le cas ou on MET A JOUR un fichier LOM d'une ressource
    QString                          m_lang; /** la langue utilisée dans tout l'éditeur par defaut */

    void uploadeFile(QString URICode); //Uploade le fichier (mutualisation du code entre update et upload)

    QMenu                           *m_menuAbulEduMediathequePushlwMotsCles;

    QMap<QString,QString> m_fichiersXML;

    /**
      * Expédition du fichier LOM pour mettre à jour un fichier
      */
    void uploadeLOM();

    /** Enregistre le numéro d'index courant du QStackedWidget, mais mis à jour après exécution d'instructions dans on_stackedWidgetAbulEduMediathequePush_currentChanged(int arg1)
      * Permet d'accéder dans cette fonction à la page d'où on vient. */
    QWidget* m_previousStackedWidgetPage;


signals:
    /**
      * Informe que le fichier a ete uploadé (code 1 = OK, 0 = inconnu, -1 = ERR)
      */
    void signalMediathequePushFileUploaded(int code);
    void signalMediathequePushFileOver(bool trueFalse);
    void signalMediathequePushCloseOrHide();
};

#endif // ABULEDUMEDIATHEQUEPUSH_H
