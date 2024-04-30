/** Classe AbulEduNetworkThreadV1
  * @see https://redmine.ryxeo.com/projects/
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#include "abuledunetworkthreadv1.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduNetworkThreadV1::AbulEduNetworkThreadV1(const QUrl &url, QObject *parent) :
    QThread(parent),
    m_url(url)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__ << url;
}

AbulEduNetworkThreadV1::~AbulEduNetworkThreadV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduNetworkThreadV1::run()
{
    ABULEDU_LOG_TRACE() << "  Lancement du thread ... sur " << m_url;

    QDesktopServices::openUrl(m_url);
    connect(this, SIGNAL(finished ()) , this, SLOT(deleteLater ()),Qt::UniqueConnection);
    start();
    ABULEDU_LOG_DEBUG() << "  Fin de lancement du thread ...";
}
