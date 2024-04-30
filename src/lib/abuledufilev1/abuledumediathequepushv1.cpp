/** Classe AbulEduMediathequePushV1
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

#include "abuledumediathequepushv1.h"

#ifndef DEBUG_ABULEDUFILEV1
#include <abuledudisableloggerv1.h>
#endif

AbulEduMediathequePushV1::AbulEduMediathequePushV1(QWidget *parent, QString mediathequeOrData) :
    QWidget(parent),
    AbulEduMediathequeCommon(),
    ui(new Ui::AbulEduMediathequePushV1)
{
    //Surtout pas sinon les logiciels "plantent": ils font un new dans le constructeur mais ensuite juste des show ... et nous dans le close
    //on se suicide ... donc le 2° show -> plantage
    //    setAttribute(Qt::WA_DeleteOnClose);

    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //important de l'avoir avant la creation du sharepointer pour avoir le "nom" qui est défini dans le .ui
    ui->setupUi(this);

    m_mimetype = "application/abuledu";
    m_updateFileWithThisURICode    = "";
    m_updateFileLOMWithThisURICode = "";
    //    m_lom = new AbulEduSCOLOMFrV1();
    m_lom = QSharedPointer<AbulEduSCOLOMFrV1>(new AbulEduSCOLOMFrV1(), &QObject::deleteLater);
    m_abuledufile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1(this), &QObject::deleteLater);
    m_abuledufile->setObjectName("depuis abuledumediathequepush");

    m_nam = abeApp->getAbeNetworkAccessManager();
    //Surtout pas modale sinon la popup sso ne peut pas se placer au dessus !
    //    setWindowModality(Qt::ApplicationModal);

    //Pour pousser une ressource sur data on n'a surtout pas besoin de compléter les 2 premieres pages !
    if(mediathequeOrData == "data") {
        m_rpc = new MaiaXmlRpcClient(this);
        abeSetSourceEnum(AbulEduMediathequePushV1::abeData);
    }
    else if(mediathequeOrData == "fichiers") {
        m_rpc = new MaiaXmlRpcClient(this);
        abeSetSourceEnum(AbulEduMediathequePushV1::abeFichiers);
    }
    else {
        m_rpc = new MaiaXmlRpcClient(this);
        abeSetSourceEnum(AbulEduMediathequePushV1::abeMediatheque);
        slotUpdateInterfaceFromLom();
    }

    /// Creation du menu contextuel "supprimer" le mot cle
    QStyle* style =  QApplication::style(); // récupération du style systeme
    /// MENU LISTWIDGET (Supprimer)
    m_menuAbulEduMediathequePushlwMotsCles = new QMenu(ui->AbulEduMediathequePushlwMotsCles);
    QIcon iconSupprimer = style->standardIcon(QStyle::SP_DialogResetButton); //On récupère l'icône désiré
    QAction *a_supprimer = new QAction(trUtf8("&Supprimer de la selection"),m_menuAbulEduMediathequePushlwMotsCles);
    a_supprimer->setIcon(iconSupprimer);
    a_supprimer->setIconVisibleInMenu(true);
    a_supprimer->connect(a_supprimer, SIGNAL(triggered()), this, SLOT(slotDeleteItemKeywords()), Qt::UniqueConnection);
    m_menuAbulEduMediathequePushlwMotsCles->addAction(a_supprimer);

    //La gestion des mots cles
    connect(ui->AbulEduMediathequePushleMotCle, SIGNAL(abeSignalKeywordsDownloaded()), this, SLOT(slotKeywordsDownloaded()),Qt::UniqueConnection);
    QShortcut *AbulEduMediathequePushShortcutMotsCles = new QShortcut(QKeySequence(Qt::Key_Delete), ui->AbulEduMediathequePushlwMotsCles, 0, 0, Qt::WidgetShortcut);
    connect(AbulEduMediathequePushShortcutMotsCles, SIGNAL(activated()), this, SLOT(slotDeleteItemKeywords()),Qt::UniqueConnection);

    /* Lecture des fichiers XML de ScoLOM et remplissage du treeWidget qui permet de les choisir */
    QDir ressource(":/xml");
    QDomDocument domDocument;
    QByteArray xml;
    foreach (QString fileName, ressource.entryList()) {
        QFile fi(":/xml/" + fileName);
        if(fi.exists() && fi.open(QIODevice::ReadOnly | QIODevice::Text)) {
            fi.seek(0);
            //            xml = fi.readAll();
            //            domDocument.setContent(xml);
            fi.close();
            m_fichiersXML.insert(fileName,fi.fileName());
        }
        //        if (m_localDebug) ABULEDU_LOG_DEBUG()<<m_fichiersXML;
    }


    //Ajout de la racine à partir du fichier XML 28 (objectifs)

    ui->twLOM9_1->clear();
    QTreeWidgetItem *racineObjectif = new QTreeWidgetItem();
    QFile fi(m_fichiersXML.value("objectif"));
    racineObjectif->setText(0,trUtf8("Objectif de classification"));
    ui->twLOM9_1->invisibleRootItem()->addChild(racineObjectif);
    //    racineObjectif->setHidden(true);
    if(fi.exists() && fi.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fi.seek(0);
        QByteArray xml = fi.readAll();
        domDocument.clear();
        domDocument.setContent(xml);
        QDomElement docElem = domDocument.documentElement();
        examineChildElementsBis(docElem, racineObjectif);
        fi.close();
    }
    m_fichiersXML.remove("objectif");


    //parcours des autres fichiers XML et recherche de l'endroit ou il faut les accrocher
    QList<QTreeWidgetItem *> listeTrouves;
    QMapIterator<QString,QString> iter(m_fichiersXML);
    while(iter.hasNext())
    {
        iter.next();
        QTreeWidgetItem *item0 = new QTreeWidgetItem();
        item0->setText(0,iter.key());
        QFont gras(item0->font(0).family(),item0->font(0).pointSize(),QFont::Bold,false);
        item0->setFont(0,gras);
        //        ui->twLOM9_1->invisibleRootItem()->addChild(item0);
        //        ABULEDU_LOG_DEBUG() << "On cherche " << iter.key() << " dans l'arbre ...";
        QList<QTreeWidgetItem *> listeTemp = ui->twLOM9_1->findItems(iter.key(),Qt::MatchRecursive);
        if(listeTemp.size() > 0) {
            //            ABULEDU_LOG_DEBUG() << "  trouve";
            QTreeWidgetItem *colleIci = listeTemp.first();
            item0->setData(0,Qt::WhatsThisRole,colleIci->data(0,Qt::WhatsThisRole).toString());
            //            ABULEDU_LOG_DEBUG()<<" /*/*/*/*/*/*/*/* ";
            //            ABULEDU_LOG_DEBUG()<<"lecture maintenant de it : "<<item0->data(0,Qt::WhatsThisRole).toString();
            //            ABULEDU_LOG_DEBUG()<<" /*/*/*/*/*/*/*/* ";
            listeTrouves.append(colleIci);
            colleIci->parent()->addChild(item0);
            colleIci->parent()->removeChild(colleIci);
            ui->twLOM9_1->removeItemWidget(colleIci,0);
            ui->twLOM9_1->update();
        }

        QFile fi(iter.value());
        if(fi.exists() && fi.open(QIODevice::ReadOnly | QIODevice::Text)) {
            fi.seek(0);

            QByteArray xml = fi.readAll();
            domDocument.clear();
            domDocument.setContent(xml);
            QDomElement docElem = domDocument.documentElement();
            examineChildElements(docElem, item0);
            fi.close();
        }
    }
    ui->twLOM9_1->expandAll();
    m_previousStackedWidgetPage = ui->widgetAbulEduMediathequePushPage1;
    connect(this, SIGNAL(signalMediathequePushFileOver(bool)),ui->frmAbulEduMediathequePushPrecedentSuivant, SLOT(setEnabled(bool)),Qt::UniqueConnection);
}

AbulEduMediathequePushV1::~AbulEduMediathequePushV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
    m_lom.clear();
    delete m_rpc;
    delete m_menuAbulEduMediathequePushlwMotsCles;
    m_abuledufile.clear();
}

