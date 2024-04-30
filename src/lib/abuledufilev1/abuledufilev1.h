/** Classe AbulEduFileV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2010-2013 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2012 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
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


#ifndef ABULEDUFILEV1_H
#define ABULEDUFILEV1_H

#include <QTemporaryFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QDateTime>
#include <QFileInfo>
#include <QPainter>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QtSvg/QSvgRenderer>
#include <QSharedPointer>
#include <QTextDocumentWriter>
#include <sys/stat.h>

#ifdef __WITHEXIV2__
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#endif

#include "quazip.h"
#include "quazipfile.h"
#include "abuleduscolomfrv1.h"
#include "abuledumessageboxv1.h"
#include "abuledumimetypev1.h"
#include "abuleduloggerv1.h"

class AbulEduFileV1 : public QObject
{
    Q_OBJECT

public:
    explicit AbulEduFileV1(QObject *parent = 0);
    ~AbulEduFileV1();

    /**
      * Cette méthode retourne un identifiant unique pour le fichier temporaire
      * Elle est implémentée pour pallier les risques de sécurité liés au fichier codé en "dur"
      * Utilisation d'un QTemporyFile afin d'avoir un nom unique
      * C'est un fichier créé avec un id imprévisible
      * Je me sers de cet id pour renommer le dossier temp dont j'ai besoin =)
      * note -> le file temp créé s'efface en même tps que la destruction de l'objet
      */
    static const QString abeUniqIDTemp();

    /** @obsolete, claque un qwarning
      */
    static QString uniqIDTemp();


    /** Méthode qui redimensionne une image et la copie dans le repertoire destination sous le nom original.jpg
      * @warning : l'extension du fichier de destination est .jpg
      * @brief  Les formats supportés sont : "jpeg", "bmp", "png", "ppm", "xbm", "xpm" et "svg".
      *         Cependant, dans le cas du svg et du png, l'image retournée aura un fond blanc.
      *         Dans le cas des autres formats, l'image sera redimensionnée au profit de la largeur (@see QPixmap::scaledToWidth()).
      * @param QFileInfo *fileInfoImage , l'image d'origine a redimensionnée
      * @param int largeurMaxSouhaitee  , la largeur maximum souhaitée pour le redimensionnement
      * @param QString cheminDestination, la destination de l'image redimensionnée (le résultat)
      * @return bool, true si réussi, false si échec
      */
    bool resizeImage(QFileInfo *fileInfoImage, int largeurMaxSouhaitee, const QString& cheminDestination);


    /** Sauvegarde au format .abe -> tentative de convegence vers le format OpenDocument
      *
      * cf https://redmine.ryxeo.com/projects/ryxeo/wiki/Format_de_fichier_commun
      *
      * format = zip -> fichier zip tout con
      * format = abe -> fichier abe de type opendocument si on y arrive
      * destFileName = nom complet du fichier de destination (répertoire y compris, nom absolu)
      * fileList = liste des noms de fichiers a inclure dans l'archive (noms absolu)
      * fileBase = base commune de tous les fichiers a inclure (ce qui sera pris pour racine commune, nom absolu)
      * Exemple
      *  fileList = /home/toto/leterrier/logiciel/projet/conf/fichier1, /home/toto/leterrier/logiciel/projet/data/fichierA, /home/toto/leterrier/logiciel/projet/data/fichierB
      *  fileBase = /home/toto/leterrier/logiciel/projet
      *  destFileName = /home/toto/mon_exercice.abe
      */
    bool abeFileSave(QString destFileName, QStringList fileList, QString fileBase = "", QString fileFormat = "zip", bool makeThumbnail = true);

    /** Prépare la sauvegarde, le pointeur abuledufilev1 sera ensuite passé à abuleduboxfilemanager qui fera "l'export" et écrira le fichier
      * soit sur le disque, soit sur la box, soit ...
      *
      * cf https://redmine.ryxeo.com/projects/ryxeo/wiki/Format_de_fichier_commun
      *
      * format = zip -> fichier zip tout con
      * format = abe -> fichier abe de type opendocument si on y arrive
      * fileList = liste des noms de fichiers a inclure dans l'archive (noms absolu)
      * fileBase = base commune de tous les fichiers a inclure (ce qui sera pris pour racine commune, nom absolu)
      * Exemple
      *  fileList = /home/toto/leterrier/logiciel/projet/conf/fichier1, /home/toto/leterrier/logiciel/projet/data/fichierA, /home/toto/leterrier/logiciel/projet/data/fichierB
      *  fileBase = /home/toto/leterrier/logiciel/projet
      */
    bool abeFileExportPrepare(const QStringList& fileList, const QString &fileBase = "", const QString &fileFormat = "zip");

    /** Export le fichier preparé à l'étape précédente
      * sera probablement utilisé par abuleduboxfilemanager quand l'utilisateur aura décidé du nom du fichier
      */
    bool abeFileExportFile(const QString& destFileName);

    /** Ouverture d'un fichier
      * format = zip -> fichier zip tout con, on l'extrait dans le repertoire dirDest
      * format = abe -> fichier abe de type opendocument si on y arrive -> traitement a definir
      */
    bool abeFileOpen(const QString& srcFile, const QString &fileFormat = "zip");
    bool abeFileOpen(const QString& srcFile, QDir *dest, const QString &fileFormat = "zip");

    // =============================================================================================================

    /** Copie les meta data d'un fichier abb vers un fichier abe
      * exemple : m_abuleduFile->abeFileAddMetaDataFromABB(ui->mediathequeGetABB->abeGetLOM(), ui->mediathequeGetABB->abeGetFile()->abeFileGetContent(0).baseName());
      *
      */
    bool abeFileAddMetaDataFromABB(QSharedPointer<AbulEduSCOLOMFrV1> abbFile, const QString& abbFileName);

    /** Retourne des informations de "debug" de l'objet */
    QString abeFileDebug();

    /************************************************************************************
                                Getter & Setter
    ************************************************************************************/

    /** Affectation du nom du fichier, sans l'ouvrir */
    void abeFileSetFilename(const QString& filename);
    const QFileInfo abeFileGetFileName(){ return QFileInfo(m_filename);}

    /** La langue du contenu intellectuel de la ressource, fr-fr, en-uk */
    void abeFileSetLanguage(const QString &lang){m_language = lang;}
    const QString& abeFileGetLanguage(){return m_language;}

    /** L'entité principalement responsable de la création du contenu de la ressource */
    void abeFileSetCreator(const QString &creator){m_creator = creator;}
    const QString& abeFileGetCreator(){ return m_creator;}

    /** Une date associée avec un événement dans le cycle de vie de la ressource au format ISO8601 */
    void abeFileSetDate(const QString &date){m_date = date;}
    const QString& abeFileGetDate(){return m_date;}

    /** Typiquement, le sujet sera décrit par un ensemble de mots-clefs ou de phrases ou un code de classification qui précisent le sujet */
    void abeFileSetSubject(const QString &subject){m_subject = subject;}
    const QString& abeFileGetSubject(){return m_subject;}

    /** Typiquement, un titre sera le nom par lequel la ressource est officiellement connue */
    void abeFileSetTitle(const QString &title){m_title = title;}
    const QString& abeFileGetTitle(){return m_title;}

    /** Typiquement, un élément Droits contiendra un état du droit à gérer une ressource, ou la reférence à un service fournissant cette information.*/
    void abeFileSetRights(const QString &rights){m_rights = rights;}
    const QString& abeFileGetRights(){return m_rights;}

    /** Une Description peut contenir, mais ce n'est pas limitatif un résumé, une table des matières, une référence à une représentation graphique du contenu, ou un texte libre */
    void abeFileSetDescription(const QString &description){m_description = description;}
    const QString& abeFileGetDescription(){return m_description;}

    /** Exemples d'Editeurs incluent une personne, une organisation, ou un service. Typiquement, le nom d'une maison d'édition*/
    void abeFileSetPublisher(const QString &publisher){m_publisher = publisher;}
    const QString& abeFileGetPublisher(){return m_publisher;}

    /** Une entité qui a contribué à la création du contenu*/
    void abeFileSetContributor(const QString &contributor){m_contributor = contributor;}
    const QString& abeFileGetContributor(){return m_contributor;}

    /** La liste des mots clés */
    void abeFileSetKeywords(const QString &keywords){m_keywords = keywords;}
    const QString& abeFileGetKeywords(){return m_keywords;}

    /** Le type inclut des termes décrivant des catégories, fonctions ou genres généraux pour le contenu, ou des niveaux d'agrégation, Il est recommandé de choisir la valeur du type dans une liste de valeurs
      * @see http://dublincore.org/documents/resource-typelist/ */
    void abeFileSetType(const QString &type){m_type = type;}
    const QString abeFileGetType(){return m_type;}

    /** Typiquement, Format peut inclure le media ou les dimensions de la ressource, Exemples de dimensions incluent la taille et la durée. Il est recommandé de choisir la valeur du format dans une liste (ex.mime)
      * @see http://www.iana.org/assignments/media-types/index.html */
    void abeFileSetFormat(const QString &format){m_format = format;}
    const QString& abeFileGetFormat(){return m_format;}

    /** Il est recommandé d'identifier la ressource par une chaîne de caractère ou un nombre conforme à un sytème formel d'identification, ex. ISBN, ISSN, URL */
    void abeFileSetIdentifier(const QString &identifier){m_identifier = identifier;}
    const QString& abeFileGetIdentifier(){ return m_identifier;}

    /** La ressource actuelle peut avoir été dérivée d'une autre ressource source, en totalité ou en partie.  */
    void abeFileSetSource(const QString &source){m_source = source;}
    const QString& abeFileGetSource(){return m_source;}

    /** Une référence à une autre ressource qui a un rapport avec cette ressource */
    void abeFileSetRelation(const QString &relation){m_relation = relation;}
    const QString& abeFileGetRelation(){return m_relation;}

    /** La portée ou la couverture spatio-temporelle de la ressource. Il est recommandé de choisir la  valeur de Couverture dans un vocabulaire contrôlé (par exemple, un thésaurus de noms géographiques, comme[TGN]) */
    void abeFileSetCoverage(const QString &coverage){m_coverage = coverage;}
    const QString abeFileGetCoverage(){return m_coverage;}

    /** Definit la version des modules supporté */
    void abeFileSetUnitVersion(const QString& version){m_unitVersion = version;}
    const QString& abeFileGetUnitVersion(){return m_unitVersion;}

    /** Retourne le nom complet du repertoire temporaire dans lequel le fichier est extrait */
    const QDir& abeFileGetDirectoryTemp();

    /** Retourne un pointeur sur le fichier LOM */
    const QSharedPointer<AbulEduSCOLOMFrV1> abeFileGetLOM(){return m_lom;}

    /** Permet d'affecter des tags EXIF (pour les images)
      * @param file le fichier pour lequel il faut modifier les tags exif
      * @param artiste le nom de l'auteur
      * @param copyright la licence utilisée
      * @uniqID l'identifiant unique de type http://data.abuledu.org/URI/47f422e0 LIMITE a 128 caracteres, on supprime donc tout le nom 'texte' pour ne garder que la clé
      */
    static bool abeFileSetExiv2Tags(const QFileInfo& file, const QString& artist, const QString& copyright, const QString& uniqID);

    /** Permet de récupérer des tags EXIF (pour les images)
      * @param file le fichier pour lequel il faut récupérer les tags exif
      * @return tableau associatif des données récupérées
      */
    static QMap<QString, QString> abeFileGetExiv2Tags(const QFileInfo& file);

    /** Copie tous les tags EXIF d'un fichier vers un autre */
    static bool abeFileCopyExiv2Tags(const QString& src, const QString& dst);

    /** Permet d'avoir en retour un format de fichier bien formatté en Mo / Ko etc. */
    static QString abeFileFormatSize(qint64 num);

    /** Retourne le contenu de l'archive
      * Dans le cas particulier d'un .abb il n'y aura qu'une seule ressource
      * Dans le cas des .abe on pourra faire une boucle
      */
    const QFileInfo abeFileGetContent(int num);

    /** Retourne la liste des fichiers contenus dans l'archive (initialise quand on ouvre un fichier) */
    QStringList abeFileGetFileList(bool withReservedFileNames=false);

    /** Calcule la somme MD5 du fichier et la retourne */
    const QString abeFileGetMD5(){return abeFileGetMD5(m_filename);}

    /** Calcule la somme MD5 du fichier passé en paramètre et la retourne */
    static const QString abeFileGetMD5(const QString &otherFileName);

    /** Retourne le fichier sous forme d'une chaine encodée en base64 (utile pour les transferts push/get vers/depuis la médiathèque) */
    const QString abeFileGetBase64Encoded();

    /** Retourne directement le thumbnail du fichier
      * Exemple QIcon icon(abuledufile->abeFileGetIcon()) */
    QPixmap abeFileGetIcon();

    /** Retourne le nom de l'application qui a ete utilise pour creer ce fichier */
    const QString& abeFileGetOriginalSoftware(){ return m_originalSoftware;}

    // ======================================================================== UPDATeur
    void abeFileUpdateLOM();
    void abeFileUpdateDesc();


    // ======================================================================== Divers
    /** Extrait le thumbnail du fichier source vers la destination destination :)
      * Exemple abeExtractIcon("/tmp/toto.abe","/tmp/thumbnails/toto.png")
      */
    static void abeFileExtractIcon(const QString& source, const QString& destination);

    /** Extrait n'importe quel fichier de l'archive et le copie dans destination
      *
      * @param source nom du fichier abe source
      * @param fileSearch nom du fichier qu'on souhaite extraire
      * @param destination nom du fichier complet sous lequel on veut l'exporter
      * @return bool true si l'opération s'est bien passée
      *
      * @code
      *     if(abeFileExtract("/tmp/toto.abe","Thumbnails/thumbnail.png","/tmp/toto.png")) ...
      * @endcode
      */
    bool abeFileExtract(const QString& source, const QString& fileSearch, const QString& destination);

    /** Permet de dire si on veut ou pas exporter le fichier lom.xml dans le fichier .abe (ne marche que pour le format .abe) */
    void abeFileSetLOMExport(bool value=true) {m_exportLOM = value;}

    /** icham 20130809 : je fais un clear de l'ancien pointeur (comme c'etait ds le .cpp)
      * Par contre, est-ce que c'est pertinent ? (QSharedPointer ref = 0 -> nouvelle affectat°)
      */
    void abeFileSetLOM(QSharedPointer<AbulEduSCOLOMFrV1> lom){m_lom.clear();m_lom = lom;}

    /** Nettoie un répertoire (temporaire) */
    static bool abeCleanDirectory(const QString& dirPath, const QString& dirInitial);

    /** Supprime le repertoire dirPath et tout ce qu'il contient (récursif)
      * utilisation du code de l'implémentation de Qt 5
      */
    static bool abeCleanDirectoryRecursively(const QString &dirPath, int trashSize = 0);

    /** Nettoie le répertoire temporaire du fichier */
    bool abeClean();

    /** Retourne récursivement sous forme de QStringList le contenu du dossier dont le nom est passé en paramètre */
    QStringList abeFileGetRecursiveContent(const QString dirName);

