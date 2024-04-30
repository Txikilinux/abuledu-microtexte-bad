/** Classe AbulEduFileV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2010-2013 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2011-2012 Rokia Lamrani Alaoui <rokia.lamrani.alaoui@gmail.com>
  * @author 2012 Icham Sirat <icham.sirat@ryxeo.com>
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

#include "abuledufilev1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduFileV1::AbulEduFileV1(QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "parent:" << parent;

    abeClean();


    ABULEDU_LOG_DEBUG() << "=================================================== AbulEduFileV1 : new : "
                        << m_directoryTemp << " parent : " << parent;
}

AbulEduFileV1::~AbulEduFileV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "=================================================== ~AbulEduFileV1 [temp: "
                        << m_isTemporaryDir << "] ("
                        << objectName() << ")"
                        << " : " << m_directoryTemp.absolutePath();
    /* Nettoyage lors de la suppression de l'objet: si on est dans le temp path c'est clairement un repertoire temporaire on vire tout */
    if((m_isTemporaryDir && m_directoryTemp.absolutePath() != "" && m_directoryTemp.absolutePath() != QDir("").absolutePath() && m_directoryTemp.absolutePath() != "/")
            || m_directoryTemp.absolutePath().startsWith(QDir::tempPath()))
    {
        ABULEDU_LOG_DEBUG() << objectName() << "  == rmpath : " << m_directoryTemp.absolutePath();
        //appel recursif, c'est une extraction temporaire !
        abeCleanDirectoryRecursively(m_directoryTemp.absolutePath());
    }
    m_lom.clear();
}

const QString AbulEduFileV1::abeUniqIDTemp()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QTemporaryFile file;
    QString strArenvoyer ="";
    if (file.open()) {
        strArenvoyer = file.fileName();
        file.close();
    }
    //Dans le cas ou ca ne marcherait pas on fais un nom a l'ancienne
    if(strArenvoyer.isEmpty()) {
        strArenvoyer = QDir::tempPath() + "/" + abeApp->applicationName() + "-temp";
    }
    return strArenvoyer;
}

bool AbulEduFileV1::resizeImage(QFileInfo *fileInfoImage, int largeurMaxSouhaitee, const QString &cheminDestination)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QStringList formatsSupportesBase;
    formatsSupportesBase << "jpg" << "jpeg" << "bmp" << "ppm" << "xbm" << "xpm" ;
    QStringList formatsSupportesAutres;
    formatsSupportesAutres << "png" << "svg";

    if(formatsSupportesBase.contains(fileInfoImage->suffix().toLower()))
    {
        QImage *imageBase = new QImage(largeurMaxSouhaitee, largeurMaxSouhaitee, QImage::Format_ARGB32);
        imageBase->fill(Qt::transparent);

        if(imageBase->load(fileInfoImage->absoluteFilePath()))
        {
            QImage imageRetaillee = imageBase->scaledToWidth(largeurMaxSouhaitee, Qt::SmoothTransformation);

            if(!imageRetaillee.save(cheminDestination + QDir::separator() + fileInfoImage->baseName() + ".jpg","JPEG"))
            {
                ABULEDU_LOG_DEBUG() << "Image non retaillee";
                return false;
            }
            return true;
        }
        else
        {
            ABULEDU_LOG_DEBUG() << "Probleme chargement ...";
            return false;
        }
    }
    if(formatsSupportesAutres.contains(fileInfoImage->suffix().toLower())){
        QPixmap pix(fileInfoImage->absoluteFilePath());
        float pixRatio;
        /* Protection destinée à empêcher une division par 0 aux lignes dessous */
        if(pix.height() == 0 || pix.width() == 0){
            pixRatio = 1;
        }
        else{
            pixRatio = (float)pix.width()/pix.height();
        }
        float hauteurMaxSouhaitee = largeurMaxSouhaitee/pixRatio;
        if(fileInfoImage->suffix().toLower() == "svg")
        {
            ABULEDU_LOG_DEBUG() << "C'est du svg, nous allons utilisé qsvgRender";
            m_svgRenderer =  new QSvgRenderer(fileInfoImage->absoluteFilePath());
            if (m_svgRenderer->isValid())
            {
                QImage *img = new QImage(largeurMaxSouhaitee, hauteurMaxSouhaitee, QImage::Format_ARGB32);
                img->fill(QColor(Qt::white).rgb());
                QPainter pai(img);
                m_svgRenderer->render(&pai);
                QPixmap resultat = QPixmap::fromImage(*img);

                delete m_svgRenderer;
                if(!resultat.save(cheminDestination + QDir::separator() + fileInfoImage->baseName() + ".jpg","JPEG"))
                {
                    ABULEDU_LOG_DEBUG() << "Probleme sauvegarde SVG";
                    return false;
                }
                return true;
            }
            else
            {
                ABULEDU_LOG_DEBUG() << "le fichier svg est invalide !";
                return false;
            }
        }
        else if(fileInfoImage->suffix().toLower() == "png")
        {
            QImage *img = new QImage(largeurMaxSouhaitee, hauteurMaxSouhaitee, QImage::Format_ARGB32);
            img->fill(QColor(Qt::white).rgb());
            QPainter painter(img);
            QImage imageBase(fileInfoImage->absoluteFilePath());
            QRectF cible(0,0,largeurMaxSouhaitee,largeurMaxSouhaitee);
            painter.drawImage(cible, imageBase, imageBase.rect());
            if(!img->save(cheminDestination + QDir::separator() + fileInfoImage->baseName() + ".jpg","JPEG"))
            {
                ABULEDU_LOG_DEBUG() << "Problem Sauvegarde PNG";
                return false;
            }
            return true;
        }
    }
    else
    {
        ABULEDU_LOG_DEBUG() << "Le format de l'image n'est pas supporte";
        return false;
    }
    return false;
}

QString AbulEduFileV1::uniqIDTemp()
{
    ABULEDU_LOG_WARN() << "AbulEduFileV1::uniqIDTemp : fonction obsolete, merci d'utiliser AbulEduFileV1::abeUniqIDTemp";
    return abeUniqIDTemp();
}


//--------------------------------------------------------------------------------------------------------------------
/**
  * @fn cette fonction permet de générer un fichier .zip ou .abe contenant tout ce que le developpeur souhaite
  * @param destFileName = nom du fichier de destination
  * @param fileList: une liste de fichiers a inclure dans l'archive
  * @param fileBase: le chemin de base pour "relativiser" les chemins des fichiers a inclure, par exemple
  *                  fileList contient /home/utilisateurs/eric.seigne/leterrier/leterrier-imageo/lesanimaux/data/images/girafe.png
  *                  et le logiciel a en fait uniquement besoin d'avoir "lesanimaux/data/images/girafe.png"
  *                  donc fileBase vaudrait "/home/utilisateurs/eric.seigne/leterrier/leterrier-imageo/"
  * @param fileFormat: le format du fichier a generer: abe pour un fichier opendocument "abuledu" (dev en cours)
  *                    ou abb pour des ressources brutes
  *                    ou zip pour faire un fichier .zip tout simple
  */
bool AbulEduFileV1::abeFileSave(QString destFileName, QStringList fileList,
                                QString fileBase, QString fileFormat, bool makeThumbnail)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "  destFileName:" << destFileName
                        << "  fileList:"     << fileList
                        << "  fileBase:"     << fileBase
                        << "  fileFormat:"   << fileFormat;

    bool retour = false;
    bool thumbailIsPresent = false;