void AbulEduMediathequePushV1::on_btnAbulEduMediathequePushUpload_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //ui->btnAbulEduMediathequePushUpload->setEnabled(false);
    //On ne peut plus revenir au précédent/suivant
    ui->frmAbulEduMediathequePushPrecedentSuivant->setEnabled(false);

    //On vide les "logs"
    ui->teAbulEduMediathequePushLog->clear();

    //Dans le cas particulier de la mediatheque on met a jour le fichier lom a partir de toutes les
    //données contenues dans les deux premieres pages
    if(m_mediathequeOrData == AbulEduMediathequePushV1::abeMediatheque) {
        slotUpdateLomFromInterface();
    }

    //En definitive on a 3 cas de figure possible:
    //1. Expédition de fichier
    //1.a sur DATA
    //1.b sur MEDIATHEQUE
    //2. Mise à jour de fichier
    //2.a sur DATA
    //2.b sur MEDIATHEQUE
    //3. Mise à jour de descripteur lom
    //3.a sur DATA
    //3.b sur MEDIATHEQUE

    //Si on n'est pas en mise à jour de LOM (c'est qu'on envoie le fichier "tout court")
    if(m_updateFileLOMWithThisURICode == "") {

        //On n'a pas d'uri pour ce fichier
        if(m_updateFileWithThisURICode == "") {

            //Cas 1. expédition de fichier
            if(m_mediathequeOrData == AbulEduMediathequePushV1::abeData) {
                ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("1. On demande une URI a data.abuledu.org"));
            }
            else if(m_mediathequeOrData == AbulEduMediathequePushV1::abeFichiers) {
                ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("1. On demande une URI a fichiers.abuledu.org"));
            }
            else {
                ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("1. On demande une URI a mediatheque.abuledu.org"));
            }

            //1. On demande une URI au serveur
            QList<QVariant> params = QList<QVariant>() << "";
            m_rpc->call("abe.getNewURI", params,
                        this, SLOT(rpcGetNewURI(QVariant&, QNetworkReply*)),
                        this, SLOT(rpcFault(int, const QString &)));
            QApplication::setOverrideCursor(Qt::WaitCursor);
        }
        else {
            //Cas 2. on met à jour

            //On envoie une commande speciale qui n'existe pas encore et qui permettra d'actualiser une ressource
            uploadeFile(m_updateFileWithThisURICode);
        }
    }
    else {
        //Cas 3. on met à jour LOM
        uploadeLOM();
    }
    QApplication::processEvents();
    //2. On affecte cette URI au fichier LOM abe -> c'est dans rpcGetNewURI
    //3. On créé le fichier abe
    //4. On uploade le fichier
}


void AbulEduMediathequePushV1::rpcGetNewURI(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "============================="
                        << v
                        << "=============================";

    if(m_mediathequeOrData == AbulEduMediathequePushV1::abeData) {
        ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("2. Réponse de data à la demande d'URI"));
    }
    else if(m_mediathequeOrData == AbulEduMediathequePushV1::abeFichiers) {
        ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("2. Réponse du service fichiers à la demande d'URI"));
    }
    else {
        ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("2. Réponse de la médiathèque à la demande d'URI"));
    }
    if(v.toMap().value("URI").toString().trimmed() != "0") {
        QApplication::restoreOverrideCursor();

        //3. On met à jour le fichier abe pour avoir l'URI
        QString identifiant;
        // est reutilise plus bas pour le nom du fichier
        QString URICode = v.toMap().value("URI").toString().trimmed();
        if(m_mediathequeOrData == AbulEduMediathequePushV1::abeData) {
            identifiant = "http://data.abuledu.org/URI/"+URICode;
        }
        else if(m_mediathequeOrData == AbulEduMediathequePushV1::abeFichiers) {
            identifiant = "http://fichiers.abuledu.org/URI/"+URICode;
        }
        else {
            identifiant = "http://mediatheque.abuledu.org/URI/"+URICode;
        }

        //Sluggify transposé en c++
        QString titleSlug = sluggify(m_abuledufile->abeFileGetLOM()->abeLOMgetGeneralTitle("fre").toLower());

        identifiant.append("-" + titleSlug);
        m_abuledufile->abeFileSetIdentifier(identifiant);

        //3.bis on met à jour le LOM (URI)
        m_lom = m_abuledufile->abeFileGetLOM();
        m_lom->abeLOMsetGeneralIdentifierCatalog("URI"); //Juste URI
        m_lom->abeLOMsetGeneralIdentifierEntry(identifiant); //Et là c'est http://data.abuledu.org/URI/xxxxx

        //Mise a jour du fichier (meta.xml)
        m_abuledufile->abeFileSetSource(identifiant);

        ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("3. Affectation de l'URI suivante pour notre ressource: ") + identifiant);
        ABULEDU_LOG_DEBUG() << "Mise à jour :" << m_abuledufile->abeFileGetIdentifier();
        uploadeFile(URICode);
    }
}

