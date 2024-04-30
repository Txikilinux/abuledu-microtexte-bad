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

#ifndef ABULEDUAUDIOLEVELWIDGETV1_H
#define ABULEDUAUDIOLEVELWIDGETV1_H

#include <QWidget>
#include <QGradient>
#include "abuleduloggerv1.h"

///
/// \brief The AbulEduAudioLevelWidgetV1 class petit widget pour montrer le niveau sonore
///
class AbulEduAudioLevelWidgetV1 : public QWidget
{
    Q_OBJECT
public:
    explicit AbulEduAudioLevelWidgetV1(QWidget *parent = 0);

public slots:
    /** Fixe le niveau à afficher dans l'intervalle [0.0,1.0]
     *  @param level Le niveau à afficher compris entre 0.0 et 1.0 (qreal)
     */
    void slotAbulEduAudioLevelWidgetSetLevel(qreal level);

    void slotAbulEduAudioLevelWidgetSetActivate(bool activate);
protected:
    /** Dessine le widget en fonction du level passé dans la fonction setLevel*/
    void paintEvent(QPaintEvent *event);

private:
    qreal           m_level;
    bool            m_activated;
    QLinearGradient gradient;
};

#endif // ABULEDUAUDIOLEVELWIDGETV1_H
