/** Classe AbulEduBoxFileManagerV1
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013 Icham Sirat <icham.sirat@ryxeo.com>
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

#include "abuleduboxfilemanagerv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduBoxFileManagerV1::AbulEduBoxFileManagerV1(QWidget *parent, AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerOpenOrSave openOrSave) :
    QWidget(parent),
    ui(new Ui::AbulEduBoxFileManagerV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_importApplicationName = "";
    m_iAmSure = false;
    ui->setupUi(this);
    this->setObjectName("AbulEduBoxFileManagerV1");

    /* Instanciation variables locales */
    m_sender = 0;
    m_filters = QStringList();
    m_parent = parent;
    m_quelleBox = abePC;
    m_nam = abeApp->getAbeNetworkAccessManager();
    m_rpcAbulEduBox = new MaiaXmlRpcClient(QUrl(abe::ABE_WAN_SERVERFILESURI + "index.php"), this);
    connect(m_nam,SIGNAL(ssoAuthStatus(int)),this,SLOT(slotSSOStatusChanged(int)), Qt::UniqueConnection);

    abeSetOpenOrSaveEnum(openOrSave);
    m_abuleduFile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1(this), &QObject::deleteLater);
    m_abuleduFile->setObjectName("depuis abuleduboxfilemanager");

    /* Création arborescence si non existante */
    QDir dirTemp(abeApp->getAbeHomepath() + "/leterrier/" + "/abebox/" + abeApp->applicationName());
    if(!dirTemp.exists()) {
        dirTemp.mkpath(dirTemp.absolutePath());
    }
    QDir dirTempThumbs(abeApp->getAbeHomepath() + "/leterrier/" + "/abebox/" + abeApp->applicationName() + "/.thumbnails");
    if(!dirTempThumbs.exists()) {
        dirTempThumbs.mkpath(dirTempThumbs.absolutePath());
    }
    m_dirABE           = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName());
    m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName() + "/.thumbnails");


    /* Par defaut on affiche */
    ui->twComplet->show();

#if QT_VERSION < 0x050000
    ui->twComplet->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
#endif

    /* Signal pour téléchargement de fichier */
    connect(ui->widgetMediathequeGet, SIGNAL(signalMediathequeFileDownloaded(QSharedPointer<AbulEduFileV1>, int)),
            SIGNAL(signalAbeFileSelected(QSharedPointer<AbulEduFileV1>)), Qt::UniqueConnection);

    ui->btnSupprimer->setEnabled(true);
    ui->cbChoixAppliImporter->setVisible(false);

    /* Ajout des entrées de la combo box de liste des formats de fichiers d'import ... à maintenir et releaser une nouvelle
        version à chaque nouvelle application ! */
    ui->cbChoixAppliImporter->blockSignals(true);
    ui->cbChoixAppliImporter->insertItem(0,trUtf8(""),"");
    ui->cbChoixAppliImporter->insertItem(1,trUtf8("Raconte-moi"),"leterrier-racont");
    ui->cbChoixAppliImporter->insertItem(1,trUtf8("ALLER"),"leterrier-aller");
    ui->cbChoixAppliImporter->insertItem(1,trUtf8("Imagéo"),"leterrier-imageo");
    ui->cbChoixAppliImporter->insertItem(1,trUtf8("Images séquentielles"),"leterrier-imagessequentielles");
    ui->cbChoixAppliImporter->insertItem(1,trUtf8("Contour"),"leterrier-contour");
    ui->cbChoixAppliImporter->insertItem(1,trUtf8("Mulot"),"leterrier-mulot");
    ui->cbChoixAppliImporter->insertItem(1,trUtf8("Rallye lecture"),"leterrier-rallyelect");
    ui->cbChoixAppliImporter->model()->sort(0);
    ui->cbChoixAppliImporter->blockSignals(false);

    if(parent != 0) {
        //Super important si on veut que le widget garde son style (css) arrière plan et autre quand on l'intègre dans
        //un autre widget !
        setAttribute(Qt::WA_StyledBackground, true);
        ABULEDU_LOG_DEBUG() << "AbulEduBoxFileManagerV1: ********** " << objectName(); // << " ::::: " << styleSheet();
    }
    else {
        ABULEDU_LOG_DEBUG() << "AbulEduBoxFileManagerV1 sans parents : ********** " << objectName(); // << " ::::: " << styleSheet();
    }

    /* Par defaut, affichage complet */
    abeSetDisplaySimpleOrCompleteEnum(abeDisplayComplete);

    /* On enleve la croix de la ssoBox integree dans le stackedWidget */
    ui->widgetAbuleduSSOLogin->abeSetButtonCloseVisible(false);

    /* Activation du bouton "Ouvrir" contextualisée */
    connect(ui->twComplet, SIGNAL(itemSelectionChanged()), this, SLOT(slotManageOpenButton()));
    connect(ui->lwSimple, SIGNAL(itemSelectionChanged()), this, SLOT(slotManageOpenButton()));
    ui->btnOuvrir->setEnabled(false);
    ui->btnSupprimer->setEnabled(false);

    /* On cache les wigets de recherche de la BoxFileManager */
    ui->widgetMediathequeGet->abeSetSearchWidgetsVisible(false);

    foreach(AbulEduFlatBoutonV1* lblBtn,ui->frmCompletHaut->findChildren<AbulEduFlatBoutonV1*>()){
        lblBtn->setTexteAlignement(Qt::AlignBottom);
        lblBtn->abeFlatBoutonSetBackgroundAlignement(Qt::AlignHCenter | Qt::AlignTop);
    }

    /* Personnalisation */
    if(QFile("./data/abuledufilev1/abuleduboxfilemanagerv1.conf").exists()){
        QSettings* config = new QSettings("./data/abuledufilev1/abuleduboxfilemanagerv1.conf",QSettings::IniFormat);
        QString readStyle;
        if(!config->value("buttons-background").toString().isEmpty()){
            readStyle.append(config->value("buttons-background").toString()).append(";");
        }
        else{
            readStyle.append("background-color:#79ac2b;");
        }
        ui->frmCompletHaut->setStyleSheet(QString("QFrame{%1}").arg(readStyle));
    }
}

void AbulEduBoxFileManagerV1::slotManageOpenButton()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Si on a 1 item selectionne (TableWidget ou ListWidget) */
    if(ui->twComplet->selectedItems().count() > 0 || ui->lwSimple->selectedItems().count() > 0){
        ui->btnOuvrir->setEnabled(true);
        ui->btnSupprimer->setEnabled(true);
    }
    else {
        ui->btnOuvrir->setEnabled(false);
        ui->btnSupprimer->setEnabled(false);
    }
}

void AbulEduBoxFileManagerV1::showEvent(QShowEvent *)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->leRecherche->setFocus();
}

AbulEduBoxFileManagerV1::~AbulEduBoxFileManagerV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
    delete m_rpcAbulEduBox;
    m_abuleduFile.clear();
}

void AbulEduBoxFileManagerV1::abeSetFile(QSharedPointer<AbulEduFileV1> file)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << file->objectName();
    m_abuleduFile = file;
    ui->widgetMediathequeGet->abeSetFile(m_abuleduFile);
}