//NOTE IMPORTANTE: PAS DE CONST POUR POUVOIR TRAVAILLER SUR LES VARIABLES AVANT DE LES AFFECTER AUX VARIABLES
//MEMBRES ...

    /* Ajout du "/" final pour fileBase s'il manque */
    if( fileBase.right(1) != "/" ) {
        fileBase.append("/");
    }

    /* Dans le cas de la mediathequepush, il ne faut pas qu'on remette le PATH au moment de re-générer le fichier abe */
    if(m_directoryTemp.absolutePath().startsWith(QDir::tempPath())) {
        QDir().rmdir(m_directoryTemp.absolutePath());
    }

    // qDebug() << " Avant " << m_directoryTemp.absolutePath() << " et fileBase " << fileBase;
    if(!fileBase.trimmed().isEmpty()) {
        m_directoryTemp.cd(fileBase);
    }

    // qDebug() << " Après " << m_directoryTemp.absolutePath();
    /* Format abb == abe mais c'est juste pour etre plus comprehensible par les developpeurs, idem pour les abf */
    if(fileFormat == "abb" || fileFormat == "abf") {
        fileFormat = "abe";
    }

    /* Si on decide de faire un fichier abe de type opendocument */
    if(fileFormat == "abe") {
        for (int i = 0; i < m_reservedFileNames.size(); i++) {
            fileList.removeAll(fileBase + m_reservedFileNames.at(i));
        }

        /* fichier .abe ou .abb (ressource brute) ou abf (ressource intermédiaire) */
        if(!destFileName.toLower().endsWith(".abe") && !destFileName.toLower().endsWith(".abb")
                && !destFileName.toLower().endsWith(".abf")) {
            destFileName.append(".abe");
        }
        /* Creation du fichier meta.xml cf OpenDocument */
        m_domDocument.clear();
        QDomElement officeDocMeta = m_domDocument.createElement("office:document-meta");
        m_domDocument.appendChild(officeDocMeta);
        officeDocMeta.setAttribute("xmlns:office","urn:oasis:names:tc:opendocument:xmlns:office:1.0");
        officeDocMeta.setAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
        officeDocMeta.setAttribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
        officeDocMeta.setAttribute("xmlns:meta", "urn:oasis:names:tc:opendocument:xmlns:meta:1.0");
        officeDocMeta.setAttribute("xmlns:ooo", "http://openoffice.org/2004/office");
        officeDocMeta.setAttribute("xmlns:grddl", "http://www.w3.org/2003/g/data-view#");
        officeDocMeta.setAttribute("office:version", "1.2");
        officeDocMeta.setAttribute("grddl:transformation", "http://docs.oasis-open.org/office/1.2/xslt/odf2rdf.xsl");
        QDomElement officeMeta = m_domDocument.createElement("office:meta");
        officeDocMeta.appendChild(officeMeta);

        QString os("");
#if defined(Q_OS_ANDROID)
        os="Android";
#elif defined(Q_OS_LINUX)
        os="Unix";
#elif defined(Q_OS_WIN)
        os="Windows";
#elif defined(Q_WS_MAC)
        os="Mac";
#endif

        domAddElement("meta:generator", abeApp->applicationName() +  "/" + abeApp->applicationVersion() + "$" + os + " " + abeApp->getAbeApplicationLongName(), &officeMeta);

        vCard v = m_lom->abeLOMgetLifeCycleContributionEntities("author",QDate::currentDate()).at(0);
        m_creator = v.getFullName();

        domAddElement("meta:initial-creator", m_creator, &officeMeta);
        domAddElement("dc:creator", m_creator, &officeMeta);

        QString langueLOM;
        if(m_lom->abeLOMgetGeneralLanguage().size() > 0) {
            langueLOM = m_lom->abeLOMgetGeneralLanguage().first();
        }

        m_title = m_lom->abeLOMgetGeneralTitle(langueLOM);
        domAddElement("dc:title", m_title, &officeMeta);

        if(m_lom->abeLOMgetGeneralDescription(langueLOM).size() > 0) {
            m_description = m_lom->abeLOMgetGeneralDescription(langueLOM).first();
        }
        domAddElement("dc:description", m_description, &officeMeta);

        m_date = QDate::currentDate().toString(Qt::ISODate);
        domAddElement("dc:date", m_date, &officeMeta);

        m_language = QLocale::system().name().section('_', 0, 0);
        domAddElement("dc:language", m_language, &officeMeta);

        m_keywords = m_lom->abeLOMgetGeneralKeywords(langueLOM).join(",");
        /* @todo@ a ameliorer, il semblerait qu'il faille un bloc meta:keyword par mot clé et donc utiliser une qstringlist */
        domAddElement("meta:keyword", m_keywords, &officeMeta);

        //2015.05.03 -> relecture de http://dublincore.org/documents/dces/ ... dc:subect doit contenir les mots cles
        domAddElement("dc:subject", m_keywords, &officeMeta);


        /* les elements suivants ne font pas partie de la norme, je les garde sous le coude au cas-ou ...
           domAddElement("dc:publisher", m_publisher, &officeMeta); // TODO
           domAddElement("dc:contributor", m_contributor, &officeMeta); // TODO
           domAddElement("dc:type", m_type, &officeMeta); // TODO
           domAddElement("dc:format", m_format, &officeMeta); // TODO
           domAddElement("dc:identifier", m_identifier, &officeMeta); // TODO
           domAddElement("dc:source", m_source, &officeMeta); // TODO
           domAddElement("dc:relation", m_relation, &officeMeta); // TODO
           domAddElement("dc:coverage", m_coverage, &officeMeta); // TODO
           domAddElement("dc:rights", m_rights, &officeMeta); */

        QDomNode noeud = m_domDocument.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        m_domDocument.insertBefore(noeud,m_domDocument.firstChild());

    }
    else {
        if(!destFileName.toLower().endsWith(".zip")) {
            destFileName.append(".zip");
        }
    }

    //INITIALISATION DES VARIABLES ICI ET NON AU DEBUT DE LA FONCTION, C'EST NORMAL
    m_filename   = destFileName;
    m_fileList   = fileList;
    m_fileformat = fileFormat;

    /* Creation d'une archive ZIP dans tous les cas */
    QuaZip zipDestination(destFileName);
    if(zipDestination.open(QuaZip::mdCreate)) {
        /* On sauvegarde tout dans ce fichier */
        QuaZipFile fichierDehors(&zipDestination);
        /* Les droits par defaut en 644 pour les fichiers contenus dans le .zip */
        QuaZipNewInfo newInfo("");
        int permission644 = 33188;
        newInfo.externalAttr = (permission644 << 16L);

        /* Si on decide de faire un fichier abe de type opendocument */
        if(fileFormat == "abe") {
            const QByteArray metaXML  = m_domDocument.toByteArray();
            // En tout premier le mimetype en non compresse
            //        QByteArray mime("application/abuledu.document");
            //        mime.append(abeApp->applicationName());
            // Je continue a utiliser celui de opendocument pour que alfresco l'ouvre et mange les metadonnees ...
            // a terme on trouvera le moyen de dire a alfresco de ne pas se casser la tete :)
            //            QByteArray mime("application/vnd.oasis.opendocument.text");
            const QByteArray mime("application/abuledu");

            newInfo.name = "mimetype";
            if(fichierDehors.open(QIODevice::WriteOnly,newInfo, NULL, 0, 0, 0)) {
                fichierDehors.write(mime);
                fichierDehors.close();
            }

            /* Fichier "interne" abuledu -> octet 94
             * Evite de decompresser l'archive pour avoir le nom du logiciel qui l'a cree ... */
            newInfo.name = "abuledu";
            QByteArray mime2(abeApp->applicationName().toUtf8());
            if(fichierDehors.open(QIODevice::WriteOnly,newInfo, NULL, 0, 0, 0)) {
                fichierDehors.write(mime2);
                fichierDehors.close();
            }

            /* Numero de version du module */
            newInfo.name = "version";
            QByteArray mime3(abeFileGetUnitVersion().toUtf8());
            if(fichierDehors.open(QIODevice::WriteOnly, newInfo, NULL, 0, 0, 0)) {
                fichierDehors.write(mime3);
                fichierDehors.close();
            }

            /* meta.xml */
            newInfo.name = "meta.xml";
            if(fichierDehors.open(QIODevice::WriteOnly, newInfo)) {
                fichierDehors.write(metaXML);
                fichierDehors.close();
            }

            /* content.xml */
            const QByteArray contentXML = makeContentXML();
            newInfo.name = "content.xml";
            if(fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                fichierDehors.write(contentXML);
                fichierDehors.close();
            }

            /* META-INF/manifest.xml */
            const QByteArray manifestXML = makeManifestXML(fileList, fileBase);
            newInfo.name = "META-INF/manifest.xml";
            if(fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                fichierDehors.write(manifestXML);
                fichierDehors.close();
            }

            /* manifest.rdf */
            const QByteArray manifestRDF = makeManifestRDF();
            newInfo.name = "manifest.rdf";
            if(fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                fichierDehors.write(manifestRDF);
                fichierDehors.close();
            }

            /* lom.xml */
            if(fileFormat == "abe" && m_exportLOM) {
                const QByteArray lom = m_lom->abeLOMExportAsXML();
                newInfo.name = "lom.xml";
                if(fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                    fichierDehors.write(lom);
                    fichierDehors.close();
                }
            }
        } /* #fin m_fileformat == abe */

        /* Dans tous les cas zip ou abe on integre les fichiers qui composent le projet ...
         * question, dans l'opendocument, faut-il stocker les fichiers dans un repertoire special ?
         * Les fichiers */
        for(int i = 0; i < fileList.count(); i++) {
            const QString chemin = fileList.at(i);
            QFile fic(chemin);
            QFileInfo fi(fic);
            QString fileName = fi.absoluteFilePath();
            fileName.remove(fileBase);
            ABULEDU_LOG_DEBUG() << "On ajoute le fichier " << chemin << " -> " << fileName;
            newInfo.name = fileName;
            if(fic.open(QIODevice::ReadOnly) && fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                fichierDehors.write(fic.read(fic.size()));
                fichierDehors.close();
                fic.close();
            }

            if(makeThumbnail){
                /* Si dans la liste on a une image -> hop on fait le thumbnail (de la 1ere image de la liste)
                 * Ensuite, le fichier Thumbnails/thumbnail.png */
                if(!thumbailIsPresent &&
                        (fileName.toLower().endsWith(".jpg") ||
                         fileName.toLower().endsWith(".jpeg") ||
                         fileName.toLower().endsWith(".svg") ||
                         fileName.toLower().endsWith(".png")))
                {
                    QPixmap imageTemp(fi.absoluteFilePath());

                    QPixmap thumbnail = imageTemp.scaled(QSize(124,124), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    QPixmap thumbnailFond = QPixmap(":/abuledufilev1/fond-vide");
                    QPainter p( &thumbnailFond );
                    /* On centre le thumbnail sur le cadre de fond */
                    p.drawPixmap(2 + (124-thumbnail.width())/2, 2 + (124-thumbnail.height())/2, thumbnail.width(), thumbnail.height(), thumbnail);

                    newInfo.name = "Thumbnails/thumbnail.png";
                    if(fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                        thumbnailFond.save(&fichierDehors,"PNG");
                        fichierDehors.close();
                    }
                    /* Evite de refaire un autre thumbnail si on a d'autres images dans la liste */
                    thumbailIsPresent = true;
                }

                /* Si c'est une vidéo, on regarde, peut-être qu'on a un thumbnail en cache */
                if(!thumbailIsPresent && (fileName.toLower().endsWith(".ogv")))
                {
                    const QFileInfo fiThumb(fi.absolutePath() + "/.thumbnails/" + fi.fileName().replace("ogv","png"));
                    if(fiThumb.exists()) {
                        QPixmap thumbnailFond(fiThumb.absoluteFilePath());
                        newInfo.name = "Thumbnails/thumbnail.png";
                        if(fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                            thumbnailFond.save(&fichierDehors,"PNG");
                            fichierDehors.close();
                        }
                        /* Evite de refaire un autre thumbnail */
                        thumbailIsPresent = true;
                    }
                }

                /* Vérifie si on n'a pas de thumbnail il faut en créer un à la mano */
                if(!thumbailIsPresent) {
                    QPixmap thumbnail(":/abuledufilev1/fond-vide");
                    if(m_format.startsWith("text/")) {
                        thumbnail = QPixmap(":/abuledufilev1/fond-text");
                    }
                    else if(m_format.startsWith("audio/")) {
                        thumbnail = QPixmap(":/abuledufilev1/fond-music");
                    }
                    else if(m_format.startsWith("video/")) {
                        thumbnail = QPixmap(":/abuledufilev1/fond-video");
                    }
                    newInfo.name = "Thumbnails/thumbnail.png";
                    if(fichierDehors.open(QIODevice::WriteOnly,newInfo)) {
                        thumbnail.save(&fichierDehors,"PNG");
                        fichierDehors.close();
                    }
                }
            }
        }

        zipDestination.close();
        retour = true;
    }
    else {
        QWidget* top = 0;
        if(qApp->topLevelWidgets().size() > 0){
            top = qApp->topLevelWidgets().last();
        }
        AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(trUtf8("Erreur de création du fichier d'export !"),
                                                              trUtf8("Erreur d'export, vérifiez bien que vous avez les droits pour écrire ce fichier (%1) et re-essayez.")
                                                              .arg(destFileName),
                                                              true,top);
        msgBox->show();
    }

    //    if (retour == true)
    //    {
    //        emit abeSignalFileOpen(1);
    //    }
    //    else
    //    {
    //        emit abeSignalFileOpen(-1);
    //    }
    return retour;
}

