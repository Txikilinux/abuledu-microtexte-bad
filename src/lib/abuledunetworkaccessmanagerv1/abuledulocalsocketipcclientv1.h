/** Classe AbulEduLocalSocketIpcClientV1 pour passage de ticket SSO entre applications
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

#ifndef ABULEDULOCALSOCKETIPCCLIENTV1_H
#define ABULEDULOCALSOCKETIPCCLIENTV1_H

#include <QObject>
#include <QtNetwork>
#include "simplecrypt.h"
#include "abuleduloggerv1.h"

class AbulEduLocalSocketIpcClientV1 : public QObject
{
    Q_OBJECT
public:
    AbulEduLocalSocketIpcClientV1(const QString& remoteServername, QObject *parent = 0);
    virtual ~AbulEduLocalSocketIpcClientV1();

    /** Pour ajouter un serveur dans notre liste */
    bool abeSlotAddRemoteServer(const QString& remoteServername);

    /** Pour supprimer un serveur de notre liste */
    bool abeSlotDelRemoteServer(const QString& remoteServername);

signals:

public slots:
    /** Envoie le message a un serveur distant */
    void abeSlotSendMessageToServer(const QString& message, const QString& server);

    /** Envoie le message a tous nos serveurs distants */
    void abeSlotSendMessageToServers(const QString &message);
    void abeSlotSocketConnected();
    void abeSlotSocketDisconnected();
    void abeSlotChangeRemoteServer(const QString &remoteServername);
    void abeSlotSocketReadReady();
    void abeSlotSocketError(QLocalSocket::LocalSocketError);

private:
    QLocalSocket*   m_socket;
    quint16         m_blockSize;
    QString         m_message;
    QStringList     m_serverName;
    bool            m_useCrypt;
};

#endif // ABULEDULOCALSOCKETIPCCLIENTV1_H
