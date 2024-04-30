 /** MicroTexte pour Tablette
  *
  * @warning aucun traitement d'erreur n'est pour l'instant implémenté
  * @see https://redmine.ryxeo.com/projects/abuledu-microtexte
  * @author 2011 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2012-2014 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2013-2014 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2013-2014 Icham Sirat <icham.sirat@ryxeo.com>
  * @see The GNU Public License (GPL)
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractPrintDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_QuitOnClose);

    ui->setupUi(this);
    m_hauteurToolBar = 48;

    m_isCloseRequested  = false;
    m_isNewFile         = true;
    m_wantNewFile       = false;
    m_wantOpenFile      = false;
    setWindowFlags(Qt::FramelessWindowHint);

#ifdef Q_OS_WIN
    switch(QSysInfo::windowsVersion())
    {
    case QSysInfo::WV_2000: ABULEDU_LOG_DEBUG()<< "Windows 2000";break;
    case QSysInfo::WV_XP: ABULEDU_LOG_DEBUG()<< "Windows XP";break;
    case QSysInfo::WV_VISTA: ABULEDU_LOG_DEBUG()<< "Windows Vista";break;
    case QSysInfo::WV_WINDOWS7: ABULEDU_LOG_DEBUG()<< "Windows Seven";break;
    case QSysInfo::WV_WINDOWS8: ABULEDU_LOG_DEBUG()<< "Windows 8";break;
    default: ABULEDU_LOG_DEBUG()<< "Windows";break;
    }
#endif

    installTranslator();

    /***************************** AbeMediatheque ***************************************/
    ui->abeMediathequeGet->abeSetSourceEnum(AbulEduMediathequeGetV1::abeData);
    ui->abeMediathequeGet->abeHideBoutonTelecharger();
    ui->abeMediathequeGet->abeSetCustomBouton1(trUtf8("Insérer l'image"));
    ui->abeMediathequeGet->abeSetCustomBouton1Download(true);
    ui->abeMediathequeGet->abeSetDefaultView(AbulEduMediathequeGetV1::abeMediathequeThumbnails);

    /* Attention au cas où il n'y a pas de réponse, on est bloqué à un endroit du stackedWidget */
    connect(ui->abeMediathequeGet, SIGNAL(signalMediathequeFileDownloaded(QSharedPointer<AbulEduFileV1>,int)), this, SLOT(slotMediathequeDownload(QSharedPointer<AbulEduFileV1>,int)),Qt::UniqueConnection);
    connect(ui->abeMediathequeGet, SIGNAL(signalAbeMediathequeGetCloseOrHide()),this, SLOT(showTextPage()),Qt::UniqueConnection);

    m_abuledufile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1, &QObject::deleteLater);
    setCurrentFileName(m_abuledufile->abeFileGetDirectoryTemp().absolutePath() + "/document.html");

    /***************************** AbeBoxFileManager ***************************************/
    ui->abeBoxFileManager->abeMediathequeGetHideCloseBouton(true);
    connect(ui->abeBoxFileManager, SIGNAL(signalAbeFileSelected(QSharedPointer<AbulEduFileV1>)), this, SLOT(slotOpenFile(QSharedPointer<AbulEduFileV1>)), Qt::UniqueConnection);
    connect(ui->abeBoxFileManager, SIGNAL(signalAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation,QString,bool)),
            this, SLOT(slotAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation,QString,bool)), Qt::UniqueConnection);
    connect(ui->abeBoxFileManager, SIGNAL(signalAbeFileCloseOrHide()),this, SLOT(showTextPage()), Qt::UniqueConnection);
    connect(ui->abeBoxFileManager, SIGNAL(signalAbeFileSelected(QSharedPointer<AbulEduFileV1>)), this, SLOT(showTextPage()), Qt::UniqueConnection);

    connect(ui->teZoneTexte->document(), SIGNAL(modificationChanged(bool)), this, SLOT(setWindowModified(bool)), Qt::UniqueConnection);
    //    /* On émet un signal inquant si le texte a été modifié */
    //    connect(ui->teZoneTexte->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(somethingHasChangedInText(bool)), Qt::UniqueConnection);

    /* Le curseur a été déplacé*/
    connect(ui->teZoneTexte, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorMoved()), Qt::UniqueConnection);

    initMultimedia();
    initSignalMapperFontChange();
    initSignalMapperFormFontChange();

    /***************************** Chargement des Fonts ***************************************/
    QFontDatabase fonts;
    if(!fonts.addApplicationFont(":/abuledutextev1/Ecolier")) {
        ABULEDU_LOG_DEBUG() << "Erreur sur :/fonts/ECOLIER.TTF";
    }
    if(!fonts.addApplicationFont(":/abuledutextev1/Cursive")) {
        ABULEDU_LOG_DEBUG() << "Erreur sur :/fonts/CURSIVE.TTF";
    }

