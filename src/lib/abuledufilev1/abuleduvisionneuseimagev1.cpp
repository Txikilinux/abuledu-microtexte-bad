/** Classe AbulEduVisionneuseImageV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Icham Sirat <icham.sirat@ryxeo.com>
  * @author 2012-2014 Éric Seigne <eric.seigne@ryxeo.com>
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

#include "abuleduvisionneuseimagev1.h"

#ifndef DEBUG_ABULEDUFILEV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduVisionneuseImageV1::AbulEduVisionneuseImageV1(QWidget *parent, bool closeOnPress)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ ;
    m_parent = parent;

    m_closeOnPress = closeOnPress;

    m_imageLabel = new QLabel(this);
    m_imageLabel->setBackgroundRole(QPalette::Base);
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_imageLabel->setScaledContents(true);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_imageLabel);
    setCentralWidget(m_scrollArea);

    //On ferme au bout de 5 secondes ?
    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    connect(m_timer, SIGNAL(timeout()),this,SLOT(close()), Qt::UniqueConnection);

#ifndef QT_NO_PRINTER
    m_printer = new QPrinter();
#endif
    creerActions();
//    creerMenus();

    setWindowTitle(trUtf8("Visionneuse Image"));
    resize(500, 400);
    if (m_parent) {
        setMaximumSize(m_parent->size());
    }
}

AbulEduVisionneuseImageV1::~AbulEduVisionneuseImageV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    delete m_scrollArea;
    delete m_timer;
    /* Les actions et menus ne sont plus créés, donc il n'y a pas à les détruire */
}

void AbulEduVisionneuseImageV1::ouvrir()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QString nomFichier = QFileDialog::getOpenFileName(this,
                                                      trUtf8("Ouvrir fichier"), QDir::currentPath());
    if (!nomFichier.isEmpty()) {
        QImage image(nomFichier);
        if (image.isNull()) {
            QMessageBox::information(this, trUtf8("Visionneuse Image"),
                                     trUtf8("Impossible d'ouvrir %1.").arg(nomFichier));
            return;
        }
        m_imageLabel->setPixmap(QPixmap::fromImage(image));
        m_scaleFactor = 1.0;

        m_action_imprimer->setEnabled(true);
        m_action_ajusterFenetre->setEnabled(true);
        majActions();

        if (!m_action_ajusterFenetre->isChecked())
            m_imageLabel->adjustSize();
    }

}

void AbulEduVisionneuseImageV1::ouvrirFicher(const QString &chemin)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << chemin;
    if (!chemin.isEmpty()) {
        QImage image(chemin);
        if (image.isNull()) {
            QMessageBox::information(this, trUtf8("Visionneuse Image"),
                                     trUtf8("Impossible d'ouvrir %1.").arg(chemin));
            return;
        }
        m_imageLabel->setPixmap(QPixmap::fromImage(image));
        m_scaleFactor = 1.0;

        //        m_action_imprimer->setEnabled(true);
        //        m_action_ajusterFenetre->setEnabled(true);
        //        majActions();

        //        if (!m_action_ajusterFenetre->isChecked())
        //            m_imageLabel->adjustSize();
    }
}


void AbulEduVisionneuseImageV1::ouvrirFicher(const QUrl &uri)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_imageLabel->setPixmap(QPixmap());

    connect(abeApp->getAbeNetworkAccessManager(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUrlDownloaded(QNetworkReply*)), Qt::UniqueConnection);
    QNetworkReply *reply = abeApp->getAbeNetworkAccessManager()->get(QNetworkRequest(uri));
    reply->setObjectName("fromAbulEduVisionneuseImageV1");
    m_listNetworkReply.append(reply);
}

void AbulEduVisionneuseImageV1::abeVisionneuseImageSetTimeout(int ms)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if(ms <= 0) {
        m_isTimerUsed = false;
        m_timer->stop();
    }
    else {
        m_isTimerUsed = true;
        m_timer->setInterval(ms);
    }
}

void AbulEduVisionneuseImageV1::fixeTempsAffichageMax(int ms)
{
    ABULEDU_LOG_WARN() << __PRETTY_FUNCTION__<< " IS DEPRECATED please use abeVisionneuseImageSetTimeout";
    abeVisionneuseImageSetTimeout(ms);
}