void AbulEduBoxFileManagerV1::abeSetOpenOrSaveEnum(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerOpenOrSave openOrSave, const QString &importApplicationName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_openOrSave = openOrSave;
    m_importApplicationName = importApplicationName;

    /* Gestion du cas spécial de l'import: on affiche la combobox*/
    if(m_openOrSave == abeImport) {
        ui->cbChoixAppliImporter->show();
    }
    else {
        ui->cbChoixAppliImporter->hide();
    }

    nettoyageComplet();
}

void AbulEduBoxFileManagerV1::abeSetSender(QObject *s)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_sender = s;
}

void AbulEduBoxFileManagerV1::abeRefresh(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation location)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "location:" << location;
    qDebug() << __PRETTY_FUNCTION__ << "location:" << location;

    switch(location){
    case abePC:
        qDebug() << "BTN MON PC";
        on_btnMonPC_clicked();
        break;
    case abeBoxPerso:
        qDebug() << "BTN BOX PERSO";
        on_btnBoxPerso_clicked();
        break;
    case abeBoxShare:
        qDebug() << "BTN BOX PART";
        on_btnBoxPartagee_clicked();
        break;
    case abeMediatheque:
        qDebug() << "BTN MEDIA";
        on_btnMediatheque_clicked();
        break;
    }

    ui->twComplet->resizeColumnsToContents();
    ui->twComplet->resizeRowsToContents();
    ui->twComplet->horizontalHeader()->setStretchLastSection(true);
}

void AbulEduBoxFileManagerV1::abeSetProjectName(const QString &name)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "name:"<< name;
    ui->leNomFichierSauvegarder->setText(name);
}

void AbulEduBoxFileManagerV1::abeMediathequeGetHideCloseBouton(const bool &yesNo)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "hide:" << yesNo;
    if(yesNo){
        ui->widgetMediathequeGet->abeHideCloseBouton(true);
    }
    else{
        ui->widgetMediathequeGet->abeHideCloseBouton(false);
    }
}

void AbulEduBoxFileManagerV1::rpcAbulEduBoxFileList(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    Q_UNUSED(reply);
    QStringList iconesPasEnCache;       //icones de fichiers de box perso
    QStringList iconesGroupePasEnCache; //icones de fichiers de box partagees

    ui->twComplet->setSortingEnabled(false);
    ui->lwSimple->setSortingEnabled(false);

    /* Dans le cas d'un import, il faut changer les repertoires de lecture des fichiers abe */
    if(m_openOrSave == abeImport)
    {
        m_dirABE           = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + m_importApplicationName);
        m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + m_importApplicationName + "/.thumbnails");
    }
    else {
        m_dirABE           = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName());
        m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName() + "/.thumbnails");
    }

    ABULEDU_LOG_DEBUG() << "  ============================="
                        << "  " << v.toMap().value("Listing").toString()
                        << "  =============================";

    QApplication::restoreOverrideCursor();

    //Structure retournée: $listingD .= "d;$lataille;$ladate;$entry;$groupe;\n";
    //ou                   $listingF .= "-;$lataille;$ladate;$entry;$groupe;\n";
    QStringList files = QString::fromUtf8(v.toMap().value("Listing").toByteArray()).split("\n")/*v.toMap().value("Listing").toString().split("\n")*/;
    QStringList lesGroupes;

    //on est obligé d'avoir un 2° index pour eviter d'afficher des lignes vides quand on a les groupes
    int j = 0;
    for(int i = 0; i < files.count(); i++) {
        QStringList infos = files.at(i).split(";");
        //Si on a un "groupe" on n'affiche rien, on ajoute dans la boite deroulante la valeur et c'est tout
        if(infos.at(0) == "g") {
            QString leGroupe = infos.at(4).trimmed();
            if(!lesGroupes.contains(leGroupe)) {
                lesGroupes << leGroupe;
            }
            continue;
        }
        if(infos.count() > 2) {
            ui->twComplet->insertRow(j);
            AbulEduFileManagerTableWidgetItemV1 *item1 = new AbulEduFileManagerTableWidgetItemV1(infos.at(0));
            AbulEduFileManagerTableWidgetItemV1 *item2 = new AbulEduFileManagerTableWidgetItemV1(infos.at(1));
            AbulEduFileManagerTableWidgetItemV1 *item3 = new AbulEduFileManagerTableWidgetItemV1(infos.at(2));
            AbulEduFileManagerTableWidgetItemV1 *item4 = new AbulEduFileManagerTableWidgetItemV1(infos.at(3));
            AbulEduFileManagerTableWidgetItemV1 *item5 = new AbulEduFileManagerTableWidgetItemV1(infos.at(4));

            //Et on ajoute aussi dans la partie Simple
            QListWidgetItem *itemSimple = new QListWidgetItem();

            QString fileName = infos.at(3);
            QString leGroupe = infos.at(4).trimmed();
            if(!lesGroupes.contains(leGroupe)) {
                lesGroupes << leGroupe;
            }
            /* On cherche une icone */
            QFileInfo fi;
            //Le fichier cache local du thumbnail est prefixe du nom de la box
            switch (m_quelleBox) {
            case abeBoxShare:
                fi.setFile(m_dirThumbnailsABE.absolutePath() + "/" + ABE_ENUMTOSTRING().value(m_quelleBox) + "-" + leGroupe + "-" + fileName.replace(".abe",".png"));
                if(!fi.exists()){
                    iconesGroupePasEnCache << infos.at(3) + ":" + infos.at(4);
                }
                else {
                    itemSimple->setIcon(QIcon(fi.absoluteFilePath())); // ajout de l'icone sur l'item
                    item1->setIcon(QIcon(fi.absoluteFilePath()));
                }
                break;
            default:
                fi.setFile(m_dirThumbnailsABE.absolutePath() + "/" + ABE_ENUMTOSTRING().value(m_quelleBox) + "-" + fileName.replace(".abe",".png"));
                if(!fi.exists()) {
                    iconesPasEnCache << infos.at(3);
                }
                else {
                    itemSimple->setIcon(QIcon(fi.absoluteFilePath())); // ajout de l'icone sur l'item
                    item1->setIcon(QIcon(fi.absoluteFilePath()));
                }
                break;
            }
            ABULEDU_LOG_DEBUG() << infos.at(3);
            fileName = infos.at(3);
            item1->setData(Qt::UserRole+1, fileName);
            item1->setData(Qt::UserRole+2, leGroupe);
            item2->setData(Qt::UserRole+1, fileName);
            item2->setData(Qt::UserRole+2, leGroupe);
            item3->setData(Qt::UserRole+1, fileName);
            item3->setData(Qt::UserRole+2, leGroupe);
            item4->setData(Qt::UserRole+1, fileName);
            item4->setData(Qt::UserRole+2, leGroupe);
            item5->setData(Qt::UserRole+1, fileName);
            item5->setData(Qt::UserRole+2, leGroupe);

            ui->twComplet->setItem(j, 0, item1);
            ui->twComplet->setItem(j, 1, item2);
            ui->twComplet->setItem(j, 2, item3);
            ui->twComplet->setItem(j, 3, item4);
            ui->twComplet->setItem(j, 4, item5);

            // itemSimple->setIcon(QIcon("/usr/share/icons/autoscan-network.png")); // ajout de l'icone sur l'item
            itemSimple->setData(Qt::UserRole+1, fileName);
            itemSimple->setData(Qt::UserRole+2, leGroupe);
            ui->lwSimple->insertItem(j,itemSimple);

            ABULEDU_LOG_DEBUG() << "MON DEBUG : "<< fileName << QString::fromUtf8(fileName.toLocal8Bit()) ;
        }
        j++;
    }

    ui->cbGroupeSauvegarder->addItems(lesGroupes);

    if(iconesPasEnCache.size() > 0 ||  iconesGroupePasEnCache.size() > 0) {
        QVariantMap tableau;
        tableau["appname"]       = abeApp->applicationName();
        tableau["filelist"]      = iconesPasEnCache.join(";");
        tableau["fileGrouplist"] = iconesGroupePasEnCache.join(";");

        ABULEDU_LOG_DEBUG() << ">> Icones pas en cache ... on cherche " << tableau;
        qDebug() << ">> Icones pas en cache ... on cherche " << tableau;

        QApplication::setOverrideCursor(Qt::WaitCursor);

        QList<QVariant> params = QList<QVariant>() << tableau;
        m_rpcAbulEduBox->call("abuledu.files.getThumbnails", params,
                              this, SLOT(rpcGetThumbnails(QVariant&, QNetworkReply*)),
                              this, SLOT(rpcFault(int, const QString &)));
    }

    ui->twComplet->resizeColumnsToContents();
    ui->twComplet->setSortingEnabled(true);
    ui->lwSimple->setSortingEnabled(true);

