/** Classe ActivityFilter
  * @see http://www.qtcentre.org/threads/1464-Detecting-user-inaction
  * @author 2006 Jacob Rideout
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  *
  * Permet de mettre en place la "présentation démo" sur la page d'accueil d'une application
  * l'idée est de détecter l'inactivité de l'utilisateur et de la gérer ...
  *
  * Utilisation, dans votre main.cpp (a étant la abuleduapplication)
  *
  * ActivityFilter *ef;
  * ef = new ActivityFilter(&a);
  * a.installEventFilter(ef);
  * QObject::connect(ef, SIGNAL( userInactive() ),
  *                  w,  SLOT( slotDemo() ));
  *
  * Et dans la classe "w" un slot
  * public slots:
  *   void slotDemo();
  */

#include "activityfilter.h"

ActivityFilter::ActivityFilter(QObject *parent) : QObject(parent), m_interval(3000)
{
    m_isEnable = true;
    m_timer = new QTimer(this);
    m_timer->start(m_interval);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void ActivityFilter::setRunning(bool trueFalse)
{
    m_isEnable = trueFalse;
    if(!trueFalse)
    {
        m_timer->stop();
    }
//    else
//    {
//        m_timer->start(m_interval);
//    }
}

bool ActivityFilter::eventFilter(QObject *obj, QEvent *event)
{
    // this is needed or else the timer is reset by its own timeout signal event before the signal is sent
    if(event->type() == QEvent::MouseMove) {
        emit userActive();
    }

    if (m_isEnable && event->type() != QEvent::Timer) {
        m_timer->start(m_interval);
    }
    // just a wrapper, we don't want to override any events
    return QObject::eventFilter(obj, event);
}

void ActivityFilter::timeout()
{
    if(m_timer->isActive())
    {
        emit userInactive();
    }
}
