/** Classe AbulEduMediathequeGetV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
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

#include "abuledumediathequegetv1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduMediathequeGetV1::AbulEduMediathequeGetV1(QWidget *parent, const QString& mediathequeOrData) :
    QWidget(parent),
    AbulEduMediathequeCommon(),
    ui(new Ui::AbulEduMediathequeGetV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_nam = abeApp->getAbeNetworkAccessManager();
    ui->setupUi(this);

    m_rpc = new MaiaXmlRpcClient(this);
    if(mediathequeOrData == "data") {
        abeSetSourceEnum(AbulEduMediathequeGetV1::abeData);
    }
    else if(mediathequeOrData == "fichiers") {
        abeSetSourceEnum(AbulEduMediathequeGetV1::abeFichiers);
    }
    else {
        abeSetSourceEnum(AbulEduMediathequeGetV1::abeMediatheque);
    }

    /* Gestion de la twMediathequeGetComplet qui contient la liste complete des réponses */
    for(int i = 0; i < ui->twMediathequeGetComplet->columnCount(); i++)    {
        /* La largeur de la colonne */
        ui->twMediathequeGetComplet->setColumnWidth(i,100);
        for(int j = 0; j < ui->twMediathequeGetComplet->rowCount(); j++) {
            QTableWidgetItem *tempVide = new QTableWidgetItem("");
            ui->twMediathequeGetComplet->setItem(j,i,tempVide);
        }
    }
    /* choix: sur une petite résolution on supprime type et date, sur une grande résolution on affiche tout */
    if(width() < 800) {
        ui->twMediathequeGetComplet->setColumnWidth(1,0);
        ui->twMediathequeGetComplet->setColumnWidth(2,0);
    }


    m_abuledufile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1(this), &QObject::deleteLater);
    m_abuledufile->setObjectName("depuis abuledumediathequeget");
    m_abuleduLOM = QSharedPointer<AbulEduSCOLOMFrV1>(new AbulEduSCOLOMFrV1(), &QObject::deleteLater);

    /* Création de l'arborescence de téléchargement et mise en cache des fichiers */
    m_dirABE = QDir(abeApp->getAbeHomepath() + "/leterrier/.abebox_cache/" + abeApp->applicationName());
    if(!m_dirABE.exists()) {
        m_dirABE.mkpath(m_dirABE.absolutePath());
    }

    m_dirABB = QDir(abeApp->getAbeHomepath() + "/leterrier/abbbox/");
    if(!m_dirABB.exists()) {
        m_dirABB.mkpath(m_dirABB.absolutePath());
    }

    m_dirABF = QDir(abeApp->getAbeHomepath() + "/leterrier/abfbox/");
    if(!m_dirABF.exists()) {
        m_dirABF.mkpath(m_dirABF.absolutePath());
    }

    /* Création de l'arborescence de téléchargement et mise en cache des vignettes */
    /* 1. pour les fichiers abe */
    m_dirThumbnailsABE = QDir(abeApp->getAbeHomepath() + "/leterrier/.abebox_cache/" + abeApp->applicationName() + "/.thumbnails");
    if(!m_dirThumbnailsABE.exists()) {
        m_dirThumbnailsABE.mkpath(m_dirThumbnailsABE.absolutePath());
    }

    /* 2. pour les fichiers abb */
    m_dirThumbnailsABB = QDir(abeApp->getAbeHomepath() + "/leterrier/abbbox/.thumbnails");
    if(!m_dirThumbnailsABB.exists()) {
        m_dirThumbnailsABB.mkpath(m_dirThumbnailsABB.absolutePath());
    }

    ui->btnCustom1->hide();

    /* et par défaut on affiche le widget bienvenue */
    ui->stackedWidget->setCurrentWidget(ui->PageRecherche);
    ui->frmMerciNouvelleRecherche->hide();

#if QT_VERSION >= 0x040700
    ui->leRecherche->setPlaceholderText(trUtf8("Saisissez ici votre recherche ..."));
#endif

    /* On affiche l'image en grand ... la visionneuse d'icham ? */
    m_visio = new AbulEduVisionneuseImageV1(this);
    m_visio->hide();
    //Je veux debugger cet objet
//    m_visio->abeVisionneuseImageSetLocalDebug(true);
    m_visio->abeVisionneuseImageSetTimeout(0);
//    m_visio->abeVisionneuseImageSetDeleteOnClose(Qt::WA_DeleteOnClose);
}

void AbulEduMediathequeGetV1::abeSetSearchWidgetsVisible(const bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->frmSearch->setVisible(visible);
}

void AbulEduMediathequeGetV1::abeSetListView()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    on_tbVueListe_clicked();
}

void AbulEduMediathequeGetV1::abeSetIconeView()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    on_tbVueIcones_clicked();
}

AbulEduMediathequeGetV1::~AbulEduMediathequeGetV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete m_visio;
    delete ui;
    delete m_rpc;
    m_abuledufile.clear();
}

void AbulEduMediathequeGetV1::abeSetFile(QSharedPointer<AbulEduFileV1> file)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_abuledufile = file;
    m_abuleduLOM = m_abuledufile->abeFileGetLOM();
}