bool AbulEduFileV1::abeFileExportPrepare(const QStringList &fileList, const QString &fileBase, const QString &fileFormat)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "  name : " << objectName()
                        << "  fileBase : " << fileBase
                        << "  m_fileBase : " << m_fileBase
                        << "  fileFormat : " << fileFormat;
    bool retour = false;

    if(fileList.size() > 0 && !fileBase.isEmpty() && !fileFormat.isEmpty()) {
        m_fileList = fileList;
        //Pas de doublons dans l'archive svp
        m_fileList.removeDuplicates();
        m_fileBase = fileBase;
        m_fileformat = fileFormat;
        retour = true;
    }

    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << " retour : " << retour;
    return retour;
}

bool AbulEduFileV1::abeFileExportFile(const QString &destFileName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "  name : " << objectName()
                        << "  fileBase : " << m_fileBase;
    return this->abeFileSave(destFileName,m_fileList,m_fileBase,m_fileformat);
}

//--------------------------------------------------------------------------------------------------------------------
bool AbulEduFileV1::abeFileOpen(const QString &srcFile, const QString &fileFormat)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "  file : " << srcFile;
    //Cas particulier d'un appel sans le répertoire temporaire de destination mais si jamais l'objet contient déjà
    //un fichier extrait dans un répertoire temporaire, on nettoie avant de créer un nouveau point
    //    if(m_isTemporaryDir) {
    if((m_isTemporaryDir && m_directoryTemp.absolutePath() != "") || m_directoryTemp.absolutePath().startsWith(QDir::tempPath())) {

        /// @todo@ a optimiser encore plus pour eviter le bug de nettoyage dans le répertoire perso
        abeCleanDirectoryRecursively(m_directoryTemp.absolutePath());
    }

    //On ne passe pas de repertoire de destination de ce fait on doit en creer un temporaire !
    QString tmp = abeUniqIDTemp();
    QDir *directoryTemp = new QDir(tmp);

    ABULEDU_LOG_DEBUG() << "AbulEduFileV1:abeFileOpen repertoire pas encore existant " << directoryTemp;
    return this->abeFileOpen(srcFile, directoryTemp, fileFormat);
}