void AbulEduMediathequePushV1::uploadeFile(QString URICode)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_mediathequeOrData == AbulEduMediathequePushV1::abeData) {
        //Mise à jour des tags EXIF si on a des images dans l'archive
        QStringList fics = m_abuledufile->abeFileGetFileList();
        for(int i = 0; i< fics.size(); i++) {
            QFileInfo fi(fics.at(i));
            if(AbulEduMimeTypeV1::get(fi.suffix()) == "image/jpeg") {
                AbulEduFileV1::abeFileSetExiv2Tags(QFileInfo(fics.at(i)),
                                                   m_abuledufile->abeFileGetCreator(),
                                                   m_abuledufile->abeFileGetRights(),
                                                   m_abuledufile->abeFileGetIdentifier().split("-").first());
            }
        }
    }
    else {
        //Quand on expedie sur mediatheque il faut savoir quel est le logiciel auteur pour creer l'association de mot cle
        m_abuledufile->abeFileGetLOM()->abeLOMaddGeneralKeyword("fre",abeApp->applicationName());
    }

    if (!m_abuledufile->abeFileGetLOM()->abeLOMgetLifeCycleContributionRoles().contains("initiator"))
    {
        m_abuledufile->abeFileGetLOM()->abeLOMaddLifeCycleContributionRole("initiator", abeApp->getAbeIdentite()->abeGetVCard(), QDate::currentDate());
    }
    //on sauvegarde le module sous un nom de fichier temporaire "jetable"
    // -> probleme avec data qui aurait plus besoins que ça se sauvegarde sous le nom reel du fichier
    // mais dans un repertoire temporaire
    if(m_mediathequeOrData == AbulEduMediathequePushV1::abeData) {
        m_abuledufile->abeFileSave(QDir::tempPath() + "/" + m_abuledufile->abeFileGetFileName().fileName(),
                                   m_abuledufile->abeFileGetFileList(true),
                                   m_abuledufile->abeFileGetDirectoryTemp().absolutePath(),
                                   "abb");
    }
    else if(m_mediathequeOrData == AbulEduMediathequePushV1::abeFichiers) {
        m_abuledufile->abeFileSave(QDir::tempPath() + "/" + m_abuledufile->abeFileGetFileName().fileName(),
                                   m_abuledufile->abeFileGetFileList(true),
                                   m_abuledufile->abeFileGetDirectoryTemp().absolutePath(),
                                   "abf");
    }
    else {
        m_abuledufile->abeFileSave(QDir::tempPath() + "/" + m_abuledufile->abeFileGetFileName().fileName(),
                                   m_abuledufile->abeFileGetFileList(true),
                                   m_abuledufile->abeFileGetDirectoryTemp().absolutePath(),
                                   "abe");
    }

    //Vérification pour éviter d'envoyer des fichiers "vides" (< 6 Ko c'est qu'il y a un soucis)
    QFileInfo fAbeI(m_abuledufile->abeFileGetFileName().absoluteFilePath());
    if(fAbeI.size() < 1000) {
        QWidget* top = 0;
        if(qApp->topLevelWidgets().size() > 0){
            top = qApp->topLevelWidgets().last();
        }
        AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(trUtf8("Erreur de fichier"), trUtf8("Le fichier semble incomplet (taille totale de %1 octets)").arg(fAbeI.size()),true,top);
        msgBox->show();
        return;
    }
    //Eric pour debug
    //        AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(trUtf8("Fichier ok"), trUtf8("Fichier OK mais on evite de pourrir data (%1 octets)").arg(fAbeI.size()));
    //        msgBox->show();
    //        return;

    ABULEDU_LOG_DEBUG() << m_abuledufile->abeFileGetLOM()->abeLOMExportAsXML()
                        << "AbulEduMediathequePushV1::uploadeFile sauvegarde du fichier : " << m_abuledufile->abeFileGetFileName().absoluteFilePath();

    /*    Décommenter le "return" ci-dessous pour que l'upload sur la médiathèque ne se fasse pas vraiment
      ça permet de faire de vrai tests sans polluer la médiathèque */
    //    return;

    if(m_updateFileWithThisURICode == "") {
        if(m_mediathequeOrData == AbulEduMediathequePushV1::abeData) {
            ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("4. Sauvegarde du fichier .abb avec cette URI dans la partie LOM."));
        }
        else if(m_mediathequeOrData == AbulEduMediathequePushV1::abeFichiers) {
            ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("4. Sauvegarde du fichier .abf avec cette URI dans la partie LOM."));
        }
        else {
            ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("4. Sauvegarde du fichier .abe avec cette URI dans la partie LOM."));
        }

    }
    else {
        ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("Sauvegarde du fichier avant upload."));
    }

    //Eric pour debug
    //        AbulEduMessageBoxV1* msgBox = new AbulEduMessageBoxV1(trUtf8("Fichier ok"), trUtf8("Fichier OK mais on evite de pourrir data (%1)").arg(fAbeI.absoluteFilePath()));
    //        msgBox->show();
    //        return;

    //On uploade le fichier
    ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("Upload du fichier en cours ..."));
    //_ 'DOCUMENT_TITRE', 'DOCUMENT_DESCRIPTIF', 'CONTRIB_NOM', 'CONTRIB_PRENOM', 'CONTRIB_EMAIL', 'DOCUMENT_KEYWORD','LIST_MATIERES',
    //'LIST_NIVEAUX', 'DOCUMENT_FICHIER_NOM', (base64encoded) 'DOCUMENT_FICHIER_CONTENT', 'DOCUMENT_MIME', 'DOCUMENT_MD5', 'LIST_COMPETENCES'
    QVariantMap tableau;
    QString titre = m_abuledufile->abeFileGetLOM()->abeLOMgetGeneralTitle("fre").trimmed();
    tableau["DOCUMENT_TITRE"]  = titre;

    //Attention c'est une qstringlist maintenant
    if(m_abuledufile->abeFileGetLOM()->abeLOMgetGeneralDescription("fre").count() > 0) {
        tableau["DOCUMENT_DESCRIPTIF"]  = m_abuledufile->abeFileGetLOM()->abeLOMgetGeneralDescription("fre").at(0);
    }
    else {
        tableau["DOCUMENT_DESCRIPTIF"]  = "";
    }

    tableau["document_type"]  = m_abuledufile->abeFileGetType();
    QString keywords = m_abuledufile->abeFileGetLOM()->abeLOMgetGeneralKeywords("fre").join(",");
    tableau["DOCUMENT_KEYWORD"] = keywords;
    /**
              * @todo
              */
    tableau["LIST_MATIERES"]  = "test";
    tableau["LIST_NIVEAUX"]  = "test";
    tableau["LIST_COMPETENCES"]  = "test";

    //Si on a déjà un code uri c'est que le fichier est déjà bien nommé, on le passe un coup au slug histoire de
    //rattraper les noms de fichiers exotiques qu'on a laissé passer au début mais c'est tout
    if(m_updateFileWithThisURICode != "") {
        tableau["DOCUMENT_URI"] = m_updateFileWithThisURICode;
        tableau["DOCUMENT_FICHIER_NOM"]  = sluggify(m_abuledufile->abeFileGetFileName().completeBaseName()) + "." + m_abuledufile->abeFileGetFileName().suffix();
    }
    else {
        //Pour eviter d'avoir 20 fichiers qui portent le meme nom on ajoute le hash URI
        //toto.abe -> toto-hashcode.abe et on nettoie au passage pour eviter les noms de fichiers
        //impossibles :)
        QString nomFichierLocal = m_abuledufile->abeFileGetFileName().completeBaseName().toLower().trimmed();

        //Si jamais on n'a pas de nom de fichier (cf imageo)
        if(nomFichierLocal.isEmpty()) {
            nomFichierLocal = m_abuledufile->abeFileGetTitle();
            //Si jamais le fichier n'a pas de titre on essaye d'aller dans LOM
            if(nomFichierLocal.isEmpty()) {
                nomFichierLocal = m_abuledufile->abeFileGetLOM()->abeLOMgetGeneralTitle("fre");
            }
        }
        QString filenameUnique = sluggify(nomFichierLocal) + "-" + URICode + "." + m_abuledufile->abeFileGetFileName().suffix();
        tableau["DOCUMENT_FICHIER_NOM"]  = filenameUnique;
    }

    tableau["DOCUMENT_MIME"]  = m_mimetype;
    tableau["DOCUMENT_MD5"]  = m_abuledufile->abeFileGetMD5();

    ABULEDU_LOG_DEBUG() << "Expedition des donnees suivantes : " << tableau;

    tableau["DOCUMENT_FICHIER_CONTENT"]  = m_abuledufile->abeFileGetBase64Encoded();
    QList<QVariant> params = QList<QVariant>() << tableau;

    //Evite les multi upload
    ui->btnAbulEduMediathequePushUpload->setEnabled(false);

    //On active la barre de progression mais attention c'est une upload bar
    m_rpc->activateProgressBar(true, true);

    //envoi normal
    if(m_updateFileWithThisURICode == "") {
        ABULEDU_LOG_DEBUG() << "abe.setFile";
        m_rpc->call("abe.setFile", params,
                    this, SLOT(rpcSetFile(QVariant&, QNetworkReply*)),
                    this, SLOT(rpcFault(int, const QString &)));
    }
    //ou mise à jour de la ressource
    else {
        ABULEDU_LOG_DEBUG() << "abe.updateFile";
        //spécial mise à jour
        m_rpc->call("abe.updateFile", params,
                    this, SLOT(rpcSetFile(QVariant&, QNetworkReply*)),
                    this, SLOT(rpcFault(int, const QString &)));
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequePushV1::uploadeLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //On uploade le fichier
    ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("Upload du fichier LOM en cours ..."));
    QVariantMap tableau;

    /** @todo */
    tableau["document_type"]  = m_lom->property("document_type").toString();

    /** @todo */
    tableau["LIST_MATIERES"]  = "test";
    tableau["LIST_NIVEAUX"]  = "test";
    tableau["LIST_COMPETENCES"]  = "test";

    //On a forcément un code uri
    tableau["DOCUMENT_URI"] = m_updateFileLOMWithThisURICode;

    ABULEDU_LOG_DEBUG() << "Expedition des donnees suivantes : " << tableau;

    QByteArray fileData = m_lom->abeLOMExportAsXML();
    tableau["LOM_FICHIER_CONTENT"]  = QString(fileData.toBase64());

    QList<QVariant> params = QList<QVariant>() << tableau;

    //Evite les multi upload
    ui->btnAbulEduMediathequePushUpload->setEnabled(false);

    //On active la barre de progression mais attention c'est une upload bar
    m_rpc->activateProgressBar(true, true);

    //envoi normal
    m_rpc->call("abe.updateFileLOM", params,
                this, SLOT(rpcSetFile(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequePushV1::rpcSetFile(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);

    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "============================="
                        << v
                        << "=============================";
    QApplication::restoreOverrideCursor();
    //On desactive la barre de progression
    m_rpc->activateProgressBar(false);

    QString codeRetour = v.toMap().value("status").toString();
    ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("5. Réponse du serveur à la demande d'upload"));
    ui->teAbulEduMediathequePushLog->appendPlainText(codeRetour);
    ui->teAbulEduMediathequePushLog->appendPlainText(v.toMap().value("SendOK").toString());
    QString message = QString::fromUtf8(v.toMap().value("Message").toByteArray().constData());
    ui->teAbulEduMediathequePushLog->appendPlainText(message);

    int code = 0;
    if(codeRetour.startsWith("2")) {
        code = 1;
    }
    else if(codeRetour.startsWith("4") || codeRetour.startsWith("5")) {
        code = -1;
    }

    emit signalMediathequePushFileUploaded(code);
    emit signalMediathequePushFileOver(true);

}

void AbulEduMediathequePushV1::abeSetFile(QSharedPointer <AbulEduFileV1> file)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << file->abeFileGetDirectoryTemp()
                        << file->abeFileGetFileList(true);
    m_abuledufile = file;
    m_lom = m_abuledufile->abeFileGetLOM();
    slotUpdateInterfaceFromLom();
    //    ui->Lom->abeWLOMsetABELOMFrV1(m_lom);
    //    ui->LomSuite->abeWLOMsetABELOMFrV1(m_lom);
    //    ui->LomFR->abeWLOMsetABELOMFrV1(m_lom);
    //    ui->ScolomFR->abeWLOMsetABESCOLOMFrV1(m_lom);

    //Nettoyage de l'affichage et activation des boutons
    ui->teAbulEduMediathequePushLog->clear();
    ui->btnAbulEduMediathequePushUpload->setEnabled(true);
}

void AbulEduMediathequePushV1::abeSetLOM(QSharedPointer<AbulEduSCOLOMFrV1> lom)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_lom = lom;
    slotUpdateInterfaceFromLom();

    //    ui->Lom->abeWLOMsetABELOMFrV1(m_lom);
    //    ui->LomSuite->abeWLOMsetABELOMFrV1(m_lom);
    //    ui->LomFR->abeWLOMsetABELOMFrV1(m_lom);
    //    ui->ScolomFR->abeWLOMsetABESCOLOMFrV1(m_lom);
}

void AbulEduMediathequePushV1::on_btnAbulEduMediathequePushClose_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    close();
}

void AbulEduMediathequePushV1::rpcFault(int code, const QString &errorMessage)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << code << errorMessage;
    QApplication::restoreOverrideCursor();
    m_rpc->activateProgressBar(false);

    //Tentative de réouverture d'une session SSO en cas de timeout
    if(code == -32300) {
        m_nam->abeSSOLogin();
    }

    ui->btnAbulEduMediathequePushUpload->setEnabled(true);
}

