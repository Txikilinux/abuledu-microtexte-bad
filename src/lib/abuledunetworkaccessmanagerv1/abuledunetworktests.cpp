/**
  * Classe
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
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

#include "abuledunetworktests.h"

#ifndef DEBUG_ABULEDUNETWORKACCESSMANAGERV1
    #include "abuledudisableloggerv1.h"
#endif

AbulEduNetworkTests::AbulEduNetworkTests(QObject *parent) :
    QObject(parent)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_nbPingTotal = 10;
    m_nbPingCurrent = 0;
    m_timerTests = new QTimer(this);
    m_timerTimeOut = new QTimer(this);

    //il faut avoir une solution pour éviter une sorte de boucle infinie de tests
    m_nbMaxTests = 0;
}

AbulEduNetworkTests::~AbulEduNetworkTests()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    delete m_timerTimeOut;
    delete m_timerTests;
}

void AbulEduNetworkTests::abeRunTest(const QString &domainTest, int portTest, int nbTests)
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_nbPingTotal   = nbTests;
    m_portTest      = portTest;
    m_domainTest    = domainTest;
    m_nbPingCurrent = 0;

    m_socket.setSocketOption(QAbstractSocket::LowDelayOption,1);

    /* IS 20140724 @see https://redmine.ryxeo.com/issues/3800 */
    if(!m_socket.isOpen())
        m_socket.connectToHost(m_domainTest,m_portTest);

    connect(&m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)), Qt::UniqueConnection);
    connect(&m_socket,SIGNAL(connected()),this,SLOT(slotConnected()), Qt::UniqueConnection);

    //Toutes les 5 secondes on relance le test de connexion
    m_timerTests->setInterval(5000);
    connect(m_timerTests,SIGNAL(timeout()),this,SLOT(slotRestartTest()), Qt::UniqueConnection);
    m_timerTests->start();

    //Et si au bout de 4 secondes pas de réponse on stoppe
    m_timerTimeOut->setInterval(4000);
    connect(m_timerTimeOut,SIGNAL(timeout()),this,SLOT(slotError()), Qt::UniqueConnection);
    m_timerTimeOut->start();
    m_time.start();
}

void AbulEduNetworkTests::abeStopTest()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    m_timerTimeOut->stop();
    m_timerTests->stop();
}

void AbulEduNetworkTests::slotRestartTest()
{
    ABULEDU_LOG_TRACE() << __PRETTY_FUNCTION__;

    //il faut avoir une solution pour éviter une sorte de boucle infinie de tests
    //pour l'instant c'est 10 max ...
    if(m_nbMaxTests > 10) {
        abeStopTest();
    }
    else {

        m_timerTests->stop();
        m_timerTimeOut->stop();

        m_socket.close();
        m_socket.connectToHost(m_domainTest,m_portTest);

        m_timerTests->start();
        m_timerTimeOut->start();
        m_time.start();
    }
}

void AbulEduNetworkTests::slotConnected()
{
    ABULEDU_LOG_TRACE() <<  m_time.elapsed() << "ms";

    m_timerTimeOut->stop();
    m_socket.close();
    abeStopTest();
    emit signalOk(m_time.elapsed());
}

void AbulEduNetworkTests::slotError()
{
    ABULEDU_LOG_TRACE() <<  m_time.elapsed() << "ms";

    m_timerTimeOut->stop();
    m_socket.abort();
    m_socket.close();

    emit signalError(1,m_time.elapsed());
}

void AbulEduNetworkTests::slotError(const QAbstractSocket::SocketError &socketError)
{
    ABULEDU_LOG_TRACE() << socketError  <<  m_time.elapsed() << "ms";

    m_timerTimeOut->stop();
    m_socket.abort();
    m_socket.close();

    emit signalError(2,m_time.elapsed());
}