#ifndef QT_NO_PRINTER
    /* Gestion Impression */
    m_printer = new QPrinter(QPrinter::HighResolution);
    m_printDialog = new QPrintDialog(m_printer, this);
    m_printDialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    m_printDialog->setStyleSheet("background-color:#FFFFFF");
    /* #4078 -> impossible d'integrer QPrintDialog dans un widget sous windows */
#ifndef Q_OS_WIN
    ui->glPrint->addWidget(m_printDialog);
    connect(m_printDialog, SIGNAL(rejected()), this, SLOT(showTextPage()), Qt::UniqueConnection);
    connect(m_printDialog, SIGNAL(accepted(QPrinter*)), this, SLOT(filePrint(QPrinter*)), Qt::UniqueConnection);
#endif
#endif
#ifndef __ABULEDUTABLETTEV1__MODE__
    /* On Centre la fenetre */
    abeApp->abeCenterWindow(this);
    ui->teZoneTexte->setFocus();
#endif

    /***************************** Gestion du retour de la page à propos ***************************************/
    connect(ui->pageAbout, SIGNAL(signalAbeAproposBtnCloseClicked()), this, SLOT(showTextPage()),Qt::UniqueConnection);

    /***************************** Page par défaut ***************************************/
    ui->stackedWidget->setCurrentWidget(ui->pageTexte);

    /***************************** Font par défaut ***************************************/
    connect(ui->teZoneTexte, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(slotCurrentCharFormatChanged(QTextCharFormat)));
    m_fontSize = 30;            /* taille par defaut */

    m_textCharFormat = ui->teZoneTexte->textCursor().charFormat();

    /* Connexion des boutons */
    connect(ui->btnNewMicroTexte, SIGNAL(clicked()), SLOT(on_abeMenuFeuilleBtnNew_clicked()), Qt::UniqueConnection);
    connect(ui->btnOpenMicroTexte, SIGNAL(clicked()), SLOT(on_abeMenuFeuilleBtnOpen_clicked()), Qt::UniqueConnection);
    connect(ui->btnSaveMicroTexte, SIGNAL(clicked()), SLOT(on_abeMenuFeuilleBtnSave_clicked()), Qt::UniqueConnection);
    connect(ui->btnMajusculeMicroTexte, SIGNAL(clicked()), SLOT(slotFontCaps()), Qt::UniqueConnection);
    connect(ui->btnMinusculeMicroTexte, SIGNAL(clicked()), SLOT(slotFontLower()), Qt::UniqueConnection);
    // connect(ui->btnCursiveMicroTexte, SIGNAL(clicked()), SLOT(slotChangeFont(QString)));
    connect(ui->btnDecreasePoliceMicroTexte, SIGNAL(clicked()), SLOT(on_btn_decrease_clicked()), Qt::UniqueConnection);
    connect(ui->btnIncreasePoliceMicroTexte, SIGNAL(clicked()), SLOT(on_btn_increase_clicked()), Qt::UniqueConnection);
    connect(ui->btnPrintMicroTexte, SIGNAL(clicked()), SLOT(on_abeMenuFeuilleBtnPrint_clicked()), Qt::UniqueConnection);
    connect(ui->btnHelpMicroTexte, SIGNAL(clicked()), SLOT(on_abeMenuFeuilleBtnHelp_clicked()), Qt::UniqueConnection);

    connect(ui->btnQuitMicroTexte, SIGNAL(clicked()), SLOT(on_abeMenuFeuilleBtnQuit_clicked()), Qt::UniqueConnection);

    /* Caps par defaut */
    ui->btnMajusculeMicroTexte->click();

    setWindowModified(false);
}