void AbulEduMediathequeGetV1::abeStartDownloadLOM()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->stackedWidget->currentWidget() == ui->pageResultats && !ui->twMediathequeGetComplet->selectedItems().isEmpty()) {
        m_docID = ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+1).toString();
    }
    else if(ui->stackedWidget->currentWidget() == ui->pageResultatsIcones && !ui->lwMediathequeGetSimple->selectedItems().isEmpty()) {
        m_docID = ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+1).toString();
    }

    QList<QVariant> params = QList<QVariant>() << m_docID;

    m_rpc->call("abe.getLOM", params,
                this, SLOT(rpcGetLOM(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    /* On affiche le sablier le temps que ça télécharge ... */
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequeGetV1::abeSearchVisible(bool visible)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(visible)
        ui->frmRecherche->show();
    else
        ui->frmRecherche->hide();
}

void AbulEduMediathequeGetV1::abeSearch(const QString& search)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->leRecherche->setText(search);
    on_btnRecherche_clicked();
}

void AbulEduMediathequeGetV1::abeSetSourceEnum(const enumMediathequeOrData& mediathequeOrData)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_mediathequeOrData = mediathequeOrData;
    if(mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
        m_rpc->setUrl(QUrl("http://data.abuledu.org/Xserver2.php"));
    }
    else if(mediathequeOrData == AbulEduMediathequeGetV1::abeFichiers) {
        m_rpc->setUrl(QUrl("http://fichiers.abuledu.org/Xserver2.php"));
    }
    else {
        m_rpc->setUrl(QUrl("http://mediatheque.abuledu.org/Xserver2.php"));
    }
}

void AbulEduMediathequeGetV1::abeSetCustomBouton1(const QString& titre)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(titre.isEmpty())
        ui->btnCustom1->hide();
    else {
        ui->btnCustom1->setText(titre);
        ui->btnCustom1->show();
    }
}

void AbulEduMediathequeGetV1::abeSetDownloadOnSingleClick(bool enable)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(enable) {
        connect(ui->lwMediathequeGetSimple, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lwMediathequeGetSimple_itemDoubleClicked(QListWidgetItem*)), Qt::UniqueConnection);
        connect(ui->twMediathequeGetComplet, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(on_twMediathequeGetComplet_itemDoubleClicked(QTableWidgetItem*)), Qt::UniqueConnection);
    }
    else {
        disconnect(ui->lwMediathequeGetSimple, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lwMediathequeGetSimple_itemDoubleClicked(QListWidgetItem*)));
        disconnect(ui->twMediathequeGetComplet, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(on_twMediathequeGetComplet_itemDoubleClicked(QTableWidgetItem*)));
    }
}

void AbulEduMediathequeGetV1::abeHideCloseBouton(const bool& yesNo)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(yesNo) {
        ui->tbClose->hide();
    }
    else {
        ui->tbClose->show();
    }
}