#if QT_VERSION < 0x050000
    ui->twComplet->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
#endif

    updateLabelInfo();
    abeSwitchPageSimpleOrComplete();
}

void AbulEduBoxFileManagerV1::rpcFault(int code, const QString &errorMessage)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< "code:"<< code << "errorMessage:" << errorMessage;
    qDebug() << __PRETTY_FUNCTION__<< "code:"<< code << "errorMessage:" << errorMessage;

    m_rpcAbulEduBox->activateProgressBar(false);
    /* Tentative de réouverture d'une session SSO en cas de timeout */
    if(code == -32300) {
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageAbulEduBoxFileManagerSSOLogin);
    }
}

void AbulEduBoxFileManagerV1::rpcFaultFileUpload(int code, const QString &errorMessage)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<< "code:"<< code << "errorMessage:" << errorMessage;
    qDebug() << __PRETTY_FUNCTION__<< "code:"<< code << "errorMessage:" << errorMessage;

    /* Tentative de réouverture d'une session SSO en cas de timeout */
    if(code == -32300) {
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageAbulEduBoxFileManagerSSOLogin);
    }
    emit signalAbeFileSaved(abePC,m_abuleduFile->abeFileGetFileName().absoluteFilePath(),false);
}

void AbulEduBoxFileManagerV1::rpcFileUpload(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "  ============================================================"
                        << v.toMap()
                        << "  ============================================================";
    /**
      * @todo gérer l'erreur éventuelle: le code retour du serveur (la transmission réseau est ptet ok mais c'est pas pour autant que le fichier est bien stocké "la bas"
      */
    /* et on supprime le fichier temporaire */
    emit signalAbeFileSaved(abeBoxFileManagerGetSavingLocation(), m_abuleduFile->abeFileGetFileName().fileName(), true);

}

void AbulEduBoxFileManagerV1::slotUploadFile(const QString &fileName, const QString &groupe, const QString &destName)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //Si on est authentifie
    if(m_nam->abeSSOAuthenticationStatus() == 1) {
        QFileInfo fi(fileName);
        QCryptographicHash md5(QCryptographicHash::Md5);
        if(fi.exists()) {
            QString fichierBase64;
            QFile fic(fileName);
            if (fic.open(QIODevice::ReadOnly)) {
                QByteArray fileData = fic.readAll();
                fichierBase64 = fileData.toBase64();
                md5.addData(fileData);
                fic.close();
            }
            else {
                ABULEDU_LOG_DEBUG()  << "Erreur d'ouverture du fichier ";
                return;
            }

            QVariantMap tableau;
            tableau["appname"]      = abeApp->applicationName();
            if(destName == "same"){
                tableau["fichier_nom"]  = fi.fileName();
            }
            else
            {
                tableau["fichier_nom"]  = destName;
            }
            tableau["groupe_nom"]   = groupe;
            tableau["fichier_md5"]  = QString(md5.result().toHex());
            ABULEDU_LOG_DEBUG() << "on envoie " << tableau;
            tableau["fichier_content"]  = fichierBase64;
            QList<QVariant> params = QList<QVariant>() << tableau;

            m_rpcAbulEduBox->activateProgressBar(true,true);
            m_rpcAbulEduBox->call("abuledu.files.setFile", params,
                                  this, SLOT(rpcFileUpload(QVariant&, QNetworkReply*)),
                                  this, SLOT(rpcFaultFileUpload(int, const QString &)));

        }
    }
    else {
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageAbulEduBoxFileManagerSSOLogin);
        m_nam->abeOnLoginSuccessGoto(this, SLOT(on_btnNomFichierSauvegarder_clicked()));
    }
}

void AbulEduBoxFileManagerV1::on_twComplet_itemDoubleClicked(QTableWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString filename(item->data(Qt::UserRole+1).toString());
    ABULEDU_LOG_DEBUG() << "filename:" << filename;

    /* Si on est en mode sauvegarde on demande confirmation pour écraser le fichier sélectionné */
    if(m_openOrSave == abeSave) {
        demandeConfirmationSauvegardeFichier(filename);
    }
    else {
        on_btnOuvrir_clicked();
    }
}