void AbulEduMediathequePushV1::gestionBtnSuivantPrecedent(int mouvement)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //On se déplace cf le mouvement ...
    /* on teste un minimum qu'on ne nous expedie pas trop loin ... */
    if(ui->stackedWidgetAbulEduMediathequePush->currentIndex()+mouvement <= ui->stackedWidgetAbulEduMediathequePush->count()
            && ui->stackedWidgetAbulEduMediathequePush->currentIndex()+mouvement >= 0) {

        ui->stackedWidgetAbulEduMediathequePush->setCurrentIndex(ui->stackedWidgetAbulEduMediathequePush->currentIndex()+mouvement);

        if(ui->stackedWidgetAbulEduMediathequePush->currentIndex() == 0) {
            ui->btnAbulEduMediathequePushPrecedent->setEnabled(false);
        }
        else {
            ui->btnAbulEduMediathequePushPrecedent->setEnabled(true);
        }

        if(ui->stackedWidgetAbulEduMediathequePush->currentIndex() == ui->stackedWidgetAbulEduMediathequePush->count()-1) {
            ui->btnAbulEduMediathequePushSuivant->setEnabled(false);
        }
        else {
            ui->btnAbulEduMediathequePushSuivant->setEnabled(true);
        }
    }
}

QString AbulEduMediathequePushV1::sluggify(QString str)
{
    ABULEDU_LOG_WARN()  <<  " IS DEPRECATED please use AbulEduApplicationV1::sluggify(QString str)";
    return abeApp->sluggify(str);
}

void AbulEduMediathequePushV1::abeSetMimeType(QString mimetype)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_mimetype = mimetype;
}

void AbulEduMediathequePushV1::abeSetSource(QString mediathequeOrData)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(mediathequeOrData == "data") {
        abeSetSourceEnum(AbulEduMediathequePushV1::abeData);
    }
    else if(mediathequeOrData == "fichiers") {
        abeSetSourceEnum(AbulEduMediathequePushV1::abeFichiers);
    }
    else {
        abeSetSourceEnum(AbulEduMediathequePushV1::abeMediatheque);
    }
}

void AbulEduMediathequePushV1::abeSetSourceEnum(enumMediathequeOrData mediathequeOrData)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_mediathequeOrData = mediathequeOrData;
    if(mediathequeOrData == AbulEduMediathequePushV1::abeData) {
        ABULEDU_LOG_TRACE() << "  enumData";
        m_rpc->setUrl(QUrl("https://data.abuledu.org/Xupload.php"));
        //La page par défaut à afficher
        ui->stackedWidgetAbulEduMediathequePush->setCurrentWidget(ui->widgetAbulEduMediathequePushPage3);
        //Et on masque les boutons precedent/suivant qui n'ont rien a faire la pour une ressource brute
        ui->frmAbulEduMediathequePushPrecedentSuivant->hide();
    }
    else if(mediathequeOrData == AbulEduMediathequePushV1::abeFichiers) {
        ABULEDU_LOG_TRACE() << "  enumFichiers";
        m_rpc->setUrl(QUrl("https://fichiers.abuledu.org/Xupload.php"));
        //La page par défaut à afficher
        ui->stackedWidgetAbulEduMediathequePush->setCurrentWidget(ui->widgetAbulEduMediathequePushPage1);
        //Et l'actualisation des boutons
        gestionBtnSuivantPrecedent(0);
    }
    else {
        ABULEDU_LOG_TRACE() << "  enumElse";
        m_rpc->setUrl(QUrl("https://mediatheque.abuledu.org/Xupload.php"));
        //La page par défaut à afficher
        ui->stackedWidgetAbulEduMediathequePush->setCurrentWidget(ui->widgetAbulEduMediathequePushPage1);
        //Et l'actualisation des boutons
        gestionBtnSuivantPrecedent(0);
    }
}

AbulEduMediathequePushV1::enumMediathequeOrData AbulEduMediathequePushV1::abeGetSourceEnum() const
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_mediathequeOrData;
}

void AbulEduMediathequePushV1::abeSetUpdateURI(QString uriCode)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << uriCode;
    m_updateFileWithThisURICode = uriCode;
}

void AbulEduMediathequePushV1::abeSetUpdateURIforLOMupdate(QString uriCode)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< uriCode;
    m_updateFileLOMWithThisURICode = uriCode;
    ui->btnAbulEduMediathequePushUpload->setEnabled(true);
}

