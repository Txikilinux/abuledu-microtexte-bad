/** @class AbulEduLoadingAnimationV1
  * @file  abuleduloadinganimation.h
  *
  * @mainpage   AbulEduLoadingAnimationV1
  *             Cette classe représente un élément graphique qui permet de faire patienter l'utilisateur. \n
  *             Il se compose d'un QWidget embarquant un gif animé. \n
  *             Il est à utiliser pour les traitements longs afin que l'utilisateur ne puisse pas cliquer partout sur l'interface, \n
  *             sans pour autant avoir l'impression que le logiciel à planter (feed-back). \n
  *
  *
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

#ifndef ABULEDULOADINGANIMATIONV1_H
#define ABULEDULOADINGANIMATIONV1_H

#include <QLabel>
#include <QMovie>
#include <QSize>
#include <QVBoxLayout>
#include <QGraphicsEffect>
#include <QEvent>
#include "abuleduloggerv1.h"
#include "abuledusingletonv1.h"

using namespace AbulEduSingleton;

class AbulEduLoadingAnimationV1 : public AbulEduSingletonV1<AbulEduLoadingAnimationV1>, QWidget {

    friend class AbulEduSingletonV1<AbulEduLoadingAnimationV1>;

public:
    void start(QWidget *parent, const QString& status = QWidget::trUtf8("Veuillez patienter..."));
    /** @todo SUPPR param parent (passage par variable interne m_parent) */
    void stop(QWidget *parent);

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    AbulEduLoadingAnimationV1();
    ~AbulEduLoadingAnimationV1();

    void init(const QString& animationPath, const QSize& size);
    void center();

    QPointer<QLabel>            m_container;
    QPointer<QMovie>            m_animation;
    QGraphicsColorizeEffect     *m_colorEffect;
    QString                     m_status;
    QVBoxLayout                 *m_layout;
    QLabel                      *m_statusLabel;
    QWidget                     *m_parent;
};

#endif // ABULEDULOADINGANIMATIONV1_H