void MainWindow::initMultimedia()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    m_multimedia = new AbulEduMultiMediaV1(AbulEduMultiMediaV1::Sound, ui->frmControlAudio);
    m_multimedia->abeMultiMediaGetAudioControlWidget()->abeControlAudioSetSpeedControlVisible(true);
    m_multimedia->abeMultiMediaGetAudioControlWidget()->abeControlAudioGetFrameSpeed()->setStyleSheet("color:#0a73f4;");
    m_multimedia->abeMultiMediaSetButtonVisible(AbulEduMultiMediaV1::BtnMagnifyingGlass | AbulEduMultiMediaV1::BtnPrevious | AbulEduMultiMediaV1::BtnNext | AbulEduMultiMediaV1::BtnHide | AbulEduMultiMediaV1::BtnRecord,false);
    m_multimedia->abeMultiMediaGetAudioControlWidget()->abeControlAudioSetDirection(QBoxLayout::TopToBottom);
    m_multimedia->abeMultiMediaForceStop();
    m_multimedia->abeMultiMediaSetTextVisible(false);
    connect(m_multimedia->abeMultiMediaGetAudioControlWidget(), SIGNAL(signalAbeControlAudioPlayClicked()),this, SLOT(slotReadContent()),Qt::UniqueConnection);

    /** @todo autres langues ? */
    if(m_multimedia->abeMultiMediaGetTTSlang() != AbulEduPicottsV1::fr){
        m_multimedia->abeMultimediaSetTTS(AbulEduPicottsV1::fr);
    }
}

void MainWindow::initSignalMapperFontChange()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    m_signalMapperFontChange = new QSignalMapper(this);
    connect(m_signalMapperFontChange, SIGNAL(mapped(QString)), SLOT(slotChangeFont(QString)), Qt::UniqueConnection);
}

void MainWindow::initSignalMapperFormFontChange()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    m_signalMapperFontFormChange = new QSignalMapper(this);
    connect(m_signalMapperFontFormChange, SIGNAL(mapped(QString)), SLOT(slotChangeFormFont(QString)), Qt::UniqueConnection);
}

void MainWindow::installTranslator()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    m_locale = QLocale::system().name().section('_', 0, 0);
    myappTranslator.load("abuledu-microtexte_"+m_locale, "./lang");
    abeApp->installTranslator(&myappTranslator);
    /* pour avoir les boutons des boîtes de dialogue dans la langue locale (fr par défaut) */
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    abeApp->installTranslator(&qtTranslator);
}

MainWindow::~MainWindow()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    delete ui;
    m_abuledufile->abeClean();
}

QTextDocument *MainWindow::abeTexteGetDocument()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    return ui->teZoneTexte->document();
}

bool MainWindow::fileSave()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    setCurrentFileName(m_abuledufile->abeFileGetDirectoryTemp().absolutePath() + "/document.html");

        ABULEDU_LOG_DEBUG() << "Ecriture dans le fichier " << m_fileName;

    QFileInfo fi(m_fileName);

    QTextDocumentWriter writer(fi.absoluteFilePath(),"HTML");
    bool success = writer.write(ui->teZoneTexte->document());
    if (success)
        ui->teZoneTexte->document()->setModified(false);

    /* Le 1er fichier de la liste, c'est le fichier document maitre html */
    QStringList liste(m_fileName);
    /* Parcours du repertoire data pour enquiller tous les autres fichiers */
    QDir dir(fi.absolutePath() + "/data/");
    /* Attention a ne pas prendre le repertoire "." et ".." */
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        if(fileInfo.isFile()) {
            liste << fileInfo.absoluteFilePath();
        }
    }

    m_abuledufile->abeFileExportPrepare(liste, fi.absolutePath(), "abe");

    if (m_isNewFile) {
        fileSaveAs();
    }

    ui->abeBoxFileManager->abeSetOpenOrSaveEnum(AbulEduBoxFileManagerV1::abeSave);
    ui->stackedWidget->setCurrentWidget(ui->pageBoxFileManager);

    return success;
}