void AbulEduBoxFileManagerV1::rpcAbulEduBoxGetFile(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //                        << "  ============================="
    //                        << v
    //                        << "  =============================";
    Q_UNUSED(reply);
    QApplication::restoreOverrideCursor();
    m_rpcAbulEduBox->activateProgressBar(false);

    ABULEDU_LOG_DEBUG() << v.toMap().value("status").toString() << reply->errorString();

    if(v.toMap().value("status").toString() == "200") {
        //Soit un fichier temporaire
        //QTemporaryFile *file = new QTemporaryFile(QDir::tempPath() + "/AbulEduMediathequeGetV1-XXXXXXX" + v.toMap().value("ID").toString() + ".abe");


        //Soit un fichier dont le nom est evident pour qu'on puisse ne pas le re-telecharger dans 20 secondes
        //si on veut le réouvrir / rechercher à nouveau
        //QFile file(QDir::tempPath() + "/AbulEduMediathequeGetV1-" + m_docID + ".abe");
        //ou son nom officiel sur la boutique
        QString temporaire(QDateTime::currentDateTime().toString("hhmmsszzz"));
        QString filename(QDir::tempPath() + "/" + QString::fromUtf8(v.toMap().value("fichier_name", "AbulEduBoxFileManagerV1-" + temporaire + ".abe").toByteArray()));
        QFile file(filename);

        if(file.open(QIODevice::WriteOnly)) {
            QByteArray temp = QByteArray::fromBase64(v.toMap().value("fichier_content").toByteArray());
            file.write(temp, temp.size());
            file.close();

            //A noter, c'est une mauvaise idee de passer par un qdatastream: il ajoute quelques octets en entete qui
            //plantent tout !
            //QDataStream out(&file);
            //out << QByteArray::fromBase64(v.toMap().value("fichier_content").toByteArray());
            //file.close();

            ABULEDU_LOG_DEBUG() << "Fichier sauvegarde dans :" << file.fileName();

            //Si c'est un fichier abe on l'ouvre nous-même
            if(file.fileName().endsWith(".abe")) {
                //            m_abuleduFile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1, &QObject::deleteLater);
                //            m_abuleduFile->setObjectName("depuis abuleduboxfilemanager (2)");
                m_abuleduFile->abeFileOpen(filename);
                if(m_openOrSave == abeImport)
                {
                    emit signalAbeFileSelectedImport(m_abuleduFile);
                }
                else
                {
                    emit signalAbeFileSelected(m_abuleduFile);
                }
                ABULEDU_LOG_DEBUG() <<"emission de selected pour "<<m_abuleduFile->abeFileGetFileName().absoluteFilePath();
            }
            else {
                QDesktopServices::openUrl(QUrl(filename, QUrl::TolerantMode));
            }
        }
    }
    else {
        /* Erreur, impossible de télécharger/écrire le fichier */
        emit signalAbeBoxFileManagerDownloadError(reply->error());
        /*On affiche une popup uniquement si l'UI est visible
          par exemple dans abuledu-boitamots on appelle une méthode de l'abuleduboxfilemanager pour télécharger le fichier alors qu'elle n'est n'est pas visible ... */
        if(isVisible()) {
            AbulEduMessageBoxV1 *box = new AbulEduMessageBoxV1(trUtf8("Erreur"), trUtf8("Erreur de téléchargement du fichier !"),true,ui->stackedWidgetAbulEduBoxFileManager->currentWidget());
            box->show();
        }
    }
}

void AbulEduBoxFileManagerV1::on_leRecherche_returnPressed()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->widgetMediathequeGet->abeClear(true);
    ui->widgetMediathequeGet->abeSearch(ui->leRecherche->text());
    ui->leRecherche->clear();
}

void AbulEduBoxFileManagerV1::on_lwSimple_itemDoubleClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString filename(item->data(Qt::UserRole+1).toString());

    //Si on est en mode sauvegarde on demande confirmation pour écraser le fichier sélectionné
    if(m_openOrSave == abeSave) {
        demandeConfirmationSauvegardeFichier(filename);
    }
    else {
        //        on_btnSimpleOuvrir_clicked();
        on_btnOuvrir_clicked();
    }
}

void AbulEduBoxFileManagerV1::on_btnOuvrir_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    /* 20130902:Icham: ouvrir soit en mode complet (premier if), soit en mode simple. */
    if(ui->twComplet->selectedItems().count() > 0) {
        QString filename(ui->twComplet->selectedItems().at(0)->data(Qt::UserRole+1).toString());
        QString groupe(ui->twComplet->selectedItems().at(0)->data(Qt::UserRole+2).toString());
        ABULEDU_LOG_DEBUG() << "AbulEduBoxFileManagerV1::on_btnOuvrir_clicked " << filename << " du groupe " << groupe;
        ouvertureFichier(filename,groupe);
    }
    else if(ui->lwSimple->selectedItems().count() > 0) {
        QString filename(ui->lwSimple->selectedItems().at(0)->data(Qt::UserRole+1).toString());
        QString groupe(ui->lwSimple->selectedItems().at(0)->data(Qt::UserRole+2).toString());
        ABULEDU_LOG_DEBUG() << "AbulEduBoxFileManagerV1::on_btnOuvrir_clicked " << filename << " du groupe " << groupe;
        ouvertureFichier(filename,groupe);
    }
}

void AbulEduBoxFileManagerV1::on_btnSupprimer_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(abeBoxFileManagerGetSavingLocation() == abePC) {
        if( ui->twComplet->selectedItems().count() > 0) {
            ABULEDU_LOG_DEBUG() << "On est sur la ligne " << ui->twComplet->selectedItems().at(0)->row();
            QFileInfo fi(ui->twComplet->selectedItems().at(0)->data(Qt::UserRole+1).toString());
            ABULEDU_LOG_DEBUG() << "Fichier : " << fi.absoluteFilePath();
            if(fi.exists()) {
                /* Pas de if(siVisible) ici : si on a cliqué c'est qu'elle est visible */
                AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Supprimer un fichier"),trUtf8("Êtes-vous sûr de vouloir supprimer ce fichier ?"),true,ui->pageComplet);
                msg->abeSetModeEnum(AbulEduMessageBoxV1::abeAcceptCancelButton);
                connect(msg, SIGNAL(signalAbeMessageBoxCancel()),msg, SLOT(close()),Qt::UniqueConnection);
                connect(msg, SIGNAL(signalAbeMessageBoxOK()),this,SLOT(slotDeleteFile()),Qt::UniqueConnection);
                msg->show();
            }
        }
    }
    else
    {
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Problème"),trUtf8("Suppression impossible dans les répertoires distants"),true,ui->pageComplet);
        msg->show();
    }
    return;
}

void AbulEduBoxFileManagerV1::slotDeleteFile()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFile::remove(ui->twComplet->selectedItems().at(0)->data(Qt::UserRole+1).toString());
    delete ui->twComplet->selectedItems().at(0);
    if( ui->twComplet->selectedItems().count() > 0) {
        ui->twComplet->removeRow(ui->twComplet->selectedItems().at(0)->row());
    }
}

void AbulEduBoxFileManagerV1::nettoyageComplet()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    ui->twComplet->clearContents();
    ui->twComplet->setRowCount(0);
    ui->lwSimple->clear();

#if QT_VERSION < 0x050000
    ui->twComplet->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
#endif

    ui->lblAchetezEspaceStockage->hide();
    ui->cbGroupeSauvegarder->clear();

    switch(m_openOrSave){
    case abeSave:
        ui->widgetFichierSauvegarder->show();
        /* On cache la possibilité de passer en affichage simple */
        ui->btnChangeAffichage->setVisible(false);
        ui->separationVH1->setVisible(false);
        /* Et le moteur de recherche qui n'est pas utile */
        ui->leRecherche->setVisible(false);
        ui->btnRechercher->setVisible(false);
        /* le bouton ouvrir est caché */
        ui->btnOuvrir->setVisible(false);
        ui->btnSupprimer->setVisible(false);
        /* on enregistre pas sur la mediatheque */
        ui->btnMediatheque->setVisible(false);
        break;
    case abeOpen:
        ui->widgetFichierSauvegarder->hide();
        /* On affiche la possibilité de passer en affichage simple         */
        ui->btnChangeAffichage->setVisible(true);
        ui->separationVH1->setVisible(true);
        //Et le moteur de recherche est affiche
        ui->leRecherche->setVisible(true);
        ui->btnRechercher->setVisible(true);
        /* les boutons d'actions Abe sont dispo (coller/copier...) */
//        ui->frActionsAbe->show();
        /* le bouton ouvrir est visible */
        ui->btnOuvrir->setVisible(true);
        ui->btnSupprimer->setVisible(true);
        /* on peut aller voir la mediatheque */
        ui->btnMediatheque->setVisible(true);
        setWindowTitle(trUtf8("Gestionnaire de fichiers AbulÉdu : Mode Ouverture"));
        break;
    case abeImport:
        break;
    }

    /* 20130404:Icham  transformat° if-if-if en switch */
    switch(m_quelleBox){
    case abePC:
        ui->btnMonPC->setChecked(true);
        break;
    case abeBoxPerso:
        ui->btnBoxPerso->setChecked(true);
        break;
    case abeBoxShare:
        ui->btnBoxPartagee->setChecked(true);
        ui->lblNomGroupeSauvegarder->setEnabled(true);
        ui->cbGroupeSauvegarder->setEnabled(true);
        break;
    case abeMediatheque:
        ui->btnMediatheque->setChecked(true);
        /* Pour que les personnes clique sur Telecharger et non Ouvrir */
//        ui->btnOuvrir->setVisible(false);
//        ui->btnSupprimer->setVisible(false);
        break;
    default:
        break;
    }
}

