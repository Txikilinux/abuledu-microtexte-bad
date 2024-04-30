/** Classe AbulEduLocalSocketIpcServerV1 pour passage de ticket SSO entre applications
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * A partir de l'exemple suivant:
  * http://www.developer.nokia.com/Community/Wiki/Qt_IPC_using_QLocalSocket_&_QLocalServer
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

#ifndef ABULEDULOCALSOCKETIPCSERVERV1_H
#define ABULEDULOCALSOCKETIPCSERVERV1_H

#include <QObject>
#include <QtNetwork>
#include <unistd.h>
#include "abuleduloggerv1.h"
#include "simplecrypt.h"

class AbulEduLocalSocketIpcServerV1: public QObject
{
    Q_OBJECT
public:
    AbulEduLocalSocketIpcServerV1(const QString& servername, QObject *parent);
    virtual ~AbulEduLocalSocketIpcServerV1();

signals:
    void abeSignalMessageReceived(const QString&);

public slots:
    void abeSlotSocketNewConnection();

private:
    QLocalServer*       m_server;
    bool                m_useCrypt;
    QSettings           m_settings;
};

#endif // ABULEDULOCALSOCKETIPCSERVERV1_H