void AbulEduMediathequeGetV1::abeSearchWaitingModeration()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QList<QVariant> params = QList<QVariant>();

    m_rpc->call("abe.getValidationWaitingList", params,
                this, SLOT(rpcGetValidationWaitingList(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    /* On affiche le sablier le temps que ça télécharge ... */
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequeGetV1::abeSearchMyRessourcesToModify()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QVariantMap tableau;
    tableau["email"] = m_nam->abeGetSSOLogin();

    QList<QVariant> params = QList<QVariant>() << tableau;

    m_rpc->call("abe.getMyRessourcesToModifyList", params,
                this, SLOT(rpcGetMyRessourcesToModifyList(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    /* On affiche le sablier le temps que ça télécharge ... */
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequeGetV1::abeSetSource(const QString& mediathequeOrData)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(mediathequeOrData == "data") {
        abeSetSourceEnum(AbulEduMediathequeGetV1::abeData);
    }
    else if(mediathequeOrData == "fichiers") {
        abeSetSourceEnum(AbulEduMediathequeGetV1::abeFichiers);
    }
    else {
        abeSetSourceEnum(AbulEduMediathequeGetV1::abeMediatheque);
    }
}

void AbulEduMediathequeGetV1::abeSetDefaultView(const AbulEduMediathequeGetV1::enumMediathequeView& mediathequeView)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(abeMediathequeList == mediathequeView)
        ui->tbVueListe->click();
    else
        ui->tbVueIcones->click();
}

void AbulEduMediathequeGetV1::on_btnRecherche_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->btnImage->setIconeNormale("");
    ui->btnImage->update();

    if(!ui->leRecherche->text().trimmed().isEmpty()) {
        ui->btnImage->setIconeNormale("");
        /* en SQL * c'est % */
        if("*" == ui->leRecherche->text()) {
            ui->leRecherche->setText("%");
        }

        QListWidgetItem *item = new QListWidgetItem(QIcon(":/abuledumediathequegetv1/supprimer"), ui->leRecherche->text().trimmed());
        ui->lwRechercheMotsCles->addItem(item);
        ui->leRecherche->clear();

        slotLanceRecherche();
    }
}

void AbulEduMediathequeGetV1::slotLanceRecherche()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1WaitingPage);
    AbulEduLoadingAnimationV1::getInstance()->start(ui->AbulEduMediathequeGetV1WaitingWidget);

    abeClear(false);
    /* Pour les recherches c'est un peu plus complique, il faut passer un tableau */
    QVariantMap tableau;

    QString unmot = "";
    const int size = ui->lwRechercheMotsCles->count();
    for(int i = 0; i < size; i++) {
        if(i > 0) {
            unmot += ";";
        }
        unmot += ui->lwRechercheMotsCles->item(i)->text();
    }

    if(unmot.isEmpty()) {
        ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
        AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
        return;
    }

    tableau["recherche"]         = unmot;
    tableau["boolcond"]          = "AND";
    tableau["LIST_MATIERES"]     = "";
    tableau["LIST_NIVEAUX"]      = "";

    QStringList formats;
    if(m_searchFileMime.testFlag(abeMediathequeFileMimeAudio))
        formats.append("audio");

    if(m_searchFileMime.testFlag(abeMediathequeFileMimeImage))
        formats.append("image");

    if(m_searchFileMime.testFlag(abeMediathequeFileMimeText))
        formats.append("text");

    if(m_searchFileMime.testFlag(abeMediathequeFileMimeVideo))
        formats.append("video");

    /* Utilisation du pipe pour faire une REGEXP côté mysql */
    tableau["document_mime"] = formats.join("|");

    /* Quand la recherche est sur les data brutes on ne limite pas le champ a une seule application type */
    if(AbulEduMediathequeGetV1::abeData != m_mediathequeOrData ) {
        if(abeApp->applicationName() != "abuledu-alacarte")
            tableau["appName"] = abeApp->applicationName();
    }

    tableau["unauteur"]      = "";
    QList<QVariant> params = QList<QVariant>() << tableau;

    m_rpc->call("abe.getSearchList", params,
                this, SLOT(rpcGetSearchList(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    /* On affiche le sablier le temps que ça télécharge ... */
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequeGetV1::rpcGetSearchList(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QStringList iconesPasEnCache;
    QStringList iconesFileName;

    ABULEDU_LOG_DEBUG() << "============================="
                        << v
                        << "=============================";

    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();

    if("200" == v.toMap().value("status").toString()) {
        /* Si on est sur une page d'accueil, d'info ou autre on passe sur le résultat */
        if(ui->stackedWidget->currentWidget() != ui->pageResultats
                && ui->stackedWidget->currentWidget() != ui->pageResultatsIcones) {
            ui->stackedWidget->setCurrentWidget(ui->pageResultats);
        }

        int num = 0;
        for(int i = 0; i < v.toMap().count(); i++) {
            //        qDebug() << "On tourne pour " << i;
            QMap<QString,QVariant> monItem = v.toMap().value(QString::number(i)).toMap();
            QString titre = QString::fromUtf8(monItem.value("docTitle").toByteArray());

            /* Si le titre est vide on passe au suivant (bug coté serveur) --> variable num */
            if(titre.trimmed().isEmpty()) continue;

            QString Size;
            const uint taille = monItem.value("docSize").toUInt();
            /* taille en Go */
            if (taille > 1073741824) Size = QString(" %1 Go").arg((double) taille / 1073741824, 0, 'f', 1);
            /* En Mo */
            else if (taille > 1048576) Size = QString(" %1 Mo").arg((double) taille / 1048576, 0, 'f', 1);
            /* En Ko */
            else if (taille > 1024) Size = QString(" %1 Ko").arg(taille / 1024);
            /* En octet */
            else Size = QString(" %1 octets").arg(taille);

            /* Au passage on fais un test de sauvegarde locale "mise en cache" */
            QString docFileName = monItem.value("docFileName").toString();
            QString docThumbnail = docFileName;
            QFileInfo fi;
            if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
                fi.setFile(m_dirThumbnailsABB.absolutePath() + "/" + docThumbnail.replace(".abb",".png"));
            }
            else if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeFichiers) {
                fi.setFile(m_dirThumbnailsABF.absolutePath() + "/" + docThumbnail.replace(".abf",".png"));
            }
            else {
                fi.setFile(m_dirThumbnailsABE.absolutePath() + "/" + docThumbnail.replace(".abe",".png"));
            }

            if(!fi.exists()) {
                iconesPasEnCache << monItem.value("docId").toString();
                iconesFileName   << fi.fileName();
            }

            /* Ensuite l'auteur */
            const QString auteur = QString::fromUtf8(monItem.value("docAuteur").toByteArray());

            /* Le résumé */
            const QString resume = QString::fromUtf8(monItem.value("docResume").toByteArray());

            /* Les mots clés */
            const QString motscles = QString::fromUtf8(monItem.value("docMotscles").toByteArray());

            /* La somme MD5 pour verifier si le fichier local est le bon par rapport au fichier distant
             * ou s'il faut actualiser le cache */
            const QString md5 = QString::fromUtf8(monItem.value("docMD5").toByteArray());

            /* Le type mime */
            const QString docMimeType = QString::fromUtf8(monItem.value("docMime").toByteArray());

            /* Le LOM */
            const QByteArray docLOM = QByteArray::fromBase64(monItem.value("docLOM").toByteArray());

            ajouteItemList(num, titre, monItem.value("docType").toString(), monItem.value("docDate").toString(), Size,
                           monItem.value("docId").toString(), monItem.value("URI").toString(), fi,
                           auteur, monItem.value("docNote").toInt(), resume, motscles, md5, docFileName, docMimeType, docLOM);
            num++;
        }

        ui->lwRechercheHierarchie->clear();
        QStringList liste;
        if(!v.toMap().value("hierarchie").isNull()) {
            liste = QString::fromUtf8(v.toMap().value("hierarchie").toByteArray()).split(";");
            for(int i = 0; i < liste.size(); i++) {
                QString value = liste.at(i);
                if(ui->lwRechercheHierarchie->findItems(value,Qt::MatchExactly).size() == 0) {
                    QListWidgetItem *item = new QListWidgetItem(QIcon(""),value);
                    ui->lwRechercheHierarchie->addItem(item);
                }
            }
        }

        if(!v.toMap().value("TA").isNull()) {
            liste = QString::fromUtf8(v.toMap().value("TA").toByteArray()).split(";");
            for(int i = 0; i < liste.size(); i++) {
                QString value = liste.at(i);
                if(ui->lwRechercheHierarchie->findItems(value,Qt::MatchExactly).size() == 0) {
                    QListWidgetItem *item = new QListWidgetItem(QIcon(""),value);
                    ui->lwRechercheHierarchie->addItem(item);
                }
            }
        }

        if(!v.toMap().value("RS").isNull()) {
            liste = QString::fromUtf8(v.toMap().value("RS").toByteArray()).split(";");
            for(int i = 0; i < liste.size(); i++) {
                QString value = liste.at(i);
                if(ui->lwRechercheHierarchie->findItems(value,Qt::MatchExactly).size() == 0) {
                    QListWidgetItem *item = new QListWidgetItem(QIcon(""),value);
                    ui->lwRechercheHierarchie->addItem(item);
                }
            }
        }

        if(iconesPasEnCache.size() > 0) {
            QVariantMap tableau;
            tableau["ids"]        = iconesPasEnCache.join(",");
            tableau["fileNames"]  = iconesFileName.join(",");

            ABULEDU_LOG_DEBUG() << "<< on cherche " << tableau;

            QApplication::setOverrideCursor(Qt::WaitCursor);

            QList<QVariant> params = QList<QVariant>() << tableau;
            m_rpc->call("abe.getThumbnails", params,
                        this, SLOT(rpcGetThumbnails(QVariant&, QNetworkReply*)),
                        this, SLOT(rpcFault(int, const QString &)));
        }
    }
    else {
        ui->stackedWidget->setCurrentWidget(ui->pageEchec);
        QString recherche;
        for(int i = 0; i < ui->lwRechercheMotsCles->count(); i++) {
            recherche.append(" + " + ui->lwRechercheMotsCles->item(i)->text());
        }
        ui->leRechercheRappel->setText(recherche);
        ui->btnTelecharger->setEnabled(false);
        ui->btnSignaler->setEnabled(true);
        ui->teCommentairesRappel->clear();
    }

    ui->twMediathequeGetComplet->resizeColumnsToContents();
    ui->twMediathequeGetComplet->resizeRowsToContents();
    ui->twMediathequeGetComplet->horizontalHeader()->setStretchLastSection(true);
}

void AbulEduMediathequeGetV1::rpcGetFile(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << "============================="
                        << v
                        << "=============================";
    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();
    m_rpc->activateProgressBar(false);

    /* Soit un fichier temporaire
     * QTemporaryFile *file = new QTemporaryFile(QDir::tempPath() + "/AbulEduMediathequeGetV1-XXXXXXX" + v.toMap().value("ID").toString() + ".abe");
     *
     * Soit un fichier dont le nom est evident pour qu'on puisse ne pas le re-telecharger dans 20 secondes
     * si on veut le réouvrir / rechercher à nouveau
     * QFile file(QDir::tempPath() + "/AbulEduMediathequeGetV1-" + m_docID + ".abe");
     * ou son nom officiel sur la boutique
     */
    QString filename;
    if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
        filename = (m_dirABB.absolutePath() + "/" + v.toMap().value("name", "AbulEduMediathequeGetV1-" + m_docID ).toString());
    }
    else if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeFichiers) {
        filename = (m_dirABF.absolutePath() + "/" + v.toMap().value("name", "AbulEduMediathequeGetV1-" + m_docID ).toString());
    }
    else {
        filename = (m_dirABE.absolutePath() + "/" + v.toMap().value("name", "AbulEduMediathequeGetV1-" + m_docID ).toString());
    }
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly)) {
        QByteArray temp = QByteArray::fromBase64(v.toMap().value("content").toByteArray());
        file.write(temp, temp.size());
        file.close();

        /* A noter, c'est une mauvaise idee de passer par un qdatastream: il ajoute quelques octets en entete qui plantent tout !
         * QDataStream out(&file);
         * out << temp;
         * file.close();
         */
        ABULEDU_LOG_DEBUG() << "Fichier sauvegarde dans :" << file.fileName() << "taille " << v.toMap().value("size").toString()
                            <<  " a comparer avec " << temp.size() << "\n\n"
                            << " taille base64 : " << v.toMap().value("base64size").toString() << " -> " << v.toMap().value("content").toByteArray().size();
        handleOpenFile(filename);
    }
    else {
        //* rreur, impossible de télécharger/écrire le fichier */
        emit signalMediathequeFileDownloaded(m_abuledufile, -1);
    }
}

