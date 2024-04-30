/** @file abuleduloadinganimationv1.cpp
  * @author 2013-2014 Icham Sirat <icham.sirat@ryxeo.com>
  * @date 2014 02 25
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduLoadingAnimationV1
  * @see The GNU Public License (GPL)
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

#include "abuleduloadinganimationv1.h"

#ifndef DEBUG_ABULEDULOADINGANIMATIONV1
    #include "abuledudisableloggerv1.h"
#endif

void AbulEduLoadingAnimationV1::start(QWidget *parent, const QString &status)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_parent = parent;
    m_parent->installEventFilter(this);

    m_parent->setEnabled(false);
    /* Création de l'effet coloré à chaque fois, sinon pas bon au 2eme appel si creation ds constructeur */
    m_parent->setGraphicsEffect(m_colorEffect);
    m_parent->repaint();

    this->setParent(m_parent);

    if(!status.isEmpty()){
        m_status = status;
        m_statusLabel->setText(m_status);
        adjustSize();
    }

    center();

    if(!this->m_animation.isNull() &&
       (this->m_animation->state() == QMovie::NotRunning ||
        this->m_animation->state() == QMovie::Paused)) {
        this->m_animation->start();
        this->setHidden(false);
    }

    update();
    repaint();
}


void AbulEduLoadingAnimationV1::stop(QWidget *parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    parent->setEnabled(true);

    if(!this->m_animation.isNull()) {
        if(this->m_animation->state() == QMovie::Running) {
            this->m_animation->stop();
            this->setHidden(true);
        }
    }
    if(parent->graphicsEffect())
        parent->graphicsEffect()->setEnabled(false);
}

bool AbulEduLoadingAnimationV1::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::Resize) center();

    return QObject::eventFilter(obj, ev);
}

AbulEduLoadingAnimationV1::AbulEduLoadingAnimationV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    init(":/data/anim2", QSize());
    setWindowFlags(Qt::FramelessWindowHint);
}

AbulEduLoadingAnimationV1::~AbulEduLoadingAnimationV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_animation->deleteLater();
    m_container->deleteLater();
    m_colorEffect->deleteLater();
    m_statusLabel->deleteLater();
}

void AbulEduLoadingAnimationV1::init(const QString &animationPath, const QSize &size)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /* Création QMovie pour l'animation */
    m_animation = new QMovie(animationPath);

    m_colorEffect  = new QGraphicsColorizeEffect(this);
    m_colorEffect->setColor(QColor("gray"));

    QSize s = size;
    if(s.isEmpty()) {
        /* Trouver la 1ere frame du gif */
        m_animation->jumpToNextFrame();
        /* Prendre la taille de l'image */
        s = m_animation->currentPixmap().size();
        /* Aller au début */
        m_animation->jumpToFrame(0);
    }
    /* Utilisation d'un QLabel pour affichage*/
    m_container = new QLabel(this, Qt::FramelessWindowHint);
    /* Affectation du QMovie dans le label */
    m_container->setMovie(m_animation);

    m_container->setAlignment(Qt::AlignCenter);
    m_container->setAutoFillBackground(true);
    m_container->setAttribute(Qt::WA_TranslucentBackground);
    m_container->setBackgroundRole(QPalette::NoRole);

    m_statusLabel  = new QLabel(this, Qt::FramelessWindowHint);
    m_statusLabel->setStyleSheet("font: bold 20px; color : black");
    m_statusLabel->setBackgroundRole(QPalette::NoRole);
    m_statusLabel->setAlignment(Qt::AlignCenter);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_container);
    m_layout->addWidget(m_statusLabel);
    m_layout->setAlignment(Qt::AlignCenter);

    adjustSize();

    /* Cache par defaut */
    setHidden(true);
}

void AbulEduLoadingAnimationV1::center()
{
    this->move(((m_parent->width() / 2) - (this->width() / 2)),
               ((m_parent->height() / 2) - (this->height() / 2)));

}