bool MainWindow::fileSaveAs()
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    m_isNewFile = false;
    return fileSave();
}

void MainWindow::setCurrentFileName(const QString &fileName)
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    m_fileName = fileName;
    /* Comme le nom vient de changer, c'est que le fichier vient d'être crée ou vient d'être sauvegardé */
    ui->teZoneTexte->document()->setModified(false);

    QDir rep(m_abuledufile->abeFileGetDirectoryTemp().absolutePath());
    if(!rep.exists()) {
        rep.mkpath(m_abuledufile->abeFileGetDirectoryTemp().absolutePath());
    }

    QString shownName;
    if (fileName.isEmpty()) {
        shownName = trUtf8("Sans nom") +"[*]";
    }
    else {
        shownName = QFileInfo(m_fileName).fileName() + "[*]";
    }

    /* Au cas ou le widget serait un topLevelWidget() */
    setWindowTitle(shownName);
    /* On émet un signal avec le nom du fichier suivi de [*] pour affichage dans titre de fenêtre */
    emit fileNameHasChanged(shownName);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    ABULEDU_LOG_DEBUG() << __PRETTY_FUNCTION__;

    if(!isWindowModified() || (m_isNewFile && ui->teZoneTexte->toPlainText().isEmpty())){
        e->accept();
        return;
    }
    else
    {
        e->ignore();
        m_isCloseRequested = true;
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Enregistrer le projet"),trUtf8("Le projet comporte des modifications non enregistrées. Voulez-vous sauvegarder ?"),true,ui->stackedWidget->currentWidget());
        msg->abeSetModeEnum(AbulEduMessageBoxV1::abeYesNoCancelButton);
        /* Je commente le code ci-dessous tant que je n'ai pas pushé la modif dans la lib AbulEduMessageBoxV1 */

        /* Normalement, et jusqu'à qu'on ait décidé où mettre le fichier ogg, le code ci-dessous, parce qu'il ne trouvera pas le ogg, devrait provoquer la synthèse vocale */
        QString soundPath = QString();
//        if(QFile(abeApp->applicationDirPath() + "/data/sons/confirmationFermeture.ogg").exists()){
//            soundPath = abeApp->applicationDirPath() + "/data/sons/confirmationFermeture.ogg";
//        }
        ABULEDU_LOG_DEBUG()<<soundPath;
        msg->abeMessageBoxSetMultimedia(soundPath);
        msg->show();
        connect(msg,SIGNAL(signalAbeMessageBoxYES()),SLOT(on_abeMenuFeuilleBtnSave_clicked()),Qt::UniqueConnection);
        connect(msg,SIGNAL(signalAbeMessageBoxNO()),SLOT(deleteLater()),Qt::UniqueConnection);
    }
}

#ifndef QT_NO_PRINTER
void MainWindow::filePrint(QPrinter *printer)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* On imprime */
    ui->teZoneTexte->print(printer);
    /* On affiche un message */
    QString message("Impression en cours");
    AbulEduMessageBoxV1* msgImpression = new AbulEduMessageBoxV1(trUtf8("Impression"), message,true,ui->pagePrint);
    msgImpression->setWink();
    msgImpression->show();
    connect(msgImpression, SIGNAL(signalAbeMessageBoxCloseOrHide()), this, SLOT(showTextPage()), Qt::UniqueConnection);
}
#endif