void AbulEduMediathequeGetV1::handleOpenFile(const QString& filename)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* cas particulier si on est avec alacarte ... */
    if("abuledu-alacarte" == abeApp->applicationName()) {
        /* On peut essayer de l'ouvrir via le systeme */
        QDesktopServices::openUrl(filename);
        emit signalMediathequeFileDownloaded(m_abuledufile, 0);
    }
    else {
        /* afaire mise a jour pour gerer les fichiers .abe */
        if(filename.endsWith(".abe") || filename.endsWith(".abb") || filename.endsWith(".abf")) {
            m_abuledufile->abeFileOpen(filename,"zip");
            m_abuleduLOM= m_abuledufile->abeFileGetLOM();
            emit signalMediathequeFileDownloaded(m_abuledufile, 1);
        }
        else {
            /* On peut essayer de l'ouvrir via le systeme
             * (Question, à vérifier, je crois que c'est le cas particulier où on télécharge un .deb un .exe ou autre ?) */
            QDesktopServices::openUrl(filename);
            emit signalMediathequeFileDownloaded(m_abuledufile, 0);
        }
    }
}

void AbulEduMediathequeGetV1::rpcGetThumbnails(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
//                        << "============================="
//                        << v
//                        << "=============================";
    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();

    for(int i = 0; i < v.toMap().count(); i++) {
        //        qDebug() << "On tourne pour " << i;
        QMap<QString,QVariant> monItem = v.toMap().value(QString::number(i)).toMap();

        QString docFileName = QString::fromUtf8(monItem.value("docFileName").toByteArray().constData());
        QFileInfo fi;
        if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
            fi.setFile(m_dirThumbnailsABB.absolutePath() + "/" + docFileName.replace(".abb",".png"));
        }
        else if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeFichiers) {
            fi.setFile(m_dirThumbnailsABF.absolutePath() + "/" + docFileName.replace(".abf",".png"));
        }
        else {
            fi.setFile(m_dirThumbnailsABE.absolutePath() + "/" + docFileName.replace(".abe",".png"));
        }

        ABULEDU_LOG_TRACE() << "  sauvegarde du thumbnail : " << fi.absoluteFilePath();

        QFile file(fi.absoluteFilePath());
        if(file.open(QIODevice::WriteOnly)) {
            ABULEDU_LOG_TRACE()  << "  ouverture du fichier de thumbnail ok";
            QByteArray temp = QByteArray::fromBase64(monItem.value("docThumbnail").toByteArray());
            file.write(temp, temp.size());
            file.close();

            /* Il faudrait rafraichir le ui.lwMediathequeGetSimple */
            lwMediathequeGetSimpleSetIcon(fi.fileName(), fi);
            /* Mais pour pouvoir faire cet appel il faut avoir un setText qui affiche du texte sous les icones */
            //            QList <QListWidgetItem*> listeItems = ui->lwMediathequeGetSimple->findItems(fi.fileName(),Qt::MatchExactly);
            //            if(!listeItems.isEmpty()) {
            //                //                qDebug()  << __FILE__ <<  __LINE__ << __FUNCTION__;
            //                lwMediathequeGetSimpleSetIcon(listeItems.at(0), fi);
            //            }
        }
    }
}

