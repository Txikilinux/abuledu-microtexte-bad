/** Classe AbulEduNetworkTests
  * @author 2013 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
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

#ifndef ABULEDUNETWORKTESTS_H
#define ABULEDUNETWORKTESTS_H

#include <QObject>
#include <QTcpSocket>
#include <QUrl>
#include <QTimer>
#include <QTime>
#include "abuleduloggerv1.h"

class AbulEduNetworkTests : public QObject
{
    Q_OBJECT
public:
    explicit AbulEduNetworkTests(QObject *parent = 0);
    virtual ~AbulEduNetworkTests();

signals:
    void signalError(int errorCode, int elapsedTimeInMs); /* errorCode = 2 : timeout et = 1 pour les autres */
    void signalOk(int elapsedTimeInMs);
    
public slots:
    void abeRunTest(const QString& domainTest, int portTest, int nbTests = 10);
    void abeStopTest();

private slots:
    void slotRestartTest();
    void slotConnected();
    void slotError();
    void slotError(const QAbstractSocket::SocketError& socketError);

private:
    QTcpSocket m_socket;
    QTime      m_time;          /* Pour calculer le temps de reponse */
    QTimer    *m_timerTimeOut;
    QTimer    *m_timerTests;
    int        m_nbPingTotal;   /* Nombre total de ping a faire */
    int        m_nbPingCurrent; /* Numero du ping en cours */
    int        m_portTest;      /* Numero du port sur lequel se connecter */
    int        m_nbMaxTests;    /* Pour eviter des pings loop infinite */
    QString    m_domainTest;    /* Nom de domaine du serveur sur lequel se connecter */
};

#endif // ABULEDUNETWORKTESTS_H