void AbulEduBoxFileManagerV1::abeSwitchPageSimpleOrComplete()
{
    switch (abeGetDisplaySimpleOrCompleteEnum()) {
    case abeDisplayComplete:
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageComplet);
        break;
    case abeDisplaySimple:
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageSimple);
        break;
    }
    updateLabelInfo();
}

void AbulEduBoxFileManagerV1::updateLabelInfo()
{
    QString toShow;

    /* Emplacement */

    switch (m_quelleBox) {
    case abePC:
        toShow =+ "Disque Local | ";
        break;
    case abeBoxPerso:
        toShow =+ "Box Personnelle | ";
        break;
    case abeBoxShare:
        toShow =+ "Box Partagée | ";
        break;
    case abeMediatheque:
        toShow =+ "Médiathèque | ";
        break;
    }
    /* Affichage Simple/Complet */
    switch (abeGetDisplaySimpleOrCompleteEnum()) {
    case abeDisplayComplete:
        toShow += "Affichage Complet ";
        break;
    case abeDisplaySimple:
        toShow += "Affichage Simple";
        break;
    }
    ui->lblInfo->setText(toShow);
    ui->lblInfo->update();
}

void AbulEduBoxFileManagerV1::on_btnMonPC_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    m_quelleBox = abePC;
    nettoyageComplet();
    updateLabelInfo();

    ui->twComplet->setSortingEnabled(false);
    ui->lwSimple->setSortingEnabled(false);

    QStringList filters;
    filters << "*.abe";
    if(m_filters.isEmpty())
    {
        m_filters = filters;
    }

    //Dans le cas d'un import, il faut changer les repertoires de lecture des fichiers abe
    if(m_openOrSave == abeImport)
    {
        m_dirABE           = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + m_importApplicationName);
        m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + m_importApplicationName + "/.thumbnails");
    }
    else {
        m_dirABE           = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName());
        m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName() + "/.thumbnails");
    }

    m_dirABE.setNameFilters(m_filters);
    m_dirABE.setFilter(QDir::Files);
    m_dirABE.setSorting(QDir::Name | QDir::IgnoreCase | QDir::LocaleAware);
    QFileInfoList list = m_dirABE.entryInfoList();
    for(int i = 0; i < list.count(); i++)
    {
        // ========================== SIMPLE DEBUT
        ABULEDU_LOG_DEBUG() << " Fichier dans rep utilisateur: " << list.at(i).completeBaseName();
        //Pour tests on affiche toutes les vignettes d'un répertoire
        QListWidgetItem *item = new QListWidgetItem();

        QFileInfo ficone(m_dirThumbnailsABE.absolutePath() + "/" + list.at(i).completeBaseName() + ".png");
        //Si l'icone n'existe pas on demande a l'extraire ...
        if(!ficone.exists() && m_filters == filters) {
            AbulEduFileV1::abeFileExtractIcon(list.at(i).absoluteFilePath(),ficone.absoluteFilePath());
        }

        QIcon icone(ficone.absoluteFilePath());//pour la mettre  à coté de l'item
        item->setIcon(icone); // ajout de la petite icone sur l'item
        //        item->setText(list.at(i).fileName());
        item->setData(Qt::UserRole+1,list.at(i).absoluteFilePath());
        //        item->setCheckState(Qt::Unchecked);
        ui->lwSimple->insertItem(i,item);
        // ========================== SIMPLE FIN


        // ========================== COMPLET DEBUT
        ABULEDU_LOG_DEBUG() << " Fichier dans rep utilisateur (complet): " << list.at(i).completeBaseName();
        ui->twComplet->insertRow(i);

        AbulEduFileManagerTableWidgetItemV1 *item1 = new AbulEduFileManagerTableWidgetItemV1();
        item1->setIcon(icone);
        AbulEduFileManagerTableWidgetItemV1 *item2 = new AbulEduFileManagerTableWidgetItemV1(QString::number(list.at(i).size()));
        AbulEduFileManagerTableWidgetItemV1 *item3 = new AbulEduFileManagerTableWidgetItemV1(list.at(i).lastModified().toString("dd/MM/yyyy h:m"));
        AbulEduFileManagerTableWidgetItemV1 *item4 = new AbulEduFileManagerTableWidgetItemV1(list.at(i).fileName());

        item1->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());
        item2->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());
        item3->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());
        item4->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());

        ui->twComplet->setItem(i, 0, item1);
        ui->twComplet->setItem(i, 1, item2);
        ui->twComplet->setItem(i, 2, item3);
        ui->twComplet->setItem(i, 3, item4);
        // ========================== COMPLET FIN
    }

    //Et dans le répertoire de l'application uniquement si on est pas en mode import !
    if(m_openOrSave != abeImport)
    {
        QDir dirAppABE;

        #if defined(Q_OS_ANDROID)
                dirAppABE.setPath("file://android_asset/");
        #elif defined(Q_WS_MAC)
                dirAppABE.setPath(QCoreApplication::applicationDirPath() + "/data/");
        #else
                dirAppABE.setPath(QDir::currentPath() + "/data/");
        #endif
        dirAppABE.setNameFilters(m_filters);
        dirAppABE.setFilter(QDir::Files);
        dirAppABE.setSorting(QDir::Name | QDir::IgnoreCase | QDir::LocaleAware);
        list = dirAppABE.entryInfoList();
        for(int i = 0; i < list.count(); i++)
        {
            // ========================== SIMPLE DEBUT
            ABULEDU_LOG_DEBUG() << " Fichier dans rep appli: " << list.at(i).completeBaseName();
            /* Pour tests on affiche toutes les vignettes d'un répertoire */
            QListWidgetItem *item = new QListWidgetItem();
            QFileInfo ficone(m_dirThumbnailsABE.absolutePath() + "/" + list.at(i).completeBaseName() + ".png");
            /* Si l'icone n'existe pas on demande a l'extraire ... */
            if(!ficone.exists() && m_filters == filters) {
                AbulEduFileV1::abeFileExtractIcon(list.at(i).absoluteFilePath(),ficone.absoluteFilePath());
            }

            QIcon icone(ficone.absoluteFilePath());//pour la mettre  à coté de l'item
            item->setIcon(icone); // ajout de la petite icone sur l'item
            //        item->setText(list.at(i).fileName());
            item->setData(Qt::UserRole+1,list.at(i).absoluteFilePath());
            //        item->setCheckState(Qt::Unchecked);
            ui->lwSimple->insertItem(i,item);
            // ========================== SIMPLE FIN
            // ========================== COMPLET DEBUT
            ABULEDU_LOG_DEBUG() << " Fichier dans rep de l'appli (complet): " << list.at(i).completeBaseName();
            ui->twComplet->insertRow(i);

            AbulEduFileManagerTableWidgetItemV1 *item1 = new AbulEduFileManagerTableWidgetItemV1();
            item1->setIcon(icone);
            AbulEduFileManagerTableWidgetItemV1 *item2 = new AbulEduFileManagerTableWidgetItemV1(QString::number(list.at(i).size()));
            AbulEduFileManagerTableWidgetItemV1 *item3 = new AbulEduFileManagerTableWidgetItemV1(list.at(i).lastModified().toString("dd/MM/yyyy h:m"));
            AbulEduFileManagerTableWidgetItemV1 *item4 = new AbulEduFileManagerTableWidgetItemV1(list.at(i).fileName());

            item1->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());
            item2->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());
            item3->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());
            item4->setData(Qt::UserRole+1, list.at(i).absoluteFilePath());

            ui->twComplet->setItem(i, 0, item1);
            ui->twComplet->setItem(i, 1, item2);
            ui->twComplet->setItem(i, 2, item3);
            ui->twComplet->setItem(i, 3, item4);
            // ========================== COMPLET FIN

        }
    }

    ui->twComplet->setSortingEnabled(true);
    ui->lwSimple->setSortingEnabled(true);
    abeSwitchPageSimpleOrComplete();
}