void AbulEduMediathequeGetV1::rpcSendAlertNoRessource(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "============================="
                        << v
                        << "=============================";
    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();
    ui->stackedWidget->setCurrentWidget(ui->PageRecherche);
    ui->frmMerciNouvelleRecherche->show();
    /* Et on cache dans 5 secondes */
    QTimer::singleShot(5000,ui->frmMerciNouvelleRecherche,SLOT(hide()));
}

void AbulEduMediathequeGetV1::rpcGetValidationWaitingList(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Pour l'instant c'est strictement la même chose :) */
    abeClear(true);
    rpcGetSearchList(v,reply);
}

void AbulEduMediathequeGetV1::rpcGetMyRessourcesToModifyList(QVariant &v, QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Pour l'instant c'est strictement la même chose :) */
    abeClear(true);
    rpcGetSearchList(v,reply);
}

void AbulEduMediathequeGetV1::rpcGetLOM(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    /* Le GetLOM */
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "============================="
                        << v
                        << "=============================";
    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();

    QMap<QString,QVariant> monItem = v.toMap();
    m_abuleduLOM->abeLOMsetXML(QByteArray::fromBase64(monItem.value("docLOM").toByteArray()));
    emit signalMediathequeLomDownloaded();
}

void AbulEduMediathequeGetV1::rpcVote(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    /* Le Vote */
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "============================="
                        << v
                        << "=============================";
    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();
}

void AbulEduMediathequeGetV1::slotDownloadFinished(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* Si ce n'est pas pour moi je droppe */
    if("AbulEduMediathequeGetDownload" != reply->objectName()) return;

    disconnect(m_nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
    /** @todo Sauvegarde disque dur */
    ABULEDU_LOG_WARN() << "A implementer, on sauvegarde le fichier sur le disque dur ...";
}

void AbulEduMediathequeGetV1::rpcFault(int code, const QString &errorMessage)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "==================================="
                        << code << errorMessage
                        << "===================================";
    m_rpc->activateProgressBar(false);
    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();

    /* Tentative de réouverture d'une session SSO en cas de timeout */
    if(code == -32300) {
        /* 2013.01.16 : decision pour la tablette */
#ifdef __ABULEDUTABLETTEV1__MODE__
#else
        m_nam->abeSSOLogin();
#endif
    }
}

void AbulEduMediathequeGetV1::on_twMediathequeGetComplet_itemSelectionChanged()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->twMediathequeGetComplet->selectedItems().count() > 0) {
        QMap<int,QVariant> values;
        for(int i = 1; i <= 13; i++) {
            values.insert(Qt::UserRole+i,ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+i));
        }
        communItemSelectionChange(values);
    }
}

