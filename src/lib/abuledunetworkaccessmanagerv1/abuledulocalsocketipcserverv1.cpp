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

#include "abuledulocalsocketipcserverv1.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduLocalSocketIpcServerV1::AbulEduLocalSocketIpcServerV1(const QString &servername, QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    /** On utilise un qsettings pour eventuellement supprimer un ipc pipe ou autre si l'appli a planté ou qu'on l'a
      * quitter sans passer par les destructeurs
      * Note: On Unix if the server crashes without closing listen will fail with AddressInUseError. To create a new
      * server the file should be removed. On Windows two local servers can listen to the same pipe at the same time,
      * but any connections will go to one of the server.
      */
    QSettings m_settings(qApp->organizationName(),qApp->applicationName());

    /** Peut-etre un problème ... ou pas, mais parfois l'application ne redémarre pas uniquement à cause du fichier .lock
      * comme c'est un fichier par application je me permet de décider arbitrairement de virer le .lock a lancement s'il existe
      */
#ifdef Q_OS_LINUX
    QFile fiLock(m_settings.fileName() + ".lock");
    if(fiLock.exists()) {
        fiLock.close();
        fiLock.remove();
    }
#endif
    if((!m_settings.value("AbulEduLocalSocketIpcServerV1").toString().isEmpty())
            && (!m_settings.value("AbulEduLocalSocketIpcServerV1-PID").toString().isEmpty()))
    {
        /* On essaye de supprimer le pipe existant */
#ifdef Q_OS_LINUX
        QFile f("/tmp/" + servername);
        QFile fPID("/proc/" + m_settings.value("AbulEduLocalSocketIpcServerV1-PID").toString());

        /* Si le pipe existe *et* que le processus n'existe plus alors on supprime le pipe */
        if(f.exists() && ! fPID.exists()) {
            ABULEDU_LOG_DEBUG() <<"  Tentative de suppression du vieux pipe ...";
            f.remove();
            m_settings.remove("AbulEduLocalSocketIpcServerV1");
            m_settings.remove("AbulEduLocalSocketIpcServerV1-PID");
            m_settings.sync();
        }
#endif
        /** @todo windows et mac */
#ifdef Q_OS_WIN

#endif
#ifdef Q_OS_MAC

#endif

    }

    m_useCrypt = true;
    m_server = new QLocalServer(this);
    if (m_server->listen(servername)) {
        if(m_server->isListening()) {
            ABULEDU_LOG_DEBUG() << "  Server is listening";
            m_settings.setValue("AbulEduLocalSocketIpcServerV1", servername);
#ifdef Q_OS_LINUX
            m_settings.setValue("AbulEduLocalSocketIpcServerV1-PID", getpid());
#endif
#ifdef Q_OS_WIN
            //m_settings.setValue("AbulEduLocalSocketIpcServerV1-PID",_getpid());
#endif
            /** @todo windows mac */
#ifdef Q_OS_MAC

#endif
        }
        else {
            ABULEDU_LOG_WARN() << "  Not able to start the Server";
        }
    }
    else {
        ABULEDU_LOG_WARN()  << "  Not able to start the Server";
    }

    connect(m_server, SIGNAL(newConnection()), this, SLOT(abeSlotSocketNewConnection()), Qt::UniqueConnection);
}

AbulEduLocalSocketIpcServerV1::~AbulEduLocalSocketIpcServerV1()
{
    m_settings.remove("AbulEduLocalSocketIpcServerV1");
    m_settings.remove("AbulEduLocalSocketIpcServerV1-PID");
    m_settings.sync();
    delete m_server;
}


void AbulEduLocalSocketIpcServerV1::abeSlotSocketNewConnection()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    QLocalSocket *clientConnection = m_server->nextPendingConnection();

    if(!clientConnection) {
       ABULEDU_LOG_DEBUG() << "QLocalSocket *clientConnection = m_server->nextPendingConnection() == NULL";
       ABULEDU_LOG_DEBUG() << "Pas possible de créer une nouvelle connexion IPC";
       return;
    }

    if(clientConnection->waitForReadyRead()){
        connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()), Qt::UniqueConnection);

        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_4_0);
        if (clientConnection->bytesAvailable() > (int)sizeof(quint16)) {
            QString message;
            in >> message;

            if(m_useCrypt) {
                //attention a garder les cles synchro avec abuledulocalsocketipcclientv1.cpp ligne 74
                SimpleCrypt crypted_message(Q_UINT64_C(932221987109275));
                emit abeSignalMessageReceived(crypted_message.decryptToString(message));
            }
            else {
                emit abeSignalMessageReceived(message);
            }
        }
    }
    else { /* clientConnection->waitForReadyRead() == false */
        ABULEDU_LOG_DEBUG() << "clientConnection->waitForReadyRead() == false [A GERER]";
    }
}