void AbulEduMediathequePushV1::abeFastModerationAccept(QString uriCode)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << uriCode;
    QVariantMap tableau;
    if (!m_lom->abeLOMgetLifeCycleContributionRoles().contains("terminator"))
    {
        m_lom->abeLOMaddLifeCycleContributionRole("terminator", abeApp->getAbeIdentite()->abeGetVCard(), QDate::currentDate());
    }
    tableau["DOCUMENT_URI"] = uriCode;
    QByteArray fileData = m_lom->abeLOMExportAsXML();
    tableau["LOM_FICHIER_CONTENT"]  = QString(fileData.toBase64());

    ABULEDU_LOG_DEBUG() << "Expedition des donnees suivantes : " << tableau;

    QList<QVariant> params = QList<QVariant>() << tableau;

    //Evite les multi upload
    ui->btnAbulEduMediathequePushUpload->setEnabled(false);

    //On active la barre de progression mais attention c'est une upload bar
    m_rpc->activateProgressBar(true, true);

    //envoi normal
    m_rpc->call("abe.moderationAccept", params,
                this, SLOT(rpcSetFile(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequePushV1::abeFastModerationDeny(QString uriCode, QString motif)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << uriCode << motif;
    QVariantMap tableau;
    tableau["DOCUMENT_URI"] = uriCode;
    tableau["MODERATION_MOTIF"] = motif;

    ABULEDU_LOG_DEBUG() << "Expedition des donnees suivantes : " << tableau;

    QList<QVariant> params = QList<QVariant>() << tableau;

    //Evite les multi upload
    ui->btnAbulEduMediathequePushUpload->setEnabled(false);

    //On active la barre de progression mais attention c'est une upload bar
    m_rpc->activateProgressBar(true, true);

    //envoi normal
    m_rpc->call("abe.moderationDeny", params,
                this, SLOT(rpcSetFile(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequePushV1::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    ui->teAbulEduMediathequePushLog->appendPlainText(trUtf8("0. authentification SSO"));
    if(m_nam->abeSSOAuthenticationStatus() != 1) {
        m_nam->abeSSOLogin();
    }
    //Si on a un parent, on vire le bouton "fermer la fenetre"
    if(parent() != 0) {
        ui->btnAbulEduMediathequePushClose->hide();
    }
}

void AbulEduMediathequePushV1::slotAbeMediathequePushClean()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->cbLOMLang->setCurrentIndex(0);
    ui->leLOMFR1_2->clear();
    ui->teSCOLOMFR1_4->clear();
    ui->AbulEduMediathequePushlwMotsCles->clear();
    ui->cbLOM1_10->setCurrentIndex(0);
    ui->lwLOM1_10->clear();
    ui->lwScoLOMFR2_3->clear();
    ui->cbLOM5_2->setCurrentIndex(0);
    ui->lwLOM5_2->clear();
    ui->cbLOM5_6->setCurrentIndex(0);
    ui->lwLOM5_6->clear();
    ui->teLOM5_10->clear();
    ui->cbLOM5_12->setCurrentIndex(0);
    ui->lwLOM5_12->clear();
    ui->cbLOM5_14->setCurrentIndex(0);
    ui->lwLOM5_14->clear();
    ui->cbLOM5_15->setCurrentIndex(0);
    ui->lwLOM5_15->clear();
    ui->cbLOM5_16->setCurrentIndex(0);
    ui->lwLOM5_16->clear();
    ui->cbLOM6Licence->setCurrentIndex(0);
    ui->lwLOM9_1selection->clear();
    ui->teAbulEduMediathequePushLog->clear();
    ui->stackedWidgetAbulEduMediathequePush->setCurrentWidget(ui->widgetAbulEduMediathequePushPage1);
}

void AbulEduMediathequePushV1::slotUpdateInterfaceFromLom()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotAbeMediathequePushClean();
    /* Avant tt chose, on ajoute l'utilisateur actuel comme auteur après avoir vidé les champs */
    m_abuledufile->abeFileGetLOM()->abeLOMaddLifeCycleContributionRole("initiator", abeApp->getAbeIdentite()->abeGetVCard(), QDate::currentDate());

    // ================================================= Page 1
    /*
       Langue (général) on décide de n'avoir qu'une seule langue par fichier .abe, c'est donc un sous ensemble de la norme
       donc on a le droit (tm) ... par defaut on utilise celle qui se trouve dans le fichier LOM, si cette information n'est
       pas présente on s'appuie sur la locale système
    */
    m_lang = "";
    if(m_lom->abeLOMgetGeneralLanguage().count() > 0)
    {
        m_lang = m_lom->abeLOMgetGeneralLanguage().first();
    }
    else
    {
        if (QLocale::system().name().section('_', 0, 0) == "fr")
        {
            m_lang = "fre";
        }
        else if (QLocale::system().name().section('_', 0, 0) == "en")
        {
            m_lang = "eng";
        }
        else if (QLocale::system().name().section('_', 0, 0) == "es")
        {
            m_lang = "spa";
        }
    }

    /** @todo: ecrire pour les autres langues */
    ui->cbLOMLang->blockSignals(true);
    ui->cbLOMLang->clear();
    ui->cbLOMLang->addItems(m_lom->abeLOMgetAvailableLanguages().values());

    //La boite déroulante du choix de la langue
    ui->cbLOMLang->setCurrentIndex(ui->cbLOMLang->findText(m_lom->abeLOMgetAvailableLanguages().value(m_lang)));

    ui->cbLOMLang->blockSignals(false);

    //Le titre du document
    ui->leLOMFR1_2->setText(m_lom->abeLOMgetGeneralTitle(m_lang));

    //La description
    if(m_lom->abeLOMgetGeneralDescription(m_lang).count() > 0) {
        QString titre = m_lom->abeLOMgetGeneralDescription(m_lang).first().trimmed();
        if(titre == "") {
            titre = trUtf8("Écrivez ici le titre de la ressource");
        }
        ui->teSCOLOMFR1_4->setText(titre);
    }
    else {
        ui->teSCOLOMFR1_4->setText(trUtf8("Écrivez ici une description rapide de la ressource ..."));
    }

    //On complète la liste des mots cles déjà utilisés
    ui->AbulEduMediathequePushlwMotsCles->blockSignals(true);
    ui->AbulEduMediathequePushlwMotsCles->clear();
    QStringList kwords = m_lom->abeLOMgetGeneralKeywords(m_lang);
    ui->AbulEduMediathequePushlwMotsCles->addItems(kwords);
    ui->AbulEduMediathequePushlwMotsCles->blockSignals(false);

    //Typologie générale des documents
    ui->cbLOM1_10->blockSignals(true);
    ui->cbLOM1_10->clear();
    ui->cbLOM1_10->addItems(m_lom->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().values());

    QString lastItemToDisplay; // On garde en mémoire le dernier item pour affichage

    /* Icham 2013.05.27 Remplissage ComboBox 1.10 */
    if(!m_lom->abeLOMgetGeneralDocumentTypology().isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetGeneralDocumentTypology()) {
            lastItemToDisplay = toAdd.split(";").last();
            on_cbLOM1_10_currentIndexChanged(lastItemToDisplay);
        }
        if(!lastItemToDisplay.isEmpty()){
            ui->cbLOM1_10->setCurrentIndex(ui->cbLOM1_10->findText(lastItemToDisplay));
        }
        ui->cbLOM1_10->blockSignals(false);
    }
    else {
        ui->cbLOM1_10->blockSignals(false);
        ui->cbLOM1_10->setCurrentIndex(ui->cbLOM1_10->findText(m_lom->abeLOMgetGeneralDocumentTypologyVocabSCOLOMFR().value("collection de données")));
    }

    ui->cbRole->clear();
    ui->cbRole->addItems(m_lom->abeLOMgetLifeCycleContributionRoleVocabLOMFR().values());
    ui->cbRole->addItems(m_lom->abeLOMgetLifeCycleContributionRoleVocabLOM().values());

    //Les auteurs qui se trouvent déjà dans le fichier LOM
    //On liste tous les elements de lifecycle
    ui->lwScoLOMFR2_3->clear();
    QStringList lesRoles = m_lom->abeLOMgetLifeCycleContributionRoles();
    for(int i = 0; i < lesRoles.size(); i++) {
        //les dates pour ce role
        QString role = lesRoles.at(i);
        QString roleTraduis = m_lom->abeLOMgetLifeCycleContributionRoleVocabLOMFR().value(role);
        //Si c'est pas un mot de LOMFR ... il est ptet de LOM ?
        if(roleTraduis.isEmpty()) {
            roleTraduis = m_lom->abeLOMgetLifeCycleContributionRoleVocabLOM().value(role);
        }
        QList<QDate> lesDates = m_lom->abeLOMgetLifeCycleContributionDates(role);
        for(int j = 0; j < lesDates.size(); j++) {
            QDate date = lesDates.at(j);
            //les entites ayant eu ce role a cette date
            QList<vCard> lesEntites = m_lom->abeLOMgetLifeCycleContributionEntities(role, date);
            for(int k = 0; k < lesEntites.size(); k++) {
                vCard entite = lesEntites.at(k);
                QListWidgetItem *item;

                //Si on a un content provider c'est une ressource utilisée via data ... on interdit sa suppression !
                if(role == "content provider") {
                    item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeLectureSeule"), QString("%1 (%2)").arg(entite.getFullName()).arg(roleTraduis));
                    item->setData(Qt::UserRole,false);
                }
                else {
                    item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), QString("%1 (%2)").arg(entite.getFullName()).arg(roleTraduis));
                    item->setData(Qt::UserRole,true);
                }

                ABULEDU_LOG_DEBUG() << "  On ajoute dans la liste des entites " << entite.toByteArray() << " pour le role " << role << " au " << date;

                item->setData(Qt::UserRole+1,entite.toByteArray());
                item->setData(Qt::UserRole+2,role);
                item->setData(Qt::UserRole+3,date);
                ui->lwScoLOMFR2_3->addItem(item);
            }
        }
    }

    ui->deDate->setDate(QDate::currentDate());

    // ================================================= Page 2

    //5.2
    ui->cbLOM5_2->blockSignals(true);
    ui->cbLOM5_2->clear();
    ui->cbLOM5_2->addItems(m_lom->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().values());
    ui->cbLOM5_2->addItems(m_lom->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().values());
    ui->cbLOM5_2->addItems(m_lom->abeLOMgetEducationalLearningResourceTypeVocabLOM().values());

    /* Icham 2013.05.27 Remplissage du Combo-Box 5.2 */
    if(!m_lom->abeLOMgetEducationalLearningResourceType().isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetEducationalLearningResourceType()) {
            lastItemToDisplay = toAdd.split(";").last();
            if(m_lom->abeLOMgetEducationalLearningResourceTypeVocabLOM().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalLearningResourceTypeVocabLOM().value(lastItemToDisplay);
            else if(m_lom->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalLearningResourceTypeVocabLOMFR().value(lastItemToDisplay);
            else if(m_lom->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalLearningResourceTypeVocabSCOLOMFR().value(lastItemToDisplay);
            on_cbLOM5_2_currentIndexChanged(lastItemToDisplay);
        }
        ui->cbLOM5_2->setCurrentIndex(ui->cbLOM5_2->findText(lastItemToDisplay));
        ui->cbLOM5_2->blockSignals(false);
    }
    else {
        ui->cbLOM5_2->blockSignals(false);
        ui->cbLOM5_2->setCurrentIndex(ui->cbLOM5_2->findText(m_lom->abeLOMgetEducationalLearningResourceTypeVocabLOM().value("exercise")));
    }

    //5.6
    ui->cbLOM5_6->blockSignals(true);
    ui->cbLOM5_6->clear();
    ui->cbLOM5_6->addItems(m_lom->abeLOMgetEducationalContextVocabLOMFR().values());
    ui->cbLOM5_6->addItems(m_lom->abeLOMgetEducationalContextVocabLOM().values());
    /* Icham 2013.05.27 Remplissage ComboBox 5.6 avec un foreach */
    if(!m_lom->abeLOMgetEducationalContext().isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetEducationalContext()) {
            lastItemToDisplay = toAdd.split(";").last();
            if(m_lom->abeLOMgetEducationalContextVocabLOM().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalContextVocabLOM().value(lastItemToDisplay);
            else if(m_lom->abeLOMgetEducationalContextVocabLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalContextVocabLOMFR().value(lastItemToDisplay);
            on_cbLOM5_6_currentIndexChanged(lastItemToDisplay);
        }
        ui->cbLOM5_6->setCurrentIndex(ui->cbLOM5_6->findText(lastItemToDisplay));
        ui->cbLOM5_6->blockSignals(false);
    }
    else {
        ui->cbLOM5_6->blockSignals(false);
        ui->cbLOM5_6->setCurrentIndex(ui->cbLOM5_6->findText(m_lom->abeLOMgetEducationalContextVocabLOM().value("school")));
        ui->cbLOM5_6->setCurrentIndex(ui->cbLOM5_6->findText(m_lom->abeLOMgetEducationalContextVocabLOMFR().value("enseignement primaire")));
    }

    //5.10
    /* Icham 2013.05.27 Remplissage TextEdit 5.10 */
    if(!m_lom->abeLOMgetEducationalDescription(m_lang).isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetEducationalDescription(m_lang)) {
            ui->teLOM5_10->setText(toAdd);
        }
    }

    //5.12
    ui->cbLOM5_12->blockSignals(true);
    ui->cbLOM5_12->clear();
    ui->cbLOM5_12->addItems(m_lom->abeLOMgetEducationalActivitiesVocabSCOLOMFR().values());
    ui->cbLOM5_12->addItems(m_lom->abeLOMgetEducationalActivitiesVocabLOMFR().values());
    /* Icham 2013.05.27 Remplissage ComboBox 5.12 avec un foreach */
    if(!m_lom->abeLOMgetEducationalActivities().isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetEducationalActivities()) {
            lastItemToDisplay = toAdd.split(";").last();
            if(m_lom->abeLOMgetEducationalActivitiesVocabSCOLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalActivitiesVocabSCOLOMFR().value(lastItemToDisplay);
            else if(m_lom->abeLOMgetEducationalActivitiesVocabLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalActivitiesVocabLOMFR().value(lastItemToDisplay);
            on_cbLOM5_12_currentIndexChanged(lastItemToDisplay);
        }
        ui->cbLOM5_12->setCurrentIndex(ui->cbLOM5_12->findText(lastItemToDisplay));
    }
    ui->cbLOM5_12->blockSignals(false);

    //5.14
    ui->cbLOM5_14->blockSignals(true);
    ui->cbLOM5_14->clear();
    ui->cbLOM5_14->addItems(m_lom->abeLOMgetEducationalLocationVocabSCOLOMFR().values());
    /* Icham 2013.05.27 Remplissage ComboBox 5.14 avec un foreach */
    if(!m_lom->abeLOMgetEducationalLocation().isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetEducationalLocation()) {
            lastItemToDisplay = toAdd.split(";").last();
            if(m_lom->abeLOMgetEducationalLocationVocabSCOLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalLocationVocabSCOLOMFR().value(lastItemToDisplay);
            on_cbLOM5_14_currentIndexChanged(lastItemToDisplay);
        }
        ui->cbLOM5_14->setCurrentIndex(ui->cbLOM5_14->findText(lastItemToDisplay));
        ui->cbLOM5_14->blockSignals(false);
    }
    else {
        ui->cbLOM5_14->blockSignals(false);
        ui->cbLOM5_14->setCurrentIndex(ui->cbLOM5_14->findText(m_lom->abeLOMgetEducationalLocationVocabSCOLOMFR().value("en salle de classe")));
        ui->cbLOM5_14->setCurrentIndex(ui->cbLOM5_14->findText(m_lom->abeLOMgetEducationalLocationVocabSCOLOMFR().value("en salle informatique")));
        ui->cbLOM5_14->setCurrentIndex(ui->cbLOM5_14->findText(m_lom->abeLOMgetEducationalLocationVocabSCOLOMFR().value("en salle multimédia")));
    }

    //5.15
    ui->cbLOM5_15->blockSignals(true);
    ui->cbLOM5_15->clear();
    ui->cbLOM5_15->addItems(m_lom->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().values());
    /* Icham 2013.05.27 Remplissage ComboBox 5.15 avec un foreach */
    if(!m_lom->abeLOMgetEducationalPedagogicModality().isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetEducationalPedagogicModality()) {
            lastItemToDisplay = toAdd.split(";").last();
            if(m_lom->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalPedagogicModalityVocabSCOLOMFR().value(lastItemToDisplay);
            on_cbLOM5_15_currentIndexChanged(lastItemToDisplay);
        }
        ui->cbLOM5_15->setCurrentIndex(ui->cbLOM5_15->findText(lastItemToDisplay));
    }
    ui->cbLOM5_15->blockSignals(false);

    //5.16
    ui->cbLOM5_16->blockSignals(true);
    ui->cbLOM5_16->clear();
    ui->cbLOM5_16->addItems(m_lom->abeLOMgetEducationalToolsVocabSCOLOMFR().values());
    /* Icham 2013.05.27 Remplissage ComboBox 5.15 avec un foreach */
    if(!m_lom->abeLOMgetEducationalTools().isEmpty()){
        foreach (const QString toAdd, m_lom->abeLOMgetEducationalTools()) {
            lastItemToDisplay = toAdd.split(";").last();
            if(m_lom->abeLOMgetEducationalToolsVocabSCOLOMFR().contains(lastItemToDisplay))
                lastItemToDisplay = m_lom->abeLOMgetEducationalToolsVocabSCOLOMFR().value(lastItemToDisplay);
            on_cbLOM5_16_currentIndexChanged(lastItemToDisplay);
        }
        ui->cbLOM5_16->setCurrentIndex(ui->cbLOM5_16->findText(lastItemToDisplay));
        ui->cbLOM5_16->blockSignals(false);
    }
    else {
        ui->cbLOM5_16->blockSignals(false);
        ui->cbLOM5_16->setCurrentIndex(ui->cbLOM5_16->findText(m_lom->abeLOMgetEducationalToolsVocabSCOLOMFR().value("ordinateur")));
        ui->cbLOM5_16->setCurrentIndex(ui->cbLOM5_16->findText(m_lom->abeLOMgetEducationalToolsVocabSCOLOMFR().value("tablette informatique")));
        ui->cbLOM5_16->setCurrentIndex(ui->cbLOM5_16->findText(m_lom->abeLOMgetEducationalToolsVocabSCOLOMFR().value("vidéoprojecteur")));
    }

    //6
    ui->cbLOM6Licence->blockSignals(true);
    if(!m_lom->abeLOMgetRightsDescription(m_lang).isEmpty()){
        ui->cbLOM6Licence->setCurrentIndex(ui->cbLOM6Licence->findText(m_lom->abeLOMgetRightsDescription(m_lang)));
        ui->cbLOM6Licence->blockSignals(false);
    }
    else {
        ui->cbLOM6Licence->blockSignals(false);
        ui->cbLOM6Licence->setCurrentIndex(0);
    }
}

void AbulEduMediathequePushV1::slotUpdateLomFromInterface()
{
    /** @todo */

    // =============================================================== page 1
    m_lom->abeLOMsetGeneralTitle(m_lang,ui->leLOMFR1_2->text().trimmed());
    m_lom->abeLOMsetGeneralDescription(m_lang,ui->teSCOLOMFR1_4->toPlainText());
    m_lom->abeLOMclearGeneralKeywords();
    for(int i = 0; i < ui->AbulEduMediathequePushlwMotsCles->count(); i++) {
        m_lom->abeLOMaddGeneralKeyword(m_lang,ui->AbulEduMediathequePushlwMotsCles->item(i)->text());
    }

    m_lom->abeLOMclearGeneralDocumentTypology();
    for(int i = 0; i < ui->lwLOM1_10->count(); i++) {
        m_lom->abeLOMaddGeneralDocumentTypology(ui->lwLOM1_10->item(i)->text());
    }

    //2.3 contributions
    m_lom->abeLOMclearLifeCycleContributionRole();
    for(int i = 0; i < ui->lwScoLOMFR2_3->count(); i++) {
        QListWidgetItem *item = ui->lwScoLOMFR2_3->item(i);
        QList<vCard> entities = vCard::fromByteArray(item->data(Qt::UserRole+1).toByteArray());
        vCard entity = entities.first();
        m_lom->abeLOMaddLifeCycleContributionRole(item->data(Qt::UserRole+2).toString(),
                                                  entity,
                                                  item->data(Qt::UserRole+3).toDate());
    }

    // =============================================================== page 2
    m_lom->abeLOMclearEducationalLearningResourceType();
    for(int i = 0; i < ui->lwLOM5_2->count(); i++) {
        QListWidgetItem *item = ui->lwLOM5_2->item(i);
        m_lom->abeLOMaddEducationalLearningResourceType(item->text());
    }

    m_lom->abeLOMclearEducationalContext();
    for(int i = 0; i < ui->lwLOM5_6->count(); i++) {
        QListWidgetItem *item = ui->lwLOM5_6->item(i);
        m_lom->abeLOMaddEducationalContext(item->text());
    }

    m_lom->abeLOMclearEducationalDescription();
    m_lom->abeLOMaddEducationalDescription(m_lang,ui->teLOM5_10->toPlainText());

    m_lom->abeLOMclearEducationalActivities();
    for(int i = 0; i < ui->lwLOM5_12->count(); i++) {
        QListWidgetItem *item = ui->lwLOM5_12->item(i);
        m_lom->abeLOMaddEducationalActivities(item->text());
    }

    m_lom->abeLOMclearEducationalLocation();
    for(int i = 0; i < ui->lwLOM5_14->count(); i++) {
        QListWidgetItem *item = ui->lwLOM5_14->item(i);
        m_lom->abeLOMaddEducationalLocation(item->text());
    }

    m_lom->abeLOMclearEducationalPedagogicModality();
    for(int i = 0; i < ui->lwLOM5_15->count(); i++) {
        QListWidgetItem *item = ui->lwLOM5_15->item(i);
        m_lom->abeLOMaddEducationalPedagogicModality(item->text());
    }

    m_lom->abeLOMclearEducationalTools();
    for(int i = 0; i < ui->lwLOM5_16->count(); i++) {
        QListWidgetItem *item = ui->lwLOM5_16->item(i);
        m_lom->abeLOMaddEducationalTools(item->text());
    }

    m_lom->abeLOMsetRightsCost("no");
    m_lom->abeLOMsetRightsCopyrightAndOtherRestrictions("yes");
    m_lom->abeLOMsetRightsDescription(m_lang,ui->cbLOM6Licence->currentText());
}

void AbulEduMediathequePushV1::on_btnAbulEduMediathequePushSuivant_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    gestionBtnSuivantPrecedent(1);
}

void AbulEduMediathequePushV1::on_btnAbulEduMediathequePushPrecedent_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    gestionBtnSuivantPrecedent(-1);
}

void AbulEduMediathequePushV1::on_cbLOMLang_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_lang = arg1;
}