void AbulEduBoxFileManagerV1::on_btnMediatheque_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    m_quelleBox = abeMediatheque;
    ABULEDU_LOG_DEBUG() << "ACTIV W" << qApp->activeWindow();
    nettoyageComplet();
    updateLabelInfo();

    /* Decision ES 20130809 pour android on teste et SSL étant absent on avance en mode non authentifie */
#ifdef Q_OS_ANDROID
#else
    if(m_nam->abeSSOAuthenticationStatus() == 1) {
#endif
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageMediatheque);

        //Et par défaut on affiche les X premiers documents disponibles sur la médiathèque
        ui->leRecherche->setText("*");
        on_leRecherche_returnPressed();

        /* Decision ES 20130809 pour android on teste et SSL étant absent on avance en mode non authentifie */
#ifdef Q_OS_ANDROID
#else
    }
    else {
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageAbulEduBoxFileManagerSSOLogin);
        ui->widgetAbuleduSSOLogin->abeFocus();
        m_nam->abeOnLoginSuccessGoto(this, SLOT(on_btnMediatheque_clicked()));
        m_nam->abeOnLoginFailureGoto(this, SLOT(ifAuthenticateFailed()));
    }
#endif
}

void AbulEduBoxFileManagerV1::on_btnBoxPerso_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    m_quelleBox = abeBoxPerso;
    nettoyageComplet();
    updateLabelInfo();
    /* Si on est authentifie */
    if(m_nam->abeSSOAuthenticationStatus() == 1) {
        QVariantMap tableau;
        tableau["appname"]  = abeApp->applicationName();
        QList<QVariant> params = QList<QVariant>() << tableau;
        m_rpcAbulEduBox->call("abuledu.files.getList", params,
                              this, SLOT(rpcAbulEduBoxFileList(QVariant&, QNetworkReply*)),
                              this, SLOT(rpcFault(int, const QString &)));
    }
    else {
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageAbulEduBoxFileManagerSSOLogin);
        ui->widgetAbuleduSSOLogin->abeFocus();
        m_nam->abeOnLoginSuccessGoto(this, SLOT(on_btnBoxPerso_clicked()));
        m_nam->abeOnLoginFailureGoto(this, SLOT(ifAuthenticateFailed()));
    }
}

void AbulEduBoxFileManagerV1::on_btnBoxPartagee_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    m_quelleBox = abeBoxShare;
    nettoyageComplet();
    updateLabelInfo();
    ui->lblAchetezEspaceStockage->show();
    /* Si on est authentifie */
    if(m_nam->abeSSOAuthenticationStatus() == 1) {
        QVariantMap tableau;
        tableau["appname"]  = abeApp->applicationName();
        QList<QVariant> params = QList<QVariant>() << tableau;
        m_rpcAbulEduBox->call("abuledu.files.getSharedList", params,
                              this, SLOT(rpcAbulEduBoxFileList(QVariant&, QNetworkReply*)),
                              this, SLOT(rpcFault(int, const QString &)));
    }
    else {
        ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageAbulEduBoxFileManagerSSOLogin);
        ui->widgetAbuleduSSOLogin->abeFocus();
        m_nam->abeOnLoginSuccessGoto(this, SLOT(on_btnBoxPartagee_clicked()));
        m_nam->abeOnLoginFailureGoto(this, SLOT(ifAuthenticateFailed()));
    }
}

void AbulEduBoxFileManagerV1::on_btnNomFichierSauvegarder_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->leNomFichierSauvegarder->text().trimmed().isEmpty()) {
        AbulEduMessageBoxV1 *box = new AbulEduMessageBoxV1(trUtf8("Erreur"), trUtf8("Vous n'avez pas donné de nom au fichier !"),true,ui->stackedWidgetAbulEduBoxFileManager->currentWidget());
        box->show();
        return;
    }

    if(m_quelleBox == abePC)
    {
        slotAbeBoxFileManagerSaveOnPC();
    }
    else if(m_quelleBox == abeBoxPerso || m_quelleBox == abeBoxShare)
    {
        slotAbeBoxFileManagerSaveOnBox(m_quelleBox);
    }
    else {
        AbulEduMessageBoxV1 *box = new AbulEduMessageBoxV1(trUtf8("Erreur"), trUtf8("Erreur de destination de sauvegarde !"),true,ui->stackedWidgetAbulEduBoxFileManager->currentWidget());
        box->show();
    }

    ABULEDU_LOG_DEBUG() <<"sauvegarde du fichier .abe dans le dossier "<<m_abuleduFile->abeFileGetFileName().absoluteFilePath();

    /* #3936 clear text for next show */
    ui->leNomFichierSauvegarder->clear();

    //Il ne faut pas lancer ce signal si on va vers un slot !!!
    //emit signalAbeFileCloseOrHide();
}

