/** Classe AbulEduNetworkThreadV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
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

#ifndef ABULEDUNETWORKTHREADV1_H
#define ABULEDUNETWORKTHREADV1_H

#include <QUrl>
#include <QDesktopServices>
#include <QThread>
#include "abuleduloggerv1.h"

class AbulEduNetworkThreadV1 : public QThread
{
    Q_OBJECT
public:
    explicit AbulEduNetworkThreadV1(const QUrl& url, QObject *parent = 0);
    virtual ~AbulEduNetworkThreadV1();
    void run();

private:
    QUrl m_url;
};

#endif // ABULEDUNETWORKTHREADV1_H