void AbulEduMediathequeGetV1::on_btnTelecharger_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString md5;
    QString docFileName;

    if(ui->stackedWidget->currentWidget() == ui->pageResultats && !ui->twMediathequeGetComplet->selectedItems().isEmpty()) {
        m_docID = ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+1).toString();
        md5 = ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+8).toString();
        docFileName = ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+9).toString();
    }
    else if(ui->stackedWidget->currentWidget() == ui->pageResultatsIcones && !ui->lwMediathequeGetSimple->selectedItems().isEmpty()) {
        m_docID = ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+1).toString();
        md5 = ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+8).toString();
        docFileName = ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+9).toString();
    }
    else {
        return;
    }

    ABULEDU_LOG_DEBUG() << " on telecharge " << m_docID << " " << docFileName;

    /* On vérifie si on n'a pas déjà ce fichier en cache */
    QFileInfo fi;
    if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
        fi.setFile(m_dirABB.absolutePath() + "/" + docFileName);
    }
    else if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeFichiers) {
        fi.setFile(m_dirABF.absolutePath() + "/" + docFileName);
    }
    else {
        fi.setFile(m_dirABE.absolutePath() + "/" + docFileName);
    }

    ABULEDU_LOG_DEBUG() << "on demande le document " << docFileName
                        << " dont la somme MD5 est " << md5
                        << "sur le disque il devrait etre ici " << fi.absoluteFilePath();

    /* Si le fichier existe, on calcule sa somme md5 et on compare */
    if(fi.exists()) {
        QString md5verif = m_abuledufile->abeFileGetMD5(fi.absoluteFilePath());
        /* La somme md5 est identique, on ne télécharge pas */
        if(md5verif == md5) {
            ABULEDU_LOG_DEBUG() << "Ce fichier est déjà la";
            handleOpenFile(fi.absoluteFilePath());
            return;
        }
        else {
            ABULEDU_LOG_DEBUG() << "Ce fichier est déjà la mais la somme md5 differe : " << md5verif;
        }
    }

    /* On garde dans un coin la docID pour recomposer le nom du fichier telecharge */
    QList<QVariant> params = QList<QVariant>() << m_docID;

    m_rpc->activateProgressBar(true);

    m_rpc->call("abe.getFile", params,
                this, SLOT(rpcGetFile(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

void AbulEduMediathequeGetV1::on_btnCustom1_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalMediathequeCustomBouton1Clicked();
}

void AbulEduMediathequeGetV1::on_cbAvertirQuandDispo_toggled(bool checked)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->leAdresseMailRappel->setEnabled(checked);
    ui->lblAdresseMail->setEnabled(checked);
}

void AbulEduMediathequeGetV1::on_btnSignaler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->btnSignaler->setEnabled(false);
    /* Pour l'instant on ne gère que pour data */
    if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
        QVariantMap tableau;
        tableau["mot"]               = ui->leRechercheRappel->text().trimmed();
        tableau["commentaires"]      = ui->teCommentairesRappel->toPlainText().trimmed();
        tableau["email"]             = ui->leAdresseMailRappel->text().trimmed();
        QList<QVariant> params = QList<QVariant>() << tableau;

        m_rpc->call("abe.sendAlertNoRessource", params,
                    this, SLOT(rpcSendAlertNoRessource(QVariant&, QNetworkReply*)),
                    this, SLOT(rpcFault(int, const QString &)));
        /* On affiche le sablier le temps que ça télécharge ...*/
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
    }
}

void AbulEduMediathequeGetV1::ajouteItemList(int num, const QString& titre, const QString& type, const QString& date, const QString& size,
                                             const QString& docId, const QString& URI, const QFileInfo& ficInfo, const QString& contributeur, int note,
                                             const QString& resume, const QString& motscles, const QString& md5, const QString& docFileName, const QString& docMimeType,
                                             const QByteArray& docLOM)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << num << titre << type << note;

    /* la liste d'items */
    QTableWidgetItem *item1 = new QTableWidgetItem(titre);
    QTableWidgetItem *item2 = new QTableWidgetItem(type);
    QTableWidgetItem *item3 = new QTableWidgetItem(date);
    QTableWidgetItem *item4 = new QTableWidgetItem(size);
    QTableWidgetItem *item5 = new QTableWidgetItem(note);
    QTableWidgetItem *item6 = new QTableWidgetItem(contributeur);

    item1->setData(Qt::UserRole+1,docId);
    item1->setData(Qt::UserRole+2,URI);
    item1->setData(Qt::UserRole+3,ficInfo.absoluteFilePath());
    item1->setData(Qt::UserRole+4,contributeur);
    item1->setData(Qt::UserRole+5,note);
    item1->setData(Qt::UserRole+6,resume);
    item1->setData(Qt::UserRole+7,motscles);
    item1->setData(Qt::UserRole+8,md5);
    item1->setData(Qt::UserRole+9,docFileName);
    item1->setData(Qt::UserRole+10,docMimeType);
    item1->setData(Qt::UserRole+11,docLOM);
    //Le 12 est pris pour l'icone item1->setData(Qt::UserRole+12,titre);
    item1->setData(Qt::UserRole+13,titre);

    ui->twMediathequeGetComplet->setRowCount(num);
    ui->twMediathequeGetComplet->insertRow(num);
    ui->twMediathequeGetComplet->setItem(num, 0, item1);
    ui->twMediathequeGetComplet->setItem(num, 1, item2);
    ui->twMediathequeGetComplet->setItem(num, 2, item3);
    ui->twMediathequeGetComplet->setItem(num, 3, item4);
    ui->twMediathequeGetComplet->setItem(num, 4, item5);
    ui->twMediathequeGetComplet->setItem(num, 5, item6);

    /* la liste en icones */
    /* Pour tests on affiche toutes les vignettes d'un répertoire */
    QListWidgetItem *itemIcone = new QListWidgetItem();
    itemIcone->setData(Qt::UserRole+1,docId);
    itemIcone->setData(Qt::UserRole+2,URI);
    itemIcone->setData(Qt::UserRole+3,ficInfo.absoluteFilePath());
    itemIcone->setData(Qt::UserRole+4,contributeur);
    itemIcone->setData(Qt::UserRole+5,note);
    itemIcone->setData(Qt::UserRole+6,resume);
    itemIcone->setData(Qt::UserRole+7,motscles);
    itemIcone->setData(Qt::UserRole+8,md5);
    itemIcone->setData(Qt::UserRole+9,docFileName);
    itemIcone->setData(Qt::UserRole+10,docMimeType);
    itemIcone->setData(Qt::UserRole+11,docLOM);

    QFileInfo ficone;
    if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
        ficone.setFile(m_dirThumbnailsABB.absolutePath() + "/" + ficInfo.completeBaseName() + ".png");
    }
    else if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeFichiers) {
        ficone.setFile(m_dirThumbnailsABF.absolutePath() + "/" + ficInfo.completeBaseName() + ".png");
    }
    else {
        ficone.setFile(m_dirThumbnailsABE.absolutePath() + "/" + ficInfo.completeBaseName() + ".png");
    }
    itemIcone->setData(Qt::UserRole+12,ficone.fileName());
    itemIcone->setData(Qt::UserRole+13,titre);
    lwMediathequeGetSimpleSetIcon(itemIcone, ficone);
    //pour pouvoir utiliser le findItem
    //    itemIcone->setText(ficInfo.fileName());
    ui->lwMediathequeGetSimple->insertItem(num,itemIcone);
}