void MainWindow::slotMediathequeDownload(QSharedPointer<AbulEduFileV1> abeFile, int code)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    Q_UNUSED(code)
    QString file = abeFile->abeFileGetContent(0).absoluteFilePath();
    QString filename = abeFile->abeFileGetContent(0).baseName() + ".png";

    ABULEDU_LOG_DEBUG() << "  slotMediathequeDownload : " << file << " et " << filename;

    QUrl Uri ( QString ( "mydata://data/%1" ).arg ( filename ) );
    QImage image = QImageReader ( file ).read().scaledToWidth(150,Qt::SmoothTransformation);

    QFileInfo fi(m_fileName);
    QString imageDest = QString("%1/data/%2").arg(fi.absolutePath()).arg(filename);
    //    Uri.setUrl(imageDest);

    QTextDocument * textDocument = ui->teZoneTexte->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    QTextCursor cursor = ui->teZoneTexte->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );

    QDir rep(fi.absolutePath() + "/data/");
    if(!rep.exists()) {
        rep.mkpath(fi.absolutePath() + "/data/");
    }
    if(!image.save(imageDest)) {
        //        if (m_localDebug) qDebug() << "******* ERREUR de sauvegarde de " << imageDest;
    }
    ABULEDU_LOG_DEBUG() << "Sauvegarde de l'image dans " << imageDest;

    imageFormat.setName(Uri.toString());
    cursor.insertImage(imageFormat);
    cursor.insertText("\n");

    //Les sources et l'auteur (?)
    QTextListFormat listFormat;
    cursor.insertList(listFormat);
    QTextCharFormat fmt;
    fmt.setFontItalic(true);
    cursor.insertText("Source: " + abeFile->abeFileGetIdentifier() + "\n",fmt);
    cursor.insertText("Auteur: " + abeFile->abeFileGetCreator(),fmt);

    /* Retour normal */
    QTextBlockFormat blockFormat;
    fmt.setFontItalic(false);
    cursor.insertBlock(blockFormat,fmt);

    ui->stackedWidget->setCurrentWidget(ui->pageTexte);
}

void MainWindow::fileOpen()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->abeBoxFileManager->abeSetOpenOrSaveEnum(AbulEduBoxFileManagerV1::abeOpen);
    ui->abeBoxFileManager->abeRefresh(AbulEduBoxFileManagerV1::abePC);
    ui->stackedWidget->setCurrentWidget(ui->pageBoxFileManager);
}

void MainWindow::slotOpenFile(QSharedPointer<AbulEduFileV1> abeFile)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__  << abeFile;

    /* Correction du bug de non apparition de l'image */
    if(abeFile)
    {
        m_abuledufile = abeFile;
    }
        ABULEDU_LOG_DEBUG() << "Ouverture du fichier " << m_abuledufile->abeFileGetFileName().filePath();
        ABULEDU_LOG_DEBUG()<<" dont le repertoire temporaire est "<<m_abuledufile->abeFileGetDirectoryTemp().absolutePath();
        ABULEDU_LOG_DEBUG()<<m_fileName;

    m_isNewFile = false;
    setCurrentFileName(m_abuledufile->abeFileGetContent(0).absoluteFilePath());

    /* lecture du fichier html */
    QFile  htmlFile(m_fileName);
    if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QString htmlContent;
    QTextStream in(&htmlFile);
    while (!in.atEnd()) {
        htmlContent.append(in.readLine());
    }

    QTextDocument *document = new QTextDocument();
    document->setHtml(htmlContent);
    ui->teZoneTexte->setDocument(document);

    /* chargement des ressources dans le textDocument... */
    QTextDocument * textDocument = ui->teZoneTexte->document();
    QStringList liste = m_abuledufile->abeFileGetFileList();
    for(int i = 0; i < liste.size(); i++) {
        QFileInfo fi(liste.at(i));
        if(fi.suffix() == "png") {
            QUrl Uri ( QString ( "mydata://data/%1" ).arg ( fi.fileName() ) );
            QImage image = QImageReader(fi.absoluteFilePath()).read();
            textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
            ABULEDU_LOG_DEBUG() << " ++ " << fi.absoluteFilePath() << " en tant que " << Uri;
        }
    }
    ui->teZoneTexte->update();
    setWindowModified(false);
}