bool AbulEduFileV1::abeFileOpen(const QString& srcFile, QDir *dest, const QString &fileFormat)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour = false;
    bool estUnFichierABE = false;

    /* fichier .abe n'a pas de version (avant juin 2013) donc initialisation a 1.0 */
    abeFileSetUnitVersion("1.0");
    m_fileList.clear();

    if(fileFormat == "zip") {
        m_filename = srcFile;
        if(dest->exists()) {
            m_isTemporaryDir = false;        }
        else {
            m_isTemporaryDir = true;
        }

        m_directoryTemp.setPath(dest->absolutePath());
        //bug 03.05.2015 : le repertoire n'existe pas ...
        if(!m_directoryTemp.exists()) {
            m_directoryTemp.mkpath(dest->absolutePath());
        }
        //Permet juste d'extraire le fichier zip srcFile dans le rep dirDest
        //pour tests uniquement
        QuaZip zipSource(srcFile);

        //si on arrive ici c'est que tout est OK et qu'on peut donc décompresser le fichier
        //dans le bon répertoire :)
        ABULEDU_LOG_DEBUG() << "AbulEduFileV1:abeFileOpen(2) mkpath " << m_directoryTemp.absolutePath();
        //Et on repasse le fichier en revue
        QFile out;
        if(zipSource.open(QuaZip::mdUnzip)) {
            zipSource.goToFirstFile();
            for (int i=1; i <= zipSource.getEntriesCount(); i++) {
                QuaZipFile fic(&zipSource);
                if (!fic.open(QIODevice::ReadOnly)){
                    ABULEDU_LOG_DEBUG() << "Erreur de lecture du fichier";
                    //                    emit abeSignalFileOpen(-1);
                    return false;
                }

                QString name=fic.getActualFileName();
                m_fileList << m_directoryTemp.absolutePath() + "/" + name;
                if(fic.getZipError()!=UNZ_OK) {
                    qWarning() << "testRead(): file.getFileName(): " << fic.getZipError();
                    //                    emit abeSignalFileOpen(-1);
                    return false;
                }
                QString dirn = m_directoryTemp.absolutePath() + "/" + name;
                if (name.contains('/')) { // subdirectory support
                    dirn.chop(dirn.length() - dirn.lastIndexOf("/"));
                    QDir().mkpath(dirn);
                }
                out.setFileName(m_directoryTemp.absolutePath() + "/" + name);
                out.open(QIODevice::WriteOnly);
                out.write(fic.read(fic.size()));
                ABULEDU_LOG_DEBUG() << "  On copie " << fic.getActualFileName() << " dans " << out.fileName();
                out.close();
                fic.close();

                //Et c'est ici qu'il faut gérer les fichiers spéciaux
                if(m_reservedFileNames.contains(name)) {
                    /**
                      * @todo: gérer des codes d'erreurs si jamais les fichiers speciaux ne sont pas corrects
                      */
                    fileOpenHandleSpecialFile(name,m_directoryTemp.absolutePath() + "/" + name);
                    estUnFichierABE = true;
                }

                zipSource.goToNextFile();
            }
            zipSource.close();
            retour = true;
        }
    }

    //Si c'est un fichier ABE et que la taille du fichier n'a pas été renseignée dans LOM on peut faire un
    //rattrapage
    if(estUnFichierABE && m_lom->abeLOMgetTechnicalSize() == "") {
        QFileInfo fi(m_filename);
        QString Size;
        qint64 taille = fi.size();

        // taille en Go
        if (taille > 1073741824) {
            Size = QString(" %1 Go").arg((double) taille / 1073741824, 0, 'f', 1);
        }
        // En Mo
        else if (taille > 1048576) {
            Size = QString(" %1 Mo").arg((double) taille / 1048576, 0, 'f', 1);
        }
        // En Ko
        else if (taille > 1024) {
            Size = QString(" %1 Ko").arg(taille / 1024);
        }
        // En octet
        else {
            Size = QString(" %1 octets").arg(taille);
        }
        m_lom->abeLOMsetTechnicalSize(Size);
    }
    //N'a pas a etre géré ici maintenant qu'on retourne un bool
    //    else {
    //        AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(trUtf8("Ouverture de fichier abe"), trUtf8("L'ouverture de fichiers .abe n'est pas encore prise en compte ..."));
    //        msgBox->show();
    //    }
    //    if (retour == true)
    //    {
    //        emit abeSignalFileOpen(1);
    //    }
    //    else
    //    {
    //        emit abeSignalFileOpen(-1);
    //    }
    return retour;
}

void AbulEduFileV1::abeFileSetFilename(const QString &filename)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_filename = filename;
}

bool AbulEduFileV1::abeFileAddMetaDataFromABB(QSharedPointer<AbulEduSCOLOMFrV1> abbFile, const QString &abbFileName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Ajout des metadata
    QDate ladate = abbFile->abeLOMgetLifeCycleContributionDates("author").first();
    if(!ladate.isValid()){
        ladate = QDate::currentDate();
    }
    vCard card = abbFile->abeLOMgetLifeCycleContributionEntities("author",ladate).first();
    m_lom->abeLOMaddLifeCycleContributionRole(trUtf8("fournisseur de contenu"), card, ladate);
    ABULEDU_LOG_DEBUG() << "  on ajoute au LOM le contributeur : " << card.getFullName() << " a la date du " << ladate;
    //Ajout des mots clés
    QString langue = m_lom->abeLOMgetGeneralLanguage().first();
    QStringList kwords = abbFile->abeLOMgetGeneralKeywords(langue);
    for(int i = 0; i < kwords.size(); i++) {
        m_lom->abeLOMaddGeneralKeyword(langue,kwords.at(i));
        ABULEDU_LOG_DEBUG() << "  on ajoute le mot cle : ( " << langue << " )" << kwords.at(i);
    }

    //Et on ajoute aussi le fichier LOM de description de l'image
    QFile *fichierDehors = new QFile(abeFileGetDirectoryTemp().absolutePath()+ "/data/" + abbFileName +".xml",this);
    QByteArray lom = abbFile->abeLOMExportAsXML();
    if(fichierDehors->open(QIODevice::WriteOnly)) {
        fichierDehors->write(lom);
        fichierDehors->close();
    }
    else {
        return false;
    }
    return true;
}

const QString AbulEduFileV1::abeFileGetMD5(const QString& otherFileName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFile fic(otherFileName);
    if (fic.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = fic.readAll();
        fic.close();
        return QString((QCryptographicHash::hash(fileData, QCryptographicHash::Md5)).toHex());
    }
    else
        return QString("");

}

const QString AbulEduFileV1::abeFileGetBase64Encoded()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString retour = "";
    QFile fic(m_filename);
    if (fic.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = fic.readAll();
        retour = fileData.toBase64();
        fic.close();
    }
    return retour;
}

//--------------------------------------------------------------------------------------------------------------------
//Genere automatiquement un bloc XML pour eviter que ca ne soit trop verbeux lahaut
void AbulEduFileV1::domAddElement(const QString &tagName, const QString &value, QDomElement *parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomElement e = m_domDocument.createElement(tagName);
    parent->appendChild(e);
    QDomText t = m_domDocument.createTextNode(value);
    e.appendChild(t);
}

/** ********************************************************************************************************* */

/** Gère l'ouverture des fichiers spéciaux contenu dans un fichier .abe */
void AbulEduFileV1::fileOpenHandleSpecialFile(const QString &fileName, const QString &absoluteFilePath)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(fileName == "abuledu") {
        QFile fi(absoluteFilePath);
        if(fi.exists() && fi.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_originalSoftware = fi.readAll();
        }
    }
    if(fileName == "version") {
        QFile fi(absoluteFilePath);
        if(fi.exists() && fi.open(QIODevice::ReadOnly | QIODevice::Text)) {
            abeFileSetUnitVersion(fi.readAll());
        }
    }
    else if(fileName == "lom.xml") {
        ABULEDU_LOG_DEBUG() << "Fichier lom.xml ...";
        m_lom->abeLOMsetFile(absoluteFilePath);

        //dans le fichier LOM on a l'URI de la ressource sur data.abuledu.org ou mediatheque.abuledu.org ...
        QStringList urls = m_lom->abeLOMgetTechnicalLocation();
        for(int i = 0; i < urls.count(); i++) {
            if(urls.at(i).contains("data.abuledu.org") || urls.at(i).contains("mediatheque.abuledu.org")|| urls.at(i).contains("fichiers.abuledu.org")) {
                m_identifier = urls.at(i);
                break;
            }
        }


        //Toutes les dates
        QList<QDate> lDates = m_lom->abeLOMgetLifeCycleContributionDates("author");
        //Puis le "1er auteur"
        if(!lDates.isEmpty()){
            vCard v = m_lom->abeLOMgetLifeCycleContributionEntities("author",lDates.first()).first();
            m_creator = v.getFullName().toLatin1();
        }
        QString langueLOM;
        if(m_lom->abeLOMgetGeneralLanguage().size() > 0) {
            langueLOM = m_lom->abeLOMgetGeneralLanguage().first();
        }

        m_language = langueLOM;
        m_title = m_lom->abeLOMgetGeneralTitle(langueLOM);
        m_subject = m_lom->abeLOMgetGeneralKeywords(langueLOM).join(",");
        m_description = m_lom->abeLOMgetGeneralDescription(langueLOM).first();
        if(m_lom->abeLOMgetLifeCycleContributionDates("author").isEmpty()){
            m_date = QString();
        }
        else{
            m_date = m_lom->abeLOMgetLifeCycleContributionDates("author").first().toString("yyyy-MM-dd");
        }
        m_keywords = m_lom->abeLOMgetGeneralKeywords(langueLOM).join(",");
    }