void AbulEduMediathequeGetV1::communItemSelectionChange(const QMap<int,QVariant>& val)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* On a stocké dans le UserRole+3 le thumbnail lors du téléchargement de la liste */
    ui->btnImage->setIconeNormale(val.value(Qt::UserRole+3).toString());
    ui->btnImage->update();

    ABULEDU_LOG_DEBUG() << val.value(Qt::UserRole+5);

    ui->lblRessourceIDValue->setText(val.value(Qt::UserRole+1).toString());

    QString motCles = val.value(Qt::UserRole+7).toString();
    QString resume  = val.value(Qt::UserRole+6).toString();
    QString titre  = val.value(Qt::UserRole+13).toString();

    ui->AbulEduMediathequeGetV1lblTitre->setText(titre);
    ui->AbulEduMediathequeGetV1teMotsCles->setHtml(trUtf8("<h3><center>Mots Clés</center></h3>") + motCles);
    ui->teResume->setHtml(trUtf8("<h3><center>Résumé</center></h3>") + resume);
    QString docFileName = val.value(Qt::UserRole+9).toString();

    QFileInfo fi;
    if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeData) {
        fi.setFile(m_dirABB.absolutePath() + "/" + docFileName);
    }
    else if(m_mediathequeOrData == AbulEduMediathequeGetV1::abeFichiers) {
        fi.setFile(m_dirABF.absolutePath() + "/" + docFileName);
    }
    else {
        fi.setFile(m_dirABE.absolutePath() + "/" + docFileName);
    }

    if(fi.exists()) {
        m_abuledufile->abeFileSetFilename(fi.absoluteFilePath());
        m_abuleduLOM = m_abuledufile->abeFileGetLOM();
    }
    else {
        m_abuleduLOM->abeLOMsetXML(val.value(Qt::UserRole+11).toByteArray());
    }

    m_docID = val.value(Qt::UserRole+1).toString();


    ui->btnTelecharger->setEnabled(true);
}

void AbulEduMediathequeGetV1::lwMediathequeGetSimpleSetIcon(QListWidgetItem *itemIcone, const QFileInfo& ficone)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QIcon icone;
    /* Si l'icone n'existe pas on ... */
    if(ficone.exists())
        icone = QIcon(ficone.absoluteFilePath()); /* pour la mettre  à coté de l'item */
    else
        icone = QIcon(":/abuledufilev1/fond-vide"); /* pour la mettre  à coté de l'item */

    /* ajout de la petite icone sur l'item */
    itemIcone->setIcon(icone);
}

void AbulEduMediathequeGetV1::lwMediathequeGetSimpleSetIcon(QString itemName, const QFileInfo& ficone)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    /* recherche de l'item */
    for(int i = 0; i < ui->lwMediathequeGetSimple->count(); i++) {
        QListWidgetItem *item = ui->lwMediathequeGetSimple->item(i);
        if(item->data(Qt::UserRole+12) == itemName) {
            lwMediathequeGetSimpleSetIcon(item, ficone);
            /* pas la peine d'aller plus loin */
            return;
        }
    }
}

void AbulEduMediathequeGetV1::on_lwMediathequeGetSimple_itemSelectionChanged()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->lwMediathequeGetSimple->selectedItems().count() > 0) {
        QMap<int,QVariant> values;
        for(int i = 1; i <= 13; i++) {
            values.insert(Qt::UserRole+i,ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+i));
        }
        communItemSelectionChange(values);
    }
}

void AbulEduMediathequeGetV1::on_tbVueIcones_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->tbVueListe->setChecked(false);
    ui->tbVueIcones->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->pageResultatsIcones);
}

void AbulEduMediathequeGetV1::on_tbVueListe_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->tbVueListe->setChecked(true);
    ui->tbVueIcones->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->pageResultats);
}

void AbulEduMediathequeGetV1::abeCheckDoublon(const QString& uri)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QList<QVariant> params = QList<QVariant>() << uri;
    m_rpc->call("abe.checkDoublon", params,
                this, SLOT(rpcCheckDoublon(QVariant&, QNetworkReply*)),
                this, SLOT(rpcFault(int, const QString &)));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

QString AbulEduMediathequeGetV1::abeGetCurrentSelectedItemURI()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->stackedWidget->currentWidget() == ui->pageResultats && !ui->twMediathequeGetComplet->selectedItems().isEmpty()) {
        return ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+2).toString();
    }
    else if(ui->stackedWidget->currentWidget() == ui->pageResultatsIcones && !ui->lwMediathequeGetSimple->selectedItems().isEmpty()) {
        return ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+2).toString();
    }
    else {
        return "";
    }
}

