/** Classe AbulEduMediaGraphicsButtonV1
  *
  * @author 2013 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
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
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#include "abuledumediagraphicsbuttonv1.h"
#include "abuleduapplicationv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduMediaGraphicsButtonV1::AbulEduMediaGraphicsButtonV1(QGraphicsItem *parentForButton, AbulEduMultiMediaWidgetContainer *parentForMultiMedia,
                                                           QList<AbulEduMediaMedias> listMedias,
                                                           AbulEduPicottsV1::AbulEduLang language,
                                                           bool isTextVisible):
    QGraphicsObject(0),
    m_medias(listMedias),
    m_language(language),
    m_multiMedia(0),
    m_textIsVisible(isTextVisible),
    m_parentPlayer(parentForMultiMedia),
    m_isNormalState(true)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    initAbulEduMediaGraphicsButton(parentForButton);
}

void AbulEduMediaGraphicsButtonV1::initAbulEduMediaGraphicsButton(QGraphicsItem *parentForButton)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    setAcceptedMouseButtons(Qt::RightButton | Qt::LeftButton);
    setFlag( ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemClipsToShape);
    setAcceptHoverEvents(true);
    abeMediaGraphicsButtonSetFixedSize(30,30);
    connect(this, SIGNAL(signalAbeGraphicsButtonClicGaucheReleased(QPoint)), this, SLOT(slotAbeCreateMultiMedia()), Qt::UniqueConnection);
    m_pathPixmapNormal = ":/controlaudio/more";
    m_pathPixmapHover = ":/controlaudio/more_hover";
    setParentItem(parentForButton);
    if(m_parentPlayer)
        m_parentPlayer->setVisible(false);
}

AbulEduMediaGraphicsButtonV1::~AbulEduMediaGraphicsButtonV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduMediaGraphicsButtonV1::abeMediaGraphicsButtonSetFixedSize(QSize size)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_buttonSize = size;
    m_pixMapNormal = QPixmap(m_pathPixmapNormal);
    m_pixMapNormal = m_pixMapNormal.scaled(m_buttonSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_pixMapHover = QPixmap(m_pathPixmapHover);
    m_pixMapHover = m_pixMapHover.scaled(m_buttonSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    prepareGeometryChange();
    update(boundingRect());
}

void AbulEduMediaGraphicsButtonV1::abeMediaGraphicsButtonSetFixedSize(int width, int height)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    abeMediaGraphicsButtonSetFixedSize(QSize(width, height));
}

void AbulEduMediaGraphicsButtonV1::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    QGraphicsItem::mousePressEvent(event);
    event->accept();
}

void AbulEduMediaGraphicsButtonV1::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    switch(event->button())
    {
    case Qt::RightButton :
        ABULEDU_LOG_DEBUG() << "Clic droit";
        emit signalAbeGraphicsButtonClicDroitReleased(boundingRect().center().toPoint());
        break;
    case Qt::LeftButton :
        emit signalAbeGraphicsButtonClicGaucheReleased(boundingRect().center().toPoint());
        break;
    default:
        break;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void AbulEduMediaGraphicsButtonV1::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_isNormalState = false;
    QGraphicsItem::hoverEnterEvent(event);
}

void AbulEduMediaGraphicsButtonV1::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_isNormalState = true;
    QGraphicsItem::hoverLeaveEvent(event);
}

void AbulEduMediaGraphicsButtonV1::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(m_isNormalState)
    {
        painter->drawPixmap(0, 0, m_buttonSize.width(), m_buttonSize.height(), m_pixMapNormal);
    }
    else
    {
        painter->drawPixmap(0, 0, m_buttonSize.width(), m_buttonSize.height(), m_pixMapHover);
    }
}

QRectF AbulEduMediaGraphicsButtonV1::boundingRect() const
{
    return QRectF(QPoint(0,0), m_buttonSize);
}

void AbulEduMediaGraphicsButtonV1::slotAbeCreateMultiMedia()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << m_textIsVisible;
    if(!m_multiMedia)
    {
        m_multiMedia = new AbulEduMultiMediaV1(m_medias, AbulEduMultiMediaV1::Diapos, m_language, m_textIsVisible, AbulEduMultiMediaSettingsV1::Automatic, m_parentPlayer);
        m_multiMedia->abeMultiMediaSetButtonVisible(AbulEduMultiMediaV1::BtnNext | AbulEduMultiMediaV1::BtnPrevious | AbulEduMultiMediaV1::BtnMagnifyingGlass, false);
        if(m_parentPlayer)
        {
            connect(m_multiMedia->abeMultiMediaGetAudioControlWidget(), SIGNAL(signalAbeControlAudioBtnHideClicked()), m_parentPlayer, SLOT(hide()),Qt::UniqueConnection);

            m_multiMedia->setFixedSize(m_parentPlayer->size());
//            m_multiMedia->move((m_multiMedia->width()-m_multiMedia->abeMultiMediaGetViewerWidget()->width())/2,0);
            m_multiMedia->move(0,0);
        }
    }
    m_multiMedia->show();
    emit signalAbeGraphicsButtonMultiMediaWidgetCreated(m_multiMedia);
    if(m_parentPlayer)
        m_parentPlayer->setVisible(true);
}