void MainWindow::slotAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation location, QString fileName, bool success)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << fileName << " et " << success;

    QString emplacement;
    switch (location) {
    case AbulEduBoxFileManagerV1::abePC:
        emplacement = trUtf8("votre ordinateur");
        break;
    case AbulEduBoxFileManagerV1::abeBoxPerso:
        emplacement = trUtf8("votre abeBox personnelle");
        break;
    case AbulEduBoxFileManagerV1::abeBoxShare:
        emplacement = trUtf8("une abeBox partagée");
        break;
    case AbulEduBoxFileManagerV1::abeMediatheque:
        emplacement = trUtf8("AbulEdu-Médiathèque");
        break;
    default:
        emplacement = trUtf8("un endroit inconnu");
        break;
    }
    QString message;
    if(success)
    {
        message = trUtf8("Votre fichier a été enregistré dans ")+emplacement;
        if (!fileName.isEmpty()){
            message.append(trUtf8(" sous le nom : ")+fileName.split("/").last());
        }
    }
    else{
        message = trUtf8("Votre fichier n'a pas pu être enregistré...");
    }
    AbulEduMessageBoxV1* msgEnregistrement = new AbulEduMessageBoxV1(trUtf8("Enregistrement"), message,true,ui->pageBoxFileManager);
    if(!m_wantOpenFile){
        connect(msgEnregistrement, SIGNAL(signalAbeMessageBoxCloseOrHide()), this, SLOT(showTextPage()), Qt::UniqueConnection);
    }
    if(success)
    {
        msgEnregistrement->setWink();
    }
    if(m_isCloseRequested)
    {
        connect(msgEnregistrement,SIGNAL(signalAbeMessageBoxCloseOrHide()),this,SLOT(deleteLater()),Qt::UniqueConnection);
    }
    msgEnregistrement->show();
    if(m_wantNewFile){
        slotClearCurrent();
        m_wantNewFile = false;
    }
    if(m_wantOpenFile){
        fileOpen();
        m_wantOpenFile = false;
    }
}

void MainWindow::on_abeMenuFeuilleBtnPrint_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

#ifndef QT_NO_PRINTER
    if(!m_printDialog->isVisible()){
        m_printDialog->showNormal();
    }
#endif
#ifndef Q_OS_WIN32
    /* #4078 -> impossible d'integrer QPrintDialog dans un widget sous windows */
    ui->stackedWidget->setCurrentWidget(ui->pagePrint);
#endif
}

void MainWindow::on_abeMenuFeuilleBtnHelp_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->stackedWidget->setCurrentWidget(ui->pageAbout);
}

void MainWindow::on_abeMenuFeuilleBtnQuit_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    close();
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << "page courante : "<< arg1 << ui->stackedWidget->widget(arg1)->objectName();

    /* #3932 Barre d'outils cachée lorsqu'on est pas sur la pageTexte */
    if(ui->stackedWidget->widget(arg1)->objectName() != "pageTexte") {
        ui->frTopMicroTexte->setVisible(false);
    }
    else{
        ui->frTopMicroTexte->setVisible(true);
    }
}

void MainWindow::slotClearCurrent()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Je veux faire un nouveau texte, mais je ne veux pas changer d'abe */
    m_abuledufile->abeCleanDirectoryRecursively(m_abuledufile->abeFileGetDirectoryTemp().absolutePath());
    ui->teZoneTexte->clear();
    setWindowModified(false);
}

void MainWindow::on_abeMenuFeuilleBtnOpen_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(isWindowModified()){
        m_wantOpenFile = true;
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Ouvrir un projet"),trUtf8("Le projet actuel comporte des modifications non enregistrées. Voulez-vous sauvegarder ?"),true,ui->stackedWidget->currentWidget());
        msg->abeSetModeEnum(AbulEduMessageBoxV1::abeYesNoCancelButton);
        msg->show();
        connect(msg,SIGNAL(signalAbeMessageBoxYES()),SLOT(on_abeMenuFeuilleBtnSave_clicked()),Qt::UniqueConnection);
        connect(msg,SIGNAL(signalAbeMessageBoxNO()),SLOT(fileOpen()),Qt::UniqueConnection);
    }
    else{
        fileOpen();
    }
    return;
}