void AbulEduBoxFileManagerV1::slotAbeBoxFileManagerSaveOnPC()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QDir m_dir(abeApp->getAbeHomepath() + "/leterrier/" + "/abebox/" + abeApp->applicationName());
    ABULEDU_LOG_DEBUG() <<" je cherche s'il y a "<<ui->leNomFichierSauvegarder<<" dans le dossier "<<m_dir.absolutePath();
    m_dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    bool fileExist = false;
    QString fileName = QString();
    int i = 0;
    while(i < m_dir.entryInfoList().size() && !fileExist)
    {
        if(m_dir.entryInfoList().at(i).baseName() == ui->leNomFichierSauvegarder->text().trimmed())
        {
            fileExist = true;
            fileName = m_dir.entryInfoList().at(i).fileName();
        }
        i++;
    }
    /* Cas le plus classique, le fichier n'existe pas on sauvegarde normalement
     * Autre possibilité : le fichier existe *ET* on a forcé son enregistrement */
    if((!fileExist) || (fileExist && m_iAmSure)) {
        bool codeRetour = m_abuleduFile->abeFileExportFile(m_dir.absolutePath() + "/" + ui->leNomFichierSauvegarder->text().trimmed());
        emit signalAbeFileSaved(abePC,m_abuleduFile->abeFileGetFileName().absoluteFilePath(),codeRetour);
        ABULEDU_LOG_DEBUG() <<"signal émis par "<<objectName()<<" avec PC et "<<m_abuleduFile->abeFileGetFileName().absoluteFilePath();
        if(m_iAmSure) {
            //On repasse le flag à faux pour la suite
            m_iAmSure = false;
        }
    }
    /* Par contre si le fichier existe déjà (et qu'on a pas encore décidé de forcer) on demande confirmation */
    else {
        demandeConfirmationSauvegardeFichier(fileName);
    }
}

void AbulEduBoxFileManagerV1::slotAbeBoxFileManagerSaveOnBox(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation location)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Je récupère la liste des fichiers présents. Ils sont tous dans un QTableWidgetItem dans le colonne 1 */
    QFileInfoList fichiersPresents;
    for(int i = 0; i < ui->twComplet->rowCount();i++)
    {
        fichiersPresents << QFileInfo(ui->twComplet->item(i,1)->data(Qt::UserRole+1).toString());
    }
    bool fileExist = false;
    QString fileName = QString();
    int i = 0;
    while(i < fichiersPresents.size() && !fileExist)
    {
        if(fichiersPresents.at(i).baseName() == ui->leNomFichierSauvegarder->text().trimmed())
        {
            fileExist = true;
            fileName = fichiersPresents.at(i).fileName();
        }
        i++;
    }

    /* Cas le plus classique, le fichier n'existe pas on sauvegarde normalement
     * Autre possibilité : le fichier existe *ET* on a forcé son enregistrement */
    if((!fileExist) || (fileExist && m_iAmSure)) {
        //On enregistre tout ça dans un fichier temporaire
        bool codeRetour = m_abuleduFile->abeFileExportFile(QDir::tempPath() + "/" + ui->leNomFichierSauvegarder->text().trimmed());
        if(codeRetour) {
            //et ensuite on uploade
            if(location == abeBoxPerso) {
                slotUploadFile(m_abuleduFile->abeFileGetFileName().absoluteFilePath());
            }
            else if(location == abeBoxShare) {
                slotUploadFile(m_abuleduFile->abeFileGetFileName().absoluteFilePath(), ui->cbGroupeSauvegarder->currentText().trimmed());
            }
            else {
                /* Normalement on ne passera jamais ici mais au cas où ... */
                AbulEduMessageBoxV1 *box = new AbulEduMessageBoxV1(trUtf8("Problème"), trUtf8("Erreur à l'enregistrement du fichier !"),true,ui->stackedWidgetAbulEduBoxFileManager->currentWidget());
                box->show();
            }
            ABULEDU_LOG_DEBUG() <<"signal émis "<<objectName()<<" avec BoxPerso et "<<m_abuleduFile->abeFileGetFileName().fileName();
        }
        //erreur de création du fichier temporaire
        else {
            emit signalAbeFileSaved(abePC,m_abuleduFile->abeFileGetFileName().absoluteFilePath(),codeRetour);
        }
        if(m_iAmSure) {
            //On repasse le flag à faux pour la suite
            m_iAmSure = false;
        }
    }
    //Par contre si le fichier existe déjà (et qu'on a pas encore décidé de forcer) on demande confirmation
    else {
        demandeConfirmationSauvegardeFichier(fileName);
    }
}

void AbulEduBoxFileManagerV1::rpcGetThumbnails(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //                        << "  ============================="
    //                        << v
    //                        << "  =============================";
    Q_UNUSED(reply);

    qDebug() << __PRETTY_FUNCTION__ << reply->error() ;
    QApplication::restoreOverrideCursor();

    /* Pour appliquer l'icône dès reception */
    QList<QListWidgetItem *>  lwi = ui->lwSimple->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    QList<QTableWidgetItem *> twi = ui->twComplet->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    //Dans le cas d'un import, il faut changer les repertoires de lecture des fichiers abe
    if(m_openOrSave == abeImport)
    {
        m_dirABE           = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + m_importApplicationName);
        m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + m_importApplicationName + "/.thumbnails");
    }
    else {
        m_dirABE           = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName());
        m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/abebox/" + abeApp->applicationName() + "/.thumbnails");
    }

    //QVariant(QVariantMap,
    //    QMap(
    //          ("Message", QVariant(QString, "") )
    //          ("SendOK" ,  QVariant(bool, true) )
    //          ( "status" ,  QVariant(QString, "200") )
    //          ( "tabThumb" ,  QVariant(
    //               QVariantList, (QVariant(QVariantMap, QMap(
    //                      ("name", QVariant(QString, "les trois frères.abe") )
    //                      ( "thumb" ,  QVariant(QString,
    if(v.toMap().value("status").toString() == "200") {
        for(int i = 0; i < v.toMap().value("tabThumb").toList().size(); i++) {
            QString nomFichier = v.toMap().value("tabThumb").toList().at(i).toMap().value("name").toString();
            QString nomFichierCacheLocal = m_dirThumbnailsABE.absolutePath() + "/" + ABE_ENUMTOSTRING().value(m_quelleBox) + "-" + nomFichier.replace(".abe",".png");
            ABULEDU_LOG_TRACE() << "  sauvegarde du thumbnail : " << nomFichierCacheLocal;
            QFile file(nomFichierCacheLocal);
            if(file.open(QIODevice::WriteOnly)) {
                QByteArray temp = QByteArray::fromBase64(v.toMap().value("tabThumb").toList().at(i).toMap().value("thumb").toByteArray());
                file.write(temp, temp.size());
                file.close();
                qDebug() << "Thumbnail SAVE OK";
                qDebug() << nomFichier << nomFichierCacheLocal;
                /* On cherche le QListWidgetItem et on met l'icône */
                foreach (QListWidgetItem *i, lwi) {
                    /* Qt::UserRole+1 = nom | Qt::UserRole+2 = groupe */
                    if(nomFichier.contains(i->data(Qt::UserRole+1).toString().replace(".abe", ".png"))){
                        i->setIcon(QIcon(nomFichierCacheLocal));
                        qDebug() << "++ Icone QListWidgetItem OK";
                        lwi.removeOne(i);
                        qDebug() << lwi.count();
                        break;
                    } else {
                        qDebug() << "++ Icone QListWidgetItem KO";
                    }
                }
                /* Pareil pour la QTableWidget */
                foreach (QTableWidgetItem *i, twi) {
                    /* Qt::UserRole+1 = nom | Qt::UserRole+2 = groupe */
                    if(nomFichier.contains(i->data(Qt::UserRole+1).toString().replace(".abe", ".png"))){
                        i->setIcon(QIcon(nomFichierCacheLocal));
                        qDebug() << "++ Icone QTableWidgetItem OK";
                        twi.removeOne(i);
                        qDebug() << twi.count();
                        break;
                    } else {
                        qDebug() << "++ Icone QTableWidgetItem KO";
                    }
                }

            } else {
                qDebug() << "Thumbnail SAVE KO";
            }
        }
    }
}

