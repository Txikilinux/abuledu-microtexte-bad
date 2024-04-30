/** Classe ActivityFilter
  * @see http://www.qtcentre.org/threads/1464-Detecting-user-inaction
  * @author 2006 Jacob Rideout
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  *
  * Permet de mettre en place la "présentation démo" sur la page d'accueil d'une application
  * l'idée est de détecter l'inactivité de l'utilisateur et de la gérer ...
  */


#ifndef ACTIVITYFILTER_H
#define ACTIVITYFILTER_H

#include <QObject>
#include <QtGui>
#include <QtCore>

class ActivityFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int interval READ interval WRITE setInterval)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning)

public:
    ActivityFilter(QObject *parent = 0);
    int interval () const { return m_interval; }
    void setInterval ( int msec ) { m_interval = msec; }
    bool isRunning() {return m_timer->isActive();}
    void setRunning(bool trueFalse);

signals:
    void userInactive();
    void userActive();

public slots:
    void timeout();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QTimer *m_timer;
    int     m_interval;
    bool    m_isEnable;
};

#endif // ACTIVITYFILTER_H