signals:
    void abeSignalFileOpen(int errcode);

public slots:

private:
    void domAddElement(const QString& tagName, const QString& value, QDomElement *parent);
    void fileOpenHandleSpecialFile(const QString& fileName, const QString& absoluteFilePath);
    QByteArray makeContentXML();
    QByteArray makeManifestXML(const QStringList& fileList, const QString& fileBaseName);
    QByteArray makeManifestRDF();

    bool m_isTemporaryDir; /** Flag pour savoir si on extrait le fichier abe dans un repertoire temporaire (en ce cas on le nettoie a la sortie) ou pas */
    QDomDocument m_domDocument; /** */

    //Le dublin core source traduction http://www-rocq.inria.fr/~vercoust/METADATA/DC-fr.1.1.html
    QString m_language;    /** La langue du contenu intellectuel de la ressource, fr-fr, en-uk */
    QString m_creator;     /** L'entité principalement responsable de la création du contenu de la ressource */
    QString m_date;        /** Une date associée avec un événement dans le cycle de vie de la ressource au format ISO8601 */
    QString m_subject;     /** Typiquement, le sujet sera décrit par un ensemble de mots-clefs ou de phrases ou un code de classification qui précisent le sujet */
    QString m_title;       /** Typiquement, un titre sera le nom par lequel la ressource est officiellement connue */
    QString m_rights;      /** Typiquement, un élément Droits contiendra un état du droit à gérer une ressource, ou la reférence à un service fournissant cette information.*/
    QString m_description; /** Une Description peut contenir, mais ce n'est pas limitatif un résumé, une table des matières, une référence à une représentation graphique du contenu, ou un texte libre */
    QString m_publisher;   /** Exemples d'Editeurs incluent une personne, une organisation, ou un service. Typiquement, le nom d'une maison d'édition */
    QString m_contributor; /** Une entité qui a contribué à la création du contenu */
    QString m_keywords;    /** Typiquement, la liste des mots clés au format suivant: mot1, mot2, mot3 */
    QString m_type;        /** Le type inclut des termes décrivant des catégories, fonctions ou genres généraux pour le contenu, ou des niveaux d'agrégation, Il est recommandé de choisir la valeur du type dans une liste de valeurs
                             * @see http://dublincore.org/documents/resource-typelist/
                             */
    QString m_format;      /** Typiquement, Format peut inclure le media ou les dimensions de la ressource, Exemples de dimensions incluent la taille et la durée. Il est recommandé de choisir la valeur du format dans une liste (ex.mime)
                             * @see http://www.iana.org/assignments/media-types/index.html
                             */
    QString m_identifier;  /** Il est recommandé d'identifier la ressource par une chaîne de caractère ou un nombre conforme à un sytème formel d'identification, ex. ISBN, ISSN, URL */
    QString m_source;      /** La ressource actuelle peut avoir été dérivée d'une autre ressource source, en totalité ou en partie.  */
    QString m_relation;    /** Une référence à une autre ressource qui a un rapport avec cette ressource */
    QString m_coverage;    /** La portée ou la couverture spatio-temporelle de la ressource. Il est recommandé de choisir la  valeur de Couverture dans un vocabulaire contrôlé (par exemple, un thésaurus de noms géographiques, comme[TGN]) */

    QString m_filename;    /** Le nom du fichier tout simplement */
    QString m_fileformat;  /** Le format du fichier (abe ou zip) */
    QString m_fileBase;    /** le chemin de base pour "relativiser" les chemins des fichiers a inclure, par exemple
                             * fileList contient /home/utilisateurs/eric.seigne/leterrier/leterrier-imageo/lesanimaux/data/images/girafe.png
                             * et le logiciel a en fait uniquement besoin d'avoir "lesanimaux/data/images/girafe.png"
                             * donc fileBase vaudrait "/home/utilisateurs/eric.seigne/leterrier/leterrier-imageo/"
                             */
    QString m_originalSoftware; /** Nom de code du logiciel utilise pour creer ce fichier */

    QString m_unitVersion; /** Numéro de la version du module */

    QDir m_directoryTemp;  /** Un repertoire temporaire dans lequel on extrait le fichier */

    bool     m_exportLOM;  /** Permet de choisir si on veut ou pas export du LOM dans le projet (ne marche qu'avec le format abe)*/
    QSharedPointer<AbulEduSCOLOMFrV1> m_lom;    /** Le debut de la gestion LOM dans abuledufilev1 */

    QStringList m_fileList; /** La liste des fichiers contenu dans l'archive */
    QStringList m_reservedFileNames; /** La liste des fichiers réservés */

    QSvgRenderer *m_svgRenderer; /** Permet de gérer le format svg @see resizeImage(..) */
};

#endif // ABULEDUFILEV1_H
