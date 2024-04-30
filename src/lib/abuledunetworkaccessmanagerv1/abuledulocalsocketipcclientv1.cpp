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

#include "abuledulocalsocketipcclientv1.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduLocalSocketIpcClientV1::AbulEduLocalSocketIpcClientV1(const QString &remoteServername, QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_socket        = new QLocalSocket(this);
    m_serverName    = QStringList(remoteServername);
    m_useCrypt      = true;

    connect(m_socket, SIGNAL(connected()), this, SLOT(abeSlotSocketConnected()), Qt::UniqueConnection);
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(abeSlotSocketDisconnected()), Qt::UniqueConnection);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(abeSlotSocketReadReady()),Qt::UniqueConnection);
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(abeSlotSocketError(QLocalSocket::LocalSocketError)),Qt::UniqueConnection);
}

AbulEduLocalSocketIpcClientV1::~AbulEduLocalSocketIpcClientV1()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
    m_socket->abort();
}

bool AbulEduLocalSocketIpcClientV1::abeSlotAddRemoteServer(const QString &remoteServername)
{
    ABULEDU_LOG_TRACE() << remoteServername;

    if(!m_serverName.contains(remoteServername)) {
        m_serverName.append(remoteServername);
        return true;
    }
    return false;
}

bool AbulEduLocalSocketIpcClientV1::abeSlotDelRemoteServer(const QString &remoteServername)
{
    ABULEDU_LOG_TRACE() << remoteServername;

    if(m_serverName.contains(remoteServername)) {
        m_serverName.removeOne(remoteServername);
        return true;
    }
    return false;
}

void AbulEduLocalSocketIpcClientV1::abeSlotSendMessageToServer(const QString &message, const QString &server)
{
    ABULEDU_LOG_TRACE() << QString("message = %s | server = %s").arg(message, server);

    if(m_serverName.contains(server)) {
        m_socket->abort();
        m_message = message;
        m_socket->connectToServer(server);
    }
}

void AbulEduLocalSocketIpcClientV1::abeSlotSendMessageToServers(const QString &message)
{
    ABULEDU_LOG_TRACE() << message;

    m_socket->abort();
    m_message = message;
    for(int i = 0; i < m_serverName.size(); i++) {
        ABULEDU_LOG_DEBUG() << "  AbulEduLocalSocketIpcClientV1 send " << message << " to " << m_serverName.at(i);
        m_socket->connectToServer(m_serverName.at(i));
        m_socket->waitForConnected(1000);
    }
}


void AbulEduLocalSocketIpcClientV1::abeSlotSocketConnected()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    /* Quand alacarte se comporte comme un client il faut crypter */
    if(qApp->applicationName() == "abuledu-alacarte" && m_useCrypt) {
        /* C'est là qu'il faut gérer le trousseau de cles des logiciels ... et que ça va devenir un peu compliqué */
        SimpleCrypt crypted_message(Q_UINT64_C(932221987109275));
        out << crypted_message.encryptToString(m_message);
    }
    else {
        if(m_useCrypt) {
            SimpleCrypt crypted_message(Q_UINT64_C(932221987109275));
            out << crypted_message.encryptToString(m_message);
        }
        else {
            out << m_message;
        }
    }
    out.device()->seek(0);
    m_socket->write(block);
    m_socket->flush();
}

void AbulEduLocalSocketIpcClientV1::abeSlotSocketDisconnected()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduLocalSocketIpcClientV1::abeSlotChangeRemoteServer(const QString &remoteServername)
{
    ABULEDU_LOG_TRACE() << "  Configuration de l'application cliente : " << remoteServername;

    m_serverName.clear();
    m_serverName.append(remoteServername);
}

void AbulEduLocalSocketIpcClientV1::abeSlotSocketReadReady()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;
}

void AbulEduLocalSocketIpcClientV1::abeSlotSocketError(QLocalSocket::LocalSocketError e)
{
    ABULEDU_LOG_TRACE() << this->objectName() << " :: " << e;
}