void AbulEduVisionneuseImageV1::ajusterFenetreAuMieux()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool isPortrait = (m_imageLabel->pixmap()->width() < m_imageLabel->pixmap()->height());
    QPixmap ret;
    if (isPortrait) {
        ret = m_imageLabel->pixmap()->scaledToHeight(height(), Qt::SmoothTransformation);
    }
    else {
        ret = m_imageLabel->pixmap()->scaledToWidth(width(), Qt::SmoothTransformation);
    }
    m_imageLabel->setPixmap(ret);
    setFixedSize(ret.size());
    m_scrollArea->setWidgetResizable(true);
    if (m_parent) {
        move((m_parent->width()-width())/2,(m_parent->height()-height())/2);
    }
}

void AbulEduVisionneuseImageV1::slotUrlDownloaded(QNetworkReply *reply)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //Si c'est pas pour moi
    if(! m_listNetworkReply.contains(reply))  return;

    //Et il ne fait maintenant plus partie de ma liste de reply a gerer
    m_listNetworkReply.removeOne(reply);
    if (reply->error() != QNetworkReply::NoError) {
        ABULEDU_LOG_DEBUG() << "  Error in" << reply->url() << ":" << reply->errorString();
        return;
    }
    const QVariant attribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (attribute.isValid()) {
        QUrl url = attribute.toUrl();
        ABULEDU_LOG_DEBUG() << "must go to:" << url;
        return;
    }
    ABULEDU_LOG_DEBUG() << "URI:" << reply->url()
                        << "  ContentType:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());
    m_imageLabel->setPixmap(pixmap);
    m_scaleFactor = 1.0;
    m_imageLabel->adjustSize();
    this->resize(m_imageLabel->size().width()+4,m_imageLabel->size().height()+4);

    reply->deleteLater();
    disconnect(abeApp->getAbeNetworkAccessManager(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUrlDownloaded(QNetworkReply*)));
}

void AbulEduVisionneuseImageV1::imprimer()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_ASSERT(m_imageLabel->pixmap());
#ifndef QT_NO_PRINTER
    QPrintDialog dialog(m_printer, this);
    if (dialog.exec()) {
        QPainter painter(m_printer);
        QRect rect = painter.viewport();
        QSize size = m_imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(m_imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *m_imageLabel->pixmap());
    }
#endif
}

void AbulEduVisionneuseImageV1::zoomIn()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    scaleImage(1.25);
}

void AbulEduVisionneuseImageV1::zoomOut()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    scaleImage(0.8);
}

void AbulEduVisionneuseImageV1::tailleNormale()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_imageLabel->adjustSize();
    m_scaleFactor = 1.0;
}

void AbulEduVisionneuseImageV1::ajusterFenetre()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    bool isAjustable = m_action_ajusterFenetre->isChecked();
    m_scrollArea->setWidgetResizable(isAjustable);
    if (!isAjustable) {
        tailleNormale();
    }
    //    majActions();
}

void AbulEduVisionneuseImageV1::aPropos()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QMessageBox::about(this, trUtf8("A Propos"),
                       trUtf8("<p><b>La Visionneuse d'Image</b></p>"
                              "<p>Permet d'afficher une image, de zoomer, d'ajuster la taille"
                              " de l'image à la fenetre et d'imprimer (pdf). </p>"));
}