//    else if(fileName == "meta.xml") {
//        ABULEDU_LOG_DEBUG() << "Fichier meta.xml ...";
//        m_domDocument.clear();
//        QFile fi(absoluteFilePath);
//        if(fi.exists() && fi.open(QIODevice::ReadOnly | QIODevice::Text)) {
//            m_domDocument.setContent(&fi);
// Et ensuite on initialise toutes les variables -> bug detecte 2 mai 2015 : certains meta.xml ne sont pas mis a jour alors que lom.xml oui
//            m_title       = m_domDocument.documentElement().elementsByTagName("dc:title").at(0).toElement().text();
//            m_creator     = m_domDocument.documentElement().elementsByTagName("dc:creator").at(0).toElement().text();
//            m_date        = m_domDocument.documentElement().elementsByTagName("dc:date").at(0).toElement().text();
//            m_subject     = m_domDocument.documentElement().elementsByTagName("dc:subject").at(0).toElement().text();
//            m_description = m_domDocument.documentElement().elementsByTagName("dc:description").at(0).toElement().text();
//            m_language    = m_domDocument.documentElement().elementsByTagName("dc:language").at(0).toElement().text();
//        }
//    }
}

//--------------------------------------------------------------------------------------------------------------------
QByteArray AbulEduFileV1::makeContentXML()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Et on construit un "content.xml" qui dira "ce fichier n'est pas un véritable fichier odf :)"
    m_domDocument.clear();
    QDomElement officeDocContent = m_domDocument.createElement("office:document-content");
    m_domDocument.appendChild(officeDocContent);

    officeDocContent.setAttribute("xmlns:office","urn:oasis:names:tc:opendocument:xmlns:office:1.0");
    officeDocContent.setAttribute("xmlns:style", "urn:oasis:names:tc:opendocument:xmlns:style:1.0");
    officeDocContent.setAttribute("xmlns:text", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
    officeDocContent.setAttribute("xmlns:table", "urn:oasis:names:tc:opendocument:xmlns:table:1.0");
    officeDocContent.setAttribute("xmlns:draw", "urn:oasis:names:tc:opendocument:xmlns:drawing:1.0");
    officeDocContent.setAttribute("xmlns:fo", "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0");
    officeDocContent.setAttribute("xmlns:number", "urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0");
    officeDocContent.setAttribute("xmlns:svg", "urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0");
    officeDocContent.setAttribute("xmlns:chart", "urn:oasis:names:tc:opendocument:xmlns:chart:1.0");
    officeDocContent.setAttribute("xmlns:dr3d", "urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0");
    officeDocContent.setAttribute("xmlns:math", "http://www.w3.org/1998/Math/MathML");
    officeDocContent.setAttribute("xmlns:form", "urn:oasis:names:tc:opendocument:xmlns:form:1.0");
    officeDocContent.setAttribute("xmlns:script", "urn:oasis:names:tc:opendocument:xmlns:script:1.0");
    officeDocContent.setAttribute("xmlns:ooow", "http://openoffice.org/2004/writer");
    officeDocContent.setAttribute("xmlns:oooc", "http://openoffice.org/2004/calc");
    officeDocContent.setAttribute("xmlns:dom", "http://www.w3.org/2001/xml-events");
    officeDocContent.setAttribute("xmlns:xforms", "http://www.w3.org/2002/xforms");
    officeDocContent.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
    officeDocContent.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    officeDocContent.setAttribute("xmlns:rpt", "http://openoffice.org/2005/report");
    officeDocContent.setAttribute("xmlns:of", "urn:oasis:names:tc:opendocument:xmlns:of:1.2");
    officeDocContent.setAttribute("xmlns:xhtml", "http://www.w3.org/1999/xhtml");
    officeDocContent.setAttribute("xmlns:officeooo", "http://openoffice.org/2009/office");
    officeDocContent.setAttribute("xmlns:tableooo", "http://openoffice.org/2009/table");
    officeDocContent.setAttribute("xmlns:field", "urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0");
    officeDocContent.setAttribute("xmlns:formx", "urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0");
    officeDocContent.setAttribute("xmlns:css3t", "http://www.w3.org/TR/css3-text/");
    officeDocContent.setAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
    officeDocContent.setAttribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
    officeDocContent.setAttribute("xmlns:meta", "urn:oasis:names:tc:opendocument:xmlns:meta:1.0");
    officeDocContent.setAttribute("xmlns:ooo", "http://openoffice.org/2004/office");
    officeDocContent.setAttribute("xmlns:grddl", "http://www.w3.org/2003/g/data-view#");
    officeDocContent.setAttribute("office:version", "1.2");
    officeDocContent.setAttribute("grddl:transformation", "http://docs.oasis-open.org/office/1.2/xslt/odf2rdf.xsl");

    QDomElement officeContentBody = m_domDocument.createElement("office:body");
    officeDocContent.appendChild(officeContentBody);

    QDomElement officeContentText = m_domDocument.createElement("office:text");
    officeContentBody.appendChild(officeContentText);

    QDomElement officeContentTextP = m_domDocument.createElement("text:p");
    //comment opendocumentFormat interprète le style name P1, sachant que la valeur de cet attribut est NCName
    //officeContentTextP.setAttribute("text:style-name","P1");
    officeContentText.appendChild(officeContentTextP);

    QDomElement officeContentTextT = m_domDocument.createElement("text:span");
    //comment opendocumentFormat interprète le style name T1, sachant que la valeur de cet attribut est NCName
    //officeContentTextT.setAttribute("text:style-name","T1");

    QDomText t = m_domDocument.createTextNode("Bonjour, ceci n'est pas un véritable fichier opendocument mais une réutilisation astucieuse de ce super format de fichier ...");
    officeContentTextT.appendChild(t);
    officeContentTextP.appendChild(officeContentTextT);

    QDomElement officeContentTextP1 = m_domDocument.createElement("text:p");
    officeContentTextP1.setAttribute("xhtml:about","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Element");
    officeContentTextP1.setAttribute("xhtml:property","dc:publisher");
    officeContentText.appendChild(officeContentTextP1);
    QDomElement officeContentTextT1 = m_domDocument.createElement("text:span");
    //il faudrait mettre à jour la valeur de cette propriété régulièrement sans avoir à ouvrir le code pour la modifier .
    QDomText t1 = m_domDocument.createTextNode("namePublisher");
    officeContentTextT1.appendChild(t1);
    officeContentTextP1.appendChild(officeContentTextT1);

    QDomElement officeContentTextP2 = m_domDocument.createElement("text:p");
    officeContentTextP2.setAttribute("xhtml:about","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Element");
    officeContentTextP2.setAttribute("xhtml:property","dc:contributor");
    officeContentText.appendChild(officeContentTextP2);
    QDomElement officeContentTextT2 = m_domDocument.createElement("text:span");
    //il faudrait mettre à jour la valeur de cette propriété régulièrement sans avoir à ouvrir le code pour la modifier
    QDomText t2 = m_domDocument.createTextNode("nameContributor");
    officeContentTextT2.appendChild(t2);
    officeContentTextP2.appendChild(officeContentTextT2);

    QDomElement officeContentTextP3 = m_domDocument.createElement("text:p");
    officeContentTextP3.setAttribute("xhtml:about","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Element");
    officeContentTextP3.setAttribute("xhtml:property","dc:identifier");
    officeContentText.appendChild(officeContentTextP3);
    QDomElement officeContentTextT3 = m_domDocument.createElement("text:span");
    //il faudrait mettre à jour la valeur de cette propriété régulièrement sans avoir à ouvrir le code pour la modifier
    QDomText t3 = m_domDocument.createTextNode("nameIdentifier");
    officeContentTextT3.appendChild(t3);
    officeContentTextP3.appendChild(officeContentTextT3);

    QDomElement officeContentTextP4 = m_domDocument.createElement("text:p");
    officeContentTextP4.setAttribute("xhtml:about","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Element");
    officeContentTextP4.setAttribute("xhtml:property","dc:source");
    officeContentText.appendChild(officeContentTextP4);
    QDomElement officeContentTextT4 = m_domDocument.createElement("text:span");
    //il faudrait mettre à jour la valeur de cette propriété régulièrement sans avoir à ouvrir le code pour la modifier
    QDomText t4 = m_domDocument.createTextNode("nameSource");
    officeContentTextT4.appendChild(t4);
    officeContentTextP4.appendChild(officeContentTextT4);

    QDomElement officeContentTextP5 = m_domDocument.createElement("text:p");
    officeContentTextP5.setAttribute("xhtml:about","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Element");
    officeContentTextP5.setAttribute("xhtml:property","dc:relation");
    officeContentText.appendChild(officeContentTextP5);
    QDomElement officeContentTextT5 = m_domDocument.createElement("text:span");
    //il faudrait mettre à jour la valeur de cette propriété régulièrement sans avoir à ouvrir le code pour la modifier
    QDomText t5 = m_domDocument.createTextNode("nameRelation");
    officeContentTextT5.appendChild(t5);
    officeContentTextP5.appendChild(officeContentTextT5);

    QDomElement officeContentTextP6 = m_domDocument.createElement("text:p");
    officeContentTextP6.setAttribute("xhtml:about","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Element");
    officeContentTextP6.setAttribute("xhtml:property","dc:coverage");
    officeContentText.appendChild(officeContentTextP6);
    QDomElement officeContentTextT6 = m_domDocument.createElement("text:span");
    //il faudrait mettre à jour la valeur de cette propriété régulièrement sans avoir à ouvrir le code pour la modifier
    QDomText t6 = m_domDocument.createTextNode("nameCoverage");
    officeContentTextT6.appendChild(t6);
    officeContentTextP6.appendChild(officeContentTextT6);

    QDomElement officeContentTextP7 = m_domDocument.createElement("text:p");
    officeContentTextP7.setAttribute("xhtml:about","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Element");
    officeContentTextP7.setAttribute("xhtml:property","dc:rights");
    officeContentText.appendChild(officeContentTextP7);
    QDomElement officeContentTextT7 = m_domDocument.createElement("text:span");
    //il faudrait mettre à jour la valeur de cette propriété régulièrement sans avoir à ouvrir le code pour la modifier
    QDomText t7 = m_domDocument.createTextNode("nameRights");
    officeContentTextT7.appendChild(t7);
    officeContentTextP7.appendChild(officeContentTextT7);

    QDomNode noeudContent = m_domDocument.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_domDocument.insertBefore(noeudContent,m_domDocument.firstChild());

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

//--------------------------------------------------------------------------------------------------------------------
QByteArray AbulEduFileV1::makeManifestXML(const QStringList &fileList, const QString &fileBaseName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Et on construit un "manifest.xml"
    m_domDocument.clear();
    QDomElement officeDocContent = m_domDocument.createElement("manifest:manifest");
    m_domDocument.appendChild(officeDocContent);

    officeDocContent.setAttribute("xmlns:manifest","urn:oasis:names:tc:opendocument:xmlns:manifest:1.0");

    officeDocContent.setAttribute("manifest:version","1.2");
    QDomElement e = m_domDocument.createElement("manifest:file-entry");
    e.setAttribute("manifest:media-type","application/vnd.oasis.opendocument.text");
    //e.setAttribute("manifest:version","1.2"); sur file-entry cet attribut est optionnel
    e.setAttribute("manifest:full-path","/");
    officeDocContent.appendChild(e);

    e.clear();
    e = m_domDocument.createElement("manifest:file-entry");
    e.setAttribute("manifest:media-type","text/xml");
    e.setAttribute("manifest:full-path","content.xml");
    officeDocContent.appendChild(e);

    e.clear();
    e = m_domDocument.createElement("manifest:file-entry");
    e.setAttribute("manifest:media-type","text/xml");
    e.setAttribute("manifest:full-path","meta.xml");
    officeDocContent.appendChild(e);

    //Ajout des fichiers complementaires
    for(int i = 0; i < fileList.count(); i++) {
        QString leFichier = fileList.at(i);
        QFileInfo fi(leFichier);
        leFichier.remove(fileBaseName);
        //ajout du dossier Images
        if(fi.suffix().toLower() == "png") {
            //            if (m_localDebug) qDebug() << "ajout images";
            e.clear();
            e=m_domDocument.createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type","image/png");
            e.setAttribute("manifest:full-path",leFichier);
            officeDocContent.appendChild(e);
        }
        else if(fi.suffix().toLower() == "jpg" || fi.suffix().toLower() == "jpeg") {
            e.clear();
            e=m_domDocument.createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type","image/jpeg");
            e.setAttribute("manifest:full-path",leFichier);
            officeDocContent.appendChild(e);
        }
        else if(fi.suffix().toLower() == "conf" || fi.suffix().toLower() == "text"
                || fi.suffix().toLower() == "txt" || fi.suffix().toLower() == "xml" ) {
            e.clear();
            e=m_domDocument.createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type","text/xml");
            e.setAttribute("manifest:full-path",leFichier);
            officeDocContent.appendChild(e);
        }
        else if(fi.suffix().toLower() == "wav" || fi.suffix().toLower() == "mp3"
                || fi.suffix().toLower() == "ogg" || fi.suffix().toLower() == "ogv" ) {
            e.clear();
            e=m_domDocument.createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type","application/vnd.sun.star.media");
            e.setAttribute("manifest:full-path",leFichier);
            officeDocContent.appendChild(e);
        }
    }
    QDomNode noeud = m_domDocument.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_domDocument.insertBefore(noeud,m_domDocument.firstChild());

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

//--------------------------------------------------------------------------------------------------------------------
QByteArray AbulEduFileV1::makeManifestRDF()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Et on construit un "manifest.rdf"
    m_domDocument.clear();
    QDomElement officeDocContent = m_domDocument.createElement("rdf:RDF");
    m_domDocument.appendChild(officeDocContent);

    officeDocContent.setAttribute("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");

    QDomElement e = m_domDocument.createElement("rdf:Description");
    //e.setAttribute("xmlns:ns1","http://docs.oasis-open.org/ns/office/1.2/meta/pkg#");
    e.setAttribute("rdf:about","content.xml");
    officeDocContent.appendChild(e);

    QDomElement e_fils = m_domDocument.createElement("rdf:type");
    e_fils.setAttribute("rdf:resource","http://docs.oasis-open.org/ns/office/1.2/meta/odf#ContentFile");
    e.appendChild(e_fils);

    e.clear();
    e_fils.clear();
    e = m_domDocument.createElement("rdf:Description");
    e.setAttribute("rdf:about","");
    officeDocContent.appendChild(e);
    e_fils = m_domDocument.createElement("ns0:hasPart");
    e_fils.setAttribute("xmlns:ns0","http://docs.oasis-open.org/ns/office/1.2/meta/pkg#");
    e_fils.setAttribute("rdf:resource","content.xml");
    e.appendChild(e_fils);

    e.clear();
    e_fils.clear();
    e = m_domDocument.createElement("rdf:Description");
    //e.setAttribute("xmlns:ns1","http://docs.oasis-open.org/ns/office/1.2/meta/pkg#");
    e.setAttribute("rdf:about","");
    officeDocContent.appendChild(e);
    e_fils = m_domDocument.createElement("rdf:type");
    e_fils.setAttribute("rdf:resource","http://docs.oasis-open.org/ns/office/1.2/meta/odf#Document");
    e.appendChild(e_fils);

    QDomNode noeud = m_domDocument.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_domDocument.insertBefore(noeud,m_domDocument.firstChild());

    QByteArray XML  = m_domDocument.toByteArray();
    return XML;
}

const QFileInfo AbulEduFileV1::abeFileGetContent(int num)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString fic;
    int nb = 0;
    for(int i = 0; i < m_fileList.size() && fic.isNull(); i++) {
        /* Si c'est pas un fichier réservé (ex. lom.xml) */
        QString fichier = m_fileList.at(i);
        fichier.remove(m_directoryTemp.absolutePath() + "/");
        if(!m_reservedFileNames.contains(fichier)) {
            /* Si c'est le numéro du fichier demandé */
            if(nb == num) {
                fic = m_fileList.at(i);
            }
            nb++;
        }
    }
    return QFileInfo(fic);
}

