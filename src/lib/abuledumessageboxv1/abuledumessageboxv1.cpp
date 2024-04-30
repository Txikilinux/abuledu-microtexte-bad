/** Classe AbulEduMessageBoxV1
  *
  * @author 2012-2016 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @author 2013 Éric Seigne <eric.seigne@ryxeo.com>
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

#include "abuledumessageboxv1.h"

#ifndef DEBUG_ABULEDUMESSAGEBOXV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMessageBoxV1::AbulEduMessageBoxV1(const QString &titre, const QString &texte, bool doYouWantToClose, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbulEduMessageBoxV1)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeApp->installEventFilter(this);

    m_parent = parent;
    m_hasToBeClosed = doYouWantToClose;

    if(m_hasToBeClosed) {
        setAttribute(Qt::WA_DeleteOnClose);
    }
    m_isWink = false;
    premierAffichage = false;
    ui->setupUi(this);

    ui->lblTitre->setFont(QFont(abeApp->font().family(),ui->lblTitre->font().pointSize()));
    ui->lblTitre->setText(titre);
    ui->lblTexte->setFont(QFont(abeApp->font().family(),ui->lblTexte->font().pointSize()));
    ui->lblTexte->setText(texte);
    setAttribute( Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    m_config = new QSettings(":/abuledumessageboxv1/settings/conf", QSettings::IniFormat,this);
    setTitleColor(QColor(m_config->value("colors/title").toString()));
    setTextColor(QColor(m_config->value("colors/text").toString()));
    if(!m_config->value("colors/background").toString().isEmpty()) {
        setBackgroundColor(QColor(m_config->value("colors/background").toString()));
    }
    else {
        setBackgroundColor(QColor("#e3e4ed"));
    }

//    setAttribute(Qt::WA_DeleteOnClose);

    if(!m_config->value("colors/btnFermerTexteNormal").toString().isEmpty()) {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            btn->setCouleurTexteNormale(m_config->value("colors/btnFermerTexteNormal").toString());
        }
    }
    else {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            btn->setCouleurTexteNormale(QColor(233,45,0));
        }
    }
    if(!m_config->value("colors/btnFermerTexteSurvol").toString().isEmpty()) {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            btn->setCouleurTexteSurvol(m_config->value("colors/btnFermerTexteSurvol").toString());
        }
    }
    else {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            btn->setCouleurTexteSurvol(QColor(249,80,0));
        }
    }
    if(!m_config->value("colors/btnFermerTextePressed").toString().isEmpty()) {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            btn->setCouleurTextePressed(m_config->value("colors/btnFermerTextePressed").toString());
        }
    }
    else {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            btn->setCouleurTextePressed(QColor(249,80,0));
        }
    }

    if(!m_config->value("colors/btnFermerFondNormal").toString().isEmpty()) {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            /* Dans la théorie, QColor est sensé accepter l'hexadécimal AVEC canal alpha. Dans la pratique, les nouilles ci-dessous */
            QString stringColor = m_config->value("colors/btnFermerFondNormal").toString();
            QString alpha;
            if(stringColor.remove("#").length() == 8) {
                alpha = stringColor.left(2);
                stringColor = stringColor.right(6);
            }
            stringColor.prepend("#");
            QColor col(stringColor);
            bool ok;
            int hex = alpha.toInt(&ok, 16);
            if(ok) {
                col.setAlpha(hex);
            }
            btn->setCouleurFondNormale(col);
            btn->setCouleurFondSurvol(col);
        }
    }

    if(!m_config->value("colors/btnFermerFondPressed").toString().isEmpty()) {
        foreach(AbulEduFlatBoutonV1* btn,ui->mainFrame->findChildren<AbulEduFlatBoutonV1*>()) {
            /* Dans la théorie, QColor est sensé accepter l'hexadécimal AVEC canal alpha. Dans la pratique, les nouilles ci-dessous */
            QString stringColor = m_config->value("colors/btnFermerFondPressed").toString();
            QString alpha;
            if(stringColor.remove("#").length() == 8) {
                alpha = stringColor.left(2);
                stringColor = stringColor.right(6);
            }
            stringColor.prepend("#");
            QColor col(stringColor);
            bool ok;
            int hex = alpha.toInt(&ok, 16);
            if(ok) {
                col.setAlpha(hex);
            }
            btn->setCouleurFondPressed(col);
        }
    }

    if(m_parent == 0) {
        /* On en fait une fenetre modale par défaut */
        setWindowModality(Qt::ApplicationModal);
        abeApp->abeCenterWindow(this); /* on centre la fenetre */
        raise();
    }
    else{
        /* On centre l'AbulEduMessageBoxV1 */
        move((m_parent->geometry().width() - width())/2,
             (m_parent->geometry().height() - height())/2);
    }

    abeSetModeEnum(abeCloseButton);

    connect(this, SIGNAL(signalAbeMessageBoxCancel()),this, SLOT(setLastClicked()));
    connect(this, SIGNAL(signalAbeMessageBoxOK()),this, SLOT(setLastClicked()));
    connect(this, SIGNAL(signalAbeMessageBoxCloseOrHide()),this, SLOT(setLastClicked()));
    connect(this, SIGNAL(signalAbeMessageBoxNO()),this, SLOT(setLastClicked()));
    connect(this, SIGNAL(signalAbeMessageBoxYES()),this, SLOT(setLastClicked()));
}