void AbulEduVisionneuseImageV1::creerActions()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_action_ouvrir = new QAction(trUtf8("&Ouvrir..."), this);
    m_action_ouvrir->setShortcut(trUtf8("Ctrl+O"));
    connect(m_action_ouvrir, SIGNAL(triggered()), this, SLOT(ouvrir()), Qt::UniqueConnection);

    m_action_imprimer = new QAction(trUtf8("&Imprimer..."), this);
    m_action_imprimer->setShortcut(trUtf8("Ctrl+P"));
    m_action_imprimer->setEnabled(false);
    connect(m_action_imprimer, SIGNAL(triggered()), this, SLOT(imprimer()), Qt::UniqueConnection);

    m_action_quitter = new QAction(trUtf8("&Quitter"), this);
    m_action_quitter->setShortcut(trUtf8("Ctrl+Q"));
    connect(m_action_quitter, SIGNAL(triggered()), this, SLOT(close()), Qt::UniqueConnection);

    m_action_zoomIn = new QAction(trUtf8("Zoom &In (25%)"), this);
    m_action_zoomIn->setShortcut(trUtf8("Ctrl++"));
    m_action_zoomIn->setEnabled(false);
    connect(m_action_zoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()), Qt::UniqueConnection);

    m_action_zoomOut = new QAction(trUtf8("Zoom &Out (25%)"), this);
    m_action_zoomOut->setShortcut(trUtf8("Ctrl+-"));
    m_action_zoomOut->setEnabled(false);
    connect(m_action_zoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()), Qt::UniqueConnection);

    m_action_tailleNormal = new QAction(trUtf8("&Taille Normale"), this);
    m_action_tailleNormal->setShortcut(trUtf8("Ctrl+S"));
    m_action_tailleNormal->setEnabled(false);
    connect(m_action_tailleNormal, SIGNAL(triggered()), this, SLOT(tailleNormale()), Qt::UniqueConnection);

    m_action_ajusterFenetre = new QAction(trUtf8("&Ajuster a la fenetre"), this);
    m_action_ajusterFenetre->setEnabled(false);
    m_action_ajusterFenetre->setCheckable(true);
    m_action_ajusterFenetre->setShortcut(trUtf8("Ctrl+F"));
    connect(m_action_ajusterFenetre, SIGNAL(triggered()), this, SLOT(ajusterFenetre()), Qt::UniqueConnection);

    m_action_aPropos = new QAction(trUtf8("&A propos"), this);
    connect(m_action_aPropos, SIGNAL(triggered()), this, SLOT(aPropos()), Qt::UniqueConnection);

}

void AbulEduVisionneuseImageV1::creerMenus()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_menu_fichier = new QMenu(trUtf8("&Fichier"), this);
    m_menu_fichier->addAction(m_action_ouvrir);
    m_menu_fichier->addAction(m_action_imprimer);
    m_menu_fichier->addSeparator();
    m_menu_fichier->addAction(m_action_quitter);

    m_menu_vue = new QMenu(trUtf8("&Vue"), this);
    m_menu_vue->addAction(m_action_zoomIn);
    m_menu_vue->addAction(m_action_zoomOut);
    m_menu_vue->addAction(m_action_tailleNormal);
    m_menu_vue->addSeparator();
    m_menu_vue->addAction(m_action_ajusterFenetre);

    m_menu_aide = new QMenu(trUtf8("&Aide"), this);
    m_menu_aide->addAction(m_action_aPropos);

    menuBar()->addMenu(m_menu_fichier);
    menuBar()->addMenu(m_menu_vue);
    menuBar()->addMenu(m_menu_aide);
}

void AbulEduVisionneuseImageV1::majActions()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    //    m_action_zoomIn->setEnabled(!m_action_ajusterFenetre->isChecked());
    //    m_action_zoomOut->setEnabled(!m_action_ajusterFenetre->isChecked());
    //    m_action_tailleNormal->setEnabled(!m_action_ajusterFenetre->isChecked());
}

void AbulEduVisionneuseImageV1::scaleImage(double facteur)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_ASSERT(m_imageLabel->pixmap());
    m_scaleFactor *= facteur;
    m_imageLabel->resize(m_scaleFactor * m_imageLabel->pixmap()->size());

    adjustScrollBar(m_scrollArea->horizontalScrollBar(), facteur);
    adjustScrollBar(m_scrollArea->verticalScrollBar(), facteur);

    m_action_zoomIn->setEnabled(m_scaleFactor < 3.0);
    m_action_zoomOut->setEnabled(m_scaleFactor > 0.333);
}

void AbulEduVisionneuseImageV1::adjustScrollBar(QScrollBar *scrollBar, double facteur)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    scrollBar->setValue(int(facteur * scrollBar->value()
                            + ((facteur - 1) * scrollBar->pageStep()/2)));
}

void AbulEduVisionneuseImageV1::mouseReleaseEvent(QMouseEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* On ne close pas l'objet */
    if(!m_closeOnPress) return;

    //La je ne comprends pas, il faudra m'expliquer ...
    //En bref, je clique une fois -> 5 secondes plus tard l'image disparait
    //et je clique une 2° fois l'image disparait dessuite ...
    if(event->button() == Qt::LeftButton && m_timer->isActive()) {
        ABULEDU_LOG_DEBUG() << "Close";
        m_timer->stop();
        close();
    }
    //modification: si le timer n'est pas utilisé un simple clic sur l'image pour fermer
    if(m_isTimerUsed) {
        ABULEDU_LOG_DEBUG() << "Timer start";
        m_timer->start();
    }
    else {
        ABULEDU_LOG_DEBUG() << "Close";
        close();
    }
}