void AbulEduMediathequePushV1::slotKeywordsDownloaded()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->AbulEduMediathequePushlwSuggestions->clear();
    QStringList l = ui->AbulEduMediathequePushleMotCle->abeGetKeywordsDownloaded();
    for(int i = 0; i < l.count(); i++) {
        QListWidgetItem *item = new QListWidgetItem(l.at(i));
        ui->AbulEduMediathequePushlwSuggestions->insertItem(0,item);
    }
    ui->AbulEduMediathequePushlwSuggestions->update();
}

void AbulEduMediathequePushV1::on_AbulEduMediathequePushlwSuggestions_itemDoubleClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString m = item->text();
    QList<QListWidgetItem*> l = ui->AbulEduMediathequePushlwMotsCles->findItems(m,Qt::MatchExactly);

    if(l.isEmpty()) {
        QListWidgetItem *itemAjout = new QListWidgetItem(m);
        ui->AbulEduMediathequePushlwMotsCles->insertItem(0,itemAjout);

        ui->AbulEduMediathequePushlwSuggestions->removeItemWidget(item);
        delete(item);
    }
}

void AbulEduMediathequePushV1::on_AbulEduMediathequePushbtnAjoutMotCle_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->AbulEduMediathequePushleMotCleManuel->text() != "") {
        on_AbulEduMediathequePushbtnAjoutMotCle_2_clicked();
    }
    else {
        //Si on est en train de choisir des mots clés suggérés
        if(ui->AbulEduMediathequePushlwSuggestions->selectedItems().count()>0) {
            QString m = ui->AbulEduMediathequePushlwSuggestions->selectedItems().at(0)->text();
            QList<QListWidgetItem*> l = ui->AbulEduMediathequePushlwMotsCles->findItems(m,Qt::MatchExactly);

            if(l.isEmpty()) {
                QListWidgetItem *itemAjout = new QListWidgetItem(m);
                ui->AbulEduMediathequePushlwMotsCles->insertItem(0,itemAjout);

                ui->AbulEduMediathequePushlwSuggestions->removeItemWidget(ui->AbulEduMediathequePushlwSuggestions->selectedItems().at(0));
                delete(ui->AbulEduMediathequePushlwSuggestions->selectedItems().at(0));
            }

        }
    }
}