const QDir &AbulEduFileV1::abeFileGetDirectoryTemp()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!m_directoryTemp.exists()) {
        m_directoryTemp.mkpath(m_directoryTemp.absolutePath());
    }
    return m_directoryTemp;
}

QStringList AbulEduFileV1::abeFileGetFileList(bool withReservedFileNames)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(withReservedFileNames) {
        return m_fileList;
    }
    //Sinon on supprime les fichiers speciaux
    else {
        QStringList tmp;
        for(int i = 0; i <  m_fileList.count(); i++) {
            QString fic = m_fileList.at(i);
            fic.remove(m_directoryTemp.absolutePath() + "/");
            if(!m_reservedFileNames.contains(fic)) {
                tmp << m_fileList.at(i);
            }
        }
        return tmp;
    }
}

/** ========================================================================================= UPDATEr */

/** MAJ du fichier LOM inclus dans l'archive */
void AbulEduFileV1::abeFileUpdateLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFileInfo fi(m_filename);
    if(fi.exists()) {
        //todo hardcode zip
        this->abeFileOpen(m_filename, "zip");
        //mettre a jour le fichier LOM qui doit se trouver dans
        //this->abeFileGetDirectoryTemp() + "lom.xml"
        QFileInfo filom(m_directoryTemp.absolutePath() + "/lom.xml");

        if(filom.exists()) {
            QFile lom(filom.absoluteFilePath());

            if (!lom.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

            QTextStream out(&lom);
            out << m_lom->abeLOMExportAsXML();
            lom.close();
        }
    }
}