void AbulEduBoxFileManagerV1::on_twComplet_itemClicked(QTableWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Si on est en mode sauvegarde
    if(m_openOrSave == abeSave) {
        QFileInfo fi(item->data(Qt::UserRole+1).toString());
        ABULEDU_LOG_DEBUG() << "AbulEduBoxFileManagerV1::on_twComplet_itemClicked " << fi.baseName();
        ui->leNomFichierSauvegarder->setText(fi.baseName());
//        ABULEDU_LOG_DEBUG() << " =========== verif =========" << ui->btnAjouter->isEnabled() << " et " << ui->btnColler->isEnabled();
    }
}

void AbulEduBoxFileManagerV1::demandeConfirmationSauvegardeFichier(const QString &filename)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QFileInfo fi(filename);
    AbulEduMessageBoxV1 *box = new AbulEduMessageBoxV1(trUtf8("Confirmation"), trUtf8("Êtes-vous certain de vouloir remplacer le fichier %1 ?").arg(fi.baseName()),true,ui->stackedWidgetAbulEduBoxFileManager->currentWidget());
    box->abeSetModeEnum(AbulEduMessageBoxV1::abeAcceptCancelButton);
    box->setWhatsThis(fi.baseName());
    connect(box, SIGNAL(signalAbeMessageBoxOK()),SLOT(slotAbeBoxFileManagerConfirmSave()));
    box->show();
}

void AbulEduBoxFileManagerV1::ouvertureFichier(const QString &filename, const QString &groupe, bool progressBarVisible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(m_quelleBox == abePC) /* Fichier local */ {
        m_abuleduFile->abeFileOpen(filename);
        if(m_openOrSave == abeImport) {
            emit signalAbeFileSelectedImport(m_abuleduFile);
        }
        else {
            emit signalAbeFileSelected(m_abuleduFile);
        }
        ABULEDU_LOG_DEBUG() <<"emission de selected pour "<<m_abuleduFile->abeFileGetFileName().absoluteFilePath();
    }
    else /* Téléchargement */ {
        QVariantMap tableau;
        tableau["appname"]  = abeApp->applicationName();
        tableau["filename"] = filename;
        tableau["groupe"] = groupe;
        QList<QVariant> params = QList<QVariant>() << tableau;
        ABULEDU_LOG_DEBUG() << "On telecharge : " << tableau;
        m_rpcAbulEduBox->activateProgressBar(progressBarVisible);
        m_rpcAbulEduBox->call("abuledu.files.getFile", params,
                              this, SLOT(rpcAbulEduBoxGetFile(QVariant&, QNetworkReply*)),
                              this, SLOT(rpcFault(int, const QString &)));
        QApplication::setOverrideCursor(Qt::WaitCursor);
    }
}

void AbulEduBoxFileManagerV1::on_btnFermer_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeFileCloseOrHide();
}

void AbulEduBoxFileManagerV1::ifAuthenticateFailed()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduMessageBoxV1* messageAide = new AbulEduMessageBoxV1(trUtf8("Restriction"),trUtf8("Cet espace n'est disponible qu'en étant authentifié..."),true,ui->stackedWidgetAbulEduBoxFileManager->currentWidget());
    messageAide->show();
    on_btnMonPC_clicked();
}

void AbulEduBoxFileManagerV1::slotAbeBoxFileManagerConfirmSave()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //C'est ici qu'on peut passer le flag à vrai
    m_iAmSure = true;
    AbulEduMessageBoxV1 *boxSender = static_cast<AbulEduMessageBoxV1 *>(sender());
    ui->leNomFichierSauvegarder->setText(boxSender->whatsThis());
    on_btnNomFichierSauvegarder_clicked();
}

void AbulEduBoxFileManagerV1::on_cbChoixAppliImporter_currentIndexChanged(int index)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_importApplicationName = ui->cbChoixAppliImporter->itemData(index,Qt::UserRole).toString();

    //Refresh pour actualiser la liste en fonction du nouveau choix
    abeRefresh(m_quelleBox);
}

void AbulEduBoxFileManagerV1::slotSSOStatusChanged(const int code)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "code:" << code;
    if(code == 1) {
        //Si @servecole -> authentification locale
        if(m_nam->abeGetSSOLogin().contains("@" + abe::ABE_LAN_SERVERNAME)) {
            m_rpcAbulEduBox->setUrl(QUrl(abe::ABE_LAN_SERVERFILESURI + "index.php"));
            ABULEDU_LOG_DEBUG() << "AbulEduBoxFileManagerV1::slotSSOStatusChanged :: ON PASSE SUR SERVEUR LOCAL";
        }
    }
}

void AbulEduBoxFileManagerV1::abeSetDisplaySimpleOrCompleteEnum(const AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerDisplaySimpleOrComplete &simpleOrComplete)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__;

    m_displaySimpleOrComplete = simpleOrComplete;
}

void AbulEduBoxFileManagerV1::on_btnChangeAffichage_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    qDebug() << __PRETTY_FUNCTION__ << ui->stackedWidgetAbulEduBoxFileManager->currentWidget()->objectName();

    switch(abeGetDisplaySimpleOrCompleteEnum()){
    case abeDisplaySimple:
        qDebug() << "affichage SIMPLE, on passe en COMPLET";
        abeSetDisplaySimpleOrCompleteEnum(abeDisplayComplete);
        if(ui->stackedWidgetAbulEduBoxFileManager->currentWidget() == ui->pageMediatheque){
            /* Cas particulier, on ne change pas de page */
            ui->widgetMediathequeGet->abeSetListView();
        } else {
            ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageComplet);
        }
        ui->btnChangeAffichage->setIcones(QString(":/abuleduboxfilemanagerv1/affichageComplet"));
        break;

    case abeDisplayComplete:
        qDebug() << "affichage COMPLET, on passe en SIMPLE";
        abeSetDisplaySimpleOrCompleteEnum(abeDisplaySimple);
        if(ui->stackedWidgetAbulEduBoxFileManager->currentWidget() == ui->pageMediatheque){
            /* Cas particulier, on ne change pas de page */
            ui->widgetMediathequeGet->abeSetIconeView();
        } else {
            ui->stackedWidgetAbulEduBoxFileManager->setCurrentWidget(ui->pageSimple);
        }
        ui->btnChangeAffichage->setIcones(QString(":/abuleduboxfilemanagerv1/affichageSimple"));
        break;
    }

    /* Dans tt les cas, desactivation du bouton "Ouvrir" et du bouton "Supprimer" (20151112 Ajout Philippe pour bouton Supprimer) */
    ui->btnOuvrir->setEnabled(false);
    ui->btnSupprimer->setEnabled(false);
    updateLabelInfo();
}

void AbulEduBoxFileManagerV1::on_btnRechercher_clicked()
{
    on_leRecherche_returnPressed();
}