void AbulEduMediathequePushV1::on_AbulEduMediathequePushbtnAjoutMotCle_2_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString m = ui->AbulEduMediathequePushleMotCleManuel->text();
    QList<QListWidgetItem*> l = ui->AbulEduMediathequePushlwMotsCles->findItems(m,Qt::MatchExactly);
    if(l.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(m);
        ui->AbulEduMediathequePushlwMotsCles->insertItem(0, item);
    }
    ui->AbulEduMediathequePushleMotCleManuel->clear();

}

void AbulEduMediathequePushV1::on_AbulEduMediathequePushleMotCleManuel_returnPressed()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString m = ui->AbulEduMediathequePushleMotCleManuel->text().trimmed();
    QList<QListWidgetItem*> l = ui->AbulEduMediathequePushlwMotsCles->findItems(m,Qt::MatchExactly);

    if(l.isEmpty()) {
        QListWidgetItem *itemAjout = new QListWidgetItem(m);
        ui->AbulEduMediathequePushlwMotsCles->insertItem(0,itemAjout);
    }
    ui->AbulEduMediathequePushleMotCleManuel->clear();
}

void AbulEduMediathequePushV1::slotDeleteItemKeywords()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    ABULEDU_LOG_DEBUG() << "MainWindow::slotDeleteItem (1)";
    delete ui->AbulEduMediathequePushlwMotsCles->currentItem();
}

void AbulEduMediathequePushV1::on_cbLOM1_10_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Quand on sélectionne un item on l'ajoute a la liste "magique"
       Sauf s'il y est déjà (Philippe 20130527) */
    if(ui->lwLOM1_10->findItems(arg1,Qt::MatchExactly).size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), arg1);
        ui->lwLOM1_10->addItem(item);
    }
}

void AbulEduMediathequePushV1::on_lwLOM1_10_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}

void AbulEduMediathequePushV1::on_btnPlus2_3_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), QString("%1 (%2)").arg(ui->cbEntite->currentText()).arg(ui->cbRole->currentText()));
    item->setData(Qt::UserRole,true);
    item->setData(Qt::UserRole+1,ui->cbEntite->abeGetCurrentVCard().toByteArray());
    item->setData(Qt::UserRole+2,ui->cbRole->currentText());
    item->setData(Qt::UserRole+3,ui->deDate->date());
    ui->lwScoLOMFR2_3->addItem(item);
    m_lom->abeLOMaddLifeCycleContributionRole(ui->cbRole->currentText(),ui->cbEntite->abeGetCurrentVCard(),ui->deDate->date());
}

void AbulEduMediathequePushV1::on_lwScoLOMFR2_3_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Uniquement si on a le droit de supprimer cet item !
    if(item->data(Qt::UserRole).toBool()) {
        vCard entity;
        entity.fromByteArray(item->data(Qt::UserRole+1).toByteArray());
        m_lom->abeLOMremoveLifeCycleContributionRole(item->data(Qt::UserRole+2).toString(),
                                                     entity,
                                                     item->data(Qt::UserRole+3).toDate());
        delete item;
    }
}

void AbulEduMediathequePushV1::on_cbLOM5_2_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Quand on sélectionne un item on l'ajoute a la liste "magique"
    //Sauf s'il y est déjà (Philippe 20130527)
    if(ui->lwLOM5_2->findItems(arg1,Qt::MatchExactly).size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), arg1);
        ui->lwLOM5_2->addItem(item);
    }
}

void AbulEduMediathequePushV1::on_lwLOM5_2_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}

void AbulEduMediathequePushV1::on_cbLOM5_6_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Quand on sélectionne un item on l'ajoute a la liste "magique"
    //Sauf s'il y est déjà (Philippe 20130527)
    if(ui->lwLOM5_6->findItems(arg1,Qt::MatchExactly).size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), arg1);
        ui->lwLOM5_6->addItem(item);
    }
}

void AbulEduMediathequePushV1::on_lwLOM5_6_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}

void AbulEduMediathequePushV1::on_cbLOM5_12_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Quand on sélectionne un item on l'ajoute a la liste "magique"
    //Sauf s'il y est déjà (Philippe 20130527)
    if(ui->lwLOM5_12->findItems(arg1,Qt::MatchExactly).size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), arg1);
        ui->lwLOM5_12->addItem(item);
    }
}


void AbulEduMediathequePushV1::on_lwLOM5_12_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}

void AbulEduMediathequePushV1::on_cbLOM5_14_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Quand on sélectionne un item on l'ajoute a la liste "magique"
    //Sauf s'il y est déjà (Philippe 20130527)
    if(ui->lwLOM5_14->findItems(arg1,Qt::MatchExactly).size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), arg1);
        ui->lwLOM5_14->addItem(item);
    }
}


void AbulEduMediathequePushV1::on_lwLOM5_14_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}

void AbulEduMediathequePushV1::on_cbLOM5_15_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Quand on sélectionne un item on l'ajoute a la liste "magique"
    //Sauf s'il y est déjà (Philippe 20130527)
    if(ui->lwLOM5_15->findItems(arg1,Qt::MatchExactly).size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), arg1);
        ui->lwLOM5_15->addItem(item);
    }
}


void AbulEduMediathequePushV1::on_lwLOM5_15_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}


void AbulEduMediathequePushV1::on_cbLOM5_16_currentIndexChanged(const QString &arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Quand on sélectionne un item on l'ajoute a la liste "magique"
    //Sauf s'il y est déjà (Philippe 20130527)
    if(ui->lwLOM5_16->findItems(arg1,Qt::MatchExactly).size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), arg1);
        ui->lwLOM5_16->addItem(item);
    }
}


void AbulEduMediathequePushV1::on_lwLOM5_16_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}