void MainWindow::on_abeMenuFeuilleBtnSave_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Je n'enregistre pas si la zone de texte est vide */
    if(ui->teZoneTexte->toPlainText().isEmpty()) return;

    if (fileSave()){
        ui->stackedWidget->setCurrentWidget(ui->pageBoxFileManager);
        ui->abeBoxFileManager->abeSetOpenOrSaveEnum(AbulEduBoxFileManagerV1::abeSave);
        ui->abeBoxFileManager->abeSetFile(m_abuledufile);
        ui->abeBoxFileManager->abeRefresh(AbulEduBoxFileManagerV1::abePC);
    }
    setWindowModified(false);
}

void MainWindow::on_abeMenuFeuilleBtnNew_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(isWindowModified()){
        m_wantNewFile = true;
        AbulEduMessageBoxV1* msg = new AbulEduMessageBoxV1(trUtf8("Nouveau projet"),trUtf8("Le projet comporte des modifications non enregistrées. Voulez-vous sauvegarder ?"),true,ui->stackedWidget->currentWidget());
        msg->abeSetModeEnum(AbulEduMessageBoxV1::abeYesNoCancelButton);
        /* Je commente le code ci-dessous tant que je n'ai pas pushé la modif dans la lib AbulEduMessageBoxV1 */

        /* Normalement, et jusqu'à qu'on ait décidé où mettre le fichier ogg, le code ci-dessous, parce qu'il ne trouvera pas le ogg, devrait provoquer la synthèse vocale */
        QString soundPath = QString();
        if(QFile(abeApp->applicationDirPath() + "/data/sons/confirmationFermeture.ogg").exists()){
            soundPath = abeApp->applicationDirPath() + "/data/sons/confirmationFermeture.ogg";
        }
        ABULEDU_LOG_DEBUG()<<soundPath;
        msg->abeMessageBoxSetMultimedia(soundPath);
        msg->show();
        connect(msg,SIGNAL(signalAbeMessageBoxYES()),SLOT(on_abeMenuFeuilleBtnSave_clicked()),Qt::UniqueConnection);
        connect(msg,SIGNAL(signalAbeMessageBoxNO()),SLOT(slotClearCurrent()),Qt::UniqueConnection);    }
    else{
        slotClearCurrent();
    }
    return;
}

void MainWindow::showAbeMediathequeGet()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->abeMediathequeGet->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->pageMediathequeGet);
}

void MainWindow::showTextPage()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    ui->stackedWidget->setCurrentWidget(ui->pageTexte);
}

void MainWindow::slotChangeLangue(const QString &lang)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << " en "<<lang;

    /* #3766 : le retranslateUi() efface tout le texte */
    const QString textToReplace = ui->teZoneTexte->document()->toHtml();

    qApp->removeTranslator(&qtTranslator);
    qApp->removeTranslator(&myappTranslator);

    qtTranslator.load("qt_" + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);
    myappTranslator.load("abuledu-microtexte_" + lang, "lang");
    qApp->installTranslator(&myappTranslator);
    ui->retranslateUi(this);

    ui->teZoneTexte->setHtml(textToReplace);
}

void MainWindow::slotSessionAuthenticated(bool enable)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << enable;

    if(enable)
        abeApp->getAbeNetworkAccessManager()->abeSSOLogin();
}

void MainWindow::slotReadContent()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ ;

    (ui->teZoneTexte->textCursor().hasSelection()) ? (m_textToSpeech = ui->teZoneTexte->textCursor().selectedText()) : (m_textToSpeech = ui->teZoneTexte->toPlainText());

    if(m_textToSpeech.isEmpty())
        return;

    m_multimedia->abeMultiMediaSetNewMedia(AbulEduMediaMedias(QString(),QString(),m_textToSpeech));
    m_multimedia->abeMultiMediaPlay();
    m_textToSpeech.clear();
}