/** Supprime le repertoire temporaire et tout ce qu'il contient (récursif) */
bool AbulEduFileV1::abeCleanDirectory(const QString &dirPath, const QString &dirInitial)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(dirInitial);
    ABULEDU_LOG_WARN() << "  Obsolete function, please use AbulEduFileV1::abeCleanDirectoryRecursively";
    return abeCleanDirectoryRecursively(dirPath);
}

/** Supprime le repertoire dirPath et tout ce qu'il contient (récursif) */
bool AbulEduFileV1::abeCleanDirectoryRecursively(const QString &dirPath, int trashSize)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QStringList localTrash;
    /* Locale car fonction statique ! */
    bool success = true;
    /* not empty -- we must empty it first */
    QDirIterator di(dirPath, QDir::AllEntries | QDir::NoSymLinks | QDir::System| QDir::NoDotAndDotDot);
    while (di.hasNext()) {
        di.next();
        const QFileInfo& fi = di.fileInfo();
        if(fi.absolutePath() == abeApp->getAbeHomepath()) {
            ABULEDU_LOG_WARN()<< "abeCleanDirectoryRecursively() leads to homePath. Probably wrong. Canceled.";
            return false;
        }
        if (fi.isDir()) {
            abeCleanDirectoryRecursively(di.filePath(),localTrash.size()); // recursive
        }
        else {
            if(localTrash.size() > 200 - trashSize) {
                ABULEDU_LOG_WARN()<< "Directory would contain more than 200 files. Probably wrong. Canceled";
                return false;
            }
            else
            {
                localTrash.append(di.filePath());
            }
            ABULEDU_LOG_DEBUG()  << "  rm " <<  di.filePath();
        }
    }

    QStringListIterator localTrashIter(localTrash);
    while(localTrashIter.hasNext()) {
        bool ok2;
        ok2 = QFile::remove(localTrashIter.next());
        if (!ok2) {
            success = false;
        }
    }
    if (success) {
        success = QDir().rmdir(dirPath);
        ABULEDU_LOG_DEBUG()  << " rmdir " <<  dirPath;
    }

    return success;
}

bool AbulEduFileV1::abeClean()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDateTime now = QDateTime::currentDateTime();
    m_language    = "fr-FR";
    m_creator     = "";
    m_date        = now.toString(Qt::ISODate);
    m_subject     = "";
    m_title       = "";
    m_rights      = "";
    m_description = "";
    m_publisher   = "";
    m_contributor = "";
    m_keywords    = "";
    m_type        = "";
    m_format      = "";
    m_identifier  = "";
    m_source      = "";
    m_relation    = "";
    m_coverage    = "";
    m_fileBase    = "";
    m_filename    = "";
    m_fileformat  = "";
    m_originalSoftware = "";

    /** Specificité AbulEdu : AbulEduFile doit être accompagné d'une AbulEduApplication
      * Si ce n'est pas le cas, chaine vide. Necessaire pour les tests unitaires
      */
    if(qApp && qApp->inherits("AbulEduApplicationV1")){
        m_unitVersion = abeApp->getAbeApplicationUnitVersion();
    }
    else{
        m_unitVersion = "";
    }
    //par defaut on n'est pas dans un répertoire temp
    m_isTemporaryDir = false;

    //Mais on initialise quand meme la variable ... pour eviter d'avoir 400 /tmp/qt_temp.XXX
    //je ne créé le répertoire que si c'est nécessaire
    QString directoryTemp = abeUniqIDTemp();
    m_directoryTemp.setPath(directoryTemp);

    m_exportLOM = true;
    m_lom = QSharedPointer<AbulEduSCOLOMFrV1>(new AbulEduSCOLOMFrV1(), &QObject::deleteLater);

    //La liste des noms de fichiers reserves pour eviter d'avoir des doublons
    m_reservedFileNames << "mimetype" << "abuledu" << "meta.xml" << "content.xml"
                        << "META-INF/manifest.xml" << "manifest.rdf" << "lom.xml"
                        << "Thumbnails/thumbnail.png" << "version";

    if(m_directoryTemp.exists() && m_isTemporaryDir) {
        ABULEDU_LOG_DEBUG() << "  on supprime " << m_directoryTemp.absolutePath() << " depuis " << parent() ;
        return abeCleanDirectoryRecursively(m_directoryTemp.absolutePath());
    }
    return true;
}

QStringList AbulEduFileV1::abeFileGetRecursiveContent(const QString dirName)
{
    QStringList result;
    QDir dir(dirName);
    //Attention a ne pas prendre le repertoire "." et ".."
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        if(fileInfo.isDir()) {
            /* C'est ici que le parcours est récursif */
            result << abeFileGetRecursiveContent(fileInfo.absoluteFilePath());
        }
        else {
            result << fileInfo.absoluteFilePath();
        }
    }
    return result;
}

void AbulEduFileV1::abeFileExtractIcon(const QString &source, const QString &destination)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Attention fonction statique donc pas de m_localDebug et autres acces a des variables membres ! */
    AbulEduFileV1 f;
    if(f.abeFileExtract(source, "Thumbnails/thumbnail.png", destination)) {
        return;
    }
    else {
        QWidget* top = 0;
        if(qApp->topLevelWidgets().size() > 0){
            top = qApp->topLevelWidgets().last();
        }
        AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(trUtf8("Fichier ABE non conforme"),
                                                              trUtf8("Votre fichier abe ne respecte pas la norme, il ne contient pas de vignette.\n\nNom du fichier:\n%1").arg(source),
                                                              true,top);
        msgBox->show();
    }
}

QPixmap AbulEduFileV1::abeFileGetIcon()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* On cherche dans le cache */
    QFileInfo fi(m_filename);
    /* Pas super, il faut réfléchir a une mutualisation du code de gestion du cache entre abuledumediatheque et abuledufile */
    if(fi.suffix() == "abb") {
        return QPixmap(abeApp->getAbeHomepath() + "/leterrier/abbbox/.thumbnails/" + fi.fileName().replace(".abb",".png"));
    }
    else if(fi.suffix() == "abf") {
        return QPixmap(abeApp->getAbeHomepath() + "/leterrier/abbbox/.thumbnails/" + fi.fileName().replace(".abf",".png"));
    }
    else if(fi.suffix() == "abe") {
        return QPixmap(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName() + "/.thumbnails/" + fi.fileName().replace(".abe",".png"));
    }
    else {
        ABULEDU_LOG_DEBUG() << "on n'a pas l'icone";
        return QPixmap();
    }
}