void AbulEduMediathequePushV1::on_AbulEduMediathequePushlwMotsCles_customContextMenuRequested(const QPoint &pos)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if (ui->AbulEduMediathequePushlwMotsCles->itemAt(pos) != NULL) // j'ai un item à cet endroit, j'appelle mon menu
    {
        m_menuAbulEduMediathequePushlwMotsCles->exec(ui->AbulEduMediathequePushlwMotsCles->mapToGlobal(pos));
    }
    else // sinon je fais rien
    {
        ABULEDU_LOG_DEBUG() << "Pas d'item";
    }
}

/** Il faut aussi récupérer là les ID */
void AbulEduMediathequePushV1::examineChildElements(const QDomElement &parentElement, QTreeWidgetItem *parentItem,bool fin)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QDomElement element = parentElement.firstChild().toElement();
    while (!element.isNull()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        QFont normalFont = item->font(0);
        QFont italique(item->font(0).family(),item->font(0).pointSize(),item->font(0).weight(),true);
        if(!fin){
            item->setFont(0,italique);
        }
        if(QString::compare(element.tagName(),"TERME",Qt::CaseInsensitive) == 0 || QString::compare(element.tagName(),"TERME_TE",Qt::CaseInsensitive) == 0)
        {
            if(QString::compare(element.tagName(),"TERME_TE",Qt::CaseInsensitive) == 0)
            {
                item->setFont(0,normalFont);
                parentItem->setFont(0,italique);
            }
            QDomElement toto = element.firstChildElement("INTITULE");
            QString strNom = toto.text();
            QDomElement titi = element.firstChildElement("ID");
            //            ABULEDU_LOG_DEBUG()<<" +-+-+-+-+-+-+-+- ";
            //            ABULEDU_LOG_DEBUG()<<"écriture : "<<titi.text();
            //            ABULEDU_LOG_DEBUG()<<" +-+-+-+-+-+-+-+- ";
            item->setData(0,Qt::WhatsThisRole,titi.text());
            //            ABULEDU_LOG_DEBUG()<<" vérification : "<<item->data(0,Qt::WhatsThisRole).toString();
            item->setText(0, strNom);
            parentItem->addChild(item);
            examineChildElements(element, item);
        }
        else {
            examineChildElements(element, parentItem,true);
        }
        if (element.firstChildElement("INTITULE").text() != "")
        {
        }
        element = element.nextSibling().toElement();
    }

}

void AbulEduMediathequePushV1::examineChildElementsBis(const QDomElement &parentElement, QTreeWidgetItem *parentItem)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDomElement element = parentElement.firstChild().toElement();
    while (!element.isNull()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        if(QString::compare(element.tagName(),"TERME",Qt::CaseInsensitive) == 0 || QString::compare(element.tagName(),"TERME_TE",Qt::CaseInsensitive) == 0)
        {
            QDomElement toto = element.firstChildElement("INTITULE");
            QString strNom = toto.text();
            QDomElement titi = element.firstChildElement("ID");
            //            ABULEDU_LOG_DEBUG()<<" +-+-+-+-+-+-+-+- ";
            //            ABULEDU_LOG_DEBUG()<<"écriture : "<<titi.text();
            //            ABULEDU_LOG_DEBUG()<<" +-+-+-+-+-+-+-+- ";
            item->setData(0,Qt::WhatsThisRole,titi.text());
            //            ABULEDU_LOG_DEBUG()<<" vérification : "<<item->data(0,Qt::WhatsThisRole).toString();
            item->setText(0, strNom);
            if (m_fichiersXML.keys().contains(strNom))
            {
                parentItem->addChild(item);
            }
            examineChildElements(element, item);// ici le classique parce qu'on parcourt les sous étages d'un qu'on sait qu'il fallait l'ajouter ?
        }
        else {
            examineChildElementsBis(element, parentItem);
        }
        element = element.nextSibling().toElement();
    }
}

void AbulEduMediathequePushV1::on_twLOM9_1_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(item->childCount() > 0) {
        return;
    }
    QStringList chemin;
    QTreeWidgetItem *it = item;
    while(it->text(0) != trUtf8("Objectif de classification")){
        chemin.append(it->text(0));
        it = it->parent();
    }
    ABULEDU_LOG_DEBUG() << "Chemin Taxon ... " << chemin;
    QStringList cheminRetourne;
    for (int i = chemin.size()-1; i >=0 ; i--)
    {
        cheminRetourne << chemin[i];
    }
    QListWidgetItem *sel = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), item->text(0));
    sel->setData(Qt::WhatsThisRole,item->data(0,Qt::WhatsThisRole).toString());
    sel->setToolTip(cheminRetourne.join("/"));
    ui->lwLOM9_1selection->addItem(sel);
}

void AbulEduMediathequePushV1::on_lwLOM9_1selection_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;
}

void AbulEduMediathequePushV1::on_stackedWidgetAbulEduMediathequePush_currentChanged(int arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(m_previousStackedWidgetPage == ui->widgetAbulEduMediathequePushPageLOM9)
    {
        m_lom->abeLOMclearClassificationPurposes();
        QString langue = m_lom->abeLOMgetAvailableLanguages().key(ui->cbLOMLang->currentText());
        for(int i = 0; i<ui->lwLOM9_1selection->count(); i++)
        {
            QString intitule = ui->lwLOM9_1selection->item(i)->toolTip();
            QStringList tutu = intitule.split("/");
            QString purpose = tutu.takeAt(0);
            QString identifiant = ui->lwLOM9_1selection->item(i)->data(Qt::WhatsThisRole).toString();
            QStringList decoupage =  (identifiant.split("-"));
            decoupage.removeLast();
            decoupage.removeLast();
            QString sourceTaxonPath = decoupage.join("-");
            intitule = tutu.join("/");
            ABULEDU_LOG_DEBUG() << " ===================== "<<arg1<<" ========================== "
                                << purpose
                                << intitule
                                << identifiant
                                << sourceTaxonPath
                                <<" =========================================================== ";

            m_lom->abeLOMaddClassificationPurpose(purpose,LomLS(langue,sourceTaxonPath),identifiant,LomLS(langue,intitule));
        }
        ui->lwLOM9_1selection->clear();
    }

    if (ui->stackedWidgetAbulEduMediathequePush->currentWidget() == ui->widgetAbulEduMediathequePushPageLOM9)
    {

        if(m_lom->abeLOMgetClassificationPurposes().isEmpty()){
            /* on crée un item avec domaine 1 - s'approprier un environnement informatique de travail */
            QList<QTreeWidgetItem *> listeTemp = ui->twLOM9_1->findItems("approprier un environnement informatique de travail",Qt::MatchContains | Qt::MatchRecursive);//Qt::MatchRecursive
            ABULEDU_LOG_DEBUG()<<" DéJà J'Y PASSE ..... "<<listeTemp.size();
            if(listeTemp.size() == 1){
                on_twLOM9_1_itemDoubleClicked(listeTemp.first(),0);
            }
        }
        else{

            QHashIterator<QString, QHash<QString, QHash<QString, QString> > > iteratorObjectif(m_lom->abeLOMgetClassificationPurposes());
            while(iteratorObjectif.hasNext())
            {
                iteratorObjectif.next();
                QString objectif = iteratorObjectif.key().split(";").last();
                if (m_lom->abeLOMgetClassificationPurposeVocabSCOLOMFR().contains(objectif))
                {
                    objectif = m_lom->abeLOMgetClassificationPurposeVocabSCOLOMFR().value(objectif);
                }
                else if (m_lom->abeLOMgetClassificationPurposeVocabLOMFR().contains(objectif))
                {
                    objectif = m_lom->abeLOMgetClassificationPurposeVocabLOMFR().value(objectif);
                }
                else if (m_lom->abeLOMgetClassificationPurposeVocabLOM().contains(objectif))
                {
                    objectif = m_lom->abeLOMgetClassificationPurposeVocabLOM().value(objectif);
                }
                QHashIterator<QString, QHash<QString, QString> > iteratorTaxonPath(iteratorObjectif.value());
                while(iteratorTaxonPath.hasNext())
                {
                    iteratorTaxonPath.next();
                    QHashIterator<QString, QString> iteratorTaxon(iteratorTaxonPath.value());
                    while(iteratorTaxon.hasNext())
                    {
                        iteratorTaxon.next();
                        LomLS entry;
                        entry.fromQString(iteratorTaxon.value());
                        QString texteListItem = entry.get(entry.keys().last());
                        QString toolTipListItem = objectif+"/"+texteListItem;
                        texteListItem = texteListItem.split("/").last();
                        QListWidgetItem *sel = new QListWidgetItem(QIcon(":/abuledumediathequepushv1/AbulEduMediathequeSupprimer"), texteListItem);
                        sel->setData(Qt::WhatsThisRole,iteratorTaxon.key());
                        sel->setToolTip(toolTipListItem);
                        ui->lwLOM9_1selection->addItem(sel);
                    }
                }
            }
        }
    }
    m_previousStackedWidgetPage = ui->stackedWidgetAbulEduMediathequePush->widget(arg1);
}

void AbulEduMediathequePushV1::on_btnAbulEduMediathequePushQuit_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalMediathequePushCloseOrHide();
}