AbulEduMessageBoxV1::~AbulEduMessageBoxV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete ui;
    m_config->deleteLater();
}

#ifdef __ABULEDU_MULTIMEDIA_AVAILABLE__
void AbulEduMessageBoxV1::abeMessageBoxSetMultimedia(QString oggPath)
{
    AbulEduMultiMediaV1* mm = new AbulEduMultiMediaV1(AbulEduMultiMediaV1::Sound, ui->frm_multimedia);
    mm->abeMultiMediaSetButtonVisible(AbulEduMultiMediaV1::BtnMagnifyingGlass | AbulEduMultiMediaV1::BtnPrevious | AbulEduMultiMediaV1::BtnNext | AbulEduMultiMediaV1::BtnHide | AbulEduMultiMediaV1::BtnRecord,false);
    mm->abeMultiMediaSetTextVisible(false);
    mm->abeMultiMediaForceStop();
    QHBoxLayout* hl = new QHBoxLayout(ui->frm_multimedia);
    hl->addWidget(mm->abeMultiMediaGetAudioControlWidget());
    if(oggPath.isNull()){
        mm->abeMultimediaSetTTS(AbulEduPicottsV1::fr);
        mm->abeMultiMediaSetNewMedia(AbulEduMediaMedias(QString(),QString(),ui->lblTexte->text()));
    }
    else if(QFile(oggPath).exists()){
        mm->abeMultimediaSetTTS(AbulEduPicottsV1::None);
        mm->abeMultiMediaSetNewMedia(AbulEduMediaMedias(QString(),oggPath));
    }
    else {
        ABULEDU_LOG_WARN()<<"File not found";
    }
    this->move(x(),y()-ui->frm_multimedia->height());
}
#endif

void AbulEduMessageBoxV1::setTitleColor(const QColor &color)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->lblTitre->setStyleSheet(ui->lblTitre->styleSheet().replace(ui->lblTitre->styleSheet().indexOf("#"),7,color.name()));
}

void AbulEduMessageBoxV1::abeMessageBoxSetTitleFontPointSize(int pointSize)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__<<pointSize;
    int b = -1;
    int e = -1;
    b = ui->lblTitre->styleSheet().indexOf("font-size:")+10; /* 10 est la longueur de la chaine "font-size:"  */
    if(b > -1) {
        e = ui->lblTitre->styleSheet().indexOf("pt",b);
    }
    if(e > -1){
        QString str = ui->lblTitre->styleSheet().mid(b,e-b);
        ui->lblTitre->setStyleSheet(ui->lblTitre->styleSheet().replace(str,QString::number(pointSize)));
    }
}

void AbulEduMessageBoxV1::setTextColor(const QColor &color)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->lblTexte->setStyleSheet(ui->lblTexte->styleSheet()+"color:"+color.name()+";");
}

void AbulEduMessageBoxV1::setBackgroundColor(const QColor &color)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->mainFrame->setStyleSheet(ui->mainFrame->styleSheet()+"background-color:"+color.name()+";");
}

void AbulEduMessageBoxV1::setWink(bool trueFalse)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << trueFalse;
    if(trueFalse)
    {
        ui->lblIcone->setPixmap(QPixmap(":/abuledumessageboxv1/icon/wink"));
        qDebug() << ui->lblIcone->size() <<  ui->lblIcone->pixmap()->size();
    }
    else
    {
        ui->lblIcone->setPixmap(QPixmap(":/abuledumessageboxv1/icon/oups"));
    }
    m_isWink = trueFalse;
}

void AbulEduMessageBoxV1::abeMessageBoxSetSpecialIcon(const QString &iconPath)
{
    if(QFile(iconPath).exists())
    {
        ui->lblIcone->setPixmap(QPixmap(iconPath));
    }
    else
    {
        ABULEDU_LOG_DEBUG() << "L'icône que vous essayez d'ajouter dans l'AbulEduMessageBoxV1 n'existe pas ";
    }
}