bool AbulEduFileV1::abeFileExtract(const QString &source, const QString &fileSearch, const QString &destination)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool retour = false;
    QFileInfo src(source);
    if(src.exists()) {
        QuaZip zipSource(source);
        QFile out;
        if(zipSource.open(QuaZip::mdUnzip)) {
            zipSource.goToFirstFile();
            for (int i=1; (i <= zipSource.getEntriesCount()) && (retour == false); i++) {
                QuaZipFile fic(&zipSource);
                if (!fic.open(QIODevice::ReadOnly)){
                    ABULEDU_LOG_DEBUG() << "Erreur de lecture du fichier";
                    return false;
                }

                QString name=fic.getActualFileName();
                if(fic.getZipError()!=UNZ_OK) {
                    ABULEDU_LOG_WARN() << "testRead(): file.getFileName(): " << fic.getZipError();
                    return false;
                }

                ABULEDU_LOG_DEBUG() << "  On est sur " << name << " et on cherche " << fileSearch;
                if(name == fileSearch) {
                    out.setFileName(destination);
                    if(out.open(QIODevice::WriteOnly)) {
                        out.write(fic.read(fic.size()));
                        ABULEDU_LOG_DEBUG() << "  On copie " << fic.getActualFileName() << " dans " << out.fileName();
                        retour = true;
                        out.close();
                    }
                    fic.close();
                }
                zipSource.goToNextFile();
            }
            zipSource.close();
        }
    }
    return retour;
}

bool AbulEduFileV1::abeFileSetExiv2Tags(const QFileInfo &file, const QString &artist, const QString &copyright, const QString &uniqID)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!file.exists()) return false;

#ifdef __WITHEXIV2__
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open((const char*) (QFile::encodeName(file.absoluteFilePath())));

    if (image.get() != 0)
    {
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();

        bool setArtist       = false;
        bool setCopyright    = false;
        bool setSoftware     = false;
        bool setUniqueID     = false;
        bool setUserComments = false;
        bool setDescription  = false;
        Exiv2::ExifData::const_iterator end = exifData.end();
        //                qDebug() << "============================ DEBUT ===============================";
        for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
        {
            if (i->key() == "Exif.Image.Artist") {
                exifData["Exif.Image.Artist"]    = artist.toStdString();
                setArtist = true;
            }
            if (i->key() == "Exif.Image.Copyright") {
                exifData["Exif.Image.Copyright"]    = copyright.toStdString();
                setCopyright = true;
            }
            if (i->key() == "Exif.Image.Software") {
                exifData["Exif.Image.Software"]  = QString(abeApp->applicationName() + "/" + abeApp->applicationVersion()).toStdString();
                setSoftware = true;
            }
            if(i->key() == "Exif.Photo.ImageUniqueID") {
                exifData["Exif.Photo.ImageUniqueID"]  = uniqID.toStdString();
                setUniqueID = true;
            }
            if(i->key() == "Exif.Photo.UserComment") {
                exifData["Exif.Photo.UserComment"]  = uniqID.toStdString();
                setUserComments = true;
            }
            if(i->key() == "Exif.Image.ImageDescription") {
                exifData["Exif.Image.ImageDescription"]  = uniqID.toStdString();
                setDescription = true;
            }

        }
        /* Si on n'a pas les meta il faut les ajouter */
        /* Mise à jour de l'auteur */
        if(!setArtist) {
            Exiv2::ExifKey k("Exif.Image.Artist");
            Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::asciiString);
            v->read(artist.toStdString());
            exifData.add(k,v.get());
        }

        if(!setCopyright) {
            Exiv2::ExifKey k2("Exif.Image.Copyright");
            Exiv2::Value::AutoPtr v2 = Exiv2::Value::create(Exiv2::asciiString);
            v2->read(copyright.toStdString());
            exifData.add(k2,v2.get());
        }

        if(!setSoftware) {
            Exiv2::ExifKey k3("Exif.Image.Software");
            Exiv2::Value::AutoPtr v3 = Exiv2::Value::create(Exiv2::asciiString);
            v3->read(QString(abeApp->applicationName() + "/" + abeApp->applicationVersion()).toStdString());
            exifData.add(k3,v3.get());
        }

        if(!setUniqueID) {
            Exiv2::ExifKey k4("Exif.Photo.ImageUniqueID");
            Exiv2::Value::AutoPtr v4 = Exiv2::Value::create(Exiv2::asciiString);
            v4->read(uniqID.toStdString());
            exifData.add(k4,v4.get());
        }

        if(!setUserComments) {
            Exiv2::ExifKey k5("Exif.Photo.UserComment");
            Exiv2::Value::AutoPtr v5 = Exiv2::Value::create(Exiv2::asciiString);
            v5->read(uniqID.toStdString());
            exifData.add(k5,v5.get());
        }

        if(!setDescription) {
            Exiv2::ExifKey k6("Exif.Image.ImageDescription");
            Exiv2::Value::AutoPtr v6 = Exiv2::Value::create(Exiv2::asciiString);
            v6->read(uniqID.toStdString());
            exifData.add(k6,v6.get());
        }

        image->setExifData(exifData);
        image->writeMetadata();
        return true;
    }
    return false;
#else
    return false;
#endif
}

QMap<QString, QString> AbulEduFileV1::abeFileGetExiv2Tags(const QFileInfo &file)
{
    QMap<QString, QString> mapRetour;
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(!file.exists()) return mapRetour;

#ifdef __WITHEXIV2__
    try {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open((const char*) (QFile::encodeName(file.absoluteFilePath())));
        if (image.get() != 0)
        {
            image->readMetadata();
            Exiv2::ExifData &exifData = image->exifData();

            Exiv2::ExifData::const_iterator end = exifData.end();
            //                qDebug() << "============================ DEBUT ===============================";
            for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i)
            {
                QString value = i->value().toString().c_str();
                QString key = i->key().c_str();
                mapRetour.insertMulti(key, value);
            }
        }
    }
    catch(std::exception &e) {
        //nothing ?
    }
    return mapRetour;
#else
    return mapRetour;
#endif
}

bool AbulEduFileV1::abeFileCopyExiv2Tags(const QString &src, const QString &dst)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
#ifdef __WITHEXIV2__
    QFileInfo fiSrc(src);
    QFileInfo fiDst(dst);
    if(fiSrc.exists() && fiDst.exists() && AbulEduMimeTypeV1::get(fiSrc.suffix()) == "image/jpeg") {
        Exiv2::Image::AutoPtr imageA = Exiv2::ImageFactory::open((const char*) (QFile::encodeName(src)));
        Exiv2::Image::AutoPtr imageB = Exiv2::ImageFactory::open((const char*) (QFile::encodeName(dst)));
        if (imageA.get() != 0) {
            try {
                imageA->readMetadata();
                Exiv2::ExifData &exifData = imageA->exifData();

                imageB->setExifData(exifData);
                imageB->writeMetadata();
                return true;
            }
            catch(std::exception &e) {
                return false;
            }
        }
    }
    return false;
#else
    return false;
#endif
}

QString AbulEduFileV1::abeFileFormatSize(qint64 num)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString total;
    const qint64 kb = 1024;
    const qint64 mb = 1024 * kb;
    const qint64 gb = 1024 * mb;
    const qint64 tb = 1024 * gb;

    if (num >= tb) total = trUtf8("%1TB").arg(QString::number(qreal(num) / tb, 'f', 2));
    else if(num >= gb) total = trUtf8("%1GB").arg(QString::number(qreal(num) / gb, 'f', 2));
    else if(num >= mb) total = trUtf8("%1MB").arg(QString::number(qreal(num) / mb, 'f', 1));
    else if(num >= kb) total = trUtf8("%1KB").arg(QString::number(qreal(num) / kb,'f', 1));
    else total = trUtf8("%1 bytes").arg(num);

    return total;
}

QString AbulEduFileV1::abeFileDebug()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString retour("  @@@@ " + objectName() + " : " + m_isTemporaryDir + " et " + m_directoryTemp.absolutePath() + " nom du fichier " + m_filename);
    return retour;
}
