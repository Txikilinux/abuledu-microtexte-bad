/** Classe AbulEduAudioLevelWidgetV1
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

#include <QPainter>
#include "abuleduaudiolevelwidgetv1.h"

#ifndef DEBUG_ABULEDUMULTIMEDIAV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduAudioLevelWidgetV1::AbulEduAudioLevelWidgetV1(QWidget *parent) :
    QWidget(parent)  ,
    m_level(0.0),
    m_activated(true),
    gradient(QPointF(0,0), QPointF(1,0))
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient.setColorAt(0, Qt::green);
    gradient.setColorAt(0.8, Qt::yellow);
    gradient.setColorAt(1, Qt::red);
}

void AbulEduAudioLevelWidgetV1::slotAbulEduAudioLevelWidgetSetActivate(bool activate)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_activated = activate;
    update();
}

void AbulEduAudioLevelWidgetV1::slotAbulEduAudioLevelWidgetSetLevel(qreal level)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    if (m_level != level)
    {
        if(!m_activated)
        {
            m_level = 0.0;
        }
        else
        {
            m_level = level;
        }
        update();
    }
}

void AbulEduAudioLevelWidgetV1::paintEvent(QPaintEvent *event)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    Q_UNUSED(event);

    QPainter painter(this);
    // dessin du fond
    qreal widthLevel = m_level * width();
    painter.fillRect(0, 0, width(), height(), gradient);
    // dessin du masque
    painter.fillRect(widthLevel, 0, width(), height(), Qt::black);
}