void AbulEduMediathequeGetV1::abeRemoveCurrentSelectedItem()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ui->stackedWidget->currentWidget() == ui->pageResultats && !ui->twMediathequeGetComplet->selectedItems().isEmpty()) {
        ui->twMediathequeGetComplet->removeRow(ui->twMediathequeGetComplet->selectedItems().at(0)->row());
    }
    else if(ui->stackedWidget->currentWidget() == ui->pageResultatsIcones && !ui->lwMediathequeGetSimple->selectedItems().isEmpty()) {
        ui->lwMediathequeGetSimple->removeItemWidget(ui->lwMediathequeGetSimple->selectedItems().at(0));
    }
}

void AbulEduMediathequeGetV1::abeHideBoutonTelecharger()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->btnTelecharger->hide();
}

void AbulEduMediathequeGetV1::abeHideInfoPanel(bool hide)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(hide) {
        ui->AbulEduMediathequeGetV1InfoPanel->hide();
    }
    else {
        ui->AbulEduMediathequeGetV1InfoPanel->show();
    }
}

void AbulEduMediathequeGetV1::abeSetCustomBouton1Download(bool enable)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(enable) {
        connect(ui->btnCustom1,SIGNAL(clicked()),ui->btnTelecharger,SLOT(click()),Qt::UniqueConnection);
    }
    else {
        disconnect(ui->btnCustom1,SIGNAL(clicked()),ui->btnTelecharger,SLOT(click()));
    }
}

void AbulEduMediathequeGetV1::abeCustomBouton1SetDownload(bool enable)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeSetCustomBouton1Download(enable);
}

void AbulEduMediathequeGetV1::abeStartDownload()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->btnTelecharger->click();
}

void AbulEduMediathequeGetV1::abeClear(bool clearMotsCles)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->twMediathequeGetComplet->setSortingEnabled(false);
    ui->twMediathequeGetComplet->clearContents();
    ui->twMediathequeGetComplet->setRowCount(0);
    ui->lwMediathequeGetSimple->clear();

    ui->AbulEduMediathequeGetV1teMotsCles->clear();
    ui->teResume->clear();
    ui->btnImage->setIconeNormale("");
    ui->btnImage->update();
    ui->lblRessourceIDValue->clear();
    ui->AbulEduMediathequeGetV1lblTitre->clear();
    if(clearMotsCles) {
        ui->lwRechercheMotsCles->clear();
        ui->lwRechercheHierarchie->clear();
    }
}

void AbulEduMediathequeGetV1::abeSetSearchFileMime(const AbulEduMediathequeGetV1::enumMediathequeSearchFileMimes& type)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_searchFileMime = type;
}

void AbulEduMediathequeGetV1::rpcCheckDoublon(QVariant &v, QNetworkReply *reply)
{
    Q_UNUSED(reply);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__
                        << "============================="
                        << v
                        << "=============================";

    AbulEduLoadingAnimationV1::getInstance()->stop(ui->AbulEduMediathequeGetV1ThumbnailPage);
    ui->AbulEduMediathequeGetV1InfoPanel->setCurrentWidget(ui->AbulEduMediathequeGetV1ThumbnailPage);
    QApplication::restoreOverrideCursor();
    QString monItem = v.toMap().value("docId").toString();
    if(monItem != "-1") {
        ABULEDU_LOG_DEBUG() << monItem;

        QMessageBox::warning(this, trUtf8("abuledu-data"),
                             trUtf8("Attention il semblerait que cette ressource ait déjà été indexée !\nVérifiez à l'aide de la recherche..."),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }
}

void AbulEduMediathequeGetV1::on_lwRechercheMotsCles_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete item;

    //On supprime les mots cles de la hierarchie si on est de retour à zéro
    if(ui->lwRechercheMotsCles->count() == 0) {
        ui->lwRechercheHierarchie->clear();
    }

    slotLanceRecherche();
}

void AbulEduMediathequeGetV1::on_lwRechercheHierarchie_itemClicked(QListWidgetItem *item)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->lwRechercheMotsCles->clear();
    ui->leRecherche->setText(item->text());
    on_btnRecherche_clicked();
}

void AbulEduMediathequeGetV1::on_lwMediathequeGetSimple_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    on_btnTelecharger_clicked();
}

void AbulEduMediathequeGetV1::on_twMediathequeGetComplet_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    on_btnTelecharger_clicked();
}

void AbulEduMediathequeGetV1::on_btnImage_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString uri;
    QString mime;
    if(ui->stackedWidget->currentWidget() == ui->pageResultats && !ui->twMediathequeGetComplet->selectedItems().isEmpty()) {
        uri  = ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+9).toString();
        mime = ui->twMediathequeGetComplet->selectedItems().at(0)->data(Qt::UserRole+10).toString();
    }
    else if(ui->stackedWidget->currentWidget() == ui->pageResultatsIcones && !ui->lwMediathequeGetSimple->selectedItems().isEmpty()) {
        uri  = ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+9).toString();
        mime = ui->lwMediathequeGetSimple->selectedItems().at(0)->data(Qt::UserRole+10).toString();
    }

    //Côte serveur on calcule toutes les images en 512 au format jpeg
    m_visio->ouvrirFicher(QUrl("http://data-cache.abuledu.org/512/" + uri.replace(".abb",".jpg")));
#ifdef __ABULEDUTABLETTEV1__MODE__
    m_visio->showFullScreen();
#else
    m_visio->show();
#endif
}

void AbulEduMediathequeGetV1::on_tbClose_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMediathequeGetCloseOrHide();
}

void AbulEduMediathequeGetV1::on_leRecherche_editingFinished()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->btnRecherche->setFocus();
}