/*************************************************************************************************************************************
 *
 *  GESTION CHANGEMENT POLICE
 *
 * ***********************************************************************************************************************************/
void MainWindow::slotChangeFont(const QString &font)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << font << ui->teZoneTexte->textCursor().charFormat();

    m_textCharFormat.setFontFamily(font);
    m_textCharFormat.setFont(font);
    m_textCharFormat.setFontPointSize(m_fontSize);

    if(ui->btnMajusculeMicroTexte->isChecked())
        m_textCharFormat.setFontCapitalization(QFont::AllUppercase);
    else if(ui->btnMinusculeMicroTexte->isChecked())
        m_textCharFormat.setFontCapitalization(QFont::AllLowercase);

    mergeFormatOnWordOrSelection(m_textCharFormat);
}

void MainWindow::slotChangeFormFont(const QString &form)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__  << form;
    /* On l'applique */
    mergeFormatOnWordOrSelection(m_textCharFormat);
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QTextCursor cursor = ui->teZoneTexte->textCursor();
    //    if (cursor.hasSelection())
    //        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->teZoneTexte->mergeCurrentCharFormat(format);
}

void MainWindow::slotCurrentCharFormatChanged(QTextCharFormat tcf)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ /*<< tcf.fontFamily() << m_textCharFormat.fontFamily() << ui->teZoneTexte->currentFont()*/ ;

    /*  Ici gérer les changements de police et repercuter sur interface */
    if (tcf.fontFamily() == "Cursive standard" ){
        ui->btnCursiveMicroTexte->setChecked(true);
    }
    ABULEDU_LOG_DEBUG() << "Alignement : " <<ui->teZoneTexte->alignment();
    ABULEDU_LOG_DEBUG() << "Color : " <<ui->teZoneTexte->textCursor().charFormat().foreground().color();

    /* Repercussions Majuscule/Minuscule/Cursive Microtexte */
    if(ui->teZoneTexte->textCursor().charFormat().fontCapitalization()== QFont::AllUppercase){
        ABULEDU_LOG_DEBUG() << "Test CAPS OK";
        ui->btnMajusculeMicroTexte->setChecked(true);
    }
    else if(ui->teZoneTexte->textCursor().charFormat().fontCapitalization() == QFont::AllLowercase){
        ABULEDU_LOG_DEBUG() << "Test LOWER OK";
        ui->btnMinusculeMicroTexte->setChecked(true);
    }

    if(/*m_textCharFormat.fontFamily() != tcf.fontFamily() && */ui->teZoneTexte->toPlainText().isEmpty()){
        ABULEDU_LOG_DEBUG() << "+++++++++++++++++++++++++++++++   +++++++++++++++++++++++++  " << "C'est mon cas ";
        mergeFormatOnWordOrSelection(m_textCharFormat);
        //        m_textCharFormat = tcf;
    }
}

void MainWindow::slotFontCaps()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    QTextCharFormat tcf;
    //    m_textCharFormat.setFontCapitalization(QFont::AllUppercase);
    //    mergeFormatOnWordOrSelection(m_textCharFormat);
    slotChangeFont("Andika");
}

void MainWindow::slotFontLower()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    QTextCharFormat tcf;
    //    m_textCharFormat.setFontCapitalization(QFont::AllLowercase);
    //    mergeFormatOnWordOrSelection(m_textCharFormat);
    slotChangeFont("Andika");
}

void MainWindow::on_teZoneTexte_textChanged()
{
    //no debug : flood
    //    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ ;
    if(!isWindowModified() && !ui->teZoneTexte->document()->isEmpty()) {
        setWindowModified(true);
    }
}

void MainWindow::slotChangeFontSize(int newSize)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << newSize ;
    qreal pointSize = newSize;
    if(newSize > 0){
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::on_btn_increase_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_fontSize += 2;
    slotChangeFontSize(m_fontSize);
}

void MainWindow::on_btn_decrease_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_fontSize -= 2;
    slotChangeFontSize(m_fontSize);
}

void MainWindow::on_btnCursiveMicroTexte_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    slotChangeFont("Cursive standard");
}