void AbulEduMessageBoxV1::abeSetModeEnum(AbulEduMessageBoxV1::enumAbulEduMessageBoxMode mode)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Par défaut on cache tous les boutons */
    ui->btnAnnuler->hide();
    ui->btnOK->hide();
    ui->btnFermer->hide();
    ui->btnOui->hide();
    ui->btnNon->hide();

    /* Puis on affiche ceux qu'il faut en fonction de la situation */
    switch (mode) {
    case abeYesNoButton:
        ui->btnOui->show();
        ui->btnNon->show();
        break;

    case abeOKButton:
        ui->btnOK->show();
        break;

    case abeCloseButton:
        ui->btnFermer->show();
        break;

    case abeAcceptCancelButton:
        ui->btnAnnuler->show();
        ui->btnOK->show();
        break;

    case abeAcceptCancelCloseButton:
        ui->btnAnnuler->show();
        ui->btnOK->show();
        ui->btnFermer->show();
        break;

    case abeYesNoCancelButton:
        ui->btnAnnuler->show();
        ui->btnOui->show();
        ui->btnNon->show();
        break;
    default:
        break;
    }

}

void AbulEduMessageBoxV1::on_btnFermer_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMessageBoxCloseOrHide();
    emit signalAbeMessageBoxButtonClicked(AbulEduMessageBoxV1::CloseOrHide);
    if(m_hasToBeClosed) {
        close();
    }
    else {
        hide();
    }
}

void AbulEduMessageBoxV1::on_btnOK_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMessageBoxOK();
    emit signalAbeMessageBoxButtonClicked(AbulEduMessageBoxV1::Accept);
    if(m_hasToBeClosed) {
        close();
    }
    else {
        hide();
    }
}

void AbulEduMessageBoxV1::on_btnAnnuler_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMessageBoxCancel();
    emit signalAbeMessageBoxButtonClicked(AbulEduMessageBoxV1::Reject);
    if(m_hasToBeClosed) {
        close();
    }
    else {
        hide();
    }
}

AbulEduFlatBoutonV1 *AbulEduMessageBoxV1::getLastClicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_lastClicked;
}

void AbulEduMessageBoxV1::setLastClicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_lastClicked = static_cast<AbulEduFlatBoutonV1*> (sender());
}

void AbulEduMessageBoxV1::abeMessageBoxUpdate(const QString& newTitle, const QString& newText, bool isWink)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    ui->lblTitre->setText(newTitle);
    ui->lblTexte->setText(newText);
    setWink(isWink);
    update();
    setVisible(true);
}

bool AbulEduMessageBoxV1::eventFilter(QObject *obj, QEvent *event)
{
    // ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    if(!isVisible()){
        return QObject::eventFilter(obj, event);
    }
    /* Pas localDebug car il y a trop de message */
    if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        /* Protection contre les clics frénétiques */
        if(!keyEvent->isAutoRepeat())
        {
            /* Navigation avec la touche Entrée : l'appui sur la touche Entrée provoque : */
            switch(keyEvent->key())
            {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if(ui->btnOK->isVisible()){
                    ui->btnOK->click();
                }
                else if(ui->btnFermer->isVisible()){
                    ui->btnFermer->click();
                }
                break;
            case Qt::Key_Escape:
                if(ui->btnAnnuler->isVisible()){
                    ui->btnAnnuler->click();
                }
            default:
                break;
            }
            return true;
        }
        return QObject::eventFilter(obj, event);
    }
    else
    {
        /* On fait suivre l'évènement, sinon tout est bloqué */
        return QObject::eventFilter(obj, event);
    }
}

void AbulEduMessageBoxV1::showInformation(const QString &text, QWidget *parent, const QString &title, bool doYouWantToClose)
{
    ABULEDU_LOG_WARN()  <<  " IS DEPRECATED please use abeMessageBoxShowInformation(const QString &text, QWidget *parent, const QString &title, bool isWink, bool doYouWantToClose)";
    AbulEduMessageBoxV1* info = new AbulEduMessageBoxV1(title,text,doYouWantToClose,parent);
    info->show();
}

void AbulEduMessageBoxV1::abeMessageBoxShowInformation(const QString &text, QWidget *parent, const QString &title, bool isWink, bool doYouWantToClose)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    AbulEduMessageBoxV1* info = new AbulEduMessageBoxV1(title,text,doYouWantToClose,parent);
    info->setWink(isWink);
    info->show();
}

bool AbulEduMessageBoxV1::abeMessageBoxGetWink()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    return m_isWink;
}

void AbulEduMessageBoxV1::on_btnOui_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMessageBoxYES();
    emit signalAbeMessageBoxButtonClicked(AbulEduMessageBoxV1::Yes);
    if(m_hasToBeClosed) {
        close();
    }
    else {
        hide();
    }
}

void AbulEduMessageBoxV1::on_btnNon_clicked()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    emit signalAbeMessageBoxNO();
    emit signalAbeMessageBoxButtonClicked(AbulEduMessageBoxV1::No);
    if(m_hasToBeClosed) {
        close();
    }
    else {
        hide();
    }
}
